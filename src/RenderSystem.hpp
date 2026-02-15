#pragma once

#include "Pipeline.hpp"
#include "Model.hpp"
#include "FrameInfo.hpp"

#include "Camera.hpp"

#include <memory>
#include <vector>

const std::string absolutePath = "C:\\cplusplus\\VulkanRender\\VulkanRender\\";

struct PushConstantData {
	alignas(16) glm::mat4 model;
};

namespace myvk {
	class RenderSystem {
	public:
		RenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void render(FrameInfo& frame, Model* model);
	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		Device& device;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}