#ifndef WINDOW_WINDOW_HPP_
#define WINDOW_WINDOW_HPP_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace myvk {
	class Window {
	public:
		static int width;
		static int height;
		static GLFWwindow* window;

		static int init(int width, int height, const char* title);

		Window(int width, int height, const char* title);
		~Window();

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		static bool isShouldClose();
		static void setShouldClose(bool flag);
		static void swapBuffers();
		static void setCursorMode(int mode);
	};
}


#endif
