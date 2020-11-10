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



class Subject;
class Observer {
 public:
  ~Observer() {}
  virtual void Update() = 0;
  virtual void UpdateOrderIssue() {}
  virtual void UpdateOrderExecute() {}
  virtual void UpdateReinforcements(int) {}
  virtual void UpdateStats(int, std::string, bool) {}
  void UpdateSubject(Subject* subject);

 protected:
  Subject* currentSubject = nullptr;
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
  virtual void Subject::NotifyOrderIssue();
  virtual void Subject::NotifyOrderExecute();
  virtual void Subject::NotifyReinforcements(int troops);
  virtual void Subject::NotifyStats(int numTerritories, std::string name,
                                    bool added);

  int getId();

  std::string GetName();

 private:
  std::string name;
  int id = 0;
  std::list<Observer*>* observers;
};

class GameStatisticsObserver : public Observer {
 public:
  GameStatisticsObserver(int numPlayers, int numTerritories);
  void UpdateStats(int numTerritories, std::string name, bool added);
  void Update();

 private:
  // std::list<int> playerIndexes;
  std::vector<Subject*> players;
  std::vector<std::string> statsInformation;
  float* ownedPercentages;
  int totalTerritories;
  int numPlayers;
};

class PhaseObserver : public Observer {
 public:
  void Update();
  void UpdateReinforcements(int troops);
  void UpdateOrderIssue();
  void UpdateOrderExecute();

 private:
  enum Phase { Reinforcement = 0, IssueOrder, ExecuteOrder, None };
  std::vector<std::string> phaseInformation;
  Phase currentPhase = Phase::None;
};
