#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <limits>
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
	int totalTime = 4900;
	AlarmClock ac(totalTime);
	int bestHole;
	int value;
	Board copy(b);
	if (s == SOUTH)
		value = -10000;
	else
		value = 10000;

	minimax(copy, s, bestHole, value, ac, 0);
	// if minimax failed
	if (bestHole < 0 || bestHole > b.holes())
	{
		//cout << "minimax failed" << endl;
		for (int i = 1; i <= b.holes(); i++)
		{
			if (b.beans(s, i) > 0)
				return i;
		}
	}
	return bestHole;
}

void SmartPlayer::minimax(Board& b, Side s, int& bestHole, int& value, const AlarmClock& ac, int depth) const
{
	int endHole;
	int v2;
	int h2 = -1;
	Side endSide;

	// If we reach max depth or time out, return
	if (depth > MAX_DEPTH || ac.timedOut())
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
	int numHolesCanPlay = b.holes();
	for (int i = 1; i <= b.holes(); i++)
	{
		if (b.beans(s, i) == 0)
			numHolesCanPlay--;
	}
	for (int i = 1; i <= b.holes(); i++)
	{	
		bool flipped = false;
		// no beans to sow
		if (b.beans(s, i) == 0)
			continue;
	
		b.sow(s, i, endSide, endHole);
		// landed in our pot
		if (endSide == s && endHole == POT)
		{
			// switch opponent to the current side which will be overwritten in subsequent call
			s = opponent(s);
			flipped = true;
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
		Board copy(b);
		minimax(copy, opponent(s), h2, v2, ac, depth + 1);
		if (flipped)
			s = opponent(s);
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
		
	}
	return;
}

