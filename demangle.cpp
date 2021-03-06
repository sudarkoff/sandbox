#include <windows.h>
#include <stdio.h>
#include <imagehlp.h>

int main(int argc, char* argv[])
{
   if (argc == 1)
      printf ("\nUndecorates Visual C++ mangled names. "
              "\nUsage: demangle <decorated_name> [[<decorated_name>] ...] \n\n");

   char *undecorated_name = new char[513];
   memset(undecorated_name, 0, 513);
   char *decorated_name = new char[513];

   int i = 1;

   while (i < argc) {
      strcpy(decorated_name, argv[i]);

      if (UnDecorateSymbolName(decorated_name, undecorated_name, 513, UNDNAME_COMPLETE) > 0) 
         printf ("%s\n", undecorated_name);
      else
         printf ("Unknown\n");

      i++;
   }

   delete decorated_name;
   delete undecorated_name;

   return(0);
}
