#include "PlayerStrategies.h"

// Player strategies
PlayerStrategy::PlayerStrategy() : player(nullptr) {}
PlayerStrategy::PlayerStrategy(Player* player) : player(player) {}
PlayerStrategy::PlayerStrategy(const PlayerStrategy& toCopy) {
  this->player = toCopy.player;
}
PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& rightSide) {
  player = rightSide.player;
  return *this;
}
PlayerStrategy::~PlayerStrategy() {}
std::ostream& operator<<(std::ostream& out, const PlayerStrategy& toOutput) {
  out << "Abstract player strategy";
  // temporary fix
  return out;
}
void PlayerStrategy::setPlayer(Player* player) { this->player = player; }

// Human player strategy
HumanPlayerStrategy::HumanPlayerStrategy() : PlayerStrategy() {}
HumanPlayerStrategy::HumanPlayerStrategy(Player* player)
    : PlayerStrategy(player) {}
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& toCopy)
    : PlayerStrategy(toCopy) {}
HumanPlayerStrategy& HumanPlayerStrategy::operator=(
    const HumanPlayerStrategy& rightSide) {
  player = rightSide.player;
  return *this;
}
HumanPlayerStrategy::~HumanPlayerStrategy() {}
std::ostream& operator<<(std::ostream& out,
                         const HumanPlayerStrategy& toOutput) {
  out << "Human player strategy";
  return out;
}

bool HumanPlayerStrategy::issueOrder() {
    if (player->GetReinforcementPoolCount() > 0) {
        /* Displays the available territories to deploy to */
        std::vector<Territory*> terrsToDeploy = *(player->GetOwnedTerritories());
        std::cout << "Here are the territories available for a Deploy order: " << std::endl;
        for (int i = 0; i < terrsToDeploy.size(); i++) 
            std::cout << "\t" << (i+1) << " - " << *(terrsToDeploy[i]->GetName()) << std::endl;

        /* Prompting the player for a choice */
        int territoryNum = HumanPlayerStrategy::promptForValueInRange(1, terrsToDeploy.size());
        std::vector<Territory*> territoryToDeployTo;
        territoryToDeployTo.push_back(terrsToDeploy[territoryNum-1]);

        std::cout << "\nYou currently have a reinforcement pool of " << player->GetReinforcementPoolCount() 
            << " troops.\nHow many troops do you want to deploy?" << std::endl;
        int numTroops = HumanPlayerStrategy::promptForValueInRange(0, player->GetReinforcementPoolCount());

        Order* deployOrderToAdd = new Deploy(player, territoryToDeployTo[0], numTroops); // creating an Deploy order
        territoryToDeployTo[0]->IncreaseToDeploy(numTroops); // increases the number of deployed troops to the territory
        player->TakeArmiesFromReinforcementPool(numTroops); // removes the number of armies used from the reinforcement pool
        player->AddOrderToPlayer(deployOrderToAdd); // adding the order to the player's list of orders

        std::cout << "\n--- A Deploy order was created on "
            << *(territoryToDeployTo[0]->GetName()) << " with " << numTroops 
            << " troops and added to the list of orders of Player " << player->GetName() << ".\n" << std::endl;
        territoryToDeployTo.pop_back(); // making sure the vector never has more than one element
        return true;
    }
    else {
        std::cout << "Choose an action:\n "
            "1 - Advance\n "
            "2 - Play a card\n "
            "3 - Commit\n";
        int choice = HumanPlayerStrategy::promptForValueInRange(1, 3);

        // Advance
        if (choice == 1) {
            /* Displays the player's choices of source territories */
            std::vector<Territory*> ownedTerrs = *(player->GetOwnedTerritories());
            std::cout << "\nHere is a list of source territories to advance from: " << std::endl;
            int counter = 0;
            std::vector<int> indexSource;
            for (int i = 0; i < ownedTerrs.size(); i++) {
                if (ownedTerrs[i]->GetAvailableTroops() > 0) {
                    std::cout << "\t" << counter + 1 << " - " << *(ownedTerrs[i]->GetName())
                        << "  (" << ownedTerrs[i]->GetAvailableTroops() << " troops)" << std::endl;
                    counter++;
                    indexSource.push_back(i); // keeps the index of the source territory in ownedTerrs
                }
            }
            if (counter == 0) {
                std::cout << "\tAll available troops have been moved." 
                    << "--- No Advance order was created.\n" << std::endl;
                return true;
            }

            std::vector<Territory*> sourceTerritory;
            int source = HumanPlayerStrategy::promptForValueInRange(1, counter);
            sourceTerritory.push_back(ownedTerrs[indexSource[source-1]]);

            /* Displays the player's choice of target territories */
            std::vector<Territory*> adjacentTerrsToAttack = player->GetAdjacentTerritories();
            std::cout << "\nHere is a list of target territories to advance to: " << std::endl;
            for (int i = 0; i < adjacentTerrsToAttack.size(); i++) {
                std::cout << "\t" << i + 1 << " - " << *(adjacentTerrsToAttack[i]->GetName())
                    << " (" << adjacentTerrsToAttack[i]->GetAvailableTroops() << " troops)" << std::endl;
            }
            std::vector<Territory*> targetTerritory;
            int target = HumanPlayerStrategy::promptForValueInRange(1, adjacentTerrsToAttack.size());
            targetTerritory.push_back(adjacentTerrsToAttack[target - 1]);


            /* Validation of number of troops to use for Advance order */
            std::cout << "\nYou currently have " << sourceTerritory[0]->GetAvailableTroops()
                << " troops available in " << *(sourceTerritory[0]->GetName()) 
                << ".\nHow many troops do you want to advance?" << std::endl;
            int numberTroops = HumanPlayerStrategy::promptForValueInRange(0, sourceTerritory[0]->GetAvailableTroops());
            std::cout << std::endl;

            /* Creating an Advance order and adding it to the player's list of orders */
            Order* advance = new Advance(player, sourceTerritory[0], targetTerritory[0], numberTroops);
            sourceTerritory[0]->IncreaseStandByTroops(numberTroops); // increase standByTroops by the number of troops used for the advance
            player->AddOrderToPlayer(advance); 

            /* Outputs information about the order that was created */
            std::cout << "--- An Advance order was created from " 
                << *(sourceTerritory[0]->GetName()) << " to " << *(targetTerritory[0]->GetName()) 
                << " with " << numberTroops << " troops and added to the list of orders of Player " << player->GetName() << ".\n" << std::endl;
            sourceTerritory.pop_back(); // making sure both vectors never have more than one element
            targetTerritory.pop_back();
            return true;
        }
        // Play a card
        else if (choice == 2) {
            
            std::vector<Card*> playerCards = *(player->GetHand());
            if (playerCards.size() == 0) {
                std::cout << "\n\tYou don't have any cards to play.\n--- No card was played.\n" << std::endl;
                return true;
            }

            std::cout << "\nChoose a card to play:\n";
            for (int i = 0; i < playerCards.size(); i++) {
                std::cout << "\t" << i + 1 << " - ";
                playerCards[i]->print(std::cout);
                std::cout << "\n";
            }
            int cardChoice = HumanPlayerStrategy::promptForValueInRange(0, playerCards.size());

            player->PlayCard(cardChoice); // plays the given card, which in turn adds the order to the list of orders
            std::cout << "\n--- A card was played.\n" << std::endl;
            return true;
        }
        // Commit (finish)
        else
            return false;
    }
}
std::vector<Territory*> HumanPlayerStrategy::toDefend() {
    std::cout << "Here is a list of territories to defend: " << std::endl;
    std::vector<Territory*> allOwnedTerrs = *(player->GetOwnedTerritories());
    for (int i = 0; i < allOwnedTerrs.size(); i++) // displays all the territories to defend
        std::cout << "\t" << i+1 << " - " << *(allOwnedTerrs[i]->GetName()) << std::endl;

    std::vector<Territory*> terrToDefend;
    int territoryName = HumanPlayerStrategy::promptForValueInRange(1, allOwnedTerrs.size());
    terrToDefend.push_back(allOwnedTerrs[territoryName - 1]);

    return terrToDefend;
}
std::vector<Territory*> HumanPlayerStrategy::toAttack() {
    std::cout << "Here is a list of territories to attack: " << std::endl;
    std::vector<Territory*> allTerrsToAttack = player->GetAdjacentTerritories();
    for (int i = 0; i < allTerrsToAttack.size(); i++) // displays all the territories to attack
        std::cout << "\t" << i + 1 << " - " << *(allTerrsToAttack[i]->GetName()) << std::endl;

    std::vector<Territory*> terrToAttack;
    int territoryName = HumanPlayerStrategy::promptForValueInRange(1, allTerrsToAttack.size());
    terrToAttack.push_back(allTerrsToAttack[territoryName - 1]);

    return terrToAttack;
}

int HumanPlayerStrategy::promptForValueInRange(int min, int max) {
  int choice{-1};
  while (true) {
    std::cout << "Enter your choice (" << min << " - " << max << ") : ";
    std::cin >> choice;
    if (choice < min || choice > max)
        std::cout << " !! Invalid choice. Try again.\n" << std::endl;
    else
        break;
  }
  return choice;
}

// Aggressive player strategy
AggressivePlayerStrategy::AggressivePlayerStrategy() : PlayerStrategy() {}
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* player)
    : PlayerStrategy(player) {}
AggressivePlayerStrategy::AggressivePlayerStrategy(
    const AggressivePlayerStrategy& toCopy)
    : PlayerStrategy(toCopy) {}
AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(
    const AggressivePlayerStrategy& rightSide) {
  player = rightSide.player;
  return *this;
}
AggressivePlayerStrategy::~AggressivePlayerStrategy() {}
std::ostream& operator<<(std::ostream& out,
                         const AggressivePlayerStrategy& toOutput) {
  out << "Aggressive player strategy";
  return out;
}

bool AggressivePlayerStrategy::issueOrder() {
  std::vector<Territory*> td = toDefend();
  if (player->GetReinforcementPoolCount() > 0) {
    if (td.size() > 0) {  // Deploy all troops on highest priority territory.
      Territory* territory = toDefend().at(0);
      player->AddOrderToPlayer(
          new Deploy(player, territory, player->GetReinforcementPoolCount()));
      territory->IncreaseToDeploy(player->GetReinforcementPoolCount());
      player->SetReinforcementPool(0);
      return true;
    }
  }
  // We only play cards for the human strategy
  //if (player->GetHand()->size() > 0) {
  //  player->PlayCard(0);
  //  return true;
  //}
  int troops;
  for (Territory* target : td) {
    for (Territory* source : *target->GetNeighbors()) {
      troops = source->GetAvailableTroops();
      if ((source->GetPlayer() == player) && (troops > 0) &&
          (Territory::HasMoreTroops(target, source))) {
        source->IncreaseStandByTroops(troops);
        player->AddOrderToPlayer(new Advance(player, source, target, troops));
        return true;
      }
    }
  }
  std::vector<Territory*> ta = toAttack();
  for (Territory* target : ta) {
    for (Territory* source : *target->GetNeighbors()) {
      troops = source->GetAvailableTroops();
      if ((troops > 0) && (source->GetPlayer() == player)) {
        source->IncreaseStandByTroops(troops);
        player->AddOrderToPlayer(new Advance(player, source, target, troops));
        return true;
      }
    }
  }
  return false;
}
std::vector<Territory*> AggressivePlayerStrategy::toDefend() {
  std::vector<Territory*> territories = *player->GetOwnedTerritories();
  std::random_shuffle(territories.begin(), territories.end());
  // Sorts territories by number of troops (Decreasing)
  std::stable_sort(territories.begin(), territories.end(),
                   Territory::HasMoreTroops);
  return territories;
}
std::vector<Territory*> AggressivePlayerStrategy::toAttack() {
  std::vector<Territory*> territories = player->GetAdjacentTerritories();
  std::random_shuffle(territories.begin(), territories.end());
  // Sorts territories by number of troops (Decreasing)
  std::stable_sort(territories.begin(), territories.end(),
                   Territory::HasMoreTroops);
  return territories;
}

// Benevolent Player Strategy
BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy() {}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* player)
    : PlayerStrategy(player) {}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(
    const BenevolentPlayerStrategy& toCopy)
    : PlayerStrategy(toCopy) {}
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(
    const BenevolentPlayerStrategy& rightSide) {
  player = rightSide.player;
  return *this;
}
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}
std::ostream& operator<<(std::ostream& out,
                         const BenevolentPlayerStrategy& toOutput) {
  out << "Benevolent player strategy";
  return out;
}

bool BenevolentPlayerStrategy::issueOrder() {
  std::vector<Territory*> td = toDefend();
  if (player->GetReinforcementPoolCount() > 0) {
    if (td.size() > 0) {  // Deploy a troop on highest priority territory.
      Territory* territory = toDefend().at(0);
      player->AddOrderToPlayer(new Deploy(player, territory, 1));
      territory->IncreaseToDeploy(1);
      player->TakeArmiesFromReinforcementPool(1);
      return true;
    }
  }
  // We only play cards for the human strategy
  //if (player->GetHand()->size() > 0) {
  //  player->PlayCard(0);
  //  return true;
  //}
  int troops;
  for (Territory* target : td) {
    int targetTroops = target->GetAvailableTroops();
    for (Territory* source : *target->GetNeighbors()) {
      troops = source->GetAvailableTroops();
      if ((source->GetPlayer() == player) && (troops > targetTroops + 1)) {
        int numberOfTroopsToAdvance = (troops + targetTroops) / 2;
        // TODO - delete this if statement
        if (numberOfTroopsToAdvance < 1) {
          std::cout << "****************\nPOSSIBLE INFINITE LOOP "
                       "DETECTED!!!\n****************"
                    << std::endl;
        }
        source->IncreaseStandByTroops(numberOfTroopsToAdvance);
        player->AddOrderToPlayer(
            new Advance(player, source, target, numberOfTroopsToAdvance));
        return true;
      }
    }
  }
  return false;
}
std::vector<Territory*> BenevolentPlayerStrategy::toDefend() {
  std::vector<Territory*> territories = *player->GetOwnedTerritories();
  std::random_shuffle(territories.begin(), territories.end());
  // Sorts territories by number of available troops (Increasing)
  std::stable_sort(territories.begin(), territories.end(),
                   Territory::HasLessAvailableTroops);
  return territories;
}
std::vector<Territory*> BenevolentPlayerStrategy::toAttack() {
  // Returns an empty vector since this player does not wish to attack.
  return std::vector<Territory*>();
}

// Neutral Player Strategy
NeutralPlayerStrategy::NeutralPlayerStrategy() : PlayerStrategy() {}
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* player)
    : PlayerStrategy(player) {}
NeutralPlayerStrategy::NeutralPlayerStrategy(
    const NeutralPlayerStrategy& toCopy)
    : PlayerStrategy(toCopy) {}
NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(
    const NeutralPlayerStrategy& rightSide) {
  player = rightSide.player;
  return *this;
}
NeutralPlayerStrategy::~NeutralPlayerStrategy() {}
std::ostream& operator<<(std::ostream& out,
                         const NeutralPlayerStrategy& toOutput) {
  out << "Neutral player strategy";
  return out;
}

bool NeutralPlayerStrategy::issueOrder() { return false; }
std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
  // Temporary return
  return std::vector<Territory*>();
}
std::vector<Territory*> NeutralPlayerStrategy::toAttack() {
  // Temporary return
  return std::vector<Territory*>();
}
