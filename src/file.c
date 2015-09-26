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
#include <string.h>

#include "nls.h"
#include "file.h"

#include <unistd.h>
#if defined(__FreeBSD__) || defined(__OpenBSD__)
#include <sys/sysctl.h>
#endif

int contains_data(FILE *fp, unsigned long ulPosition,
		  const void *pData, unsigned int uiLen)
{
   unsigned char aucBuf[MAX_DATA_LEN];

   unsigned long start_read = ulPosition;
   unsigned long to_read = uiLen;

#if defined(__FreeBSD__) || defined(__OpenBSD__)
   /* cannot read/write directly from/to disk at random offsets */
   /* must read/write from/to pagesize boundaries */

   /*Rounding functions for multiples that are powers of two:*/
   /*Up:   return ((number + multiple - 1) & ~(multiple - 1));*/
   /*Down: return (number & ~(multiple - 1));*/

   unsigned long pagesize = sysconf(_SC_PAGE_SIZE);
   start_read = ulPosition & ~(pagesize - 1); /* round down */
   to_read = (((ulPosition + uiLen) + pagesize - 1) & ~(pagesize - 1)) - start_read; /* round up */
#endif

   if(to_read > MAX_DATA_LEN)
   {
      fprintf(stderr, _("Error: contains_data called with uiLen > %d,\n"),
	      MAX_DATA_LEN);
      fprintf(stderr, _("please increase MAX_DATA_LEN in file.h\n"));
      return 0;
   }
   
   if(fseek(fp, start_read, SEEK_SET))
      return 0;
   if(!fread(aucBuf, to_read, 1, fp))
      return 0;
   if(memcmp(pData, aucBuf + ulPosition - start_read, uiLen))
      return 0;
   return 1;
} /* contains_data */

int write_data(FILE *fp, unsigned long ulPosition,
	       const void *pData, unsigned int uiLen)
{
   unsigned long start_write = ulPosition;
   unsigned long to_write = uiLen;

#if defined (__FreeBSD__) || defined(__OpenBSD__)
   /* cannot read/write directly from/to disk at random offsets */
   /* must read/write from/to pagesize boundaries */
   unsigned long pagesize = sysconf(_SC_PAGE_SIZE);
   unsigned char aucBuf[MAX_DATA_LEN];

   start_write = ulPosition & ~(pagesize - 1); /* round down */
   to_write = (((ulPosition + uiLen) + pagesize - 1) & ~(pagesize - 1)) - start_write; /* round up */

   if(fseek(fp, start_write, SEEK_SET))
      return 0;
   if(!fread(aucBuf, to_write, 1, fp))
      return 0;

    /* modify in the local buffer first */
    memcpy(aucBuf + ulPosition - start_write, pData, uiLen);
    pData = aucBuf;
#endif

   if(fseek(fp, start_write, SEEK_SET))
      return 0;
   if(!fwrite(pData, to_write, 1, fp))
      return 0;
   return 1;
} /* write_data */
