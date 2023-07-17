/* Belov Danil, 10-6, 24.11.2021 */

#include <stdio.h>
#include <conio.h>

#include "huf.h"

#define STR_MAX 100

INT main( INT argc, CHAR *argv[] )
{
  BOOL run = TRUE;
  CHAR InFileName[STR_MAX], OutFileName[STR_MAX];

  SetDbgMemHooks();

  system("chcp 1251");

  if (argc == 1)
  {
    printf("0 - exit program | 1 - statistic | 2 - compress file | 3 - decompress\n\n");
    while (TRUE)
      switch(_getch())
      {
      case '0':
        run = FALSE;
        break;
      case '1':
        printf("Input file name: ");
        GetStr(InFileName, STR_MAX);
        Stat(InFileName);
        break;
      case '2':
        printf("Compress: ");
        GetStr(InFileName, STR_MAX);
        GetStr(OutFileName, STR_MAX);
        printf("Save compressed file to: ");
        Compress(InFileName, OutFileName);
        break;
      case '3':
        printf("Decompress: ");
        GetStr(InFileName, STR_MAX);
        GetStr(OutFileName, STR_MAX);
        printf("Save decomprassed file to: ");
        Compress(InFileName, OutFileName);
        break;
    }
  }
  else 
    if (argc == 3 && argv[1][0] == 's')
      Stat(argv[2]);
    else if (argc == 4 && argv[1][0] == 'c')
      Compress(argv[3], argv[2]);
    else if (argc == 4 && argv[1][0] == 'd')
      Decompress(argv[3], argv[2]);
    else
      printf("Unrecognized switch\n Use: %s {c|d|s} <InFile> <OutFile>\n", argv[0]);
  return 0;
}