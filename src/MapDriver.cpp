#include <iostream>
#include <string>

#include "Map.h"

//#define MAP_DRIVER
#ifdef MAP_DRIVER

int main() { 
  Continent c1 = Continent("c1", 0);
  Continent c2 = Continent("c2", 0);

  c1.CreateTerritory(0, "t0");
  c1.CreateTerritory(1, "t1");
  c2.CreateTerritory(2, "t2");
  c2.CreateTerritory(3, "t3");

  Map map = Map(4, 2);

  map.AddContinent(&c1);
  map.AddContinent(&c2);

  map.AddBorder({0, 1, 3});
  map.AddBorder({1, 0, 2});
  map.AddBorder({2, 0});
  map.AddBorder({3, 0});

  std::cout << map.Validate() << std::endl;
}

#endif