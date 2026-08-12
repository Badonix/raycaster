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
    if (state[SDL_SCANCODE_S]) player.move(-move_speed, map);
    if (state[SDL_SCANCODE_A]) player.rotate(rot_speed);
    if (state[SDL_SCANCODE_D]) player.rotate(-rot_speed);
}
void Game::render_frame() {
    renderer.clear(0xFF000000);
    int w = renderer.get_width();
    int h = renderer.get_height();
    for(int x = 0; x < w; x++){
      double cameraX = 2 * x / double(w) - 1;

      double rayX = cameraX * player.planeX + player.dirX;
      double rayY = cameraX * player.planeY + player.dirY;

      int mapX = int(player.posX);
      int mapY = int(player.posY);

      double sideDistX;
      double sideDistY;

      double deltaDistX = (rayX == 0) ? 1e30 : abs(1 / rayX);
      double deltaDistY = (rayY == 0) ? 1e30 : abs(1 / rayY);

      int stepX;
      int stepY;

      if (rayX < 0){
        stepX = -1;
        sideDistX = (player.posX - mapX) * deltaDistX;
      } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
      }

      if (rayY < 0){
        stepY = -1;
        sideDistY = (player.posY - mapY) * deltaDistY;
      }else{
        stepY = 1;
        sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
      }     

      double perpWallDist;
      int hit = 0;
      int side;
      while (hit == 0)
      {
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        if (map[mapX][mapY] > 0) hit = 1;
      } 

      if(side == 0){
        perpWallDist = (sideDistX - deltaDistX);
      } else{
        perpWallDist = (sideDistY - deltaDistY);
      }

      int lineHeight = (int)(h / perpWallDist);
      int drawStart = -lineHeight / 2 + h / 2;
      if(drawStart < 0) drawStart = 0;
      int drawEnd = lineHeight / 2 + h / 2;
      if(drawEnd >= h) drawEnd = h - 1;

      uint32_t color;
      switch(map[mapX][mapY])
      {
        case 1:  color = 0xFFFF0000;  break;
        case 2:  color = 0xFF0000FF;  break;
        case 3:  color = 0xFF00FF00;   break;
        case 4:  color = 0xFFFFFFFF;  break;
        default: color = 0xFF2504A4; break;
      }

      if (side == 1){
        color = (color - 0xFF000000) / 2 + 0xFF000000;
      }

      for(int i = drawStart; i < drawEnd; i++){
        renderer.draw_pixel(x, i, color);
      }

    }
    renderer.present();
}
