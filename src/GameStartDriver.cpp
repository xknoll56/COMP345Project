#include "GameEngine.h"

//#define GAME_START_DRIVER
#ifdef GAME_START_DRIVER

int main() {
  GameEngine engine = GameEngine();
  engine.Init();
  engine.StartupPhase();

  // return Territories of each player 
  for (int i = 0; i < engine.getPlayers().size(); i++) {
      std::cout << "Player " << engine.getPlayers().at(i)->GetName() << " owns the following territories: \n";
      for (Territory* terr : *engine.getPlayers().at(i)->GetOwnedTerritories())
          std::cout << *terr->GetName() << std::endl;

      std::cout << std::endl;
  }

  // return reinforcement pool count of each player 
  for (int i = 0; i < engine.getPlayers().size(); i++) {
      std::cout << engine.getPlayers().at(i)->GetReinforcementPoolCount();
  }


}

#endif