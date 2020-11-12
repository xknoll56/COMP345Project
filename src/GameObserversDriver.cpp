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
#include "MapLoader.h"
#include "Player.h"

//#define OBSERVER_DRIVER
#ifdef OBSERVER_DRIVER
int main() {
  MapLoader* ml = new MapLoader();
  Map* map = ml->GenerateMap("MapFiles/France.map");

  Player p;
  Player p2;

  p.AddTerritoryToPlayer(map->GetTerritories()->at(0));
  p2.AddTerritoryToPlayer(map->GetTerritories()->at(1));
  // p.name = "the name";
  GameStatisticsObserver gso =
      GameStatisticsObserver(map->GetTerritories()->size());
  PhaseObserver ph = PhaseObserver();

  p.Attach(&gso);
  p2.Attach(&gso);
  p.Attach(&ph);
  p2.Attach(&ph);

  gso.AddPlayer(&p);
  gso.AddPlayer(&p2);
  ph.AddPlayer(&p);
  ph.AddPlayer(&p2);

  gso.Start();
  system("Pause");
  p.AddArmiesToReinforcementPool(100);
  system("Pause");
  p2.AddArmiesToReinforcementPool(200);
  system("Pause");
  p.IssueOrder();
  system("Pause");
  p2.IssueOrder();
  system("Pause");

  p.RemoveTerritoryFromPlayer(p.GetOwnedTerritories()->at(0));
  system("Pause");

  for (Territory* territory : *map->GetTerritories())
    p2.AddTerritoryToPlayer(territory);
  return 0;
}
#endif