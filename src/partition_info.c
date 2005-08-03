#include <stdio.h>

#include "file.h"
#include "identify.h"
#include "partition_info.h"

int write_partition_number_of_heads(FILE *fp)
{
   unsigned char aucBuf[2];
   unsigned short s = partition_number_of_heads(fp);

   if(!s)
      return 0;
   /* Converting a number like this is not necessary as long as we are on
      i386 compatible CPUs, however, the following code might make the program
      more portable... */
   aucBuf[0] = s & 0xff;
   aucBuf[1] = (s & 0xff00) >> 8;
   return write_data(fp, 0x1a, aucBuf, 2);
} /* write_partition_number_of_heads */

int write_partition_start_sector_number(FILE *fp)
{
   unsigned char aucBuf[4];
   unsigned long l = partition_start_sector(fp);

   if(!l)
      return 0;
   /* Converting a number like this is not necessary as long as we are on
      i386 compatible CPUs, however, the following code might make the program
      more portable... */
   aucBuf[0] = l & 0xff;
   aucBuf[1] = (l & 0xff00) >> 8;
   aucBuf[2] = (l & 0xff0000) >> 16;
   aucBuf[3] = (l & 0xff000000) >> 24;
   return write_data(fp, 0x1c, aucBuf, 4);
} /* write_partition_start_sector_number */

int write_partition_physical_disk_drive_id(FILE *fp)
{
   unsigned char ucId = 0x80; /* C: */
   
   return write_data(fp, 0x40, &ucId, 1);
} /* write_partition_physical_disk_drive_id */
