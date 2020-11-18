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

#include <ostream>
#include <vector>

#include "Orders.h"

class Player;
class Deck;
class Order;

class Card {
 public:
  Card();
  Card(Deck* deck);
  Card(const Card& card);
  virtual ~Card();

  // The play method of each card is basically a factory method that returns an
  // order of the correct type
  virtual Order* play();

  Card& operator=(const Card& card);

  friend std::ostream& operator<<(std::ostream& out, const Card& card);

  virtual std::ostream& print(std::ostream& out) const;

 protected:
  Deck* deck;
};

class BombCard : public Card {
 public:
  BombCard();
  BombCard(Deck* deck);
  BombCard(const BombCard& BombCard);

  virtual ~BombCard();  // deconstructor

  BombCard& operator=(const BombCard& bombCard);

  virtual std::ostream& print(std::ostream& out) const override;

  Order* play() override;
};

class ReinforcementCard : public Card {
 public:
  ReinforcementCard();
  ReinforcementCard(Deck* deck);
  ReinforcementCard(const ReinforcementCard& ReinforcementCard);

  virtual ~ReinforcementCard();  // deconstructor

  ReinforcementCard& operator=(const ReinforcementCard& reinforcementCard);

  virtual std::ostream& print(std::ostream& out) const override;

  Order* play() override;
};

class BlockadeCard : public Card {
 public:
  BlockadeCard();
  BlockadeCard(Deck* deck);
  BlockadeCard(const BlockadeCard& BlockadeCard);

  virtual ~BlockadeCard();  // deconstructor

  BlockadeCard& operator=(const BlockadeCard& blockadeCard);

  virtual std::ostream& print(std::ostream& out) const override;

  Order* play() override;
};

class AirliftCard : public Card {
 public:
  AirliftCard();
  AirliftCard(Deck* deck);
  AirliftCard(const AirliftCard& AirliftCard);

  virtual ~AirliftCard();  // deconstructor

  AirliftCard& operator=(const AirliftCard& airliftCard);

  virtual std::ostream& print(std::ostream& out) const override;

  Order* play() override;
};

class DiplomacyCard : public Card {
 public:
  DiplomacyCard();
  DiplomacyCard(Deck* deck);
  DiplomacyCard(const DiplomacyCard& DiplomacyCard);

  virtual ~DiplomacyCard();  // deconstructor

  DiplomacyCard& operator=(const DiplomacyCard& diplomacyCard);

  virtual std::ostream& print(std::ostream& out) const override;

  Order* play() override;
};

class Deck {
 private:
  std::vector<Card*> cards;

 public:
  void addCard(Card* c);

  Card* draw();

  Deck();

  Deck(const Deck& Deck);

  Deck& operator=(const Deck& deck);

  friend std::ostream& operator<<(std::ostream& out, const Deck& f1);

  virtual ~Deck();
};

class Hand {
 private:
  std::vector<Card*> cards;

 public:
  Hand();

  Hand(const Hand& Hand);

  Hand& operator=(const Hand& hand);

  virtual ~Hand();

  void addCard(Card* c);

  Card* playFirstCard();

  friend std::ostream& operator<<(std::ostream& out, const Hand& f1);
};

int getRandomInt(
    int start,
    int end);  // free function to get a random number between an interval