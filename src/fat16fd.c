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
#include "fat16fd.h"

int entire_fat_16_fd_br_matches(FILE *fp)
{
   #include "br_fat16_0x0.h"
   #include "br_fat16fd_0x3e.h"

   return
      ( contains_data(fp, 0x0, br_fat16_0x0, sizeof(br_fat16_0x0)) &&
	/* BIOS Parameter Block might differ between systems */
	contains_data(fp, 0x3e, br_fat16_0x3e, sizeof(br_fat16_0x3e)) );
} /* entire_fat_16_fd_br_matches */

int write_fat_16_fd_br(FILE *fp, int bKeepLabel)
{
   #include "label_11_char.h"
   #include "br_fat16_0x0.h"
   #include "br_fat16fd_0x3e.h"

   if(bKeepLabel)
      return
	 ( write_data(fp, 0x0, br_fat16_0x0, sizeof(br_fat16_0x0)) &&
	   /* BIOS Parameter Block should not be overwritten */
	   write_data(fp, 0x3e, br_fat16_0x3e, sizeof(br_fat16_0x3e)) );
   else
      return
	 ( write_data(fp, 0x0, br_fat16_0x0, sizeof(br_fat16_0x0)) &&
	   /* BIOS Parameter Block should not be overwritten */
	   write_data(fp, 0x2b, label_11_char, sizeof(label_11_char)) &&
	   write_data(fp, 0x3e, br_fat16_0x3e, sizeof(br_fat16_0x3e)) );      
} /* write_fat_16_fd_br */
