#include "Player.h"

#include <vector>

class GameEngine {
 private:
  std::vector<Player*> players; // Keep these in order of play.
  Map map;

 public:
  GameEngine();
  int MainGameLoop();
  void ReinforcementPhase();
  void IssueOrdersPhase();
  bool ExecuteOrdersPhase();
  void IssueOrder(Player* player);
};

int GameEngine::MainGameLoop() {
  while (true) {
    ReinforcementPhase();
    IssueOrdersPhase();
    if (ExecuteOrdersPhase()) {
      return 0;
    }
  }
}

void GameEngine::ReinforcementPhase() { 
  std::vector<Continent>* continents;
  for (int i = 0; i < players.size(); i++) {
    // players.at(i)->GenerateReinforcements();
  }
  for (int i = 0; i < continents->size(); i++) {
    // continents->at(i).GivePlayerBonus();
  }
}

void GameEngine::IssueOrdersPhase() { 
    for (int i = 0; i < players.size(); i++) {
      this->IssueOrder(players.at(i));
    }
}

void GameEngine::IssueOrder(Player* player) {
  std::vector<Territory*>* toDefend;
  std::vector<Territory*>* toAttack;
  toDefend = &(player->toDefend());
  toAttack = &(player->toAttack());
  // Prompt the user to play a card if they want to.
  // while (player->GetReinforcements() > 0)  {
  //   prompt the user to add reinforcements to a territories in toDefend.
  // }
  // Prompt the user to play a card if they want to.
  while (true) {
    // Prompt the player to advance troops.
    // When the player chooses to stop issuing orders, return.
    return;
  } 
}

bool GameEngine::ExecuteOrdersPhase() {

}
