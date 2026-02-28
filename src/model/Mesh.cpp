#include "Mesh.hpp"
#include "../Fonts.hpp"

#include <iostream>

namespace myvk {


TextMesh::TextMesh(Text* text) : MeshInstance(512), text(text) {
    buildVertices();
}

void TextMesh::buildVertices() {
    Font* font = text->getFont();
    FontInfo& fontInfo = font->info();

    FontSample* sample = font->sample();
    AtlasBitmap& atlasBitmap = font->bitmap();

    char32_t prevCodepoint = 0;
    for(auto codepoint : text->getContent()) {
        std::cout << static_cast<uint32_t>(codepoint) << std::endl;

        uint32_t index = sample->getCharIndex(codepoint);
        GlyphInfo& glyph = fontInfo.infoGlyphs[index];

        const Tile& tile = atlasBitmap.getTile(index);

        vertices.push_back({(float)(pen_x)+tile.width,  (float)(tile.height+pen_y), 0.0f, tile.u2, tile.v1, 0.0f, 0.0f, 0.0f, 1.0f});
        vertices.push_back({(float)(pen_x)+tile.width,  (float)pen_y,               0.0f, tile.u2, tile.v2, 0.0f, 0.0f, 0.0f, 1.0f});
        vertices.push_back({(float)pen_x,              (float)pen_y,               0.0f, tile.u1, tile.v2, 0.0f, 0.0f, 0.0f, 1.0f});
        vertices.push_back({(float)pen_x,              (float)(tile.height+pen_y), 0.0f, tile.u1, tile.v1, 0.0f, 0.0f, 0.0f, 1.0f});

        indices.push_back(indCount+0);
        indices.push_back(indCount+1);
        indices.push_back(indCount+2);
        indices.push_back(indCount+2);
        indices.push_back(indCount+3);
        indices.push_back(indCount+0);
        indCount+=4;

        pen_x += glyph.advance_x;
        prevCodepoint = codepoint;
    }
}

}