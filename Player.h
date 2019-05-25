#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "Side.h"

class Board;
class Player
{
public:
	Player(std::string name);
	// Create a Player with the indicated name.
	std::string name() const;
	// Return the name of the player.
	virtual bool isInteractive() const { return false; }
	// Return false if the player is a computer player.
	// Return true if the player is human.Most kinds of players will be computer players.
	virtual int chooseMove(const Board& b, Side s) const = 0;
	// Every concrete class derived from this class must implement this function 
	// so that if the player were to be playing side s and had to make a move given board b,
	// the function returns the move the player would choose.
	// If no move is possible, return −1.
	virtual ~Player() {}
	// Since this class is designed as a base class, it should have a virtual destructor.
};

class HumanPlayer : public Player
{
public:
	virtual bool isInteractive() const { return true; }
	virtual int chooseMove(const Board& b, Side s) const;
	~HumanPlayer();
};

class BadPlayer : public Player
{
public:
	virtual int chooseMove(const Board& b, Side s) const;
	~BadPlayer();
};

class SmartPlayer : public Player
{
public:
	virtual int chooseMove(const Board& b, Side s) const;
	~SmartPlayer();

};



#endif