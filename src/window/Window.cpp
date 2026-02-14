#include <iostream>

#include "Window.hpp"

using namespace myvk;

Window::Window(int width, int height, const char* title) : width(width), height(height) {
	init(title);
}

Window::~Window() {
	if (window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

int Window::init(const char* title) {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	return 0;
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to craete window surface");
	}
}

void Window::framebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height) {
	auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	window->frameBufferResized = true;
	window->width = width;
	window->height = height;
}

void Window::setCursorMode(int mode) {
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

bool Window::isShouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool flag) {
	glfwSetWindowShouldClose(window, flag);
}

void Window::swapBuffers() {
	glfwSwapBuffers(window);
}
