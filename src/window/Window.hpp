#ifndef WINDOW_WINDOW_HPP_
#define WINDOW_WINDOW_HPP_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace myvk {
	class Window {
	public:
		GLFWwindow* window;

		int init(const char* title);

		Window(int width, int height, const char* title);
		~Window();

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		bool isShouldClose();
		void setShouldClose(bool flag);
		void swapBuffers();
		void setCursorMode(int mode);

		bool wasWindowResized() { return frameBufferResized; }
		void resetWindowResizedFlag() { frameBufferResized = false; }
	private:
		static void framebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);

		int width;
		int height;
		bool frameBufferResized = false;
	};
}


#endif
