#pragma once

#include "Descriptors.hpp"
#include "Device.hpp"
#include "FrameInfo.hpp"
#include "Texture.hpp"
#include "vulkan/vulkan.h"
#include "Swapchain.hpp"

#include <memory>

namespace myvk {

class Material {
public:
    Material(DescriptorPool& pool,
             DescriptorSetLayout& layout,
             std::shared_ptr<Texture> albedo);

    void create(DescriptorPool& pool,
                DescriptorSetLayout& layout,
                std::shared_ptr<Texture> albedo);
    void bind(VkCommandBuffer commandBuffer, VkPipelineLayout& layout, int frame);
private:
    std::vector<VkDescriptorSet> descriptorSets;
    std::shared_ptr<Texture> albedo;
};

using MaterialPtr = std::shared_ptr<Material>;

}