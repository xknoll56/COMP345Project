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

#pragma once

#include <vector>
#include <ostream>

class Card {
public:
    Card(int value);

    Card(const Card &card);

    virtual ~Card(); //deconstructor

    virtual void play();

    int getValue() const;
     
    Card& operator= (const Card& card);

    friend std::ostream& operator<<(std::ostream& out, const Card& card);

    virtual std::ostream& print(std::ostream& out) const;

protected:
    int value; //only included for now to be able to let the user decide which card he wants to play
};

class BombCard : public Card {
public:
    BombCard(int value);

    BombCard(const BombCard &BombCard);

    virtual ~BombCard();//deconstructor

    BombCard& operator= (const BombCard& bombCard);

    virtual std::ostream& print(std::ostream& out) const override;

    void play() override;
};

class ReinforcementCard : public Card {
public:
    ReinforcementCard(int value);

    ReinforcementCard(const ReinforcementCard &ReinforcementCard);

    virtual ~ReinforcementCard(); //deconstructor

    ReinforcementCard& operator= (const ReinforcementCard& reinforcementCard);

    virtual std::ostream& print(std::ostream& out) const override;

    void play() override;
};

class BlockadeCard : public Card {
public:
    BlockadeCard(int value);

    BlockadeCard(const BlockadeCard& BlockadeCard);

    virtual ~BlockadeCard(); //deconstructor

    BlockadeCard& operator= (const BlockadeCard& blockadeCard);

    virtual std::ostream& print(std::ostream& out) const override;

    void play() override;
};

class AirliftCard : public Card {
public:
    AirliftCard(int value);

    AirliftCard(const AirliftCard& AirliftCard);

    virtual ~AirliftCard(); //deconstructor

    AirliftCard& operator= (const AirliftCard& airliftCard);

    virtual std::ostream& print(std::ostream& out) const override;

    void play() override;
};

class DiplomacyCard : public Card {
public:
    DiplomacyCard(int value);

    DiplomacyCard(const DiplomacyCard& DiplomacyCard);

    virtual ~DiplomacyCard(); //deconstructor

    DiplomacyCard& operator= (const DiplomacyCard& diplomacyCard);

    virtual std::ostream& print(std::ostream& out) const override;

    void play() override;
};



class Deck {
private:
    std::vector<Card*> cards;
public:
    void addCard(Card* c);

    Card* draw();

    Deck();

    Deck(const Deck& Deck);

    Deck& operator= (const Deck& deck);

    friend std::ostream& operator<<(std::ostream& out, const Deck& f1);

    virtual ~Deck();

};

class Hand {
private:
    std::vector<Card*> cards;
public:
    Hand();

    Hand(const Hand& Hand);

    Hand& operator= (const Hand& hand);
    
    virtual ~Hand();

    void addCard(Card* c);

    Card* playFirstCard();

    Card* useCard(int value);

    friend std::ostream& operator<<(std::ostream& out, const Hand& f1);
};

int getRandomInt(int start, int end); // free function to get a random number between an interval