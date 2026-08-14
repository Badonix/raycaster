#pragma once
#include "irenderer.hpp"
#include "player.hpp"
#include "texture.hpp"

struct Ray {
    double rayX;
    double rayY;
    int mapX;
    int mapY;
    double deltaDistX;
    double deltaDistY;
    double sideDistX;
    double sideDistY;
    int stepX;
    int stepY;
};

struct RayHit {
    int mapX;
    int mapY;
    int side;
    double sideDistX;
    double sideDistY;
    double deltaDistX;
    double deltaDistY;
    double perpWallDist;
    int texX;
    int texture;
};

struct ColumnProjection {
    int lineHeight;
    int drawStart;
    int drawEnd;
    int side;
};

class Game{
  private:
    IRenderer& renderer;
    Texture& textures;
    bool is_running = false;
    Player player;
    static constexpr int MAP_WIDTH = 24;
    static constexpr int MAP_HEIGHT = 24;
    static const int map[MAP_WIDTH][MAP_HEIGHT];
    int current_fps = 0;
    void process_input(double frame_time);
    void render_frame();
    void render_floor_and_ceiling();
    void render_walls();

    Ray create_ray(int x, int screen_width) const;
    RayHit perform_dda(Ray ray) const;
    ColumnProjection calculate_projection(const RayHit& hit, int screen_height) const;
    uint32_t select_wall_color(int tile_type, int side) const;
    void draw_column(int x, const ColumnProjection& proj, uint32_t color);
    void draw_texture_column(int x, const ColumnProjection& proj, int texture, int wallX);

  public:
    explicit Game(IRenderer& renderer, Texture& textures);
    bool init();
    void run();
};
