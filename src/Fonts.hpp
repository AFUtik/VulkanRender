#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Atlas.hpp"

class FontSample {
    FT_Face face;

    friend class FontHandler;
public:
    FontSample();
    ~FontSample();
};

class Font {
    AtlasBitmap atlasBitmap;

    float scale_ = 1.0f;
    uint32_t px_h;
public:
    FontSample *sample;

    Font() {};

    void setPixelSize(uint32_t px_h);
    void scale(float value);
};


class Text {
    Font* font;
    int pen_x = 0;
    int pen_y = 0;

    std::string content;
public:
    
};

class FontHandler {
private:
    std::unordered_map<std::string, std::unique_ptr<FontSample>> fontMap;
    FT_Library library;

    std::string defaultFontPath = ""; 
    std::string defaultFontName = "";

    void initLibrary();
public:
    FontHandler();
    ~FontHandler();

    // void reload(); 

    void loadFromDisk(const std::string &fontname, const std::string &path);
    void loadFromMemory(const std::string &fontname, const std::vector<uint8_t> buffer);

    FontSample* getFontSample(const std::string& fontname);
};