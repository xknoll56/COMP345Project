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

#include "Cards.h"

#include <iostream>
#include <random>

Card::Card(int value=0): value(value)
{
}


Card::Card(const Card &card): Card(card.value)
{
}

int Card::getValue() const
{
    return value;
}

Card& Card::operator=(const Card& card)
{
   value = card.value;
   return *this;
}

std::ostream& Card::print(std::ostream& out) const
{
    out << "\nValue of the Card is : ";
    return out;
}

void Card::play()
{
}

Card::~Card()
{

}

void BombCard::play()
{
    std::cout <<"\nBombCard order issued\n";
}

BombCard::BombCard(int value) : Card(value)
{
}

BombCard::BombCard(const BombCard& bombCard):BombCard(bombCard.value)
{
}

BombCard::~BombCard()
{

}

BombCard& BombCard::operator=(const BombCard& bombCard)
{
    value = bombCard.value;
    return *this;
}

std::ostream& BombCard::print(std::ostream& out) const
{
    out << "\nBomb Card =  ";
    return out;
}

void ReinforcementCard::play()
{
    std::cout << "\nReinforcementCard order issued\n";
}

ReinforcementCard::ReinforcementCard(int value) : Card(value)
{

}

ReinforcementCard::ReinforcementCard(const ReinforcementCard& ReinforcementCard)
{
}

ReinforcementCard::~ReinforcementCard()
{

}

ReinforcementCard& ReinforcementCard::operator=(const ReinforcementCard& reinforcementCard)
{
    value = reinforcementCard.value;
    return *this;
}

std::ostream& ReinforcementCard::print(std::ostream& out) const
{
    out << "\Reinforcement Card =  ";
    return out;
}

void BlockadeCard::play()
{
    std::cout << "\nBlockadeCard order issued\n";
}

BlockadeCard::BlockadeCard(int value) : Card(value)
{

}

BlockadeCard::BlockadeCard(const BlockadeCard& BlockadeCard)
{
}

BlockadeCard::~BlockadeCard()
{

}

BlockadeCard& BlockadeCard::operator=(const BlockadeCard& blockadeCard)
{
    value = blockadeCard.value;
    return *this;
}

std::ostream& BlockadeCard::print(std::ostream& out) const
{
    out << "\nBlockade Card =  ";
    return out;
}

void AirliftCard::play()
{
    std::cout << "\nAirliftCard order issued\n";
}

AirliftCard::AirliftCard(int value) : Card(value)
{

}

AirliftCard::AirliftCard(const AirliftCard& AirliftCard)
{
}

AirliftCard::~AirliftCard()
{

}

AirliftCard& AirliftCard::operator=(const AirliftCard& airliftCard)
{
    value = airliftCard.value;
    return *this;
}

std::ostream& AirliftCard::print(std::ostream& out) const
{
    out << "\Airlift Card =  ";
    return out;
}

void DiplomacyCard::play()
{
    std::cout << "\nDiplomacyCard order issued\n";
}

DiplomacyCard::DiplomacyCard(int value) : Card(value)
{

}

DiplomacyCard::DiplomacyCard(const DiplomacyCard& DiplomacyCard)
{
}

DiplomacyCard::~DiplomacyCard()
{

}

DiplomacyCard& DiplomacyCard::operator=(const DiplomacyCard& diplomacyCard)
{
    value = diplomacyCard.value;
    return *this;
}

std::ostream& DiplomacyCard::print(std::ostream& out) const
{
    out << "\Diplocamy Card =  ";
    return out;
}


void Deck::addCard(Card* c)
{
    cards.push_back(c);
}

Card* Deck::draw()
{
    // get a random int
    int randomNumber = getRandomInt(0, cards.size() - 1);
    std::cout << "random numbers chosen from the deck = " << randomNumber << '\n';

    std::swap(cards[randomNumber], cards.back());

    Card* drawnCard = cards.back();
    cards.pop_back();

    return drawnCard;
}

Deck::Deck()
{
}

Deck::Deck(const Deck& Deck)
{

}

Deck& Deck::operator=(const Deck& deck)
{
    return *this;
}


Deck::~Deck()
{

}

void Hand::addCard(Card* c)
{
    cards.push_back(c);
}

Card* Hand::playFirstCard()
{
    Card* cardFound = nullptr;
    Card* ptr_card = cards.at(0);
    Card card = *ptr_card;
    std::swap(cards[0], cards.back());
    cardFound = cards.back();
    cards.pop_back();
    cardFound->play();
    if (cardFound == nullptr)
    {
        return nullptr;
    }
    return cardFound;

}


Card* Hand::useCard(int value)
{
    Card* cardFound = nullptr;

    for (int i = 0; i < cards.size(); ++i)
    {
        Card* ptr_card = cards.at(i);
        Card card = *ptr_card;

        if (card.getValue() == value)
        {
            std::swap(cards[i], cards.back());
            cardFound = cards.back();
            cards.pop_back();
        }
    }

    // Case 1 : no card found
    if (cardFound == nullptr)
    {
        return nullptr;
    }

    // Case 2 : we found a card with value
    cardFound->play();

    return cardFound;
}

Hand::Hand()
{
}

Hand::Hand(const Hand& Hand)
{
}

Hand& Hand::operator=(const Hand& hand)
{
    return *this;
}

Hand::~Hand()
{

}

std::ostream& operator<<(std::ostream& out, const Card& card)
{
    return card.print(out) << card.value;
}

std::ostream& operator<<(std::ostream& out, const Deck& deck)
{
    for (int i = 0; i < deck.cards.size(); ++i)
    {
        Card* ptr_card = deck.cards.at(i);
        Card card = *ptr_card;
        out << card;
    }
    out << "\n";
    return out;
}

std::ostream& operator<<(std::ostream& out, const Hand& hand)
{
    for (int i = 0; i < hand.cards.size(); ++i)
    {
        Card* ptr_card = hand.cards.at(i);
        Card card = *ptr_card;

        out << card;
    }
    out << "\n";
    return out;
}

int getRandomInt(int start, int end) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(start, end); // distribution in range [start, end]

    return dist6(rng);
}
