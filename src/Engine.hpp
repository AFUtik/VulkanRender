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
		//void loadModels();

		Window window{ WIDTH, HEIGHT, "Vulkan Engine" };
		Device device{ window };

		FrameInfo frameInfo;
		Camera camera;
	};
}