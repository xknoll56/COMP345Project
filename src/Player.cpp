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

// Default constructor
Player::Player()
    : ownedTerritories(std::vector<Territory*>(0)),
      handOfCards(std::vector<Card*>(0)),
      listOfOrders(new OrdersList()),
      reinforcementPool(0) {}
// Parametric constructor
Player::Player(std::vector<Territory*> terr, int numberOfArmies)
    : handOfCards(std::vector<Card*>(0)),
      reinforcementPool(numberOfArmies),
      listOfOrders(new OrdersList()) {
  for (Territory* t : terr) this->AddTerritoryToPlayer(t);
}

// Copy constructor
Player::Player(const Player& pCopy) {
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
    delete terr;
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
std::vector<Territory*> Player::toDefend() { return ownedTerritories; }

// Returns a vector of pointers of territories to attack
std::vector<Territory*> Player::toAttack(Map& map) {
  const std::vector<Territory*>* const vectorAllTerritories =
      map.GetTerritories();
  std::vector<Territory*> territoriesToAttack;

  for (int i = 0; i < vectorAllTerritories->size(); i++) {
    for (int j = 0; j < ownedTerritories.size(); j++) {
      if (*(ownedTerritories[j]->GetName()) !=
          *(vectorAllTerritories->at(i)->GetName()))
        territoriesToAttack.push_back(vectorAllTerritories->at(i));
    }
  }
  return territoriesToAttack;
}

// Creates an Order object and adds it to the vector of pointers of orders
bool Player::IssueOrder() {
  // Creating a Deploy order
  AddOrderToPlayer(new Deploy(this, this->ownedTerritories[0], 0));
  if (listOfOrders->GetList()->size() > 12) {
    return false;
  }
  return true;
}

// Adds the given territory pointer to the vector of owned territories
void Player::AddTerritoryToPlayer(Territory* territoryToAdd) {
  ownedTerritories.push_back(territoryToAdd);
  territoryToAdd->SetPlayer(this);
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
  reinforcementPool += std::max(0, numberOfArmies);
}

int Player::GetReinforcementPoolCount() { return reinforcementPool; }

// Removes a certain number of armies from the pool
// Returns the number of armies removed, which may be different
// (i.e. if less armies left than was asked for)
int Player::TakeArmiesFromReinforcementPool(int requestedNumberOfArmies) {
  requestedNumberOfArmies = std::max(0, requestedNumberOfArmies);
  if (requestedNumberOfArmies > reinforcementPool) {
    int toReturn{reinforcementPool};
    reinforcementPool = 0;
    return reinforcementPool;
  }
  reinforcementPool -= requestedNumberOfArmies;
  return requestedNumberOfArmies;
}

const std::vector<Territory*>* Player::GetOwnedTerritories() {
  return &ownedTerritories;
}

void Player::SetReinforcementPool(int amount) { reinforcementPool = amount; }