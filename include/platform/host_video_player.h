#ifndef PLATFORM_HOST_VIDEO_PLAYER_H
#define PLATFORM_HOST_VIDEO_PLAYER_H

#include "platform/host_ea_container.h"

#include <cstdint>
#include <string>
#include <vector>

namespace host {

struct VideoFrame {
    int width = 0;
    int height = 0;
    std::vector<uint8_t> rgba;
};

struct VideoPlaybackInfo {
    std::string path;
    EaVideoKind kind = EaVideoKind::Unknown;
    int width = 0;
    int height = 0;
    double frame_rate = 30.0;
};

class VideoPlayer {
public:
    bool open(const std::string& path);
    void close();

    bool is_open() const { return open_; }
    const VideoPlaybackInfo& info() const { return info_; }

    bool next_frame(VideoFrame& frame);

    const EaAudioConfig& audio_config() const { return audio_config_; }
    bool drain_audio_pcm(std::vector<uint8_t>& pcm_s16_stereo);

private:
    bool open_mpc(const std::string& path, const std::vector<uint8_t>& file);
    bool open_vp6(const std::string& path, const std::vector<uint8_t>& file);
    bool open_bink(const std::string& path);

    bool open_;
    VideoPlaybackInfo info_{};
    std::vector<std::vector<uint8_t>> packets_;
    std::vector<EaMediaPacket> timeline_;
    size_t packet_index_ = 0;
    EaAudioConfig audio_config_{};

    void* decoder_ = nullptr;
};

class Renderer;

bool play_boot_movies(const std::string& disc_root, Renderer* display);

} // namespace host

#endif /* PLATFORM_HOST_VIDEO_PLAYER_H */
