#ifndef NONG_IMAGE_H
#define NONG_IMAGE_H

#include <string>
#include <vector>
#include "NONG/color.h"

namespace NONG {
    class Image
    {
    private:
        int width, height, channels;
        unsigned char* cpuPixels;

    public:
        Image(const std::vector<Color>& pixels, const int width, const int height);
        Image(const unsigned char* byteArray, const int arraySize);
        Image(const std::string& path);

        int getWidth() const;
        int getHeight() const;
        int getChannels() const;
        unsigned char* getCpuPixels() const;

        ~Image();
    };
}

#endif