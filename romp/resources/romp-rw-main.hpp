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

#include <unistd.h>
namespace romp { 
  void init_trace_dir() {
    std::string args = "mkdir -p \"" + OPTIONS.trace_dir + "\"";
    system(args.c_str());
  //   try {
  //     std::ofstream test(OPTIONS.trace_dir + "/dir-test.tmp" );
  //     if (test.fail()) {
  //       std::string args = "mkdir -p " + OPTIONS.trace_dir;
  //       system(args.c_str());
  //     }
  //   } catch (...) {
  //     std::cerr << "\nERROR : Failed to verify trace dir exists: ``"<< OPTIONS.trace_dir << "`` \t[dev-error]\n";
  //   }
  // }
}
}

int main(int argc, char **argv) {

  ::romp::options::parse_args(argc, argv);

  if (::romp::OPTIONS.do_trace)
    ::romp::init_trace_dir();

  if (::romp::OPTIONS.do_single)
    ::romp::launch_single(::romp::OPTIONS.rand_seed);

  else
    ::romp::launch_threads(::romp::OPTIONS.rand_seed);
    // ::romp::launch_OpenMP(::romp::OPTIONS.rand_seed);


  return EXIT_SUCCESS;
}

//TODO add in signal intercept for kill/Ctrl-C to gracefully kill threads & output traces etc.
