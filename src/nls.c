#include <locale.h>

#include "nls.h"

void nls_init(void)
{   
#ifndef NO_LIBINTL_OR_GETTEXT
   setlocale (LC_ALL, "");
   bindtextdomain (PACKAGE, LOCALEDIR);
   textdomain (PACKAGE);
#endif
} /* nls_init */
