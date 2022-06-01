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

class Error_handler /* error-reporting mechanism. */
{
  int numerrors;
  int numwarnings;

 private:
  void vError(const char *fmt, va_list argp);
  void vWarning(const char *fmt, va_list argp);

 public:
  // initializer
  Error_handler(void);

 /* as of now printing the errors directly*/

}
extern char *gFileName;  /* file support. */
extern char *codefile;
extern char *in; 
/*---------------------------------------------------------------------------*/
/* class argclass                                                            */
/*---------------------------------------------------------------------------*/
class argclass 
{
 class argclass
{
  int argc;
  char **argv;

public:
  // variables
  char *filename;	// name of the input file
  bool print_license;   // will do for future 
  bool help;		// whether to print option
  const bool checking;	/* runtime checking? */
  int threadcount;
  bool randomwalk;
  //bool trace;//can be avoided
  int threadcount;
  
  // initializer
  argclass(int ac, char** av);

  
  void PrintInfo( void ); 
  void PrintOptions( void );
  void PrintLicense( void ); 
};
/********************
  variable declarations
  ********************/
argclass *args;
