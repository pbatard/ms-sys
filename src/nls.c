#include <locale.h>

#include "nls.h"

void nls_init(void)
{   
   setlocale (LC_ALL, "");
   bindtextdomain (PACKAGE, LOCALEDIR);
   textdomain (PACKAGE);
} /* nls_init */
