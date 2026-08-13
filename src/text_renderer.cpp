#include "text_renderer.hpp"

static const uint8_t DIGIT_PATTERNS[13][5] = {
    {0b111, 0b101, 0b101, 0b101, 0b111}, {0b010, 0b110, 0b010, 0b010, 0b111},
    {0b111, 0b001, 0b111, 0b100, 0b111}, {0b111, 0b001, 0b111, 0b001, 0b111},
    {0b101, 0b101, 0b111, 0b001, 0b001}, {0b111, 0b100, 0b111, 0b001, 0b111},
    {0b111, 0b100, 0b111, 0b101, 0b111}, {0b111, 0b001, 0b010, 0b010, 0b010},
    {0b111, 0b101, 0b111, 0b101, 0b111}, {0b111, 0b101, 0b111, 0b001, 0b111},
    {0b111, 0b100, 0b110, 0b100, 0b100}, {0b111, 0b101, 0b111, 0b100, 0b100},
    {0b111, 0b100, 0b111, 0b001, 0b111}};

void TextRenderer::draw_digit(IRenderer &renderer, int index, int startX,
                              int startY, int scale, uint32_t color) {
  if (index < 0 || index > 12)
    return;

  for (int r = 0; r < 5; r++) {
    uint8_t rowBits = DIGIT_PATTERNS[index][r];
    for (int c = 0; c < 3; c++) {
      if (rowBits & (1 << (2 - c))) {
        for (int dy = 0; dy < scale; dy++) {
          for (int dx = 0; dx < scale; dx++) {
            renderer.draw_pixel(startX + c * scale + dx,
                                startY + r * scale + dy, color);
          }
        }
      }
    }
  }
}

void TextRenderer::draw_string(IRenderer &renderer, const std::string &text,
                               int startX, int startY, int scale,
                               uint32_t color) {
  int curX = startX;
  for (char ch : text) {
    int idx = -1;
    if (ch >= '0' && ch <= '9') {
      idx = ch - '0';
    } else if (ch == 'F' || ch == 'f') {
      idx = 10;
    } else if (ch == 'P' || ch == 'p') {
      idx = 11;
    } else if (ch == 'S' || ch == 's') {
      idx = 12;
    }

    if (idx >= 0) {
      draw_digit(renderer, idx, curX, startY, scale, color);
    }
    curX += (3 + 1) * scale;
  }
}
