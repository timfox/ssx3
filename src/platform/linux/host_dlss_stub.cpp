#include "platform/host_dlss.h"

#include <iostream>

namespace host {

bool DlssUpscaler::init(VkInstance,
                        const VulkanDeviceContext&,
                        VkFormat,
                        VkExtent2D,
                        VkExtent2D) {
    std::cerr << "[dlss]    not available: build with DLSS_SDK_ROOT pointing at NVIDIA/DLSS clone\n";
    std::cerr << "          git clone https://github.com/NVIDIA/DLSS && export DLSS_SDK_ROOT=$PWD/DLSS\n";
    active_ = false;
    return false;
}

void DlssUpscaler::shutdown() {
    active_ = false;
}

void DlssUpscaler::resize(VkExtent2D, VkExtent2D) {}

void DlssUpscaler::evaluate(VkCommandBuffer,
                            VkImageView,
                            VkImage,
                            VkImageView,
                            VkImage,
                            VkExtent2D,
                            VkExtent2D) {}

} // namespace host
