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
\t-s    \t\t   \trandom seed to be fed into the random walker. If not provided, default seed is considered.\n");
}
static void parse_args(int argc, char **argv) {

  for (;;) // as an infinite loop consider changing to finite loop with argc as check ??
    enum {
      OPT_DEPTH = 100,
      OPT_NO_DEADLOCK_DETECTION,
      OPT_TRACE,
      OPT_THREAD,
      OPT_RANDOMWALKERS,
      OPT_RANDOMSEED
    };

    static struct option opts[] = {
        {"depth", required_argument, 0, OPT_}, // whats the point of 0 in here ?
        {"nodeadlock-detection", required_argument, 0, 'd'}, // not required of args right ??
        {"help", no_argument, 0, 'h'},
        {"threads", required_argument, 0, 't'},
        {"rand_walker", required_argument, 0, 'w'},
        {"rand_seed", required_argument, 0, 's'},
        {"trace", required_argument, 0, OPT_TRACE},//how to do the trace like trace with options ???
        {0, 0, 0, 0},// for no args kind 
    };

int option_index = 0;
int c = getopt_long(argc, argv, "hdt:w:s:", opts, &option_index);

    if (c == -1)
      break;

    switch (c) {

    case 'h': // --help
      print_help();
      exit(EXIT_SUCCESS);

    case 't': { // --threads ...
      bool valid = true;
      try {
        options.threads = optarg;
        if (options.threads < 0)
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
case 't': { // --threads ...
      bool valid = true;
      try {
        options.threads = optarg;
        if (options.threads < 0)
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
case 't': { // --threads ...
      bool valid = true;
      try {
        options.threads = optarg;
        if (options.threads < 0)
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
    case 'v': // --verbose
      options.log_level = LogLevel::INFO;
      set_log_level(options.log_level);
      break;

    case '?':
      std::cerr << "run `" << argv[0] << " --help` to see available options\n";
      exit(EXIT_FAILURE);

    case OPT_TRACE: // --trace ...
      if (strcmp(optarg, "handle_reads") == 0) {
        options.traces |= TC_HANDLE_READS;
      } else if (strcmp(optarg, "handle_writes") == 0) {
        options.traces |= TC_HANDLE_WRITES;
      } else if (strcmp(optarg, "memory_usage") == 0) {
        options.traces |= TC_MEMORY_USAGE;
      } else if (strcmp(optarg, "queue") == 0) {
        options.traces |= TC_QUEUE;
      } else if (strcmp(optarg, "set") == 0) {
        options.traces |= TC_SET;
      } else if (strcmp(optarg, "symmetry_reduction") == 0) {
        options.traces |= TC_SYMMETRY_REDUCTION;
      } else if (strcmp(optarg, "all") == 0) {
        options.traces = uint64_t(-1);
      } else {
        std::cerr << "invalid --trace argument \"" << optarg << "\"\n"
                  << "valid arguments are \"handle_reads\", \"handle_writes\", "
                     "\"memory_usage\", \"queue\", \"set\", and "
                     "\"symmetry_reduction\"\n";
        exit(EXIT_FAILURE);
      }
      break;

    case OPT_DEADLOCK_DETECTION: // --deadlock-detection ...
      if (strcmp(optarg, "off") == 0) {
        options.deadlock_detection = DeadlockDetection::OFF;
      } else if (strcmp(optarg, "stuck") == 0) {
        options.deadlock_detection = DeadlockDetection::STUCK;
      } else if (strcmp(optarg, "stuttering") == 0) {
        options.deadlock_detection = DeadlockDetection::STUTTERING;
      } else {
        std::cerr << "invalid argument to --deadlock-detection, \"" << optarg
                  << "\"\n";
        exit(EXIT_FAILURE);
      }
      break;

    case OPT_BOUND: { // --bound ...
      bool valid = true;
      try {
        options.bound = optarg;
        if (options.bound < 0)
          valid = false;
      } catch (std::invalid_argument &) {
        valid = false;
      }
      if (!valid) {
        std::cerr << "invalid --bound argument \"" << optarg << "\"\n";
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

 