#include "platform/host_menu_audio.h"

#include "platform/host_disc.h"
#include "platform/host_fe_flow.h"
#include "platform/host_log.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace host {
namespace {

constexpr float kAmbientVolume = 0.44f;
constexpr float kMusicVolume = 0.58f;
constexpr float kStemVolume = 0.48f;
constexpr float kSfxVolume = 0.72f;

struct WavData {
    int sample_rate = 44100;
    int channels = 2;
    std::vector<std::int16_t> pcm;
};

struct AlVoice {
    ALuint buffer = 0;
    ALuint source = 0;
    bool loaded = false;
};

ALCdevice* g_alc_device = nullptr;
ALCcontext* g_alc_context = nullptr;
bool g_audio_open = false;
FEScreen g_screen = FEScreen::Title;
int g_stem_index = 0;

AlVoice g_ambient;
AlVoice g_menu_music;
AlVoice g_music_stem_a;
AlVoice g_music_stem_b;
AlVoice g_music_stem_c;
AlVoice g_sfx_navigate;
AlVoice g_sfx_confirm;
AlVoice g_sfx_back;
AlVoice g_sfx_start;

bool read_u32_le(const std::uint8_t* bytes, std::size_t size, std::size_t& cursor, std::uint32_t& out) {
    if (cursor + 4 > size) {
        return false;
    }
    out = static_cast<std::uint32_t>(bytes[cursor]) |
          (static_cast<std::uint32_t>(bytes[cursor + 1]) << 8) |
          (static_cast<std::uint32_t>(bytes[cursor + 2]) << 16) |
          (static_cast<std::uint32_t>(bytes[cursor + 3]) << 24);
    cursor += 4;
    return true;
}

bool read_u16_le(const std::uint8_t* bytes, std::size_t size, std::size_t& cursor, std::uint16_t& out) {
    if (cursor + 2 > size) {
        return false;
    }
    out = static_cast<std::uint16_t>(bytes[cursor]) |
          (static_cast<std::uint16_t>(bytes[cursor + 1]) << 8);
    cursor += 2;
    return true;
}

bool load_wav_file(const fs::path& path, WavData& out) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    file.seekg(0, std::ios::end);
    const auto file_size = static_cast<std::size_t>(file.tellg());
    if (file_size < 44) {
        return false;
    }
    std::vector<std::uint8_t> bytes(file_size);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(file_size));
    if (static_cast<std::size_t>(file.gcount()) != file_size) {
        return false;
    }
    if (std::memcmp(bytes.data(), "RIFF", 4) != 0) {
        return false;
    }

    std::size_t cursor = 12;
    std::uint16_t audio_format = 0;
    std::uint16_t channels = 0;
    std::uint32_t sample_rate = 0;
    std::uint16_t bits_per_sample = 0;
    std::size_t data_offset = 0;
    std::uint32_t data_size = 0;

    while (cursor + 8 <= file_size) {
        char chunk_id[4]{};
        std::memcpy(chunk_id, bytes.data() + cursor, 4);
        cursor += 4;
        std::uint32_t chunk_size = 0;
        if (!read_u32_le(bytes.data(), file_size, cursor, chunk_size)) {
            return false;
        }
        const std::size_t chunk_data = cursor;
        if (std::memcmp(chunk_id, "fmt ", 4) == 0) {
            if (!read_u16_le(bytes.data(), file_size, cursor, audio_format) ||
                !read_u16_le(bytes.data(), file_size, cursor, channels)) {
                return false;
            }
            if (!read_u32_le(bytes.data(), file_size, cursor, sample_rate)) {
                return false;
            }
            std::uint32_t byte_rate = 0;
            std::uint16_t block_align = 0;
            if (!read_u32_le(bytes.data(), file_size, cursor, byte_rate) ||
                !read_u16_le(bytes.data(), file_size, cursor, block_align) ||
                !read_u16_le(bytes.data(), file_size, cursor, bits_per_sample)) {
                return false;
            }
        } else if (std::memcmp(chunk_id, "data", 4) == 0) {
            data_offset = chunk_data;
            data_size = chunk_size;
        }
        cursor = chunk_data + chunk_size + (chunk_size & 1u);
    }

    if (audio_format != 1 || bits_per_sample != 16 || channels < 1 || channels > 2 || sample_rate == 0 ||
        data_offset == 0 || data_size < 2) {
        return false;
    }

    const std::size_t sample_count = data_size / (bits_per_sample / 8);
    out.sample_rate = static_cast<int>(sample_rate);
    out.channels = static_cast<int>(channels);
    out.pcm.resize(sample_count);
    std::memcpy(out.pcm.data(), bytes.data() + data_offset, data_size);
    return true;
}

ALenum wav_format(const WavData& wav) {
    return wav.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
}

bool al_check(const char* what) {
    const ALenum err = alGetError();
    if (err == AL_NO_ERROR) {
        return true;
    }
    host_log("audio", (std::string(what) + ": OpenAL " + std::to_string(err)).c_str());
    return false;
}

void destroy_voice(AlVoice& voice) {
    if (voice.source != 0) {
        alSourceStop(voice.source);
        alDeleteSources(1, &voice.source);
        voice.source = 0;
    }
    if (voice.buffer != 0) {
        alDeleteBuffers(1, &voice.buffer);
        voice.buffer = 0;
    }
    voice.loaded = false;
}

bool upload_voice(const fs::path& path, AlVoice& voice, float volume, bool loop) {
    WavData wav{};
    if (!load_wav_file(path, wav) || wav.pcm.empty()) {
        return false;
    }

    alGenBuffers(1, &voice.buffer);
    alGenSources(1, &voice.source);
    if (!al_check("gen")) {
        destroy_voice(voice);
        return false;
    }

    const auto* pcm_bytes = reinterpret_cast<const ALvoid*>(wav.pcm.data());
    const ALsizei byte_size = static_cast<ALsizei>(wav.pcm.size() * sizeof(std::int16_t));
    alBufferData(voice.buffer,
                 wav_format(wav),
                 pcm_bytes,
                 byte_size,
                 static_cast<ALsizei>(wav.sample_rate));
    if (!al_check("bufferData")) {
        destroy_voice(voice);
        return false;
    }

    alSourcei(voice.source, AL_BUFFER, static_cast<ALint>(voice.buffer));
    alSourcef(voice.source, AL_GAIN, volume);
    alSourcei(voice.source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    if (!al_check("source")) {
        destroy_voice(voice);
        return false;
    }

    voice.loaded = true;
    return true;
}

void play_loop(AlVoice& voice) {
    if (!voice.loaded) {
        return;
    }
    alSourcePlay(voice.source);
    al_check("play loop");
}

void stop_voice(AlVoice& voice) {
    if (voice.loaded && voice.source != 0) {
        alSourceStop(voice.source);
    }
}

void play_sfx_voice(AlVoice& voice) {
    if (!voice.loaded || !g_audio_open) {
        return;
    }
    alSourceStop(voice.source);
    alSourcei(voice.source, AL_LOOPING, AL_FALSE);
    alSourcef(voice.source, AL_GAIN, kSfxVolume);
    alSourcePlay(voice.source);
    al_check("play sfx");
}

void play_stem_hit() {
    AlVoice* stems[] = {&g_music_stem_a, &g_music_stem_b, &g_music_stem_c};
    AlVoice* voice = nullptr;
    for (int attempt = 0; attempt < 3; ++attempt) {
        voice = stems[g_stem_index % 3];
        g_stem_index = (g_stem_index + 1) % 3;
        if (voice->loaded) {
            break;
        }
        voice = nullptr;
    }
    if (!voice || !voice->loaded || !g_audio_open) {
        return;
    }
    alSourceStop(voice->source);
    alSourcei(voice->source, AL_LOOPING, AL_FALSE);
    alSourcef(voice->source, AL_GAIN, kStemVolume);
    alSourcePlay(voice->source);
    al_check("play stem");
}

void stop_all_voices() {
    const AlVoice* voices[] = {
        &g_ambient,      &g_menu_music,   &g_music_stem_a, &g_music_stem_b, &g_music_stem_c,
        &g_sfx_navigate, &g_sfx_confirm,  &g_sfx_back,     &g_sfx_start,
    };
    for (const AlVoice* voice : voices) {
        if (voice->loaded && voice->source != 0) {
            alSourceStop(voice->source);
        }
    }
}

void destroy_all_voices() {
    destroy_voice(g_ambient);
    destroy_voice(g_menu_music);
    destroy_voice(g_music_stem_a);
    destroy_voice(g_music_stem_b);
    destroy_voice(g_music_stem_c);
    destroy_voice(g_sfx_navigate);
    destroy_voice(g_sfx_confirm);
    destroy_voice(g_sfx_back);
    destroy_voice(g_sfx_start);
}

bool open_openal() {
    g_alc_device = alcOpenDevice(nullptr);
    if (!g_alc_device) {
        host_log("audio", "alcOpenDevice failed");
        return false;
    }

    g_alc_context = alcCreateContext(g_alc_device, nullptr);
    if (!g_alc_context) {
        host_log("audio", "alcCreateContext failed");
        alcCloseDevice(g_alc_device);
        g_alc_device = nullptr;
        return false;
    }

    if (!alcMakeContextCurrent(g_alc_context)) {
        host_log("audio", "alcMakeContextCurrent failed");
        alcDestroyContext(g_alc_context);
        alcCloseDevice(g_alc_device);
        g_alc_context = nullptr;
        g_alc_device = nullptr;
        return false;
    }

    g_audio_open = true;
    return true;
}

void close_openal() {
    if (g_alc_context) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(g_alc_context);
        g_alc_context = nullptr;
    }
    if (g_alc_device) {
        alcCloseDevice(g_alc_device);
        g_alc_device = nullptr;
    }
    g_audio_open = false;
}

void apply_screen_mix() {
    if (!g_audio_open) {
        return;
    }

    stop_voice(g_menu_music);
    stop_voice(g_music_stem_a);
    stop_voice(g_music_stem_b);
    stop_voice(g_music_stem_c);

    if (g_screen == FEScreen::Title) {
        stop_voice(g_ambient);
        if (g_ambient.loaded) {
            alSourcef(g_ambient.source, AL_GAIN, kAmbientVolume);
            play_loop(g_ambient);
        }
        return;
    }

    stop_voice(g_ambient);
    if (g_menu_music.loaded) {
        alSourcef(g_menu_music.source, AL_GAIN, kMusicVolume);
        play_loop(g_menu_music);
    }
}

constexpr const char kMenuAudioDir[] = "assets/host/audio";

} // namespace

const char* host_menu_audio_dir() {
    return kMenuAudioDir;
}

bool host_menu_audio_ready() {
    const fs::path dir = kMenuAudioDir;
    return fs::is_regular_file(dir / "title_ambient.wav") &&
           fs::is_regular_file(dir / "menu_music.wav");
}

bool host_menu_audio_ready(const std::string& asset_dir) {
    (void)asset_dir;
    return host_menu_audio_ready();
}

bool host_menu_audio_ensure() {
    if (host_menu_audio_ready()) {
        return true;
    }

    const fs::path audio_big = disc().resolve("data/audio/audio.big");
    if (!fs::is_regular_file(audio_big)) {
        host_log("audio", "missing disc data/audio/audio.big — cannot decode menu banks");
        return false;
    }

    const fs::path disc_root = disc().root();
    const std::string cmd =
        "python3 scripts/extract_menu_audio.py --disc \"" + disc_root.string() + "\"";
    host_log("audio", "decoding menu banks from audio.big …");
    std::cout << "[audio]   " << cmd << '\n';
    const int rc = std::system(cmd.c_str());
    if (rc != 0) {
        std::cerr << "[audio]   extract failed (exit " << rc << ")\n";
        return false;
    }

    if (!host_menu_audio_ready()) {
        host_log("audio", "decode finished but menu WAVs still missing (need menu_music.wav)");
        return false;
    }
    host_log("audio", ("menu audio ready under " + std::string(kMenuAudioDir)).c_str());
    return true;
}

bool host_menu_audio_ensure(const std::string& asset_dir) {
    (void)asset_dir;
    return host_menu_audio_ensure();
}

void host_menu_audio_start() {
    host_menu_audio_stop();

    const fs::path audio_dir = fs::path(kMenuAudioDir);
    if (!host_menu_audio_ready()) {
        host_log("audio", "menu audio assets missing — run scripts/extract_menu_audio.py --disc disc");
        return;
    }

    if (!open_openal()) {
        return;
    }

    const struct {
        const char* name;
        AlVoice* voice;
        float volume;
        bool loop;
    } clips[] = {
        {"title_ambient.wav", &g_ambient, kAmbientVolume, true},
        {"menu_music.wav", &g_menu_music, kMusicVolume, true},
        {"menu_stem_a.wav", &g_music_stem_a, kStemVolume, false},
        {"menu_stem_b.wav", &g_music_stem_b, kStemVolume, false},
        {"menu_stem_c.wav", &g_music_stem_c, kStemVolume, false},
    };

    int loaded_loops = 0;
    int loaded_stems = 0;
    for (const auto& entry : clips) {
        if (upload_voice(audio_dir / entry.name, *entry.voice, entry.volume, entry.loop)) {
            if (entry.loop) {
                ++loaded_loops;
            } else {
                ++loaded_stems;
            }
        } else {
            host_log("audio", (std::string("missing ") + entry.name).c_str());
        }
    }

    const struct {
        const char* name;
        AlVoice* voice;
    } sfx[] = {
        {"sfx_navigate.wav", &g_sfx_navigate},
        {"sfx_confirm.wav", &g_sfx_confirm},
        {"sfx_back.wav", &g_sfx_back},
        {"sfx_start.wav", &g_sfx_start},
    };

    int loaded_sfx = 0;
    for (const auto& entry : sfx) {
        if (upload_voice(audio_dir / entry.name, *entry.voice, kSfxVolume, false)) {
            ++loaded_sfx;
        } else {
            host_log("audio", (std::string("missing sfx ") + entry.name).c_str());
        }
    }

    if (loaded_loops == 0) {
        host_log("audio", "no menu loops loaded");
        destroy_all_voices();
        close_openal();
        return;
    }

    g_screen = FEScreen::Title;
    g_stem_index = 0;
    apply_screen_mix();

    host_log("audio", "OpenAL menu audio started");
    std::cout << "[audio]   OpenAL loops=" << loaded_loops << " stems=" << loaded_stems
              << " sfx=" << loaded_sfx << '\n';
}

void host_menu_audio_start(const std::string& asset_dir) {
    (void)asset_dir;
    host_menu_audio_start();
}

void host_menu_audio_on_screen(FEScreen screen) {
    if (!g_audio_open) {
        return;
    }
    if (g_screen == screen) {
        return;
    }
    g_screen = screen;
    apply_screen_mix();
}

void host_menu_audio_stop() {
    stop_all_voices();
    destroy_all_voices();
    close_openal();
}

void host_menu_audio_play_sfx(MenuSfx sfx) {
    if (!g_audio_open) {
        return;
    }
    switch (sfx) {
    case MenuSfx::Navigate:
        play_sfx_voice(g_sfx_navigate);
        if (g_screen != FEScreen::Title) {
            play_stem_hit();
        }
        break;
    case MenuSfx::Confirm:
        play_sfx_voice(g_sfx_confirm);
        break;
    case MenuSfx::Back:
        play_sfx_voice(g_sfx_back);
        break;
    case MenuSfx::Start:
        play_sfx_voice(g_sfx_start);
        break;
    }
}

} // namespace host
