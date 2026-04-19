#include "NONG/color.h"

#include <cmath>
#include <algorithm>

namespace NONG {
    Color::Color() : Color(1, 1, 1, 1) { }
    Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
    Color::Color(const Color& color) : r(color.r), g(color.g), b(color.b), a(color.a) { }

    Color Color::fromRGB(float r, float g, float b, float a) {
        return Color(r, g, b, a);
    }

    Color Color::from8Bit(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    Color Color::fromHEX(const std::string& hex) {
        std::string h = hex;
        if (!h.empty() && h[0] == '#') {
            h = h.substr(1);
        }

        // Support both 6-character (RRGGBB) and 8-character (RRGGBBAA) hex codes
        if (h.length() == 6 || h.length() == 8) {
            uint8_t r = std::stoul(h.substr(0, 2), nullptr, 16);
            uint8_t g = std::stoul(h.substr(2, 2), nullptr, 16);
            uint8_t b = std::stoul(h.substr(4, 2), nullptr, 16);
            uint8_t a = (h.length() == 8) ? std::stoul(h.substr(6, 2), nullptr, 16) : 255;
            
            return from8Bit(r, g, b, a);
        }
        
        return Color(1.0f, 1.0f, 1.0f, 1.0f); 
    }

    Color Color::fromHSV(float h, float s, float v, float a) {
        float c = v * s;
        float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
        float m = v - c;

        float r_ = 0, g_ = 0, b_ = 0;
        
        if (h >= 0 && h < 60)      { r_ = c; g_ = x; b_ = 0; }
        else if (h >= 60 && h < 120) { r_ = x; g_ = c; b_ = 0; }
        else if (h >= 120 && h < 180) { r_ = 0; g_ = c; b_ = x; }
        else if (h >= 180 && h < 240) { r_ = 0; g_ = x; b_ = c; }
        else if (h >= 240 && h < 300) { r_ = x; g_ = 0; b_ = c; }
        else if (h >= 300 && h < 360) { r_ = c; g_ = 0; b_ = x; }

        return Color(r_ + m, g_ + m, b_ + m, a);
    }

    Color Color::white() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
    Color Color::black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
    Color Color::red()   { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
    Color Color::green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
    Color Color::blue()  { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
    Color Color::clear() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }

    std::tuple<float, float, float> Color::GetHSV() const {
        float cmax = std::max({r, g, b});
        float cmin = std::min({r, g, b});
        float diff = cmax - cmin;

        float h = 0.0f;
        float s = 0.0f;
        float v = cmax;

        if (cmax > 0.0f) {
            s = diff / cmax;
        }

        if (diff > 0.0f) {
            if (cmax == r) {
                h = std::fmod((g - b) / diff, 6.0f);
            } else if (cmax == g) {
                h = ((b - r) / diff) + 2.0f;
            } else {
                h = ((r - g) / diff) + 4.0f;
            }
            h *= 60.0f;
            if (h < 0.0f) {
                h += 360.0f;
            }
        }

        return std::make_tuple(h, s, v);
    }
}