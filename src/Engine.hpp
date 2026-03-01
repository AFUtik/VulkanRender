#pragma once

#include "glm/gtc/type_precision.hpp"
#include "vulkan/vulkan_core.h"
#include "window/Events.hpp"
#include "window/Window.hpp"
#include "Renderer.hpp"

#include "Camera.hpp"
#include "Descriptors.hpp"
#include "Buffer.hpp"
#include "FrameInfo.hpp"

#include "model/Model.hpp"

#include <memory>
#include <vector>

#include "Fonts.hpp"

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
		void createGlobalLayouts();
		void createUILayouts();
		void loadModels();
		
		

		Window window{ WIDTH, HEIGHT, "Vulkan Engine" };
		Device device{ window };
		Renderer renderer{ window, device };

		FontHandler fontHandler;

		std::unique_ptr<Font> curFont = nullptr;
		std::unique_ptr<Text> curText = nullptr;

		Camera camera;

		std::unique_ptr<DescriptorPool> globalPool{};
		std::unique_ptr<DescriptorSetLayout> globalSetLayout;
		std::unique_ptr<DescriptorSetLayout> materialSetLayout;
		std::vector<VkDescriptorSet> globalDescriptorSets;
		std::vector<VkDescriptorSetLayout> globalLayouts;
		std::vector<std::unique_ptr<Buffer>> globalUniform;

		std::unique_ptr<DescriptorSetLayout> uiSetLayout;
		std::unique_ptr<DescriptorSetLayout> uiMaterialSetLayout;
		std::vector<VkDescriptorSet> uiDescriptorSets;
		std::vector<VkDescriptorSetLayout> uiLayouts;
		std::vector<std::unique_ptr<Buffer>> uiUniform;

		std::shared_ptr<Model> model;
		std::shared_ptr<Model> textModel;
	};
}