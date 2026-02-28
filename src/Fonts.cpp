#include "Fonts.hpp"
#include "Atlas.hpp"
#include "freetype/freetype.h"
#include "freetype/ftimage.h"
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
    sample_->loadGlyphs(info_);
    sample_->toBitmap(info_, atlasBitmap);
}

// TEXT //

Text::Text(Font* font, std::u32string content) : font(font), content(content) {

}



// FONT SAMPLE //

FontSample::FontSample(FT_Face face) : face(face) {
    loadCharset();
}

FontSample::~FontSample() {

}

void FontSample::loadCharset() {
    FT_UInt glyph_index;
    FT_ULong charcode = FT_Get_First_Char(face, &glyph_index);

    while (glyph_index != 0) {
        uint32_t index = static_cast<uint32_t>(charset.size());

        charset.push_back({charcode, glyph_index}), 
        charind.emplace(charcode, index);
        charcode = FT_Get_Next_Char(face, charcode, &glyph_index);
    }
}

void FontSample::loadGlyphs(FontInfo& fontInfo) {
    FT_Set_Pixel_Sizes(face, 0, fontInfo.pxHeight);
    for(int i = 0; i < charset.size(); i++) {
        uint32_t glyph_index = charset[i].second;

        FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING);
        fontInfo.infoGlyphs.emplace_back(
            face->glyph->advance.x >> 6,
            face->glyph->advance.y >> 6
        );
    }
}

int FontSample::getKerning(const FontInfo& fontInfo, char32_t prevChar, char32_t nextChar) {
    FT_Set_Pixel_Sizes(face, 0, fontInfo.pxHeight);

    FT_Vector delta;
    FT_Get_Kerning(
        face, 
        charset[getCharIndex(prevChar)].second, 
        charset[getCharIndex(nextChar)].second, 
        FT_KERNING_DEFAULT, 
        &delta
    );

    return delta.x >> 6; // to px
}
 
uint32_t FontSample::getCharIndex(char32_t character) {
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

    for(auto& [charcode, glyph_index] : charset) {
        if(FT_Load_Char(face, charcode, FT_LOAD_RENDER)) continue;
    
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
