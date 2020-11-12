// COMP 345 - Project
// Part 2
//
// Sophie Renaud       -  40132563
// Joey Abou-Chaker    -  40055551
// Jordan Goulet       -  40075688
// Xavier Knoll        -  40132134
// Sébastien Champoux  -  40133449
//
// Based on the 'https://www.warzone.com/' game.
#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

class Player;
class Subject;
class Order;
class Card;
class Observer {
 public:
  ~Observer() {}
  virtual void Update() = 0;

 protected:
  Observer() {}

 private:
};

class Subject {
 public:
  Subject::Subject();
  Subject::~Subject();
  virtual void Subject::Attach(Observer* obs);
  void Subject::Detach(Observer* obs);
  void Subject::Notify();
  int getId();
  std::string GetName();

 private:
  std::string name;
  int id = 0;
  std::list<Observer*>* observers;
};

enum Phase { Reinforcement = 0, IssueOrders = 1, ExecuteOrders = 2, None };

class GameStatisticsObserver : public Observer {
 public:
  void Update();
  void AddPlayer(Player* player);
  GameStatisticsObserver(int numTerritories);
  void Start();

 private:
  std::vector<Player*> players;
  int numTerritories;
  bool gameStarted = false;
};
class PhaseObserver : public Observer {
 public:
  void Update();
  void AddPlayer(Player* player);

 private:
  void UpdateReinforcements(Player* player);
  void UpdateIssueOrders(Player* player);
  void UpdateExecuteOrders(Player* player);
  std::vector<Player*> players;
  Phase currentPhase = Phase::None;
  struct StreamInsertions {
    std::string message;
    Player* player = nullptr;
    Order* order = nullptr;
    Card* card = nullptr;
  };
  std::list<StreamInsertions> streamInsertions;
};
