#include "game.hpp"
#include "SDL2/SDL.h"
#include "text_renderer.hpp"
#include <cmath>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

const int Game::map[24][24] = {
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 4, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 0, 7, 7, 7, 7, 7},
    {4, 0, 5, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1},
    {4, 0, 6, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8},
    {4, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 1},
    {4, 0, 8, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8},
    {4, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1},
    {4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 1},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 6, 0, 6, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2},
    {4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 2},
    {4, 0, 0, 5, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2},
    {4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3}};

Game::Game(IRenderer &r, Texture &t) : renderer(r), textures(t) {}

bool Game::init() {
  return renderer.init(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster");
}

void Game::run() {
  is_running = true;
  Uint64 old_time = SDL_GetTicks64();
  Uint64 fps_timer = old_time;
  int frame_count = 0;

  while (is_running) {
    Uint64 curr_time = SDL_GetTicks64();
    double frame_time = (curr_time - old_time) / 1000.0;
    old_time = curr_time;
    frame_count++;

    if (curr_time - fps_timer >= 1000) {
      current_fps = frame_count;
      frame_count = 0;
      fps_timer = curr_time;
    }

    process_input(frame_time);
    render_frame();
  }
}

void Game::process_input(double frame_time) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      is_running = false;
  }
  const Uint8 *state = SDL_GetKeyboardState(nullptr);
  if (state[SDL_SCANCODE_ESCAPE])
    is_running = false;
  double move_speed = frame_time * 5.0;
  double rot_speed = frame_time * 3.0;
  if (state[SDL_SCANCODE_W])
    player.move(move_speed, map);
  if (state[SDL_SCANCODE_S])
    player.move(-move_speed, map);
  if (state[SDL_SCANCODE_A])
    player.rotate(rot_speed);
  if (state[SDL_SCANCODE_D])
    player.rotate(-rot_speed);
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

  if (hit.side == 0) {
    hit.perpWallDist = (hit.sideDistX - hit.deltaDistX);
  } else {
    hit.perpWallDist = (hit.sideDistY - hit.deltaDistY);
  }

  double wallX;
  if (hit.side == 0) {
    wallX = player.posY + hit.perpWallDist * ray.rayY;
  } else {
    wallX = player.posX + hit.perpWallDist * ray.rayX;
  }
  wallX -= floor(wallX);

  int texX = int(wallX * double(textures.tex_width));
  if (hit.side == 0 && ray.rayX > 0) {
    texX = textures.tex_width - texX - 1;
  }
  if (hit.side == 1 && ray.rayY < 0) {
    texX = textures.tex_width - texX - 1;
  }

  hit.texX = texX;
  hit.texture = map[hit.mapX][hit.mapY] - 1;
  return hit;
}

ColumnProjection Game::calculate_projection(const RayHit &hit,
                                            int screen_height) const {
  ColumnProjection proj;

  proj.lineHeight = static_cast<int>(screen_height / hit.perpWallDist);
  proj.drawStart = -proj.lineHeight / 2 + screen_height / 2;
  if (proj.drawStart < 0)
    proj.drawStart = 0;

  proj.drawEnd = proj.lineHeight / 2 + screen_height / 2;
  if (proj.drawEnd >= screen_height)
    proj.drawEnd = screen_height - 1;
  proj.side = hit.side;

  return proj;
}

uint32_t Game::select_wall_color(int tile_type, int side) const {
  uint32_t color;
  switch (tile_type) {
  case 1:
    color = 0xFFFF0000;
    break;
  case 2:
    color = 0xFF0000FF;
    break;
  case 3:
    color = 0xFF00FF00;
    break;
  case 4:
    color = 0xFFFFFFFF;
    break;
  default:
    color = 0xFF2504A4;
    break;
  }

  if (side == 1) {
    color = (color - 0xFF000000) / 2 + 0xFF000000;
  }

  return color;
}

void Game::draw_texture_column(int x, const ColumnProjection &proj, int tx,
                               int texX) {
  double step = 1.0 * textures.tex_height / proj.lineHeight;
  double texPos =
      (proj.drawStart - renderer.get_height() / 2.0 + proj.lineHeight / 2.0) *
      step;
  for (int y = proj.drawStart; y < proj.drawEnd; y++) {
    int texY = (int)texPos & (textures.tex_height - 1);
    texPos += step;
    Uint32 color = textures.get_texture_pixel(tx, texX, texY);
    if (proj.side == 1)
      color = ((color >> 1) & 0x7F7F7F) | 0xFF000000;
    renderer.draw_pixel(x, y, color);
  }
}

void Game::draw_column(int x, const ColumnProjection &proj, uint32_t color) {
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
    // uint32_t color = select_wall_color(map[hit.mapX][hit.mapY], hit.side);
    // draw_column(x, proj, color);
    draw_texture_column(x, proj, hit.texture, hit.texX);
  }

  std::string fps_text = "FPS " + std::to_string(current_fps);
  TextRenderer::draw_string(renderer, fps_text, 10, 10, 2,
                            0xFF00FF00);

  renderer.present();
}
