#pragma once

#include "Pipeline.hpp"
#include "Model.hpp"
#include "FrameInfo.hpp"

#include "Camera.hpp"

#include <memory>
#include <vector>
#include <map>

const std::string absolutePath = "C:\\cplusplus\\VulkanRender\\VulkanRender\\";

struct PushConstantData {
	alignas(16) glm::mat4 model;
};

namespace myvk {
	class RenderSystem {
	public:
		RenderSystem(Device& device, VkRenderPass renderPass, std::vector<VkDescriptorSetLayout> layouts);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void render(FrameInfo& frame, Model *model);
		//void addModel(Model* model);
	private:
		void createPipelineLayout(const std::vector<VkDescriptorSetLayout>& layouts);
		void createPipeline(VkRenderPass renderPass);

		Device& device;

		//std::vector<Model*> models;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}