#ifndef NONG_COLOR_H
#define NONG_COLOR_H

#include <string>
#include <tuple>
#include <stdint.h>


namespace NONG {
    class Color
    {
        public:
            float r,g,b,a;

        Color();
        Color(float r, float g, float b, float a = 1.0f);
        Color(const Color& color);

        static Color fromHSV(float h, float s, float v, float a = 1.0f);
        static Color fromRGB(float r, float g, float b, float a = 1.0f);
        static Color from8Bit(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        static Color fromHEX(const std::string& hex);

        static Color white();
        static Color black();
        static Color red();
        static Color green();
        static Color blue();
        static Color clear();

        std::tuple<float, float, float> GetHSV() const;
    };
}

#endif