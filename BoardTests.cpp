#include "board.h"
#include "unittest.h"
#include "string.h"
#include "stdafx.h"
#include <string>

void Board::TestDirection(int xMax, int yMax, int x, int y, Direction dir, bool fExpected, int xExpected, int yExpected, const char *szTest)
{
	Board board(xMax, yMax);
	Position pos = { x, y };
	Position posOut;

	UTAssertIsTrue(fExpected == board.FMovePosition(dir, pos, posOut), "Board::TestDirection", szTest);
	TUTAssertAreEqual(xExpected, posOut.x, "Board::TestDirection", szTest, __FILE__, __LINE__);
	TUTAssertAreEqual(yExpected, posOut.y, "Board::TestDirection", szTest, __FILE__, __LINE__);

	UTAssertIsTrue(fExpected == board.FMovePosition(dir, pos, pos), "Board::TestDirection in-place pos", szTest);
	TUTAssertAreEqual(xExpected, pos.x, "Board::TestDirection.x in-place pos", szTest, __FILE__, __LINE__);
	TUTAssertAreEqual(yExpected, pos.y, "Board::TestDirection.y in-place pos", szTest, __FILE__, __LINE__);

}

void Board::TestDirectionMult(int xMax, int yMax, int x, int y, int nLength, Direction dir, bool fExpected, int xExpected, int yExpected, const char *szTest)
{
	Board board(xMax, yMax);
	Position pos = { x, y };
	Position posOut;

	UTAssertIsTrue(fExpected == board.FMovePositionMult(dir, pos, posOut, nLength), "Board::TestDirectionMult", szTest);
	TUTAssertAreEqual(xExpected, posOut.x, "Board::TestDirectionMult.x", szTest, __FILE__, __LINE__);
	TUTAssertAreEqual(yExpected, posOut.y, "Board::TestDirectionMult.y", szTest, __FILE__, __LINE__);

	UTAssertIsTrue(fExpected == board.FMovePositionMult(dir, pos, pos, nLength), "Board::TestDirectionMult in-place pos", szTest);
	TUTAssertAreEqual(xExpected, pos.x, "Board::TestDirectionMult.x in-place pos", szTest, __FILE__, __LINE__);
	TUTAssertAreEqual(yExpected, pos.y, "Board::TestDirectionMult.y in-place pos", szTest, __FILE__, __LINE__);

}

void Board::TestInit(void)
{
	Board board(2, 3);

	TUTAssertAreEqual('.', board.m_rgchBoard[0], "Board::TestInit", "TestSimple rgchBoard", __FILE__, __LINE__);
	TUTAssertAreEqual('.', board.m_rgchBoard[1], "Board::TestInit", "TestSimple rgchBoard", __FILE__, __LINE__);
	TUTAssertAreEqual('.', board.m_rgchBoard[2], "Board::TestInit", "TestSimple rgchBoard", __FILE__, __LINE__);
	TUTAssertAreEqual('.', board.m_rgchBoard[3], "Board::TestInit", "TestSimple rgchBoard", __FILE__, __LINE__);
	TUTAssertAreEqual('.', board.m_rgchBoard[4], "Board::TestInit", "TestSimple rgchBoard", __FILE__, __LINE__);
	TUTAssertAreEqual('.', board.m_rgchBoard[5], "Board::TestInit", "TestSimple rgchBoard", __FILE__, __LINE__);
	TUTAssertAreEqual(2, board.m_xMax, "Board::TestInit", "TestSimple xMax", __FILE__, __LINE__);
	TUTAssertAreEqual(3, board.m_yMax, "Board::TestInit", "TestSimple yMax", __FILE__, __LINE__);
}

void Board::TestPosition(void)
{
	Board board(5, 5);

	board.TestDirection(5, 5, 1, 1, Up, true, 1, 0, "Up, in bounds");
	board.TestDirection(5, 5, 1, 1, UpRight, true, 2, 0, "UpRight, in bounds");
	board.TestDirection(5, 5, 1, 1, Right, true, 2, 1, "Right, in bounds");
	board.TestDirection(5, 5, 1, 1, DownRight, true, 2, 2, "DownRight, in bounds");
	board.TestDirection(5, 5, 1, 1, Down, true, 1, 2, "Down, in bounds");
	board.TestDirection(5, 5, 1, 1, DownLeft, true, 0, 2, "DownLeft, in bounds");
	board.TestDirection(5, 5, 1, 1, Left, true, 0, 1, "Left, in bounds");
	board.TestDirection(5, 5, 1, 1, UpLeft, true, 0, 0, "UpLeft, in bounds");

	board.TestDirection(5, 5, 1, 0, Up, false, 1, 0, "Up, out-of-bounds");
	board.TestDirection(5, 5, 5, 1, UpRight, false, 5, 1, "UpRight, out-of-bounds");
	board.TestDirection(5, 5, 5, 1, DownRight, false, 5, 1, "DownRight, out-of-bounds");
	board.TestDirection(5, 5, 1, 5, Down, false, 1, 5, "Down, out-of-bounds");
	board.TestDirection(5, 5, 0, 1, Left, false, 0, 1, "Left, out-of-bounds");

	board.TestDirectionMult(5, 5, 2, 2, 2, Up, true, 2, 0, "Up, in bounds");
	board.TestDirectionMult(5, 5, 2, 2, 2, UpRight, true, 4, 0, "UpRight, in bounds");
	board.TestDirectionMult(5, 5, 2, 2, 2, Right, true, 4, 2, "Right, in bounds");
	board.TestDirectionMult(5, 5, 2, 2, 2, DownRight, true, 4, 4, "DownRight, in bounds");
	board.TestDirectionMult(5, 5, 2, 2, 2, Down, true, 2, 4, "Down, in bounds");
	board.TestDirectionMult(5, 5, 2, 2, 2, DownLeft, true, 0, 4, "DownLeft, in bounds");
	board.TestDirectionMult(5, 5, 2, 2, 2, Left, true, 0, 2, "Left, in bounds");
	board.TestDirectionMult(5, 5, 2, 2, 2, UpLeft, true, 0, 0, "UpLeft, in bounds");

	board.TestDirectionMult(5, 5, 1, 0, 2, Up, false, 1, 0, "Up, out-of-bounds");
	board.TestDirectionMult(5, 5, 4, 1, 2, UpRight, false, 4, 1, "UpRight, out-of-bounds");
	board.TestDirectionMult(5, 5, 4, 1, 2, DownRight, false, 4, 1, "DownRight, out-of-bounds");
	board.TestDirectionMult(5, 5, 1, 5, 2, Down, false, 1, 5, "Down, out-of-bounds");
	board.TestDirectionMult(5, 5, 0, 1, 2, Left, false, 0, 1, "Left, out-of-bounds");

	board.TestDirectionMult(2, 2, 0, 0, 1, Next, true, 1, 0, "Next, in bounds");
	board.TestDirectionMult(2, 2, 1, 1, 1, Next, false, 1, 1, "Next, out-of-bounds");

	board.TestDirectionMult(2, 2, 0, 0, -1, Next, false, 0, 0, "Next (-1), out-of-bounds");
	board.TestDirectionMult(2, 2, 1, 1, -1, Next, true, 0, 1, "Next (-1), in bounds");

	board.TestDirectionMult(3, 4, 1, 1, 2, Next, true, 0, 2, "Next (2) wrapping");
}

void Board::TestPlace(int xMax, int yMax, int x, int y, const char *szInitial, const char *szWord, Direction dir, bool fExpected, const char *szExpected, const char *szTest)
{
	Board board(xMax, yMax, szInitial);
	Position pos = { x, y };

	memcpy(board.m_rgchBoard, szInitial, xMax * yMax);
	TUTAssertAreEqual(fExpected, board.FPlace(szWord, pos, dir), "TestPlace FPlace", szTest, __FILE__, __LINE__);

	UTAssertIsTrue(memcmp(board.m_rgchBoard, szExpected, xMax * yMax) == 0, "TestPlace memcmp", szTest);
}

void Board::TestPosToOffset(int xMax, int yMax, int x, int y, int offsetExpected, const char *szTest)
{
	Board board(xMax, yMax);
	Position pos = { x, y };

	TUTAssertAreEqual(offsetExpected, board.PosToOffset(pos), "TestPosToOffset", szTest, __FILE__, __LINE__);
}

void Board::TestOffsetToPos(int xMax, int yMax, int offset, int xExpected, int yExpected, const char *szTest)
{
	Board board(xMax, yMax);
	Position posOut;

	board.OffsetToPos(offset, posOut);

	TUTAssertAreEqual(xExpected, posOut.x, "TestOffsetToPos.x", szTest, __FILE__, __LINE__);
	TUTAssertAreEqual(yExpected, posOut.y, "TestOffsetToPos.x", szTest, __FILE__, __LINE__);
}

void Board::TestOffsets(void)
{
	TestPosToOffset(1, 1, 0, 0, 0, "test origin");
	TestPosToOffset(1, 1, 0, 2, 2, "test pastx2 end");
	TestPosToOffset(1, 1, 0, 1, 1, "test past end single box");
	TestPosToOffset(3, 2, 0, 0, 0, "test origin 3x2");
	TestPosToOffset(3, 2, 2, 1, 5, "test end 3x2");
	TestPosToOffset(3, 2, 0, 1, 3, "test middle 3x2");

	TestOffsetToPos(1, 1, 0, 0, 0, "test origin 1x1");
	TestOffsetToPos(1, 1, 2, 0, 2, "test middle 1x1");
	TestOffsetToPos(1, 1, 3, 0, 3, "test end 1x1");
	TestOffsetToPos(3, 2, 0, 0, 0, "test origin 3x2");
	TestOffsetToPos(3, 2, 4, 1, 1, "test middle 3x2");
	TestOffsetToPos(3, 2, 5, 2, 1, "test end 3x2");
	TestOffsetToPos(3, 2, 8, 2, 2, "test past end 3x2");
}

void Board::TestPlaces(void)
{
#define TEST_CASE "no-overlap"

	Board::TestPlace(3, 3, 0, 0, ".........", "foo", Right, true, "foo......", "simple in bounds right " TEST_CASE);
	Board::TestPlace(3, 3, 0, 0, ".........", "foo", Down, true, "f..o..o..", "simple in bounds down " TEST_CASE);
	Board::TestPlace(3, 3, 2, 1, ".........", "foo", Left, true, "...oof...", "simple in bounds left " TEST_CASE);
	Board::TestPlace(3, 3, 2, 2, ".........", "foo", Up, true, "..o..o..f", "simple in bounds up " TEST_CASE);
	Board::TestPlace(3, 3, 0, 0, ".........", "foo", DownRight, true, "f...o...o", "simple in bounds DownRight " TEST_CASE);
	Board::TestPlace(3, 3, 2, 0, ".........", "foo", DownLeft, true, "..f.o.o..", "simple in bounds DownLeft " TEST_CASE);
	Board::TestPlace(3, 3, 2, 2, ".........", "foo", UpLeft, true, "o...o...f", "simple in bounds UpLeft " TEST_CASE);
	Board::TestPlace(3, 3, 0, 2, ".........", "foo", UpRight, true, "..o.o.f..", "simple in bounds UpRight " TEST_CASE);

#undef TEST_CASE
#define TEST_CASE "overlap OK"

	Board::TestPlace(3, 3, 0, 0, ".o.......", "foo", Right, true, "foo......", "simple in bounds right " TEST_CASE);
	Board::TestPlace(3, 3, 0, 0, "...o.....", "foo", Down, true, "f..o..o..", "simple in bounds down " TEST_CASE);
	Board::TestPlace(3, 3, 2, 1, "....o....", "foo", Left, true, "...oof...", "simple in bounds left " TEST_CASE);
	Board::TestPlace(3, 3, 2, 2, ".....o...", "foo", Up, true, "..o..o..f", "simple in bounds up " TEST_CASE);
	Board::TestPlace(3, 3, 0, 0, "....o....", "foo", DownRight, true, "f...o...o", "simple in bounds DownRight " TEST_CASE);
	Board::TestPlace(3, 3, 2, 0, "....o....", "foo", DownLeft, true, "..f.o.o..", "simple in bounds DownLeft " TEST_CASE);
	Board::TestPlace(3, 3, 2, 2, "....o....", "foo", UpLeft, true, "o...o...f", "simple in bounds UpLeft " TEST_CASE);
	Board::TestPlace(3, 3, 0, 2, ".....a...", "foo", UpRight, true, "..o.oaf..", "simple in bounds UpRight " TEST_CASE);

#undef TEST_CASE
#define TEST_CASE "overlap NOT OK"
	Board::TestPlace(3, 3, 0, 2, "....aa...", "foo", UpRight, false, "....aaf..", "simple in bounds UpRight " TEST_CASE);
}

void Board::TestPlaceChar(void)
{
	Board board(2, 3);
	Position pos = { 1, 1 };

	UTAssertIsTrue(board.FPlaceChar('a', pos), "TestPlaceChar", "simple test nooverlap");
	UTAssertIsTrue(memcmp("...a..", board.m_rgchBoard, 2 * 3) == 0, "Board::TestPlaceChar", "TestSimple memcmp");

	UTAssertIsTrue(board.FPlaceChar('a', pos), "TestPlaceChar", "simple test overlap match ok");
	UTAssertIsTrue(memcmp("...a..", board.m_rgchBoard, 2 * 3) == 0, "Board::TestPlaceChar", "TestSimple memcmp overlap match ok");

	UTAssertIsTrue(!board.FPlaceChar('b', pos), "TestPlaceChar", "simple test overlap match not ok");
	UTAssertIsTrue(memcmp("...a..", board.m_rgchBoard, 2 * 3) == 0, "Board::TestPlaceChar", "TestSimple memcmp overlap match ok");

	pos.y = 0;
	UTAssertIsTrue(board.FPlaceChar('b', pos), "TestPlaceChar", "simple test nooverlap nonempty");
	UTAssertIsTrue(memcmp(".b.a..", board.m_rgchBoard, 2 * 3) == 0, "Board::TestPlaceChar", "TestSimple memcmp nooverlap nonempty");
}

void Board::TestAssign(void)
{
	Board b1(2, 3);

	memcpy(b1.m_rgchBoard, "t.e.st", 2 * 3);

	Board b2 = b1;

	UTAssertIsTrue(memcmp(b1.m_rgchBoard, b2.m_rgchBoard, 2 * 3) == 0, "TestAssign", "TestAssignSimple rgchBoard");
	TUTAssertAreEqual(b1.m_xMax, b2.m_xMax, "TestAssign", "TestAssignSimple xMax", __FILE__, __LINE__);
	TUTAssertAreEqual(b1.m_yMax, b2.m_yMax, "TestAssign", "TestAssignSimple yMax", __FILE__, __LINE__);
	UTAssertIsTrue(b1.m_rgchBoard != b2.m_rgchBoard, "TestAssign", "TestAssignSimple m_rgchBoard pointers not equal");
}

void Board::TestBoardLessThan(int xMax, int yMax, char *rgchLeft, char *rgchRight, bool fExpected, const char *szTest)
{
	Board left(xMax, yMax);
	Board right(xMax, yMax);

	memcpy(left.m_rgchBoard, rgchLeft, xMax * yMax);
	memcpy(right.m_rgchBoard, rgchRight, xMax * yMax);

	TUTAssertAreEqual(fExpected, left < right, "TestBoardLessThan", szTest, __FILE__, __LINE__);
}

void Board::TestBoardGreaterThan(int xMax, int yMax, char *rgchLeft, char *rgchRight, bool fExpected, const char *szTest)
{
	Board left(xMax, yMax);
	Board right(xMax, yMax);

	memcpy(left.m_rgchBoard, rgchLeft, xMax * yMax);
	memcpy(right.m_rgchBoard, rgchRight, xMax * yMax);

	TUTAssertAreEqual(fExpected, left > right, "TestBoardGreaterThan", szTest, __FILE__, __LINE__);
}

void Board::TestBoardEqualTo(int xMax, int yMax, char *rgchLeft, char *rgchRight, bool fExpected, const char *szTest)
{
	Board left(xMax, yMax);
	Board right(xMax, yMax);

	memcpy(left.m_rgchBoard, rgchLeft, xMax * yMax);
	memcpy(right.m_rgchBoard, rgchRight, xMax * yMax);

	TUTAssertAreEqual(fExpected, left == right, "TestBoardEqualTo", szTest, __FILE__, __LINE__);
}

void Board::TestBoardComparators(void)
{
	TestBoardLessThan(3, 4, "aaabbbcccddd", "bbbcccdddaaa", true, "test less than");
	TestBoardLessThan(3, 4, "bbbcccdddaaa", "aaabbbcccddd", false, "test less than FALSE");
	TestBoardLessThan(3, 4, "aaabbbcccddd", "aaabbbcccddd", false, "test less than (EQUAL)");

	TestBoardGreaterThan(3, 4, "aaabbbcccddd", "bbbcccdddaaa", false, "test greater than FALSE");
	TestBoardGreaterThan(3, 4, "bbbcccdddaaa", "aaabbbcccddd", true, "test greater than");
	TestBoardGreaterThan(3, 4, "aaabbbcccddd", "aaabbbcccddd", false, "test greater than (EQUAL)");

	TestBoardEqualTo(3, 4, "aaabbbcccddd", "bbbcccdddaaa", false, "test equal to FALSE");
	TestBoardEqualTo(3, 4, "bbbcccdddaaa", "aaabbbcccddd", false, "test equal to FALSE");
	TestBoardEqualTo(3, 4, "aaabbbcccddd", "aaabbbcccddd", true, "test equal to (EQUAL)");

}

void Board::Test(void)
{
	Board::TestPosition();
	Board::TestInit();
	Board::TestPlaceChar();
	Board::TestPlaces();
	Board::TestAssign();
	Board::TestOffsets();
	Board::TestBoardComparators();
}
