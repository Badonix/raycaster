#pragma once
#include <string>
#include <vector>
#include <cstdint>

class TextureLoader {
public:
    static bool load_png(const std::string& filepath, std::vector<uint32_t>& out_pixels, int& out_width, int& out_height);
};
