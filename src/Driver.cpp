#include "GameEngine.h"

#define DRIVER
#ifdef DRIVER

int main() {
  GameEngine engine = GameEngine();
  engine.Init();
  engine.StartupPhase();
  engine.MainGameLoop();
}
#endif