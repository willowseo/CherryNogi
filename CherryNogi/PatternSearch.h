#pragma once
#include "stdafx.h"
#include <windows.h>


struct pattern{
	LPTSTR name;
	int id;
	int CheckPoints;
	int offset[32];
	BYTE BytePattern[32];
};


class CPatternSearch
{
public:
	CPatternSearch(pattern p);
	~CPatternSearch(void);
	void Search();
	DWORD BytePatternSearch();

	TCHAR lpszname[MAX_PATH];
	int id;
	int CheckPoints;
	int* offset;
	BYTE* BytePattern;
	DWORD address;

	DWORD SearchPOS;
	DWORD SearchEnd;
};

