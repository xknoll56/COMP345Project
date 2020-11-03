#include "GameEngine.h"

//#define GAME_START_DRIVER
#ifdef GAME_START_DRIVER

int main() {
  GameEngine engine = GameEngine();
  engine.Init();
}

#endif