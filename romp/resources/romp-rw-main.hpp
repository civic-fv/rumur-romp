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

#ifdef __romp__GENERATED_CODE
#include "romp-rw.hpp"
#include "romp-rse.hpp"
#endif



/**
 * @brief 
 * @param argv 
 * @param args 
 * @return \c int - 
 */
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

#ifdef __romp__GENERATED_CODE
#include "romp-rw.hpp"
#include "romp-rse.hpp"
#endif



/**
 * @brief 
 * @param argv 
 * @param args 
 * @return \c int - 
 */
void setup_infile(char *filename)
{
  /* pre-checking on filename. */
  int len = strlen(filename);
  if (filename[len - 2] != '.' || filename[len - 1] != 'm') {
    Error.FatalError("Murphi only handles files ending in \".m\"");
  } else {
    gFileName = filename;
    yyin = fopen(filename, "r");	// yyin is flex's global for the input file
    if (yyin == NULL) {
      Error.FatalError("%s:No such file or directory.", filename);
    }
  }
}
int main(int argc, char *argv[])
{
  int error;
  args = new argclass(argc, argv);
  init_globals();//initialize all global variables to be done 
  error = yyparse();
  
}









