#pragma once
#include "GameObservers.h"

void Observer::UpdateSubject(Subject* subject) {
  this->currentSubject = subject;
}

Subject::Subject() {
  static int i = 0;
  id = i;
  name = std::to_string(i);
  observers = new std::list<Observer*>();
  i++;
}

Subject::~Subject() { delete observers; }

void Subject::Attach(Observer* obs) { observers->push_back(obs); }

void Subject::Detach(Observer* obs) { observers->remove(obs); }

void Subject::Notify() {
  std::list<Observer*>::iterator it = observers->begin();
  for (; it != observers->end(); ++it) {
    (*it)->UpdateSubject(this);
    (*it)->Update();
  }
}

void Subject::NotifyOrderIssue() {
  std::list<Observer*>::iterator it = observers->begin();
  for (; it != observers->end(); ++it) {
    (*it)->UpdateSubject(this);
    (*it)->UpdateOrderIssue();
    (*it)->Update();
  }
}

void Subject::NotifyOrderExecute() {
  std::list<Observer*>::iterator it = observers->begin();
  for (; it != observers->end(); ++it) {
    (*it)->UpdateSubject(this);
    (*it)->UpdateOrderExecute();
    (*it)->Update();
  }
}

void Subject::NotifyReinforcements(int troops) {
  std::list<Observer*>::iterator it = observers->begin();
  for (; it != observers->end(); ++it) {
    (*it)->UpdateSubject(this);
    (*it)->UpdateReinforcements(troops);
    (*it)->Update();
  }
}

void Subject::NotifyStats(int numTerritories, std::string name, bool added) {
  std::list<Observer*>::iterator it = observers->begin();
  for (; it != observers->end(); ++it) {
    (*it)->UpdateSubject(this);
    (*it)->UpdateStats(numTerritories, name, added);
    (*it)->Update();
  }
}

int Subject::getId() { return id; }

std::string Subject::GetName() { return name; }

GameStatisticsObserver::GameStatisticsObserver(int numPlayers,
                                               int numTerritories) {
  this->numPlayers = numPlayers;
  ownedPercentages = new float[numPlayers];
  for (int i = 0; i < numPlayers; i++) ownedPercentages[i] = 0.0f;
  this->totalTerritories = numTerritories;
  players.reserve(numPlayers);
}

void GameStatisticsObserver::UpdateStats(int numTerritories, std::string name,
                                         bool added) {
  if (players.size() < numPlayers) {
    std::vector<Subject*>::iterator it =
        std::find(players.begin(), players.end(), currentSubject);
    if (it == players.end()) players.push_back(currentSubject);
  }
  ownedPercentages[currentSubject->getId()] =
      (float)numTerritories / (float)totalTerritories;
  if (added) {
    statsInformation.push_back("Territory " + name +
                               " has been added to player " +
                               currentSubject->GetName());
  } else {
    if (numTerritories == 0) {
      std::vector<Subject*>::iterator it =
          std::find(players.begin(), players.end(), currentSubject);
      if (it != players.end()) players.erase(it);
    }
    statsInformation.push_back("Territory " + name +
                               " has been removed to player " +
                               currentSubject->GetName());
  }
}

void GameStatisticsObserver::Update() {
  system("cls");
  std::cout << "Player Statistics: " << std::endl;
  for (Subject* player : players)
    std::cout << "Player " << player->getId() << " owns "
              << ownedPercentages[player->getId()] * 100.0f << ". ";
  std::cout << std::endl;
  for (std::string s : statsInformation) std::cout << s << std::endl;
  statsInformation.clear();
}

void PhaseObserver::Update() {
  for (std::string s : phaseInformation) std::cout << s << std::endl;
}

void PhaseObserver::UpdateReinforcements(int troops) {
  if (currentPhase != Phase::Reinforcement) {
    phaseInformation.clear();
    currentPhase = Phase::Reinforcement;
  }
  phaseInformation.push_back("Player " + currentSubject->GetName() +
                             ": Reinforcement phase");
  phaseInformation.push_back("\tAdding " + std::to_string(troops) +
                             " units to reinforcement pool");
}

void PhaseObserver::UpdateOrderIssue() {
  if (currentPhase != Phase::IssueOrder) {
    phaseInformation.clear();
    currentPhase = Phase::IssueOrder;
  }
  phaseInformation.push_back("Player " + currentSubject->GetName() +
                             ": Issue orders phase");
}

void PhaseObserver::UpdateOrderExecute() {
  if (currentPhase != Phase::ExecuteOrder) {
    phaseInformation.clear();
    currentPhase = Phase::ExecuteOrder;
  }
  phaseInformation.push_back("Player " + currentSubject->GetName() +
                             ": Executing orders phase");
}
