#include "player.hpp"
#include <cmath>

void Player::move(double dist, const int map[24][24]){
  if(map[int(posX + dist * dirX)][int(posY)] == 0){
    posX += dist * dirX;
  }
  if(map[int(posX)][int(posY + dist * dirY)] == 0){
    posY += dist * dirY;
  }
}

void Player::rotate(double angle){
  double oldDirX = dirX;
  dirX = dirX * std::cos(angle) - dirY * std::sin(angle);
  dirY = oldDirX * std::sin(angle) + dirY * std::cos(angle);
  double oldPlaneX = planeX;
  planeX = planeX * std::cos(angle) - planeY * std::sin(angle);
  planeY = oldPlaneX * std::sin(angle) + planeY * std::cos(angle);
}
