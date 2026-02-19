#include "Material.hpp"

namespace myvk {

Material::Material(DescriptorPool& pool,
                   DescriptorSetLayout& layout,
                   std::shared_ptr<Texture> albedo) : albedo(albedo)
{
   create(pool, layout, albedo);
}

void Material::create(DescriptorPool& pool,
                DescriptorSetLayout& layout,
                std::shared_ptr<Texture> albedo) 
{
    VkDescriptorImageInfo imageInfo;
	if(albedo) {
		imageInfo.sampler   = albedo->getSampler();
		imageInfo.imageView = albedo->getView();
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

    descriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
	for(int i = 0; i < descriptorSets.size(); i++) {
		DescriptorWriter(layout, pool)
			.writeImage(0, &imageInfo)
			.build(descriptorSets[i]);
	}
}

void Material::bind(VkCommandBuffer commandBuffer, VkPipelineLayout& pipelineLayout, int frame) {
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout,
        1,
        1,
        &descriptorSets[frame],
        0,
	    nullptr);
}

}