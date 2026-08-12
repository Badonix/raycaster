#include <SDL2/SDL.h>
#include "sdlrenderer.hpp"
#include "game.hpp"


int main(int argc, char* argv[]) {
  SdlRenderer renderer;
  Game game(renderer);
  if(!game.init()){
    return 1;
  }
  game.run();
}
