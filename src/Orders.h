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

#include <iostream>
#include <string>
#include <vector>

#include "Map.h"
#include "Player.h"

class Player;
class Territory;
class Graph;
class Map;
class OrdersVisitor;

class Order {
 public:
  Order();
  Order(Player* player);
  Order(const Order& toCopy);
  virtual ~Order();
  Order& operator=(const Order& rightSide);

  virtual bool validate();
  virtual void execute() = 0;
  virtual void acceptVisitor(OrdersVisitor* visitor) = 0;
  void disableOrder();
  Player* getPlayer();

  friend std::ostream& operator<<(std::ostream& out, const Order& toOutput);

 protected:
  Player* player;
  bool wasExecuted{false};
  bool isEnabled;

 private:
  virtual std::ostream& doPrint(std::ostream& out) const = 0;
};

// Deploy armies to one of player's territories
class Deploy : public Order {
 public:
  Deploy();
  Deploy(Player* player, Territory* territory, int requestedNumberOfArmies);
  Deploy(const Deploy& toCopy);
  ~Deploy();
  Deploy& operator=(const Deploy& rightSide);

  friend std::ostream& operator<<(std::ostream& out, const Deploy& toOutput);
  virtual bool validate();
  virtual void execute();
  virtual void acceptVisitor(OrdersVisitor* visitor);

 private:
  int numberOfArmies;
  Territory* territoryToDeploy;
  virtual std::ostream& doPrint(std::ostream& out) const;
};

// Advance armies from source to target territories, attacking if needed
class Advance : public Order {
 public:
  Advance();
  Advance(Player* player, Territory* sourceTerritory,
          Territory* targetTerritory, int numberOfArmies);
  Advance(const Advance& toCopy);
  ~Advance();
  Advance& operator=(const Advance& rightSide);

  friend std::ostream& operator<<(std::ostream& out, const Advance& toOutput);

  virtual bool validate();
  virtual void execute();
  virtual void acceptVisitor(OrdersVisitor* visitor);
  void disableDrawAfterConquer();
  Player* getOpponent();

 private:
  Territory* sourceTerritory;
  Territory* targetTerritory;
  int numberOfArmies;
  bool drawAfterConquer;
  virtual std::ostream& doPrint(std::ostream& out) const;
};

// Bomb an opponent's adjacent territory
class Bomb : public Order {
 public:
  Bomb();
  Bomb(Player* player, Territory* targetTerritory);
  Bomb(const Bomb& toCopy);
  ~Bomb();
  Bomb& operator=(const Bomb& rightSide);

  friend std::ostream& operator<<(std::ostream& out, const Bomb& toOutput);

  virtual bool validate();
  virtual void execute();
  virtual void acceptVisitor(OrdersVisitor* visitor);

  Player* getOpponent();

 private:
  Territory* targetTerritory;
  virtual std::ostream& doPrint(std::ostream& out) const;
};

// Triple armies on one of player's territories, making it neutral
class Blockade : public Order {
 public:
  Blockade();
  Blockade(Player* player, Territory* territoryToBlockade);
  Blockade(const Blockade& toCopy);
  ~Blockade();
  Blockade& operator=(const Blockade& rightSide);

  friend std::ostream& operator<<(std::ostream& out, const Blockade& toOutput);

  virtual bool validate();
  virtual void execute();
  virtual void acceptVisitor(OrdersVisitor* visitor);

 private:
  Territory* territoryToBlockade;
  virtual std::ostream& doPrint(std::ostream& out) const;
};

// Negotiate peace for a turn between player and opponent
class Negotiate : public Order {
 public:
  Negotiate();
  Negotiate(Player* player, Player* opponent);
  Negotiate(const Negotiate& toCopy);
  ~Negotiate();
  Negotiate& operator=(const Negotiate& rightSide);

  friend std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput);

  virtual bool validate();
  virtual void execute();
  virtual void acceptVisitor(OrdersVisitor* visitor);

 private:
  Player* opponent;
  virtual std::ostream& doPrint(std::ostream& out) const;
};

// Advance armies from a player's territory to any territory
class Airlift : public Order {
 public:
  Airlift();
  Airlift(Player* player, Territory* sourceTerritory,
          Territory* targetTerritory, int numberOfArmies);
  Airlift(const Airlift& toCopy);
  ~Airlift();
  Airlift& operator=(const Airlift& rightSide);

  friend std::ostream& operator<<(std::ostream& out, const Airlift& toOutput);

  virtual bool validate();
  virtual void execute();
  virtual void acceptVisitor(OrdersVisitor* visitor);
  void disableDrawAfterConquer();
  Player* getOpponent();

 private:
  Territory* sourceTerritory;
  Territory* targetTerritory;
  int numberOfArmies;
  bool drawAfterConquer;
  virtual std::ostream& doPrint(std::ostream& out) const;
};

class OrdersList {
 public:
  OrdersList();
  OrdersList(const OrdersList& toCopy);
  ~OrdersList();
  OrdersList& operator=(const OrdersList& rightSide);

  // Add an order to the end of the orders list
  void queue(Order* o);

  // Takes order at position orderPosition and moves it at newOrderPosition,
  // shifting other orders accordingly Position" is 1-indexed instead of
  // 0-indexed (more intuitive to the player)
  void move(int orderPosition, int newOrderPosition);

  // Removes the order at position (i.e. 1-indexed) in the list
  // Method is called remove instead of delete b/c delete is a
  // reserved keyword, which drove Visual Studio nuts
  void remove(int position);

  friend std::ostream& operator<<(std::ostream& outs,
                                  const OrdersList& toOutput);
  int getListSize();

  // Run a visitor over every order in the list
  void visitOrders(OrdersVisitor* visitor);

 private:
  std::vector<Order*>* ordersList;
};

// Class that contains the algorithm to displace troops
// and attacks if the target is an opponent
class MoveTroops {
 public:
  MoveTroops();
  MoveTroops(const MoveTroops& toCopy);
  MoveTroops(Player* player, Territory* source, Territory* target,
             int numberOfArmies);
  MoveTroops& operator=(const MoveTroops& rightSide);

  friend std::ostream& operator<<(std::ostream& out,
                                  const MoveTroops& toOutput);

  // Method to call to execute the troops displacement
  void ExecuteTheMove();
  bool AttackerConqueredTarget();

 private:
  Player* player;
  Territory* source;
  Territory* target;
  int numberOfArmies;
  bool wasExecuted;
  bool attackerConquered;

  bool PlayerOwnsTarget();
  void MoveArmies();
  void AttackTarget();
  bool AttackerKilledDefenderArmy();
  bool DefenderKilledAttackerArmy();
};

// Orders visitors
// To perform operations on lists of visitors when their type matters
class OrdersVisitor {
 public:
  virtual ~OrdersVisitor();

  // All of these do nothing by default, and will only perform an operation if
  // redefined in subtypes
  virtual void VisitDeploy(Deploy* order);
  virtual void VisitAdvance(Advance* order);
  virtual void VisitAirlift(Airlift* order);
  virtual void VisitBomb(Bomb* order);
  virtual void VisitBlockade(Blockade* order);
  virtual void VisitNegotiate(Negotiate* order);
};

// Visitor that visits orders and disables attack orders between player and
// opponent
class NegotiateVisitor : public OrdersVisitor {
 public:
  NegotiateVisitor();
  NegotiateVisitor(Player* player, Player* opponent);
  NegotiateVisitor(NegotiateVisitor& tocopy);
  NegotiateVisitor& operator=(const NegotiateVisitor& rightSide);
  virtual ~NegotiateVisitor();

  virtual void VisitAdvance(Advance* order);
  virtual void VisitAirlift(Airlift* order);
  virtual void VisitBomb(Bomb* order);

 private:
  Player* player;
  Player* opponent;
  // void DisableIfPlayerAndOpponent(Order* order, Player* opponent);
};

// Visitor that visits orders and disables card draw when player conquers a
// territory
class DisableCardDrawVisitor : public OrdersVisitor {
 public:
  DisableCardDrawVisitor();
  DisableCardDrawVisitor(Player* player);
  DisableCardDrawVisitor(DisableCardDrawVisitor& toCopy);
  DisableCardDrawVisitor& operator=(const DisableCardDrawVisitor& rightSide);
  virtual ~DisableCardDrawVisitor();

  virtual void VisitAdvance(Advance* order);
  virtual void VisitAirlift(Airlift* order);

 private:
  Player* player;
};