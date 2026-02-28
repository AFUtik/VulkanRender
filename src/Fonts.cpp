#include "Fonts.hpp"
#include "Atlas.hpp"
#include "freetype/freetype.h"
#include "model/Texture.hpp"
#include <memory>
#include <stdexcept>
#include <iostream>

#include <ft2build.h>


// FONT //

Font::Font(FontSample* sample_) : sample_(sample_) 
{
    update();
}

void Font::update() {
    sample_->toBitmap(info, atlasBitmap);
}

// TEXT //

Text::Text(Font* font, std::u32string content) : font(font), content(content) {

}

// FONT SAMPLE //

FontSample::FontSample(FT_Face face) : face(face) {
    loadCharset();
}

FontSample::~FontSample() {
    if(face) FT_Done_Face(face);
}

void FontSample::loadCharset() {
    FT_UInt glyph_index;
    FT_ULong charcode = FT_Get_First_Char(face, &glyph_index);

    while (glyph_index != 0) {
        charind.emplace(charcode, charset.size());
        charset.push_back(static_cast<uint32_t>(charcode));
        charcode = FT_Get_Next_Char(face, charcode, &glyph_index);
    }
}
 
uint32_t FontSample::getIndex(char32_t character) {
    return charind[character];
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


void FontSample::toBitmap(const FontInfo& fontInfo, AtlasBitmap& bitmap) {
    if (!face) return;
    FT_Set_Pixel_Sizes(face, 0, fontInfo.pxHeight);

    //fontInfo.ascender  = face->size->metrics.ascender >> 6;
    //fontInfo.descender = face->size->metrics.descender >> 6;
    //fontInfo.lineGap   = (face->size->metrics.height >> 6) - (fontInfo.ascender - fontInfo.descender);

    AtlasDescriptor atlasDesc(2048, 2048, 2, 2);
    bitmap.create(atlasDesc);

    for(uint32_t c : charset) {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
    
        FT_GlyphSlot g = face->glyph;
        Texture2D glyphImage(g);

        bitmap.extend(atlasDesc.pack(&glyphImage), &glyphImage);
    }
}

// FONT HANDLER //

FontHandler::FontHandler() {
    initLibrary();
    loadFromDisk(defaultFontName, defaultFontPath);
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

void FontHandler::loadFromDisk(const std::string &fontname, const std::string &path) {
    if(fontMap.find(fontname) != fontMap.end()) {
        std::cerr << fontname << " already loaded." << std::endl;
        return;
    }
    
    FT_Face newFace = nullptr;
    FT_Error error = FT_New_Face(library, path.c_str(), 0, &newFace);    

    if ( error == FT_Err_Unknown_File_Format )
    {
        throw std::runtime_error("Can't read font, unknown format");
    }
    else if ( error )
    {
        throw std::runtime_error("Failed to read font face");
    }
    fontMap.emplace(fontname, std::make_unique<FontSample>(newFace));
}

void FontHandler::loadFromMemory(const std::string &fontname, const std::vector<uint8_t> buffer) {
    if(fontMap.find(fontname) != fontMap.end()) {
        std::cerr << fontname << " already loaded." << std::endl;
        return;
    }

    FT_Face newFace = nullptr;

    if(FT_New_Memory_Face(library, buffer.data(), buffer.size(), 0, &newFace)) 
    {
        throw std::runtime_error("Failed to load memory face");
    }
    fontMap.emplace(fontname, std::make_unique<FontSample>(newFace));
}

FontSample* FontHandler::getFontSample(const std::string& fontname) {
    if(fontMap.find(fontname) != fontMap.end()) {
        return fontMap[fontname].get();
    } else {
        std::cerr << fontname << " not found in fonts, returned default font" << std::endl;
        return fontMap[defaultFontName].get();
    }
}
