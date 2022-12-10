#pragma once
#include"Deck.h"
#include"Card.h"
#include<vector>

class Player
{
public:

	vector<Card> playerCards;
	int playerMoney;
	int bet;

	void TakeCard(Card card)
	{
		playerCards.push_back(card);
	}

	void MakeBet(int num)
	{
		bet = num;
		playerMoney = playerMoney - bet;
	}

	int getCardsValues()
	{
		int result = 0;
		for (int i = 0;i < playerCards.size();i++)
		{
			result += playerCards.at(i).value;
		}
		return result;
	}

	void giveWinCash()
	{
		playerMoney = playerMoney + bet * 2;
	}

	void Draw()
	{
		playerMoney += bet;
	}
};