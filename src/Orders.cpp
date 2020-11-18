// COMP 345 - Project
// Part 1
//
// Sophie Renaud       -  40132563
// Joey Abou-Chaker    -  40055551
// Jordan Goulet       -  40075688
// Xavier Knoll        -  40132134
// S�bastien Champoux  -  40133449
//
// Based on the 'https://www.warzone.com/' game.

#include "Orders.h"

#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <cmath>

#include "GameEngine.h"

// Orders List
OrdersList::OrdersList() {
  ordersList = new std::vector<Order*>();
  priorityLevel = 0;
}

OrdersList::OrdersList(const OrdersList& toCopy) {
  // TODO implement copy constructor correctly
  ordersList = toCopy.ordersList;
  priorityLevel = toCopy.priorityLevel;
}

OrdersList::~OrdersList() {
  // The orders don't really deserve an existence outside of the orders list
  // so we'll delete them on the orders list's destruction
  if (ordersList->size() > 0) {
    for (auto order : *ordersList) {
      delete order;
    }
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
  // Index = remove 1 to account for position being 1-indexed
  // We will also delete the order to avoid memory leaks,
  // the orders don't deserve to live outside the orders list
  int index{position - 1};
  delete (*ordersList)[index];
  ordersList->erase(ordersList->begin() + index);
}

int OrdersList::getListSize() { return ordersList->size(); }

void OrdersList::visitOrders(OrdersVisitor* visitor) {
  for (std::vector<Order*>::iterator it = ordersList->begin();
       it != ordersList->end(); ++it) {
    (*it)->acceptVisitor(visitor);
  }
}

Order* OrdersList::popNextOrder() {
  if (ordersList->size() < 1) {
    return nullptr;
  }
  Order* order = nullptr;
  switch (priorityLevel) {
    case 0:  // Deploy
      std::cout << "priority level: " << priorityLevel << std::endl;
      std::cout << "size: " << ordersList->size() << std::endl;
      for (int i = 0; i < ordersList->size(); i++) {
        std::cout << "index: " << i << std::endl;
        std::cout << "order: " << ordersList->at(i) << std::endl;
        order = dynamic_cast<Deploy*>(ordersList->at(i));
        if (order) {
          ordersList->erase(ordersList->begin() + i);
          return order;
        }
      }
      // There are no more deploy orders in the list
      priorityLevel++;
    case 1:  // Airlift
      for (int i = 0; i < ordersList->size(); i++) {
        order = dynamic_cast<Airlift*>(ordersList->at(i));
        if (order) {
          ordersList->erase(ordersList->begin() + i);
          return order;
        }
      }
      // There are no more deploy orders in the list
      priorityLevel++;
    case 2:  // Blockade
      for (int i = 0; i < ordersList->size(); i++) {
        order = dynamic_cast<Blockade*>(ordersList->at(i));
        if (order) {
          ordersList->erase(ordersList->begin() + i);
          return order;
        }
      }
      // There are no more deploy orders in the list
      priorityLevel++;
    case 3:  // Other
      if (ordersList->size() > 0) {
        order = ordersList->at(0);
        ordersList->erase(ordersList->begin());
        return order;
      }
      // There are no more orders in the list
      priorityLevel = 0;
    default:
      return nullptr;
  }
}

// Orders
Order::Order() : player(nullptr), isEnabled(true) {}

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

bool Order::validate() {
  std::cout << "Order is enabled: " << isEnabled
            << ", Order wasn't executed yet: " << !wasExecuted << ", ";
  return (player != NULL && isEnabled && !wasExecuted);
}

void Order::setPlayer(Player* p) { player = p; }

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

Deploy::~Deploy() {}

Deploy& Deploy::operator=(const Deploy& rightSide) {
  Order::operator=(rightSide);
  territoryToDeploy = rightSide.territoryToDeploy;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Deploy& toOutput) {
  return toOutput.doPrint(out);
}

bool Deploy::validate() {
  bool baseValidation{Order::validate()};
  bool playerOwnsTerritory{territoryToDeploy != NULL &&
                           territoryToDeploy->GetPlayer() == player};
  std::cout << "Player owns territory: " << playerOwnsTerritory << std::endl;
  return (baseValidation && territoryToDeploy != NULL && playerOwnsTerritory);
}

void Deploy::execute() {
  if (!validate()) {
    std::cout << "DEPLOY ORDER: disabled or invalid." << std::endl;
    return;
  }
  wasExecuted = true;

  int armiesFromPool = player->TakeArmiesFromReinforcementPool(numberOfArmies);
  if (armiesFromPool != numberOfArmies) {
    numberOfArmies = armiesFromPool;
  }
  territoryToDeploy->AddTroops(numberOfArmies);
  std::cout << "DEPLOY ORDER: Deployed " << numberOfArmies << " troops to "
            << *territoryToDeploy->GetName() << std::endl;
}

void Deploy::setNumberOfArmies(int n) { numberOfArmies = std::max(0, n); }
void Deploy::setTerritoryToDeploy(Territory* t) { territoryToDeploy = t; }

void Deploy::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitDeploy(this);
}

std::ostream& Deploy::doPrint(std::ostream& out) const {
  out << "Deploy orders by " << player << " of " << numberOfArmies
      << " troops on " << *territoryToDeploy->GetName();
  return out;
}

Advance::Advance()
    : Order(),
      sourceTerritory(),
      targetTerritory(),
      numberOfArmies(0),
      drawAfterConquer(true) {}

Advance::Advance(Player* player, Territory* sourceTerritory,
                 Territory* targetTerritory, int numberOfArmies)
    : Order(player),
      sourceTerritory(sourceTerritory),
      targetTerritory(targetTerritory),
      drawAfterConquer(true) {
  this->numberOfArmies = std::max(0, numberOfArmies);
}
Advance::Advance(const Advance& toCopy) : Order(toCopy) {
  this->sourceTerritory = toCopy.sourceTerritory;
  this->targetTerritory = toCopy.targetTerritory;
  this->numberOfArmies = toCopy.numberOfArmies;
  this->drawAfterConquer = toCopy.drawAfterConquer;
}

Advance::~Advance() {}

Advance& Advance::operator=(const Advance& rightSide) {
  Order::operator=(rightSide);
  sourceTerritory = rightSide.sourceTerritory;
  targetTerritory = rightSide.targetTerritory;
  numberOfArmies = rightSide.numberOfArmies;
  drawAfterConquer = rightSide.drawAfterConquer;
  return *this;
}

bool Advance::validate() {
  bool baseValidation{Order::validate()};
  bool playerOwnsSource =
      (sourceTerritory != NULL && sourceTerritory->GetPlayer() == player);
  bool territoriesAreAdjacent =
      (targetTerritory != NULL &&
       sourceTerritory->TestAdjacencyTo(targetTerritory));
  std::cout << "Player owns source territory: " << playerOwnsSource
            << ", Territories are adjacent: " << territoriesAreAdjacent
            << std::endl;
  return (baseValidation && playerOwnsSource && territoriesAreAdjacent);
}

void Advance::execute() {
  if (!validate()) {
    std::cout << "ADVANCE ORDER: disabled or invalid" << std::endl;
    return;
  }
  wasExecuted = true;

  std::cout << "ADVANCE ORDER: ";
  MoveTroops moveTroops(player, sourceTerritory, targetTerritory,
                        numberOfArmies);
  moveTroops.ExecuteTheMove();
  if (moveTroops.AttackerConqueredTarget() && drawAfterConquer) {
    DisableCardDrawVisitor disableDrawVisitor(player);
    player->GetOrdersList()->visitOrders(&disableDrawVisitor);
    player->DrawCard();
    std::cout << " A card was drawn and added to " << player << "'s hand.";
  } else {
    std::cout << " No card was drawn";
  }
  std::cout << std::endl;
}

void Advance::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitAdvance(this);
}

void Advance::disableDrawAfterConquer() { drawAfterConquer = false; }

Player* Advance::getOpponent() { return targetTerritory->GetPlayer(); }

void Advance::setSourceTerritory(Territory* t) { sourceTerritory = t; }

void Advance::setTargetTerritory(Territory* t) { targetTerritory = t; }

void Advance::setNumberOfArmies(int n) { numberOfArmies = std::max(0, n); }

std::ostream& Advance::doPrint(std::ostream& out) const {
  out << "Advance order of " << numberOfArmies << " from "
      << *sourceTerritory->GetName() << " to " << *targetTerritory->GetName();
  return out;
}

std::ostream& operator<<(std::ostream& out, const Advance& toOutput) {
  return toOutput.doPrint(out);
}

Bomb::Bomb() : Order(), targetTerritory() {}

Bomb::Bomb(Player* player, Territory* targetTerritory)
    : Order(player), targetTerritory(targetTerritory) {}

Bomb::Bomb(const Bomb& toCopy) : Order(toCopy) {
  this->targetTerritory = toCopy.targetTerritory;
}

Bomb::~Bomb() {}

Bomb& Bomb::operator=(const Bomb& rightSide) {
  Order::operator=(rightSide);
  targetTerritory = rightSide.targetTerritory;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Bomb& toOutput) {
  return toOutput.doPrint(out);
}

bool Bomb::validate() {
  bool baseValidation{Order::validate()};
  bool playerDoesntOwnTarget{targetTerritory != NULL &&
                             targetTerritory->GetPlayer() != player};
  std::cout << "Player doesn't own target: " << playerDoesntOwnTarget
            << std::endl;
  return (baseValidation && playerDoesntOwnTarget);
}

void Bomb::execute() {
  if (!validate()) {
    std::cout << "BOMB ORDER: disabled or invalid." << std::endl;
    return;
  }
  wasExecuted = true;
  int numberOfDestroyedArmies =
      std::floor((float)targetTerritory->GetTroops() / 2.0f);
  targetTerritory->RemoveTroops(numberOfDestroyedArmies);
  std::cout << "BOMB ORDER: destroyed " << numberOfDestroyedArmies
            << " troops in " << *targetTerritory->GetName() << std::endl;
}

void Bomb::acceptVisitor(OrdersVisitor* visitor) { visitor->VisitBomb(this); }

Player* Bomb::getOpponent() { return targetTerritory->GetPlayer(); }

void Bomb::setTargetTerritory(Territory* t) { targetTerritory = t; }

std::ostream& Bomb::doPrint(std::ostream& out) const {
  out << "Bomb order on " << *targetTerritory->GetName();
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

bool Blockade::validate() {
  bool baseValidation{Order::validate()};
  bool playerOwnsTerritory = (territoryToBlockade != NULL &&
                              territoryToBlockade->GetPlayer() == player);
  std::cout << "Player owns territory: " << playerOwnsTerritory << std::endl;
  return (baseValidation && playerOwnsTerritory);
}

void Blockade::execute() {
  if (!validate()) {
    std::cout << "BLOCKADE ORDER: disabled or invalid." << std::endl;
    return;
  }
  wasExecuted = true;
  // Double number of troops
  int numberOfTroops = territoryToBlockade->GetTroops();
  territoryToBlockade->AddTroops(numberOfTroops);
  // Transfer ownership to neutral player
  territoryToBlockade->GetPlayer()->RemoveTerritoryFromPlayer(
      territoryToBlockade);
  territoryToBlockade->SetPlayer(GameEngine::GetNeutralPlayer());
  std::cout
      << "BLOCKADE ORDER: " << numberOfTroops << " troops were added to "
      << *territoryToBlockade->GetName()
      << ", and ownership of the territory was transferred to the null player."
      << std::endl;
}

void Blockade::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitBlockade(this);
}

void Blockade::setTerritory(Territory* t) { territoryToBlockade = t; }

std::ostream& Blockade::doPrint(std::ostream& out) const {
  out << "Blockade order on " << *territoryToBlockade->GetName();
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

bool Negotiate::validate() {
  bool baseValidation{Order::validate()};
  bool opponentIsDefined{opponent != NULL};
  bool opponentIsNotPlayer{player != opponent};
  bool opponentIsNotNeutralPlayer{opponent != GameEngine::GetNeutralPlayer()};
  std::cout << "Opponent is defined: " << opponentIsDefined
            << ", Opponent is not player : " << opponentIsNotPlayer << ", ";
  std::cout << "Opponent is not neutral player : " << opponentIsNotNeutralPlayer
            << std::endl;
  return (baseValidation && opponentIsDefined && opponentIsNotPlayer &&
          opponentIsNotNeutralPlayer);
}

void Negotiate::execute() {
  if (!validate()) {
    std::cout << "NEGOTIATE ORDER: disabled or invalid." << std::endl;
    return;
  }
  wasExecuted = true;

  // Run the negotiate visitor over the order's list of player and opponent
  NegotiateVisitor playerNegotiateVisitor(player, opponent);
  player->GetOrdersList()->visitOrders(&playerNegotiateVisitor);

  NegotiateVisitor opponentNegotiateVisitor(opponent, player);
  opponent->GetOrdersList()->visitOrders(&opponentNegotiateVisitor);

  std::cout << "NEGOTIATE ORDER: all attacks between " << player << " and "
            << opponent << " until the end of the turn were disabled."
            << std::endl;
}

void Negotiate::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitNegotiate(this);
}

void Negotiate::setOpponent(Player* o) { opponent = o; }

std::ostream& Negotiate::doPrint(std::ostream& out) const {
  out << "Negotiate order between " << player << " and " << opponent;
  return out;
}

std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput) {
  return toOutput.doPrint(out);
}

Airlift::Airlift()
    : Order(),
      sourceTerritory(),
      targetTerritory(),
      numberOfArmies(0),
      drawAfterConquer(true) {}

Airlift::Airlift(Player* player, Territory* sourceTerritory,
                 Territory* targetTerritory, int numberOfArmies)
    : Order(player),
      sourceTerritory(sourceTerritory),
      targetTerritory(targetTerritory),
      drawAfterConquer(true) {
  this->numberOfArmies =
      std::max(0, std::min(numberOfArmies, sourceTerritory->GetTroops()));
}

Airlift::Airlift(const Airlift& toCopy) : Order(toCopy) {
  this->sourceTerritory = toCopy.sourceTerritory;
  this->targetTerritory = toCopy.targetTerritory;
  this->numberOfArmies = toCopy.numberOfArmies;
  this->drawAfterConquer = toCopy.drawAfterConquer;
}

Airlift::~Airlift() {}

Airlift& Airlift::operator=(const Airlift& rightSide) {
  Order::operator=(rightSide);
  sourceTerritory = rightSide.sourceTerritory;
  targetTerritory = rightSide.targetTerritory;
  numberOfArmies = rightSide.numberOfArmies;
  drawAfterConquer = rightSide.drawAfterConquer;
  return *this;
}

bool Airlift::validate() {
  bool baseValidation{Order::validate()};
  bool playerOwnsSource =
      (sourceTerritory != NULL && sourceTerritory->GetPlayer() == player);
  std::cout << "Player owns source territory: " << playerOwnsSource
            << std::endl;
  return (baseValidation && playerOwnsSource);
}

void Airlift::execute() {
  if (!validate()) {
    std::cout << "AIRLIFT ORDER: disabled or invalid" << std::endl;
    return;
  }
  wasExecuted = true;

  std::cout << "AIRLIFT ORDER: ";
  MoveTroops moveTroops(player, sourceTerritory, targetTerritory,
                        numberOfArmies);
  moveTroops.ExecuteTheMove();
  if (moveTroops.AttackerConqueredTarget() && drawAfterConquer) {
    // TODO Player draws card
    DisableCardDrawVisitor disableDrawVisitor(player);
    player->GetOrdersList()->visitOrders(&disableDrawVisitor);
    player->DrawCard();
    std::cout << " A card was drawn and added to " << player << "'s hand.";
  } else {
    std::cout << " No card was drawn";
  }
  std::cout << std::endl;
}

void Airlift::acceptVisitor(OrdersVisitor* visitor) {
  visitor->VisitAirlift(this);
}

void Airlift::disableDrawAfterConquer() { drawAfterConquer = false; }

Player* Airlift::getOpponent() { return targetTerritory->GetPlayer(); }

void Airlift::setSourceTerritory(Territory* t) { sourceTerritory = t; }

void Airlift::setTargetTerritory(Territory* t) { targetTerritory = t; }

void Airlift::setNumberOfArmies(int n) { numberOfArmies = std::max(0, n); }

std::ostream& Airlift::doPrint(std::ostream& out) const {
  std::cout << "Airlift order from " << *sourceTerritory->GetName() << " to "
            << *targetTerritory->GetName();
  return out;
}

std::ostream& operator<<(std::ostream& out, const Airlift& toOutput) {
  return toOutput.doPrint(out);
}

// Class that moves troops and attacks if necessary
MoveTroops::MoveTroops()
    : source(nullptr),
      target(nullptr),
      player(nullptr),
      numberOfArmies(0),
      wasExecuted(false),
      attackerConquered(false) {}

MoveTroops::MoveTroops(const MoveTroops& toCopy) {
  this->player = toCopy.player;
  this->source = toCopy.source;
  this->target = toCopy.target;
  this->numberOfArmies = toCopy.numberOfArmies;
  this->wasExecuted = toCopy.wasExecuted;
  this->attackerConquered = toCopy.attackerConquered;
}

MoveTroops& MoveTroops::operator=(const MoveTroops& rightSide) {
  player = rightSide.player;
  source = rightSide.source;
  target = rightSide.target;
  numberOfArmies = rightSide.numberOfArmies;
  wasExecuted = rightSide.wasExecuted;
  attackerConquered = rightSide.attackerConquered;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const MoveTroops& toOutput) {
  out << "Move " << toOutput.numberOfArmies << " troops object from "
      << toOutput.source->GetName() << " to " << toOutput.target->GetName();
  return out;
}

MoveTroops::MoveTroops(Player* player, Territory* source, Territory* target,
                       int numberOfArmies)
    : player(player),
      source(source),
      target(target),
      wasExecuted(false),
      attackerConquered(false) {
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
  std::cout << "Moved " << numberOfArmies << " from " << *source->GetName()
            << " to " << *target->GetName() << std::endl;
}

void MoveTroops::AttackTarget() {
  // Initialize random seed
  srand(time(NULL));
  int ennemyKilledArmies{0};
  int attackerKilledArmies{0};
  int numberOfAttacks{numberOfArmies + target->GetTroops()};

  for (int i = 0; i < numberOfAttacks; i++) {
    if (i % 2 == 0) {
      if (AttackerKilledDefenderArmy()) {
        target->RemoveTroops(1);
        ennemyKilledArmies++;
      }
    } else {
      if (DefenderKilledAttackerArmy()) {
        source->RemoveTroops(1);
        numberOfArmies--;
        attackerKilledArmies++;
      }
    }
    if (target->GetTroops() == 0 || numberOfArmies == 0) {
      break;
    }
  }
  std::cout << "Attacker killed " << ennemyKilledArmies << " ennemy armies on "
            << *target->GetName() << ", " << attackerKilledArmies
            << " attacking armies from " << *source->GetName()
            << " were killed.";

  // If no troops left in target, player takes ownership
  // and we assign the number of surviving armies to the territory
  if (target->GetTroops() == 0) {
    Player* opponent = target->GetPlayer();
    opponent->RemoveTerritoryFromPlayer(target);
    player->AddTerritoryToPlayer(target);
    target->SetPlayer(player);

    target->SetTroops(numberOfArmies);
    attackerConquered = true;
    std::cout << " " << player << " conquered " << *target->GetName()
              << " which now contains " << numberOfArmies << " troops.";
  }
}

// Returns true if rand() is in [0, 6[ over the range [0, 10[
bool MoveTroops::AttackerKilledDefenderArmy() { return (rand() % 10 < 6); }

// Returns true if rand() is in [0, 7[ over the range [0, 10[
bool MoveTroops::DefenderKilledAttackerArmy() { return (rand() % 10 < 7); }

bool MoveTroops::AttackerConqueredTarget() { return attackerConquered; }

// Visitors
// The default visitor does nothing, any operation must be defined in subtypes
OrdersVisitor::~OrdersVisitor() {}
std::ostream& operator<<(std::ostream& out, const OrdersVisitor& toOutput) {
  out << "Base orders visitor";
  return out;
}
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

std::ostream& operator<<(std::ostream& out, const NegotiateVisitor& toOutput) {
  out << "Negotiate visitor that will disable all attacks between "
      << toOutput.player << " and " << toOutput.opponent;
  return out;
}

void NegotiateVisitor::VisitAdvance(Advance* order) {
  if (order->getPlayer() == player && order->getOpponent() == opponent) {
    order->disableOrder();
  }
}

void NegotiateVisitor::VisitAirlift(Airlift* order) {
  if (order->getPlayer() == player && order->getOpponent() == opponent) {
    order->disableOrder();
  }
}

void NegotiateVisitor::VisitBomb(Bomb* order) {
  if (order->getPlayer() == player && order->getOpponent() == opponent) {
    order->disableOrder();
  }
}

// Visitor that disables card drawing after an attack
DisableCardDrawVisitor::DisableCardDrawVisitor() : player() {}
DisableCardDrawVisitor::DisableCardDrawVisitor(Player* player)
    : player(player) {}
DisableCardDrawVisitor::DisableCardDrawVisitor(DisableCardDrawVisitor& toCopy) {
  this->player = toCopy.player;
}
DisableCardDrawVisitor& DisableCardDrawVisitor::operator=(
    const DisableCardDrawVisitor& rightSide) {
  player = rightSide.player;
  return *this;
}

DisableCardDrawVisitor::~DisableCardDrawVisitor() {}

std::ostream& operator<<(std::ostream& out,
                         const DisableCardDrawVisitor& toOutput) {
  out << "Disable card draw visitor that will disable card draw for "
      << toOutput.player;
  return out;
}

void DisableCardDrawVisitor::VisitAdvance(Advance* order) {
  if (order->getPlayer() == player) {
    order->disableDrawAfterConquer();
  }
}
void DisableCardDrawVisitor::VisitAirlift(Airlift* order) {
  if (order->getPlayer() == player) {
    order->disableDrawAfterConquer();
  }
}

// Configure order visitor
ConfigureOrdersVisitor::ConfigureOrdersVisitor() : player() {}
ConfigureOrdersVisitor::ConfigureOrdersVisitor(Player* player)
    : player(player) {}
ConfigureOrdersVisitor::ConfigureOrdersVisitor(ConfigureOrdersVisitor& toCopy) {
  this->player = toCopy.player;
}
ConfigureOrdersVisitor& ConfigureOrdersVisitor::operator=(
    const ConfigureOrdersVisitor& rightSide) {
  player = rightSide.player;
  return *this;
}
ConfigureOrdersVisitor::~ConfigureOrdersVisitor() {}

void ConfigureOrdersVisitor::VisitDeploy(Deploy* order) {
  order->setTerritoryToDeploy(player->ToDefend()[0]);
  order->setNumberOfArmies(player->GetReinforcementPoolCount());
}
void ConfigureOrdersVisitor::VisitAirlift(Airlift* order) {
  const std::vector<Territory*>* territories = player->GetOwnedTerritories();
  int troops = 0;
  int i = 0;
  Territory* source = nullptr;
  // 12 attemps
  while ((troops < 1) && (i < 12)) {
    source = territories->at(rand() % territories->size());
    troops = source->GetAvailableTroops();
  }
  order->setNumberOfArmies(troops);
  order->setSourceTerritory(source);
  order->setTargetTerritory(territories->at(rand() % territories->size()));
}
void ConfigureOrdersVisitor::VisitBomb(Bomb* order) { 
  Territory* target = nullptr;
  player->GenerateToAttack();
  std::vector<Territory*> territories = player->ToAttack();
  if (territories.size() > 0) {
    target = territories.at(0);
    for (Territory* t : player->ToAttack()) {
      if (t->GetTroops() > target->GetTroops()) {
        target = t;
      }
    }
  }
  order->setTargetTerritory(target);
}
void ConfigureOrdersVisitor::VisitBlockade(Blockade* order) {
  order->setTerritory(player->GetOwnedTerritories()->at(rand() % player->GetOwnedTerritories()->size()));
}
void ConfigureOrdersVisitor::VisitNegotiate(Negotiate* order) {
  order->setOpponent(player->GetGameEngine()->getPlayers().at(rand() % player->GetGameEngine()->getPlayers().size()));
}