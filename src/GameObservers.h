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

#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

class Player;
class Subject;
class Order;
class Card;

//Base observer abstract class.
class Observer {
public:
	~Observer() {}
	virtual void Update() = 0;

protected:
	Observer() {}

private:
};

//Subject class, to be inherited by player
class Subject {
public:
	Subject();
	~Subject();
	//Attach the subject to an observer
	virtual void Attach(Observer* obs);
	//Detach the subject to an observer
	void Detach(Observer* obs);
	//Notify all observers
	void Notify();
	//Get the id of the subject
	int getId();
	//Get the name of the subject
	std::string GetName();

private:
	//TODO, these should go into player
	std::string name;
	int id = 0;
	//A list of all the concrete observers
	std::list<Observer*>* observers;
};

//Enum for determining the current phase each of the players is on.
enum Phase { Reinforcement = 0, IssueOrders = 1, ExecuteOrders = 2, None };

//Concrete observer class for outputing player percent ownage stats
class GameStatisticsObserver : public Observer {
public:

	GameStatisticsObserver(int numTerritories);
	~GameStatisticsObserver();
	//The update method will calculate the percent of territories owned by each player and determine if a player is still  in the game or has won 
	void Update();
	//Adds a player
	void AddPlayer(Player* player);
	//Starts the observer, should be done after all players are attached and map is distributed.
	void Start();

private:
	//An array of players
	std::vector<Player*> players;
	//Total number of territories on the map
	int numTerritories;
	bool gameStarted = false;
};

class PhaseObserver : public Observer {
public:
	PhaseObserver();
	~PhaseObserver();
	//Update will display which player is currently playing on which phase
	void Update();
	//Adds a player to the array
	void AddPlayer(Player* player);

private:
	//Displays information about reinforcements
	void UpdateReinforcements(Player* player);
	//Displays information about orders being issued
	void UpdateIssueOrders(Player* player);
	//Displays information about orders being executed
	void UpdateExecuteOrders(Player* player);
	//An array of all the players
	std::vector<Player*> players;
	//The current phase that a player is on
	Phase currentPhase = Phase::None;

};
