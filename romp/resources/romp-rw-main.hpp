/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp-rw-main.hpp
 * 
 * @brief 
 * 
 * @date 2022/07/12
 * @version 0.1
 */


#ifndef __romp__GENERATED_CODE
#include "romp-rw.hpp"  // LANGUAGE SERVER SUPPORT ONLY !!
#include "romp-rw-options.hpp" // LANGUAGE SERVER SUPPORT ONLY !!
#endif

int main(int argc, char **argv) {

  ::romp::options::parse_args(argc, argv);

  if (::romp::OPTIONS.do_single)
    ::romp::launch_single();

  else
    ::romp::launch_OpenMP();


  return EXIT_SUCCESS;
}

//TODO add in signal intercept for kill/Ctrl-C to gracefully kill threads & output traces etc.
