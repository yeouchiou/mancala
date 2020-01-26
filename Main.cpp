#include "Board.h"
#include "Player.h"
#include "Game.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

void doBoardTests()
{
	
	Board b(3, 2);
	assert(b.holes() == 3 && b.totalBeans() == 12 &&
		b.beans(SOUTH, POT) == 0 && b.beansInPlay(SOUTH) == 6);
	b.setBeans(SOUTH, 1, 1);
	b.moveToPot(SOUTH, 2, SOUTH);
	assert(b.totalBeans() == 11 && b.beans(SOUTH, 1) == 1 &&
		b.beans(SOUTH, 2) == 0 && b.beans(SOUTH, POT) == 2 &&
		b.beansInPlay(SOUTH) == 3);
	Side es;
	int eh;
	b.sow(SOUTH, 3, es, eh);
	assert(es == NORTH && eh == 3 && b.beans(SOUTH, 3) == 0 &&
		b.beans(NORTH, 3) == 3 && b.beans(SOUTH, POT) == 3 &&
		b.beansInPlay(SOUTH) == 1 && b.beansInPlay(NORTH) == 7);
	// check across end of vector
	b.sow(NORTH, 1, es, eh);
	assert(es == SOUTH && eh == 1 && b.beans(NORTH, POT) == 1 &&
		b.beans(NORTH, 1) == 0 && b.beans(SOUTH, 1) == 2);
	
	// test board looping
	/*
	Board b(4, 0);
	b.setBeans(NORTH, 4, 1);
	b.setBeans(SOUTH, 3, 7);
	Side es;
	int eh;
	b.sow(SOUTH, 3, es, eh);
	*/
}

void doPlayerTests()
{
	HumanPlayer hp("Marge");
	assert(hp.name() == "Marge"  &&  hp.isInteractive());
	BadPlayer bp("Homer");
	assert(bp.name() == "Homer" && !bp.isInteractive());
	//SmartPlayer sp("Lisa");
	//assert(sp.name() == "Lisa" && !sp.isInteractive());
	Board b(3, 2);
	b.setBeans(SOUTH, 2, 0);
	cout << "=========" << endl;
	int n = hp.chooseMove(b, SOUTH);
	cout << "=========" << endl;
	assert(n == 1 || n == 3);
	n = bp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
	//n = sp.chooseMove(b, SOUTH);
	//assert(n == 1 || n == 3);
}


void doGameTests()
{
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Board b(3, 0);
	b.setBeans(SOUTH, 1, 2);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 2);
	Game g(b, &bp1, &bp2);
	bool over;
	bool hasWinner;
	Side winner;
	g.display();
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
		g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

	g.move();
	g.display();
	//   0  1  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
	g.move();
	g.display();
	//   1  0  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move();
	g.display();
	//   1  0  0
	// 0         3
	//   0  0  1
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

	g.move();
	g.display();
	//   0  0  0
	// 1         4
	//   0  0  0
	g.status(over, hasWinner, winner);
	assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
	assert(hasWinner && winner == SOUTH);
}

int main()
{
	
	doBoardTests();
	//doPlayerTests();
	//doGameTests();
	cout << "Passed all tests" << endl;
	
	/*	
	HumanPlayer bp1("Marge");
	//SmartPlayer bp2("Homer");
	HumanPlayer bp2("Homer");
	Board b(4, 4);
	Game g(b, &bp1, &bp2);
	g.play();
	*/	
}

// Non member function implementations

bool isOver(const Board& b)
{
	if (b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0)
		return true;
	else
		return false;
}

int heuristic(const Board& b)
{
	return b.beans(SOUTH, POT) - b.beans(NORTH, POT);
}
