#pragma once
#include <cstdint>

class IRenderer{
  public:
    virtual ~IRenderer() = default;

    virtual bool init(int w, int h, const char* title) = 0;
    virtual void clear(uint32_t color) = 0;
    virtual void draw_pixel(int x, int y, uint32_t color) = 0;
    virtual void present() = 0;
    virtual void cleanup() = 0;

    virtual int get_width() const = 0;
    virtual int get_height() const = 0;
};
