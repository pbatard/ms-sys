#ifndef BR_H
#define BR_H

#include <stdio.h>

/* returns TRUE if the file has a boot record, otherwise FALSE.
   The file position will change when this function is called! */
int is_br(FILE *fp);

/* returns TRUE if the file has a LILO boot record, otherwise FALSE.
   The file position will change when this function is called! */
int is_lilo_br(FILE *fp);

/* returns TRUE if the file has a Microsoft master boot record, otherwise
   FALSE.The file position will change when this function is called! */
int is_dos_mbr(FILE *fp);

/* Writes a master boot record to a file, returns TRUE on success, otherwise
   FALSE */
int write_mbr(FILE *fp);

#endif
