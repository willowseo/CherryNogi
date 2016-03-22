#include "StdAfx.h"
#include "PatternSearch.h"


CPatternSearch::CPatternSearch(pattern p)
{
	id = p.id;
	CheckPoints = p.CheckPoints;

	wcscpy(lpszname, p.name);
	offset = new int[sizeof(p.offset) / 4];
	memcpy(offset, p.offset, sizeof(p.offset));

	BytePattern = new BYTE[sizeof(p.BytePattern)];
	memcpy(BytePattern, p.BytePattern, sizeof(p.BytePattern));
	address = 0;
	SearchPOS = 0x401000;	
	SearchEnd = 0x2FFFFFF;
}


CPatternSearch::~CPatternSearch(void)
{
}

DWORD CPatternSearch::BytePatternSearch()
{
	int CheckIndex = 0;
	for (SearchPOS; SearchPOS < SearchEnd; SearchPOS++) {
		while (1) {
			if (CheckIndex == CheckPoints)
				return SearchPOS;
			if (*(BYTE*)(SearchPOS + offset[CheckIndex]) == BytePattern[CheckIndex])
				CheckIndex++;
			else
				break;
		}
		CheckIndex = 0;
	}
	return 0;
}
