#pragma once
#include <vector>

#include "Player.h"
#include "MapLoader.h"
#include "Cards.h"
#include "Orders.h"
#include "Map.h"

class GameEngine {
 private:
  Map* map;
  std::vector<Player*> players;  // Keep these in order of play.
  Deck* deck;
  static Player* neutralPlayer; // Used by blockade order
  GameStatisticsObserver* gameStatsObs;
  PhaseObserver* phaseObs;

 public:
  GameEngine(){};
  ~GameEngine();
  void Init();
  void StartupPhase();
  int MainGameLoop();
  void ReinforcementPhase();
  void IssueOrdersPhase();
  bool ExecuteOrdersPhase();
  void RoundRobin(bool(Player::*func)());
  std::vector<Player*> getPlayers();
  static Player* GetNeutralPlayer();
  Map* GetMap();
  void PlayerDrawCard(Player* player);
};
