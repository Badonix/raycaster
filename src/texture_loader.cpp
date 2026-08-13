#define STB_IMAGE_IMPLEMENTATION
#include "texture_loader.hpp"
#include "stb_image.h"
#include <iostream>

bool TextureLoader::load_png(const std::string &filepath,
                             std::vector<uint32_t> &out_pixels, int &out_width,
                             int &out_height) {
  int channels;
  unsigned char *data =
      stbi_load(filepath.c_str(), &out_width, &out_height, &channels, 4);
  if (!data) {
    std::cerr << "Failed to load texture image: " << filepath << " - "
              << stbi_failure_reason() << std::endl;
    return false;
  }

  out_pixels.resize(out_width * out_height);
  for (int i = 0; i < out_width * out_height; i++) {
    uint8_t r = data[i * 4 + 0];
    uint8_t g = data[i * 4 + 1];
    uint8_t b = data[i * 4 + 2];
    uint8_t a = data[i * 4 + 3];

    out_pixels[i] =
        (static_cast<uint32_t>(a) << 24) | (static_cast<uint32_t>(r) << 16) |
        (static_cast<uint32_t>(g) << 8) | (static_cast<uint32_t>(b));
  }

  stbi_image_free(data);
  return true;
}
