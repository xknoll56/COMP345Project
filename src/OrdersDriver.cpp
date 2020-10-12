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

//#define ORDERS_DRIVER
#ifdef ORDERS_DRIVER

#include <string>
#include <iostream>

#include "Map.h"
#include "Orders.h"
#include "Player.h"

int main() {
  // Create some players and territories to do the tests
  Player *player = new Player();
  Player *opponent = new Player();
  Continent continent = Continent("Continent", 0);
  Territory *playerTerritory = 
      continent.CreateTerritory(101, "Player's territory");
  Territory *opponentTerritory =
      continent.CreateTerritory(102, "Opponent's territory");
  Map map = Map(2, 1);
  map.AddBorder({101, 102});
  map.AddBorder({102, 101});
  playerTerritory->SetPlayer(player);
  opponentTerritory->SetPlayer(opponent);

  // Test the orders
  std::cout << "Testing the orders:" << std::endl << std::endl;
  Order *deploy = new Deploy(player, playerTerritory);
  Order *advance = new Advance(player, playerTerritory, opponentTerritory, &map);
  Order *bomb = new Bomb(player, playerTerritory, opponentTerritory, &map);
  Order *blockade = new Blockade(player, playerTerritory);
  Order *negotiate = new Negotiate(player, opponent);
  Order *airlift = new Airlift(player, playerTerritory, opponentTerritory);

  std::cout << "Deploy is valid: " << deploy->validate() << std::endl;
  deploy->execute();
  std::cout << "Advance is valid: " << advance->validate() << std::endl;
  advance->execute();
  std::cout << "Bomb is valid: " << bomb->validate() << std::endl;
  bomb->execute();
  std::cout << "Blockade is valid: " << blockade->validate() << std::endl;
  blockade->execute();
  std::cout << "Negotiate is valid: " << negotiate->validate() << std::endl;
  negotiate->execute();
  std::cout << "Airlift is valid: " << airlift->validate() << std::endl;
  airlift->execute();

  // Test the orders list
  std::cout << std::endl
            << "===" << std::endl
            << std::endl
            << "Testing the orders list" << std::endl;

  OrdersList *ordersList = new OrdersList();
  ordersList->queue(deploy);
  ordersList->queue(advance);
  ordersList->queue(bomb);
  ordersList->queue(blockade);
  ordersList->queue(negotiate);
  ordersList->queue(airlift);

  std::cout << *ordersList << std::endl;

  // Move orders within the list
  // Expected after move: Bomb, Advance, Deploy, Negotiate, Blockade, Airlift
  ordersList->move(1, 4);
  ordersList->move(3, 5);
  ordersList->move(2, 1);

  std::cout << *ordersList << std::endl;

  // Remove orders
  // Expected: Bomb, Deploy, Negotiate, Blockade
  ordersList->remove(2);
  ordersList->remove(5);

  std::cout << *ordersList << std::endl;

  // Finish the tests, delete everything
  // Note that the orderslist takes care of
  // deleting the orders it contains
  delete ordersList;
  delete player;
  delete opponent;
  delete playerTerritory;
  delete opponentTerritory;

  // Take care of dangling pointers as well?
  // Not sure if necessary as the program stops after but just in case
  ordersList = nullptr;
  player = nullptr;
  opponent = nullptr;
  playerTerritory = nullptr;
  opponentTerritory = nullptr;

  return 0;
};
#endif
