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

//#define MAP_LOADER_DRIVER
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
    // If the map is valid, it will be displayed, otherwise if exit is entered quit.
    if (map != nullptr && map->Validate()) {
      for (int i = 0; i < map->GetContinents().size(); ++i) {
        Continent* continent = map->GetContinents()[i];
        std::cout << continent->GetName() << std::endl;
        for (int j = 0; j < continent->GetTerritories()->size(); ++j) {
          Territory* territory = &(continent->GetTerritories())->at(j);
          std::cout << *territory->GetName() << " Neighbors: ";
          std::vector<Territory*> neighbors = map->GetNeighbors(territory);
          for (int k = 0; k < neighbors.size(); ++k) {
            Territory* neb = neighbors[k];
            std::cout << "  " << *neb->GetName();
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
