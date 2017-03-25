#pragma once

#include <stdio.h>
#define cElements(ary) (sizeof(ary) / sizeof(ary[0]))

extern bool vfRunningUnitTests;


void UTAssertIsTrueProc(bool fExpression, const char *szTest, const char *szCase, const char *szFile, int nLine);
#define UTAssertIsTrue(fExpression, szTest, szCase) \
    UTAssertIsTrueProc(fExpression, szTest, szCase, __FILE__, __LINE__)

void UTAssertAreEqual(int Expected, int Actual, const char *szTest, const char *szCase);
void UTAssertAreEqual(short Expected, short Actual, const char *szTest, const char *szCase);
void UTAssertAreEqual(unsigned short Expected, unsigned short Actual, const char *szTest, const char *szCase);
void UTAssertAreEqual(char Expected, char Actual, const char *szTest, const char *szCase);

inline void UTAssertIsFalse(bool fExpression, const char *szTest, const char *szCase)
{
	UTAssertIsTrue(!fExpression, szTest, szCase);
}

inline void PrintError(const char *szTest, const char *szCase, int Expected, int Actual, const char *szFile, int nLine)
{
	fprintf(stderr, "Unit Test FAILED: %s (%s).  Expected(%d), actual (%d)  (%s: %d)\n", szTest, szCase, Expected, Actual, szFile, nLine);
}


inline void PrintError(const char *szTest, const char *szCase, unsigned int Expected, unsigned int Actual, const char *szFile, int nLine)
{
	fprintf(stderr, "Unit Test FAILED: %s (%s).  Expected(%d), actual (%d)  (%s: %d)\n", szTest, szCase, Expected, Actual, szFile, nLine);
}

inline void PrintError(const char *szTest, const char *szCase, float Expected, float Actual, const char *szFile, int nLine)
{
	fprintf(stderr, "Unit Test FAILED: %s (%s).  Expected(%f), actual (%f).  (%s: %d)\n", szTest, szCase, Expected, Actual, szFile, nLine);
}

template<typename T> void TUTAssertAreEqual(T Expected, T Actual, const char *szTest, const char *szCase, const char *szFile, int nLine)
{
	if (Expected == Actual)
		return;

	PrintError(szTest, szCase, Expected, Actual, szFile, nLine);
}

#define FileLine   __FILE__, __LINE__

void RunTests();

