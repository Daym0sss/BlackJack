#pragma once
#include "Card.h"
#include<vector>
#include<time.h>

using namespace std;

class Deck
{
public:
	vector<Card> cards;

	void initializeDeck()
	{
		cards.clear();
		/*
			initialize 2-10 cards
		*/
		int suitIndex = 0;
		while (suitIndex != 4)
		{
			string suit = "";
			switch (suitIndex)
			{
				case 0:
				{
					suit = "spikes";
					break;
				}
				case 1:
				{
					suit = "diamonds";
					break;
				}
				case 2:
				{
					suit = "hearts";
					break;
				}
				case 3:
				{
					suit = "cross";
					break;
				}
			}
			for (int i = 0;i < 9;i++)
			{
				Card tmp;
				tmp.value = i + 2;
				tmp.image = std::to_string(i + 2);
				tmp.suit = suit;
				cards.push_back(tmp);
			}
			suitIndex++;
		}


		/*
			initalize J-K/
		*/

		suitIndex = 0;
		while (suitIndex != 4)
		{
			string suit = "";
			switch (suitIndex)
			{
				case 0:
				{
					suit = "spikes";
					break;
				}
				case 1:
				{
					suit = "diamonds";
					break;
				}
				case 2:
				{
					suit = "hearts";
					break;
				}
				case 3:
				{
					suit = "cross";
					break;
				}
			}
			
			Card tmp;
			tmp.value = 10;
			tmp.image = "J";
			tmp.suit = suit;
			cards.push_back(tmp);

			tmp.value = 10;
			tmp.image = "Q";
			tmp.suit = suit;
			cards.push_back(tmp);

			tmp.value = 10;
			tmp.image = "K";
			tmp.suit = suit;
			cards.push_back(tmp);

			suitIndex++;
		}

		/*
			initialize A
		*/
		suitIndex = 0;
		while (suitIndex != 4)
		{
			string suit = "";
			switch (suitIndex)
			{
				case 0:
				{
					suit = "spikes";
					break;
				}
				case 1:
				{
					suit = "diamonds";
					break;
				}
				case 2:
				{
					suit = "hearts";
					break;
				}
				case 3:
				{
					suit = "cross";
					break;
				}
			}
			Card tmp;
			tmp.value = 11;
			tmp.image = "A";
			tmp.suit = suit;
			cards.push_back(tmp);

			suitIndex++;
		}
	}

	Card getCard()
	{
		srand(time(NULL));
		int index = rand() % cards.size();
		Card result = cards.at(index);
		
		cards.erase(cards.begin() + index);

		return result;
	}
};