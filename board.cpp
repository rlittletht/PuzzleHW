
#include "board.h"
#include "unittest.h"
#include "string.h"
#include "stdafx.h"
#include <string>

/*----------------------------------------------------------------------------
	%%Function: AppendBoardsToBoards
	%%Qualified: AppendBoardsToBoards
	%%Contact: rlittle
	
	transfers ownership of all the boards in boardsToAppend into boards
----------------------------------------------------------------------------*/
void AppendBoardsToBoards(std::vector<Board *> *boards, const std::vector<Board *> *boardsToAppend)
{
	if (boardsToAppend != NULL && boardsToAppend->size() > 0)
	{
		// append the boards we just got back into the list we will return
		std::vector<Board *>::const_iterator it = boardsToAppend->begin();

		for (; it != boardsToAppend->end(); it++)
		{
			boards->push_back(*it);
		}
	}
}

/*----------------------------------------------------------------------------
	%%Function: Board
	%%Qualified: Board::Board
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
Board::Board(int x, int y)
{
	m_xMax = x;
	m_yMax = y;

	m_rgchBoard = new char[x * y];
	for (int i = 0; i < x * y; i++)
		m_rgchBoard[i] = '.';
}

/*----------------------------------------------------------------------------
	%%Function: Board
	%%Qualified: Board::Board
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
Board::Board(int x, int y, const std::string &init)
{
	m_xMax = x;
	m_yMax = y;

	if (x * y != init.length())
		Die("bogus init string passed to board::board");

	m_rgchBoard = new char[x * y];
	memcpy(m_rgchBoard, init.c_str(), x * y);
}

/*----------------------------------------------------------------------------
	%%Function: Board
	%%Qualified: Board::Board
	%%Contact: rlittle

----------------------------------------------------------------------------*/
Board::Board(const Board &rhs)
{
	m_xMax = rhs.m_xMax;
	m_yMax = rhs.m_yMax;
	m_rgchBoard = new char[rhs.m_xMax * rhs.m_yMax];
	memcpy(m_rgchBoard, rhs.m_rgchBoard, rhs.m_xMax * rhs.m_yMax);
}

/*----------------------------------------------------------------------------
	%%Function: FLegalPosition
	%%Qualified: Board::FLegalPosition
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::FLegalPosition(const Position &pos) const
{
	if (pos.x < 0 || pos.y < 0)
		return false;

	if (pos.x >= m_xMax || pos.y >= m_yMax)
		return false;

	return true;
}

/*----------------------------------------------------------------------------
	%%Function: Print
	%%Qualified: Board::Print
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
void Board::Print(void) const
{
	int i;
	char *rgch = new char(m_xMax + 1);

	rgch[m_xMax] = 0;	// only have to set this once since we will always be moving the board rows behind this location
	for (i = 0; i < m_yMax; i++)
	{
		Position pos = { 0, i };
		int offset = PosToOffset(pos);

		memcpy(rgch, &m_rgchBoard[offset], m_xMax);
		printf("  %s\n", rgch);
	}
}

/*----------------------------------------------------------------------------
	%%Function: FMovePositionMult
	%%Qualified: Board::FMovePositionMult
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::FMovePositionMult(Direction dir, const Position &posIn, Position &posOutReal, int nLength) const
{
	Position posOut;

	// since they might try to pass in posIn and posOutReal as the same reference, we need to be careful
	// to not change posOutReal until we are all done... put everything in posOut until we are done

	posOut = posIn;

	if (dir == Next)
	{
		int offset = PosToOffset(posIn);
		offset += nLength;
		OffsetToPos(offset, posOut);
	}
	else
	{
		switch (dir)
		{
		case UpRight:
		case Up:
		case UpLeft:
			posOut.y -= nLength;
			break;
		case DownRight:
		case Down:
		case DownLeft:
			posOut.y += nLength;
			break;
		}

		switch (dir)
		{
		case UpRight:
		case DownRight:
		case Right:
			posOut.x += nLength;
			break;
		case UpLeft:
		case DownLeft:
		case Left:
			posOut.x -= nLength;
			break;
		}
	}

	if (FLegalPosition(posOut))
	{
		posOutReal = posOut;
		return true;
	}
	else
	{
		posOutReal = posIn;
		return false;
	}
}

/*----------------------------------------------------------------------------
	%%Function: FMovePosition
	%%Qualified: Board::FMovePosition
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::FMovePosition(Direction dir, const Position &posIn, Position &posOut) const
{
	return FMovePositionMult(dir, posIn, posOut, 1);
}

/*----------------------------------------------------------------------------
	%%Function: PosToOffset
	%%Qualified: Board::PosToOffset
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
int Board::PosToOffset(const Position &pos) const
{
	return pos.y * m_xMax + pos.x;
}

/*----------------------------------------------------------------------------
	%%Function: OffsetToPos
	%%Qualified: Board::OffsetToPos
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
void Board::OffsetToPos(int offset, Position &posOut) const
{
	posOut.y = offset / m_xMax;
	posOut.x = offset % m_xMax;
}

/*----------------------------------------------------------------------------
	%%Function: FMatchChar
	%%Qualified: Board::FMatchChar
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::FMatchChar(char ch, const Position &pos) const
{
	if (!FLegalPosition(pos))
		return false;

	if (m_rgchBoard[pos.y * m_xMax + pos.x] != ch)
		return false;

	return true;
}

/*----------------------------------------------------------------------------
	%%Function: FMatch
	%%Qualified: Board::FMatch
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::FMatch(const std::string &strWord, const Position &pos, Direction dir) const
{
	// see if the word fits
	Position posOut;
	int nLength = strWord.length();

	if (!FMovePositionMult(dir, pos, posOut, nLength - 1))
		return false;

	posOut = pos;

	// ok, it fits.  see if it matches
	for (int i = 0; i < nLength; i++)
	{
		if (!FMatchChar(strWord[i], posOut))
			return false;

		if (i + 1 < nLength && !FMovePosition(dir, posOut, posOut))
			Die("internal error");
	}

	return true;
}

/*----------------------------------------------------------------------------
	%%Function: FSchrodingersChar
	%%Qualified: Board::FSchrodingersChar
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::FSchrodingersChar(const Position &pos) const
{
	return FMatchChar('.', pos);
}

/*----------------------------------------------------------------------------
	%%Function: PermuteWhitespace
	%%Qualified: Board::PermuteWhitespace
	%%Contact: rlittle

	for every ., create a new board for every character permutation
	from the given position, find the next '.' and permute/recurse
----------------------------------------------------------------------------*/
std::vector<Board *> *Board::PermuteWhitespace(const Position &pos, const std::vector<std::string> &vecIllegal)
{
	std::vector<Board *> *boards = NULL; 
	Position posCur = pos;

	// find the next '.'
	while (FLegalPosition(posCur) && !FSchrodingersChar(posCur))
	{
		if (!FMovePosition(Next, posCur, posCur))
		{
			posCur = { -1,-1 };
			break;
		}
	}

	// if we're out of legal positions, the board is done
	if (!FLegalPosition(posCur))
	{
		boards = new std::vector<Board *>();
		boards->push_back(this);
		return boards;
	}

	char rgchPossible[] = "abcdefghijklmnopqrstuvwxyz";

	Position posNext;

	if (!FMovePosition(Next, posCur, posNext))
	{
		// force posNext to be illegal so we bail out in the recursive call below
		posNext = { -1, -1 };
	}

	for (unsigned int i = 0; i < strlen(rgchPossible); i++)
	{
		Board *pBoardNew = NewBoard(this); // copy this board and start placing chars

		if (pBoardNew->FPlaceChar(rgchPossible[i], posCur) && !pBoardNew->HasWords(vecIllegal))
		{
			if (boards == NULL)
				boards = new std::vector<Board *>();

			AppendBoardsToBoards(boards, pBoardNew->PermuteWhitespace(posNext, vecIllegal));
		}
		else
		{
			FreeBoard(pBoardNew);
			pBoardNew = NULL;
		}
	}

	return boards;
}

/*----------------------------------------------------------------------------
	%%Function: FPlaceChar
	%%Qualified: Board::FPlaceChar
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::FPlaceChar(char ch, const Position &pos)
{
	if (!FLegalPosition(pos))
		return false;

	if (m_rgchBoard[pos.y * m_xMax + pos.x] != '.' && m_rgchBoard[pos.y * m_xMax + pos.x] != ch)
		return false;

	m_rgchBoard[pos.y * m_xMax + pos.x] = ch;
	return true;
}

/*----------------------------------------------------------------------------
	%%Function: FPlace
	%%Qualified: Board::FPlace
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::FPlace(const std::string &strWord, const Position &pos, Direction dir)
{
	// see if the word fits
	Position posOut;
	int nLength = strWord.length();

	if (!FMovePositionMult(dir, pos, posOut, nLength - 1))
		return false;

	posOut = pos;

	// ok, it fits.  fill it in
	for (int i = 0; i < nLength; i++)
	{
		if (!FPlaceChar(strWord[i], posOut))
			return false;

		if (i + 1 < nLength && !FMovePosition(dir, posOut, posOut))
			Die("internal error");
	}

	return true;
}


/*----------------------------------------------------------------------------
	%%Function: HasWord
	%%Qualified: Board::HasWord
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::HasWord(const std::string &word) const
{
	Position pos = { 0, 0 };

	do // for each position
	{
		// don't bother to check if the first char doesn't match!
		if (!FMatchChar(word[0], pos))
			continue;

		int dir = 0;

		while (dir < (int) Next)
		{
			if (FMatch(word, pos, (Direction) dir))
				return true;

			dir++;
		}
	} while (FMovePosition(Next, pos, pos));

	return false;
}

/*----------------------------------------------------------------------------
	%%Function: HasWords
	%%Qualified: Board::HasWords
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::HasWords(const std::vector<std::string> &words) const
{
	std::vector<std::string>::const_iterator itIllegal = words.begin();

	while (itIllegal != words.end())
	{
		if (HasWord(*itIllegal))
			return true;
		itIllegal++;
	}

	return false;
}

/*----------------------------------------------------------------------------
	%%Function: operator=
	%%Qualified: Board::operator=
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
Board &Board::operator=(const Board &rhs)
{
	m_xMax = rhs.m_xMax;
	m_yMax = rhs.m_yMax;
	m_rgchBoard = new char[rhs.m_xMax * rhs.m_yMax];
	memcpy(m_rgchBoard, rhs.m_rgchBoard, rhs.m_xMax * rhs.m_yMax);
	return *this;
}

/*----------------------------------------------------------------------------
	%%Function: operator<
	%%Qualified: Board::operator<
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::operator<(const Board &right) const
{
	return strncmp(m_rgchBoard, right.m_rgchBoard, m_xMax * m_yMax) < 0;
}

/*----------------------------------------------------------------------------
	%%Function: operator>
	%%Qualified: Board::operator>
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::operator>(const Board &right) const
{
	return strncmp(m_rgchBoard, right.m_rgchBoard, m_xMax * m_yMax) > 0;
}

/*----------------------------------------------------------------------------
	%%Function: operator==
	%%Qualified: Board::operator==
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool Board::operator==(const Board &right) const
{
	return strncmp(m_rgchBoard, right.m_rgchBoard, m_xMax * m_yMax) == 0;
}

/*----------------------------------------------------------------------------
	%%Function: ~Board
	%%Qualified: Board::~Board
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
Board::~Board()
{
	if (m_rgchBoard)
		delete[] m_rgchBoard;
	m_rgchBoard = NULL;
}

/*----------------------------------------------------------------------------
	%%Function: SetBoard
	%%Qualified: Board::SetBoard
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
void Board::SetBoard(const char *rgchBoard)
{
	memcpy(m_rgchBoard, rgchBoard, m_xMax * m_yMax);

}

const char *Board::RawBoard(void)
{
	return m_rgchBoard;
}

#define OBJECTPOOL	1

#if OBJECTPOOL
std::vector<Board *> boardsPool;

#if ASYNC
#include <mutex>
std::mutex mutexObjectPool;
#endif // ASYNC

#endif

#include <inttypes.h>
int64_t cBoardsAllocated = 0;
int64_t cBoardsReused = 0;

void FreeBoard(Board *pBoard)
{
#if OBJECTPOOL
#if ASYNC
	std::lock_guard<std::mutex> guard(mutexObjectPool);
#endif
	boardsPool.push_back(pBoard);
#else
	delete(pBoard);
#endif
}

Board *NewBoard(Board *pBoardSource)
{
#if OBJECTPOOL
#if ASYNC
	std::lock_guard<std::mutex> guard(mutexObjectPool);
#endif
#endif
	Board *pBoard;

#if OBJECTPOOL
	if (boardsPool.size() > 0)
	{
		pBoard = boardsPool.back();
		boardsPool.pop_back();
		pBoard->SetBoard(pBoardSource->RawBoard());
		cBoardsReused++;
	}
	else
#endif
	{
		pBoard = new Board(*pBoardSource);
		cBoardsAllocated++;
		if (cBoardsAllocated % 1000000 == 0)
			fprintf(stderr, "Allocated %" PRId64 " boards, Reused %" PRId64 " boards, Pool= %d boards\n", cBoardsAllocated, cBoardsReused, boardsPool.size());
	}
	return pBoard;
}
