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
	// ssssssssssssssssssssssss
	return -999;
}
SmartPlayer::~SmartPlayer()
{
}
