#include "SdlRenderer.hpp"
#include <algorithm>

bool SdlRenderer::init(int width, int height, const char* title) {
    this->width = width;
    this->height = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, 0
    );
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );
    if (!texture) return false;

    pixel_buffer.resize(width * height, 0);
    return true;
}

void SdlRenderer::clear(uint32_t color) {
    std::fill(pixel_buffer.begin(), pixel_buffer.end(), color);
}

void SdlRenderer::draw_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixel_buffer[y * width + x] = color;
    }
}

void SdlRenderer::present() {
    SDL_UpdateTexture(texture, nullptr, pixel_buffer.data(), width * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void SdlRenderer::cleanup() {
    if (texture)  { SDL_DestroyTexture(texture);  texture = nullptr; }
    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (window)   { SDL_DestroyWindow(window);     window = nullptr; }
    SDL_Quit();
}

int SdlRenderer::get_width() const {
  return width;
}

int SdlRenderer::get_height() const {
  return height;
}
