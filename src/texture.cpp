#include "texture.hpp"
#include "texture_loader.hpp"
#include <iostream>

Texture::Texture(){
  generate_random_textures();
}

int Texture::load_texture(const std::string& filepath) {
  int w = 0, h = 0;
  std::vector<uint32_t> pixels;
  if (TextureLoader::load_png(filepath, pixels, w, h)) {
    if (w == tex_width && h == tex_height) {
      textures.push_back(std::move(pixels));
      return static_cast<int>(textures.size()) - 1;
    } else {
      std::cerr << "Texture dimension mismatch for " << filepath << ": expected " << tex_width << "x" << tex_height << " but got " << w << "x" << h << std::endl;
    }
  }
  return -1;
}

bool Texture::load_from_files(const std::vector<std::string>& filepaths) {
  textures.clear();
  textures.reserve(filepaths.size());
  bool all_loaded = true;
  for (const auto& path : filepaths) {
    if (load_texture(path) == -1) {
      all_loaded = false;
    }
  }
  if (textures.empty()) {
    generate_random_textures();
  }
  return all_loaded;
}

void Texture::generate_random_textures() {
  textures.clear();
  textures.resize(8);
  for (int i = 0; i < 8; i++) {
    textures[i].resize(tex_width * tex_height);
  }

  for (int x = 0; x < tex_width; x++) {
    for (int y = 0; y < tex_height; y++) {
      int xorcolor = (x * 256 / tex_width) ^ (y * 256 / tex_height);
      int ycolor = y * 256 / tex_height;
      int xycolor = y * 128 / tex_height + x * 128 / tex_width;
      uint32_t alpha = 0xFF000000;
      uint32_t red   = (65536 * 254 * (x != y && x != tex_width - y));
      uint32_t wood  = (xycolor + 256 * xycolor + 65536 * xycolor);
      uint32_t mask  = (256 * xycolor + 65536 * xycolor);
      uint32_t xorp  = (xorcolor + 256 * xorcolor + 65536 * xorcolor);
      uint32_t xorg  = (256 * xorcolor);
      uint32_t brick = (65536 * 192 * (x % 16 && y % 16));
      uint32_t blue  = (65536 * ycolor);
      uint32_t gray  = (128 + 256 * 128 + 65536 * 128);
      int idx = tex_width * y + x;
      textures[0][idx] = alpha | red;
      textures[1][idx] = alpha | wood;
      textures[2][idx] = alpha | mask;
      textures[3][idx] = alpha | xorp;
      textures[4][idx] = alpha | xorg;
      textures[5][idx] = alpha | brick;
      textures[6][idx] = alpha | blue;
      textures[7][idx] = alpha | gray;
    }
  }
}

int Texture::get_count() const {
  return static_cast<int>(textures.size());
}

const std::vector<uint32_t>& Texture::get_texture(int i) const {
  if (i < 0 || i >= static_cast<int>(textures.size())) return textures[0];
  return textures[i];
}

uint32_t Texture::get_texture_pixel(int i, int x, int y) const {
  if (textures.empty()) return 0xFF000000;
  if (i < 0 || i >= static_cast<int>(textures.size())) i = 0;
  if (x < 0) x = 0; else if (x >= tex_width) x = tex_width - 1;
  if (y < 0) y = 0; else if (y >= tex_height) y = tex_height - 1;
  return textures[i][y * tex_width + x];
}

