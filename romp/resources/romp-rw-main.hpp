/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp-main.hpp
 * 
 * @brief Just where the main function is.
 *        If anything else exists in here, it should only be called in here and nowhere else.
 * 
 * @date 2022/05/11
 * @version 0.1
 */


#include <string.h>
#include <new>
#include <cstdlib>


#ifndef __romp__GENERATED_CODE
#include "c_prifix.cpp"
#include "romp-rw.hpp"
// #include "romp-rse.hpp"
#include "romp-rw-options.hpp"
#endif

namespace {

using namespace std;

void init_globals()
{ 
  /*** must add globals as and when required*/
}

/* setup input file handler*/
void setup_infile(char *filename)
{
  
  int len = strlen(filename);
  if ( filename[len - 2] != '.' ||
       filename[len - 1] != 'm' )
    {
      printf("Murphi only handles files ending in \".m\"");
    }
  else
    {
      gFileName = filename;
      in = fopen(filename, "r"); // yyin is flex\'s global for the input file
      if ( (fopen(filename, "r")) == NULL )
	{
	  printf("%s:No such file or directory.", filename);
	}
    }
}

/********************
  FILE *setup_outfile(char *infilename)
  -- setup output file handler
  ********************/
FILE *setup_outfile(char *infilename)
{
  char outfilename[128];
  FILE *outfile;
  int len = strlen(infilename);

  strcpy( outfilename, infilename);
  outfilename[len - 1] = 'C';
  outfile = fopen(outfilename, "w");
  if ( outfile == NULL )
    {
      Error.FatalError("Unable to open/create %s",outfilename);
    };
  return outfile;
}

/********************
  class argclass
  ********************/
bool Sw(char* a, int ac, char** av)
{
  for (int i=1; i<ac; i++)
    if (strcmp(av[i], a) == 0)
      return TRUE;
  return FALSE;
}   

argclass::argclass(int ac, char** av)
:argc(ac), argv(av), print_license(FALSE), help(FALSE), checking(TRUE),
 randomwalk(TRUE)
{
  bool initialized_filename = FALSE;
  int i;

  if (ac==1) {  // if there is no argument, print help
    help = TRUE;
    PrintInfo();
    exit(1);
  }
  for (i=1; i<ac; i++) 
    {
      if (strcmp(av[i], "-l") == 0) {
	print_license = TRUE;
	continue;
      }  
      if (strcmp(av[i], "-h") == 0) {
	help = TRUE;
	PrintInfo();
	exit(1);
      } 
      if ( av[i][0]!='-' && !initialized_filename)
	{
	  initialized_filename = TRUE;
	  filename = av[i];
	  continue;
	}
      else if ( av[i][0]!='-' && initialized_filename)
	{
	  fprintf(stderr, "Duplicated input filename.\n");
	  exit(1);
	}
      fprintf(stderr, "Unrecognized flag. Do '%s -h' for a list of valid arguments.\n",
	      av[0]);
      exit(1);
    }

  if ( !initialized_filename ) { // print help
    help = TRUE;
    PrintInfo();
    exit(0);
  }

  PrintInfo();
} 
void argclass::PrintInfo( void )
{
  if (print_license) PrintLicense();

  if (!print_license)
    {
      printf("Call with the -l flag or read the license file for terms\n");
      printf("and conditions of use.\n");
    }

  if (!help)
    printf("Run this program with \"-h\" for the list of options.\n");

  if (help) PrintOptions();
  fflush(stdout);
}

void argclass::PrintOptions( void )
{
  printf("The options are shown as follows:\n\
\n\
\t-h    \t\t   \thelp\n\
\t-l    \t\t   \tprint license\n\
\t-w    \t\t   \tuse randomwalk\n\
\n\
An argument without a leading '-' is taken to be the input filename,\n\
\twhich must end with '.m'\n\
");
}

void argclass::PrintLicense( void )
{
  printf("License Notice: \n\n");
  printf("...............");
  fflush(stdout);
}

}
/********************
  main routines
  ********************/
int main(int argc, char *argv[])
{
  int error;
  //print_header(); not required
  args = new argclass(argc, argv);

  init_globals();//to initialise global variables 
  setup_infile(args->filename);
  error = yyparse();
  if (!error && Error.getnumerrors() == 0) {
    codefile = setup_outfile(args->filename);
    theprog->generate_code();
    fclose(codefile);
    }
}

