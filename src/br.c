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
   #include "mbr_dos.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      contains_data(fp, 0x0, mbr_dos_0x0, sizeof(mbr_dos_0x0)) &&
      contains_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* is_dos_mbr */

int is_95b_mbr(FILE *fp)
{
   #include "mbr_95b.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      contains_data(fp, 0x0,   mbr_95b_0x0,   sizeof(mbr_95b_0x0)) &&
      contains_data(fp, 0x0e0, mbr_95b_0x0e0, sizeof(mbr_95b_0x0e0)) &&
      contains_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* is_95b_mbr */

int is_2000_mbr(FILE *fp)
{
   #include "mbr_2000.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      contains_data(fp, 0x0, mbr_2000_0x0, sizeof(mbr_2000_0x0)) &&
      contains_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* is_2000_mbr */

int is_syslinux_mbr(FILE *fp)
{
   #include "mbr_syslinux.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      contains_data(fp, 0x0, mbr_syslinux_0x0, sizeof(mbr_syslinux_0x0)) &&
      contains_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* is_syslinux_mbr */

int is_zero_mbr(FILE *fp)
{
   #include "mbr_zero.h"

   return
      contains_data(fp, 0x0, mbr_zero_0x0, sizeof(mbr_zero_0x0));
	/* Don't bother to check 55AA signature */
} /* is_zero_mbr */

int write_dos_mbr(FILE *fp)
{
   #include "mbr_dos.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      write_data(fp, 0x0, mbr_dos_0x0, sizeof(mbr_dos_0x0)) &&
      write_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* write_dos_mbr */

int write_95b_mbr(FILE *fp)
{
   #include "mbr_95b.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      write_data(fp, 0x0,   mbr_95b_0x0, sizeof(mbr_95b_0x0)) &&
      write_data(fp, 0x0e0, mbr_95b_0x0e0, sizeof(mbr_95b_0x0e0)) &&
      write_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* write_95b_mbr */

int write_2000_mbr(FILE *fp)
{
   #include "mbr_2000.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      write_data(fp, 0x0, mbr_2000_0x0, sizeof(mbr_2000_0x0)) &&
      write_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* write_2000_mbr */

int write_syslinux_mbr(FILE *fp)
{
   #include "mbr_syslinux.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      write_data(fp, 0x0, mbr_syslinux_0x0, sizeof(mbr_syslinux_0x0)) &&
      write_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* write_syslinux_mbr */

int write_zero_mbr(FILE *fp)
{
   #include "mbr_zero.h"
   unsigned char aucRef[] = {0x55, 0xAA};

   return
      write_data(fp, 0x0,   mbr_zero_0x0, sizeof(mbr_zero_0x0)) &&
      write_data(fp, 0x1FE, aucRef, sizeof(aucRef));
} /* write_zero_mbr */
