/******************************************************************
    Copyright (C) 2009  Henrik Carlqvist

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************/
#include <stdio.h>

#include "file.h"
#include "fat32fd.h"

int entire_fat_32_fd_br_matches(FILE *fp)
{
   #include "br_fat32_0x0.h"
   #include "br_fat32fd_0x52.h"
   #include "br_fat32fd_0x3f0.h"

   return
      ( contains_data(fp, 0x0, br_fat32_0x0, sizeof(br_fat32_0x0)) &&
	/* BIOS Parameter Block might differ between systems */
	contains_data(fp, 0x52, br_fat32_0x52, sizeof(br_fat32_0x52)) &&
	/* Cluster information might differ between systems */
	contains_data(fp, 0x3f0, br_fat32_0x3f0, sizeof(br_fat32_0x3f0)) );
} /* entire_fat_32_fd_br_matches */

int write_fat_32_fd_br(FILE *fp, int bKeepLabel)
{
   #include "label_11_char.h"
   #include "br_fat32_0x0.h"
   #include "br_fat32fd_0x52.h"
   #include "br_fat32fd_0x3f0.h"

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
	   write_data(fp, 0x47, label_11_char, sizeof(label_11_char)) &&
	   write_data(fp, 0x52, br_fat32_0x52, sizeof(br_fat32_0x52)) &&
	   /* Cluster information is not overwritten, however, it would bo OK
	      to write 0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff here. */
	   write_data(fp, 0x3f0, br_fat32_0x3f0, sizeof(br_fat32_0x3f0)) );
} /* write_fat_32_nt_br */
