#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole) : m_nHoles(nHoles)
{
	for (int i = 0; i < nHoles; i++)
	{
		m_board.push_back(nInitialBeansPerHole);
	}
	m_board.push_back(0);
	for (int i = 0; i < nHoles; i++)	
	{
		m_board.push_back(nInitialBeansPerHole);
	}	
}

int Board::holes() const
{
	return -999;
}

int Board::beans(Side s, int hole) const
{
	return -999;
}
int Board::beansInPlay(Side s) const
{
	return -999;
}
int Board::totalBeans() const
{
	return -999;
}
bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
	return false;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
	return false;
}

bool Board::setBeans(Side s, int hole, int beans)
{
	return false;
}

// Private member functions

int Board::getSouthPot()
{
	return m_board[m_nHoles];	
}
int Board::getNorthPot()
{
	return m_board[2 * m_nHoles + 1];
}
int Board::getSoutHole(int n)
{
	if (n > 0 && n <= m_nHoles)
	{
		return m_board[2*m_nHoles + 1 - n];
	}
	else
		return -999;
}
int Board::getNorthHole(int n)
{
	if (n > 0 && n <= m_nHoles)
	{
		return m_board[n - 1];
	}
	else
		return -999;
}