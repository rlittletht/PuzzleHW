#pragma once
#include <string>
#include <vector>

struct Position
{
	int x;
	int y;
};

enum Direction
{
	Up = 0,
	UpRight = 1,
	Right = 2,
	DownRight = 3,
	Down = 4,
	DownLeft = 5,
	Left = 6,
	UpLeft = 7,
	Next = 8
};

class Board
{
private:
	int m_xMax;
	int m_yMax;
	char *m_rgchBoard;

	bool FPlaceChar(char ch, const Position &pos);

	// UNIT TESTS
	void TestDirection(int xMax, int yMax, int x, int y, Direction dir, bool fExpected, int xExpected, int yExpected, const char *szCase);
	void TestDirectionMult(int xMax, int yMax, int x, int y, int nLength, Direction dir, bool fExpected, int xExpected, int yExpected, const char *szCase);
	int PosToOffset(const Position &pos) const;
	void OffsetToPos(int offset, Position &posOut) const;

	static void TestPlace(int xMax, int yMax, int x, int y, const char *szInitial, const char *szWord, Direction dir, bool fExpected, const char *szExpected, const char *szCase);
	static void TestPlaces(void);
	static void TestPosition(void);
	static void TestInit(void);
	static void TestPlaceChar(void);
	static void TestPlace(void);
	static void TestAssign(void);
	static void TestPosToOffset(int xMax, int yMax, int x, int y, int offsetExpected, const char *szTest);
	static void TestOffsetToPos(int xMax, int yMax, int offset, int xExpected, int yExpected, const char *szTest);
	static void TestOffsets(void);
	static void TestBoardLessThan(int xMax, int yMax, char *rgchLeft, char *rgchRight, bool fExpected, const char *szTest);
	static void TestBoardGreaterThan(int xMax, int yMax, char *rgchLeft, char *rgchRight, bool fExpected, const char *szTest);
	static void TestBoardEqualTo(int xMax, int yMax, char *rgchLeft, char *rgchRight, bool fExpected, const char *szTest);
	static void TestBoardComparators(void);

public:
	~Board();

	Board(const Board &b);
	Board(int x, int y, const std::string &init);
	Board(int x, int y);
	Board() {	};

	Board &operator= (const Board &rhs);
	bool operator<  (const Board &right) const;
	bool operator>  (const Board &right) const;
	bool operator==  (const Board &right) const;

	static void Test(void);

	bool FMovePosition(Direction dir, const Position &posIn, Position &posOut) const;
	bool FMovePositionMult(Direction dir, const Position &posIn, Position &posOut, int nLength) const;

	bool FLegalPosition(const Position &pos) const;
	bool FPlace(const std::string &strWord, const Position &pos, Direction dir);
	bool FMatch(const std::string &strWord, const Position &pos, Direction dir) const;
	bool FMatchChar(char ch, const Position &pos) const;
	bool FSchrodingersChar(const Position &pos) const;
	bool HasWord(const std::string &word) const;
	bool HasWords(const std::vector<std::string> &words) const;

	std::vector<Board> *PermuteWhitespace(const Position &pos, const std::vector<std::string> &vecIllegal);

	void Print(void) const;

};

void AppendBoardsToBoards(std::vector<Board> *boards, const std::vector<Board> *boardsToAppend);
