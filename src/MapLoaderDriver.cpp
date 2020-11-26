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
#include <string>

#include "MapLoader.h"

#define MAP_LOADER_DRIVER
#ifdef MAP_LOADER_DRIVER

using std::cin;
using std::cout;
using std::endl;

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
  Map *map = mapLoader->GenerateMap(fileName);

  //cout << "Map is valid: " << endl;
  //bool mapIsValid{map->ValidateMap()};

  // Display the map somehow
  // TODO

  //delete map;
  return 0;
}
#endif
