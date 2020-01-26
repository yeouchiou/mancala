#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
using namespace std;

// Player implementations
Player::Player(std::string name) : m_name(name)
{}


// HumanPlayer implementations

HumanPlayer::HumanPlayer(std::string name) : Player(name)
{}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
	if (b.beansInPlay(s) == 0)
		return -1;
	int choice;
	do
	{
		cout << "Select a hole, " << name() << ": ";
		cin >> choice;
	} while (choice > b.holes() || choice <= 0 || b.beans(s, choice) == 0); 
	// make sure choice is legal and there are beans in that hole.
	return choice;
}

// BadPlayer implementations

BadPlayer::BadPlayer(std::string name) : Player(name)
{}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
	// Choose first hole with beans
	for (int i = 1; i <= b.holes(); i++)
	{
		if (b.beans(s, i) > 0)
			return i;
	}
	// no valid moves
	return -1;
}


// SmartPlayer implementations

SmartPlayer::SmartPlayer(std::string name) : Player(name)
{}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
	AlarmClock ac(4900);
	int bestHole;
	int value;
	Board copy(b);
	if (s == SOUTH)
		value = -10000;
	else
		value = 10000;
	minimax(copy, s, bestHole, value, ac);
	// if minimax failed
	if (bestHole < 0)
	{
		for (int i = 1; i <= b.holes(); i++)
		{
			if (b.beans(s, i) > 0)
				return i;
		}
	}
	return bestHole;
}

void SmartPlayer::minimax(Board& b, Side s, int& bestHole, int& value, AlarmClock& ac) const
{
	int endHole;
	int v2;
	int h2 = -1;
	Side endSide;

	if (ac.timedOut())
	{ 
		bestHole = -1;
		value = heuristic(b);
		return;
	}

	if (isOver(b))
	{
		bestHole = -1;
		if (b.beansInPlay(s) == 0)
		{
			for (int i = 1; i <= b.holes(); i++)
				b.moveToPot(opponent(s), i, opponent(s));
		}
		else if (b.beansInPlay(opponent(s)) == 0)
		{
			for (int i = 1; i <= b.holes(); i++)
				b.moveToPot(s, i, s);
		}
		if (heuristic(b) > 0) // South won
		{
			if (s == SOUTH)
				value = 9999;
			else
				value = -9999;
		}
		else if (heuristic(b) < 0) // North won
		{
			if (s == SOUTH)
				value = -9999;
			else
				value = 9999;
		}
		else
			value = 0; // Tied
		return;
	}
	
	for (int i = 1; i <= b.holes(); i++)
	{
		// no beans to sow
		if (b.beans(s, i) == 0)
			continue;
	
		b.sow(s, i, endSide, endHole);
		// landed in our pot
		if (endSide == s && endHole == POT)
		{
			// switch opponent to the current side which will be overwritten in subsequent call
			s = opponent(s);
		}
		// capture
		else if (endSide == s && b.beans(s, endHole) == 1)
		{
			if (b.beans(opponent(s), endHole) > 0)
			{
				b.moveToPot(s, endHole, s);
				b.moveToPot(opponent(s), endHole, s);
			}
		}

		minimax(b, opponent(s), h2, v2, ac);
		if (s == SOUTH)
		{
			if (v2 > value)
			{
				bestHole = i;
				value = v2;
			}
		}
		else
		{
			if (v2 < value)
			{
				bestHole = i;
				value = v2;
			}
		}
		return;
	}
	return;
}

