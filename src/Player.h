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

#pragma once
#include "Cards.h"
#include "GameObservers.h"
#include "Map.h"
#include "Orders.h"

class Order;
class Territory;
class Graph;
class Map;
class OrdersList;

class Player : public Subject {
 private:
  // Vector of pointers of territories
  std::vector<Territory*> ownedTerritories;
  // Vector of pointers of cards
  std::vector<Card*> handOfCards;
  // Pointer to a list of orders
  OrdersList* listOfOrders;
  // Player's reinforcement pool
  int reinforcementPool;


  // The Phase observer needs to know the current phase
  Phase phase;

 public:
  Map* map;

  Player(Map* map);
  // Parametric constructor
  Player(Map* map, std::vector<Territory*> terr, int numberOfArmies);
  // Copy constructor
  Player(const Player& pCopy);
  // Destructor
  ~Player();
  // Assignment operator
  Player& operator=(const Player& p);
  // Stream insertion operator
  friend std::ostream& operator<<(std::ostream& out, const Player& toOutput);

  // Will return a vector of pointers of territories to defend
  std::vector<Territory*> ToDefend();
  // Will return a vector of pointers of territories to attack
  std::vector<Territory*> ToAttack();
  // Will create an Order object and add it to the vector of pointers of orders
  bool IssueOrder();

  // Additional methods
  void AddTerritoryToPlayer(Territory* territoryToAdd);
  void RemoveTerritoryFromPlayer(Territory* territoryToRemove);
  void AddCardToPlayer(Card* cardToAdd);
  void AddOrderToPlayer(Order* orderToAdd);

  // Reinforcement pool
  void AddArmiesToReinforcementPool(int numberOfArmies);
  int GetReinforcementPoolCount();
  // Removes a certain number of armies from the pool
  // Returns the number of armies removed, which may be different
  // (i.e. if less armies left than was asked for)
  int TakeArmiesFromReinforcementPool(int requestedNumberOfArmies);
  const std::vector<Territory*>* GetOwnedTerritories();
  void SetReinforcementPool(int amount);
  bool ExecuteNextOrder();

  Phase GetPhase();

  OrdersList* GetOrdersList();
};
