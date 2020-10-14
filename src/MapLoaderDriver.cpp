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

#include "MapLoader.h"

#define MAP_LOADER_DRIVER
#ifdef MAP_LOADER_DRIVER

int main() {
  // Create a new map loader
  MapLoader* ml = new MapLoader();
  std::cout << (*ml);
  std::string input;
  do {
    std::cout
        << "Please Enter the name of the map file that is to be created. \n";
    // Generate the map
    std::cin >> input;
    Map* map = ml->GenerateMap("MapFiles/" + input);
    if(map->Validate()) {
      std::cout << "Map validated\n" << std::endl;
    }
    // If the map is valid, it will be displayed, otherwise if exit is entered quit.
    if (map != nullptr ) {
      for (int i = 0; i < map->getContinents()->size(); ++i) {
        Continent* continent = (*map->getContinents())[i];
        std::cout << *continent->getName() << std::endl;
        for (int j = 0; j < continent->getTerritories()->size(); ++j) {
         
          Territory* territory = (continent->getTerritories())->at(j);
          std::cout << *territory->getName() << " Neighbors: ";
          for (auto neb : *territory->getNeighbors()) {
            std::cout << "  " << *neb->getName() << "  ";
          }
          std::cout << std::endl;
        }
        std::cout << "*****************************************************"
                  << std::endl;
        std::cout << std::endl;
      }
    }

    delete map;
  } while (input.compare("exit")!=0);

  return 0;
}
#endif
