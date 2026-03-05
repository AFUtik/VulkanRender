#include "UIRenderSystem.hpp"

namespace myvk {

UIRenderSystem::UIRenderSystem(Device &device, VkRenderPass renderPass, std::vector<VkDescriptorSetLayout> layouts, GUIContext* context) : 
	RenderSystem(device), guiRender(std::make_unique<GUIRender>(context))
{
    createPipelineLayout(layouts);

	PipelineConfigInfo config{};
	Pipeline::defaultPipelineConfigInfo(config);
	config.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;

	config.depthStencilInfo.depthTestEnable  = VK_FALSE;
	config.depthStencilInfo.depthWriteEnable = VK_FALSE;

	config.colorBlendAttachment.blendEnable = VK_TRUE;
	config.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	config.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	config.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	createPipeline(renderPass, config);
}

void UIRenderSystem::renderGUI(FrameInfo& frameInfo) {
	
}

}