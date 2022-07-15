/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp-rw-options.hpp
 *
 * @brief
 *
 * @date 2022/07/12
 * @version 0.1
 */

#ifndef __romp__GENERATED_CODE
#pragma once
#endif
#include <cstddef>
#include <cstdint>
#include <gmpxx.h>
#include <string>
#include <vector>
#include <ctime>
#include <unistd.h> //parse getopt

namespace romp {
namespace options {
enum trace_category_t { // to do for trace
  TC_VERBOSE, TC_TRIPPED_STATES, TC_RW_PATH
};
enum property_category_t // to do define 
 {  TC_NODEADLOCK, TC_LIVENESS, TC_COVER , TC_ATTEMPT_GAURD
};
enum result_category_t{
    TC_HISTORY, TC_ALL, TC_ASSUMPTIONS, TC_OUTPUT
};

struct Options {
  size_t threads = 0;    // mpz_class ??
  size_t depth = 0;      // what has to be default value  ??
  size_t random_walkers; // any default value needed if not provided ?
  unsigned int rand_seed = time(NULL); // the random seed to se (defaults to current system time)
  std::string seed_str;
  /*
  define for properties and path needs to be done 
  */
};

Options options;


// input model's path
// extern std::string input_filename;

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

void print_help() {
  printf("Run the ROMP model checker with the options are shown as follows:\n\
\n\
\t-h    \t\t   \thelp\n\
\t-d    \t\t   \tdepth  Set a limit for state space exploration .Also, the verifier  will  stop  checking  beyond
              this  depth.\n\
\t-t    \t\t   \tnumber of threads the verifier should use. If you do not  specify  this
              parameter  or  pass  0, the number of threads will be chosen based on the available
              hardware threads on the platform on which you generate the model.\n\
\t-w    \t\t   \tnumber of random walkers to be launched by the verifier\n\
\t-u    \t\t   \tsingle random walker\n\
\t-s    \t\t   \trandom seed to be fed into the random walker. If not provided, default seed is considered.\n");
}
static void parse_args(int argc, char **argv) {

  //for (;;) // as an infinite loop consider changing to finite loop with argc as
           // check ??
    enum {
      OPT_DEPTH = 128,
      // OPT_NO_DEADLOCK_DETECTION,
      OPT_TRACE,
      OPT_PROPERTY,
      OPT_RESULT
    };

  static struct options opts[] = {
      {"depth", required_argument, 0, 'd'}, // whats the point of 0 in here ?
      //{"nodeadlock-detection", required_argument, 0, 'ndl'}, // not required of args right ??
      {"help", no_argument, 0, 'h'},
      {"threads", required_argument, 0, 't'},
      {"rand-walker", required_argument, 0, 'w'},
      {"rand-seed", required_argument, 0, 's'},
      {"trace", optional_argument, 0, 't'},  
      //{"trace-level", required_argument, 0, 'tl'},  // how to do the trace like trace with options ???
      {0, 0, 0, 0}, // for no args kind
  };

  int option_index = 0;
  int c = getopt_long(
      argc, argv, "hud:t:w:s:", opts,
      &option_index); // compund letters args ?? also not for trace//depth ??
  extern char *optarg;

  if (c == -1)
    break;

  switch (c) {

  case 'h': // --help
    print_help();
    exit(EXIT_SUCCESS);
    break;

  /*case 'ndl': // --no-deadlock-detection ...
    // define ndl;
    exit(EXIT_FAILURE);
    break;

  */

  case 't': { // --threads ...
    bool valid = true;
    try {
      options.threads = optarg;
      if (options.threads < 0) // any other cond to be added ??
        valid = false;
    } catch (std::invalid_argument &) {
      valid = false;
    }
    if (!valid) {
      std::cerr << "invalid --threads argument \"" << optarg << "\"\n";
      exit(EXIT_FAILURE);
    }
    break;
  }
  case 'w': { // random walkers to be launched
    bool valid = true;
    try {
      options.random_walkers = optarg;
      if (options.random_walkers < 0) // any other cond for rand walkers ??
        valid = false;
    } catch (std::invalid_argument &) {
      valid = false;
    }
    if (!valid) {
      std::cerr << "invalid --random walkers argument \"" << optarg << "\"\n";
      exit(EXIT_FAILURE);
    }
    break;
  }

  case 'u': { // --single random walker in a thread 
      options.threads = 1; // assigning thread value to be one and also rw to be one 
      options.random_walkers =1;// to launch do i need to specify 
      exit(EXIT_SUCCESS); 
    break;
  }

  case 's': { //  random seed for the random walkers
    bool valid = true;
    try {
      options.rand_seed = optarg;
      if (options.rand_seed < 0) // any other cond for rand seed
        valid = false;
    } catch (std::invalid_argument &) {
      valid = false;
    }
    if (!valid) {
      std::cerr << "invalid --random seed argument \"" << optarg << "\"\n";
      exit(EXIT_FAILURE);
    }
    break;
  }

  case '?':
    std::cerr << "run `" << argv[0] << " --help` to see available options\n";
    exit(EXIT_FAILURE);

  case OPT_TRACE: // --trace ...
    if (strcmp(optarg, "verbose") == 0) {
      options.traces |= TC_VERBOSE;
    } else if (strcmp(optarg, "tripped_state") == 0) {
      options.traces |= TC_TRIPPED_STATES;
    } else if (strcmp(optarg, "rw_path") == 0) {
      options.traces |= TC_RW_PATH;
    } else {
      std::cerr << "invalid --trace argument \"" << optarg << "\"\n"
                << "valid arguments are \"verbose\", \"tripped_state\", "
                   "\"rw_path\",\n";
      exit(EXIT_FAILURE);
    }
    break;

    case OPT_PROPERTY: // --property ...
    if (strcmp(optarg, "nodeadlock") == 0) {
      options.property |= TC_NODEADLOCK;
    } else if (strcmp(optarg, "liveness") == 0) {
      options.property |= TC_LIVENESS;
    } else if (strcmp(optarg, "cover") == 0) {
      options.property |= TC_COVER;
    } 
    else if (strcmp(optarg, "attempt_gaurd") == 0) {
      options.property |= TC_ATTEMPT_GAURD;
    } else {
      std::cerr << "invalid --property argument \"" << optarg << "\"\n"
                << "valid arguments are \"nodeadlock\", \"liveness\", "
                   "\"cover\",\"attempt_gaurd\"\n";
      exit(EXIT_FAILURE);
    }
    break;

    case OPT_RESULT: // --result ...
    if (strcmp(optarg, "history") == 0) {
      options.result|= TC_HISTORY;
    } else if (strcmp(optarg, "all") == 0) {
      options.result |= TC_ALL;
    } else if (strcmp(optarg, "assume") == 0) {
      options.result|= TC_ASSUMPTIONS;
    } 
    else if (strcmp(optarg, "output") == 0) { // todo must take an argument for directory ?? is this okay 
      options.result|= TC_OUTPUT;
    } else {
      std::cerr << "invalid --result argument \"" << optarg << "\"\n"
                << "valid arguments are \"history\", \"all\", "
                   "\"assume\",\"output\"\n";
      exit(EXIT_FAILURE);
    }
    break;


  case OPT_DEPTH: { // --depth ...
    bool valid = true;
    try {
      options.depth = optarg;
      if (options.depth < 0 && >= INT32_MAX)
        valid = false;
    } catch (std::invalid_argument &) {
      valid = false;
    }
    if (!valid) {
      std::cerr << "invalid --depth argument \"" << optarg << "\"\n";
      exit(EXIT_FAILURE);
    }
    break;
  }

  default:
    std::cerr << "unexpected error\n";
    exit(EXIT_FAILURE);
  }
}
} // namespace options
} // namespace romp
