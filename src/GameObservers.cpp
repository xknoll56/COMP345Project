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

GameStatisticsObserver::GameStatisticsObserver(int numTerritories) {
	this->numTerritories = numTerritories;
}

GameStatisticsObserver::~GameStatisticsObserver() {
	//The array should be cleared but the players are deleted in game engine.
	players.clear();
}

void GameStatisticsObserver::Start() { gameStarted = true; }

void GameStatisticsObserver::Update() {

	for (Player* player : players) {
		float ownedPercent = 100.0f * (float)player->GetOwnedTerritories()->size() /
			(float)numTerritories;
		std::cout << "Player " << player->GetName() << ": owns " << ownedPercent
			<< "% || ";
	}
	std::cout << "\n----------------------------------------------------------------" << std::endl;
	if (gameStarted) {
		for (int i = 0; i < players.size(); i++) {
			if (players[i]->GetOwnedTerritories()->size() == 0) {
				players.erase(players.begin() + i);
				std::cout << "\nPlayer " << players[i]->GetName()
					<< " has been eliminated." << std::endl;
			}
			else if (players[i]->GetOwnedTerritories()->size() == numTerritories) {
				std::cout << "\nPlayer " << players[i]->GetName()
					<< " has conquered all of the Territories!" << std::endl;
			}
		}
	}
	std::cout << "\n\n";
}

void GameStatisticsObserver::AddPlayer(Player* player) {
	this->players.push_back(player);
}

PhaseObserver::PhaseObserver() {

}

PhaseObserver::~PhaseObserver() {
	players.clear();
}

void PhaseObserver::AddPlayer(Player* player) {
	this->players.push_back(player);
}
void PhaseObserver::Update() {
	Player* current = nullptr;
	for (Player* player : players) {
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