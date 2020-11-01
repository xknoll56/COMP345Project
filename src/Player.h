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
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

class Order;
class Territory;
class Graph;
class Map;
class OrdersList;

class Player {
private:
 // Vector of pointers of territories
 std::vector<Territory*> ownedTerritories;
 // Vector of pointers of cards
 std::vector<Card*> handOfCards;
 // Pointer to a list of orders
 OrdersList* listOfOrders;
 // Player's reinforcement pool
 int reinforcementPool;
 
public:
 // Default constructor
 Player();
 // Parametric constructor
 Player(std::vector<Territory*> terr, int numberOfArmies);
 // Copy constructor
 Player(const Player& pCopy);
 // Destructor
 ~Player();
 // Assignment operator
 Player& operator =(const Player& p);
 // Stream insertion operator
 friend std::ostream& operator<<(std::ostream& out, const Player& toOutput);

 // Will return a vector of pointers of territories to defend
 std::vector<Territory*> toDefend();
 // Will return a vector of pointers of territories to attack
 std::vector<Territory*> toAttack(Map& map);   
 // Will create an Order object and add it to the vector of pointers of orders
 void issueOrder();

 // Additional methods
 void AddTerritoryToPlayer(Territory* territoryToAdd);
 void AddCardToPlayer(Card* cardToAdd);
 void AddOrderToPlayer(Order* orderToAdd);

 // Reinforcement pool
 void AddArmiesToReinforcementPool(int numberOfArmies);
 int GetReinforcementPoolCount();
 // Removes a certain number of armies from the pool
 // Returns the number of armies removed, which may be different
 // (i.e. if less armies left than was asked for)
 int TakeArmiesFromReinforcementPool(int requestedNumberOfArmies);
};