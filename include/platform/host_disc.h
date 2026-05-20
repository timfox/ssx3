#ifndef PLATFORM_HOST_DISC_H
#define PLATFORM_HOST_DISC_H

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace host {

class Disc {
public:
    void set_root(std::filesystem::path root);
    const std::filesystem::path& root() const { return root_; }

    std::filesystem::path resolve(const std::filesystem::path& relative) const;
    std::optional<std::filesystem::path> resolve_ps2_path(const std::string& ps2Path) const;

    bool exists(const std::filesystem::path& relative) const;
    std::optional<std::uintmax_t> file_size(const std::filesystem::path& relative) const;
    std::vector<std::filesystem::path> list_children(const std::filesystem::path& relative) const;

private:
    std::filesystem::path root_;
};

Disc& disc();

} // namespace host

#endif /* PLATFORM_HOST_DISC_H */
