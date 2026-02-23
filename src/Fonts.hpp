#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <string>
#include <unordered_map>

namespace myvk {

class Font {
    FT_Face face;
    uint32_t px;
public:
    
};

class Text {
    int pen_x = 0;
    int pen_y = 0;
public:
};

class FontHandler {
private:
    std::unordered_map<std::string, std::unique_ptr<Font>> facesMap;
    FT_Library library;

    void initLibrary();
public:
    FontHandler();
    ~FontHandler();

    void loadFromDisk(const std::string &fontname, const std::string &path);
    void loadFromMemory(const std::string &fontname, const std::vector<uint8_t> buffer);
};

}