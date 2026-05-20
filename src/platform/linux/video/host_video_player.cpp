#include "platform/host_video_player.h"

#include "platform/host_renderer.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <thread>

#ifdef SSX3_HAVE_FFMPEG
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}
#endif

#ifdef SSX3_HAVE_SDL
#include <SDL.h>
#endif

namespace host {

#ifdef SSX3_HAVE_FFMPEG
struct VideoDecoderImpl {
    AVFormatContext* format = nullptr;
    int stream_index = -1;
    const AVCodec* codec = nullptr;
    AVCodecContext* context = nullptr;
    AVFrame* frame = nullptr;
    AVPacket* packet = nullptr;
    SwsContext* sws = nullptr;
    int sws_src_format = -1;
    int sws_src_range = -1;
    int width = 0;
    int height = 0;

    const AVCodec* audio_codec = nullptr;
    AVCodecContext* audio_context = nullptr;
    AVFrame* audio_frame = nullptr;
    SwrContext* swr = nullptr;
    std::vector<uint8_t> pcm_stereo;
};

namespace {

bool init_decoder(VideoDecoderImpl& state, AVCodecID codec_id, int width, int height) {
    state.codec = avcodec_find_decoder(codec_id);
    if (!state.codec) {
        return false;
    }
    state.context = avcodec_alloc_context3(state.codec);
    if (!state.context) {
        return false;
    }
    if (width > 0 && height > 0) {
        state.context->width = width;
        state.context->height = height;
    }
    if (avcodec_open2(state.context, state.codec, nullptr) < 0) {
        return false;
    }
    state.frame = av_frame_alloc();
    state.packet = av_packet_alloc();
    return state.frame && state.packet;
}

void shutdown_decoder(VideoDecoderImpl& state) {
    if (state.swr) {
        swr_free(&state.swr);
    }
    if (state.audio_frame) {
        av_frame_free(&state.audio_frame);
    }
    if (state.audio_context) {
        avcodec_free_context(&state.audio_context);
    }
    if (state.sws) {
        sws_freeContext(state.sws);
        state.sws = nullptr;
    }
    state.sws_src_format = -1;
    state.sws_src_range = -1;
    if (state.packet) {
        av_packet_free(&state.packet);
    }
    if (state.frame) {
        av_frame_free(&state.frame);
    }
    if (state.context) {
        avcodec_free_context(&state.context);
    }
    if (state.format) {
        avformat_close_input(&state.format);
        state.format = nullptr;
    }
    state.codec = nullptr;
    state.audio_codec = nullptr;
    state.stream_index = -1;
    state.pcm_stereo.clear();
}

AVCodecID ea_audio_codec_id(const EaAudioConfig& config) {
    switch (config.codec_id) {
    case 1:
        return AV_CODEC_ID_ADPCM_EA_R1;
    case 3:
        return AV_CODEC_ID_ADPCM_EA_R3;
    case 7:
        return AV_CODEC_ID_ADPCM_EA;
    case 2:
    default:
        return AV_CODEC_ID_ADPCM_EA_R2;
    }
}

bool init_audio_decoder(VideoDecoderImpl& state, const EaAudioConfig& config) {
    state.audio_codec = avcodec_find_decoder(ea_audio_codec_id(config));
    if (!state.audio_codec) {
        return false;
    }
    state.audio_context = avcodec_alloc_context3(state.audio_codec);
    if (!state.audio_context) {
        return false;
    }
    state.audio_context->sample_rate = config.sample_rate;
    av_channel_layout_default(&state.audio_context->ch_layout, config.channels);
    state.audio_context->bits_per_coded_sample = 4;
    if (avcodec_open2(state.audio_context, state.audio_codec, nullptr) < 0) {
        return false;
    }
    state.audio_frame = av_frame_alloc();
    return state.audio_frame != nullptr;
}

bool ensure_swr(VideoDecoderImpl& state) {
    if (state.swr) {
        return true;
    }
    AVChannelLayout out_layout = AV_CHANNEL_LAYOUT_STEREO;
    const int ret = swr_alloc_set_opts2(&state.swr,
                                        &out_layout,
                                        AV_SAMPLE_FMT_S16,
                                        state.audio_context->sample_rate,
                                        &state.audio_context->ch_layout,
                                        state.audio_context->sample_fmt,
                                        state.audio_context->sample_rate,
                                        0,
                                        nullptr);
    if (ret < 0 || swr_init(state.swr) < 0) {
        return false;
    }
    return true;
}

bool decode_audio_packet(VideoDecoderImpl& state, const uint8_t* data, int size) {
    if (!state.audio_context || !state.audio_frame) {
        return false;
    }
    av_packet_unref(state.packet);
    if (av_new_packet(state.packet, size) < 0) {
        return false;
    }
    std::memcpy(state.packet->data, data, static_cast<size_t>(size));
    if (avcodec_send_packet(state.audio_context, state.packet) < 0) {
        return false;
    }
    if (avcodec_receive_frame(state.audio_context, state.audio_frame) < 0) {
        return false;
    }
    if (!ensure_swr(state)) {
        return false;
    }

    const int out_samples = swr_get_out_samples(state.swr, state.audio_frame->nb_samples);
    if (out_samples <= 0) {
        return false;
    }
    state.pcm_stereo.resize(static_cast<size_t>(out_samples * 2 * sizeof(int16_t)));
    uint8_t* out_data = state.pcm_stereo.data();
    const int converted =
        swr_convert(state.swr, &out_data, out_samples, const_cast<const uint8_t**>(state.audio_frame->data),
                    state.audio_frame->nb_samples);
    if (converted <= 0) {
        return false;
    }
    state.pcm_stereo.resize(static_cast<size_t>(converted * 2 * sizeof(int16_t)));
    return true;
}

bool frame_to_rgba(VideoDecoderImpl& state, VideoFrame& out) {
    state.width = state.frame->width;
    state.height = state.frame->height;
    const int src_format = state.frame->format;
    const int src_range = (state.frame->color_range == AVCOL_RANGE_JPEG) ? 1 : 0;
    if (!state.sws || state.sws_src_format != src_format || state.sws_src_range != src_range) {
        if (state.sws) {
            sws_freeContext(state.sws);
        }
        state.sws = sws_getContext(state.width,
                                   state.height,
                                   static_cast<AVPixelFormat>(src_format),
                                   state.width,
                                   state.height,
                                   AV_PIX_FMT_BGRA,
                                   SWS_BILINEAR,
                                   nullptr,
                                   nullptr,
                                   nullptr);
        state.sws_src_format = src_format;
        state.sws_src_range = src_range;
        if (!state.sws) {
            return false;
        }
        const int* coeffs = sws_getCoefficients(SWS_CS_ITU601);
        sws_setColorspaceDetails(state.sws, coeffs, src_range, coeffs, 1, 0, 1 << 16, 1 << 16);
    }

    out.width = state.width;
    out.height = state.height;
    out.rgba.resize(static_cast<size_t>(state.width * state.height * 4));
    uint8_t* dst_data[4] = {out.rgba.data(), nullptr, nullptr, nullptr};
    int dst_linesize[4] = {state.width * 4, 0, 0, 0};
    sws_scale(state.sws,
              state.frame->data,
              state.frame->linesize,
              0,
              state.height,
              dst_data,
              dst_linesize);
    return true;
}

bool decode_packet(VideoDecoderImpl& state, const uint8_t* data, int size, VideoFrame& out) {
    av_packet_unref(state.packet);
    if (av_new_packet(state.packet, size) < 0) {
        return false;
    }
    std::memcpy(state.packet->data, data, static_cast<size_t>(size));

    if (avcodec_send_packet(state.context, state.packet) < 0) {
        return false;
    }

    const int result = avcodec_receive_frame(state.context, state.frame);
    if (result == AVERROR(EAGAIN) || result == AVERROR_EOF) {
        return false;
    }
    if (result < 0) {
        return false;
    }
    return frame_to_rgba(state, out);
}

bool open_bink_format(VideoDecoderImpl& state, const std::string& path, double& frame_rate) {
    if (avformat_open_input(&state.format, path.c_str(), nullptr, nullptr) < 0) {
        return false;
    }
    if (avformat_find_stream_info(state.format, nullptr) < 0) {
        return false;
    }

    state.stream_index = av_find_best_stream(state.format, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (state.stream_index < 0) {
        return false;
    }

    AVStream* stream = state.format->streams[state.stream_index];
    state.codec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (!state.codec) {
        return false;
    }

    state.context = avcodec_alloc_context3(state.codec);
    if (!state.context) {
        return false;
    }
    if (avcodec_parameters_to_context(state.context, stream->codecpar) < 0) {
        return false;
    }
    if (avcodec_open2(state.context, state.codec, nullptr) < 0) {
        return false;
    }

    state.frame = av_frame_alloc();
    state.packet = av_packet_alloc();
    if (!state.frame || !state.packet) {
        return false;
    }

    if (stream->avg_frame_rate.num > 0 && stream->avg_frame_rate.den > 0) {
        frame_rate = av_q2d(stream->avg_frame_rate);
    } else if (stream->r_frame_rate.num > 0 && stream->r_frame_rate.den > 0) {
        frame_rate = av_q2d(stream->r_frame_rate);
    } else {
        frame_rate = 30.0;
    }
    return true;
}

bool next_bink_frame(VideoDecoderImpl& state, VideoFrame& out) {
    while (av_read_frame(state.format, state.packet) >= 0) {
        if (state.packet->stream_index != state.stream_index) {
            av_packet_unref(state.packet);
            continue;
        }
        if (avcodec_send_packet(state.context, state.packet) < 0) {
            av_packet_unref(state.packet);
            continue;
        }
        av_packet_unref(state.packet);

        const int result = avcodec_receive_frame(state.context, state.frame);
        if (result == AVERROR(EAGAIN)) {
            continue;
        }
        if (result < 0) {
            return false;
        }
        return frame_to_rgba(state, out);
    }
    return false;
}

} // namespace
#endif

bool VideoPlayer::open(const std::string& path) {
    close();
    std::vector<uint8_t> file;
    if (!read_file_bytes(path, file)) {
        std::cerr << "[video]   missing or unreadable: " << path << '\n';
        return false;
    }
    if (file.empty()) {
        std::cerr << "[video]   empty file (need full disc rip): " << path << '\n';
        return false;
    }

    info_.path = path;
    info_.kind = detect_video_kind(path, file);

    switch (info_.kind) {
    case EaVideoKind::MpcMpeg2:
        return open_mpc(path, file);
    case EaVideoKind::EaVp6:
        return open_vp6(path, file);
    case EaVideoKind::Bink:
        return open_bink(path);
    default:
        std::cerr << "[video]   unknown format: " << path << '\n';
        return false;
    }
}

void VideoPlayer::close() {
#ifdef SSX3_HAVE_FFMPEG
    if (decoder_) {
        shutdown_decoder(*static_cast<VideoDecoderImpl*>(decoder_));
        delete static_cast<VideoDecoderImpl*>(decoder_);
        decoder_ = nullptr;
    }
#endif
    packets_.clear();
    timeline_.clear();
    packet_index_ = 0;
    open_ = false;
}

bool VideoPlayer::open_mpc(const std::string& path, const std::vector<uint8_t>& file) {
#ifdef SSX3_HAVE_FFMPEG
    audio_config_ = EaAudioConfig{};
    timeline_.clear();
    if (!demux_ea_media_timeline(file, timeline_, audio_config_)) {
        std::cerr << "[video]   MPC demux failed: " << path << '\n';
        return false;
    }

    bool has_video = false;
    bool has_audio = false;
    for (const auto& entry : timeline_) {
        has_video = has_video || entry.kind == EaMediaKind::Video;
        has_audio = has_audio || entry.kind == EaMediaKind::Audio;
    }
    if (!has_video) {
        std::cerr << "[video]   MPC has no MPCh (MPEG-2) chunks: " << path << '\n';
        return false;
    }

    decoder_ = new VideoDecoderImpl();
    auto* dec = static_cast<VideoDecoderImpl*>(decoder_);
    if (!init_decoder(*dec, AV_CODEC_ID_MPEG2VIDEO, 0, 0)) {
        std::cerr << "[video]   FFmpeg MPEG-2 decoder unavailable\n";
        close();
        return false;
    }
    if (has_audio && !init_audio_decoder(*dec, audio_config_)) {
        std::cerr << "[video]   FFmpeg EA ADPCM audio decoder unavailable\n";
        close();
        return false;
    }

    info_.frame_rate = 30000.0 / 1001.0;
    open_ = true;
    packet_index_ = 0;
    std::cout << "[video]   opened MPC/MPEG-2: " << path << " (" << timeline_.size()
              << " interleaved chunks, audio " << audio_config_.sample_rate << " Hz x"
              << audio_config_.channels << ")\n";
    return true;
#else
    (void)path;
    (void)file;
    std::cerr << "[video]   build without FFmpeg (install libavcodec-dev)\n";
    return false;
#endif
}

bool VideoPlayer::open_vp6(const std::string& path, const std::vector<uint8_t>& file) {
#ifdef SSX3_HAVE_FFMPEG
    std::vector<EaChunk> chunks;
    if (!parse_ea_chunks(file, chunks)) {
        std::cerr << "[video]   VP6 parse failed: " << path << '\n';
        return false;
    }

    EaVp6Header header{};
    bool have_header = false;
    for (const auto& chunk : chunks) {
        if (chunk.fourcc == fourcc_from_cstr("MVhd")) {
            have_header = parse_vp6_header(chunk, header);
        }
    }
    if (!have_header) {
        std::cerr << "[video]   VP6 missing MVhd header: " << path << '\n';
        return false;
    }

    packets_.clear();
    for (const auto& chunk : chunks) {
        if (chunk.fourcc == fourcc_from_cstr("MV0K") || chunk.fourcc == fourcc_from_cstr("MV0F")) {
            packets_.push_back(chunk.payload);
        }
    }
    if (packets_.empty()) {
        std::cerr << "[video]   VP6 has no frame chunks: " << path << '\n';
        return false;
    }

    decoder_ = new VideoDecoderImpl();
    if (!init_decoder(*static_cast<VideoDecoderImpl*>(decoder_), AV_CODEC_ID_VP6F, static_cast<int>(header.width),
                      static_cast<int>(header.height))) {
        std::cerr << "[video]   FFmpeg VP6 decoder unavailable\n";
        close();
        return false;
    }

    info_.width = static_cast<int>(header.width);
    info_.height = static_cast<int>(header.height);
    if (header.rate_num > 0 && header.rate_den > 0) {
        info_.frame_rate = static_cast<double>(header.rate_num) / static_cast<double>(header.rate_den);
    } else {
        info_.frame_rate = 30.0;
    }
    open_ = true;
    packet_index_ = 0;
    std::cout << "[video]   opened EA/VP6: " << path << " (" << info_.width << "x" << info_.height
              << ", " << packets_.size() << " frames)\n";
    return true;
#else
    (void)path;
    (void)file;
    std::cerr << "[video]   build without FFmpeg (install libavcodec-dev)\n";
    return false;
#endif
}

bool VideoPlayer::open_bink(const std::string& path) {
#ifdef SSX3_HAVE_FFMPEG
    decoder_ = new VideoDecoderImpl();
    if (!open_bink_format(*static_cast<VideoDecoderImpl*>(decoder_), path, info_.frame_rate)) {
        std::cerr << "[video]   Bink demux/decode failed: " << path << '\n';
        close();
        return false;
    }

    open_ = true;
    std::cout << "[video]   opened Bink: " << path << '\n';
    return true;
#else
    (void)path;
    std::cerr << "[video]   build without FFmpeg (install libavcodec-dev)\n";
    return false;
#endif
}

bool VideoPlayer::drain_audio_pcm(std::vector<uint8_t>& pcm_s16_stereo) {
#ifdef SSX3_HAVE_FFMPEG
    if (!decoder_) {
        return false;
    }
    auto* dec = static_cast<VideoDecoderImpl*>(decoder_);
    if (dec->pcm_stereo.empty()) {
        return false;
    }
    pcm_s16_stereo.swap(dec->pcm_stereo);
    return true;
#else
    (void)pcm_s16_stereo;
    return false;
#endif
}

bool VideoPlayer::next_frame(VideoFrame& frame) {
#ifdef SSX3_HAVE_FFMPEG
    if (!open_ || !decoder_) {
        return false;
    }
    auto* dec = static_cast<VideoDecoderImpl*>(decoder_);

    if (info_.kind == EaVideoKind::Bink) {
        if (next_bink_frame(*dec, frame)) {
            if (info_.width == 0) {
                info_.width = frame.width;
                info_.height = frame.height;
            }
            return true;
        }
        return false;
    }

    if (!timeline_.empty()) {
        while (packet_index_ < timeline_.size()) {
            const auto& entry = timeline_[packet_index_++];
            if (entry.kind == EaMediaKind::Audio) {
                if (dec->audio_context) {
                    decode_audio_packet(*dec, entry.data.data(), static_cast<int>(entry.data.size()));
                }
                continue;
            }
            if (decode_packet(*dec, entry.data.data(), static_cast<int>(entry.data.size()), frame)) {
                if (info_.width == 0) {
                    info_.width = frame.width;
                    info_.height = frame.height;
                }
                return true;
            }
        }
        return false;
    }

    while (packet_index_ < packets_.size()) {
        const auto& packet = packets_[packet_index_++];
        if (decode_packet(*dec, packet.data(), static_cast<int>(packet.size()), frame)) {
            if (info_.width == 0) {
                info_.width = frame.width;
                info_.height = frame.height;
            }
            return true;
        }
    }
    return false;
#else
    (void)frame;
    return false;
#endif
}

#ifdef SSX3_HAVE_SDL
namespace {

SDL_AudioDeviceID g_audio_device = 0;
int g_audio_rate = 0;

void close_boot_audio() {
    if (g_audio_device != 0) {
        SDL_CloseAudioDevice(g_audio_device);
        g_audio_device = 0;
        g_audio_rate = 0;
    }
}

bool open_boot_audio(int sample_rate) {
    if (g_audio_device != 0 && g_audio_rate == sample_rate) {
        SDL_ClearQueuedAudio(g_audio_device);
        return true;
    }
    close_boot_audio();

    SDL_AudioSpec want{};
    SDL_AudioSpec have{};
    want.freq = sample_rate;
    want.format = AUDIO_S16;
    want.channels = 2;
    want.samples = 2048;
    g_audio_device = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    if (g_audio_device == 0) {
        std::cerr << "[video]   SDL audio failed: " << SDL_GetError() << '\n';
        return false;
    }
    g_audio_rate = have.freq;
    SDL_PauseAudioDevice(g_audio_device, 0);
    return true;
}

void queue_boot_audio(const std::vector<uint8_t>& pcm) {
    if (g_audio_device != 0 && !pcm.empty()) {
        SDL_QueueAudio(g_audio_device, pcm.data(), static_cast<Uint32>(pcm.size()));
    }
}

void drain_player_audio(VideoPlayer& player) {
    std::vector<uint8_t> pcm;
    while (player.drain_audio_pcm(pcm)) {
        queue_boot_audio(pcm);
    }
}

} // namespace
#endif

static std::string pick_intro_movie(const std::string& disc_root) {
    const std::vector<std::string> candidates = {
        disc_root + "/data/movies/Intro_DJ.mpc",
        disc_root + "/data/movies/intro_dj.mpc",
        disc_root + "/data/movies/Intro.mpc",
        disc_root + "/data/movies/intro.mpc",
    };
    for (const auto& path : candidates) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (file && file.tellg() > 0) {
            return path;
        }
    }
    return {};
}

bool play_boot_movies(const std::string& disc_root, Renderer* display) {
    std::vector<std::string> movies = {disc_root + "/data/movies/eabig.mpc"};
    const std::string intro = pick_intro_movie(disc_root);
    if (!intro.empty()) {
        movies.push_back(intro);
    }

#ifndef SSX3_HAVE_SDL
    (void)display;
    std::cerr << "[video]   SDL required for audio playback\n";
    return false;
#endif

#ifndef SSX3_HAVE_FFMPEG
    (void)display;
    std::cerr << "[video]   FFmpeg required for decode (see docs/VIDEO_CODECS.md)\n";
    return false;
#endif

    if (!display) {
        std::cerr << "[video]   display/renderer required for boot movies\n";
        return false;
    }

#ifdef SSX3_HAVE_SDL
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        std::cerr << "[video]   SDL audio init failed: " << SDL_GetError() << '\n';
        return false;
    }

    bool any_played = false;
    for (const auto& movie : movies) {
        if (display->should_close()) {
            break;
        }

        VideoPlayer player;
        if (!player.open(movie)) {
            continue;
        }

        any_played = true;
        const double frame_delay = 1.0 / player.info().frame_rate;
        VideoFrame frame;

        if (player.audio_config().sample_rate > 0) {
            open_boot_audio(player.audio_config().sample_rate);
        }

        bool skip_movie = false;
        while (player.next_frame(frame)) {
            drain_player_audio(player);
            if (!display->present_video_frame(frame)) {
                skip_movie = true;
                break;
            }
            if (display->consume_skip_video()) {
                std::cout << "[video]   skipped: " << movie << '\n';
                skip_movie = true;
                break;
            }
            if (display->should_close()) {
                skip_movie = true;
                break;
            }
            std::this_thread::sleep_for(
                std::chrono::duration<double>(frame_delay > 0.0 ? frame_delay : 1.0 / 30.0));
        }
        player.close();
        if (skip_movie && display->should_close()) {
            break;
        }
    }

    close_boot_audio();

    if (!any_played) {
        std::cerr << "[video]   no boot movies played (check disc/data/movies/*.mpc)\n";
        return false;
    }
    return true;
#else
    (void)disc_root;
    (void)display;
    return false;
#endif
}

} // namespace host
