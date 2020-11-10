#pragma once
#include "Player.h"
#include "GameObservers.h"
#include "MapLoader.h"




int main()
{
	MapLoader* ml = new MapLoader();
	Map* map = ml->GenerateMap("MapFiles/France.map");
	PhaseObserver* ph = new PhaseObserver(); 
	Player*  p= new Player();
	Player* p2 = new Player();
	p->AddTerritoryToPlayer(map->GetTerritories()->at(0));
	p2->AddTerritoryToPlayer(map->GetTerritories()->at(1));
	//p.name = "the name";
	p->Attach(ph);
	p2->Attach(ph);
	//p.Notify();
	p->IssueOrder();
	p2->IssueOrder();
	p->AddArmiesToReinforcementPool(100);
	p2->AddArmiesToReinforcementPool(100);
	p->ExecuteNextOrder();
	p2->ExecuteNextOrder();
	delete ml;
	delete map;
	delete p;
	delete ph;
	return 0;

}