#include <stdio.h>

#include "file.h"
#include "br.h"

int is_br(FILE *fp)
{
   /* A "file" is probably some kind of boot record if it contains the magic
      chars 0x55, 0xAA at position 0x1FE */
   unsigned char aucRef[] = {0x55, 0xAA};

   return contains_data(fp, 0x1FE, aucRef, sizeof(aucRef)); 
} /* is_br */

int is_lilo_br(FILE *fp)
{
   /* A "file" is probably a LILO boot record if it contains the magic
      chars LILO at position 0x6 or 0x2 for floppies */
   unsigned char aucRef[] = {'L','I','L','O'};

   return ( contains_data(fp, 0x6, aucRef, sizeof(aucRef)) ||
	    contains_data(fp, 0x2, aucRef, sizeof(aucRef)) ); 
} /* is_lilo_br */

int is_dos_mbr(FILE *fp)
{
   #include "mbr_0x0.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      contains_data(fp, 0x0, mbr_0x0, sizeof(mbr_0x0)) &&
      contains_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* is_dos_mbr */

int write_mbr(FILE *fp)
{
   #include "mbr_0x0.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      write_data(fp, 0x0, mbr_0x0, sizeof(mbr_0x0)) &&
      write_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* write_mbr */
