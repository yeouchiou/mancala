#ifndef SIDE_H
#define SIDE_H

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;
const int MAX = 100000;
const int MIN = -100000;
const int MAX_DEPTH = 3;

inline
Side opponent(Side s)
{
	return Side(NSIDES - 1 - s);
}

bool isOver(const Board& b);

#endif