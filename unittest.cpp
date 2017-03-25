#include <stdio.h>

#include "unittest.h"

void UTAssertIsTrueProc(bool fExpression, const char *szTest, const char *szCase, const char *szFile, int nLine)
{
	if (fExpression)
		return;

	fprintf(stderr, "Unit Test FAILED: %s (%s) [%s: %d]\n", szTest, szCase, szFile, nLine);
}

void UTAssertAreEqual(int Expected, int Actual, const char *szTest, const char *szCase)
{
	TUTAssertAreEqual<int>(Expected, Actual, szTest, szCase, FileLine);
}

void UTAssertAreEqual(short Expected, short Actual, const char *szTest, const char *szCase)
{
	if (Expected == Actual)
		return;

	fprintf(stderr, "Unit Test FAILED: %s (%s).  Expected(%d), actual (%d)\n", szTest, szCase, Expected, Actual);
}

void UTAssertAreEqual(unsigned short Expected, unsigned short Actual, const char *szTest, const char *szCase)
{
	if (Expected == Actual)
		return;

	fprintf(stderr, "Unit Test FAILED: %s (%s).  Expected(%d), actual (%d)\n", szTest, szCase, Expected, Actual);
}

void UTAssertAreEqual(char Expected, char Actual, const char *szTest, const char *szCase)
{
	if (Expected == Actual)
		return;

	fprintf(stderr, "Unit Test FAILED: %s (%s).  Expected(%d), actual (%d)\n", szTest, szCase, Expected, Actual);
}

extern void TestCore(void);

bool vfRunningUnitTests = false;

void RunTests()
{
	vfRunningUnitTests = true;
	TestCore();
	vfRunningUnitTests = false;
}

