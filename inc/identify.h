#ifndef IDENTIFY_H
#define IDENTIFY_H

#include <stdio.h>

#define NO_WRITING 0
#define AUTO_BR    1
#define MBR_DOS    2
#define FAT12_BR   3
#define FAT16_BR   4
#define FAT32_BR   5
#define FAT32NT_BR 6
#define MBR_95B    7
#define MBR_2000   8
#define MBR_VISTA  9
#define MBR_WIN7   10
#define MBR_SYSLINUX  11
#define MBR_ZERO   12
#define FAT16FD_BR 13
#define FAT32FD_BR 14
#define NTFS_BR    15
#define MBR_GPT_SYSLINUX  16
#define NUMBER_OF_RECORD_TYPES 	17

/* Returns the number of sectors on disk before partition */
unsigned long partition_start_sector(FILE *fp);
/* Returns the number of heads for the drive of a partition */
unsigned short partition_number_of_heads(FILE *fp);

/* Returns TRUE if writing a boot record of type iBr seems like a good idea,
   otherwise FALSE */
int sanity_check(FILE *fp, const char *szPath, int iBr, int bPrintMessages);

/* Prints some information about a device */
void diagnose(FILE *fp, const char *szPath);

/* Does a smart automatic selection of which boot record to write, returns
   the kind of boot record as defined above */
int smart_select(FILE *fp);

#endif
