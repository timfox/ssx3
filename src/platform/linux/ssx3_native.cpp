#include "platform/host_boot.h"
#include "platform/host_disc.h"
#include "platform/host_fe_font.h"
#include "platform/host_gameplay.h"
#include "platform/host_gfx.h"
#include "platform/host_resolution.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace host {
bool run_smoke_tests();
}

namespace {

struct BootInfo {
    fs::path systemCnfPath;
    std::string bootLine;
    std::string bootElf;
};

std::string trim(const std::string& input) {
    const auto begin = std::find_if_not(input.begin(), input.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
    const auto end = std::find_if_not(input.rbegin(), input.rend(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    }).base();

    if (begin >= end) {
        return {};
    }
    return std::string(begin, end);
}

bool startsWithBoot2(const std::string& line) {
    constexpr char prefix[] = "BOOT2";
    if (line.size() < sizeof(prefix) - 1) {
        return false;
    }
    for (size_t i = 0; i < sizeof(prefix) - 1; ++i) {
        if (std::toupper(static_cast<unsigned char>(line[i])) != prefix[i]) {
            return false;
        }
    }
    return true;
}

std::string parseBootElf(const std::string& bootLine) {
    const auto equals = bootLine.find('=');
    if (equals == std::string::npos) {
        return {};
    }

    std::string value = trim(bootLine.substr(equals + 1));
    const auto slash = value.find_last_of("\\/");
    if (slash != std::string::npos) {
        value = value.substr(slash + 1);
    }
    const auto semicolon = value.find(';');
    if (semicolon != std::string::npos) {
        value = value.substr(0, semicolon);
    }
    return trim(value);
}

std::optional<BootInfo> readBootInfo(const fs::path& discRoot) {
    const fs::path systemCnf = discRoot / "SYSTEM.CNF";
    std::ifstream stream(systemCnf);
    if (!stream) {
        return std::nullopt;
    }

    std::string line;
    while (std::getline(stream, line)) {
        line = trim(line);
        if (!startsWithBoot2(line)) {
            continue;
        }
        return BootInfo{systemCnf, line, parseBootElf(line)};
    }

    return BootInfo{systemCnf, {}, {}};
}

void reportBootMovies(const fs::path& discRoot) {
    const std::vector<fs::path> bootMovies = {
        "data/movies/eabig.mpc",
        "data/movies/Intro_DJ.mpc",
        "data/movies/Intro.mpc",
    };

    bool any_ready = false;
    for (const auto& relative : bootMovies) {
        const fs::path path = discRoot / relative;
        std::error_code error;
        if (!fs::is_regular_file(path, error)) {
            std::cout << "[movies]  missing boot FMV: " << path << '\n';
            continue;
        }
        const auto size = fs::file_size(path, error);
        if (error || size == 0) {
            std::cout << "[movies]  stub (0 bytes): " << path << '\n';
            continue;
        }
        std::cout << "[movies]  ready: " << path << " (" << size << " bytes)\n";
        any_ready = true;
    }

    if (!any_ready) {
        std::cerr << "\n*** Boot movies will not play: disc/data/movies/*.mpc are empty stubs.\n"
                     "    Copy real files from a full SSX3 disc extract, e.g.:\n"
                     "      chmod u+w disc/data/movies/*.mpc\n"
                     "      scripts/copy_boot_movies.sh /path/to/disc/or.iso\n"
                     "    See docs/VIDEO_CODECS.md\n\n";
    }
}

bool reportFile(const fs::path& path, const std::string& label) {
    std::error_code error;
    if (!fs::is_regular_file(path, error)) {
        std::cout << "[missing] " << label << ": " << path << '\n';
        return false;
    }

    const auto size = fs::file_size(path, error);
    if (error) {
        std::cout << "[found]   " << label << ": " << path << '\n';
    } else {
        std::cout << "[found]   " << label << ": " << path << " (" << size << " bytes)\n";
    }
    return true;
}

struct Options {
    fs::path discRoot = "disc";
    fs::path rebuiltPs2Elf = "out/SLUS_207.72";
    bool runGfx = false;
    bool runGame = false;
    bool bootThenGame = false;
    bool bootGame = false;
    bool bootThenGfx = false;
    bool bootVideos = true;
    bool noBootVideos = false;
    int width = 1280;
    int height = 720;
    bool vsync = true;
    host::UpscaleBackend upscale = host::UpscaleBackend::Compute;
    float renderScale = 0.67f;
    host::HostFontChoice fontChoice = host::HostFontChoice::Auto;
    std::string resolutionPreset;
    bool widthExplicit = false;
    bool heightExplicit = false;
    bool renderScaleExplicit = false;
};

void apply_resolution_preset(Options& options) {
    if (options.resolutionPreset.empty()) {
        return;
    }
    const host::ResolutionPreset* preset =
        host::resolution_preset_by_id(options.resolutionPreset.c_str());
    if (!preset) {
        std::cerr << "unknown --resolution preset: " << options.resolutionPreset << '\n';
        host::print_resolution_presets();
        std::exit(1);
    }
    if (!options.widthExplicit) {
        options.width = preset->width;
    }
    if (!options.heightExplicit) {
        options.height = preset->height;
    }
    if (!options.renderScaleExplicit && preset->suggested_render_scale > 0.0f &&
        options.upscale != host::UpscaleBackend::Direct) {
        options.renderScale = preset->suggested_render_scale;
    }
    std::cout << "[gfx]     resolution preset " << preset->id << " → " << options.width << "x"
              << options.height;
    if (!options.renderScaleExplicit && preset->suggested_render_scale > 0.0f) {
        std::cout << ", render-scale " << options.renderScale;
    }
    std::cout << '\n';
}

Options parse_args(int argc, char** argv) {
    Options options;
    int positional = 0;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--gfx") {
            options.runGfx = true;
        } else if (arg == "--game") {
            options.runGame = true;
        } else if (arg == "--boot-then-game") {
            options.bootThenGame = true;
            options.runGame = true;
        } else if (arg == "--boot-game") {
            options.bootGame = true;
        } else if (arg == "--boot-videos") {
            options.bootVideos = true;
        } else if (arg == "--no-boot-videos") {
            options.noBootVideos = true;
            options.bootVideos = false;
        } else if (arg == "--no-vsync") {
            options.vsync = false;
        } else if (arg == "--upscale" && i + 1 < argc) {
            const std::string mode = argv[++i];
            if (mode == "direct") {
                options.upscale = host::UpscaleBackend::Direct;
            } else if (mode == "compute") {
                options.upscale = host::UpscaleBackend::Compute;
            } else if (mode == "dlss") {
                options.upscale = host::UpscaleBackend::DLSS;
            } else {
                std::cerr << "unknown upscale mode: " << mode << " (use direct|compute|dlss)\n";
                std::exit(1);
            }
        } else if (arg == "--render-scale" && i + 1 < argc) {
            options.renderScale = std::stof(argv[++i]);
            options.renderScaleExplicit = true;
        } else if (arg == "--resolution" && i + 1 < argc) {
            options.resolutionPreset = argv[++i];
        } else if (arg == "--list-resolutions") {
            host::print_resolution_presets();
            std::exit(0);
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "usage: ssx3-native [--gfx] [--game] [--boot-then-game] [--boot-game] [--boot-then-gfx]\n"
                         "                  [--boot-videos] [--no-boot-videos]\n"
                         "                  [--font auto|retail|custom] [--upscale direct|compute|dlss]\n"
                         "                  [--resolution PRESET] [--list-resolutions]\n"
                         "                  [--render-scale F] [--no-vsync] [--width N] [--height N] [disc] [rebuilt_elf]\n";
            std::cout << "resolution presets: ps2 720p wxga 1280x896 900p 1080p 1200p 1440p 4k (see --list-resolutions)\n";
            std::exit(0);
        } else if (arg == "--width" && i + 1 < argc) {
            options.width = std::stoi(argv[++i]);
            options.widthExplicit = true;
        } else if (arg == "--height" && i + 1 < argc) {
            options.height = std::stoi(argv[++i]);
            options.heightExplicit = true;
        } else if (arg == "--font" && i + 1 < argc) {
            const std::string mode = argv[++i];
            if (mode == "auto") {
                options.fontChoice = host::HostFontChoice::Auto;
            } else if (mode == "retail" || mode == "fe") {
                options.fontChoice = host::HostFontChoice::Retail;
            } else if (mode == "custom" || mode == "host") {
                options.fontChoice = host::HostFontChoice::Custom;
            } else {
                std::cerr << "unknown --font mode: " << mode << " (use auto|retail|custom)\n";
                std::exit(1);
            }
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "unknown option: " << arg << '\n';
            std::exit(1);
        } else if (positional == 0) {
            options.discRoot = arg;
            ++positional;
        } else if (positional == 1) {
            options.rebuiltPs2Elf = arg;
            ++positional;
        } else {
            std::cerr << "unexpected argument: " << arg << '\n';
            std::exit(1);
        }
    }
    apply_resolution_preset(options);
    return options;
}

} // namespace

int main(int argc, char** argv) {
    const Options options = parse_args(argc, argv);
    const fs::path discRoot = options.discRoot;
    const fs::path rebuiltPs2Elf = options.rebuiltPs2Elf;

    std::cout << "SSX3 Linux native runtime\n";
    std::cout << "disc root: " << discRoot << '\n';

    host::disc().set_root(discRoot);

    const auto bootInfo = readBootInfo(discRoot);
    if (!bootInfo) {
        std::cerr << "[error]   missing or unreadable SYSTEM.CNF under " << discRoot << '\n';
        return 1;
    }

    bool ok = true;
    ok = reportFile(bootInfo->systemCnfPath, "system config") && ok;

    if (bootInfo->bootLine.empty() || bootInfo->bootElf.empty()) {
        std::cerr << "[error]   SYSTEM.CNF does not contain a usable BOOT2 line\n";
        ok = false;
    } else {
        std::cout << "[boot]    " << bootInfo->bootLine << '\n';
        ok = reportFile(discRoot / bootInfo->bootElf, "disc boot elf") && ok;
    }

    ok = reportFile(rebuiltPs2Elf, "rebuilt ps2 elf") && ok;

    const std::vector<fs::path> requiredFiles = {
        "pad0.000",
        "pad1.000",
        "netgui/ntgui.elf",
    };
    for (const auto& relativePath : requiredFiles) {
        ok = reportFile(discRoot / relativePath, "disc asset") && ok;
    }

    reportBootMovies(discRoot);

    if (!ok) {
        std::cerr << "native bootstrap failed: expected SSX3 disc files were not found.\n";
        return 1;
    }

    std::cout << "native bootstrap passed.\n";

    if (!host::run_smoke_tests()) {
        std::cerr << "host smoke tests failed.\n";
        return 1;
    }

    if (options.bootGame) {
        if (!host::run_game_boot_chain(argc, argv)) {
            std::cerr << "game boot chain failed.\n";
            return 1;
        }
        return 0;
    }

    host::host_fe_font_set_choice(options.fontChoice);

    if (options.bootThenGfx) {
        if (!host::run_retail_boot_init(argc, argv)) {
            std::cerr << "retail boot init failed.\n";
            return 1;
        }
        host::RendererConfig gfx{};
        gfx.width = options.width;
        gfx.height = options.height;
        gfx.vsync = options.vsync;
        gfx.shader_dir = "obj/host/shaders";
        gfx.upscale = options.upscale;
        gfx.render_scale = options.renderScale;
        const bool boot_videos = options.bootVideos && !options.noBootVideos;
        const bool ok_gfx = host::run_native_gfx_session(gfx, discRoot.string(), boot_videos);
        host::run_retail_boot_shutdown();
        if (!ok_gfx) {
            std::cerr << "Vulkan session after boot init failed.\n";
            return 1;
        }
        return 0;
    }

    if (options.runGame) {
        host::RendererConfig gfx{};
        gfx.width = options.width;
        gfx.height = options.height;
        gfx.vsync = options.vsync;
        gfx.shader_dir = "obj/host/shaders";
        gfx.upscale = host::UpscaleBackend::Direct;
        if (!host::run_native_world_session(gfx, options.bootThenGame)) {
            std::cerr << "3D gameplay session failed.\n";
            return 1;
        }
        std::cout << "3D gameplay session exited.\n";
        return 0;
    }

    if (options.runGfx) {
        host::RendererConfig gfx{};
        gfx.width = options.width;
        gfx.height = options.height;
        gfx.vsync = options.vsync;
        gfx.shader_dir = "obj/host/shaders";
        gfx.upscale = options.upscale;
        gfx.render_scale = options.renderScale;
        const bool boot_videos = options.bootVideos && !options.noBootVideos;
        if (!host::run_native_gfx_session(gfx, discRoot.string(), boot_videos)) {
            std::cerr << "Vulkan renderer failed.\n";
            return 1;
        }
        std::cout << "Vulkan demo exited.\n";
        return 0;
    }

    if (options.bootVideos && !options.noBootVideos) {
        host::RendererConfig gfx{};
        gfx.width = options.width;
        gfx.height = options.height;
        gfx.title = "SSX3";
        if (!host::run_native_gfx_session(gfx, discRoot.string(), true, false)) {
            std::cout << "[video]   boot movies skipped or unavailable (see docs/VIDEO_CODECS.md)\n";
        }
    }

    std::cout << "runtime status: host I/O and decompiled utilities are linked.\n";
    std::cout << "hint: --boot-game runs retail boot → FE menu (peak room Cross = race when boot active).\n";
    std::cout << "hint: --boot-then-gfx runs retail init then FMV + Vulkan title menu.\n";
    std::cout << "hint: --gfx FE flow: title → menu → Freeride / Options → Game options.\n";
    std::cout << "hint: --game flyover on procedural alpine terrain (Vulkan 3D, retail world stubs).\n";
    std::cout << "hint: --boot-then-game: retail init → load track 0 → Vulkan flyover (cSSXApp_preUpdate ticks race).\n";
    std::cout << "hint: keyboard: arrows, E/S Cross, W/Esc Triangle back, A Square options, Space start.\n";
    std::cout << "hint: --no-boot-videos skips FMV; Space/Enter skips current movie.\n";
    std::cout << "hint: --resolution 1080p|1440p|4k|ps2|720p (default 1280x720); --list-resolutions.\n";
    std::cout << "hint: F11 toggles fullscreen in --gfx / --game windows.\n";
    return 0;
}
