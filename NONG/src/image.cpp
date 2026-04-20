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

    int Image::getWidth() const { return width; }
    int Image::getHeight() const { return height; }
    int Image::getChannels() const { return channels; }
    unsigned char* Image::getCpuPixels() const { return cpuPixels; }

    Image::~Image()
    {
        stbi_image_free(cpuPixels);
    }
}