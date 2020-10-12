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
#include "Cards.h"

//#define CARDS_DRIVER
#ifdef CARDS_DRIVER

int main()
{

    // 1. Create new Deck
    Deck deck{};

    // 2. Create the Cards
    Card* card1 = new BombCard{ 10 };
    Card* card2 = new ReinforcementCard{ 20 };
    Card* card3 = new BlockadeCard{ 30 };
    Card* card4 = new AirliftCard{ 40 };
    Card* card5 = new DiplomacyCard{ 50 };
    Card* card6 = new BombCard{ 60 };
    Card* card7 = new ReinforcementCard{ 70 };
    Card* card8 = new BlockadeCard{ 80 };
    Card* card9 = new AirliftCard{ 90 };
    Card* card10 = new DiplomacyCard{ 100 };
    deck.addCard(card1);
    deck.addCard(card2);
    deck.addCard(card3);
    deck.addCard(card4);
    deck.addCard(card5);
    deck.addCard(card6);
    deck.addCard(card7);
    deck.addCard(card8);
    deck.addCard(card9);
    deck.addCard(card10);

    std::cout << "Deck before drawing cards:\n";
    std::cout << deck;
    std::cout << '\n';

    // 3. Create a Hand
    Hand hand{};
    constexpr int HAND_SIZE{ 3 }; // set size of hand 

    for (int i = 0; i < HAND_SIZE; ++i) // draw cards to fill hand  
    {
        hand.addCard(deck.draw());
    }

    std::cout << "\nDeck after drawing cards:\n";
    std::cout << deck;

    std::cout << "\nPlayer's current Hand:\n";
    std::cout << hand;

    std::cout << "\nEnter a card value you want to play:\n";
    int useCardValue;
    // WAITING FOR INPUT
    std::cin >> useCardValue; // for now if the input is invalid no card wil be played any card 

    // Use Card and get pointer to card from hand
    Card* playedCard = hand.useCard(useCardValue);

    // Add card played to deck
    if (playedCard != nullptr)
    {
        deck.addCard(playedCard);
    }

    std::cout << "\nDeck after the user inputed card is played:\n";
    std::cout << deck;

    std::cout << "\nHand after the user inputed card is played:\n";
    std::cout << hand;

    playedCard = hand.playFirstCard();

    if (playedCard != nullptr)
    {
        deck.addCard(playedCard);
    }

    playedCard = hand.playFirstCard();
    deck.addCard(playedCard);



    std::cout << "\nDeck after all cards in the hand are played:\n";
    std::cout << deck;

    std::cout << "\nHand after all cards are played:\n";
    std::cout << hand;


    return 0;
}





#endif