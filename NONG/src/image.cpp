#include "NONG/image.h"

#include <stdexcept>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace NONG {
    Image::Image(const unsigned char* byteArray, const int arraySize)
    {
        cpuPixels = stbi_load_from_memory(byteArray, arraySize, &width, &height, &channels, 4);

        if(!cpuPixels)
        {
            throw std::runtime_error("Could not load image info from byte array.");
        }
    }

    Image::Image(const std::string& path) 
    {
        cpuPixels = stbi_load(path.c_str(), &width, &height, &channels, 4);
        if(!cpuPixels) 
        {
            throw std::runtime_error("Could not load image info from file " + path + ".");
        }
    }

    Image::Image(const std::vector<Color>& pixels, const int width, const int height) : width(width), height(height), channels(4)
    {
        int size = width * height * channels;

        cpuPixels = (unsigned char*)std::malloc(size);

        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                int i = y * (width * 4) + (x * 4);
                int j = y * width + x;
                cpuPixels[i + 0] = pixels[j].r * 255;
                cpuPixels[i + 1] = pixels[j].g * 255;
                cpuPixels[i + 2] = pixels[j].b * 255;
                cpuPixels[i + 3] = pixels[j].a * 255;
            }
        }
    }

    int Image::getWidth() const { return width; }
    int Image::getHeight() const { return height; }
    int Image::getChannels() const { return channels; }
    unsigned char* Image::getCpuPixels() const { return cpuPixels; }

    Image::~Image()
    {
        stbi_image_free(cpuPixels);
    }
}