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
