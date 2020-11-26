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

void HumanPlayerStrategy::issueOrder() {}
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

void AggressivePlayerStrategy::issueOrder() {}
std::vector<Territory*> AggressivePlayerStrategy::toDefend() {
    //temporary return
    return std::vector<Territory*>();
}
std::vector<Territory*> AggressivePlayerStrategy::toAttack() {
    //temporary return
    return std::vector<Territory*>();
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

void BenevolentPlayerStrategy::issueOrder() {}
std::vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    //Temporary return
    return std::vector<Territory*>();
}
std::vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    //Temporary return
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

void NeutralPlayerStrategy::issueOrder() {}
std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
    //Temporary return
    return std::vector<Territory*>();
}
std::vector<Territory*> NeutralPlayerStrategy::toAttack() {
    //Temporary return
    return std::vector<Territory*>();
}