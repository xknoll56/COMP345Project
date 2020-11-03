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

#include "Orders.h"

#include <algorithm>

// Orders List
OrdersList::OrdersList() { ordersList = new std::vector<Order*>(); }

OrdersList::OrdersList(const OrdersList& toCopy) {
  // TODO implement copy constructor correctly
  ordersList = toCopy.ordersList;
}

OrdersList::~OrdersList() {
  // The orders don't really deserve an existence outside of the orders list
  // so we'll delete them on the orders list's destruction
  for (auto order : *ordersList) {
    delete order;
  }
  ordersList->clear();
  delete ordersList;
  ordersList = nullptr;
}

OrdersList& OrdersList::operator=(const OrdersList& rightSide) {
  ordersList = rightSide.ordersList;
  return *this;
}

std::ostream& operator<<(std::ostream& outs, const OrdersList& toOutput) {
  std::vector<Order*>* ordersList = toOutput.ordersList;
  outs << "Orders list containing :" << std::endl;
  for (std::vector<Order*>::iterator it = ordersList->begin();
       it != ordersList->end(); ++it) {
    std::cout << *(*it) << std::endl;
  }
  return outs;
}

void OrdersList::queue(Order* order) { ordersList->push_back(order); }

void OrdersList::move(int orderPosition, int newOrderPosition) {
  if (orderPosition < 1 || orderPosition > ordersList->size()) {
    return;
  }
  int orderIndex{orderPosition - 1};
  int newOrderIndex{newOrderPosition - 1};
  newOrderIndex = std::min(std::max(newOrderIndex, 0), (int)ordersList->size());

  Order* toMove = (*ordersList)[orderIndex];
  ordersList->erase(ordersList->begin() + orderIndex);
  ordersList->insert(ordersList->begin() + newOrderIndex, toMove);
}

// Position is 1-indexed instead of 0-indexed
// to make it more intuitive for users
void OrdersList::remove(int position) {
  if (position < 1 || position > ordersList->size()) {
    return;
  }
  // Remove 1 to account for position being 1-indexed
  ordersList->erase(ordersList->begin() + position - 1);
}

// Orders
Order::Order() : player() {}

Order::Order(Player* player) { this->player = player; }

Order::Order(const Order& toCopy) {
  player = toCopy.player;
  wasExecuted = toCopy.wasExecuted;
}

Order::~Order() {}

Order& Order::operator=(const Order& rightSide) {
  player = rightSide.player;
  wasExecuted = rightSide.wasExecuted;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Order& toOutput) {
  return toOutput.doPrint(out);
}

Deploy::Deploy() : Order(), territoryToDeploy(), numberOfArmies(0) {}

Deploy::Deploy(Player* player, Territory* territory,
               int requestedNumberOfArmies)
    : Order(player),
      territoryToDeploy(territory),
      numberOfArmies(requestedNumberOfArmies) {}

Deploy::Deploy(const Deploy& toCopy) : Order(toCopy) {
  this->territoryToDeploy = toCopy.territoryToDeploy;
  this->numberOfArmies = toCopy.numberOfArmies;
}

Deploy& Deploy::operator=(const Deploy& rightSide) {
  Order::operator=(rightSide);
  territoryToDeploy = rightSide.territoryToDeploy;
  return *this;
}

Deploy::~Deploy() {}

bool Deploy::validate() {
  bool playerOwnsTerritory = (territoryToDeploy->GetPlayer() == player);
  bool numberOfArmiesValid = (numberOfArmies > 0);
  return (playerOwnsTerritory && numberOfArmiesValid);
}

void Deploy::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;
  int armiesFromPool =
      player->TakeArmiesFromReinforcementPool(numberOfArmies);
  if (armiesFromPool != numberOfArmies) {
    numberOfArmies = armiesFromPool;
  }
  int newNumberOfArmies =
      territoryToDeploy->GetTroops() + armiesFromPool;
  territoryToDeploy->SetTroops(newNumberOfArmies);
}

std::ostream& Deploy::doPrint(std::ostream& out) const {
  out << "Deploy order.";
  if (wasExecuted) {
    out << " This order was executed. " << numberOfArmies << " armies were deployed to " << territoryToDeploy;
  }
  return out;
}

Advance::Advance() : Order(), sourceTerritory(), targetTerritory(), map() {}

Advance::Advance(Player* player, Territory* sourceTerritory,
                 Territory* targetTerritory, Map* map)
    : Order(player) {
  this->sourceTerritory = sourceTerritory;
  this->targetTerritory = targetTerritory;
  this->map = map;
}
Advance::Advance(const Advance& toCopy) : Order(toCopy) {
  this->sourceTerritory = toCopy.sourceTerritory;
  this->targetTerritory = toCopy.targetTerritory;
  this->map = toCopy.map;
}

Advance::~Advance() {}

Advance& Advance::operator=(const Advance& rightSide) {
  Order::operator=(rightSide);
  sourceTerritory = rightSide.sourceTerritory;
  targetTerritory = rightSide.targetTerritory;
  map = rightSide.map;
  return *this;
}

bool Advance::validate() { return true; }

void Advance::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;
  std::cout << "Advancing into another territory.\n";
}

std::ostream& Advance::doPrint(std::ostream& out) const {
  out << "Advance order.";
  if (wasExecuted) {
    out << " This order was executed, its effect was {effect}.";
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Advance& toOutput) {
  return toOutput.doPrint(out);
}

Bomb::Bomb() : Order(), sourceTerritory(), targetTerritory(), map() {}

Bomb::Bomb(Player* player, Territory* sourceTerritory,
           Territory* targetTerritory, Map* map)
    : Order(player) {
  this->sourceTerritory = sourceTerritory;
  this->targetTerritory = targetTerritory;
  this->map = map;
}

Bomb::Bomb(const Bomb& toCopy) : Order(toCopy) {
  this->sourceTerritory = toCopy.sourceTerritory;
  this->targetTerritory = toCopy.targetTerritory;
  this->map = toCopy.map;
}

Bomb::~Bomb() {}

Bomb& Bomb::operator=(const Bomb& rightSide) {
  Order::operator=(rightSide);
  sourceTerritory = rightSide.sourceTerritory;
  targetTerritory = rightSide.targetTerritory;
  map = rightSide.map;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Bomb& toOutput) {
  return toOutput.doPrint(out);
}

bool Bomb::validate() { return true; }

void Bomb::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;
  std::cout << "Bombing an adjacent territory.\n";
}

std::ostream& Bomb::doPrint(std::ostream& out) const {
  out << "Bomb order.";
  if (wasExecuted) {
    out << " This order was executed, its effect was {effect}.";
  }
  return out;
}

Blockade::Blockade() : Order(), territoryToBlockade() {}

Blockade::Blockade(Player* player, Territory* territoryToBlockade)
    : Order(player) {
  this->territoryToBlockade = territoryToBlockade;
}

Blockade::Blockade(const Blockade& toCopy) : Order(toCopy) {
  this->territoryToBlockade = toCopy.territoryToBlockade;
}

Blockade::~Blockade() {}

Blockade& Blockade::operator=(const Blockade& rightSide) {
  Order::operator=(rightSide);
  territoryToBlockade = rightSide.territoryToBlockade;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Blockade& toOutput) {
  return toOutput.doPrint(out);
}

bool Blockade::validate() { return true; }

void Blockade::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;
  std::cout << "Blockading a territory.\n";
}

std::ostream& Blockade::doPrint(std::ostream& out) const {
  out << "Blockade order.";
  if (wasExecuted) {
    out << " This order was executed, its effect was {effect}.";
  }
  return out;
}

Negotiate::Negotiate() : Order(), opponent() {}

Negotiate::Negotiate(Player* player, Player* opponent) : Order(player) {
  this->opponent = opponent;
}

Negotiate::Negotiate(const Negotiate& toCopy) : Order(toCopy) {
  this->opponent = toCopy.opponent;
}

Negotiate::~Negotiate() {}

Negotiate& Negotiate::operator=(const Negotiate& rightSide) {
  Order::operator=(rightSide);
  opponent = rightSide.opponent;
  return *this;
}

bool Negotiate::validate() { return true; }

void Negotiate::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;
  std::cout << "A peace deal was striken.\n";
}

std::ostream& Negotiate::doPrint(std::ostream& out) const {
  out << "Negotiate order.";
  if (wasExecuted) {
    out << " This order was executed, its effect was {effect}.";
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput) {
  return toOutput.doPrint(out);
}

Airlift::Airlift() : Order(), sourceTerritory(), targetTerritory() {}

Airlift::Airlift(Player* player, Territory* sourceTerritory,
                 Territory* targetTerritory)
    : Order(player) {
  this->sourceTerritory = sourceTerritory;
  this->targetTerritory = targetTerritory;
}

Airlift::Airlift(const Airlift& toCopy) : Order(toCopy) {
  this->sourceTerritory = toCopy.sourceTerritory;
  this->targetTerritory = toCopy.targetTerritory;
}

Airlift::~Airlift() {}

Airlift& Airlift::operator=(const Airlift& rightSide) {
  Order::operator=(rightSide);
  sourceTerritory = rightSide.sourceTerritory;
  targetTerritory = rightSide.targetTerritory;
  return *this;
}

bool Airlift::validate() { return true; }

void Airlift::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;
  std::cout << "Airlifting into another territory!\n";
}

std::ostream& Airlift::doPrint(std::ostream& out) const {
  out << "Airlift order.";
  if (wasExecuted) {
    out << " This order was executed, its effect was {effect}.";
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Airlift& toOutput) {
  return toOutput.doPrint(out);
}