#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Atlas.hpp"

struct GlyphInfo {
    int advance_x;
    int advance_y;
};

struct FontInfo {
    uint32_t pxHeight = 64;
    float scale = 1.0f;
    int ascender;
    int descender;
    int lineGap;

    std::vector<GlyphInfo> infoGlyphs;
};

class FontSample {
    FT_Face face = nullptr;
    std::string path;
    std::string familyName;
    std::string style;

    std::vector<std::pair<uint32_t, uint32_t>> charset;
    std::unordered_map<char32_t, uint32_t> charind;

    friend class FontHandler;

    void loadCharset();
public:
    FontSample(FT_Face face);
    ~FontSample();

    void toBitmap(const FontInfo& fontInfo, AtlasBitmap& bitmap);
    void loadGlyphs(FontInfo& fontInfo);

    int getKerning(const FontInfo& fontInfo, char32_t prevChar, char32_t nextChar);

    uint32_t getCharIndex(char32_t character);
};

class Font {
    AtlasBitmap atlasBitmap;
    FontInfo info_;

    FontSample* sample_;
    
    friend class Text;
public:
    Font(FontSample *sample_);

    FontSample* sample() {return sample_;}    
    AtlasBitmap& bitmap() {return atlasBitmap;};
    FontInfo& info() {return info_;}
    
    void update();
};

class Text {
    Font* font;
    int pen_x = 0;
    int pen_y = 0;
    std::u32string content;
public:
    Text(Font* font, std::u32string content = U"");
    

    std::u32string_view getContent() {return content;}
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

    void loadFromDisk(const std::string &fontname, const std::string &path);
    void loadFromMemory(const std::string &fontname, const std::vector<uint8_t> buffer);

    FontSample* getFontSample(const std::string& fontname);
};