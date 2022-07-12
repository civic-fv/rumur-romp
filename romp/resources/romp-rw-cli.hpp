#include<iostream>
#include <algorithm>
#include <unistd.h> //parse getopt
/*#include <cassert> 
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <memory>*/

#define no_argument    0
#define required_argument  1
#define	EXIT_FAILURE	1
#define	EXIT_SUCCESS	0
using namespace romp;


void print_help()
{
  printf("Run the ROMP model checker with the options are shown as follows:\n\
\n\
\t-h    \t\t   \thelp\n\
\t-d    \t\t   \tdepth  Set a limit for state space exploration .Also, the verifier  will  stop  checking  beyond
              this  depth.\n\
\t-t    \t\t   \tnumber of threads the verifier should use. If you do not  specify  this
              parameter  or  pass  0, the number of threads will be chosen based on the available
              hardware threads on the platform on which you generate the model.\n\
\t-w    \t\t   \tnumber of random walkers to be launched by the verifier\n\
\t-n    \t\t   \tno deadlock detection\n\
\t-s    \t\t   \trandom seed to be fed into the random walker. If not provided, default seed is considered.\n");
}
static void parse_args(int argc, char **argv) {

  for (;;) // as an infinite loop consider changing to finite loop with argc as check ??
    enum {
      OPT_DEPTH = 100,
      //OPT_NO_DEADLOCK_DETECTION,
      OPT_TRACE,
    };

    static struct option opts[] = {
        {"depth", required_argument, 0, 'd'}, // whats the point of 0 in here ?
        {"nodeadlock-detection", required_argument, 0, 'ndl'}, // not required of args right ??
        {"help", no_argument, 0, 'h'},
        {"threads", required_argument, 0, 't'},
        {"rand_walker", required_argument, 0, 'w'},
        {"rand_seed", required_argument, 0, 's'},
        {"trace", required_argument, 0, OPT_TRACE},//how to do the trace like trace with options ???
        {0, 0, 0, 0},// for no args kind 
    };

int option_index = 0;
int c = getopt_long(argc, argv, "hndlt:w:s:", opts, &option_index); // compund letters args ?? also not for trace//depth ??
extern char *optarg;		

    if (c == -1)
      break;

    switch (c) {

    case 'h': // --help
      print_help();
      exit(EXIT_SUCCESS);
      break;

    case 'ndl': // --no-deadlock-detection ...
      //define ndl;
      exit(EXIT_FAILURE);
      break;

    case 't': { // --threads ...
      bool valid = true;
      try {
        options.threads = optarg;
        if (options.threads < 0) //any other cond to be added ??
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
    case 's': { //  random seed for the random walkers 
      bool valid = true;
      try {
        options.rand_seed = optarg;
        if (options.rand_seed < 0)  // any other cond for rand seed 
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
      } 
      else {
        std::cerr << "invalid --trace argument \"" << optarg << "\"\n"
                  << "valid arguments are \"verbose\", \"tripped_state\", "
                     "\"rw_path\",\n";
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
int main(int argc, char **argv) {

  /**
   * @brief Construct a new parse args object
   * 
   */
  parse_args(argc, argv);

 // Parse input model
  // TODO shit to figure out ??
  return EXIT_SUCCESS;
}
