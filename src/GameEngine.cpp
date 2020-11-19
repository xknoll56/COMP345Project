#pragma once

#include "GameEngine.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <random>

//#define MGL_DRIVER
#ifdef MGL_DRIVER
#define STOP std::cin.get();
#define LOG(x) std::cout << "[Main Game Loop Log] " << x << std::endl
#else
#define STOP
#define LOG(X)
#endif

GameEngine::GameEngine()
    : deck(), gameStatsObs(nullptr), map(nullptr), phaseObs(nullptr) {}

// Part 1: Game Start.
void GameEngine::Init() {
  /* 1. Select a map and load it. */
  std::string map_name;
  std::string file_name;
  MapLoader* ml = new MapLoader();
  std::cout << "Here are the available maps:\n---------------------------\n"
            << "artic.map\nbigeurope.map\ncanada.map\neurope.map\nfrance."
               "map\nsolar.map\nswiss.map\n";

  // Loops until the user enters a valid map
  bool validMap = false;
  do {
    std::cout << "\nEnter your choice of map: ";
    std::cin >> map_name;
    file_name = "src/MapFiles/" + map_name;
    map = ml->GenerateMap(file_name);

    if (map == nullptr)
      std::cout << "Please try again." << std::endl;
    else {
      validMap = map->ValidateMap();
      if (!validMap) std::cout << "Not a valid map, please try again\n";
    }

  } while (map == nullptr || !validMap);

  // Deleting the map loader
  delete ml;

  /* 2. Select number of Players. */
  int num_players;

  // Loops until the user enters a valid number of players
  do {
    std::cout << "\nEnter the number of players (2-5) : ";
    std::cin >> num_players;

    if (num_players < 2 || num_players > 5)
      std::cout << "Incorrect number of players, try again." << std::endl;
  } while (num_players < 2 || num_players > 5);

  // Allocating the right number of space in the vector
  players.reserve(num_players);

  /* 3. Set Observers (Part 5). */
  bool setObs = false;
  char answer;
  char answerCap;

  // Loops until the user enters an appropriate answer
  do {
    std::cout << "\nDo you want to enable observers for the game? (Y/N) ";
    std::cin >> answer;
    answerCap = toupper(answer);
    if (answerCap != 'Y' && answerCap != 'N')
      std::cout << "Incorrect response, try again." << std::endl;
  } while (answerCap != 'Y' && answerCap != 'N');

  if (answerCap == 'Y') setObs = true;

  // Initializing the Observers
  gameStatsObs = new GameStatisticsObserver(map->GetTerritories()->size());
  phaseObs = new PhaseObserver();

  // Displaying the observer information to the user
  std::cout << "\nObservers are now " << (setObs ? "enabled." : "disabled.")
            << std::endl;

  /* 4. Create Players. */
  for (int i = 0; i < num_players; i++) {
    players.push_back(new Player(this));
    std::cout << "Player " << i + 1
              << " has been created with an empty hand of cards. " << std::endl;
  }

  // We only attach the observers and the players if we want them turned on
  if (setObs) {
    for (Player* p : players) {
      // Attaching both observers to every player
      p->Attach(gameStatsObs);
      p->Attach(phaseObs);
    }
    // Attaching every player to both observers
    gameStatsObs->AddPlayers(this);
    phaseObs->AddPlayers(this);
  }

  deck = new Deck();
  /* 5. Create a deck of cards. */
  deck->addCard(new BombCard(deck));
  deck->addCard(new ReinforcementCard(deck));
  deck->addCard(new BlockadeCard(deck));
  deck->addCard(new AirliftCard(deck));
  deck->addCard(new DiplomacyCard(deck));
  deck->addCard(new BombCard(deck));
  deck->addCard(new ReinforcementCard(deck));
  deck->addCard(new BlockadeCard(deck));
  deck->addCard(new AirliftCard(deck));
  deck->addCard(new DiplomacyCard(deck));

  // Displaying the deck information to the user
  std::cout << "\nA deck of 10 cards has been created. "
            << std::endl;  // change 10 to deck.size of something like that
}

// Part 2: Game Startup
void GameEngine::StartupPhase() {
  // 1. Randomize order of the players.
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);
  std::shuffle(players.begin(), players.end(), e);

  // 2. Distribute Territories randomly.
  std::vector<Territory*> territories = *map->GetTerritories();
  std::shuffle(territories.begin(), territories.end(), e);

  for (int i = 0; i < territories.size(); i++) {
    players.at(i % players.size())->AddTerritoryToPlayer(territories.at(i));
  }

  // 3. Give starting troops to all players.
  for (Player* player : players) {
    if (players.size() == 2)
      player->AddArmiesToReinforcementPool(40);
    else if (players.size() == 3)
      player->AddArmiesToReinforcementPool(35);
    else if (players.size() == 4)
      player->AddArmiesToReinforcementPool(30);
    else if (players.size() == 5)
      player->AddArmiesToReinforcementPool(25);
    else
      std::cout << "Incorrect number of players. Players have no army.";
  }

  gameStatsObs->Start();
}

// Part 3. Main Game Loop
void GameEngine::MainGameLoop() {
  LOG("Starting Main Game Loop...");
  STOP
  while (true) {
    ReinforcementPhase();
    IssueOrdersPhase();
    ExecuteOrdersPhase();
    // Eliminate players who don't have any more territories.
    for (int i = players.size() - 1; i >= 0; i--) {
      if (players.at(i)->GetOwnedTerritories()->size() < 1) {
        LOG("A player has been eliminated.");
        STOP
        players.erase(players.begin() + i);
      }
    }
    // If a player controls all territories, they win the game.
    if (players.size() < 2) {
      LOG("There's only one player left.");
      STOP
      if (players.at(0)->GetOwnedTerritories()->size() >= map->GetTerritories()->size()) {
        players[0]->Notify();
        LOG("Exiting Main Game Loop.");
        return;
      }
    }
  }
}

/*
Adds troops to a player's reinforcement pool at the start of the turn.
*/
void GameEngine::ReinforcementPhase() {
  LOG("Starting Reinforcement Phase...");
  STOP
  // If a player controls a continent, they get a bonus
  for (Continent* continent : map->GetContinents()) {
    Player* player = continent->GetLeader();
    if (player != nullptr) {
      player->AddArmiesToReinforcementPool(continent->GetBonus());
    }
  }
  // Troops are given based on the amount of controlled territories.
  for (Player* player : players) {
    player->AddArmiesToReinforcementPool(player->GetOwnedTerritories()->size() / 3);
    if (player->GetReinforcementPoolCount() < 3) {
      player->SetReinforcementPool(3);
    }
  }
}

/*
Calls the given boolean function for every player in a round robin fashion.
When said function returns false, the player is removed from the
round robin (until next turn).
The function returns when there ae no players left in the round robin.
*/
void GameEngine::RoundRobin(bool (Player::*func)()) {
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

// Returns all the players in the game in their order of play.
std::vector<Player*> GameEngine::getPlayers() { return players; }

std::vector<Player*>* GameEngine::getPlayersAdress() { return &players; }

/*
Calls IssueOrder in a round robin fashion until all players
have commited that they are done issuing orders.
*/
void GameEngine::IssueOrdersPhase() {
  LOG("Starting Issue Orders Phase...");
  STOP
  for (Territory* t : *map->GetTerritories()) {
    t->SetToDeploy(0);
    t->SetStandByTroops(0);
  }
  for (Player* p : players) {
    p->GenerateToAttack();
    p->GenerateToDefend();
  }
  RoundRobin(&Player::IssueOrder);
}

/*
Calls ExecuteOrder() in a round robin fashion until all players
have no more orders in their orders list.
*/
void GameEngine::ExecuteOrdersPhase() {
  LOG("Starting Execute Orders Phase...");
  STOP
  RoundRobin(&Player::ExecuteNextOrder);
}

// Returns a pointer to the game map.
Map* GameEngine::GetMap() { return map; }

void GameEngine::PlayerDrawCard(Player* player) {
  Card* card = deck->draw();
  player->AddCardToPlayer(card);
}

 // The neutral player owns territories and troops but cannot attack.
Player* GameEngine::neutralPlayer = new Player();

// Returns the neutral player.
Player* GameEngine::GetNeutralPlayer() { return neutralPlayer; }

// Destructor
GameEngine::~GameEngine() {
  for (Player* p : players) {
    delete p;
    p = nullptr;
  }
  players.clear();
  delete gameStatsObs;
  gameStatsObs = nullptr;
  delete phaseObs;
  phaseObs = nullptr;
  delete neutralPlayer;
  neutralPlayer = nullptr;
  delete map;
  map = nullptr;
  delete deck;
  deck = nullptr;
}
