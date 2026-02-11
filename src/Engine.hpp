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
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Engine();
		~Engine();

		Engine(const Engine&) = delete;
		void operator=(const Engine&) = delete;

		void run();
	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		Window window{ WIDTH, HEIGHT, "Vulkan Engine" };
		Device device{ window };
		SwapChain swapchain{device, window.getExtent()};

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;

		std::unique_ptr<Model> model;
	};
}