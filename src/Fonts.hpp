#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <string>
#include <unordered_map>

namespace myvk {

class FontSample {
    FT_Face face;

    friend class FontHandler;
public:

};

class Font {
    FontSample* sample;
    uint32_t px_h;
    
    float scale = 1.0f;
public:

};

class Text {
    Font* font;
    int pen_x = 0;
    int pen_y = 0;
public:
};

class FontHandler {
private:
    std::unordered_map<std::string, std::unique_ptr<FontSample>> fontMap;
    FT_Library library;

    void initLibrary();
public:
    FontHandler();
    ~FontHandler();

    void loadFromDisk(const std::string &fontname, const std::string &path);
    void loadFromMemory(const std::string &fontname, const std::vector<uint8_t> buffer);
};

}