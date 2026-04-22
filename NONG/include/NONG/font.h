#ifndef NONG_FONT_H
#define NONG_FONT_H

#include "NONG/texture2d.h"
#include "stb_truetype.h"
#include <string>

namespace NONG {
    class Font {
    private:
        Texture2D* atlasTexture;
        stbtt_bakedchar charData[96] = {};
        int atlasWidth;
        int atlasHeight;

        void HandleBinary(unsigned char* ttfBuffer, float size);

    public:
        // size: The pixel height of the font (e.g., 32.0f)
        Font(const std::string& ttfFilePath, float size);
        Font(unsigned char* byteArray, int arraySize, float size);
        ~Font();

        Texture2D* GetAtlas() const { return atlasTexture; }
        const stbtt_bakedchar* GetCharData() const { return charData; }
        int GetAtlasWidth() const { return atlasWidth; }   
        int GetAtlasHeight() const { return atlasHeight; } 
    };

    namespace EmbeddedFonts {
        extern const unsigned char openSansBytes[];
        extern const int openSansLength;
        
        extern const unsigned char arialBytes[];
        extern const int arialLength;
    }
}
#endif