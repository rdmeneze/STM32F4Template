#ifndef __UTILS_H__
#define __UTILS_H__

#include "defs.h"

__inline WORD MAKEWORD( BYTE bLow, BYTE bHigh )
{
	return (bHigh << 8) + bLow;
}

__inline DWORD MAKEDWORD( WORD wLow, WORD wHigh )
{
	return (wHigh << 16) + wLow;
}

#define GET_ARRAY_LEN( x ) (sizeof(x)/sizeof(x[0]))

#endif 
