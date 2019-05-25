#include "Player.h"


// Player implementations
Player::Player(std::string name)
{

}

std::string Player::name() const
{
	return "";
}


// HumanPlayer implementations

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
	return -999;
}
HumanPlayer::~HumanPlayer()
{
}

// BadPlayer implementations

int BadPlayer::chooseMove(const Board& b, Side s) const
{
	return -999;
}
BadPlayer::~BadPlayer()
{
}

// SmartPlayer implementations

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
	return -999;
}
SmartPlayer::~SmartPlayer()
{
}
