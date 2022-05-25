/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file options.hpp
 * 
 * @brief Where the command line option parsing is going to go.
 * 
 * @date 2022/05/18
 * @version 0.1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>

#define __FILE_defined
struct _IO_FILE;
typedef struct _IO_FILE FILE;


/* Extern Variables */

extern argclass *args;
extern char *gFileName;		/* file support. */
extern FILE *codefile;

extern int yyparse(void);	
class argclass 
{
  int argc;
  char **argv;
public:
  // variables
  

  // initializer
   argclass(int ac, char **av);

  // supporting routines
  void PrintOptions(void);
}


argclass::argclass(int ac, char **av)
:
argc(ac), argv(av), print_license(FALSE), help(FALSE)
{
  bool initialized_filename = FALSE;
  int i;

  if (ac == 1) {		// if there is no argument, print help
    help = TRUE;
    PrintInfo();
    exit(1);
  }

  if (strcmp(av[i], "-h") == 0) {
      help = TRUE;
      PrintInfo();
      exit(1);
    }

void argclass::PrintInfo(void)
{
  
  if (!help)
    printf("Run this program with \"-h\" for the list of options.\n");

  if (help)
    PrintOptions();
  fflush(stdout);
}

void argclass::PrintOptions(void)
{
  printf("The options are shown as follows:\n\
----- to be filled------------");
}