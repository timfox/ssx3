#ifndef PLATFORM_HOST_EA_CONTAINER_H
#define PLATFORM_HOST_EA_CONTAINER_H

#include <cstdint>
#include <string>
#include <vector>

namespace host {

struct EaChunk {
    uint32_t fourcc = 0;
    std::vector<uint8_t> payload;
};

struct EaVp6Header {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t frame_count = 0;
    uint32_t largest_frame = 0;
    uint32_t rate_den = 1;
    uint32_t rate_num = 30;
};

enum class EaVideoKind {
    Unknown,
    MpcMpeg2,
    EaVp6,
    Bink,
};

enum class EaMediaKind {
    Video,
    Audio,
};

struct EaAudioConfig {
    int sample_rate = 44100;
    int channels = 2;
    int codec_id = 0; // AVCodecID when FFmpeg enabled; 0 = ADPCM EA R2 default
};

struct EaMediaPacket {
    EaMediaKind kind = EaMediaKind::Video;
    std::vector<uint8_t> data;
};

uint32_t fourcc_from_cstr(const char* text);

bool read_file_bytes(const std::string& path, std::vector<uint8_t>& out);
bool parse_ea_chunks(const std::vector<uint8_t>& file, std::vector<EaChunk>& chunks);
bool parse_vp6_header(const EaChunk& chunk, EaVp6Header& header);

EaVideoKind detect_video_kind(const std::string& path, const std::vector<uint8_t>& file);

std::vector<EaChunk> filter_chunks(const std::vector<EaChunk>& chunks, uint32_t fourcc);

bool parse_ea_audio_header(const EaChunk& schl_chunk, EaAudioConfig& config);
bool demux_ea_media_timeline(const std::vector<uint8_t>& file,
                             std::vector<EaMediaPacket>& timeline,
                             EaAudioConfig& audio_config);

} // namespace host

#endif /* PLATFORM_HOST_EA_CONTAINER_H */
