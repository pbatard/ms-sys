#include <stdio.h>

#include "file.h"
#include "fat32nt.h"

int entire_fat_32_nt_br_matches(FILE *fp)
{
   #include "br_fat32nt_0x0.h"
   #include "br_fat32nt_0x52.h"
   #include "br_fat32nt_0x3f0.h"
   #include "br_fat32nt_0x1800.h"

   return
      ( contains_data(fp, 0x0, br_fat32nt_0x0, sizeof(br_fat32nt_0x0)) &&
	/* BIOS Parameter Block might differ between systems */
	contains_data(fp, 0x52, br_fat32nt_0x52, sizeof(br_fat32nt_0x52)) &&
	/* Cluster information might differ between systems */
	contains_data(fp, 0x3f0, br_fat32nt_0x3f0, sizeof(br_fat32nt_0x3f0)) &&
	contains_data(fp, 0x1800, br_fat32nt_0x1800, sizeof(br_fat32nt_0x1800))
	 );
} /* entire_fat_32_br_matches */

int write_fat_32_nt_br(FILE *fp, int bKeepLabel)
{
   #include "label_11_char.h"
   #include "br_fat32nt_0x0.h"
   #include "br_fat32nt_0x52.h"
   #include "br_fat32nt_0x3f0.h"
   #include "br_fat32nt_0x1800.h"

   if(bKeepLabel)
      return
	 ( write_data(fp, 0x0, br_fat32nt_0x0, sizeof(br_fat32nt_0x0)) &&
	   /* BIOS Parameter Block should not be overwritten */
	   write_data(fp, 0x52, br_fat32nt_0x52, sizeof(br_fat32nt_0x52)) &&
   /* Cluster information is not overwritten, however, it would bo OK
      to write 0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff here. */
	   write_data(fp, 0x3f0, br_fat32nt_0x3f0, sizeof(br_fat32nt_0x3f0)) &&
	   write_data(fp, 0x1800, br_fat32nt_0x1800, sizeof(br_fat32nt_0x1800))
	    );
   else
      return
	 ( write_data(fp, 0x0, br_fat32nt_0x0, sizeof(br_fat32nt_0x0)) &&
	   /* BIOS Parameter Block should not be overwritten */
	   write_data(fp, 0x47, label_11_char, sizeof(label_11_char)) &&
	   write_data(fp, 0x52, br_fat32nt_0x52, sizeof(br_fat32nt_0x52)) &&
   /* Cluster information is not overwritten, however, it would bo OK
      to write 0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff here. */
	   write_data(fp, 0x3f0, br_fat32nt_0x3f0, sizeof(br_fat32nt_0x3f0)) &&
	   write_data(fp, 0x1800, br_fat32nt_0x1800, sizeof(br_fat32nt_0x1800))
	    );
} /* write_fat_32_nt_br */
