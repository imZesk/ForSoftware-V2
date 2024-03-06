#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "textExecution.h"
bool resolveThenExecute(char *input)
{
   char *verb = strtok(input, " \n");
   char *noun = strtok(NULL, " \n");
   if (verb != NULL)
   {
      if (strcmp(verb, "salir") == 0)
      {
         return false;
      }
      else if (strcmp(verb, "mirar") == 0)
      {
         printf("Esta demasiado oscuro\n");
      }
      else if (strcmp(verb, "ir") == 0)
      {
         printf("Esta demasiado oscuro para ir\n");
      }
      else
      {
         printf("No entiendo que es '%s'.\n", verb);
      }
   }
   return true;
}