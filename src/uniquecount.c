#include <uniquecount.h>

static uint32_t	dwUniqueCount = 0;


int __svc(0) incUniqueCount (void);
int __SVC_0      (void) {
  return (++dwUniqueCount);
}
 

uint32_t GetUniqueCount( void )
{
	return incUniqueCount();
}

