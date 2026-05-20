#include "platform/host_ea_container.h"

#include <cstring>
#include <fstream>

namespace host {

uint32_t fourcc_from_cstr(const char* text) {
    uint32_t value = 0;
    std::memcpy(&value, text, 4);
    return value;
}

bool read_file_bytes(const std::string& path, std::vector<uint8_t>& out) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        return false;
    }
    const auto size = file.tellg();
    if (size < 0) {
        out.clear();
        return false;
    }
    if (size == 0) {
        out.clear();
        return true;
    }
    out.resize(static_cast<size_t>(size));
    file.seekg(0);
    file.read(reinterpret_cast<char*>(out.data()), size);
    return file.good();
}

bool parse_ea_chunks(const std::vector<uint8_t>& file, std::vector<EaChunk>& chunks) {
    chunks.clear();
    size_t offset = 0;
    while (offset + 8 <= file.size()) {
        uint32_t fourcc = 0;
        uint32_t size = 0;
        std::memcpy(&fourcc, file.data() + offset, 4);
        std::memcpy(&size, file.data() + offset + 4, 4);
        if (size < 8) {
            return false;
        }
        if (offset + size > file.size()) {
            return false;
        }
        EaChunk chunk{};
        chunk.fourcc = fourcc;
        chunk.payload.assign(file.begin() + static_cast<ptrdiff_t>(offset + 8),
                             file.begin() + static_cast<ptrdiff_t>(offset + size));
        chunks.push_back(std::move(chunk));
        offset += size;
    }
    return !chunks.empty();
}

bool parse_vp6_header(const EaChunk& chunk, EaVp6Header& header) {
    if (chunk.fourcc != fourcc_from_cstr("MVhd") || chunk.payload.size() < 24) {
        return false;
    }
    const uint8_t* data = chunk.payload.data();
    header.width = data[4] | (data[5] << 8);
    header.height = data[6] | (data[7] << 8);
    std::memcpy(&header.frame_count, data + 8, 4);
    std::memcpy(&header.largest_frame, data + 12, 4);
    std::memcpy(&header.rate_den, data + 16, 4);
    std::memcpy(&header.rate_num, data + 20, 4);
    return header.width > 0 && header.height > 0;
}

EaVideoKind detect_video_kind(const std::string& path, const std::vector<uint8_t>& file) {
    if (file.size() >= 3 && file[0] == 'B' && file[1] == 'I' && file[2] == 'K') {
        return EaVideoKind::Bink;
    }
    if (file.size() >= 4) {
        uint32_t magic = 0;
        std::memcpy(&magic, file.data(), 4);
        if (magic == fourcc_from_cstr("MVhd")) {
            return EaVideoKind::EaVp6;
        }
    }

    std::vector<EaChunk> chunks;
    if (parse_ea_chunks(file, chunks)) {
        for (const auto& chunk : chunks) {
            if (chunk.fourcc == fourcc_from_cstr("MPCh")) {
                return EaVideoKind::MpcMpeg2;
            }
            if (chunk.fourcc == fourcc_from_cstr("MVhd")) {
                return EaVideoKind::EaVp6;
            }
        }
    }

    if (path.size() >= 4) {
        const std::string ext = path.substr(path.size() - 4);
        if (ext == ".mpc") {
            return EaVideoKind::MpcMpeg2;
        }
        if (ext == ".vp6") {
            return EaVideoKind::EaVp6;
        }
        if (ext == ".bik") {
            return EaVideoKind::Bink;
        }
    }
    return EaVideoKind::Unknown;
}

namespace {

uint32_t read_arbitrary(const uint8_t* data, size_t size, size_t& pos) {
    if (pos >= size) {
        return 0;
    }
    const uint8_t length_byte = data[pos++];
    uint32_t value = 0;
    for (uint8_t i = 0; i < length_byte && pos < size; ++i) {
        value = (value << 8) | data[pos++];
    }
    return value;
}

} // namespace

bool parse_ea_audio_header(const EaChunk& schl_chunk, EaAudioConfig& config) {
    if (schl_chunk.fourcc != fourcc_from_cstr("SCHl") || schl_chunk.payload.size() < 12) {
        return false;
    }

    const uint8_t* data = schl_chunk.payload.data();
    const size_t size = schl_chunk.payload.size();
    if (std::memcmp(data, "GSTR", 4) != 0 && std::memcmp(data, "PT", 2) != 0) {
        return false;
    }

    size_t pos = 8; // skip GSTR/PT header tag + version/platform
    if (std::memcmp(data, "PT", 2) == 0) {
        pos = 4;
    }

    int compression_type = -1;
    int revision = -1;
    int revision2 = -1;
    bool in_header = true;

    while (pos < size && in_header) {
        const uint8_t tag = data[pos++];
        if (tag == 0xFD) {
            bool in_subheader = true;
            while (pos < size && in_subheader) {
                const uint8_t subtag = data[pos++];
                switch (subtag) {
                case 0x80:
                    revision = static_cast<int>(read_arbitrary(data, size, pos));
                    break;
                case 0x82:
                    config.channels = static_cast<int>(read_arbitrary(data, size, pos));
                    break;
                case 0x83:
                    compression_type = static_cast<int>(read_arbitrary(data, size, pos));
                    break;
                case 0x84:
                    config.sample_rate = static_cast<int>(read_arbitrary(data, size, pos));
                    break;
                case 0x8A:
                case 0xFF:
                    in_subheader = false;
                    in_header = (subtag != 0xFF);
                    if (subtag == 0x8A) {
                        read_arbitrary(data, size, pos);
                    }
                    break;
                case 0xA0:
                    revision2 = static_cast<int>(read_arbitrary(data, size, pos));
                    break;
                default:
                    read_arbitrary(data, size, pos);
                    break;
                }
            }
            continue;
        }
        if (tag == 0xFF) {
            break;
        }
        if (tag == 0xFC || tag == 0xFE || tag == 0x8A) {
            continue;
        }
        read_arbitrary(data, size, pos);
    }

    // SSX3 MPC (GSTR) omits standard 0x80 revision tags; SCDl payloads use big-endian
    // sample counts, matching FFmpeg's ADPCM_EA_R3.
    if (revision < 0 && compression_type < 0) {
        revision = 3;
    }

    if (compression_type == 7) {
        config.codec_id = 7; // ADPCM_EA
    } else if (revision == 1) {
        config.codec_id = 1; // ADPCM_EA_R1
    } else if (revision == 2) {
        config.codec_id = 2; // ADPCM_EA_R2
    } else if (revision == 3) {
        config.codec_id = 3; // ADPCM_EA_R3
    } else {
        config.codec_id = 2;
    }

    (void)revision2;
    if (config.sample_rate <= 0) {
        config.sample_rate = revision == 3 ? 48000 : 44100;
    }
    if (config.channels <= 0) {
        config.channels = 2;
    }
    return true;
}

bool demux_ea_media_timeline(const std::vector<uint8_t>& file,
                             std::vector<EaMediaPacket>& timeline,
                             EaAudioConfig& audio_config) {
    timeline.clear();
    std::vector<EaChunk> chunks;
    if (!parse_ea_chunks(file, chunks)) {
        return false;
    }

    const uint32_t mpch = fourcc_from_cstr("MPCh");
    const uint32_t scdl = fourcc_from_cstr("SCDl");
    const uint32_t schl = fourcc_from_cstr("SCHl");

    for (const auto& chunk : chunks) {
        if (chunk.fourcc == schl) {
            parse_ea_audio_header(chunk, audio_config);
            continue;
        }
        if (chunk.fourcc == mpch) {
            timeline.push_back({EaMediaKind::Video, chunk.payload});
        } else if (chunk.fourcc == scdl) {
            timeline.push_back({EaMediaKind::Audio, chunk.payload});
        }
    }
    return !timeline.empty();
}

std::vector<EaChunk> filter_chunks(const std::vector<EaChunk>& chunks, uint32_t fourcc) {
    std::vector<EaChunk> filtered;
    for (const auto& chunk : chunks) {
        if (chunk.fourcc == fourcc) {
            filtered.push_back(chunk);
        }
    }
    return filtered;
}

} // namespace host
