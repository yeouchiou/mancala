#include "Game.h"
#include "Player.h"
#include <iostream>
#include <string>
using namespace std;
Game::Game(const Board& b, Player* south, Player* north)
	: m_board(b), m_south(south), m_north(north), m_turn(SOUTH)
{}


void Game::display() const
{
	cout << string(2 * m_board.holes(), ' ');
	cout << m_north->name() << endl;
	cout << "    ";
	for (int i = 0; i < m_board.holes(); i++)
	{
		cout << m_board.beans(NORTH, i + 1) << "  ";
	}
	cout << endl;
	cout << " " << m_board.beans(NORTH, POT);
	cout << string(3 * m_board.holes(), ' ');
	cout << " " << m_board.beans(SOUTH, POT) << endl;

	cout << "    ";
	for (int i = 0; i < m_board.holes(); i++)
	{
		cout << m_board.beans(SOUTH, i + 1) << "  ";
	}
	cout << endl;
	cout << string(2 * m_board.holes(), ' ');
	cout << m_south->name() << endl;

}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
	if (isOver(m_board))
	{
		over = true;
		if (beans(NORTH, POT) == beans(SOUTH, POT))
		{
			// there's a tie
			hasWinner = false;
		}
		else
		{
			hasWinner = true;
		}
		if (hasWinner)
		{
			winner = beans(NORTH, POT) > beans(SOUTH, POT) ? NORTH : SOUTH;
		}
	}
	else
	{
		over = false;
	}
}

bool Game::move()
{
	if (isOver(m_board))
		return false;
	Side endSide;
	int endHole;
	int playHole = currentPlayer(m_turn)->chooseMove(m_board, m_turn);

	if (!currentPlayer(m_turn)->isInteractive())
		cout << currentPlayer(m_turn)->name() << " chooses hole " << playHole << endl;
	m_board.sow(m_turn, playHole, endSide, endHole);
	if (endSide == m_turn && endHole == POT && !isOver(m_board))
	{
		// ended up in pot so take another turn
		display();
		cout << currentPlayer(m_turn)->name() << " gets another turn." << endl;
		return move();
	}
	else if (endSide == m_turn && m_board.beans(m_turn, endHole) == 1)
	{
		// capture
		if (m_board.beans(opponent(m_turn), endHole) > 0)
		{
			m_board.moveToPot(m_turn, endHole, m_turn);
			m_board.moveToPot(opponent(m_turn), endHole, m_turn);
		}
	}
	m_turn = opponent(m_turn);
	if (isOver(m_board))
	{
		display();
		if (m_board.beansInPlay(NORTH) == 0)
		{
			cout << "Sweeping reamining beans into " << m_south->name() << "'s pot." << endl;
			for (int i = 1; i <= m_board.holes(); i++)
			{
				m_board.moveToPot(SOUTH, i, SOUTH);
			}
		}
		else if (m_board.beansInPlay(SOUTH) == 0)
		{
			cout << "Sweeping reamining beans into " << m_north->name() << "'s pot." << endl;
			for (int i = 1; i <= m_board.holes(); i++)
			{
				m_board.moveToPot(NORTH, i, NORTH);
			}
		}
		return false;
	}
	else
		return true;

}

void Game::play()
{
	// Game can't start
	display();
	if (m_board.beansInPlay(SOUTH) == 0)
	{
		for (int i = 1; i <= m_board.holes(); i++)
		{
			m_board.moveToPot(NORTH, i, NORTH);
		}
		cout << "The winner is " << m_north->name() << "." << endl;
	}


	bool over = false;
	bool hasWinner;
	Side winner;
	while (!over)
	{
		move();
		display();
		status(over, hasWinner, winner);
		if (!m_north->isInteractive() && !m_south->isInteractive())
		{
			cout << "Press ENTER to continue";
			cin.ignore(100000, '\n');
		}
	}
	if (hasWinner) 
	{
		cout << "The winner is " << currentPlayer(winner)->name() << endl;
	}
	else
	{
		cout << "There is a tie! "  << endl;
	}

}



// Private member functions


Player* Game::currentPlayer(Side s)
{
	if (s == NORTH)
		return m_north;
	else
		return m_south;
}
