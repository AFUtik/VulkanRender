#pragma once

#include "RenderSystem.hpp"

namespace myvk {

class UIRenderSystem : public RenderSystem {
public:
    UIRenderSystem(Device &device, VkRenderPass renderPass, std::vector<VkDescriptorSetLayout> layouts);
};

}