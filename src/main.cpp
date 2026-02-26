#include <iostream>
#include "Engine.hpp"

#include "Atlas.hpp"
#include "RenderSystem.hpp"
#include "model/Texture.hpp"

int main()
{
	Texture2D stone;
	stone.load(absolutePath+"resources/img/stone.png");

	Texture2D sand;
	sand.load(absolutePath+"resources/img/sand.png");

	AtlasDescriptor atlasDesc(256, 256, 0, 0);
	atlasDesc.insert(&stone);
	atlasDesc.insert(&sand);

	AtlasBitmap atlasBitmap(atlasDesc);
	atlasBitmap.save(absolutePath+"resources/img/atlas.png");

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
