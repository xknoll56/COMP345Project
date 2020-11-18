#pragma once

#include "GameEngine.h"
#include <random>
#include <algorithm>
#include <chrono>
#include <cctype>

GameEngine::GameEngine() : deck() {}

// Part 1: Game Start.
void GameEngine::Init() {
	/* 1. Select a map and load it. */
	std::string map_name;
	std::string file_name;
	MapLoader* ml = new MapLoader();
	std::cout << "Here are the available maps:\n---------------------------\n"
		<< "artic.map\nbigeurope.map\ncanada.map\neurope.map\nfrance.map\nsolar.map\nswiss.map\n";

	// Loops until the user enters a valid map
	do {
		std::cout << "\nEnter your choice of map: ";
		std::cin >> map_name;
		file_name = "src/MapFiles/" + map_name;
		map = ml->GenerateMap(file_name);

		if (map == nullptr)
			std::cout << "Please try again." << std::endl;
	} while (map == nullptr);
	
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
	
	if (answerCap == 'Y') 
		setObs = true;
		
	// Initializing the Observers
        gameStatsObs =
            new GameStatisticsObserver(map->GetTerritories()->size());
        phaseObs = new PhaseObserver();

	// Displaying the observer information to the user
	std::cout << "\nObservers are now " << (setObs ? "enabled." : "disabled.") << std::endl;

	/* 4. Create Players. */
	for (int i = 0; i < num_players; i++) {
		players.push_back(new Player(this));
		std::cout << "Player " << i + 1 << " has been created with an empty hand of cards. " << std::endl;
	} 

	// We only attach the observers and the players if we want them turned on
	if (setObs) {
		for (Player* p : players) {
			// Attaching every player to both observers
			gameStatsObs->AddPlayer(p); 
			phaseObs->AddPlayer(p);
			// Attaching both observers to every player
			p->Attach(gameStatsObs);
			p->Attach(phaseObs);
		}
	}
		
	/* 5. Create a deck of cards. */ 
	deck->addCard(new BombCard());
    deck->addCard(new ReinforcementCard());
    deck->addCard(new BlockadeCard());
    deck->addCard(new AirliftCard());
    deck->addCard(new DiplomacyCard());
    deck->addCard(new BombCard());
    deck->addCard(new ReinforcementCard());
    deck->addCard(new BlockadeCard());
    deck->addCard(new AirliftCard());
    deck->addCard(new DiplomacyCard());

	// Displaying the deck information to the user
	std::cout << "\nA deck of 10 cards has been created. " << std::endl; // change 10 to deck.size of something like that

}

// Part 2: Game Startup
void GameEngine::StartupPhase() {

  // 1. Randomize order of the players.
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(players.begin(), players.end(), e);

  // 2. Distribute Territories randomly.
  const std::vector<Territory*>* territories = map->GetTerritories();
  // std::shuffle(territories->begin(), territories->end(), e);

  for (int i = 0; i < territories->size(); i++) {
    players.at(i % players.size())->AddTerritoryToPlayer(territories->at(i));
  }


  // 3. Give starting troops to all players.
  for (Player* player: players) {
	// TODO - REMOVE THIS TEMP LINE
    // player->AddCardToPlayer(deck.draw());
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

std::vector<Player*> GameEngine::getPlayers()
{
    return players;
}

void GameEngine::IssueOrdersPhase() {
  for (Territory* t: *map->GetTerritories()) {
    t->SetToDeploy(0);
  }
  RoundRobin(&Player::IssueOrder);
}

bool GameEngine::ExecuteOrdersPhase() { 
  RoundRobin(&Player::ExecuteNextOrder);
  return true;
}

Map* GameEngine::GetMap() { return map; }

void GameEngine::PlayerDrawCard(Player* player) {
  Card* card = deck->draw();
  player->AddCardToPlayer(card);
}

Player* GameEngine::neutralPlayer = new Player();

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
