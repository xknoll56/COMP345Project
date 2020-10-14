// COMP 345 - Project
// Part 1
// Jordan Goulet - 40075688
// FirstName LastName - 00000000
// FirstName LastName - 00000000
// FirstName LastName - 00000000
// FirstName LastName - 00000000
//
// Based on the 'https://www.warzone.com/' game.

#pragma once

#include <string>
#include <vector>

#include "Player.h"

class Territory {
 private:
  std::string name;
  bool discovered;
  std::vector<Territory*> neighbors;

 public:
  Territory(std::string name) : name(name), discovered(false) {}
  void AddNeigbor(Territory* neighbor) { neighbors.push_back(neighbor); }
  const std::vector<Territory*>* const getNeighbors() const {
    return &neighbors;
  }

  void setDiscovered(bool discovered) { this->discovered = discovered; }

  bool getDiscovered() { return discovered; }
  const std::string* const getName() { return &name; }
};
class Graph {
 private:
  std::vector<Territory*> territories;
  int numTerritories;
  bool travelledAll() {
    bool travelled = true;
    for (Territory* territory : territories) {
      if (!territory->getDiscovered()) {
        travelled = false;
        break;
      }
    }

    return travelled;
  }

  void DepthFirstSearch(Territory* position) {
    std::cout << "entering territory " << *position->getName() << std::endl;
    position->setDiscovered(true);
    for (Territory* neighbor : *position->getNeighbors()) {
      if (!neighbor->getDiscovered() && contains(neighbor)) {
        std::cout << "not discovered" << std::endl;
        DepthFirstSearch(neighbor);
      }
    }
  }

 public:
  bool contains(Territory* territory) {
    std::vector<Territory*>::iterator it =
        std::find(territories.begin(), territories.end(), territory);
    if (it != territories.end())
      return true;
    else
      return false;
  }

  const std::vector<Territory*>* const getTerritories() { return &territories; }

 protected:
  void addTerritory(Territory* territory) { territories.push_back(territory); }
  bool validate(Territory* start) {
    start->setDiscovered(true);
    DepthFirstSearch(start);
    bool valid = travelledAll();
    for (Territory* terr : territories) terr->setDiscovered(false);
    return valid;
  }
};

class Continent : public Graph {
 private:
  std::string name;

 public:
  Continent(std::string name) : name(name){};
  const std::string* getName() { return &name; }
  friend class Map;
};

class Map : public Graph {
 private:
  std::vector<Continent> continents;
  std::vector<Territory> territories;
  std::vector<Continent*> continentLocations;

 public:
  friend class Continent;
  Map(int numContinents, int numTerritories) {
    continents.reserve(numContinents);
    territories.reserve(numTerritories);
  }

  const std::vector<Continent*>* const getContinents() {
    return &continentLocations;
  }
  void AddTerritory(std::string name, Continent* continent) {
    Territory territory(name);
    territories.push_back(territory);

    Territory* terr = &territories[territories.size() - 1];
    addTerritory(terr);
    continent->addTerritory(terr);
  }

  void AddContinent(std::string name) {
    Continent continent(name);
    continents.push_back(continent);
    continentLocations.push_back(&continents[continents.size() - 1]);
  }

  bool Validate() {
    bool valid = true;
    if (!Graph::validate(&territories[0])) valid = false;
    for (Continent* continent : *getContinents()) {
      if (!continent->validate(continent->getTerritories()->at(0)))
        valid = false;
    }
    return valid;
  }
};
