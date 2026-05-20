#include "platform/host_disc.h"

#include <algorithm>
#include <cctype>

namespace host {

namespace {

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

} // namespace

void Disc::set_root(std::filesystem::path root) {
    root_ = std::move(root);
}

std::filesystem::path Disc::resolve(const std::filesystem::path& relative) const {
    if (relative.is_absolute()) {
        return relative;
    }
    return root_ / relative;
}

std::optional<std::filesystem::path> Disc::resolve_ps2_path(const std::string& ps2Path) const {
    std::string value = trim(ps2Path);
    if (value.empty()) {
        return std::nullopt;
    }

    constexpr char cdromPrefix[] = "cdrom";
    if (value.size() >= sizeof(cdromPrefix) - 1) {
        bool matches = true;
        for (size_t i = 0; i < sizeof(cdromPrefix) - 1; ++i) {
            if (std::tolower(static_cast<unsigned char>(value[i])) != cdromPrefix[i]) {
                matches = false;
                break;
            }
        }
        if (matches) {
            const auto colon = value.find(':');
            if (colon != std::string::npos) {
                value = value.substr(colon + 1);
            }
        }
    }

    while (!value.empty() && (value.front() == '\\' || value.front() == '/')) {
        value.erase(value.begin());
    }

    const auto semicolon = value.find(';');
    if (semicolon != std::string::npos) {
        value = value.substr(0, semicolon);
    }

    value = trim(value);
    if (value.empty()) {
        return std::nullopt;
    }
    return resolve(value);
}

bool Disc::exists(const std::filesystem::path& relative) const {
    std::error_code error;
    return std::filesystem::is_regular_file(resolve(relative), error);
}

std::optional<std::uintmax_t> Disc::file_size(const std::filesystem::path& relative) const {
    std::error_code error;
    const auto path = resolve(relative);
    if (!std::filesystem::is_regular_file(path, error)) {
        return std::nullopt;
    }
    const auto size = std::filesystem::file_size(path, error);
    if (error) {
        return std::nullopt;
    }
    return size;
}

std::vector<std::filesystem::path> Disc::list_children(const std::filesystem::path& relative) const {
    std::vector<std::filesystem::path> children;
    std::error_code error;
    const auto dir = resolve(relative);
    if (!std::filesystem::is_directory(dir, error)) {
        return children;
    }

    for (const auto& entry : std::filesystem::directory_iterator(dir, error)) {
        if (error) {
            break;
        }
        children.push_back(entry.path().filename());
    }

    std::sort(children.begin(), children.end());
    return children;
}

Disc& disc() {
    static Disc instance;
    return instance;
}

} // namespace host
