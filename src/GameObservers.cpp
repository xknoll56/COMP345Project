#pragma once
#include "GameObservers.h"
#include "Player.h"


Subject::Subject() {
	static int i = 0;
	id = i;
	name = std::to_string(i);
	observers = new std::list<Observer*>();
	i++;
}

Subject::~Subject() { delete observers; }

void Subject::Attach(Observer* obs) { observers->push_back(obs); }

void Subject::Detach(Observer* obs) { observers->remove(obs); }

void Subject::Notify() {
	std::list<Observer*>::iterator it = observers->begin();
	for (; it != observers->end(); ++it) {
		(*it)->Update();
	}
}

int Subject::getId() { return id; }

std::string Subject::GetName() { return name; }


Subject::Subject(const Subject& toCopy) {
	observers = toCopy.observers;
	this->id = toCopy.id;
	this->name = toCopy.name;
}
Subject Subject::operator=(const Subject& toCopy) {
	observers = toCopy.observers;
	this->id = toCopy.id;
	this->name = toCopy.name;
	return *this;
}
std::ostream& operator<<(std::ostream& out, const Subject& subject) {
	out << "Subject named: " << subject.name << " has id: " << subject.id;
	return out;
}

GameStatisticsObserver::GameStatisticsObserver(int numTerritories) {
	this->numTerritories = numTerritories;
}

GameStatisticsObserver::~GameStatisticsObserver() {
	//The array should be cleared but the players are deleted in game engine.
	//players.clear();
}

void GameStatisticsObserver::Start() { gameStarted = true; }

void GameStatisticsObserver::Update() {

	if (players)
	{
		for (Player* player : *players) {
			float ownedPercent = 100.0f * (float)player->GetOwnedTerritories()->size() /
				(float)numTerritories;
			std::cout << "Player " << player->GetName() << ": owns " << ownedPercent
				<< "% || ";
		}
	}
	//Now to check if a players been eliminated
	if (playerNames.size() != players->size()) {

		for (int i = 0; i < playerNames.size(); i++) {
			bool found = false;
			int index = i;
			for (Player* player : *players) {
				if (player->GetName().compare(playerNames[i]) == 0) {
					found = true;
					index = i;
					break;
				}
			}
			if (!found) {
				std::cout << "\n***************Player " << playerNames.at(index) << " has been eliminated.*******************\n";
				playerNames.erase(playerNames.begin() + index);
				if (playerNames.size() == 1) {
					std::cout << "\n***************Player " << playerNames.at(index) << " has won the game.*******************\n";
				}
			}

		}
	}
	std::cout << "\n----------------------------------------------------------------" << std::endl;

	std::cout << "\n\n";
}

void GameStatisticsObserver::AddPlayers(GameEngine* ge) {
	players = ge->getPlayersAdress();
	for (Player* player : *players)
	{
		playerNames.push_back(player->GetName());
	}
}


GameStatisticsObserver::GameStatisticsObserver(const GameStatisticsObserver& toCopy) {
	this->gameStarted = toCopy.gameStarted;
	this->numTerritories = toCopy.numTerritories;
	this->players = toCopy.players;
}

GameStatisticsObserver GameStatisticsObserver::operator=(const GameStatisticsObserver& toCopy) {
	this->gameStarted = toCopy.gameStarted;
	this->numTerritories = toCopy.numTerritories;
	this->players = toCopy.players;
	return *this;
}
//Stream overload
std::ostream& operator<<(std::ostream& out, const GameStatisticsObserver& subject) {
	out << "GameStatsObserver currently has " << subject.playerNames.size() << " players being observed." << std::endl;
	return out;
}

PhaseObserver::PhaseObserver() {

}

PhaseObserver::~PhaseObserver() {

}

void PhaseObserver::AddPlayers(GameEngine* ge) {
	players = ge->getPlayersAdress();
}
void PhaseObserver::Update() {

	if (players)
	{
		Player* current = nullptr;
		for (Player* player : *players) {
			if (player->GetPhase() != Phase::None) current = player;
		}

		if (current != nullptr) {
			Phase phase = current->GetPhase();

			// Now knowing the phase and player stuff relative to those can be done
			switch (phase) {
			case Phase::ExecuteOrders:
				this->UpdateExecuteOrders(current);
				break;
			case Phase::IssueOrders:
				this->UpdateIssueOrders(current);
				break;
			case Phase::Reinforcement:
				this->UpdateReinforcements(current);
				break;
			}

		}
	}
}

void PhaseObserver::UpdateExecuteOrders(Player* player) {
	std::string s("Player " + player->GetName() + ": Execute orders phase\n");
	std::cout << s << "\n" << std::endl;
}

void PhaseObserver::UpdateIssueOrders(Player* player) {
	std::string s("Player " + player->GetName() + ": Issue orders phase\n");
	std::cout << s << "\n" << std::endl;
}

void PhaseObserver::UpdateReinforcements(Player* player) {
	std::string s("Player " + player->GetName() + ": Reinforcements phase\n" +
		"\t Player currently has reinforcements of: " +
		std::to_string(player->GetReinforcementPoolCount()));
	std::cout << s << "\n" << std::endl;
}

PhaseObserver::PhaseObserver(const PhaseObserver& toCopy) {
	this->players = toCopy.players;
	this->currentPhase = toCopy.currentPhase;
}
PhaseObserver PhaseObserver::operator=(const PhaseObserver& toCopy) {
	this->players = toCopy.players;
	this->currentPhase = toCopy.currentPhase;
	return *this;
}
std::ostream& operator<<(std::ostream& out, const PhaseObserver& subject) {
	out << "This phase observer is on phase: ";
	switch (subject.currentPhase) {
	case(Phase::ExecuteOrders):
		out << "Execute Orders";
		break;
	case(Phase::IssueOrders):
		out << "Issue Orders";
		break;
	case(Phase::Reinforcement):
		out << "Reinforcements";
		break;

	}
	return out;
}