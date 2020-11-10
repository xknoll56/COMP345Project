#include "GameEngine.h"

#define GAME_START_DRIVER
#ifdef GAME_START_DRIVER

int main() {
  GameEngine engine = GameEngine();
  engine.Init();
  engine.StartupPhase();

  // return Territories of each player 
  for (int i = 0; i < engine.getPlayers().size(); i++) {
      std::cout << engine.getPlayers().at(i)->GetOwnedTerritories();
  }

  // return reinforcement pool count of each player 
  for (int i = 0; i < engine.getPlayers().size(); i++) {
      std::cout << engine.getPlayers().at(i)->GetReinforcementPoolCount();
  }


}

#endif