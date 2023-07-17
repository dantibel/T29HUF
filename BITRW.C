/* Belov Danil, 10-6, 01.12.2021 */

#include <stdio.h>

#include "bitrw.h"

static FILE *BitF;
static INT BitAccum, BitPos;

/*** Write bits to file block ***/
VOID WriteBitInit( FILE *F )
{
  BitF = F;
  BitAccum = 0;
  BitPos = 7;
}

VOID WriteBit( INT Bit )
{
  BitAccum |= Bit << BitPos--; 
  if (BitPos < 0)
  {
    fputc(BitAccum, BitF);
    BitAccum = 0;
    BitPos = 7;
  }
}

VOID WriteBitClose( VOID )
{
  if (BitPos < 7)
    fputc(BitAccum, BitF);
}

/*** Read bits from file block ***/
VOID ReadBitInit( FILE *F )
{
  BitF = F;
  BitPos = -1;
}

INT ReadBit( VOID )
{
  if (BitPos < 0)
    BitAccum = fgetc(BitF), BitPos = 7;
  return (BitAccum >> BitPos--) & 1;
}