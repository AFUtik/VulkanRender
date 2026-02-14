#pragma once

#include "window/Window.hpp"
#include "Pipeline.hpp"
#include "Swapchain.hpp"
#include "Model.hpp"

#include <memory>
#include <vector>

const std::string absolutePath = "C:\\cplusplus\\VulkanRender\\VulkanRender\\";

namespace myvk {
	class Engine {
	public:
		static constexpr int WIDTH = 1920;
		static constexpr int HEIGHT = 1080;

		Engine();
		~Engine();

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		void run();
	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffers(int imageIndex);
		void freeCommandBuffers();

		Window window{ WIDTH, HEIGHT, "Vulkan Engine" };
		Device device{ window };
		
		std::unique_ptr<SwapChain> swapchain;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;

		std::unique_ptr<Model> model;
	};
}