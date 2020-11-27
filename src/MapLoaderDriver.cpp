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
#include <filesystem>



#define MAP_LOADER_DRIVER
#ifdef MAP_LOADER_DRIVER



int main() {
  // Create a new map loader
	ConquestFileReader reader;
	MapLoader* ml; 
	ml = new ConquestFileReaderAdapter(&reader);
	Map* map = ml->GenerateMap("ConquestMaps/USA.map");
	for (Continent* cont : map->GetContinents()) {
		std::cout << *cont->GetName() << std::endl;
		for (Territory* terr : *cont->GetTerritories()) {
			std::cout << *terr->GetName() << " has neighbors: ";
			for (Territory* neb : *terr->GetNeighbors()) {
				std::cout << *neb->GetName() << ", ";
			}
			std::cout << std::endl;
		}
		std::cout << "----------------------------------------------------------------" << std::endl;
		std::cout << std::endl;
	}
  return 0;
}
#endif
