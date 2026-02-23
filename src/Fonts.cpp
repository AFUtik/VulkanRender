#include "Fonts.hpp"
#include "freetype/freetype.h"
#include <stdexcept>

#include <ft2build.h>

namespace myvk 
{

FontHandler::FontHandler() {
    initLibrary();
}

void FontHandler::initLibrary() {
    if (FT_Init_FreeType( &library ))
    {
        throw std::runtime_error("Failed to init font library");
    }
}

void FontHandler::loadFromDisk(const std::string &fontname, const std::string &path) {
    FT_Face& face = facesMap[fontname];
    FT_Error error = FT_New_Face(library, path.c_str(), 0, &face);

    if ( error == FT_Err_Unknown_File_Format )
    {
        throw std::runtime_error("Can't read font, unknown format");
    }
    else if ( error )
    {
        throw std::runtime_error("Failed to read font face");
    }
}

void FontHandler::loadFromMemory(const std::string &fontname, const std::vector<uint8_t> buffer) {
    FT_Face& face = facesMap[fontname];
    if(FT_New_Memory_Face(library, buffer.data(), buffer.size(), 0, &face)) 
    {
        throw std::runtime_error("Failed to load memory face");
    }
}

}