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

//#define DEBUG
#ifdef DEBUG
#define LOG(s) std::cout << s << std::endl;
#else
#define LOG(s)
#endif

#include <string>
#include <vector>

#include "Player.h"

class Territory;
class Player;

class Continent{
 private:
  std::string name;
  std::vector<Territory> territories;
  int bonus;
 public:
  Continent();
  Continent(std::string name, int bonus);
  Continent(Continent* continent);
  Continent& operator=(const Continent &continent);
  //std::ostream& operator<<(std::ostream& out, const Continent &continent);
  std::string GetName();
  std::vector<Territory>* GetTerritories();
  Territory* CreateTerritory(int id, std::string name);
  int GetBonus();
  bool AreAllVisited();
};

class Map {
 private:
  Map();
  std::vector<std::vector<int>> borders;
  std::vector<Continent*> continents;
 public:
  Map(int vertices, int continents);
  Map(Map* map);
  Map& operator=(const Map& map);
  //std::ostream& operator<<(std::ostream& out, const Map& map);
  void AddContinent(Continent* continent);
  std::vector<Continent*> GetContinents();
  void AddBorder(std::vector<int> data);
  bool AreAdjacent(Territory* source, Territory* target);
  std::vector<Territory*> GetTerritories();
  std::vector<Territory*> GetNeighbors(Territory* territory);
  Territory* GetTerriotryByID(int id);
  std::vector<std::vector<int>>* GetBorders();
  std::vector<int>* GetBordersById(int id,
                                   std::vector<std::vector<int>>* borders);
  void Visit(int id, std::vector<std::vector<int>>* borders);
  bool Validate();
  std::vector<std::vector<int>> GetInvertedBorders();
  std::vector<std::vector<int>> GetContinentBorders(
      Continent* continent, std::vector<std::vector<int>>* borders);
  void AllSetVisited(bool b);
  bool AreAllVisited();
};

class Territory {
 private:
  std::string name;
  int id;
  Player* player;
  int troops;
  bool visited = false;

 public:
  Territory();
  Territory(int id, std::string name);
  Territory(Territory* territory);
  Territory& operator=(const Territory& territory);
 // std::ostream& operator<<(std::ostream& out, Territory &territory);
  std::string* GetName();
  void SetPlayer(Player* player);
  Player* GetPlayer();
  void SetId(int id);
  int GetId();
  void IncreaseTroops(int increase);
  void DecraseTroops(int decrease);
  int GetTroops();
  void SetVisited(bool b);
  bool GetVisited();
};