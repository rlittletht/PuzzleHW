// Puzzle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"

#include "unittest.h"
#include "board.h"

#include <vector>

#include <time.h>

/*----------------------------------------------------------------------------
	%%Function: DieProc
	%%Qualified: DieProc
	%%Contact: rlittle
	
	die with a message (catastrophic failure)
----------------------------------------------------------------------------*/
void DieProc(const char *sz, const char *szFile, int nLine)
{
	fprintf(stderr, "Died!  %s (%s, %d)\n", sz, szFile, nLine);
	exit(1);
}

/*----------------------------------------------------------------------------
	%%Function: Recurse
	%%Qualified: Recurse
	%%Contact: rlittle
	
	this is the meat of the matter...
----------------------------------------------------------------------------*/
std::vector<Board *> *Recurse(Board *board, std::vector<std::string> vecWords)
{
	std::vector<Board *> *boards = NULL;

	if (vecWords.size() == 0)
	{
		if (boards == NULL)
			boards = new std::vector<Board *>();

		boards->push_back(board);
		return boards;
	}

	// place all the words on the given board
	// return a vector of possible completed boards

	std::string strWord = vecWords.back();

	// try to place szWord at every possible position/every possible rotation
	vecWords.pop_back();
	Position pos = { 0, 0 };

	do
	{
		// try every placement direction at this location
		int dir = 0;

		if (board->FMatchChar('.', pos) || board->FMatchChar(strWord[0], pos))
		{
			while (dir < (int) Next)
			{
				Board *boardT = NewBoard(board);
				if (boardT->FPlace(strWord, pos, (Direction) dir))
				{
					// we were able to place the word here.  try to build a board with this word
					// in this location
					std::vector<Board *> *newBoards = Recurse(boardT, vecWords);
					if (boards == NULL)
						boards = new std::vector<Board *>();
					AppendBoardsToBoards(boards, newBoards);
				}
				else
				{
					FreeBoard(boardT);
					boardT = NULL;
				}
				dir++;
			}
		}
	} while (board->FMovePosition(Next, pos, pos));

	return boards;
}

#include <iostream>
#include <fstream>
#include <sstream>

/*----------------------------------------------------------------------------
	%%Function: trim
	%%Qualified: trim
	%%Contact: rlittle
	
	trim off the whitespace from the string
----------------------------------------------------------------------------*/
std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

/*----------------------------------------------------------------------------
	%%Function: ReadInFile
	%%Qualified: ReadInFile
	%%Contact: rlittle
	
	read the input file
----------------------------------------------------------------------------*/
void ReadInFile(std::string strFilename, int &xMax, int &yMax, std::vector<std::string> &vecWords, std::vector<std::string> &vecIllegal)
{
	std::ifstream inFile;

	inFile.open(strFilename);

	// get the dimensions
	std::string line;
	getline(inFile, line);

	std::stringstream ss(line);
	std::string token;
	ss >> token;
	// first token in the first line is the xMax
	xMax = std::stoi(token);

	ss >> token;
	yMax = std::stoi(token);

	// and now, read the allowed and illegal words

	while (getline(inFile, line))
	{
		if (line.length() < 2)
			continue;

		int ich = 1;
		if (line[1] == ' ')
			ich++;

		if (line[0] == '+')
			vecWords.push_back(trim(std::string(&line[ich])));
		if (line[0] == '-')
			vecIllegal.push_back(trim(std::string(&line[ich])));
	}

	inFile.close();
}

/*----------------------------------------------------------------------------
	%%Function: SwapBoardItems
	%%Qualified: SwapBoardItems
	%%Contact: rlittle
	
	swap the two board items
----------------------------------------------------------------------------*/
void SwapBoardItems(std::vector<Board *> *boards, int i1, int i2)
{
	Board *t = boards->at(i2);
	boards->at(i2) = boards->at(i1);
	boards->at(i1) = t;
}

/*----------------------------------------------------------------------------
	%%Function: PruneIllegalBoardsByIndex
	%%Qualified: PruneIllegalBoardsByIndex
	%%Contact: rlittle
	
	prune all the illegal boards from the list of boards. operates on
	boards by side affect
----------------------------------------------------------------------------*/
void PruneIllegalBoardsByIndex(std::vector<Board *> *boards, const std::vector<std::string> &vecIllegal)
{
	int i = 0;
	int iMax = boards->size();

	while (i < iMax)
	{
		if (boards->at(i)->HasWords(vecIllegal))
		{
			iMax--;
			SwapBoardItems(boards, i, iMax);
			FreeBoard(boards->at(iMax));
			boards->at(iMax) = NULL;
		}
		else
		{
			i++;
		}
	}
	if (iMax != boards->size())
	{
		boards->erase(boards->begin() + iMax, boards->end());
	}
}


/*----------------------------------------------------------------------------
	%%Function: PermuteWhitespace
	%%Qualified: PermuteWhitespace
	%%Contact: rlittle
	
	permute all the whitespace (actually, ".") into all the possible
	characters. the board's permute method will take carre of pruning 
	illegal boards
----------------------------------------------------------------------------*/
std::vector<Board *> *PermuteWhitespace(std::vector<Board *> *boards, const std::vector<std::string> &vecIllegal)
{
	// for every period, fill it in with the 26 possible characters
	std::vector<Board *> *newBoards = new std::vector<Board *>();
	std::vector<Board *>::iterator itBoard = boards->begin(); // start at the end so we can delete if we want to 

	for(; itBoard != boards->end(); itBoard++)
	{
		Position pos = { 0,0 };
		AppendBoardsToBoards(newBoards, (*itBoard)->PermuteWhitespace(pos, vecIllegal));
	}

	return newBoards;
}

void TestPruneBoard(int xMax, int yMax, const std::vector<std::string> &vecBoardInit, const std::vector<std::string> &vecIllegal, int cExpected, const char *szTest)
{
	std::vector<Board *> *boards = new std::vector<Board *>();

	for (std::vector<std::string>::const_iterator it = vecBoardInit.begin(); it != vecBoardInit.end(); it++)
	{
		Board *b = new Board(xMax, yMax, *it);

		boards->push_back(b);
	}

	PruneIllegalBoardsByIndex(boards, vecIllegal);
	TUTAssertAreEqual(cExpected, (int)boards->size(), "TestPruneBoard", szTest, __FILE__, __LINE__);
	delete boards;
}

void TestCore()
{
	Board::Test();
	TestPruneBoard(3, 2, std::vector<std::string>({ "aaadef", "aaabbb" }), std::vector<std::string>({ "def" }), 1, "2 boards remove first board");
	TestPruneBoard(3, 2, std::vector<std::string>({ "aaabbb", "aaadef", "cccddd" }), std::vector<std::string>({ "def" }), 2, "3 boards remove middle board");
	TestPruneBoard(3, 2, std::vector<std::string>({ "aaabbb", "aaadef", "cccddd" }), std::vector<std::string>({ "ddd" }), 2, "3 boards remove last board");
	TestPruneBoard(7, 5, std::vector<std::string>({ "tengamravocadoolaffubsoahcuitradecn", "tradecnchaosuiolaffubavocadotengamr", "tradecnsoahcuiolaffubavocadomagnetr", "tradecnsoahcuiolaffubavocadomagnetr", "tradecnsoahcuiolaffubavocadotengamr" }), std::vector<std::string>({ "ace", "coat" }), 2, "4 boards remove two in middle");
}

/*----------------------------------------------------------------------------
	%%Function: isBoardLessThan
	%%Qualified: isBoardLessThan
	%%Contact: rlittle
	
	compare two boards to see of left is less than right
----------------------------------------------------------------------------*/
bool isBoardLessThan(const Board *left, const Board *right)
{
	return *left < *right; 
}

#include <algorithm>
/*----------------------------------------------------------------------------
	%%Function: RemoveDuplicates
	%%Qualified: RemoveDuplicates
	%%Contact: rlittle
	
	remove duplicate boards (sort then remove)
----------------------------------------------------------------------------*/
void RemoveDuplicates(std::vector<Board *> *boards)
{
	std::sort(boards->begin(), boards->end(), isBoardLessThan);

	// now walk through and delete any dupes
	int i = 0;
	int iMax = boards->size();

	while (i < iMax - 1)
	{
		if (*(boards->at(i)) == *(boards->at(i + 1)))
		{
			iMax--;
			SwapBoardItems(boards, i, iMax);
			FreeBoard(boards->at(iMax));
			boards->at(iMax) = NULL;
		}
		else
		{
			i++;
		}
	}

	// we already freed the boards above, but we still have to remove their entries from the vector
	if (iMax < (int)boards->size())
		boards->erase(boards->begin() + iMax, boards->end());
}

/*----------------------------------------------------------------------------
	%%Function: isStringShorterThan
	%%Qualified: isStringShorterThan
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
bool isStringShorterThan(const std::string left, const std::string right)
{
	return left.length() < right.length();
}

/*----------------------------------------------------------------------------
	%%Function: main
	%%Qualified: main
	%%Contact: rlittle
	
----------------------------------------------------------------------------*/
int main(int argc, char * argv[])
{
	clock_t start = clock();

	// RunTests();

	int xMax, yMax;
	std::vector<std::string> vecWords;
	std::vector<std::string> vecIllegal;

	ReadInFile(std::string(argv[1]), xMax, yMax, vecWords, vecIllegal);
	std::sort(vecWords.begin(), vecWords.end(), isStringShorterThan);

	// sort the words by length

	Board *pBoard = new Board(xMax, yMax);

	std::vector<Board *> *boards = Recurse(pBoard, vecWords);
	PruneIllegalBoardsByIndex(boards, vecIllegal);
	std::vector<Board *> *fullBoards = PermuteWhitespace(boards, vecIllegal);
	delete boards;

	PruneIllegalBoardsByIndex(fullBoards, vecIllegal);
	RemoveDuplicates(fullBoards);

	std::vector<Board *>::iterator it = fullBoards->begin();

	printf("%d solution(s)\n", fullBoards->size());
	for (; it != fullBoards->end(); it++)
	{
		printf("Board:\n");

		(*it)->Print();
	}

	printf("elapsed: %f\n", (double) (clock() - start) / CLOCKS_PER_SEC);
}

