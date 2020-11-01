#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Player.h"


class Territory {
 private:
  std::string name;
  bool discovered;
  std::vector<Territory*> neighbors;
  void SetDiscovered(bool discovered);
  bool GetDiscovered();
  Player *player;
  int numberOfArmies;
  friend class Graph;

 public:
  Territory(std::string name) : name(name), discovered(false), player(nullptr), numberOfArmies(0) {}
  void AddNeigbor(Territory* neighbor);
  const std::vector<Territory*>* const GetNeighbors() const;
  const std::string* const GetName();
  Player* GetPlayer();
  void SetPlayer(Player* player);
  int GetNumberOfArmies();
  void SetNumberOfArmies(int numberOfArmies);
};

class Graph {
 private:
  std::vector<Territory*> territories;
  bool TravelledAll();
  void DepthFirstSearch(Territory* position);

 public:
  bool Contains(Territory* territory);
  const std::vector<Territory*>* const GetTerritories();
  Graph();

 protected:
  void AddTerritory(Territory* territory);
  bool ValidateGraph();
};

class Continent : public Graph {
 private:
  std::string name;

 public:
  Continent(std::string name) : name(name){};
  const std::string* GetName();
  friend class Map;
};

class Map : public Graph {
 private:
  std::vector<Continent> continents;
  std::vector<Territory> territories;
  std::vector<Continent*> continentLocations;


 public:
  Map(int numContinents, int numTerritories);
  const std::vector<Continent*>* const GetContinents();
  bool ValidateMap();
  Territory* CreateTerritory(std::string name, Continent* continent);
  Continent* CreateContinent(std::string name);
};
