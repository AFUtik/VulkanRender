#include <iostream>
#include "Engine.hpp"

#include "Atlas.hpp"
#include "RenderSystem.hpp"
#include "model/Texture.hpp"

#include "Fonts.hpp"

int main()
{
	FontHandler fontHandler;
	fontHandler.loadFromDisk("PlayfulTime", absolutePath+"resources/fonts/playfulTime.ttf");

	FontSample* sample = fontHandler.getFontSample("PlayfulTime");

	Font font;
	font.sample = sample;

	


	std::cout << "okay" << std::endl;

	//myvk::Engine engine{};

	//try {
	//	engine.run();
	//}
	//catch (const std::exception& e) {
	//	std::cerr << e.what() << '\n';
	//	return EXIT_FAILURE;
	//}
	return 0;
}
