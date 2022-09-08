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

// #include <unistd.h>
namespace romp { 
  void init_trace_dir() {
    std::string args = "mkdir -p \"" + OPTIONS.get_trace_dir() + "\"";
    int err = system(args.c_str());
    if (err) {
      std::cerr << "\nERROR :: trace directory ``" << OPTIONS.get_trace_dir() << "`` does not exists and could not be created !!\n\n" 
                << std::flush;
      exit(EXIT_FAILURE);
    }
  }
  
  void launch_prompt() {
    ostream_p out(std::cout,0);
    std::cout << '\n';
    OPTIONS.write_config(out);
    std::cout.flush();
    for (;;) {
      std::cout << "Correct Config? [yes=launch/no=exit]: " << std::flush;
      std::string _val, val;
      std::cin >> _val; val = "";
      for (auto c : _val) val += std::tolower(c);
      if ("yes" == val || "y" == val) return;
      if ("no" == val || "n" == val) { std::cout << std::endl; exit(EXIT_SUCCESS); }
      std::cout << "COULDN'T RECOGNISE RESPONSE; TRY AGAIN\n";
    }
  }
}

int main(int argc, char **argv) {

  ::romp::options::parse_args(argc, argv);

  if (not (::romp::OPTIONS.skip_launch_prompt))
    ::romp::launch_prompt();

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
