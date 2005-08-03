#ifndef PARTITION_INFO_H
#define PARTITION_INFO_H

/* Writes the number of heads to a partition,
   returns TRUE on success, otherwise FALSE */
int write_partition_number_of_heads(FILE *fp);

/* Writes the start sector number to a partition (also known as
   "number of hidden sectors"), returns TRUE on success, otherwise FALSE */
int write_partition_start_sector_number(FILE *fp);

/* Writes a physical disk drive id of 0x80 (for C:) to a partition */
int write_partition_physical_disk_drive_id(FILE *fp);

#endif
