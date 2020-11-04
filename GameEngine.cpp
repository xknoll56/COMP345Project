#pragma once

#include "GameEngine.h"

// Part 1: Game Start.
void GameEngine::Init() {
  // 1. Select a map.
  // 2. Select num of Players.
  players.reserve(3);
  // 3. Set Observers (Part 5).
  // 4. Load Map.
  MapLoader* ml = new MapLoader();
  map = ml->GenerateMap("MapFiles/France.map");
  // delete ml; // TODO - WHY IS THIS DELETE CRASHING THE APPLICATION???
  // 5. Create Players.
  players.push_back(new Player());
  players.push_back(new Player());
  players.push_back(new Player());
  // 6. Create a deck of cards;
  deck.addCard(new BombCard{10});
  deck.addCard(new ReinforcementCard{20});
  deck.addCard(new BlockadeCard{30});
  deck.addCard(new AirliftCard{40});
  deck.addCard(new DiplomacyCard{50});
  deck.addCard(new BombCard{60});
  deck.addCard(new ReinforcementCard{70});
  deck.addCard(new BlockadeCard{80});
  deck.addCard(new AirliftCard{90});
  deck.addCard(new DiplomacyCard{100});
}

// Part 2: Game Startup
void GameEngine::StartupPhase() {

  // 1. Randomize order of the players.
  // 2. Distribute Territories randomly.
  const std::vector<Territory*>* const territories = map->GetTerritories();
  for (int i = 0; i < territories->size(); i++) {
    players.at(i % players.size())->AddTerritoryToPlayer(territories->at(i));
  }
  // 3. Give starting troops to all players.
  for (Player* player: players) {
    player->AddArmiesToReinforcementPool(35);
  }
}

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
  for (Continent* continent: map->GetContinents()) {
    Player* player = continent->GetLeader();
    if (player != nullptr) {
      player->AddArmiesToReinforcementPool(continent->GetBonus());
    }
  }
  for (Player* player : players) {
    player->AddArmiesToReinforcementPool(player->GetOwnedTerritories()->size() / 3);
    if (player->GetReinforcementPoolCount() < 3) {
      player->SetReinforcementPool(3);
    }
  }
}

void GameEngine::RoundRobin(bool(Player::*func)()) {
  std::vector<Player*> remainingPlayers = players;
  while (remainingPlayers.size() > 0) {
    for (int i = 0; i < remainingPlayers.size(); i++) {
      Player* player = remainingPlayers.at(i);
      bool b = (player->*func)();
      if (!b) {
        remainingPlayers.erase(remainingPlayers.begin() + i);
        i--;
      }
    }
  }
}

void GameEngine::IssueOrdersPhase() {
  RoundRobin(Player::IssueOrder);
}

bool GameEngine::ExecuteOrdersPhase() { 
  RoundRobin(Player::ExecuteNextOrder); 
}
