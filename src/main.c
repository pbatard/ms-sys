#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "fat12.h"
#include "fat32.h"
#include "br.h"
#include "identify.h"
#include "nls.h"

#define VERSION "0.9 stable"

void print_help(const char *szCommand);
void print_version(void);
int parse_switches(int argc, char **argv, int *piBr,
		   int *pbForce, int *pbPrintVersion);

int main(int argc, char **argv)
{
   FILE *fp;
   int iBr = NO_WRITING;
   int bForce = 0;
   int bPrintVersion = 0;
   int iRet = 0;

   nls_init();
   if(parse_switches(argc, argv, &iBr, &bForce, &bPrintVersion))
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
   fp=fopen(argv[argc-1], iBr ? "r+b" : "rb");
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
   switch(iBr)
   {
      case NO_WRITING:
	 diagnose(fp, argv[argc-1]);
	 break;
      case MBR:
      {
	 if(write_mbr(fp))
	    printf(_("master boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing master boot record to %s\n"),
		   argv[argc-1]);
	    iRet = 1;
	 }	    
      }
      break;
      case FAT12_BR:
      {
	 if(write_fat_12_br(fp))
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
      case FAT32_BR:
      {
	 if(write_fat_32_br(fp))
	    printf(_("FAT32 boot record successfully written to %s\n"),
		   argv[argc-1]);
	 else
	 {
	    printf(_("Failed writing FAT32 boot record to %s\n"),
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
     _("    -3, --fat32     Write a FAT32 partition boot record to device\n"));
   printf(_("    -f, --force     Force writing of boot record\n"));
   printf(_("    -h, --help      Display this help and exit\n"));
   printf(_("    -m, --mbr       Write a master boot record to device\n"));
   printf(_("    -v, --version   Show program version\n"));
   printf(
_("    -w, --write     Write automaticly selected boot record to device\n\n"));
   printf(
      _("Warning: Writing the wrong kind of boot record to a device might\n"));
   printf(
      _("destroy partition information or file system!\n\n"));
} /* print_help */

void print_version(void)
{
   printf(_("ms-sys version %s\n"), VERSION);
   printf(_("Written by Henrik Carlqvist\n\n"));
   printf(_("Copyright (C) 2002 Free Software Foundation, Inc.\n"));
   printf(_("This is free software; see the source for copying conditions.  There is NO\n"));
   printf(_("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n"));
} /* print_version */

int parse_switches(int argc, char **argv, int *piBr,
		   int *pbForce, int *pbPrintVersion)
{
   int bHelp = 0;

   *piBr = NO_WRITING;
   *pbForce = 0;
   *pbPrintVersion=0;
   
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
      if(( ! strcmp("-1", argv[argc])) || ( ! strcmp("--fat12", argv[argc])))
	 *piBr = FAT12_BR;
      else if(( ! strcmp("-3", argv[argc])) ||
	      ( ! strcmp("--fat32", argv[argc])))
	 *piBr = FAT32_BR;
      else if(( ! strcmp("-f", argv[argc])) ||
	      ( ! strcmp("--force", argv[argc])))
	 *pbForce = 1;
      else if(( ! strcmp("-m", argv[argc])) ||
	      ( ! strcmp("--mbr", argv[argc])))
	 *piBr = MBR;
      else if(( ! strcmp("-w", argv[argc])) ||
	      ( ! strcmp("--write", argv[argc])))
	 *piBr = AUTO_BR;
      else if(( ! strcmp("-v", argv[argc])) ||
	      ( ! strcmp("--version", argv[argc])))
	 *pbPrintVersion = 1;
      else
	 bHelp = 1;
   }
   return bHelp;
} /* parse_switches */

