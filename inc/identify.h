#ifndef SANITY_H
#define SANITY_H

#include <stdio.h>

#define NO_WRITING 0
#define AUTO_BR    1
#define MBR        2
#define FAT12_BR   3
#define FAT16_BR   4
#define FAT32_BR   5
#define FAT32NT_BR 6
#define NUMBER_OF_RECORD_TYPES 7

/* Returns TRUE if writing a boot record of type iBr seems like a good idea,
   otherwise FALSE */
int sanity_check(FILE *fp, const char *szPath, int iBr, int bPrintMessages);

/* Prints some information about a device */
void diagnose(FILE *fp, const char *szPath);

/* Does a smart automatic selection of which boot record to write, returns
   the kind of boot record as defined above */
int smart_select(FILE *fp);

#endif
