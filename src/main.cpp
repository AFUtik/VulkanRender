#include <iostream>
#include "Engine.hpp"

#include "Atlas.hpp"
#include "RenderSystem.hpp"
#include "model/Texture.hpp"

#include "Fonts.hpp"

#include <string>

int main()
{
	myvk::Engine engine{};

	try {
		engine.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return 0;
}
