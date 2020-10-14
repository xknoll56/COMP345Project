#include "Map.h"

void Territory::AddNeigbor(Territory* neighbor) {
  neighbors.push_back(neighbor);
}
const std::vector<Territory*>* const Territory::getNeighbors() const {
  return &neighbors;
}

void Territory::setDiscovered(bool discovered) {
  this->discovered = discovered;
}

bool Territory::getDiscovered() { return discovered; }
const std::string* const Territory::getName() { return &name; }

  bool Graph::travelledAll() {
  bool travelled = true;
  for (Territory* territory : territories) {
    if (!territory->getDiscovered()) {
      travelled = false;
      break;
    }
  }

  return travelled;
}

void Graph::DepthFirstSearch(Territory* position) {
  std::cout << "entering territory " << *position->getName() << std::endl;
  position->setDiscovered(true);
  for (Territory* neighbor : *position->getNeighbors()) {
    if (!neighbor->getDiscovered() && contains(neighbor)) {
      DepthFirstSearch(neighbor);
    }
  }
}

bool Graph::contains(Territory* territory) {
  std::vector<Territory*>::iterator it =
      std::find(territories.begin(), territories.end(), territory);
  if (it != territories.end())
    return true;
  else
    return false;
}

const std::vector<Territory*>* const Graph::getTerritories() {
  return &territories;
}

void Graph::addTerritory(Territory* territory) {
  territories.push_back(territory);
}
bool Graph::validate() {
  bool valid = false;
  for (Territory* start : territories) {
    DepthFirstSearch(start);
    bool valid = travelledAll();
    for (Territory* terr : territories) terr->setDiscovered(false);
    if (valid) return true;
  }
  return valid;
}

const std::string* Continent::getName() { return &name; }

  Map::Map(int numContinents, int numTerritories) {
    continents.reserve(numContinents);
    territories.reserve(numTerritories);
  }

  const std::vector<Continent*>* const Map::getContinents() {
    return &continentLocations;
  }
  void Map::AddTerritory(std::string name, Continent* continent) {
    Territory territory(name);
    territories.push_back(territory);

    Territory* terr = &territories[territories.size() - 1];
    addTerritory(terr);
    continent->addTerritory(terr);
  }

  void Map::AddContinent(std::string name) {
    Continent continent(name);
    continents.push_back(continent);
    continentLocations.push_back(&continents[continents.size() - 1]);
  }

  bool Map::Validate() {
    bool valid = true;
    if (!Graph::validate()) valid = false;
    for (Continent* continent : *getContinents()) {
      if (!continent->validate())
        valid = false;
    }
    return valid;
  }