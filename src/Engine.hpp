#pragma once

#include "window/Window.hpp"
#include "Pipeline.hpp"

const std::string absolutePath = "C:\\cplusplus\\VulkanRender\\VulkanRender\\";

namespace myvk {
	class Engine {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Engine() {};
		~Engine() {};

		Engine(const Engine&) = delete;
		void operator=(const Engine&) = delete;

		void run();
	private:
		Window window{ WIDTH, HEIGHT, "Vulkan Engine" };
		Device device{ window };
		VkPipelineLayout pipelineLayout;
		Pipeline pipeline{
			device,
			absolutePath + "resources\\shaders\\shader.vert.spv",
			absolutePath + "resources\\shaders\\shader.frag.spv",
			Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
		};
	};
}