#include "Engine.hpp"
#include "Buffer.hpp"
#include "Descriptors.hpp"
#include "RenderSystem.hpp"
#include "Swapchain.hpp"
#include "Texture.hpp"
#include "vulkan/vulkan_core.h"

#include <memory>
#include <thread>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

using namespace myvk;

struct GlobalUbo {
	glm::mat4 projview{1.f};
};

Engine::Engine() : camera(window.width, window.height, glm::dvec3(0, 0, 0), glm::radians(90.0f)) {
	Events::init(window.window);

	globalPool = DescriptorPool::Builder(device)
		.setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SwapChain::MAX_FRAMES_IN_FLIGHT)
		.build();

	loadModels();
}

Engine::~Engine() {

}

void Engine::run() {
	std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
	for(int i = 0; i < uboBuffers.size(); i++) {
		uboBuffers[i] = std::make_unique<Buffer>(
			device, 
			sizeof(GlobalUbo),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 
			VMA_MEMORY_USAGE_CPU_TO_GPU);
		uboBuffers[i]->map();
	}

	auto globalSetLayout = DescriptorSetLayout::Builder(device)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
		.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
		.build();

	VkDescriptorImageInfo imageInfo;
	if(model->texture) {
		imageInfo.sampler = model->texture->getSampler();
		imageInfo.imageView = model->texture->getView();
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
	for(int i = 0; i < globalDescriptorSets.size(); i++) {
		auto bufferInfo = uboBuffers[i]->descriptorInfo();
		DescriptorWriter(*globalSetLayout, *globalPool)
			.writeBuffer(0, &bufferInfo)
			.writeImage (1, &imageInfo)
			.build(globalDescriptorSets[i]);
	}

	RenderSystem renderSystem(device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

	Events::toggle_cursor(&window);
	double lastTime = glfwGetTime();
	double timeAccu = 0.0f;
	const double target_fps = 60.0;
	const double H = 1.0f / target_fps;
	const double speed = 2.0;
	float camX = 0.0f;
	float camY = 0.0f;
	while (!window.isShouldClose()) {
		double currentTime = glfwGetTime();
		double frameTime = currentTime - lastTime;
		lastTime = currentTime;

		timeAccu += frameTime;
		if (timeAccu >= H) {
			if (Events::pressed(GLFW_KEY_W)) {
				camera.translate(camera.zdir() * H * speed);
			}
			if (Events::pressed(GLFW_KEY_S)) {
				camera.translate(-camera.zdir() * H * speed);
			}
			if (Events::pressed(GLFW_KEY_D)) {
				camera.translate(camera.xdir() * H * speed);
			}
			if (Events::pressed(GLFW_KEY_A)) {
				camera.translate(-camera.xdir() * H * speed);
			}
			if (Events::pressed(GLFW_KEY_TAB)) {
				Events::toggle_cursor(&window);
			}
			if (Events::pressed(GLFW_KEY_G)) {
				model->transform.rotateLocal(0.05, { 1.0, 0.0f, 0.0f });
			}
			if (Events::_cursor_locked) {
				camY += -Events::deltaY / window.height * 2;
				camX += -Events::deltaX / window.height * 2;

				if (camY < -radians(89.0f)) {
					camY = -radians(89.0f);
				}
				if (camY > radians(89.0f)) {
					camY = radians(89.0f);
				}

				camera.setRotation(glm::mat4(1.0f));
				camera.rotate(-camY, camX, 0);
			}

			if (auto commandBuffer = renderer.beginFrame()) {
				renderer.beginSwapChainRenderPass(commandBuffer);

				int frameIndex = renderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					(float)frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex]};

				camera.update();

				GlobalUbo ubo{};
				ubo.projview = camera.getProjview();
				
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				renderSystem.render(frameInfo, model.get());

				renderer.endSwapChainRenderPass(commandBuffer);
				renderer.endFrame();
			}
			timeAccu -= H;
		}
		else {
			double sleepTime = H - timeAccu;
			std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
		}
		Events::pullEvents();
	}
	vkDeviceWaitIdle(device.device());
}

void Engine::loadModels() {
	std::vector<Model::Vertex> vertices{
		{1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
		{-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
		{-1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f}
	};
	std::vector<uint32_t> indices{
		0, 1, 2,
    	2, 3, 0
	};
	model = std::make_unique<Model>(device, vertices, indices);
	model->texture = std::make_unique<Texture>(device, "C:/cplusplus/VulkanRender/VulkanRender/resources/img/green.png");
}