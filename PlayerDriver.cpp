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

#include "Player.h"
#include "Map.h"
#include "Orders.h"

//#define PLAYER_DRIVER
#ifdef PLAYER_DRIVER

int main() {

    // Creating continents
    Continent c[] = {
      Continent("W", 1),
      Continent("X", 2)
    };

    // Creating Territories
    Territory* terr1 = c[0].CreateTerritory(101, "A");
    Territory* terr2 = c[1].CreateTerritory(102, "B");
    Territory* terr3 = c[1].CreateTerritory(103, "C");
    std::vector<Territory*> territories{terr1, terr2, terr3};

    // Pointer to a vector of pointers of continents
    std::vector<Continent*> continents({ &c[0], &c[1] });

    // Creating a Map and adding the 2 continents
    Map* map = new Map(3, 2);
    map->AddContinent(&c[0]);
    map->AddContinent(&c[1]);

    // Creating Player object with default constructor
    Player* p1 = new Player();
    std::cout << "Player p1: " << *p1 << std::endl;

    // Adding a territory to p1
    p1->AddTerritoryToPlayer(terr1);

    // Creating a Player object with copy constructor
    Player *p2 = new Player(*p1);
    std::cout << "Player p2: " << *p2 << std::endl;
   
   // Adding a territory to p2
    p2->AddTerritoryToPlayer(terr2);
    std::cout << "Player p1: " << *p1 << std::endl;
    std::cout << "Player p2: " << *p2 << std::endl;

    // Creating a Player object with parametric constructor
    Player* p3 = new Player(territories);
    std::cout << "Player p3: " << *p3 << std::endl;

    // Adding an order (deploy by example)
    p1->issueOrder();
    // Creating and adding a card
    Card* card1 = new Card(0);
    p1->AddCardToPlayer(card1);

    // Assignment operator
    Player* p4 = p1;
    std::cout << "Player p1: " << *p1 << std::endl;
    std::cout << "Player p4: " << *p4 << std::endl;

    // Displaying p1's territories to defend (should be A)
    std::vector<Territory*> terrToDefend = p1->toDefend();
    std::cout << "Player p1's territories to defend:" << std::endl;
    for (Territory* t : terrToDefend)
        std::cout << (*t->GetName()) << std::endl;

    // Displaying p1's territories to attack (should be B and C)
    std::vector<Territory*> terrToAttack = p1->toAttack(*map);
    std::cout << "\nPlayer p1's territories to attack:" << std::endl;
    for (Territory* t : terrToAttack)
        std::cout << (*t->GetName()) << std::endl;

    // Deleting everything
    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete terr1;
    delete terr2;
    delete terr3;
    delete map;
    delete card1;

    p1 = nullptr;
    p2 = nullptr;
    p3 = nullptr;
    p4 = nullptr;
    terr1 = nullptr;
    terr2 = nullptr;
    terr3 = nullptr;
    map = nullptr;
    card1 = nullptr;
};

#endif
