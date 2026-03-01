#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Atlas.hpp"

struct GlyphInfo {
    int advance_x = 0;
    int bearing_x = 0;
    int bearing_y = 0;
};

struct FontData {
    FT_Face face = nullptr;
    AtlasBitmap bitmap;

    uint32_t pxHeight = 32;
    float scale = 1.0f;
    int baseline_y = 0;
    
    std::vector<GlyphInfo> infoGlyphs;
};

class FontSample {
    FT_Library library;
    std::vector<uint8_t> fontBuffer;
    std::vector<std::pair<uint32_t, uint32_t>> charset;
    std::unordered_map<char32_t, uint32_t> charind;

    std::string path;
    std::string familyName;
    std::string style;

    friend class FontHandler;

    void readFont();
    void loadCharset();

public:
    FontSample(FT_Library library, std::string_view path);
    ~FontSample();
    
    void create(FontData& data);
    void rasterize(FontData& data);

    uint32_t getGlyphIndex(char32_t character);
    uint32_t getCharIndex(char32_t character);
};

struct Font {
    FontData fontData;
    FontSample* sample;
    
    Font(FontSample *sample_);
    ~Font();

    FT_Vector getKerning(char32_t prevChar, char32_t nextChar);
};

class Text {
    Font* font;
    std::u32string content;

    uint32_t widthBox;
    uint32_t heightBox;
public:
    Text(Font* font, std::u32string content = U"");
    
    std::u32string& getContent() {return content;}
    Font* getFont() {return font;}
};

class FontHandler {
private:
    std::unordered_map<std::string, std::unique_ptr<FontSample>> fontMap;
    FT_Library library;

    std::string defaultFontPath = "C:/cplusplus/VulkanRender/VulkanRender/resources/fonts/times.ttf"; 
    std::string defaultFontName = "Times";

    void initLibrary();
public:
    FontHandler();
    ~FontHandler();

    // void reload(); 
    FontSample* createSample(std::string_view fontname, std::string_view path);
    FontSample* getSample(const std::string &fontname);
};