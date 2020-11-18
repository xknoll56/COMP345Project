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

#include "Player.h"

#include <algorithm>

Player::Player()
    : gameEngine(), listOfOrders(), phase(Phase::None), reinforcementPool(0) {}

Player::Player(GameEngine* gameEngine)
    : gameEngine(gameEngine),
      ownedTerritories(std::vector<Territory*>(0)),
      toAttack(std::vector<Territory*>(0)),
      handOfCards(std::vector<Card*>(0)),
      listOfOrders(new OrdersList()),
      reinforcementPool(0),
      phase(Phase::None) {}
// Parametric constructor
Player::Player(GameEngine* gameEngine, std::vector<Territory*> terr,
               int numberOfArmies)
    : gameEngine(gameEngine),
      handOfCards(std::vector<Card*>(0)),
      reinforcementPool(numberOfArmies),
      listOfOrders(new OrdersList()),
      phase(Phase::None) {
  for (Territory* t : terr) this->AddTerritoryToPlayer(t);
}

// Copy constructor
Player::Player(const Player& pCopy) {
  gameEngine = pCopy.gameEngine;
  for (Territory* t : pCopy.ownedTerritories) this->AddTerritoryToPlayer(t);
  handOfCards = pCopy.handOfCards;
  listOfOrders = pCopy.listOfOrders;
  reinforcementPool = pCopy.reinforcementPool;
}

// Assignment operator
Player& Player::operator=(const Player& rightP) {
  if (&rightP != this) {
    for (Territory* t : rightP.ownedTerritories) this->AddTerritoryToPlayer(t);
    handOfCards = rightP.handOfCards;
    listOfOrders = rightP.listOfOrders;
    reinforcementPool = rightP.reinforcementPool;
  }
  return *this;
}
// Destructor
Player::~Player() {
  for (auto terr : ownedTerritories) {
    // delete terr;
    // The territories should not be deleted here, only the pointers to them.
    terr = NULL;
  }
  ownedTerritories.clear();
  for (auto cards : handOfCards) {
    delete cards;
    cards = NULL;
  }
  handOfCards.clear();
  delete listOfOrders;
  listOfOrders = nullptr;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& out, const Player& toOutput) {
  out << "Player has " << toOutput.ownedTerritories.size() << " territories, "
      << toOutput.handOfCards.size() << " cards, " << toOutput.reinforcementPool
      << " armies in its reinforcement pool and " << *(toOutput.listOfOrders);
  return out;
}

// Returns a vector of pointers of territories to defend
std::vector<Territory*> Player::ToDefend() {
  return ownedTerritories;
}

void Player::GenerateToDefend() {
  // TODO - priority list rather than randomizing.
  std::random_shuffle(ownedTerritories.begin(), ownedTerritories.end());
}

// Returns a vector of pointers of territories to attack
// TODO - IMPORTANT TODO BEFORE A2 SUBMISSION!!! this should only return
// neighbors!
void Player::GenerateToAttack() {
  const std::vector<Territory*>* const vectorAllTerritories =
      gameEngine->GetMap()->GetTerritories();
  std::vector<Territory*> territoriesToAttack;
  Territory* territory;

  for (int i = 0; i < vectorAllTerritories->size(); i++) {
    territory = vectorAllTerritories->at(i);
    if (territory->GetPlayer() != this) {
      if (territory->IsNeighborTo(this)) {
        territoriesToAttack.push_back(territory);
      }
    }
  }
  // TODO - return this list as a priority list rather than randomizing.
  std::random_shuffle(territoriesToAttack.begin(), territoriesToAttack.end());
  toAttack =  territoriesToAttack;
}

std::vector<Territory*> Player::ToAttack() { return toAttack; }

// Creates an Order object and adds it to the vector of pointers of orders
bool Player::IssueOrder() {
  std::cout << "        Issuing an Order..." << std::endl;
  // TODO - Why are they making us call toAttack() every time??
  phase = Phase::IssueOrders;
  this->Notify();
  if (reinforcementPool > 0) {
    std::cout << "            Issuing a Deploy Order..." << std::endl;
    // TODO - Deploy based on threat level
    Territory* territory = ToDefend().at(rand() % ToDefend().size());
    territory->IncreaseToDeploy(1);
    AddOrderToPlayer(new Deploy(this, territory, 1));  // TODO - make sure these are getting destroyed!
    reinforcementPool--;
    return true;
  }
  if (handOfCards.size() > 0) {
    std::cout << "            Issuing a Card Order..." << std::endl;
    handOfCards.back()->play();
    handOfCards.pop_back();
    return true;
  }
  while (toAttack.size() > 0) {
    std::cout << "            Issuing an Advance (Attack) Order..." << std::endl;
    std::cout << "_attacking: " << *ToAttack().back()->GetName() << std::endl;
    int troops;
    for (Territory* t : *ToAttack().back()->GetNeighbors()) {
      troops = t->GetAvailableTroops();
      if ((troops > 0) && (t->GetPlayer() == this)) {
        t->IncreaseStandByTroops(troops);
        AddOrderToPlayer(new Advance(this, t, ToAttack().back(), troops));
        std::cout << "_success" << std::endl;
        return true;
      }
    }
    std::cout << "_fail" << std::endl;
    toAttack.pop_back();
  }
  // TODO - DEFEND;
  phase = Phase::None;
  return false;
}

// Adds the given territory pointer to the vector of owned territories
void Player::AddTerritoryToPlayer(Territory* territoryToAdd) {
  std::vector<Territory*>::iterator it = std::find(
      ownedTerritories.begin(), ownedTerritories.end(), territoryToAdd);
  if (it == ownedTerritories.end()) {
    ownedTerritories.push_back(territoryToAdd);
    territoryToAdd->SetPlayer(this);
    Notify();
  }
}

void Player::RemoveTerritoryFromPlayer(Territory* territoryToRemove) {
  std::vector<Territory*>::iterator it = std::find(
      ownedTerritories.begin(), ownedTerritories.end(), territoryToRemove);
  if (it != ownedTerritories.end()) {
    ownedTerritories.erase(it);
    territoryToRemove->SetPlayer(nullptr);
    Notify();
  }
}
// Adds the given card pointer to the vector of cards
void Player::AddCardToPlayer(Card* cardToAdd) {
  handOfCards.push_back(cardToAdd);
}
// Adds the given order pointer to the vector of orders
void Player::AddOrderToPlayer(Order* orderToAdd) {
  listOfOrders->queue(orderToAdd);
}

void Player::AddArmiesToReinforcementPool(int numberOfArmies) {
  // this->NotifyReinforcements(numberOfArmies);
  phase = Phase::Reinforcement;
  this->Notify();

  reinforcementPool += std::max(0, numberOfArmies);
  phase = Phase::None;
}

int Player::GetReinforcementPoolCount() { return reinforcementPool; }

// Removes a certain number of armies from the pool
// Returns the number of armies removed, which may be different
// (i.e. if less armies left than was asked for)
int Player::TakeArmiesFromReinforcementPool(int requestedNumberOfArmies) {
  int toReturn{std::min(requestedNumberOfArmies, reinforcementPool)};
  reinforcementPool =
      std::max(0, (reinforcementPool - requestedNumberOfArmies));
  return toReturn;
}

const std::vector<Territory*>* Player::GetOwnedTerritories() {
  return &ownedTerritories;
}

void Player::SetReinforcementPool(int amount) {
  reinforcementPool = std::max(0, amount);
}

bool Player::ExecuteNextOrder() {
  std::cout << "        Executing an Order..." << std::endl;
  phase = Phase::ExecuteOrders;
  this->Notify();
  Order* order = listOfOrders->popNextOrder();
  order->execute();
  delete order;
  phase = Phase::None;
  if (listOfOrders->getListSize() > 0) {
    return true;
  }
  return false;
}

void Player::DrawCard() { gameEngine->PlayerDrawCard(this); }

Phase Player::GetPhase() { return phase; }

OrdersList* Player::GetOrdersList() { return listOfOrders; }