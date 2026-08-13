#pragma once
#include "irenderer.hpp"
#include <string>

class TextRenderer {
public:
  static void draw_digit(IRenderer &renderer, int digit, int startX, int startY,
                         int scale, uint32_t color);
  static void draw_string(IRenderer &renderer, const std::string &text,
                          int startX, int startY, int scale, uint32_t color);
};
