#pragma once
#include "irenderer.hpp"
#include "player.hpp"

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
};

struct ColumnProjection {
    double perpWallDist;
    int lineHeight;
    int drawStart;
    int drawEnd;
};

class Game{
  private:
    IRenderer& renderer;
    bool is_running = false;
    Player player;
    static constexpr int MAP_WIDTH = 24;
    static constexpr int MAP_HEIGHT = 24;
    static const int map[MAP_WIDTH][MAP_HEIGHT];
    void process_input(double frame_time);
    void render_frame();

    Ray create_ray(int x, int screen_width) const;
    RayHit perform_dda(Ray ray) const;
    ColumnProjection calculate_projection(const RayHit& hit, int screen_height) const;
    uint32_t select_wall_color(int tile_type, int side) const;
    void draw_column(int x, const ColumnProjection& proj, uint32_t color);

  public:
    explicit Game(IRenderer& renderer);
    bool init();
    void run();
};
