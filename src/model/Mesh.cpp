#include "Mesh.hpp"
#include "../Fonts.hpp"

#include <iostream>

namespace myvk {


TextMesh::TextMesh(Text* text) : MeshInstance(512), text(text) {
    buildVertices();
}

void TextMesh::buildVertices() {
    Font* font = text->getFont();

    FontSample* sample = font->sample();
    AtlasBitmap& atlasBitmap = font->bitmap();

    for(auto codepoint : text->getContent()) {
        std::cout << static_cast<uint32_t>(codepoint) << std::endl;

        uint32_t index = sample->getIndex(codepoint);
        const Tile& tile = atlasBitmap.getTile(index);

        vertices.push_back({(float)(tile.width+pen_x), (float)(tile.height+pen_y), 0.0f, tile.u2, tile.v1, 0.0f, 0.0f, 0.0f, 1.0f});
        vertices.push_back({(float)(tile.width+pen_x), (float)pen_y,               0.0f, tile.u2, tile.v2, 0.0f, 0.0f, 0.0f, 1.0f});
        vertices.push_back({(float)pen_x,              (float)pen_y,               0.0f, tile.u1, tile.v2, 0.0f, 0.0f, 0.0f, 1.0f});
        vertices.push_back({(float)pen_x,              (float)(tile.height+pen_y), 0.0f, tile.u1, tile.v1, 0.0f, 0.0f, 0.0f, 1.0f});

        indices.push_back(indCount+0);
        indices.push_back(indCount+1);
        indices.push_back(indCount+2);
        indices.push_back(indCount+2);
        indices.push_back(indCount+3);
        indices.push_back(indCount+0);
        indCount+=4;

        pen_x += tile.width + 5;
    }
}

}