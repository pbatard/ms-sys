#include <stdio.h>

#include "file.h"
#include "fat12.h"

int is_fat_12_fs(FILE *fp)
{
   char *szMagic = "FAT12   ";

   return contains_data(fp, 0x36, szMagic, strlen(szMagic));
} /* is_fat_12_fs */

int entire_fat_12_br_matches(FILE *fp)
{
   #include "br_fat12_0x0.h"
   #include "br_fat12_0x3e.h"

   return
      ( contains_data(fp, 0x0, br_fat12_0x0, sizeof(br_fat12_0x0)) &&
	/* BIOS Parameter Block might differ between systems */
	contains_data(fp, 0x3e, br_fat12_0x3e, sizeof(br_fat12_0x3e)) );
} /* entire_fat_12_br_matches */

int write_fat_12_br(FILE *fp, int bKeepLabel)
{
   #include "label_11_char.h"
   #include "br_fat12_0x0.h"
   #include "br_fat12_0x3e.h"

   if(bKeepLabel)
      return
	 ( write_data(fp, 0x0, br_fat12_0x0, sizeof(br_fat12_0x0)) &&
	   /* BIOS Parameter Block might differ between systems */
	   write_data(fp, 0x3e, br_fat12_0x3e, sizeof(br_fat12_0x3e)) );
   else
      return
	 ( write_data(fp, 0x0, br_fat12_0x0, sizeof(br_fat12_0x0)) &&
	   /* BIOS Parameter Block might differ between systems */
	   write_data(fp, 0x2b, label_11_char, sizeof(label_11_char)) &&
	   write_data(fp, 0x3e, br_fat12_0x3e, sizeof(br_fat12_0x3e)) );
} /* write_fat_12_br */
