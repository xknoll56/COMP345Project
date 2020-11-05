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

int OrdersList::getListSize() { return ordersList->size(); }

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
  int armiesFromPool = player->TakeArmiesFromReinforcementPool(numberOfArmies);
  if (armiesFromPool != numberOfArmies) {
    numberOfArmies = armiesFromPool;
  }
  int newNumberOfArmies = territoryToDeploy->GetTroops() + armiesFromPool;
  territoryToDeploy->SetTroops(newNumberOfArmies);
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

void Advance::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitAdvance(this);
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

void Bomb::acceptVisitor(OrdersVisitor* visitor) { visitor->VisitBomb(this); }

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
  std::cout << "A peace deal was striken.\n";
}

void Negotiate::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitNegotiate(this);
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

void Airlift::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitAirlift(this);
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
  source->RemoveTroops(troopsToMove);
  target->AddTroops(troopsToMove);
}

void MoveTroops::AttackTarget() {
  int attackingArmies = numberOfArmies;
  int defendingArmies = target->GetTroops();
  int numberOfAttacks = attackingArmies + defendingArmies;
  for (int i = 0; i < numberOfAttacks; i++) {
    if (i % 2 == 0) {
      // Attacking army
      // 60% chance of killing a defending army
    } else {
      // Defending army
      // 70% chance of killing an attacking army
    }
  }
  // Change number of armies in territories
  // If necessary, change ownership of the territory
}

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
