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

#include <filesystem>

#include "MapLoader.h"
using std::cin;
using std::cout;
using std::endl;

#define MAP_LOADER_DRIVER
#ifdef MAP_LOADER_DRIVER

int main() {
  int mapType{0};
  std::string fileName;

  // Could the type of map be automatically chosen?
  while (true) {
    cout << "Which type of map should be read?" << endl;
    cout << "1) Domination" << endl;
    cout << "2) Conquest" << endl;
    cin >> mapType;
    if (mapType == 1 || mapType == 2) {
      break;
    }
    cout << "Invalid choice, not cool :(" << endl;
  }

  cout << "Enter a filename (include subdirectory if necessary):" << endl;
  cin >> fileName;

  MapLoader* mapLoader = nullptr;
  if (mapType == 1) {
    mapLoader = new MapLoader();
  } else if (mapType == 2) {
    mapLoader = new ConquestFileReaderAdapter();
  }

  // Create a new map loader
  Map* map = mapLoader->GenerateMap(fileName);
  for (Continent* cont : map->GetContinents()) {
    std::cout << *cont->GetName() << std::endl;
    for (Territory* terr : *cont->GetTerritories()) {
      std::cout << *terr->GetName() << " has neighbors: ";
      for (Territory* neb : *terr->GetNeighbors()) {
        std::cout << *neb->GetName() << ", ";
      }
      std::cout << std::endl;
    }
    std::cout
        << "----------------------------------------------------------------"
        << std::endl;
    std::cout << std::endl;
  }
  delete mapLoader;
  return 0;
}
#endif
