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

#include "Map.h"

/*
Creates a default continent with no name, no bonus, and an empty terrytory vector.
*/
Continent::Continent() {
  LOG("Creating a default Continent: " << name);
  this->territories = std::vector<Territory>();
  this->name = "";
  this->bonus = 0;
}

/*
Creates a continent with the given name and bonus and creates an empty vector of territories.
*/
Continent::Continent(std::string name, int bonus) {
  LOG("Creating a Continent: " << name);
  this->territories = std::vector<Territory>();
  this->name = name;
  this->bonus = bonus;
}

/*
Creates a copy of the given continent.
*/
Continent::Continent(Continent* continent) {
  LOG("Creating a Continent copy: " << continent->GetName());
  this->territories = *continent->GetTerritories();
  this->name = continent->GetName();
  this->bonus = continent->GetBonus();
}

/*
Assigns to this the values of the passed argument.
*/
Continent& Continent::operator=(const Continent& continent) {
  this->territories = continent.territories;
  this->name = continent.name;
  this->bonus = continent.bonus;
  return *this;
}

/*
Returns the display name of the continent.
*/
std::string Continent::GetName() { return this->name; }

/*
Returns a pointer to this continent's territories vector.
*/
std::vector<Territory>* Continent::GetTerritories() { return &this->territories; }

/*
Creates a territory inside this continent's territory vector.
id: The id of the territory can be any integer (unique to the territory).
name: The display name of the territory to be created.
*/
Territory* Continent::CreateTerritory(int id, std::string name) {
  this->territories.push_back(Territory(id, name));
  return &this->territories.back();
}

/*
Returns this continent's bonus.
The bonus is the number of extra troops a player recieves at the
start of their turn if they fully control this territory.
*/
int Continent::GetBonus() { return this->bonus; }

/*
Returns true if all the territories in the continent have their
visited flag set to true, false otherwise.
*/
bool Continent::AreAllVisited() {
  for (Territory &t : this->territories) { // For all territories in this continent
    if (!t.GetVisited()) { // A territory that hasnt been visited was found
      return false;
    }
  }
  return true;
}

/*
Creates a map.
vertices: the number of territories which are going to be in this map.
continents: the number of continents which are going to be in this map.
*/
Map::Map(int vertices, int continents) {
  LOG("Creating a Map");
  this->continents.reserve(continents);
  this->borders.reserve(vertices);
}

/*
Creates a copy of the given map.
*/
Map::Map(Map* map) {
    LOG("Creating a Map copy");
    this->continents = map->GetContinents(); 
    this->borders = *map->GetBorders();
}

/*
Assigns to this the values of the passed argument.
*/
Map& Map::operator=(const Map& map) {
  this->continents = map.continents;
  this->borders = map.borders;
  return *this;
}

/*
Adds a pointer to a continent object to this map's vector of conbtinents.
*/
void Map::AddContinent(Continent* continent) { this->continents.push_back(continent); }

/*
Returns this map's vector of continent pointers.
*/
// TODO returns a pointer
std::vector<Continent*> Map::GetContinents() { return this->continents; }

/*
Adds a border to this map's vector of borders.
Borders are vectors of integer where each integer should match
the id of a territory object.
The id of the first element in the vector is always the source vertex.
All the following ids are the is of the possible destination vertices
from the source vertex.
*/
void Map::AddBorder(std::vector<int> data) { this->borders.push_back(data); }

/*
Returns true if travel from source to target is possible, false otherwise.
Adjecency is validated through this map's border vector using the
ids of the two territories.
source: a pointer to the source territory
target: a pointer to the target territory
*/
bool Map::AreAdjacent(Territory* source, Territory* target) {
  int sourceId = source->GetId();
  int targetId = target->GetId();
  for (std::vector<int> t : this->borders) { // Loops though all the border lists in this map
    if (t.at(0) == sourceId) { // If a border with the same id as the source is found
      for (int i = 1; i < t.size(); i++) { // Loops through all possible targets in the border list
        if (t.at(i) == targetId) { // If an id matching the target id is found
          return true;
        }
      }
      return false;
    }
  }
}

/*
Creates and returns a vector of pointers to all the territories in all the
continents which are part of this map.
*/
// TODO
std::vector<Territory*> Map::GetTerritories() { 
  int size = 0;
  // First get the number of territories so we dont have to reallocate later
  for (Continent* c : this->continents) {
    size += c->GetTerritories()->size();
  }
  // create a vector of size calculated before
  std::vector<Territory*> terrs;
  terrs.reserve(size);
  for (Continent* c : this->continents) { // For each continent in this map
    for (int i = 0; i < c->GetTerritories()->size(); i++) { // For each territory in the continent
      terrs.push_back(&c->GetTerritories()->at(i)); // push a pointer to the territory inside the vector.
    }
  }
  return terrs;
}

/*
returns a pointer to the territory in this map which has the given id.
if no such territory is found, returns the nullptr.
id: the id of the territory to be searched for and returned.
*/
Territory* Map::GetTerriotryByID(int id) {
  for (Continent* c : this->continents) { // For each continent in this map
    for (int i = 0; i < c->GetTerritories()->size(); i++) { // for each territory in the continent
      if (c->GetTerritories()->at(i).GetId() == id) { // if the territory's id matches the given id
        return &c->GetTerritories()->at(i);
      }
    }
  }
  return nullptr;
}

/*
Creates and returns a vector of pointers to all the neighbors of the given territory
territory: a pointer to the territory for which to find all the neighbors.
It is possible for the vector to contain nullptr if a border couldnt be
associated with a territory in this map. Such a scenario means that the map is
not valid.
*/
// TODO
std::vector<Territory*> Map::GetNeighbors(Territory* territory) {
  std::vector<Territory*> terrs;
  int id = territory->GetId();
  for (std::vector<int> t : this->borders) { // For all border lists in this map
    if (t.at(0) == id) { // if the id of the first element matches the id of the given territory
      for (int i = 1; i < t.size(); i++) { // For each of the other id in the borderlist
        // Find in this, then return a pointer to the territory which has an id matching
        terrs.push_back(this->GetTerriotryByID(t.at(i)));
      }
      return terrs;
    }
  }
}

/*
Returns a pointer to the vector containing all of the border vectors in this map.
*/
std::vector<std::vector<int>>* Map::GetBorders() { return &this->borders; }

/*
Returns a pointer to the vector of borders of the territory which has the given id.
Returns nullptr if no border list with the given id exists on this map.
id: the id of the territory for which to find the list of borders.
*/
std::vector<int>* Map::GetBordersById(int id,
                                      std::vector<std::vector<int>>* borders) {
  for (int i = 0; i < borders->size(); i++) { // For every border list in this map
    if (borders->at(i).at(0) == id) { // If the first element in the list is the given id
      return &borders->at(i); // Return the list of borders.
    }
  }
  return nullptr;
}

/*
Performs DFS using the given borders list starting at id

Sets the visited flag of the territory with id to true.
Then visits all its neighbors in the given borders list.

id: the id of the territory currently being visited
borders: a vector containing the borders of all the territories.
each element in borders is a list of neighbors for a certain id.
the first element in the list of neighbors is the id of the source.
the rest of the elements are the possible destinations.
Note that borders does not necessarily point to this.borders.
*/
void Map::Visit(int id, std::vector<std::vector<int>>* borders) {
  Territory* territory = this->GetTerriotryByID(id); // find the territory which has this id
  if (territory->GetVisited() == true) { // If this territory has already been visited
    return; // no need to proceed with the visitation ofits neighbors.
  }
  territory->SetVisited(true);
  std::vector<int>* border = this->GetBordersById(id, borders); // Find the borders of the territory with this id
  if (border == nullptr) return;
  for (int i = 1; i < border->size(); i++) { // For each id in the list of neighbors (ecxept the first)
    this->Visit(border->at(i), borders); // Recursively visit
  }
}
/*
Creates and returns the inverse of this map's border list
*/
// TODO
std::vector<std::vector<int>> Map::GetInvertedBorders() {
  std::vector<std::vector<int>>* borders = &this->borders;
  std::vector<std::vector<int>> iborders;
  iborders.reserve(borders->size());
  // First, add the id of the source to every border list.
  for (Continent* c : this->continents) { // For each continent in this map
    std::vector<Territory>* terrs = c->GetTerritories();
    for (int i = 0; i < terrs->size(); i++) { // For each territory in the continent
      // Push back the id of the territory as the source for a list of neighbors
      iborders.push_back({terrs->at(i).GetId()}); 
    }
  }
  // get source -> destination from this map's border list and
  // place destination -> source in the inverted list.
  for (int i = 0; i < borders->size(); i++) { // For each border list in this map
    int v = borders->at(i).at(0); // v is the source of this border list
    for (int j = 1; j < borders->at(i).size(); j++) { // For each possible destination
      int u = borders->at(i).at(j); // u is a destination of this border list
      // place the inverse in the inverted list in the appropriate neighbor list.
      // u is now the source, v is the destination.
      this->GetBordersById(u, &iborders)->push_back(v);
    }
  }
  return iborders;
}

/*
Creates and returns a vector of border vectors using a given borders list 
where only the vertices which are in the given continent are considered.
continent: the continent for which we want to find the borders.
borders: the list of border lists from which to source the borders.
*/
std::vector<std::vector<int>> Map::GetContinentBorders(
    Continent* continent, std::vector<std::vector<int>>* borders) {
  // The border list contianing only ids of territories which are in the given continent
  std::vector<std::vector<int>> cborders;
  cborders.reserve(continent->GetTerritories()->size());
  // First, put the source ids into the list for all territories in the continent
  for (int i = 0; i < continent->GetTerritories()->size(); i++) { // For each territory in the given continent
    cborders.push_back({continent->GetTerritories()->at(i).GetId()});
  }
  for (std::vector<int> &b : cborders) { // for each neighbors list
    std::vector<int>* border = this->GetBordersById(b.at(0), borders);
    if (border == nullptr) continue; // if the source has no neighbor
    for (int i = 1; i < border->size(); i++) { // for each of the source's neighbors
      int id = border->at(i);
      // Find the neighbor list that corresponds
      for (int j = 0; j < cborders.size(); j++) {
        if (cborders.at(j).at(0) == id) {
          b.push_back(id); // Push the neighbor into the list.
        }
      }
    }
  } 
  return cborders;
}

/*
Sets the visited flag of all the territories which are part of this map to the given bool
*/
void Map::AllSetVisited(bool b) {
  for (Continent* c : this->continents) {
    std::vector<Territory>* terrs = c->GetTerritories();
    for (int i = 0; i < terrs->size(); i++) {
      terrs->at(i).SetVisited(b);
    }
  }
}

/*
Returns true if all the territories which are part of this map have their visited
flag set the true, returns false otherwise.
*/
bool Map::AreAllVisited() {
  for (Continent* c : this->continents) {
    std::vector<Territory>* terrs = c->GetTerritories();
    for (int i = 0; i < terrs->size(); i++) {
      if (terrs->at(i).GetVisited() == false) {
        return false;
      }
      terrs->at(i).SetVisited(false);
    }
  }
  return true;
}

/*
Validates this map.
1. For a map to be valid, the map must be a strongly connected graph.
2. Each continent in the map must be strongly connected graphs.
3. Each territory must belong to one and only one continent.
*/
bool Map::Validate() { 
  std::vector<std::vector<int>>* borders = &this->borders; // This map's list of borders
  std::vector<std::vector<int>> iborders = this->GetInvertedBorders(); // inverted list.
  this->AllSetVisited(false);

  if (this->continents.size() < 1) { // Empty maps are valid.
    return true;
  }
  // Find the id of a non-empty continent
  int id = -1;
  for (int i = 0; i < this->GetContinents().size(); i++) {
    if (this->GetContinents().at(i)->GetTerritories()->size() > 0) {
      id = i;
      break;
    }
  }
  if (id == -1) { // All continents are empty, the map is valid.
    return true;
  }

  /* 
  To check if a graph is strongly connected, we chose any node and perform DFS 
  starting with that node. Then, we invert all the edges and perform the second DFS
  starting with the same node as the first pass. A graph is connected iff all nodes
  in the graph were visited during both passes. 
  */
  // First, perform DFS on the whole map with its borders.
  try {
    this->Visit(this->continents.at(id)->GetTerritories()->at(0).GetId(), borders);
  } catch (...) {
    std::cout << "************Error: unable to traverse." << std::endl;
    return false;
  }
  if (!this->AreAllVisited()) {
    return false;
  }
  this->AllSetVisited(false);

  // Second, perform DFS on the whole map with its inverted borders.
  try {
    this->Visit(this->continents.at(id)->GetTerritories()->at(0).GetId(),
                &iborders);
  } catch (...) {
    std::cout << "************Error: unable to traverse." << std::endl;
    return false;
  }
  if (!this->AreAllVisited()) {
    return false;
  }
  this->AllSetVisited(false);

  std::vector<std::vector<int>> cborders; // Borders for the continent
  std::vector<std::vector<int>> icborders; // Inverted borders for the continent

  for (Continent* c : this->continents) {
    cborders = this->GetContinentBorders(c, borders);
    if (c->GetTerritories()->size() < 1) {
      continue;
    }
    // Third, perform DFS on each continent in this map with the continent border list.
    try {
      this->Visit(c->GetTerritories()->at(0).GetId(),
                  &cborders);
    } catch (...) {
      std::cout << "************Error: unable to traverse." << std::endl;
      return false;
    }
    if (!c->AreAllVisited()) {
      return false;
    }
    this->AllSetVisited(false);

    icborders = this->GetContinentBorders(c, &iborders);
    // Fourth, perform DFS on each continent in this map with the continent's list of inverted borders
    try {
      this->Visit(c->GetTerritories()->at(0).GetId(), &icborders);
    } catch (...) {
      std::cout << "************Error: unable to traverse." << std::endl;
      return false;
    }
    if (!c->AreAllVisited()) {
      return false;
    }
    this->AllSetVisited(false);
  }

  // Lastly, we check that territories belong to one and only one continent
  for (Continent* c : this->continents) {
    std::vector<Territory>* terrs = c->GetTerritories(); 
    for (int i = 0; i < terrs->size(); i++) {
      Territory* t = &terrs->at(i);
      if (t->GetVisited() == true) {
        return false;
      }
      t->SetVisited(true);
    }
  }
  if (!this->AreAllVisited()) {
    return false;
  }

  this->AllSetVisited(false);
  return true;
}

/*
Creates a default territory.
*/
Territory::Territory() : id(-1), name(), player(), troops(0) {
  LOG("Creating a default Territory");
}  // TODO ?

/*
Creates a territory with the given arguments
id: the id of the territory, it should be unique to each territory.
name: the display name of the territory to be created.
*/
Territory::Territory(int id, std::string name) {
  LOG("Creating a Territory: " << name);
  this->id = id;
  this->name = name;
}

/*
Creates a copy of the given territory.
*/
Territory::Territory(Territory* territory) { 
  LOG("Creating a Territory copy: " << *territory->GetName());
  this->id = territory->GetId(); 
  this->name = *territory->GetName();
}

/*
Assigns to this the values of the passed argument.
*/
Territory& Territory::operator=(const Territory& territory) {
  this->id = territory.id;
  this->name = territory.name;
  return *this;
}

/*
Returns the display name of this territory.
*/
std::string* Territory::GetName() {
  return &this->name;
}

/*
Sets the owner player of this territory to the player pointed to by the given pointer.
player: a pointer to the player which now controls this territory.
*/
void Territory::SetPlayer(Player* player) {
  this->player = player;}

/*
Returns a pointer to the player who is in control of this territory.
*/
Player* Territory::GetPlayer() {
  return this->player;}

/*
Sets the id of this territories.
Ids should be unique to each territory.
Ideally, they should be set during their creation
*/
void Territory::SetId(int id) {
  this->id = id;}

/*
Returns the id of this territory.
Each territory has a unique integer id.
*/
int Territory::GetId() {
  return this->id;}

/*
Increases the number of toops on this territory bu the given increment.
*/
void Territory::IncreaseTroops(int increase) {
  this->troops += increase;}

/*
Decreases the number of troops on this territory by the given increment.
*/
void Territory::DecraseTroops(int decrease) {
  this->troops -= decrease;}

/*
Returns the number of troops on this territory.
*/
int Territory::GetTroops() {
  return this->troops;}

/*
Sets the visited flad of this territory to true.
Used for validation.
*/
void Territory::SetVisited(bool b) {
  this->visited = b;}

/*
Returns the visited flag of this territory.
Used for validation.
*/
bool Territory::GetVisited() {
  return this->visited;}
