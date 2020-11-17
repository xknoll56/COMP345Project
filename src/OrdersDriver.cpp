// COMP 345 - Project
// Part 1
//
// Sophie Renaud       -  40132563
// Joey Abou-Chaker    -  40055551
// Jordan Goulet       -  40075688
// Xavier Knoll        -  40132134
// Sébastien Champoux  -  40133449
//
// Based on the 'https://www.warzone.com/' game.

#define ORDERS_DRIVER
#ifdef ORDERS_DRIVER

#include <iostream>
#include <string>

#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include "Cards.h"

int main() {
  // Create a map
  Map *map = new Map(1, 4);
  Continent *continent = map->CreateContinent("America", 10);
  Territory *t1 = map->CreateTerritory("Quebec", continent);
  Territory *t2 = map->CreateTerritory("New York", continent);
  Territory *t3 = map->CreateTerritory("Vermont", continent);
  Territory *t4 = map->CreateTerritory("New Hampshire", continent);
  t1->AddNeigbor(t2);
  t2->AddNeigbor(t1);
  t2->AddNeigbor(t3);
  t3->AddNeigbor(t2);
  t3->AddNeigbor(t4);
  t4->AddNeigbor(t3);
  t1->AddTroops(20);
  t2->AddTroops(3);
  t3->AddTroops(3);
  t4->AddTroops(10);

  // Create a deck of cards
  Deck *deckOfCards = new Deck();
  Card *bombCard = new BombCard(5);
  deckOfCards->addCard(bombCard);

  // Create and setup players
  Player *player = new Player(map);
  Player *opponent = new Player(map);
  player->AddArmiesToReinforcementPool(20);
  opponent->AddArmiesToReinforcementPool(10);

  player->AddTerritoryToPlayer(t1);
  t1->SetPlayer(player);
  opponent->AddTerritoryToPlayer(t2);
  t2->SetPlayer(opponent);
  opponent->AddTerritoryToPlayer(t3);
  t3->SetPlayer(opponent);
  opponent->AddTerritoryToPlayer(t4);
  t4->SetPlayer(opponent);

  // Create orders and add them to player's orders list
  Order *deploy = new Deploy(player, t1, 10);
  Order *advance = new Advance(player, t1, t2, 15, deckOfCards);
  Order *airlift = new Airlift(player, t1, t3, 10, deckOfCards);
  Order *bomb = new Bomb(player, t4);
  Order *negotiate = new Negotiate(player, opponent);
  Order *bomb2 = new Bomb(player, t4);
  Order *blockade = new Blockade(player, t1);
  Order *airliftFromOpponent = new Airlift(opponent, t4, t2, 5, deckOfCards);

  player->AddOrderToPlayer(deploy);
  player->AddOrderToPlayer(advance);
  player->AddOrderToPlayer(airlift);
  player->AddOrderToPlayer(bomb);
  player->AddOrderToPlayer(negotiate);
  player->AddOrderToPlayer(bomb2);
  player->AddOrderToPlayer(blockade);

  opponent->AddOrderToPlayer(airliftFromOpponent);

  deploy->execute();
  std::cout << std::endl;
  advance->execute();
  std::cout << std::endl;
  airlift->execute();
  std::cout << std::endl;
  bomb->execute();
  std::cout << std::endl;
  blockade->execute();
  std::cout << std::endl;
  negotiate->execute();
  std::cout << std::endl;

  std::cout << "=== The following two orders are disabled because of the " <<
               "negotiation order ==="
            << std::endl;
  bomb2->execute();  // Disabled b/c of negotiate order
  std::cout << std::endl;
  airliftFromOpponent->execute();  // Disabled b/c of negotiate order
  std::cout << std::endl;

  // Cleanup
  // No need to delete territories and continent,
  // cleaned up by map
  // Orders are deleted by OrdersList, so no need to clean that either
  // Cards are deleted by the deck or hand that contains it
  delete map;
  delete player;
  delete opponent;
  delete deckOfCards;
  return 0;
};
#endif
