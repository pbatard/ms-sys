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
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "fat12.h"
#include "fat16.h"
#include "fat16fd.h"
#include "fat32.h"
#include "fat32fd.h"
#include "fat32nt.h"
#include "ntfs.h"
#include "br.h"
#include "identify.h"
#include "nls.h"
#include "partition_info.h"

#define VERSION "2.4.0"

void print_help(const char *szCommand);
void print_version(void);
int parse_switches(int argc, char **argv, int *piBr,
		   int *pbForce, int *pbPrintVersion,
		   int *pbKeepLabel, int *pbWritePartitionInfo, int *piHeads);
int isnumber(const char *szString);

int main(int argc, char **argv)
{
   FILE *fp;
   int iBr = NO_WRITING;
   int bForce = 0;
   int bPrintVersion = 0;
   int bKeepLabel = 1;
   int bWritePartitionInfo = 0;
   int iHeads = -1;
   int iRet = 0;

   nls_init();
   if(parse_switches(argc, argv, &iBr, &bForce, &bPrintVersion,
		     &bKeepLabel, &bWritePartitionInfo, &iHeads))
   {
      print_help(argv[0]);
      return 0;
   }
   if(bPrintVersion)
   {
      print_version();
      if(argc < 3)
	 return 0;
   }
   fp=fopen(argv[argc-1], (iBr || bWritePartitionInfo) ? "r+b" : "rb");
   if(!fp)
   {
      printf(_("Unable to open %s, %s\n"), argv[argc-1], strerror(errno));
      return 1;
   }
   if(iBr == AUTO_BR)
   {
      iBr = smart_select(fp);
      if(!iBr)
	 printf(_("Unable to automaticly select boot record for %s\n"),
		argv[argc-1]);
   }
   if(iBr && !bForce)
   {
      if( ! sanity_check(fp, argv[argc-1], iBr, 1) )
      {
	 fclose(fp);
	 return 1;
      }
   }
   if( bWritePartitionInfo )
   {
      if( !iBr && !bForce)
      {
	 if( ! sanity_check(fp, argv[argc-1], FAT32_BR, 1) )
	 {
	    fclose(fp);
	    return 1;
	 }
      }
      if( write_partition_start_sector_number(fp) )
      {
	 printf(_("Start sector %ld (nr of hidden sectors) successfully written to %s\n"),
		partition_start_sector(fp),
		argv[argc-1]);
	 if( write_partition_physical_disk_drive_id(fp) )
	 {
	    printf(_("Physical disk drive id 0x80 (C:) successfully written to %s\n"),
		   argv[argc-1]);
	    if( write_partition_number_of_heads(fp, iHeads))
	    {
	       printf(_("Number of heads (%d) successfully written to %s\n"),
		      iHeads != -1 ? iHeads : partition_number_of_heads(fp),
		      argv[argc-1]);
	    }
	    else
	    {
	       printf(_("Failed writing number of heads to %s\n"),
		      argv[argc-1]);
	    }
	 }
	 else
	 {
	    printf(_("Failed writing physical disk drive id to %s\n"),
		   argv[argc-1]);
	 }
      }
      else
      {
	 printf(_("Failed writing start sector to %s, this is only possible to do with\n"),
		argv[argc-1]);
	 printf(_("real partitions!\n"));
	 iRet = 1;
      }
   }
   switch(iBr)
   {
      case NO_WRITING:
      {
	 if( ! bWritePartitionInfo )
	 {
	    diagnose(fp, argv[argc-1]);
	 }
	 break;
      }
      case MBR_WIN7:
      {
         if(write_win7_mbr(fp))
            printf(_("Windows 7 master boot record successfully written to %s\n"),
                   argv[argc-1]);
         else
         {
            printf(_("Failed writing Windows 7 master boot record to %s\n"),
                   argv[argc-1]);
            iRet = 1;
         }
      }
      break;
      case MBR_VISTA:
      {
         if(write_vista_mbr(fp))
            printf(_("Windows Vista master boot record successfully written to %s\n"),
                   argv[argc-1]);
         else
         {
            printf(_("Failed writing Windows Vista master boot record to %s\n"),
                   argv[argc-1]);
            iRet = 1;
         }
      }
      break;
      case MBR_2000:
      {
	 if(write_2000_mbr(fp))
	    printf(_("Windows 2000/XP/2003 master boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing Windows 2000/XP/2003 master boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case MBR_95B:
      {
	 if(write_95b_mbr(fp))
	    printf(_("Windows 95B/98/98SE/ME master boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing Windows 95B/98/98SE/ME master boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case MBR_DOS:
      {
	 if(write_dos_mbr(fp))
	    printf(_("DOS/Windows NT master boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing DOS/Windows NT master boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case MBR_SYSLINUX:
      {
	 if(write_syslinux_mbr(fp))
	    printf(_("Syslinux master boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing syslinux master boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case MBR_GPT_SYSLINUX:
      {
	 if(write_syslinux_gpt_mbr(fp))
	    printf(_("Syslinux GPT master boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing syslinux GPT master boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case MBR_ZERO:
      {
	 if(write_zero_mbr(fp))
	    printf(_("Empty (zeroed) master boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing empty (zeroed) master boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case FAT12_BR:
      {
	 if(write_fat_12_br(fp, bKeepLabel))
	    printf(_("FAT12 boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing FAT12 boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case FAT16_BR:
      {
	 if(write_fat_16_br(fp, bKeepLabel))
	    printf(_("FAT16 boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing FAT16 boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case FAT16FD_BR:
      {
	 if(write_fat_16_fd_br(fp, bKeepLabel))
	    printf(_("FAT16 FreeDOS boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing FAT16 FreeDOS boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case FAT32NT_BR:
      {
	 if(write_fat_32_nt_br(fp, bKeepLabel))
	    printf(_("FAT32 NT boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing FAT32 NT boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case FAT32FD_BR:
      {
	 if(write_fat_32_fd_br(fp, bKeepLabel))
	    printf(_("FAT32 FreeDOS boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing FAT32 FreeDOS boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case FAT32_BR:
      {
	 if(write_fat_32_br(fp, bKeepLabel))
	    printf(_("FAT32 DOS boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing FAT32 DOS boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case NTFS_BR:
      {
	 if(write_ntfs_br(fp))
	    printf(_("NTFS Windows 7 boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing NTFS Windows 7 boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      default:
      {
	 printf(_("Whoops, internal error, unknown boot record\n"));
      }
      break;
   }
   fclose(fp);
   return iRet;
} /* main */

void print_help(const char *szCommand)
{
   printf(_("Usage:\n\t%s [options] [device]\nOptions:\n"), szCommand);
   printf(
     _("    -1, --fat12     Write a FAT12 floppy boot record to device\n"));
   printf(
     _("    -2, --fat32nt   Write a FAT32 partition NT boot record to device\n"));
   printf(
     _("    -3, --fat32     Write a FAT32 partition DOS boot record to device\n"));
   printf(
     _("    -4, --fat32free Write a FAT32 partition FreeDOS boot record to device\n"));
   printf(
     _("    -5, --fat16free Write a FAT16 partition FreeDOS boot record to device\n"));
   printf(
     _("    -6, --fat16     Write a FAT16 partition DOS boot record to device\n"));
   printf(
     _("    -n, --ntfs      Write a NTFS partition Windows 7 boot record to device\n"));
   printf(
      _("    -l, --wipelabel Reset partition disk label in boot record\n"));
   printf(
      _("    -p, --partition Write partition info (hidden sectors, heads and drive id)\n"));
   printf(
      _("                    to boot record\n"));
   printf(
      _("    -H, --heads <n> Manually set number of heads if partition info is written\n"));
   printf(
      _("    -7, --mbr7      Write a Windows 7 MBR to device\n"));
   printf(
      _("    -i, --mbrvista  Write a Windows Vista MBR to device\n"));
   printf(
      _("    -m, --mbr       Write a Windows 2000/XP/2003 MBR to device\n"));
   printf(
      _("    -9, --mbr95b    Write a Windows 95B/98/98SE/ME MBR to device\n"));
   printf(
      _("    -d, --mbrdos    Write a DOS/Windows NT MBR to device\n"));
   printf(
      _("    -s, --mbrsyslinux    Write a syslinux MBR to device\n"));
   printf(
      _("    -t, --mbrgptsyslinux Write a syslinux GPT MBR to device\n"));
   printf(
      _("    -z, --mbrzero   Write an empty (zeroed) MBR to device\n"));
   printf(
      _("    -f, --force     Force writing of boot record\n"));
   printf(
      _("    -h, --help      Display this help and exit\n"));
   printf(
      _("    -v, --version   Show program version\n"));
   printf(
_("    -w, --write     Write automatically selected boot record to device\n\n"));
   printf(_("    Default         Inspect current boot record\n\n"));
   printf(
      _("Warning: Writing the wrong kind of boot record to a device might\n"));
   printf(
      _("destroy partition information or file system!\n\n"));
} /* print_help */

void print_version(void)
{
   printf(_("ms-sys version %s\n"), VERSION);
   printf(_("Written by Henrik Carlqvist (henca@users.SourceForge.net)\n\n"));
   printf(_("Copyright (C) 2009 Free Software Foundation, Inc.\n"));
   printf(_("This is free software; see the source for copying conditions.  There is NO\n"));
   printf(_("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n"));
} /* print_version */

int parse_switches(int argc, char **argv, int *piBr,
		   int *pbForce, int *pbPrintVersion,
		   int *pbKeepLabel, int *pbWritePartitionInfo,
		   int *piHeads)
{
   int bHelp = 0;
   int i;

   *piBr = NO_WRITING;
   *pbForce = 0;
   *pbPrintVersion = 0;
   *pbKeepLabel = 1;
   *pbWritePartitionInfo = 0;
   *piHeads = -1;
   
   
   if(argc < 2)
      return 1;
   /* Don't parse the device */
   argc--;
   if(( ! strcmp("-h", argv[argc])) ||
      ( ! strcmp("--help", argv[argc])))
      bHelp = 1;
   else if(( ! strcmp("-v", argv[argc])) ||
	   ( ! strcmp("--version", argv[argc])))
      *pbPrintVersion = 1;
   /* Don't parse the name of the program */
   while(--argc)
   {
      if( ! strcmp("--fat12", argv[argc]))
	 *piBr = FAT12_BR;
      else if( ! strcmp("--fat32nt", argv[argc]))
	 *piBr = FAT32NT_BR;
      else if( ! strcmp("--fat32", argv[argc]))
	 *piBr = FAT32_BR;
      else if( ! strcmp("--fat32free", argv[argc]))
	 *piBr = FAT32FD_BR;
      else if( ! strcmp("--fat16free", argv[argc]))
	 *piBr = FAT16FD_BR;
      else if( ! strcmp("--fat16", argv[argc]))
	 *piBr = FAT16_BR;
      else if( ! strcmp("--ntfs", argv[argc]))
	 *piBr = NTFS_BR;
      else if( ! strcmp("--force", argv[argc]))
	 *pbForce = 1;
      else if( ! strcmp("--wipelabel", argv[argc]))
	 *pbKeepLabel = 0;
      else if( ! strcmp("--partition", argv[argc]))
	 *pbWritePartitionInfo = 1;
      else if( ! strcmp("--mbr7", argv[argc]))
         *piBr = MBR_WIN7;
      else if( ! strcmp("--mbrvista", argv[argc]))
         *piBr = MBR_VISTA;
      else if( ! strcmp("--mbr", argv[argc]))
	 *piBr = MBR_2000;
      else if( ! strcmp("--mbr95b", argv[argc]))
	 *piBr = MBR_95B;
      else if( ! strcmp("--mbrdos", argv[argc]))
	 *piBr = MBR_DOS;
      else if( ! strcmp("--mbrsyslinux", argv[argc]))
	 *piBr = MBR_SYSLINUX;
      else if( ! strcmp("--mbrgptsyslinux", argv[argc]))
	 *piBr = MBR_GPT_SYSLINUX;
      else if( ! strcmp("--mbrzero", argv[argc]))
	 *piBr = MBR_ZERO;
      else if( ! strcmp("--write", argv[argc]))
	 *piBr = AUTO_BR;
      else if( ! strcmp("--version", argv[argc]))
	 *pbPrintVersion = 1;
      else if( (argv[argc][0] == '-') && (argv[argc][1] != '-') && !argv[argc][2])
      {
	 for(i=1; argv[argc][i]; i++)
	 {
	    switch(argv[argc][i])
	    {
	       case '1':
		  *piBr = FAT12_BR;
		  break;
	       case '2':
		  *piBr = FAT32NT_BR;
		  break;
	       case '3':
		  *piBr = FAT32_BR;
		  break;
	       case '4':
		  *piBr = FAT32FD_BR;
		  break;
	       case '5':
		  *piBr = FAT16FD_BR;
		  break;
	       case '6':
		  *piBr = FAT16_BR;
		  break;
	       case 'n':
		  *piBr = NTFS_BR;
		  break;
	       case 'f':
		  *pbForce = 1;
		  break;
	       case 'l':
		  *pbKeepLabel = 0;
		  break;
	       case 'p':
		  *pbWritePartitionInfo = 1;
		  break;
               case '7':
                  *piBr = MBR_WIN7;
                  break;
               case 'i':
                  *piBr = MBR_VISTA;
                  break;
	       case 'm':
		  *piBr = MBR_2000;
		  break;
	       case '9':
		  *piBr = MBR_95B;
		  break;
	       case 'd':
		  *piBr = MBR_DOS;
		  break;
	       case 's':
		  *piBr = MBR_SYSLINUX;
		  break;
	       case 't':
		  *piBr = MBR_GPT_SYSLINUX;
		  break;
	       case 'z':
		  *piBr = MBR_ZERO;
		  break;
	       case 'w':
		  *piBr = AUTO_BR;
		  break;
	       case 'v':
		  *pbPrintVersion = 1;
		  break;
	       default:
		  bHelp=1;
		  break;
	    }
	 }
      }
      else if((!strcmp("--heads", argv[argc-1]) || !strcmp("-H", argv[argc-1])) &&
	      isnumber(argv[argc]))
	 *piHeads = atoi(argv[argc--]);
      else
	 bHelp = 1;
   }
   return bHelp;
} /* parse_switches */

int isnumber(const char *szString)
{
   int bRet = (szString && *szString);
   while(bRet && *szString)
   {
      if((*szString < '0') || (*szString > '9'))
	 bRet = 0;
      szString++;
   }
   return bRet;
} /* isnumber */
