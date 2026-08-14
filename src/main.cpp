#include "game.hpp"
#include "sdlrenderer.hpp"
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  SdlRenderer renderer;
  Texture textures;
  textures.load_from_files({
    "assets/wolfenstein/eagle.png",
    "assets/wolfenstein/redbrick.png",
    "assets/wolfenstein/purplestone.png",
    "assets/wolfenstein/greystone.png",
    "assets/wolfenstein/bluestone.png",
    "assets/wolfenstein/mossy.png",
    "assets/wolfenstein/wood.png",
    "assets/wolfenstein/colorstone.png"
  });
  Game game(renderer, textures);
  if (!game.init()) {
    return 1;
  }
  game.run();
}
