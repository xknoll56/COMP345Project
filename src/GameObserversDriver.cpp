#include "GameEngine.h"

//#define GO_DRIVER
#ifdef GO_DRIVER

int main() {
	GameEngine engine = GameEngine();
	engine.Init();
	engine.StartupPhase();
	engine.MainGameLoop();
}
#endif