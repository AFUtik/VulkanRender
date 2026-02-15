#pragma once

#include "window/Events.hpp"
#include "window/Window.hpp"
#include "Renderer.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Descriptors.hpp"
#include "Buffer.hpp"
#include "FrameInfo.hpp"

#include <memory>
#include <vector>

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

		Window window{ WIDTH, HEIGHT, "Vulkan Engine" };
		Device device{ window };
		Renderer renderer{ window, device };

		Camera camera;

		std::unique_ptr<DescriptorPool> globalPool{};
		std::unique_ptr<Model> model;
	};
}