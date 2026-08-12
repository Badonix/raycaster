#pragma once
#include "irenderer.hpp"
#include "player.hpp"

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


  public:
    explicit Game(IRenderer& renderer);
    bool init();
    void run();
};
