#ifndef SIDE_H
#define SIDE_H
class Board;
enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;
const int MAX_DEPTH = 3;

inline
Side opponent(Side s)
{
	return Side(NSIDES - 1 - s);
}

bool isOver(const Board& b);
int heuristic(const Board& b);
#endif