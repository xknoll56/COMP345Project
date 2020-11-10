// COMP 345 - Project
// Part 2
//
// Sophie Renaud       -  40132563
// Joey Abou-Chaker    -  40055551
// Jordan Goulet       -  40075688
// Xavier Knoll        -  40132134
// Sébastien Champoux  -  40133449
//
// Based on the 'https://www.warzone.com/' game.
#pragma once
#include "Player.h"
#include "MapLoader.h"

//#define OBSERVER_DRIVER
#ifdef OBSERVER_DRIVER
int main() {
  MapLoader* ml = new MapLoader();
  Map* map = ml->GenerateMap("MapFiles/France.map");

  Player p;
  Player p2;

  // p.name = "the name";
  GameStatisticsObserver gso =
     GameStatisticsObserver(2, map->GetTerritories()->size());
  PhaseObserver ph = PhaseObserver();

  p.Attach(&gso);
  p2.Attach(&gso);
  p.Attach(&ph);
  p2.Attach(&ph);

    
  p.AddTerritoryToPlayer(map->GetTerritories()->at(0));
  system("pause");
  p2.AddTerritoryToPlayer(map->GetTerritories()->at(1));
  system("pause");
  p.IssueOrder();
  system("pause");
  p2.IssueOrder();
  system("pause");
  p.AddArmiesToReinforcementPool(100);
  system("pause");
  p2.AddArmiesToReinforcementPool(200);
  system("pause");
  p.ExecuteNextOrder();
  system("pause");
  p2.ExecuteNextOrder();
  system("pause");
  p.RemoveTerritoryFromPlayer(map->GetTerritories()->at(0));
  system("pause");
  for (Territory* terr: *map->GetTerritories()) {
    p2.AddTerritoryToPlayer(terr);
    //system("pause");
  }


  return 0;
}
#endif