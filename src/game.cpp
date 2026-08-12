#include "game.hpp"
#include "SDL2/SDL.h"
#include <cmath>

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

Ray Game::create_ray(int x, int screen_width) const {
  Ray ray;
  double cameraX = 2.0 * x / double(screen_width) - 1.0;

  ray.rayX = cameraX * player.planeX + player.dirX;
  ray.rayY = cameraX * player.planeY + player.dirY;

  ray.mapX = static_cast<int>(player.posX);
  ray.mapY = static_cast<int>(player.posY);

  ray.deltaDistX = (ray.rayX == 0) ? 1e30 : std::abs(1.0 / ray.rayX);
  ray.deltaDistY = (ray.rayY == 0) ? 1e30 : std::abs(1.0 / ray.rayY);

  if (ray.rayX < 0) {
      ray.stepX = -1;
      ray.sideDistX = (player.posX - ray.mapX) * ray.deltaDistX;
  } else {
      ray.stepX = 1;
      ray.sideDistX = (ray.mapX + 1.0 - player.posX) * ray.deltaDistX;
  }

  if (ray.rayY < 0) {
      ray.stepY = -1;
      ray.sideDistY = (player.posY - ray.mapY) * ray.deltaDistY;
  } else {
      ray.stepY = 1;
      ray.sideDistY = (ray.mapY + 1.0 - player.posY) * ray.deltaDistY;
  }

  return ray;
}

RayHit Game::perform_dda(Ray ray) const {
  RayHit hit;
  int is_hit = 0;

  while (is_hit == 0) {
      if (ray.sideDistX < ray.sideDistY) {
          ray.sideDistX += ray.deltaDistX;
          ray.mapX += ray.stepX;
          hit.side = 0;
      } else {
          ray.sideDistY += ray.deltaDistY;
          ray.mapY += ray.stepY;
          hit.side = 1;
      }
      if (map[ray.mapX][ray.mapY] > 0) {
          is_hit = 1;
      }
  }

  hit.mapX = ray.mapX;
  hit.mapY = ray.mapY;
  hit.sideDistX = ray.sideDistX;
  hit.sideDistY = ray.sideDistY;
  hit.deltaDistX = ray.deltaDistX;
  hit.deltaDistY = ray.deltaDistY;

  return hit;
}

ColumnProjection Game::calculate_projection(const RayHit& hit, int screen_height) const {
  ColumnProjection proj;

  if (hit.side == 0) {
      proj.perpWallDist = (hit.sideDistX - hit.deltaDistX);
  } else {
      proj.perpWallDist = (hit.sideDistY - hit.deltaDistY);
  }

  proj.lineHeight = static_cast<int>(screen_height / proj.perpWallDist);
  proj.drawStart = -proj.lineHeight / 2 + screen_height / 2;
  if (proj.drawStart < 0) proj.drawStart = 0;

  proj.drawEnd = proj.lineHeight / 2 + screen_height / 2;
  if (proj.drawEnd >= screen_height) proj.drawEnd = screen_height - 1;

  return proj;
}

uint32_t Game::select_wall_color(int tile_type, int side) const {
  uint32_t color;
  switch (tile_type) {
      case 1:  color = 0xFFFF0000; break;
      case 2:  color = 0xFF0000FF; break;
      case 3:  color = 0xFF00FF00; break;
      case 4:  color = 0xFFFFFFFF; break;
      default: color = 0xFF2504A4; break;
  }

  if (side == 1) {
      color = (color - 0xFF000000) / 2 + 0xFF000000;
  }

  return color;
}

void Game::draw_column(int x, const ColumnProjection& proj, uint32_t color) {
  for (int i = proj.drawStart; i < proj.drawEnd; i++) {
      renderer.draw_pixel(x, i, color);
  }
}

void Game::render_frame() {
  renderer.clear(0xFF000000);
  int w = renderer.get_width();
  int h = renderer.get_height();

  for (int x = 0; x < w; x++) {
      Ray ray = create_ray(x, w);
      RayHit hit = perform_dda(ray);
      ColumnProjection proj = calculate_projection(hit, h);
      uint32_t color = select_wall_color(map[hit.mapX][hit.mapY], hit.side);
      draw_column(x, proj, color);
  }

  renderer.present();
}
