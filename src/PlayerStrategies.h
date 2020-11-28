#pragma once

#include "Player.h"
#include "Map.h"

class Player;
class Territory;

class PlayerStrategy {
public:
    PlayerStrategy();
    PlayerStrategy(Player* player);
    PlayerStrategy(const PlayerStrategy& toCopy);
    PlayerStrategy& operator=(const PlayerStrategy& rightSide);
    virtual ~PlayerStrategy();

    friend std::ostream& operator<<(std::ostream& out,
        const PlayerStrategy& toOutput);

    virtual bool issueOrder() = 0;
    virtual std::vector<Territory*> toDefend() = 0;
    virtual std::vector<Territory*> toAttack() = 0;
    void setPlayer(Player* player);

protected:
    Player* player;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy();
    HumanPlayerStrategy(Player* player);
    HumanPlayerStrategy(const HumanPlayerStrategy& toCopy);
    HumanPlayerStrategy& operator=(const HumanPlayerStrategy& rightSide);
    virtual ~HumanPlayerStrategy();
    friend std::ostream& operator<<(std::ostream& out,
        const HumanPlayerStrategy& toOutput);
    virtual bool issueOrder();
    virtual std::vector<Territory*> toDefend();
    virtual std::vector<Territory*> toAttack();

    // Prompts the user for a value between min and max inclusively
    static int promptForValueInRange(int min, int max);
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    AggressivePlayerStrategy();
    AggressivePlayerStrategy(Player* player);
    AggressivePlayerStrategy(const AggressivePlayerStrategy& toCopy);
    AggressivePlayerStrategy& operator=(
        const AggressivePlayerStrategy& rightSide);
    virtual ~AggressivePlayerStrategy();
    friend std::ostream& operator<<(std::ostream& out,
        const AggressivePlayerStrategy& toOutput);
    virtual bool issueOrder();
    virtual std::vector<Territory*> toDefend();
    virtual std::vector<Territory*> toAttack();
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy();
    BenevolentPlayerStrategy(Player* player);
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& toCopy);
    BenevolentPlayerStrategy& operator=(
        const BenevolentPlayerStrategy& rightSide);
    virtual ~BenevolentPlayerStrategy();
    friend std::ostream& operator<<(std::ostream& out,
        const BenevolentPlayerStrategy& toOutput);
    virtual bool issueOrder();
    virtual std::vector<Territory*> toDefend();
    virtual std::vector<Territory*> toAttack();
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy();
    NeutralPlayerStrategy(Player* player);
    NeutralPlayerStrategy(const NeutralPlayerStrategy& toCopy);
    NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& rightSide);
    virtual ~NeutralPlayerStrategy();
    friend std::ostream& operator<<(std::ostream& out,
        const NeutralPlayerStrategy& toOutput);
    virtual bool issueOrder();
    virtual std::vector<Territory*> toDefend();
    virtual std::vector<Territory*> toAttack();
};