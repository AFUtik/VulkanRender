#pragma once

#include <vulkan/vulkan.h>

namespace myvk {

struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
  };
}
