// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define Die(sz)	DieProc(sz, __FILE__, __LINE__)
void DieProc(const char *sz, const char *szFile, int nLine);


// TODO: reference additional headers your program requires here
