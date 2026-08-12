#pragma once
#include "irenderer.hpp"
#include <vector>
#include <SDL2/SDL.h>

class SdlRenderer : public IRenderer{
    private:
      int width;
      int height;
      std::vector<uint32_t> pixel_buffer;
      SDL_Window* window;
      SDL_Renderer* renderer;
      SDL_Texture* texture;

    public:
      ~SdlRenderer() override {cleanup();};

      bool init(int w, int h, const char* title) override;
      void clear(uint32_t color) override;
      void draw_pixel(int x, int y, uint32_t color) override;
      void present() override;
      void cleanup() override;

      int get_width() const override;
      int get_height() const override;
};
