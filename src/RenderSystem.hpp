#pragma once

#include "Pipeline.hpp"
#include "Model.hpp"
#include "FrameInfo.hpp"

#include "Camera.hpp"

#include <memory>
#include <unordered_map>
#include <vector>
#include <unordered_map>

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

		void create(std::shared_ptr<Model> model);
		void render(FrameInfo& frame);
	private:
		void createPipelineLayout(const std::vector<VkDescriptorSetLayout>& layouts);
		void createPipeline(VkRenderPass renderPass);

		std::unordered_map<Model*, std::shared_ptr<Model>> models;

		Device& device;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}