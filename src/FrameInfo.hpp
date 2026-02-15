#pragma once

#include "Camera.hpp"

// lib
#include <vulkan/vulkan.h>

namespace myvk {
struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  Camera &camera;
  VkDescriptorSet globalDescriptorSet;
};
}  // namespace lve