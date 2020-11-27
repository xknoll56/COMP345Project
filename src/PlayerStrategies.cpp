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
    //temporary fix
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

bool HumanPlayerStrategy::issueOrder() { return false; }
std::vector<Territory*> HumanPlayerStrategy::toDefend() {
    //Temporary return
    return std::vector<Territory*>();
}
std::vector<Territory*> HumanPlayerStrategy::toAttack() {
    //Temporary return
    return std::vector<Territory*>();
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
    std::cout << "issuing" << std::endl;
   std::vector<Territory*> td = toDefend();
   if (player->GetReinforcementPoolCount() > 0) {
    if (td.size() > 0) { // Deploys all troops on highest priority territory.
        Territory* territory = toDefend().at(0);
        player->AddOrderToPlayer(new Deploy(player, territory, player->GetReinforcementPoolCount()));
        territory->IncreaseToDeploy(player->GetReinforcementPoolCount());
        player->SetReinforcementPool(0);
        return true;
    }
  }
  if (player->GetHand()->size() > 0) {
      player->PlayCard(0);
      return true;
  }
  int troops;
  for (Territory* target: td) {
      for (Territory* source: *target->GetNeighbors()) {
          troops = source->GetAvailableTroops();
          if ((source->GetPlayer() == player) && (troops > 0) && (Territory::HasMoreTroops(target, source))) {
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
    std::stable_sort(territories.begin(), territories.end(), Territory::HasMoreTroops);
    return territories;
}
std::vector<Territory*> AggressivePlayerStrategy::toAttack() {

    std::vector<Territory*> territories = player->GetAdjacentTerritories();
    std::random_shuffle(territories.begin(), territories.end());
    // Sorts territories by number of troops (Decreasing)
    std::stable_sort(territories.begin(), territories.end(), Territory::HasMoreTroops);
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
    return false;
}
std::vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    //Temporary return
    return std::vector<Territory*>();
}
std::vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    return *player->GetOwnedTerritories();
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
    //Temporary return
    return std::vector<Territory*>();
}
std::vector<Territory*> NeutralPlayerStrategy::toAttack() {
    //Temporary return
    return std::vector<Territory*>();
}