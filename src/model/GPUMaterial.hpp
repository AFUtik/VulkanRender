#pragma once

#include "../Descriptors.hpp"
#include "../Device.hpp"

#include "GPUTexture.hpp"

#include <memory>

namespace myvk {

class GPUMaterial {
public:
    GPUMaterial(DescriptorPool& pool,
             DescriptorSetLayout& layout,
             std::shared_ptr<GPUTexture> albedo);

    void create(std::shared_ptr<GPUTexture> albedo);
                
    void bind(VkCommandBuffer commandBuffer, VkPipelineLayout& layout, int frame) const;
private:
    DescriptorPool& pool;
    DescriptorSetLayout& layout;

    std::vector<VkDescriptorSet> descriptorSets;

    std::shared_ptr<GPUTexture> albedo;
};

}