#include "game.hpp"
#include "SDL2/SDL.h"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const int Game::map[24][24] = 
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

Game::Game(IRenderer& r) : renderer(r) {}

bool Game::init(){
  return renderer.init(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster");
}

void Game::run(){
  is_running = true;
  Uint64 old_time = SDL_GetTicks64();

  while(is_running){
    Uint64 curr_time = SDL_GetTicks64();
    double frame_time = (curr_time - old_time) / 1000.0;
    old_time = curr_time;
    process_input(frame_time);
    render_frame();
  }
}

void Game::process_input(double frame_time){
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) is_running = false;
    }
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_ESCAPE]) is_running = false;
    double move_speed = frame_time * 5.0;
    double rot_speed  = frame_time * 3.0;
    if (state[SDL_SCANCODE_W]) player.move(move_speed, map);
    if (state[SDL_SCANCODE_S]) player.move(move_speed, map);
    if (state[SDL_SCANCODE_A]) player.rotate(rot_speed);
    if (state[SDL_SCANCODE_D]) player.rotate(-rot_speed);
}
void Game::render_frame() {
    renderer.clear(0xFF222222);
    uint32_t whiteColor = 0xFFFFFFFF;
    for (int y = 100; y < 150; ++y) {
        for (int x = 100; x < 150; ++x) {
            renderer.draw_pixel(x, y, whiteColor);
        }
    }
    renderer.present();
}
