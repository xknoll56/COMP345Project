#pragma once
#include "Player.h"
#include "Orders.h"
#include <list>
#include <string>
#include <iostream>

class Player;
class Order;
class Deploy;
class Advance;

class Subject;
class Observer {
public:
	
	~Observer()
	{

	}
	virtual void Update() = 0;
	virtual void UpdateOrderIssue() = 0;
	virtual void UpdateOrderExecute() = 0;
	virtual void UpdateReinforcements(int) = 0;
	virtual void UpdateStats() = 0;
	void UpdateSubject(Subject* subject)
	{
		this->currentSubject = subject;
	}

protected:
	Subject* currentSubject = nullptr;
	Observer()
	{

	}
private:
	
};


class Subject {
public:
	

	Subject::Subject() {
		static int i = 0;
		name = std::to_string(i);
		observers = new std::list<Observer*>();
		i++;
	}

	Subject::~Subject()
	{
		delete observers;
	}

	virtual void Subject::Attach(Observer* obs)
	{
		observers->push_back(obs);
	}

	void Subject::DetachPhaseObserver(Observer* obs)
	{
		observers->remove(obs);
	}

	void Subject::Notify()
	{
		std::list<Observer*>::iterator it = observers->begin();
		for (; it != observers->end(); ++it)
		{
			(*it)->UpdateSubject(this);
			(*it)->Update();
		}
	}

	virtual void Subject::NotifyOrderIssue()
	{
		std::list<Observer*>::iterator it = observers->begin();
		for (; it != observers->end(); ++it)
		{
			(*it)->UpdateSubject(this);
			(*it)->UpdateOrderIssue();
		}
	}

	virtual void Subject::NotifyOrderExecute()
	{
		std::list<Observer*>::iterator it = observers->begin();
		for (; it != observers->end(); ++it)
		{
			(*it)->UpdateSubject(this);
			(*it)->UpdateOrderExecute();
		}
	}

	virtual void Subject::NotifyReinforcements(int troops)
	{
		std::list<Observer*>::iterator it = observers->begin();
		for (; it != observers->end(); ++it)
		{
			(*it)->UpdateSubject(this);
			(*it)->UpdateReinforcements(troops);
		}
	}


	std::string GetName()
	{
		return name;
	}
private:
	std::string name;
	std::list<Observer*>* observers;
};


class PhaseObserver : public Observer {
public:

	void Update()
	{

	}

	void UpdateReinforcements(int troops)
	{
		std::cout << "Player " << currentSubject->GetName() << ": Reinforcement phase" << std::endl;
		std::cout << "\tAdding " << troops << " units to reinforcement pool" << std::endl;
		//std::cout << "Player " << player->GetName() << ": Reinforcement phase" << std::endl;
	}

	void UpdateOrderIssue()
	{

		std::cout << "Player " << currentSubject->GetName() << ": Issue orders phase" << std::endl;
	}

	void UpdateOrderExecute()
	{
		std::cout << "Player " << currentSubject->GetName() << ": Executing orders phase" << std::endl;

	}
	void UpdateStats()
	{

	}

private:


};

class GameStatisticsObserver: public Observer
{
public:
	GameStatisticsObserver(int numPlayers)
	{

	}
private:

};
