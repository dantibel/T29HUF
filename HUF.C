/* Belov Danil, 10-6, 24.11.2021 */

#include <stdio.h>
#include <string.h>

#include "bitrw.h"
#include "huf.h"

/* Tree type definition */
typedef struct tagTREE TREE;
struct tagTREE
{
  INT Ch;
  LONG Freq;
  TREE *Left, *Right;
};

/* Code type definition */
typedef struct
{
  INT Code[47], Len;
} CODE;

static TREE *Nodes[256];
static INT NumOfNodes = 0;
static LONG Freq[256];

/* Input string function */
VOID GetStr( CHAR *S, INT Len )
{
  INT i = 0;
  CHAR c;

  while ((c = getchar()) != '\n')
    if (S != NULL && i < Len - 1)
      S[i++] = c;

  if (S != NULL && i < Len)
    S[i] = 0;
} /* End of 'GetStr' function */

/* Draw tree function */
static VOID DrawTree( TREE *T )
{
  static INT Level;

  if (T != NULL)
  {
    Level++;
    DrawTree(T->Right);
    printf("%*c%c(%d)\n", Level * 5, ' ', T->Ch, T->Freq);
    DrawTree(T->Left);
    Level--;
  }
} /* End of 'DrawTree' function */

/* Clear tree function */
static VOID ClearTree( TREE *T )
{
  if (T != NULL)
  {
    ClearTree(T->Left);
    ClearTree(T->Right);
    free(T);
    T = NULL;
    NumOfNodes--;
  }
} /* End of 'ClearTree' function */

/* Get every symbol frequency function */
static VOID CountFreq( FILE *F )
{
  INT Ch;

  memset(Freq, 0, sizeof(Freq));
  while ((Ch = fgetc(F)) != EOF)
    Freq[Ch]++;
} /* End of 'CountFreq' function */

/* Add one node to tree function */
static VOID PlaceNode( INT Ch, LONG Freq, TREE *Left, TREE *Right )
{
  TREE *tr;
  INT i;

  if ((tr = malloc(sizeof(TREE))) == NULL)
  {
    NumOfNodes--;
    return;
  }

  tr->Ch = Ch, tr->Freq = Freq, tr->Left = Left, tr->Right = Right;

  i = NumOfNodes - 1;

  while (i >= 0 && Nodes[i]->Freq < Freq)
    Nodes[i + 1] = Nodes[i], i--;
  Nodes[i + 1] = tr;
  NumOfNodes++;
} /* End of 'Placenode' function */

static VOID BuildForest( VOID )
{
  INT i = 0;

  for (i = 0; i < 256; i++)
    if (Freq[i] > 0)
      PlaceNode(i, Freq[i], NULL, NULL);
}

/* Build tree with one node from 'Nodes' function */ 
static VOID CollapseForest( VOID )
{
  INT i = 0;

  while (NumOfNodes > 1)
  {
    TREE *Last = Nodes[--NumOfNodes], *PreLast = Nodes[--NumOfNodes];

    PlaceNode(' ', Last->Freq + PreLast->Freq, PreLast, Last);
    Nodes[NumOfNodes] = NULL;

    i = NumOfNodes - 1;
    while (Nodes[i]->Freq < Nodes[NumOfNodes - 1]->Freq)
      Nodes[i - 1] = Nodes[i], i--;
    Nodes[i] = Nodes[NumOfNodes - 1];
  }
} /* End of 'CollapseForest' function */

static CODE NewCodes[256], CurCode = {{0}, 0};

/* Build new codes for elements of tree */
VOID BuildCodes( TREE *T )
{
  if (T->Left == NULL)
  {
    NewCodes[(UCHAR)T->Ch] = CurCode;
    return;
  }
  CurCode.Code[CurCode.Len++] = 0;
  BuildCodes(T->Left);
  CurCode.Len--;
 
  CurCode.Code[CurCode.Len++] = 1;
  BuildCodes(T->Right);
  CurCode.Len--;
} /* End of 'BuildCodes' function */

/* Compress file function */
BOOL Compress( CHAR *InFileName, CHAR *OutFileName )
{
  INT i, ch;
  FILE *OutF, *InF;

  if ((OutF = fopen(OutFileName, "wb")) == NULL)
    return FALSE;

  if ((InF = fopen(InFileName, "r")) == NULL)
  {
    fclose(OutF);
    return FALSE;
  }

  CountFreq(InF);

  for (i = 0; i < 256; i++)
    Nodes[i] = NULL;
  NumOfNodes = 0;
  BuildForest();
  CollapseForest();

  memset(NewCodes, 0, sizeof(NewCodes));
  BuildCodes(Nodes[0]);
  
  rewind(InF);
  fwrite(Freq, 4, 256, OutF);
  WriteBitInit(OutF);
  while ((ch = fgetc(InF)) != EOF)
    for (i = 0; i < NewCodes[ch].Len; i++)
      WriteBit(NewCodes[ch].Code[i]);
  WriteBitClose();

  fclose(OutF);
  fclose(InF);
  ClearTree(Nodes[0]);
  return TRUE;
} /* End of 'Compress' function */

/* Decompress file function */
BOOL Decompress( CHAR *InFileName, CHAR *OutFileName )
{
  INT i;
  LONG n;
  FILE *OutF, *InF;
  TREE *T;

  if ((OutF = fopen(OutFileName, "w")) == NULL)
    return FALSE;

  if ((InF = fopen(InFileName, "rb")) == NULL)
  {
    fclose(OutF);
    return FALSE;
  }

  fread(Freq, 4, 256, InF);
  
  for (i = 0; i < 256; i++)
    Nodes[i] = NULL;

  NumOfNodes = 0;
  BuildForest();
  CollapseForest();

  T = Nodes[0];
  n = Nodes[0]->Freq;
  ReadBitInit(InF);
 
  while (n > 0)
    if (T->Left == NULL)
    {
      fputc(T->Ch, OutF);
      T = Nodes[0];
      n--;
    }
    else
      if (ReadBit())
        T = T->Right;
      else
        T = T->Left;

  fclose(OutF);
  fclose(InF);
  return TRUE;
} /* End of 'Decompress' function */

/* Get stat function */
VOID Stat( CHAR *FileName )
{
  INT i, total;

  FILE *F;

  if ((F = fopen(FileName, "r")) == NULL)
    return;

  CountFreq(F);
  fclose(F);
  
  NumOfNodes = 0;

  BuildForest();
  CollapseForest();

  memset(NewCodes, 0, sizeof(NewCodes));
  
  for (total = 0, i = 0; i < 256; i++)
    if (Freq[i] > 0)
      printf("%c (%3d)[%02X] frequency: %ld\n", i, i, i, Freq[i]);

  ClearTree(Nodes[0]);
} /* End of 'Stat' function */