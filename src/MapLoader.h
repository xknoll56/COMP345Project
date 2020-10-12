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
#include <fstream>
#include <iostream>
#include <regex>

#include "Map.h"

// The map loader class will be used to simply read a map file, and instantiate
// a Map object based on the data.
class MapLoader {
 public:
  MapLoader();
  // Destruction of the map will not affect the map
  ~MapLoader();
  // The main function here, it will return a null pointer if the data is not
  // valid
  Map* GenerateMap(const std::string filePath);

 private:
  // Struct used to store all the validity data as the map loader scans a map
  // file.
  struct ValidityData {
    // The header "[files]" must be found
    bool filesFound = false;
    // The header "[continents]" must be found
    bool continentsFound = false;
    // The header "[countries]" must be found
    bool territoriesFound = false;
    // The header "[borders]" must be found
    bool bordersFound = false;
    // We will assume all the data within each of the headers until an error is
    // found, errors include numbers in the wrong places, insufficient data
    // within a line, and so on.
    bool validData = true;

    // Returns true if all the data processed is correct, to be checked at the
    // end before returning a map.
    inline bool IsValid() {
      return filesFound && continentsFound && territoriesFound &&
             bordersFound && validData;
    }
  };

  // An enum used for the GenerateMap function to process the correct type of
  // data at that time.
  enum DataType { Continents = 0, Territories, Borders, None };

  // A pointer to the map that is being generated.
  Map* generatedMap;
  // A vector of pointers to continents which will be later used to set into the
  // Maps data directly
  std::vector<Continent*> continents;
  // Counts the number of continents to generated the continents array.
  int continentsCount;
  // An index used to access the continents sizes
  int index;
  // An array of continents sizes that correspond to the number of territories
  // in each of the continents
  int* continentsSizes;
  // The number of territories in the file
  int territoriesCount;
  // An array to the number of neighbors for each of the territories
  int* neighborsSizes;
  // A pointer to the validity data
  ValidityData* validityData;
  // A untility function that splits a line of data by spaces into a vector of
  // strings
  std::vector<std::string> Split(const std::string line) const;
  // Preprocess of the continents the continents to determine the amount of
  // continents and the size of the continents vector (as well as reserving
  // space for it.
  void PreprocessContinents(const std::string line, DataType* const dataType);
  // Preprocess of the territories data to determine how many territories on the
  // map and to incriment the counter for the continents sizes that each
  // territory belongs to.
  void PreprocessTerritories(const std::string line, DataType* const dataType);
  // Preprocess of the borders data to determine how many neighbors each of the
  // territories have.
  void PreprocessBorders(const std::string line, DataType* const dataType);
  // Processes the continents data to generate Continents gameobjects, then
  // generate a Map and add the continets to that.
  void ProcessContinents(const std::string line, DataType* const dataType);
  // Process the territories to generate each of the territories and add them to
  // the generated map and continents
  void ProcessTerritories(const std::string line, DataType* const dataType);
  // Process the borders and set them to the corresponding territory
  void ProcessBorders(const std::string line, DataType* const dataType);
  // Utility function that is used in the GenerateMap function to control all
  // the processing.
  void ReadFile(const std::string path);

  // The copy constructur and assignment overload operators just create new
  // objects because this object is just used for map creation
  MapLoader(const MapLoader& toCopy);
  MapLoader& operator=(const MapLoader& rightSide);
  friend std::ostream& operator<<(std::ostream& out, const MapLoader& toOutput);
};
