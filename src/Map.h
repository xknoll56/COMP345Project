#pragma once
#include <iostream>
#include <string>
#include <vector>

class MapLoader;
class Territory {
 private:
  std::string name;
  bool discovered;
  std::vector<Territory*> neighbors;
  void setDiscovered(bool discovered);
  bool getDiscovered();
  friend class Graph;

 public:
  Territory(std::string name) : name(name), discovered(false) {}
  void AddNeigbor(Territory* neighbor);
  const std::vector<Territory*>* const getNeighbors() const;
  const std::string* const getName();
};
class Graph {
 private:
  std::vector<Territory*> territories;
  int numTerritories;
  bool travelledAll();
  void DepthFirstSearch(Territory* position);

 public:
  bool contains(Territory* territory);
  const std::vector<Territory*>* const getTerritories();
  Graph();

 protected:
  void addTerritory(Territory* territory);
  bool validate();
};

class Continent : public Graph {
 private:
  std::string name;

 public:
  Continent(std::string name) : name(name){};
  const std::string* getName();
  friend class Map;
};

class Map : public Graph {
 private:
  std::vector<Continent> continents;
  std::vector<Territory> territories;
  std::vector<Continent*> continentLocations;


 public:
  Map(int numContinents, int numTerritories);
  const std::vector<Continent*>* const getContinents();
  bool Validate();
  void AddTerritory(std::string name, Continent* continent);
  void AddContinent(std::string name);
};
