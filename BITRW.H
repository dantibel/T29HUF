/* Belov Danil, 10-6, 01.12.2021 */

#ifndef __BITRW_H_
#define __BITRW_H_

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

VOID WriteBitInit( FILE *F );
VOID WriteBit( INT Bit );
VOID WriteBitClose( VOID );
VOID ReadBitInit( FILE *F );
INT ReadBit( VOID );

#endif /*__BITRW_H_*/