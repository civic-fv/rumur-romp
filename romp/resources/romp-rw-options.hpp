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
  std::cout << "This is a murphi model checker generated from the romp random walker tool.\n"
               "   generated from: " __model__filename "\n"
               "\n\n"
               "CLI USAGE:    <> - required  {} - optional\n"
               "   ./<this-file> {options}*\n"
               "\n\n"
               "GENERAL OPTIONS:\n"
               "   -h | --help      Display help information (this page).\n"
               "   -y | -l          Launch without prompting (skip launch prompt).\n"
               "   --list-starts    Output a list of all startstates & their id's.\n"
               "     | -ls            (see --start-id/-sid for more info)\n"
               "\n"
               "RANDOM WALKER OPTIONS:\n"
               "  --depth <int>     Maximum number of rules to apply in a walk \n"
               "    | -d <int>        before termination.\n"
               "                      default: " << OPTIONS.depth << "\n"
               "  --threads <int>   Specifies the number of threads to use.\n"
               "    | -ptn <int>      default: " << OPTIONS.threads << "\n"
               "  --walks <int>     Specifies the number of random  walks to do.\n"
               "     | -w <int>       default: " << _ROMP_THREAD_TO_RW_RATIO << " * #-of-threads\n"
               "  --single-walk     Perform a single random walker on a single thread.\n"
               "     | -sw            note: overrides -ptn/--threads \n"
               "                              & -w/--walks/--walk-count\n"
               "  --seed <str/int>  Random seed for generating the random walks\n"
               "    | -s <str/int>    default: current system time\n"
               "  --even-start      Determine startstate for even distribution\n"
               "    | -es             rather than random choice (default behavior).\n"
               "  --start-id <id>   Set a single startstate to use (For all walks).\n"  
               "    | -sid <id>       <id> is an int determined by a startstate's\n"
               "                      position in the file after ruleset expansion.\n"
               "                      NOTE: overrides --even-start/-es\n"
               "\n"
               "PROPERTY CONFIGURATIONS :\n"
               "  --no-deadlock           Disable Deadlock detection.\n"
               "    | -nd                   (overrides all property options below)\n"
#ifdef __romp__ENABLE_liveness_property
               "  --liveness-check {int}  Allows you to enable a heuristic test for\n"
               "    | -lc {int}             rumur's liveness property. Where it \n"
               "                            require the properties expr to resolve\n"
               "                            to true with in {int} rule applications\n"
               "                            since the start or the last time it was\n"
               "                            true."
               "                            {int} - an optional arg (see above).\n"
               "                              default: " << OPTIONS.lcount << "\n"
#endif
#ifdef __romp__ENABLE_cover_property
               "  --complete-on-cover {int} Allows you to enable a heuristic test"
               "    | --cover-check {int}     for rumur's cover property. Where it\n"
               "    | -cc {int}               considers a walk complete once every\n"
               "                              cover property has been reached {int}\n"
               "                              times.\n"
               "                              {int} - an optional arg (see above).\n"
               "                                default: " << OPTIONS.cover_count << "\n"
#endif
               "  --attempt-guard {int}   Stop a random walk if no randomly \n"
               "    | -ag {int}             selected rule can be applied to a State\n"
               "                            after {int} many rules in a row.\n"
               "                            {int} - an optional arg (see above).\n"
               "                                default: #-of-rules in the model\n"
               "  --loop-limit {int}      Same thing as --attempt-guard/-ag. \n"
               "    | -ll {int}             {int} - an optional arg (see above).\n"
               "                                default: #-of-rules in the model\n"
               "\n"
               "Trace Options\t\n"
               "  --trace {dir-path}    Enable detailed traces to be made for every\n"
               "    | -t {dir-path}       random walk performed. Outputs as a \n"
               "                          separate ``compressed json'' file for \n"
               "                          each random walk performed.\n"
               "                          NOTE: slows down the process noticeably!\n"
               "                          {dir-path} - (optional) the directory you\n"
               "                            want to have the trace files output to.\n"
               "                            default: \""<< OPTIONS.trace_dir <<"\"\n"
               "\n"
               "RESULT OUTPUT OPTIONS:\n"
               "  --r-history <int>     Specify how much of a history of rules\n"
               "    | -rhl <int>          applied you want to see in the results."
               "                          <int> - (required) size of history buffer\n"
               "                            default: " << OPTIONS.history_length << "\n"
              //  "                          NOTE: larger the size == more RAM used.\n"
               "  --report-all          Report on all walks not just those with \n"
               "    | --all              ``errors''. Including those stopped by \n"
               "    | -a                  the --attempt-guard/-ag/--loop-limit/-ll\n"
               "                          option, or max --depth/-d being reached.\n"
#ifdef __romp__ENABLE_assume_property
               "                          This also performs --report-assume/-ra.\n"
               "  --report-assume       Report walks that are violate an assume\n"
               "    | -ra                 property (defined by rumur), rather than\n"
               "                          just discarding them as invalid states.\n"
#endif
              //  "  --output <file-path>  Specify a file you wish to output to\n"
              //  "    | -o <file-path>      instead of to stdout.\n"
               "\n"
            << std::endl;
}

std::string validate_dir_path(const std::string val) {
  auto start = val.begin();
  auto end = --(val.end());
  while (start != val.end() && isspace(*start))
    ++start;
  while (end != start && isspace(*end))
    --end;
  if (start == end) {
    std::cerr << "invalid argument : ``" << val << "`` is not a valid file/dir path !!\n" << std::flush;
    exit(EXIT_FAILURE);
  }
  if (*end != '/' && *end != '\\')
    return std::string(start, end) + '/';
  return std::string(start, end);
}

void list_starts() {
  std::cout << "\nStartState List (w/ id's)\n";
  for (size_t i=0; i<_ROMP_STARTSTATES_LEN; ++i)
    std::cout << "  (" << i << ") " << ::__caller__::STARTSTATES[i] << "\n";
  std::cout << std::flush;
}

void parse_args(int argc, char **argv) {
  bool threads_provided = false;
  bool walks_provided = false;
  bool start_provided = false;

  for (int i = 0; i < argc; ++i) {

    if ("-h" == std::string(argv[i]) || "--help" == std::string(argv[i])) { // to print help message and ( result of comparison
                                                  // against a string literal is unspecified)
      print_help();
      exit(EXIT_SUCCESS);
    } else if ("-l" == std::string(argv[i]) || "-y" == std::string(argv[i])) {
      OPTIONS.skip_launch_prompt = true;
    } else if ("-es" == std::string(argv[i]) || "--even-start" == std::string(argv[i])) {
      OPTIONS.do_even_start = true;
    } else if ("-ls" == std::string(argv[i]) || "--list-starts" == std::string(argv[i])) {
      list_starts();
      exit(EXIT_SUCCESS);
    }
     else if ("-sid" == std::string(argv[i]) || "--start-id" == std::string(argv[i])) {
      start_provided = true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { 
        ++i;
        try {
          OPTIONS.start_id = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument &ia) {
          std::cerr << "invalid argument : provided walk/search depth was not a number (NaN) !! (for -sid/--start-id flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range &oor) {
          std::cerr << "invalid argument : provided walk/search depth was out of range (for --sid/--start-id flag) must be "
                       "unsigned int (aka must be positive)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        }
      } else {
        std::cerr << "invalid argument : -sid/--start-id requires you to provide a value immediately after (positive integer),\n"
                     "                      else exclude it to use default value: "
                  <<OPTIONS.start_id<< std::endl;
        exit(EXIT_FAILURE);
      }
    }
     else if ("-d" == std::string(argv[i]) || "--depth" == std::string(argv[i])) {
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.depth = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument &ia) {
          std::cerr << "invalid argument : provided walk/search depth was not a number (NaN) !! (for -d/--depth flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range &oor) {
          std::cerr << "invalid argument : provided walk/search depth was out of range (for -d/--depth flag) must be "
                       "unsigned int (aka must be positive)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        }
      } else {
        std::cerr << "invalid argument : -d/--depth requires you to provide a value immediately after (positive integer),\n"
                     "                      else exclude it to use default value: "
                  << ((OPTIONS.depth == INT32_MAX) ? "INT32_MAX" : std::to_string(OPTIONS.depth)) << std::endl;
        exit(EXIT_FAILURE);
      }
    } else if ("-ptn" == std::string(argv[i]) || "--threads" == std::string(argv[i])) {
      threads_provided = true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) {
        ++i;
        try {
          OPTIONS.threads = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument &ia) {
          std::cerr << "invalid argument : provided walk/search threads was not a number (NaN) !! (for -ptn/--threads flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range &oor) {
          std::cerr << "invalid argument : provided walk/search threads was out of range (for -ptn/--threads flag) must be "
                       "unsigned int (aka must be positive)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        }
      } else {
        std::cerr
            << "invalid argument : -ptn/--threads requires you to provide a value immediately after (positive integer),\n"
               "                      else exclude it to use default value: "
            << OPTIONS.threads << std::endl;
        exit(EXIT_FAILURE);
      }
    } else if ("-sw" == std::string(argv[i]) || "--single-walk" == std::string(argv[i])) {
      OPTIONS.do_single = true;
    } else if ("-w" == std::string(argv[i]) || "--walks" == std::string(argv[i]) || "--walk-count" == std::string(argv[i])) {
      walks_provided = true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.random_walkers = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument &ia) {
          std::cerr << "invalid argument : provided number of random walkers was not a number (NaN) !! (for "
                       "-w/--walk-count flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range &oor) {
          std::cerr << "invalid argument : provided number of random walkers was out of range "
                       "(for -w/--walks/--walk-count flag) must "
                       "be unsigned int (aka must be positive)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        }
      } else {
        std::cerr << "invalid argument : -w/--walks requires you to provide a value immediately after (positive integer),\n"
                     "                      else exclude it to use default value (default: "
                  << _ROMP_THREAD_TO_RW_RATIO << " x #-of-threads)\n"
                  << std::flush;
        exit(EXIT_FAILURE);
      }
    } else if ("-s" == std::string(argv[i]) || "--seed" == std::string(argv[i])) {
      if (i + 1 < argc && '-' != argv[i + 1][0]) {
        OPTIONS.seed_str = argv[i + 1];
        try {
          OPTIONS.rand_seed = std::stoul(argv[i + 1], nullptr, 0);
        } catch (std::invalid_argument &ia) {
          std::hash<std::string> str_hash;
          OPTIONS.rand_seed = str_hash(argv[i + 1]);
        } catch (std::out_of_range &oor) {
          std::cerr << "invalid argument : provided seed was out of range (for -s/--seed flag) must be unsigned int\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        }
      } else { // OPTIONS.rand_seed = time(NULL); // no need
        std::cerr << "invalid argument : -s/--seed requires you to provide a seed after the flag\n"
                     "                   else exclude it to use default value: (current_system_time)\n"
                  << std::flush;
        exit(EXIT_FAILURE);
      }
    } else if ("-nd" == std::string(argv[i]) || "--no-deadlock" == std::string(argv[i])) {
      OPTIONS.deadlock = true;
#ifdef __romp__ENABLE_liveness_property
    } else if ("-lc" == std::string(argv[i]) || "--liveness-check" == std::string(argv[i])) {
      OPTIONS.liveness = true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.lcount = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument &ia) {
          std::cerr << "invalid argument : provided liveness check limit was not a number (NaN) !! (for -lc/--liveness-check "
                       "flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range &oor) {
          std::cerr << "invalid argument : provided liveness check limit was out of range (for -lc/--liveness-check flag) "
                       "must be unsigned "
                       "int (aka must be positive)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        }
      }
#endif
#ifdef __romp__ENABLE_cover_property
    } else if ("-cc" == std::string(argv[i]) || "--complete-on-cover" == std::string(argv[i]) || "--cover-check" == std::string(argv[i])) {
      OPTIONS.complete_on_cover = true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.cover_count = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument &ia) {
          std::cerr << "invalid argument : provided cover property coverage goal was not a number (NaN) !!\n"
                       "                   |-> (for -cc/--cover-check/--complete-on-cover flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range &oor) {
          std::cerr << "invalid argument : provided cover property coverage goal was out of range must be unsigned int (aka "
                       "must be positive & less than 2.147 billion)\n"
                       "                 |-> (for -cc/--cover-check/--complete-on-cover flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        }
      }
#endif
    } else if ("-ag" == std::string(argv[i]) || "-ll" == std::string(argv[i]) || "-al" == std::string(argv[i]) 
                || "--attempt-guard" == std::string(argv[i]) || "--loop-limit" == std::string(argv[i]) || "--attempt-limit" == std::string(argv[i])) {
      // OPTIONS.do_attempt_guard = true;  // just check to make sure this value is not 0
      OPTIONS.attempt_limit = _ROMP_RULE_COUNT;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.attempt_limit = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument &ia) {
          std::cerr << "invalid argument : provided attempt limit was not a number (NaN) !!\n"
                       "                   |-> (for -ag/--attempt-guard or -ll/--loop-limit flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range &oor) {
          std::cerr << "invalid argument : provided attempt limit was out of range must be unsigned int (aka "
                       "must be positive & less than 2.147 billion)\n"
                       "                   |-> (for  -ag/--attempt-guard or -ll/--loop-limit flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        }
      } else {
        OPTIONS.attempt_limit = _ROMP_RULE_COUNT;
        std::cerr << "\nWARNING : no attempt limit was provided, but it was enabled.\n"
                     "        |-> default value will be used (default: # of rules post ruleset expansion)\n"
                  << std::flush;
      }
    } else if ("-rhl" == std::string(argv[i]) || "--r-history" == std::string(argv[i])) {
      // OPTIONS.result_history = true;
      if (i + 1 < argc && '-' != argv[i + 1][0]) { // is it not argv[i+1]
        ++i;
        try {
          OPTIONS.history_length = std::stoul(argv[i], nullptr, 0);
        } catch (std::invalid_argument &ia) {
          std::cerr << "invalid argument : provided history length count was not a number (NaN) !! (for "
                       "-rhl/--r-history flag)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        } catch (std::out_of_range &oor) {
          std::cerr << "invalid argument : provided history length count was out of range (for -rhl/--r-history flag) must "
                       "be unsigned int (aka must be positive)\n"
                    << std::flush;
          exit(EXIT_FAILURE);
        }
      }
    } else if ("-a" == std::string(argv[i]) || "--all" == std::string(argv[i]) || "--report-all" == std::string(argv[i])) {
      OPTIONS.result_all = true;
#ifdef __romp__ENABLE_assume_property
    } else if ("--r-assume" == std::string(argv[i])) {
      OPTIONS.r_assume = true;
#endif
    } else if ("-o" == std::string(argv[i]) || "--output" == std::string(argv[i])) {
      OPTIONS.output_results = true;
      if (i + 1 < argc && argv[i + 1][0] != '-')
        OPTIONS.result_out_file = std::string(argv[++i]);
      else
        std::cerr << "\nWARNING : you have not specified an outfile after using the -o/--output flag !!\n"
                     "        |-> `${CWD}/results.txt` will be used by default !!\n"
                  << std::flush;
    } else if ("-t" == std::string(argv[i]) || "--trace" == std::string(argv[i])) {
      OPTIONS.do_trace = true;
      if (i + 1 < argc && argv[i + 1][0] != '-')
        OPTIONS.trace_dir = validate_dir_path(std::string(argv[++i]));
      else
        std::cerr << "\nWARNING : you have not specified an out directory after using the -t/--trace flag !!\n"
                     "        |-> `./traces/` will be used by default !!\n"
                  << std::flush;
      std::cerr << "\nWARNING : enabling traces can significantly reduce performance "
                   "& can take up a large amount of system recourses !!\n"
                << std::flush;
    }
  }
    // modifying values to match complex default values
    
    // check for inconsistent or contradictory options here
    // TODO (OPTIONAL) check OPTIONS to make sure config is valid and output
    if (OPTIONS.history_length == 0) {
      std::cerr << "\nERROR : history size cannot be 0 (--r-history/-rhl)\n" << std::flush;
      exit(EXIT_FAILURE);
    }
    if (OPTIONS.attempt_limit == 0) {
      std::cerr << "\nERROR : attempt limit cannot be 0 (--loop-limit/-ll/--attempt-guard/-ag)\n" << std::flush;
      exit(EXIT_FAILURE);
    }
    if (OPTIONS.depth == 0) {
      std::cerr << "\nERROR : max depth cannot be 0 (--depth/-d)\n" << std::flush;
      exit(EXIT_FAILURE);
    }
#ifdef __romp__ENABLE_cover_property
    if (OPTIONS.cover_count == 0) {
      std::cerr << "\nERROR : cover check goal cannot be 0 (--cover-check/-cc)\n" << std::flush;
      exit(EXIT_FAILURE);
    }
#endif
#ifdef __romp__ENABLE_cover_property
    if (OPTIONS.l_count == 0) {
      std::cerr << "\nERROR : liveness limit cannot be 0 (--liveness-check/-lc)\n" << std::flush;
      exit(EXIT_FAILURE);
    }
#endif
    if (OPTIONS.do_single) {
      if (walks_provided)
        std::cerr << "\nWARNING : -w/--walks/--walk_count is ignored when the -sw/--single-walk flag is set !!\n"
                  << std::flush;
      if (threads_provided)
        std::cerr << "\nWARNING : -ptn/--threads is ignored when the -sw/--single-walk flag(s) is set !!\n" << std::flush;
    } else { // not doing a single walk
      if (not threads_provided && OPTIONS.threads == 0) {
        std::cerr << "\nERROR : could not determine hardware thread capacity & no --threads/-ptn option was provided !!\n"
                    "         try again using the --threads/-ptn flag to set the number of threads you want to use.\n"
                    <<std::flush;
        exit(EXIT_FAILURE);
      }
      if (OPTIONS.threads == 0) {
        std::cerr << "\nERROR : number of threads cannot be 0 (--threads/-ptn)\n" << std::flush;
        exit(EXIT_FAILURE);
      }
      if (OPTIONS.random_walkers == 0) {
        std::cerr << "\nERROR : number of walks cannot be 0 (--walks/-w)\n" << std::flush;
        exit(EXIT_FAILURE);
      }
      if (not threads_provided)
        std::cerr << "\nINFO : the number of threads to use was not specified (with -ptn/--threads flags)\n"
                     "     |-> defaults to "
                  << OPTIONS.threads << std::endl;
      if (not walks_provided)
        std::cerr << "\nINFO : the number of random walkers was not specified (with -w/--walks/--walk-count flags)\n"
                     "     |-> defaults to "
                  << _ROMP_THREAD_TO_RW_RATIO * OPTIONS.threads << "(= " << _ROMP_THREAD_TO_RW_RATIO << " * "
                  << OPTIONS.threads << ")\n"
                  << std::flush;
      if (threads_provided && not walks_provided)
        OPTIONS.random_walkers = _ROMP_THREAD_TO_RW_RATIO * OPTIONS.threads;  // post parse assign default value
    }
    if (start_provided && OPTIONS.do_even_start)
      std::cerr << "\nWARNING : -w/--walks/--walk_count is ignored when the -sw/--single-walk flag is set !!\n"
                  << std::flush;
    // warnings and end with errors as appropriate
}


} // namespace options
} // namespace romp
