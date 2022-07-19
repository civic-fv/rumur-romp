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
#include "c_prefix.cpp"
#include <functional>
#include <iostream>
#include <string>
#endif

namespace romp {
namespace options {

// input model's path
// extern std::string input_filename;

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

void print_help() {
  using namespace std;
  std::cout
      << "This is a murphi model checker generated from the romp random walker tool.\n"
         "\t\tgenerated from: "__model__filename "\n"
         "\n\n"
         "CLI usage:\n"
         "\t\t./<this-file> {options}*\n"
         "\n\n"
         "The Options are shown as follows:\n"
         "-h or --help \tto list the options in the ROMP tool\n"
         "RANDOM WALKER OPTIONS :\n"
         "-d <int>or --depth <int> \tMaximum number of times rules were tried \n"
         "to be applied. If not provided then it takes a default value \n"
         "-ptn <int> or --threads <int> \tSpecifies the thread count. If not \n"
         "provided then the default value is 4\n"
         "-sw or --single_walk \tTo perform a single random walker on a single "
         "thread. It cant be used along with -ptn\n"
         "-w <int>or --walk_count <int>\t Specifies the number of random "
         "walkers to be launched. It takes a conditional argument int.\n"
         "-s <string/int> or --seed <string/int> \t Random seed for the random "
         "walkers.It takes an optional argument of string or int for the seed "
         "and if not provided default value is considered.\n\n"
      
        "PROPERTY CONFIGURATIONS :\n" 
        "-nd or --no_deadlock \t Disable Deadlock detection \n" 
        "-lc <int> or --liveness_check <int>\t Allows you to specify the max "
        "number of successful steps a random walker can have in a row before "
        "any given liveness property must evaluate to true.\n"
        "\t\t\t\t\tIt takes an optional argument int else default value of "
         "MAX_INT is considered\n"
         "-cc <int> or --complete_on_cover <int>\tIt considers the random walk "
         "to be  complete when each statement has been reached .And takes in "
         "the optional argument int if not provided then default value is "
         "MAX_INT\n"
         "-ag <int> or --attempt_guard <int> \t It is used to mark a random "
         "walker to be in deadlock state if the walker has failed to apply any "
         "rules."
         " If optional argument of integer not provided then default value of "
         "MAX_INT is considered\n\n"
         "Trace Options\n"
         "-t <str:out_dir> or --trace <str:out_dir>\t To output the trace "
         "results of each random walkers.The optional argument <str:out-dir> "
         "specifies the directory path to where the trace files has to be "
         "written.\n"
         " If not provided then it is stored in the path <path>.\n\n"
         "Results Options\n"
         "-rhl <int> or --r_history <int> Maximum number of previous states "
         "and rules to keep track off and output in the results.\n"
         "-ra or --r_all \t Report all results not just violating\n"
         "-r_assume \t Report all violating  assumptions\n"
         "-o <str:dir_path> or --output <str:dir_path> A string containing "
         "path to a directory where the system will output the diagram files\n";
}

static void parse_args(const int argc, const char **argv) {

  for (int i = 0; i < argc; ++i) {

    if ("-h" == argv[i] ||
        "--help" ==
            argv[i]) { // to print help message and ( result of comparison
                       // against a string literal is unspecified)
      print_help();
      exit(EXIT_SUCCESS);
    } else if ("-d" == argv[i] || "--depth" == argv[i]) {
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.depth = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument& ia) {
          std::cerr << "invalid argument : provided walk/search depth was not a number (NaN) !! (for -d/--depth flag)\n" << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range& oor) {
          std::cerr << "invalid argument : provided walk/search depth was out of range (for -d/--depth flag) must be unsigned int (aka must be positive)\n" << std::flush;
          exit(EXIT_FAILURE);
        }
      } else if ("-ptn" == argv[i] || "--threads" == argv[i]) {
      if (OPTIONS.threads == 1 && OPTIONS.random_walkers == 1) {
        std::cerr << "invalid argument - Number of threads cannot be specified "
                     "while using single walk\n"
                  << std::flush;
        exit(EXIT_FAILURE);
      } else if (i + 1 < argc && '-' != argv[i + 1][0]) {
        ++i;
        try{
        OPTIONS.threads = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument& ia) {
          std::cerr << "invalid argument : provided walk/search threads was not a number (NaN) !! (for -ptn/--threads flag)\n" << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range& oor) {
          std::cerr << "invalid argument : provided walk/search threads was out of range (for -ptn/--threads flag) must be unsigned int (aka must be positive)\n" << std::flush;
          exit(EXIT_FAILURE);
        }
      } 
    } else if ("-sw" == argv[i] || "--single-walk" == argv[i]) {
      OPTIONS.do_single = true;
    } else if ("-w" == argv[i] || "--walk-count" == argv[i])f (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.random_walkers = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument& ia) {
          std::cerr << "invalid argument : provided number of random walkers was not a number (NaN) !! (for -w/--walk-count flag)\n" << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range& oor) {
          std::cerr << "invalid argument : provided number of random walkers was out of range (for -d/--depth flag) must be unsigned int (aka must be positive)\n" << std::flush;
          exit(EXIT_FAILURE);
        }
      }  else if ("-s" == argv[i] || "--seed" == argv[i]) {
      if (i + 1 < argc && '-' != argv[i+1][0]) {
        OPTIONS.seed_str = argv[i+1];
        try {
          OPTIONS.rand_seed = std::stoul(argv[i+1], nullptr, 0);
        } catch (std::invalid_argument& ia) {
          std::hash<std::string> str_hash;
          OPTIONS.rand_seed = str_hash(argv[i+1]);
        } catch (std::out_of_range& oor) {
          std::cerr << "invalid argument : provided seed was out of range (for -s/--seed flag) must be unsigned int\n" << std::flush;
          exit(EXIT_FAILURE);
        }
      } else // OPTIONS.rand_seed = time(NULL); // no need 
        std::cerr << "invalid argument : -s/--seed requires you to provide a seed after the flag\n" << std::flush;
        exit(EXIT_FAILURE);
      }
    } else if ("-nd" == argv[i] || "--no-deadlock" == argv[i]) {
      OPTIONS.deadlock = true;
    } else if ("-lc" == argv[i] || "--liveness-check" == argv[i]) {
      OPTIONS.liveness= true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.lcount = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument& ia) {
          std::cerr << "invalid argument : provided liveness count was not a number (NaN) !! (for -w/--walk-count flag)\n" << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range& oor) {
          std::cerr << "invalid argument : provided liveness count was out of range (for -d/--depth flag) must be unsigned int (aka must be positive)\n" << std::flush;
          exit(EXIT_FAILURE);
        }
      }else if ("-lc" == argv[i] || "--liveness-check" == argv[i]) {
      OPTIONS.complete_count= true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.cover_count = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument& ia) {
          std::cerr << "invalid argument : provided liveness count was not a number (NaN) !! (for -w/--walk-count flag)\n" << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range& oor) {
          std::cerr << "invalid argument : provided liveness count was out of range (for -d/--depth flag) must be unsigned int (aka must be positive)\n" << std::flush;
          exit(EXIT_FAILURE);
        }
      }else if ("-ag" == argv[i] || "--attempted-guard" == argv[i]) {
      OPTIONS.ag_count= true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.cover_count = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument& ia) {
          std::cerr << "invalid argument : provided complete on cover count was not a number (NaN) !! (for -w/--walk-count flag)\n" << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range& oor) {
          std::cerr << "invalid argument : provided complete on cover count was out of range (for -d/--depth flag) must be unsigned int (aka must be positive)\n" << std::flush;
          exit(EXIT_FAILURE);
        }
      }else if ("-rhl" == argv[i] || "--r-history" == argv[i]) {
      OPTIONS.result_history = true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.hl_count = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument& ia) {
          std::cerr << "invalid argument : provided history length count was not a number (NaN) !! (for -w/--walk-count flag)\n" << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range& oor) {
          std::cerr << "invalid argument : provided history lengthF count was out of range (for -d/--depth flag) must be unsigned int (aka must be positive)\n" << std::flush;
          exit(EXIT_FAILURE);
        }
      }
    } else if ("-ra" == argv[i] || "--result-all" == argv[i]) {
      OPTIONS.result_all = true;
    } else if ("--r-assume" == argv[i]) {
      OPTIONS.r_assume = true;
    } else if ("--output" == argv[i] || "-o" == argv[i]) {
      OPTIONS.output = true;
    }
  }
  // check for inconsistent or contradictory options here
  //TODO (OPTIONAL) check OPTIONS to make sure config is valid and output warnings and end with errors as appropriate
}

/*
else if ("-t" == argv[i] || "--trace" == argv[i]) {
  if (i+1<argc && '-' != argv[i+1][0]) {
    ++i;
    OPTIONS.trace_dir = argv[i];
  }else
    OPTIONS.trace_dir = DEFAULT;// give the default path
    // TODO function to create trace files for each walk and path to be
specified from cli
}
else if ("-s" == argv[i] || "--seed" == argv[i]) {
  if (i+1<argc && '-' != argv[i+1][0]) {
    ++i;
    OPTIONS.trace_file = argv[i];
  } else {
    std::cerr << "invalid argument!! :: -s/--seed requires a parameter to
follow, but none was found !!\n" << std::flush; exit(EXIT_FAILURE);
  }
}

case '--thread': // this matches both "--threads" & "--thread-count"
case '-ptn': { // --threads ...
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
case '':
case '-w\0\0\0\0\0\0': { // random walkers to be launched
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
    options.threads = 1; // assigning thread value to be one and also rw to be
one options.random_walkers =1;// to launch do i need to specify
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
  else if (strcmp(optarg, "output") == 0) { // todo must take an argument for
directory ?? is this okay options.result|= TC_OUTPUT; } else { std::cerr <<
"invalid --result argument \"" << optarg << "\"\n"
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
}
} // namespace options
} // namespace romp
*/