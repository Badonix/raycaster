#include "texture.hpp"

Texture::Texture(){
  for(int i = 0; i < 8; i++){
    texture[i].resize(tex_width * tex_height);
  }
  generate_random_textures();
}

void Texture::generate_random_textures() {
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
      texture[0][idx] = alpha | red;
      texture[1][idx] = alpha | wood;
      texture[2][idx] = alpha | mask;
      texture[3][idx] = alpha | xorp;
      texture[4][idx] = alpha | xorg;
      texture[5][idx] = alpha | brick;
      texture[6][idx] = alpha | blue;
      texture[7][idx] = alpha | gray;
    }
  }
}

const std::vector<uint32_t>& Texture::get_texture(int i) const {
  if(i < 0 || i >= text_count) return texture[0];
  return texture[i];
}

uint32_t Texture::get_texture_pixel(int i, int x, int y) const {
  if (i < 0 || i >= text_count) i = 0;
  if (x < 0) x = 0; else if (x >= tex_width) x = tex_width - 1;
  if (y < 0) y = 0; else if (y >= tex_height) y = tex_height - 1;
  return texture[i][y * tex_width + x];
}

