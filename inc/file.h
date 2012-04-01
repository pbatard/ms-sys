#ifndef FILE_H
#define FILE_H

/* Max valid value of uiLen for contains_data */
#define MAX_DATA_LEN 10240

/* Checks if a file contains a data pattern of length uiLen at position
   ulPositoin. The file pointer will change when calling this function! */
int contains_data(FILE *fp, unsigned long ulPosition,
		  const void *pData, unsigned int uiLen);

/* Writes a data pattern of length uiLen at position ulPositoin.
   The file pointer will change when calling this function! */
int write_data(FILE *fp, unsigned long ulPosition,
	       const void *pData, unsigned int uiLen);

#endif
