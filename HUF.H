/* Belov Danil, 10-6, 24.11.2021 */

#ifndef __HUF_H_
#define __HUF_H_

#define VOID void
#include <commondf.h>

/* Debug memory allocation support */
#ifndef _NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */
#include <stdlib.h>

VOID GetStr( CHAR *S, INT Len );
VOID Stat( CHAR *FileName );
INT Compress( CHAR *InFileName, CHAR *OutFileName );
INT Decompress( CHAR *InFileName, CHAR *OutFileName );

#endif /*__HUF_H_*/