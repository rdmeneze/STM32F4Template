#ifndef __PER_PREFIX__
#define __PER_PREFIX__

#include "defs.h"

//----------------------------------------------------------------------------
//--------------------- peripherical prefix ----------------------------------

#define PREFIX_TIMER (0x01)

//----------------------------------------------------------------------------
//--------------------- peripherical prefix constructor ----------------------


__inline DWORD MAKEPREFIX( BYTE prefix, DWORD dev ) 
{
	dev &= 0x0FFFFFFF;
	prefix &= 0x0F;
	
	return (prefix << 28) | dev ;
}


#endif // ~__PER_PREFIX__

