#include "Engine.hpp"

using namespace myvk;

void Engine::run() {
	while (!window.isShouldClose()) {
		glfwPollEvents();
	}
}