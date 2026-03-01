#include "Fonts.hpp"
#include "Atlas.hpp"

#include "freetype/freetype.h"
#include "freetype/ftimage.h"
#include "model/Texture.hpp"

#include <memory>
#include <stdexcept>
#include <iostream>

#include <ft2build.h>
#include <fstream>

// FONT //

Font::Font(FontSample* sample) : sample(sample) 
{
    sample->create(fontData);
}

Font::~Font() {
    FT_Done_Face(fontData.face);
}

FT_Vector Font::getKerning(char32_t prevChar, char32_t nextChar) {
    if (FT_HAS_KERNING(fontData.face)) {
        FT_Vector delta;
        FT_Get_Kerning(fontData.face, 
            sample->getGlyphIndex(prevChar), 
            sample->getGlyphIndex(nextChar),
            FT_KERNING_DEFAULT, 
            &delta
        );
        return {delta.x >> 6, delta.y >> 6};
    }
    return {0, 0};
}

// TEXT //

Text::Text(Font* font, std::u32string content) : font(font), content(content) {

}

// FONT SAMPLE //

FontSample::FontSample(FT_Library library, std::string_view view) : library(library), path(view) {
    readFont();
    loadCharset();
}

FontSample::~FontSample() {

}

void FontSample::readFont() {
    std::ifstream file(path, std::ios::binary);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    fontBuffer.resize(size);
    file.read(reinterpret_cast<char*>(fontBuffer.data()), size);
}

void FontSample::create(FontData& data) {
    if(FT_New_Memory_Face(library, fontBuffer.data(), fontBuffer.size(), 0, &data.face)) 
    {
        throw std::runtime_error("Failed to load font from memory buffer.");
    }

    FT_Set_Pixel_Sizes(data.face, 0, data.pxHeight);
    data.baseline_y  = data.face->size->metrics.ascender >> 6;

    FT_UInt glyph_index;
    FT_ULong charcode = FT_Get_First_Char(data.face, &glyph_index);

    uint32_t i = 0;
    data.infoGlyphs.resize(charset.size());
    while (glyph_index != 0) {
        FT_Load_Glyph(data.face, glyph_index, FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING);
        GlyphInfo& glyphInfo = data.infoGlyphs[i];
        glyphInfo.advance_x = data.face->glyph->advance.x >> 6; // convertion to px
        charcode = FT_Get_Next_Char(data.face, charcode, &glyph_index);
        i++;
    }
    rasterize(data);
}

void FontSample::rasterize(FontData& data) {
    AtlasBitmap& bitmap = data.bitmap;
    AtlasDescriptor atlasDesc(2048, 2048, 2, 2);

    std::vector<Texture2D> textures;
    textures.reserve(charset.size());
    uint32_t i = 0;
    for(auto& [charcode, glyph_index] : charset) {
        if(FT_Load_Char(data.face, charcode, FT_LOAD_RENDER | FT_LOAD_NO_HINTING)) continue;
        GlyphInfo& glyph = data.infoGlyphs[i];
        FT_GlyphSlot g = data.face->glyph;

        glyph.bearing_x = g->bitmap_left;
        glyph.bearing_y = g->bitmap_top;

        textures.emplace_back(g);

        atlasDesc.insert(&textures.back());
        i++;
    }
    atlasDesc.packAll();
    bitmap.create(atlasDesc);
}

void FontSample::loadCharset() {
    FT_Face face = nullptr;
    if(FT_New_Memory_Face(library, fontBuffer.data(), fontBuffer.size(), 0, &face)) 
    {
        throw std::runtime_error("Failed to load font from memory buffer.");
    }

    FT_UInt glyph_index;
    FT_ULong charcode = FT_Get_First_Char(face, &glyph_index);

    while (glyph_index != 0) {
        uint32_t index = static_cast<uint32_t>(charset.size());
        
        charset.push_back({charcode, glyph_index}), 
        charind.emplace(charcode, index);
        charcode = FT_Get_Next_Char(face, charcode, &glyph_index);
    }
    FT_Done_Face(face);
}

uint32_t FontSample::getGlyphIndex(char32_t character) {
    return charset[charind[character]].second;
}

uint32_t FontSample::getCharIndex(char32_t character) {
    auto it = charind.find(character);
    if (it == charind.end()) {
        return 0; // fallback индекс для неизвестного символа
    }
    return it->second;
}

/*
Texture2D::Texture2D(FT_GlyphSlot g)
{
    width = g->bitmap.width;
    height = g->bitmap.rows;
    channels = 1;
    pixels = new uint8_t[width * height];

    for (int y = 0; y < height; ++y)
        memcpy(pixels + y * width, g->bitmap.buffer + y * g->bitmap.pitch, width);
}
*/

// FONT HANDLER //

FontHandler::FontHandler() {
    initLibrary();
    createSample(defaultFontName, defaultFontPath);
}

FontHandler::~FontHandler() {
    FT_Done_FreeType(library);
}

void FontHandler::initLibrary() {
    if (FT_Init_FreeType( &library ))
    {
        throw std::runtime_error("Failed to init font library");
    }
}

FontSample* FontHandler::createSample(std::string_view fontname, std::string_view path) {
    auto sample = std::make_unique<FontSample>(library, path);
    auto ptr = sample.get();
    fontMap.emplace(fontname, std::move(sample));
    return ptr;
}

FontSample* FontHandler::getSample(const std::string &fontname) {
    if(fontMap.find(fontname) != fontMap.end()) {
        return fontMap[fontname].get();
    } else {
        std::cerr << fontname << " not found in fonts, returned default font" << std::endl;
        return fontMap[defaultFontName].get();
    }
}
