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

#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <cmath>

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

int OrdersList::getListSize() { return ordersList->size(); }

void OrdersList::visitOrders(OrdersVisitor* visitor) {
  for (std::vector<Order*>::iterator it = ordersList->begin();
       it != ordersList->end(); ++it) {
    (*it)->acceptVisitor(visitor);
  }
}

// Orders
Order::Order() : player(), isEnabled(true) {}

Order::Order(Player* player) : player(player), isEnabled(true) {}

Order::Order(const Order& toCopy) {
  player = toCopy.player;
  wasExecuted = toCopy.wasExecuted;
  isEnabled = toCopy.isEnabled;
}

Order::~Order() {}

Order& Order::operator=(const Order& rightSide) {
  player = rightSide.player;
  wasExecuted = rightSide.wasExecuted;
  isEnabled = rightSide.isEnabled;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Order& toOutput) {
  return toOutput.doPrint(out);
}

void Order::disableOrder() { isEnabled = false; }

Player* Order::getPlayer() { return player; }

Deploy::Deploy() : Order(), territoryToDeploy(), numberOfArmies(0) {}

Deploy::Deploy(Player* player, Territory* territory,
               int requestedNumberOfArmies)
    : Order(player), territoryToDeploy(territory) {
  this->numberOfArmies = std::max(0, requestedNumberOfArmies);
}

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

bool Deploy::validate() { return (territoryToDeploy->GetPlayer() == player); }

void Deploy::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;

  int armiesFromPool = player->TakeArmiesFromReinforcementPool(numberOfArmies);
  if (armiesFromPool != numberOfArmies) {
    numberOfArmies = armiesFromPool;
  }
  territoryToDeploy->AddTroops(numberOfArmies);
}

void Deploy::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitDeploy(this);
}

std::ostream& Deploy::doPrint(std::ostream& out) const {
  out << "Deploy order.";
  if (wasExecuted) {
    out << " This order was executed. " << numberOfArmies
        << " armies were deployed to " << territoryToDeploy;
  }
  return out;
}

Advance::Advance()
    : Order(), sourceTerritory(), targetTerritory(), numberOfArmies(0) {}

Advance::Advance(Player* player, Territory* sourceTerritory,
                 Territory* targetTerritory, int numberOfArmies)
    : Order(player),
      sourceTerritory(sourceTerritory),
      targetTerritory(targetTerritory) {
  this->numberOfArmies = std::max(0, numberOfArmies);
}
Advance::Advance(const Advance& toCopy) : Order(toCopy) {
  this->sourceTerritory = toCopy.sourceTerritory;
  this->targetTerritory = toCopy.targetTerritory;
  this->numberOfArmies = toCopy.numberOfArmies;
}

Advance::~Advance() {}

Advance& Advance::operator=(const Advance& rightSide) {
  Order::operator=(rightSide);
  sourceTerritory = rightSide.sourceTerritory;
  targetTerritory = rightSide.targetTerritory;
  numberOfArmies = rightSide.numberOfArmies;
  return *this;
}

bool Advance::validate() {
  bool playerOwnsSource = (sourceTerritory->GetPlayer() == player);
  bool territoriesAreAdjacent =
      sourceTerritory->TestAdjacencyTo(targetTerritory);
  return (playerOwnsSource && territoriesAreAdjacent);
}

void Advance::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;

  MoveTroops* moveTroops =
      new MoveTroops(player, sourceTerritory, targetTerritory, numberOfArmies);
  moveTroops->ExecuteTheMove();
  delete moveTroops;
  moveTroops = nullptr;
}

void Advance::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitAdvance(this);
}

Player* Advance::getOpponent() { return targetTerritory->GetPlayer(); }

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

Bomb::Bomb()
    : Order(),
      sourceTerritory(),
      targetTerritory(),
      numberOfDestroyedArmies(0) {}

Bomb::Bomb(Player* player, Territory* sourceTerritory,
           Territory* targetTerritory)
    : Order(player),
      sourceTerritory(sourceTerritory),
      targetTerritory(targetTerritory),
      numberOfDestroyedArmies(0) {}

Bomb::Bomb(const Bomb& toCopy) : Order(toCopy) {
  this->sourceTerritory = toCopy.sourceTerritory;
  this->targetTerritory = toCopy.targetTerritory;
  this->numberOfDestroyedArmies = toCopy.numberOfDestroyedArmies;
}

Bomb::~Bomb() {}

Bomb& Bomb::operator=(const Bomb& rightSide) {
  Order::operator=(rightSide);
  sourceTerritory = rightSide.sourceTerritory;
  targetTerritory = rightSide.targetTerritory;
  numberOfDestroyedArmies = rightSide.numberOfDestroyedArmies;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Bomb& toOutput) {
  return toOutput.doPrint(out);
}

bool Bomb::validate() {
  bool playerOwnsSource = (sourceTerritory->GetPlayer() == player);
  bool playerDoesntOwnTarget = (targetTerritory->GetPlayer() != player);
  bool territoriesAreAdjacent =
      sourceTerritory->TestAdjacencyTo(targetTerritory);
  return (playerOwnsSource && playerDoesntOwnTarget && territoriesAreAdjacent);
}

void Bomb::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;
  numberOfDestroyedArmies =
      std::floor((float)targetTerritory->GetTroops() / 2.0f);
  targetTerritory->RemoveTroops(numberOfDestroyedArmies);
}

void Bomb::acceptVisitor(OrdersVisitor* visitor) { visitor->VisitBomb(this); }

Player* Bomb::getOpponent() { return targetTerritory->GetPlayer(); }

std::ostream& Bomb::doPrint(std::ostream& out) const {
  out << "Bomb order.";
  if (wasExecuted) {
    out << " " << sourceTerritory << " was bombed, removing "
        << numberOfDestroyedArmies << "armies.";
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

void Blockade::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitBlockade(this);
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

  // Run the negotiate visitor over the order's list of player and opponent
  NegotiateVisitor negotiateVisitor = NegotiateVisitor(player, opponent);
  player->GetOrdersList()->visitOrders(&negotiateVisitor);

  negotiateVisitor = NegotiateVisitor(opponent, player);
  opponent->GetOrdersList()->visitOrders(&negotiateVisitor);
}

void Negotiate::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitNegotiate(this);
}

std::ostream& Negotiate::doPrint(std::ostream& out) const {
  out << "Negotiate order.";
  if (wasExecuted) {
    out << " This order was executed, every attack between " << player
        << " and " << opponent << " was disabled";
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput) {
  return toOutput.doPrint(out);
}

Airlift::Airlift()
    : Order(), sourceTerritory(), targetTerritory(), numberOfArmies(0) {}

Airlift::Airlift(Player* player, Territory* sourceTerritory,
                 Territory* targetTerritory, int numberOfArmies)
    : Order(player),
      sourceTerritory(sourceTerritory),
      targetTerritory(targetTerritory) {
  this->numberOfArmies =
      std::max(0, std::min(numberOfArmies, sourceTerritory->GetTroops()));
}

Airlift::Airlift(const Airlift& toCopy) : Order(toCopy) {
  this->sourceTerritory = toCopy.sourceTerritory;
  this->targetTerritory = toCopy.targetTerritory;
  this->numberOfArmies = toCopy.numberOfArmies;
}

Airlift::~Airlift() {}

Airlift& Airlift::operator=(const Airlift& rightSide) {
  Order::operator=(rightSide);
  sourceTerritory = rightSide.sourceTerritory;
  targetTerritory = rightSide.targetTerritory;
  numberOfArmies = rightSide.numberOfArmies;
  return *this;
}

bool Airlift::validate() {
  bool playerOwnsSource = (sourceTerritory->GetPlayer() == player);
  return playerOwnsSource;
}

void Airlift::execute() {
  if (!validate()) {
    return;
  }
  wasExecuted = true;

  MoveTroops* moveTroops =
      new MoveTroops(player, sourceTerritory, targetTerritory, numberOfArmies);
  moveTroops->ExecuteTheMove();
  delete moveTroops;
  moveTroops = nullptr;
}

void Airlift::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitAirlift(this);
}

Player* Airlift::getOpponent() { return targetTerritory->GetPlayer(); }

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

// Class that moves troops and attacks if necessary
MoveTroops::MoveTroops()
    : source(), target(), player(), numberOfArmies(0), wasExecuted(false) {}

MoveTroops::MoveTroops(const MoveTroops& toCopy) {
  this->player = toCopy.player;
  this->source = toCopy.source;
  this->target = toCopy.target;
  this->numberOfArmies = toCopy.numberOfArmies;
  this->wasExecuted = toCopy.wasExecuted;
}

MoveTroops& MoveTroops::operator=(const MoveTroops& rightSide) {
  player = rightSide.player;
  source = rightSide.source;
  target = rightSide.target;
  numberOfArmies = rightSide.numberOfArmies;
  wasExecuted = rightSide.wasExecuted;
  return *this;
}

MoveTroops::MoveTroops(Player* player, Territory* source, Territory* target,
                       int numberOfArmies)
    : player(player), source(source), target(target), wasExecuted(false) {
  this->numberOfArmies = std::max(0, numberOfArmies);
}

void MoveTroops::ExecuteTheMove() {
  if (wasExecuted) {
    return;
  }
  wasExecuted = true;
  if (PlayerOwnsTarget()) {
    MoveArmies();
  } else {
    AttackTarget();
  }
}

bool MoveTroops::PlayerOwnsTarget() { return (target->GetPlayer() == player); }

void MoveTroops::MoveArmies() {
  int troopsToMove = std::min(numberOfArmies, source->GetTroops());
  if (troopsToMove != numberOfArmies) {
    numberOfArmies = troopsToMove;
  }
  source->RemoveTroops(numberOfArmies);
  target->AddTroops(numberOfArmies);
}

void MoveTroops::AttackTarget() {
  // Initialize random seed
  srand(time(NULL));

  for (int i = 0; i < (numberOfArmies + target->GetTroops()); i++) {
    if (i % 2 == 0) {
      if (AttackerKilledDefenderArmy()) {
        target->RemoveTroops(1);
      }
    } else {
      if (DefenderKilledAttackerArmy()) {
        source->RemoveTroops(1);
        numberOfArmies--;
      }
    }
  }

  // If no troops left in target, player takes ownership
  // and we assign the number of surviving armies to the territory
  if (target->GetTroops() == 0) {
    target->SetPlayer(player);
    target->SetTroops(numberOfArmies);
  }
}

// Returns true if rand() is in [0, 6[ over the range [0, 10[
bool MoveTroops::AttackerKilledDefenderArmy() { return (rand() % 10 < 6); }

// Returns true if rand() is in [0, 7[ over the range [0, 10[
bool MoveTroops::DefenderKilledAttackerArmy() { return (rand() % 10 < 7); }

std::ostream& operator<<(std::ostream& out, const MoveTroops& toOutput) {
  // TODO: insert return statement here
}

// Visitors
// The default visitor does nothing, any operation must be defined in subtypes
OrdersVisitor::~OrdersVisitor() {}
void OrdersVisitor::VisitDeploy(Deploy* order) {}
void OrdersVisitor::VisitAdvance(Advance* order) {}
void OrdersVisitor::VisitAirlift(Airlift* order) {}
void OrdersVisitor::VisitBomb(Bomb* order) {}
void OrdersVisitor::VisitBlockade(Blockade* order) {}
void OrdersVisitor::VisitNegotiate(Negotiate* order) {}

NegotiateVisitor::NegotiateVisitor() : player(), opponent() {}

NegotiateVisitor::NegotiateVisitor(Player* player, Player* opponent)
    : player(player), opponent(opponent) {}

NegotiateVisitor::NegotiateVisitor(NegotiateVisitor& tocopy) {
  this->player = tocopy.player;
  this->opponent = tocopy.opponent;
}

NegotiateVisitor& NegotiateVisitor::operator=(
    const NegotiateVisitor& rightSide) {
  player = rightSide.player;
  opponent = rightSide.opponent;
  return *this;
}

NegotiateVisitor::~NegotiateVisitor() {}

// These methods are written like this b/c getOpponent() is not part
// of Order's interface.  This would benefit from some refactoring
// obviously, I might take care of it eventually but this will do for the moment
void NegotiateVisitor::VisitAdvance(Advance* order) {
  DisableIfPlayerAndOpponent(order, order->getOpponent());
}

void NegotiateVisitor::VisitAirlift(Airlift* order) {
  DisableIfPlayerAndOpponent(order, order->getOpponent());
}

void NegotiateVisitor::VisitBomb(Bomb* order) {
  DisableIfPlayerAndOpponent(order, order->getOpponent());
}

void NegotiateVisitor::DisableIfPlayerAndOpponent(Order* order,
                                                  Player* opponent) {
  if (order->getPlayer() == player && opponent == this->opponent) {
    order->disableOrder();
  }
}
