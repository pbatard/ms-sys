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

int contains_data(FILE *fp, unsigned long ulPosition,
		  const void *pData, unsigned int uiLen)
{
   unsigned char aucBuf[MAX_DATA_LEN];

   if(uiLen > MAX_DATA_LEN)
   {
      fprintf(stderr, _("Error: contains_data called with uiLen > %d,\n"),
	      MAX_DATA_LEN);
      fprintf(stderr, _("please increase MAX_DATA_LEN in file.h\n"));
      return 0;
   }
   if(fseek(fp, ulPosition, SEEK_SET))
      return 0;
   if(!fread(aucBuf, uiLen, 1, fp))
      return 0;
   if(memcmp(pData, aucBuf, uiLen))
      return 0;
   return 1;
} /* contains_data */

int write_data(FILE *fp, unsigned long ulPosition,
	       const void *pData, unsigned int uiLen)
{
   if(fseek(fp, ulPosition, SEEK_SET))
      return 0;
   if(!fwrite(pData, uiLen, 1, fp))
      return 0;
   return 1;
} /* write_data */
