#pragma once
#include"Deck.h"
#include"Card.h"
#include<vector>
#include<iostream>

class Dealer
{
public:

	vector<Card> dealerCards;

	void TakeCard(Card card)
	{
		dealerCards.push_back(card);
	}

	int getCardsValues()
	{
		int result = 0;
		for (int i = 0;i < dealerCards.size();i++)
		{
			result += dealerCards.at(i).value;
		}
		return result;
	}
};