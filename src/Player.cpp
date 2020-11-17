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

Player::Player() {}

Player::Player(Map* map)
    : 
      map(map),
      ownedTerritories(std::vector<Territory*>(0)),
      handOfCards(std::vector<Card*>(0)),
      listOfOrders(new OrdersList()),
      reinforcementPool(0),
      phase(Phase::None) {}
// Parametric constructor
Player::Player(Map* map, std::vector<Territory*> terr, int numberOfArmies)
    : map(map),
      handOfCards(std::vector<Card*>(0)),
      reinforcementPool(numberOfArmies),
      listOfOrders(new OrdersList()),
      phase(Phase::None) {
  for (Territory* t : terr) this->AddTerritoryToPlayer(t);
}

// Copy constructor
Player::Player(const Player& pCopy) {
  map = pCopy.map;
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
std::vector<Territory*> Player::ToDefend() { return ownedTerritories; }

// Returns a vector of pointers of territories to attack
// TODO - IMPORTANT TODO BEFORE A2 SUBMISSION!!! this should only return neighbors!
std::vector<Territory*> Player::ToAttack() {
  const std::vector<Territory*>* const vectorAllTerritories =
      map->GetTerritories();
  std::vector<Territory*> territoriesToAttack;
  Territory* territory;

  for (int i = 0; i < vectorAllTerritories->size(); i++) {
    territory = vectorAllTerritories->at(i);
    if (territory->GetPlayer() != this) {
      territoriesToAttack.push_back(territory);
    }
  }
  return territoriesToAttack;
}

// Creates an Order object and adds it to the vector of pointers of orders
bool Player::IssueOrder() {
  // TODO - Why are they making us call toAttack() every time??
  phase = Phase::IssueOrders;
  this->Notify();
  std::vector<Territory*> toAttack(ToAttack());
  std::vector<Territory*> toDefend(ToDefend());
  std::string input;
  int x = -1;
  Territory* territory = nullptr;
  std::cout << GetName() << ", it's your turn." << std::endl;
  if (reinforcementPool > 0) { // TODO - This entire part needs to change to use the Deploy Order
    std::cout << "You have " << reinforcementPool << " troops left to deploy." << std::endl;
    std::cout << "choose a territory to deploy troops to:" << std::endl;
    for (int i = 0; i < toDefend.size(); i++) {
      territory = toDefend.at(i);
      std::cout << "[" << i << "] " << *territory->GetName() << ": "
                << territory->GetTroops() << " troops."
                << std::endl;
    }
    while ((x < 0) || (x >= toDefend.size())) {
      std::cout << "> ";
      std::cin >> input;
      try {
        x = std::stoi(input);
      } catch (...) {
        continue;
      }
    }
    territory = toDefend.at(x);
    x = -1;
    std::cout << "How many troops to deploy on " << *territory->GetName() << "? (maximum " << reinforcementPool << ")" << std::endl;
    while ((x < 1) || (x > reinforcementPool)) {
      std::cout << "> ";
      std::cin >> input;
      try {
        x = std::stoi(input);
      } catch (...) {
        continue;
      }
    }
    std::cout << *territory->GetName() << " troops: " << territory->GetTroops()
              << "->" << territory->GetTroops() + x << std::endl;
    std::cout << GetName() << " Reinforcement pool: " << reinforcementPool
              << "->" << reinforcementPool - x << std::endl;
    reinforcementPool -= x;
    territory->AddTroops(x);
  } 
  else {
    while (true) {
      std::cout << "Make a selection:\n[1] Play a card.\n[2] Advance troops.\n[3] Commit orders." << std::endl;
      while ((x < 1) || (x > 3)) {
        std::cout << "> ";
        std::cin >> input;
        try {
          x = std::stoi(input);
        } catch (...) {
          continue;
        }
      }
      switch (x) { 
      case 1: // Play Card
        x = -1;
        if (handOfCards.size() > 0) {
          std::cout << "Choose a card to play:" << std::endl;
          for (int i = 0; i < handOfCards.size(); i++) {
            std::cout << "[" << i << "] " << handOfCards.at(i) << std::endl; // TODO - Give names to cards ?
            while ((x < 0) || (x >= handOfCards.size())) {
              std::cout << "> ";
              std::cin >> input;
              try {
                x = std::stoi(input);
              } catch (...) {
                continue;
              }
            }

            // TODO - add card order to orders list
            handOfCards.erase(handOfCards.begin() + i); // TODO - should i return this card to the deck?
          }
        } else {
          std::cerr << "You don't have any cards!" << std::endl;
        }
        break;
      case 2: // Advance Troops
        // TODO
        x = -1;
        break;
      case 3: // Commit Orders.
        x = -1;
        return false;
      default: // Error
        std::cerr << "Error: the input was invalid" << std::endl;
        std::cin;
        exit(-1);
      }
    }
  }

  AddOrderToPlayer(new Deploy(this, this->ownedTerritories[0], 0));
  
  phase = Phase::None;
  return true;
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

void Player::SetReinforcementPool(int amount) { reinforcementPool = std::max(0, amount); }

bool Player::ExecuteNextOrder() {
  phase = Phase::ExecuteOrders;
  this->Notify();
  // Find the order with the highest priority.
  // Execute it.
  // Erase it from the list of orders.
  // return true if there are still orders to be executed.
  phase = Phase::None;
  return false;
}

Phase Player::GetPhase() { return phase; }

OrdersList* Player::GetOrdersList() { return listOfOrders; }