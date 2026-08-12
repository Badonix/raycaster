#pragma once

class Player{
  public:
    double posY = 12;
    double posX = 22;
    double dirX = -1;
    double dirY = 0;
    double planeX = 0;
    double planeY = 0.66;
    void move(double distance, const int map[24][24]);
    void rotate(double angle);
};
