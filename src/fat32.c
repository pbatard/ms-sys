#include <stdio.h>

#include "file.h"
#include "fat32.h"

int is_fat_32_fs(FILE *fp)
{
   char *szMagic = "FAT32   ";

   return contains_data(fp, 0x52, szMagic, strlen(szMagic));
} /* is_fat_32_fs */

int is_fat_32_br(FILE *fp)
{
   /* A "file" is probably some kind of FAT32 boot record if it contains the
      magic chars 0x55, 0xAA at positions 0x1FE, 0x3FE and 0x5FE */
   unsigned char aucRef[] = {0x55, 0xAA};
   unsigned char aucMagic[] = {'M','S','W','I','N','4','.','1'};
   int i;

   for(i=0 ; i<3 ; i++)
      if( ! contains_data(fp, 0x1FE + i*0x200, aucRef, sizeof(aucRef)))
	 return 0;
   if( ! contains_data(fp, 0x03, aucMagic, sizeof(aucMagic)))
      return 0;
   return 1;
} /* is_fat_32_br */

int entire_fat_32_br_matches(FILE *fp)
{
   #include "br_fat32_0x0.h"
   #include "br_fat32_0x52.h"
   #include "br_fat32_0x3f0.h"

   return
      ( contains_data(fp, 0x0, br_fat32_0x0, sizeof(br_fat32_0x0)) &&
	/* BIOS Parameter Block might differ between systems */
	contains_data(fp, 0x52, br_fat32_0x52, sizeof(br_fat32_0x52)) &&
	/* Cluster information might differ between systems */
	contains_data(fp, 0x3f0, br_fat32_0x3f0, sizeof(br_fat32_0x3f0)) );
} /* entire_fat_32_br_matches */

int write_fat_32_br(FILE *fp, int bKeepLabel)
{
   #include "br_fat32_0x0.h"
   #include "br_fat32_0x47.h"
   #include "br_fat32_0x52.h"
   #include "br_fat32_0x3f0.h"

   if(bKeepLabel)
      return
	 ( write_data(fp, 0x0, br_fat32_0x0, sizeof(br_fat32_0x0)) &&
	   /* BIOS Parameter Block should not be overwritten */
	   write_data(fp, 0x52, br_fat32_0x52, sizeof(br_fat32_0x52)) &&
	   /* Cluster information is not overwritten, however, it would bo OK
	      to write 0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff here. */
	   write_data(fp, 0x3f0, br_fat32_0x3f0, sizeof(br_fat32_0x3f0)) );
   else
      return
	 ( write_data(fp, 0x0, br_fat32_0x0, sizeof(br_fat32_0x0)) &&
	   /* BIOS Parameter Block should not be overwritten */
	   write_data(fp, 0x47, br_fat32_0x47, sizeof(br_fat32_0x47)) &&
	   /* Cluster information is not overwritten, however, it would bo OK
	      to write 0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff here. */
	   write_data(fp, 0x3f0, br_fat32_0x3f0, sizeof(br_fat32_0x3f0)) );
} /* write_fat_32_br */
