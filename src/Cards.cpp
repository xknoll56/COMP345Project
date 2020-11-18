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

#include "Orders.h"

Card::Card() : deck() {}
Card::Card(Deck* deck) : deck(deck) {}
Card::Card(const Card& card) : deck(card.deck) {}
Card& Card::operator=(const Card& card) {
  this->deck = card.deck;
  return *this;
}

std::ostream& Card::print(std::ostream& out) const {
  out << "Base card";
  return out;
}

Order* Card::play() {
  // Reput the card into the deck
  // The actual returning will be done by each subtype's implementation of
  // play()
  deck->addCard(this);
  return nullptr;
}

Card::~Card() {}

Order* BombCard::play() {
  Order* bomb = new Bomb();
  Card::play();
  return bomb;
}

BombCard::BombCard() : Card() {}

BombCard::BombCard(Deck* deck) : Card(deck) {}

BombCard::BombCard(const BombCard& bombCard) : Card(bombCard) {}

BombCard::~BombCard() {}

BombCard& BombCard::operator=(const BombCard& bombCard) { return *this; }

std::ostream& BombCard::print(std::ostream& out) const {
  out << "Bomb Card";
  return out;
}

Order* ReinforcementCard::play() {
  Order* deploy = new Deploy();
  Card::play();
  return deploy;
}

ReinforcementCard::ReinforcementCard() : Card() {}

ReinforcementCard::ReinforcementCard(Deck* deck) : Card(deck) {}

ReinforcementCard::ReinforcementCard(
    const ReinforcementCard& ReinforcementCard) {}

ReinforcementCard::~ReinforcementCard() {}

ReinforcementCard& ReinforcementCard::operator=(
    const ReinforcementCard& reinforcementCard) {
  return *this;
}

std::ostream& ReinforcementCard::print(std::ostream& out) const {
  out << "Reinforcement Card";
  return out;
}

Order* BlockadeCard::play() {
  Order* blockade = new Blockade();
  Card::play();
  return blockade;
}

BlockadeCard::BlockadeCard() : Card() {}

BlockadeCard::BlockadeCard(Deck* deck) : Card(deck) {}

BlockadeCard::BlockadeCard(const BlockadeCard& BlockadeCard) {}

BlockadeCard::~BlockadeCard() {}

BlockadeCard& BlockadeCard::operator=(const BlockadeCard& blockadeCard) {
  return *this;
}

std::ostream& BlockadeCard::print(std::ostream& out) const {
  out << "Blockade Card";
  return out;
}

Order* AirliftCard::play() {
  Order* airlift = new Airlift();
  Card::play();
  return airlift;
}

AirliftCard::AirliftCard() : Card() {}

AirliftCard::AirliftCard(Deck* deck) : Card(deck) {}

AirliftCard::AirliftCard(const AirliftCard& AirliftCard) {}

AirliftCard::~AirliftCard() {}

AirliftCard& AirliftCard::operator=(const AirliftCard& airliftCard) {
  return *this;
}

std::ostream& AirliftCard::print(std::ostream& out) const {
  out << "Airlift Card";
  return out;
}

Order* DiplomacyCard::play() {
  Order* negotiate = new Negotiate();
  Card::play();
  return negotiate;
}

DiplomacyCard::DiplomacyCard() : Card() {}

DiplomacyCard::DiplomacyCard(Deck* deck) : Card(deck) {}

DiplomacyCard::DiplomacyCard(const DiplomacyCard& DiplomacyCard) {}

DiplomacyCard::~DiplomacyCard() {}

DiplomacyCard& DiplomacyCard::operator=(const DiplomacyCard& diplomacyCard) {
  return *this;
}

std::ostream& DiplomacyCard::print(std::ostream& out) const {
  out << "Diplomacy Card";
  return out;
}

void Deck::addCard(Card* c) { cards.push_back(c); }

Card* Deck::draw() {
  // get a random int
  int randomNumber = getRandomInt(0, cards.size() - 1);
  std::cout << "random numbers chosen from the deck = " << randomNumber << '\n';

  std::swap(cards[randomNumber], cards.back());

  Card* drawnCard = cards.back();
  cards.pop_back();

  return drawnCard;
}

Deck::Deck() {}

Deck::Deck(const Deck& Deck) {}

Deck& Deck::operator=(const Deck& deck) { return *this; }

Deck::~Deck() {
  for (auto card : cards) {
    delete card;
  }
  cards.clear();
}

void Hand::addCard(Card* c) { cards.push_back(c); }

Card* Hand::playFirstCard() {
  Card* cardFound = nullptr;
  Card* ptr_card = cards.at(0);
  Card card = *ptr_card;
  std::swap(cards[0], cards.back());
  cardFound = cards.back();
  cards.pop_back();
  cardFound->play();
  if (cardFound == nullptr) {
    return nullptr;
  }
  return cardFound;
}

Hand::Hand() {}

Hand::Hand(const Hand& Hand) {}

Hand& Hand::operator=(const Hand& hand) { return *this; }

Hand::~Hand() {
  for (auto card : cards) {
    delete card;
  }
  cards.clear();
}

std::ostream& operator<<(std::ostream& out, const Card& card) {
  return card.print(out);
}

std::ostream& operator<<(std::ostream& out, const Deck& deck) {
  for (int i = 0; i < deck.cards.size(); ++i) {
    Card* ptr_card = deck.cards.at(i);
    Card card = *ptr_card;
    out << card;
  }
  out << "\n";
  return out;
}

std::ostream& operator<<(std::ostream& out, const Hand& hand) {
  for (int i = 0; i < hand.cards.size(); ++i) {
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
  std::uniform_int_distribution<std::mt19937::result_type> dist6(
      start, end);  // distribution in range [start, end]

  return dist6(rng);
}
