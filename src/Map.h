#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Player.h"

class Continent;
class Player;

class Territory {
 private:
  Player* player;
  std::string name;
  bool discovered;
  std::vector<Territory*> neighbors;
  void SetDiscovered(bool discovered);
  bool GetDiscovered();
  int troops;
  Continent* continent;
  friend class Graph;
  int toDeploy;

 public:
  Territory(std::string name, Continent* continent) : name(name), continent(continent), discovered(false), player(nullptr), troops(0), toDeploy(0) {}
  void AddNeigbor(Territory* neighbor);
  const std::vector<Territory*>* const GetNeighbors() const;
  const std::string* const GetName();
  Player* GetPlayer();
  void SetPlayer(Player* player);
  int GetTroops();
  void SetTroops(int troops);
  void AddTroops(int troops);
  // Attempts to remove provided amount of troops
  // If there are fewer troops left than requested, goes down to 0
  // Returns the amount of troops removed which may be less than the requested
  // amount
  int RemoveTroops(int troops);
  bool TestAdjacencyTo(Territory* toTest);
  int GetTotalTroops();
  void IncreaseToDeploy(int amount);
  void SetToDeploy(int toDeploy);
  int GetToDeploy();
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
  Player* GetLeader();

 protected:
  void AddTerritory(Territory* territory);
  bool ValidateGraph();
};

class Continent : public Graph {
 private:
  std::string name;
  int bonus;
 public:
  Continent(std::string name, int bonus);
  const std::string* GetName();
  int GetBonus();
  friend class Map;
};

class Map : public Graph {
 private:
  std::vector<Continent> continents;
  std::vector<Territory> territories;
  std::vector<Continent*> continentLocations;
  int territoriesCapacity;
  int continentsCapacity;

 public:
  Map(int numContinents, int numTerritories);
  const std::vector<Continent*>& const GetContinents();
  bool ValidateMap();
  Territory* CreateTerritory(std::string name, Continent* continent);
  Continent* CreateContinent(std::string name, int bonus);
};
