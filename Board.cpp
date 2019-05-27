#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole) : m_nHoles(nHoles) 
{
	if (m_nHoles <= 0)
		m_nHoles = 1;
	if (nInitialBeansPerHole < 0)
		nInitialBeansPerHole = 0;
	// set up m_board

	// set South's holes
	for (int i = 0; i < m_nHoles; i++)
		m_board.push_back(nInitialBeansPerHole);
	// set South's pot
	m_board.push_back(0);
	// set North's holes
	for (int i = 0; i < m_nHoles; i++)		
		m_board.push_back(nInitialBeansPerHole);	
	// set North's pot
	m_board.push_back(0);
}

int Board::beans(Side s, int hole) const
{
	if (isValidHole(hole))
		return m_board[getHoleIdx(s, hole)];
	else
		return -1;
}
int Board::beansInPlay(Side s) const
{
	int count = 0;
	if (s == SOUTH)
	{
		for (int i = 0; i < m_nHoles; i++)
			count += m_board[i];
	}

	else  // North
	{
		for (int i = m_nHoles + 1; i < 2 * m_nHoles + 1; i++)
			count += m_board[i];
	}
	return count;
}

int Board::totalBeans() const
{
	int count = 0;
	for (std::vector<int>::const_iterator p = m_board.begin(); p != m_board.end(); p++)
	{
		count += *p;
	}
	return count;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
	if (!isValidHole(hole) || hole == POT)
		return false;
	else
	{
		int numBeans = beans(s, hole);
		// if no beans in pot do nothing
		if (numBeans <= 0)
		{
			endSide = s;
			endHole = hole;
			return true;
		}
		int p = getHoleIdx(s, hole);
		// clear beans in the hole
		m_board[p] = 0;
		// sow
		while (numBeans > 0)
		{
			// move to next hole
			p++; 
			// reached end of board
			if (p == m_board.size()) 
				p = 0;
			// Skip opponent's pot
			if (p == getHoleIdx(opponent(s), POT))
				p++;
			m_board[p]++;
			numBeans--;	
		}
		// Figure out final hole in public numbering scheme.
		if (p == m_nHoles)
		{
			endSide = SOUTH;
			endHole = POT;
		}
		else if (0 <= p && p < m_nHoles)
		{
			endSide = SOUTH;
			endHole = p + 1;
		}
		else if (p == 2 * m_nHoles + 1)
		{
			endSide = NORTH;
			endHole = POT;
		}
		else
		{
			endSide = NORTH;
			endHole = 2 * m_nHoles + 1 - p;
		}
		return true;
	}
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
	if (!isValidHole(hole) || hole == POT)
		return false;
	else
	{
		m_board[getHoleIdx(potOwner, POT)] += m_board[getHoleIdx(s, hole)];
		m_board[getHoleIdx(s, hole)] = 0;
		return true;
	}
}

bool Board::setBeans(Side s, int hole, int beans)
{
	if (!isValidHole(hole) || beans < 0)
		return false;
	else
	{
		m_board[getHoleIdx(s, hole)] = beans;
		return true;
	}
}


// Privat member functions

bool Board::isValidHole(int hole) const
{
	// invalid hole
	if (hole > m_nHoles || hole < 0)
		return false;
	else
		return true;
}


int Board::getHoleIdx(Side s, int hole) const
{
	if (isValidHole(hole))
	{
		if (hole == POT)
		{
			if (s == SOUTH)
			{
				return m_nHoles;
			}
			else // North
			{
				return 2 * m_nHoles + 1;
			}
		}
		else
		{
			if (s == SOUTH)
			{
				return hole - 1;
			}
			else // North
			{
				return 2 * m_nHoles + 1 - hole;
			}
		}
	}
	else
		// invalid hole
		return -999;
}
