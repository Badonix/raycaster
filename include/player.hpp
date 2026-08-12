#pragma once

class Player{
  private:
    double posX = 22;
    double posY = 12;
    double dirX = -1;
    double dirY = 0;
    double planeX = 0;
    double planeY = 0.66;
  public:
    void move(double distance, const int map[24][24]);
    void rotate(double angle);
};
