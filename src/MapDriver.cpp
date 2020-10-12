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

#define MAP_DRIVER
#ifdef MAP_DRIVER

#include <iostream>
#include <string>

#include "Map.h"

/*
This driver tests the functionality of the Map class and its related classes.
*/
    int main() {
  std::cout << "Map Driver" << std::endl;

  {
    std::cout << " Map 1: empty map" << std::endl;
    std::cout << "        Expected result: valid" << std::endl;
    Map map1 = Map(0, 0);
    if (map1.Validate()) {
      std::cout << "        Result: valid" << std::endl;
    } else {
      std::cout << "        Result: invalid" << std::endl;
    }
  }
  
  {
    std::cout << " Map 2: single empty continent" << std::endl;
    std::cout << "        Expected result: valid" << std::endl;
    Continent c2_1 = Continent("c1", 0);
    Map map2 = Map(0, 1);
    map2.AddContinent(&c2_1);
    if (map2.Validate()) {
      std::cout << "        Result: valid" << std::endl;
    } else {
      std::cout << "        Result: invalid" << std::endl;
    }
  }

  {
    std::cout << " Map 3: single territory" << std::endl;
    std::cout << "        Expected result: valid" << std::endl;
    Continent c3_1 = Continent("c1", 0);
    c3_1.CreateTerritory(69, "t69");  // Territories can be given any ID
    Map map3 = Map(1, 1);
    map3.AddContinent(&c3_1);
    if (map3.Validate()) {
      std::cout << "        Result: valid" << std::endl;
    } else {
      std::cout << "        Result: invalid" << std::endl;
    }
  }

  {
    std::cout << " Map 4: multiple empty continents" << std::endl;
    std::cout << "        Expected result: valid" << std::endl;
    Continent c4_1 = Continent("c1", 0);
    Continent c4_2 = Continent("c2", 0);
    Map map4 = Map(0, 4);
    map4.AddContinent(&c4_1);
    map4.AddContinent(&c4_2);
    if (map4.Validate()) {
      std::cout << "        Result: valid" << std::endl;
    } else {
      std::cout << "        Result: invalid" << std::endl;
    }
  }

  {
    std::cout << " Map 5: two territories without borders" << std::endl;
    std::cout << "        Expected result: invalid" << std::endl;
    Continent c5_1 = Continent("c1", 0);
    c5_1.CreateTerritory(1, "t1");
    c5_1.CreateTerritory(2, "t2");
    Map map5 = Map(2, 1);
    map5.AddContinent(&c5_1);
    if (map5.Validate()) {
      std::cout << "        Result: valid" << std::endl;
    } else {
      std::cout << "        Result: invalid" << std::endl;
    }
  }

  {
    std::cout
        << " Map 6: two territories weakly connected but not strongly connected"
        << std::endl;
    std::cout << "        {A -> B}" << std::endl;
    std::cout << "        Expected result: invalid" << std::endl;
    Continent c6_1 = Continent("c1", 0);
    c6_1.CreateTerritory(1, "t1");
    c6_1.CreateTerritory(2, "t2");
    Map map6 = Map(2, 1);
    map6.AddContinent(&c6_1);
    map6.AddBorder({1, 2});
    if (map6.Validate()) {
      std::cout << "        Result: valid" << std::endl;
    } else {
      std::cout << "        Result: invalid" << std::endl;
    }
  }

  {
    std::cout << " Map 7: two territories strongly connected" << std::endl;
    std::cout << "        {A <-> B}" << std::endl;
    std::cout << "        Expected result: valid" << std::endl;
    Continent c7_1 = Continent("c1", 0);
    c7_1.CreateTerritory(1, "t1");
    c7_1.CreateTerritory(2, "t2");
    Map map7 = Map(2, 1);
    map7.AddContinent(&c7_1);
    map7.AddBorder({1, 2});
    map7.AddBorder({2, 1});
    if (map7.Validate()) {
      std::cout << "        Result: valid" << std::endl;
    } else {
      std::cout << "        Result: invalid" << std::endl;
    }
  }

  {
    std::cout << " Map 8: three vertices cycle" << std::endl;
    std::cout << "        {A -> B -> C -> A}" << std::endl;
    std::cout << "        Expected result: valid" << std::endl;
    Continent c8_1 = Continent("c1", 0);
    c8_1.CreateTerritory(1, "t1");
    c8_1.CreateTerritory(2, "t2");
    c8_1.CreateTerritory(3, "t3");
    Map map8 = Map(3, 1);
    map8.AddContinent(&c8_1);
    map8.AddBorder({1, 2});
    map8.AddBorder({2, 3});
    map8.AddBorder({3, 1});
    if (map8.Validate()) {
      std::cout << "        Result: valid" << std::endl;
    } else {
      std::cout << "        Result: invalid" << std::endl;
    }
  }

  {
    std::cout << " Map 9: connected map but disconnected continent"
              << std::endl;
    std::cout << "        Expected result: invalid" << std::endl;
    Continent c9_1 = Continent("c1", 0);
    Continent c9_2 = Continent("c2", 0);
    c9_1.CreateTerritory(1, "t1");
    c9_1.CreateTerritory(2, "t2");
    c9_2.CreateTerritory(3, "t3");
    Map map9 = Map(3, 2);
    map9.AddContinent(&c9_1);
    map9.AddContinent(&c9_2);
    map9.AddBorder({1, 3});
    map9.AddBorder({2, 3});
    map9.AddBorder({3, 1, 2});
    if (map9.Validate()) {
      std::cout << "        Result: valid" << std::endl;
    } else {
      std::cout << "        Result: invalid" << std::endl;
    }
  }

  {
    std::cout << " Map 10: connected continents but disconnected map"
              << std::endl;
    std::cout << "         Expected result: invalid" << std::endl;
    Continent c10_1 = Continent("c1", 0);
    Continent c10_2 = Continent("c2", 0);
    c10_1.CreateTerritory(1, "t1");
    c10_1.CreateTerritory(2, "t2");
    c10_2.CreateTerritory(3, "t3");
    Map map10 = Map(3, 2);
    map10.AddContinent(&c10_1);
    map10.AddContinent(&c10_2);
    map10.AddBorder({1, 2});
    map10.AddBorder({2, 1});
    map10.AddBorder({3, 1, 2});
    if (map10.Validate()) {
      std::cout << "         Result: valid" << std::endl;
    } else {
      std::cout << "         Result: invalid" << std::endl;
    }
  }

  {
    std::cout << " Map 16: wildest valid map" << std::endl;
    std::cout << "         Expected result: valid" << std::endl;
    Continent c16[4] = {Continent("EmptyLand", 0), Continent("SingleLand", 0),
                        Continent("c3", 0), Continent("c4", 0)};
    c16[1].CreateTerritory(1, "t1");
    c16[2].CreateTerritory(2, "t2");
    c16[2].CreateTerritory(3, "t3");
    c16[2].CreateTerritory(4, "t4");
    c16[2].CreateTerritory(5, "t5");
    c16[2].CreateTerritory(6, "t6");
    c16[3].CreateTerritory(7, "t7");
    c16[3].CreateTerritory(8, "t8");
    c16[3].CreateTerritory(9, "t9");
    c16[3].CreateTerritory(10, "t10");
    c16[3].CreateTerritory(11, "t11");
    c16[3].CreateTerritory(12, "t12");
    Map map16 = Map(12, 4);
    map16.AddContinent(&c16[0]);
    map16.AddContinent(&c16[1]);
    map16.AddContinent(&c16[2]);
    map16.AddContinent(&c16[3]);
    map16.AddBorder({1, 6});
    map16.AddBorder({2, 1, 3, 4});
    map16.AddBorder({3, 5});
    map16.AddBorder({4, 5, 6});
    map16.AddBorder({5, 4});
    map16.AddBorder({6, 2, 12});
    map16.AddBorder({7, 2, 8});
    map16.AddBorder({8, 12});
    map16.AddBorder({9, 8});
    map16.AddBorder({10, 9});
    map16.AddBorder({11, 7, 9, 10});
    map16.AddBorder({12, 11});
    if (map16.Validate()) {
      std::cout << "         Result: valid" << std::endl;
    } else {
      std::cout << "         Result: invalid" << std::endl;
    }
  }

}

#endif