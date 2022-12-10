#pragma once

#include "Card.h"
#include "Dealer.h"
#include "Deck.h"
#include "Player.h"
#include <string>
#include <iostream>

using namespace std;

class Game
{
public:
	
	Deck deck;
	Dealer dealer;
	Player player;

	void startGame()
	{
		deck.initializeDeck();
		player.playerMoney = 500;
		
		dealer.TakeCard(deck.getCard());
		dealer.TakeCard(deck.getCard());

		player.TakeCard(deck.getCard());
		player.TakeCard(deck.getCard());
	}

	void newGame()
	{
		if (deck.cards.size() <= 5)
		{
			deck.initializeDeck();
		}
		player.playerCards.clear();
		dealer.dealerCards.clear();

		dealer.TakeCard(deck.getCard());
		dealer.TakeCard(deck.getCard());

		player.TakeCard(deck.getCard());
		player.TakeCard(deck.getCard());
	}

	void DealerMove()
	{
		if (dealer.getCardsValues() <= 16)
		{
			dealer.TakeCard(deck.getCard());
			cout << endl << "Dealer took card" << endl;
		}
		else
		{
			cout << endl << "Dealer did not take card" << endl;
		}
	}

	void PlayerMove(char move)
	{
		if (move == 'y')
		{
			player.TakeCard(deck.getCard());
		}
		else
		{
			this->endGame();
		}
	}

	string endGame()
	{
		while (dealer.getCardsValues() <= 16)
		{
			DealerMove();
		}
		/*cout << "Dealer has: " << endl;

		for (int i = 0;i < dealer.dealerCards.size();i++)
		{
			cout << dealer.dealerCards.at(i).image << " " << dealer.dealerCards.at(i).suit << endl;
		}

		cout << endl << "Dealer result: " << dealer.getCardsValues() << endl;

		cout << "You have: " << endl;

		for (int i = 0;i < player.playerCards.size();i++)
		{
			cout << player.playerCards.at(i).image << " " << player.playerCards.at(i).suit << endl;
		}

		cout << endl << "Your result: " << player.getCardsValues() << endl;*/

		if (player.getCardsValues() > dealer.getCardsValues() && player.getCardsValues() <= 21 && dealer.getCardsValues() <= 21)
		{
			//cout << "You win" << endl;
			player.giveWinCash();
			return "player";
			//cout << "Your cash now: " << player.playerMoney;
		}
		else if (player.getCardsValues() < dealer.getCardsValues() && player.getCardsValues() <= 21 && dealer.getCardsValues() <= 21)
		{
			/*cout << "You lose" << endl;
			cout << "Your cash now: " << player.playerMoney;*/
			return "dealer";
		}
		else if (player.getCardsValues() > 21 && dealer.getCardsValues() <= 21)
		{
			/*cout << "You lose" << endl;
			cout << "Your cash now: " << player.playerMoney;*/
			return "dealer";
		}
		else if (player.getCardsValues() <= 21 && dealer.getCardsValues() > 21)
		{
			//cout << "You win" << endl;
			player.giveWinCash();
			return "player";
			//cout << "Your cash now: " << player.playerMoney;
		}
		else
		{
			//cout << "Draw" << endl;
			player.Draw();
			return "draw";
			//cout << "Your cash now: " << player.playerMoney;
		}
		//cout << endl << endl;
	}
};