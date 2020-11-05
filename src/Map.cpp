#include "Map.h"

#include <algorithm>

void Territory::AddNeigbor(Territory* neighbor) {
  neighbors.push_back(neighbor);
}

const std::vector<Territory*>* const Territory::GetNeighbors() const {
  return &neighbors;
}

void Territory::SetDiscovered(bool discovered) {
  this->discovered = discovered;
}

bool Territory::GetDiscovered() { return discovered; }
const std::string* const Territory::GetName() { return &name; }

Player* Territory::GetPlayer() { return player; }

void Territory::SetPlayer(Player* player) { this->player = player; }

int Territory::GetTroops() { return troops; }

void Territory::SetTroops(int troopsToAdd) {
  troops = std::max(0, troopsToAdd);
}

void Territory::AddTroops(int troops) { this->troops += std::max(0, troops); }

int Territory::RemoveTroops(int troopsToRemove) {
  troopsToRemove = std::max(0, troopsToRemove);
  if (troopsToRemove > troops) {
    int toReturn{troops};
    troops = 0;
    return toReturn;
  }
  troops -= troopsToRemove;
  return troopsToRemove;
}

bool Territory::TestAdjacencyTo(Territory* toTest) {
  return (std::find(neighbors.begin(), neighbors.end(), toTest) !=
          neighbors.end());
}

bool Graph::TravelledAll() {
  bool travelled = true;
  for (Territory* territory : territories) {
    if (!territory->GetDiscovered()) {
      travelled = false;
      break;
    }
  }
  return travelled;
}

Graph::Graph() {}
void Graph::DepthFirstSearch(Territory* position) {
  std::cout << "entering territory " << *position->GetName() << std::endl;
  position->SetDiscovered(true);
  for (Territory* neighbor : *position->GetNeighbors()) {
    if (!neighbor->GetDiscovered() &&
        Contains(neighbor)) {  // TODO lets see if we can do this without
                               // searching the entire graph for neighbor. Also,
                               // skip this test for DFS on a map since it
                               // contains all territories by default.
      DepthFirstSearch(neighbor);
    }
  }
}

bool Graph::Contains(Territory* territory) {
  std::vector<Territory*>::iterator it =
      std::find(territories.begin(), territories.end(), territory);
  if (it != territories.end())
    return true;
  else
    return false;
}

const std::vector<Territory*>* const Graph::GetTerritories() {
  return &territories;
}

void Graph::AddTerritory(Territory* territory) {
  territories.push_back(territory);
}

bool Graph::ValidateGraph() {
  for (Territory* start : territories) {
    DepthFirstSearch(start);
    bool valid = TravelledAll();
    for (Territory* terr : territories) terr->SetDiscovered(false);
    if (!valid) return false;
  }
  return true;
}

Player* Graph::GetLeader() {
  if (territories.size() < 1) {
    return nullptr;
  }
  Player* player = territories.at(0)->GetPlayer();
  for (Territory* territory : territories) {
    if (territory->GetPlayer() != player) {
      return nullptr;
    }
  }
  return player;
}

Continent::Continent(std::string name, int bonus) : name(name), bonus(bonus){};

const std::string* Continent::GetName() { return &name; }

int Continent::GetBonus() { return bonus; }

Map::Map(int numContinents, int numTerritories) {
  continents.reserve(numContinents);
  territories.reserve(numTerritories);
}

const std::vector<Continent*>& const Map::GetContinents() {
  return continentLocations;
}

Territory* Map::CreateTerritory(std::string name, Continent* continent) {
  territories.push_back(Territory(name, continent));

  Territory* terr = &territories[territories.size() - 1];
  AddTerritory(terr);
  continent->AddTerritory(terr);
  return terr;
}

Continent* Map::CreateContinent(std::string name, int bonus) {
  continents.push_back(Continent(name, bonus));

  Continent* cont = &continents[continents.size() - 1];

  continentLocations.push_back(cont);
  return cont;
}

bool Map::ValidateMap() {
  bool valid = true;
  if (!Graph::ValidateGraph()) return false;
  for (Continent* continent : GetContinents()) {
    if (!continent->ValidateGraph()) return false;
  }
  return valid;
}