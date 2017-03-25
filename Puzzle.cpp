// Puzzle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"

#include "unittest.h"
#include "board.h"

#include <vector>

#include <time.h>

void DieProc(const char *sz, const char *szFile, int nLine)
{
	fprintf(stderr, "Died!  %s (%s, %d)\n", sz, szFile, nLine);
	exit(1);
}

std::vector<Board> *Recurse(const Board &board, std::vector<std::string> vecWords)
{
	std::vector<Board> *boards = new std::vector<Board>();

	if (vecWords.size() == 0)
	{
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

		while (dir < (int) Next)
		{
			Board boardT = board;
			if (boardT.FPlace(strWord, pos, (Direction)dir))
			{
				// we were able to place the word here.  try to build a board with this word
				// in this location
				std::vector<Board> *newBoards = Recurse(boardT, vecWords);
				AppendBoardsToBoards(boards, newBoards);
			}
			dir++;
		}
	} while (board.FMovePosition(Next, pos, pos));

	return boards;
}

#include <iostream>
#include <fstream>
#include <sstream>

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

void SwapBoardItems(std::vector<Board> *boards, int i1, int i2)
{
	Board t = boards->at(i2);
	boards->at(i2) = boards->at(i1);
	boards->at(i1) = t;
}

void PruneIllegalBoardsByIndex(std::vector<Board> *boards, const std::vector<std::string> &vecIllegal)
{
	int i = 0;
	int iMax = boards->size();

	while (i < iMax)
	{
		if (boards->at(i).HasWords(vecIllegal))
		{
			iMax--;
			SwapBoardItems(boards, i, iMax);
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


std::vector<Board> *PermuteWhitespace(std::vector<Board> *boards, const std::vector<std::string> &vecIllegal)
{
	// for every period, fill it in with the 26 possible characters
	std::vector<Board> *newBoards = new std::vector<Board>();
	std::vector<Board>::iterator itBoard = boards->begin(); // start at the end so we can delete if we want to 

	for(; itBoard != boards->end(); itBoard++)
	{
		Position pos = { 0,0 };
		AppendBoardsToBoards(newBoards, (*itBoard).PermuteWhitespace(pos, vecIllegal));
	}

	return newBoards;
}

void TestPruneBoard(int xMax, int yMax, const std::vector<std::string> &vecBoardInit, const std::vector<std::string> &vecIllegal, int cExpected, const char *szTest)
{
	std::vector<Board> *boards = new std::vector<Board>();

	for (std::vector<std::string>::const_iterator it = vecBoardInit.begin(); it != vecBoardInit.end(); it++)
	{
		Board b(xMax, yMax, *it);

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

bool compareBoards(const Board &left, const Board &right)
{
	return left < right; 
}

#include <algorithm>
void RemoveDuplicates(std::vector<Board> *boards)
{
	std::sort(boards->begin(), boards->end(), compareBoards);

	// now walk through and delete any dupes
	int i = 0;
	int iMax = boards->size();

	while (i < iMax - 1)
	{
		if (boards->at(i) == boards->at(i + 1))
		{
			iMax--;
			SwapBoardItems(boards, i, iMax);
		}
		else
		{
			i++;
		}
	}

	if (iMax < (int)boards->size())
		boards->erase(boards->begin() + iMax, boards->end());
}

bool compareStringsByLength(const std::string left, const std::string right)
{
	return left.length() < right.length();
}

int main(int argc, char * argv[])
{
	clock_t start = clock();

	RunTests();

	int xMax, yMax;
	std::vector<std::string> vecWords;
	std::vector<std::string> vecIllegal;

	ReadInFile(std::string(argv[1]), xMax, yMax, vecWords, vecIllegal);
	std::sort(vecWords.begin(), vecWords.end(), compareStringsByLength);

	// sort the words by length

	Board board(xMax, yMax);

	std::vector<Board> *boards = Recurse(board, vecWords);
	PruneIllegalBoardsByIndex(boards, vecIllegal);
	std::vector<Board> *fullBoards = PermuteWhitespace(boards, vecIllegal);
	delete boards;

	PruneIllegalBoardsByIndex(fullBoards, vecIllegal);
	RemoveDuplicates(fullBoards);

	std::vector<Board>::iterator it = fullBoards->begin();

	printf("%d solution(s)\n", fullBoards->size());
	for (; it != fullBoards->end(); it++)
	{
		printf("Board:\n");

		(*it).Print();
	}

	printf("elapsed: %f\n", (double) (clock() - start) / CLOCKS_PER_SEC);
}

