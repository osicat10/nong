#include "NONG/font.h"
#include <fstream>
#include <vector>
#include <stdexcept>

#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb_truetype.h"

#include "NONG/color.h"

namespace NONG {

    void Font::HandleBinary(unsigned char* ttfBuffer, float size)
    {
        atlasWidth = std::max(512, (int)(size * 16)); 
        atlasHeight = std::max(512, (int)(size * 16));

        std::vector<unsigned char> tempBitmap(atlasWidth * atlasHeight);

        // 32 is the ASCII code for Space (the first printable character), 96 is the number of characters.
        stbtt_BakeFontBitmap(ttfBuffer, 0, size, tempBitmap.data(), atlasWidth, atlasHeight, 32, 96, charData);

        // STB gives us a 1-channel (Alpha) image. 
        // Our engine expects a 4-channel (RGBA) image.
        std::vector<Color> rgbaBitmap(atlasWidth * atlasHeight);
        for (int i = 0; i < atlasWidth * atlasHeight; i++) 
        {
            rgbaBitmap[i] = Color::from8Bit(255, 255, 255, tempBitmap[i]);
        }

        Image img(rgbaBitmap, atlasWidth, atlasHeight); 
        atlasTexture = new Texture2D(img);
    }

    Font::Font(const std::string& ttfFilePath, float size) 
    {
        std::ifstream file(ttfFilePath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) throw std::runtime_error("Failed to load font: " + ttfFilePath);
        
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<unsigned char> ttfBuffer(fileSize);
        if (!file.read((char*)ttfBuffer.data(), fileSize)) throw std::runtime_error("Failed to read font file");

        HandleBinary(ttfBuffer.data(), size);
    }

    Font::Font(unsigned char* byteArray, int arraySize, float size)
    {
        HandleBinary(byteArray, size);
    }

    Font::~Font() {
        delete atlasTexture;
    }

    namespace EmbeddedFonts {
        const unsigned char openSansBytes[] = {
            #embed "fonts/OpenSans-regular.ttf"
        };
        const int openSansLength = sizeof(openSansBytes);

        const unsigned char arialBytes[] = {
            #embed "fonts/OpenSans-regular.ttf"
        };
        const int arialLength = sizeof(arialBytes);
    }    
}