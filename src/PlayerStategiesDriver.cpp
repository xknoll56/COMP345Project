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

#include <iostream>
#include <string>
#include <vector>

#include "Cards.h"
#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"


#define PLAYERSTRATEGIES_DRIVER
#ifdef PLAYERSTRATEGIES_DRIVER

int main() {


    // Create and initialize a game engine
    GameEngine* gameEngine = new GameEngine();
    gameEngine->Init();
    gameEngine->StartupPhase();
    gameEngine->MainGameLoop();

    //Avoid memory leaks
    delete gameEngine;


};

#endif
