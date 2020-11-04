#include "GameEngine.h"

//#define STARTUP_PHASE_DRIVER
#ifdef STARTUP_PHASE_DRIVER

int main() {
  GameEngine engine = GameEngine();
  engine.Init();
  engine.StartupPhase();
}

#endif