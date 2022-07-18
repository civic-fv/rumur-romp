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
#include<iostream>
#include<string>
#include<functional>
#endif

namespace romp {
namespace options {

struct Options {
  size_t threads = 0;    // mpz_class ??
  size_t depth = 0;      // what has to be default value  ??
  size_t random_walkers; // any default value needed if not provided ?
  unsigned int rand_seed; // the random seed to se (defaults to current system time)
  /*std::string seed_str; // what for this ??
  bool do_single = false;
  bool do_trace = false;
  std::string trace_file = false;*/
  size_t lcount;
  size_t cover_count;
  size_t ag_count;
  size_t hlcount;
  void print_help();
  std::string trace_file_path;// path for the trace file to be created during each walk
  bool deadlock=false; // separate bool for each property or consider having a valid bool . depends on how property will be designed 
  bool result=false; // result output
  bool result_all=false;
  bool r_assume=false;
};

Options OPTIONS;


// input model's path
// extern std::string input_filename;

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

void print_help()
{
  std::cout<<"The Options are shown as follows:"  <<endl
  <<"-h or --help \tto list the options in the ROMP tool"<<endl
    <<"RANDOM WALKER OPTIONS :"<<endl
    <<endl
    <<"-d <int>or --depth <int> \tMaximum number of times rules were tried to be applied. If not provided then it takes a default value "<<endl
    <<"-ptn <int> or --threads <int> \tSpecifies the thread count. If not provided then the default value is 4"<<endl
    <<"-sw or --single_walk \tTo perform a single random walker on a single thread. It cant be used along with -ptn"<<endl
    <<"-w <int>or --walk_count <int>\t Specifies the number of random walkers to be launched. It takes a conditional argument int."<<endl
    <<"-s <string/int> or --seed <string/int> \t Random seed for the random walkers.It takes an optional argument of string or int for the seed and if not provided default value is considered."<<endl<<endl
    <<"PROPERTY CONFIGURATIONS :"<<endl
    <<endl
    <<"-nd or --no_deadlock \t Disable Deadlock detection "<<endl
    <<"-lc <int> or --liveness_check <int>\t Allows you to specify the max number of successful steps a random walker can have in a row before any given liveness property must evaluate to true."<<endl
              <<"\t\t\t\t\tIt takes an optional argument int else default value of MAX_INT is considered"<<endl
    <<"-cc <int> or --complete_on_cover <int>\tIt considers the random walk to be  complete when each statement has been reached .And takes in the optional argument int if not provided then default value is MAX_INT"<<endl
    <<"-ag <int> or --attempt_guard <int> \t It is used to mark a random walker to be in deadlock state if the walker has failed to apply any rules."
                <<" If optional argument of integer not provided then default value of MAX_INT is considered"<<endl<<endl
    <<"Trace Options"<<endl
    <<endl
    <<"-t <str:out_dir> or --trace <str:out_dir>\t To output the trace results of each random walkers.The optional argument <str:out-dir> specifies the directory path to where the trace files has to be written."<<endl
                 <<" If not provided then it is stored in the path <path>."<<endl<<endl

    <<"Results Options" <<endl
    <<endl
    <<"-rhl <int> or --r_history <int> Maximum number of previous states and rules to keep track off and output in the results."<<endl
    <<"-ra or --r_all \t Report all results not just violating"<<endl
    <<"-r_assume \t Report all violating  assumptions"<<endl
    <<"-o <str:dir_path> or --output <str:dir_path> A string containing path to a directory where the system will output the diagram files" <<endl;
}
static void parse_args(int argc, char **argv) {

  for (int i=0; i<argc; ++i){

  if ("-h" == argv[i] || "--help" == argv[i]){ // to print help message and ( result of comparison against a string literal is unspecified)
    print_help();
    exit(EXIT_SUCCESS);
  } else if("-d" ==argv[i] || "--depth" == argv[i]){
    if(i+1< argc && '-' != argv[i+1][0]){// is it not argv[i+1]  
        ++i;
          OPTIONS.depth=std::stoi(argv[i],nullptr,0);}// Do we need to make a check if depth only could be at max  int_max ? 
    else
        OPTIONS.depth = INT32_MAX;
  } else if ("-ptn" == argv[i] || "-threads" == argv[i]){
    if(OPTIONS.threads ==1 && OPTIONS.random_walkers ==1){
        std::cerr << "invalid argument - Number of threads cannot be specified while using single walk "<<std::flush;
        exit(EXIT_FAILURE);
    }else if(i+1< argc && '-' != argv[i+1][0]){
        ++i;
        OPTIONS.threads=std::stoi(argv[i],nullptr,0);}
    else
        OPTIONS.threads = 4;
  }else if ("-sw" == argv[i] || "--single_walk" == argv[i]){
    if(OPTIONS.threads > 0){
        std::cerr << "invalid argument - Number of threads argument cannot be specified while using single walk "<<std::flush;
      exit(EXIT_FAILURE);}
    else{
      OPTIONS.threads,OPTIONS.random_walkers = 1;}
  }else if("-w" ==argv[i] || "--walk_count" == argv[i]){
    if (i+1<argc && '-' != argv[i+1][0]) {
              ++i;
          OPTIONS.random_walkers=std::stoi(argv[i],nullptr,0);}
    else
        OPTIONS.random_walkers = INT32_MAX;
  } else if("-s" ==argv[i] || "--seed" == argv[i]){
    if(i+1< argc && '-' != argv[i+1][0]){
      try{
          OPTIONS.rand_seed = std::stoul(argv[i],nullptr,0);
          if(OPTIONS.rand_seed > ULONG_MAX && OPTIONS.rand_seed < 0 )  // error cond ? 
          throw OPTIONS.rand_seed;
      }catch(std::invalid_argument &){
        std::string str (OPTIONS.rand_seed);
        std::hash<std::string> str_hash;
        OPTIONS.rand_seed = str_hash(str);
      }else
        OPTIONS.rand_seed = time(NULL);}
  } else if ("-nd" == argv[i] || "--no_deadlock" == argv[i]) {
      OPTIONS.deadlock=true;
  } else if("-lc" ==argv[i] || "--liveness_check" == argv[i]){
      // any valid flag needed?
    if(i+1< argc && '-' != argv[i+1][0]){// is it not argv[i+1]  
        ++i;
          OPTIONS.lcount=std::stoi(argv[i],nullptr,0);}
    else
        OPTIONS.lcount = INT32_MAX;
  }else if("-cc" ==argv[i] || "--complete_on_cover" == argv[i]){//todo proerpty check also if flag needed
      // any valid flag needed?
    if(i+1< argc && '-' != argv[i+1][0]){// is it not argv[i+1]  
        ++i;
          OPTIONS.cover_count=std::stoi(argv[i],nullptr,0);}
    else
        OPTIONS.cover_count = INT32_MAX;
  }else if("-ag" ==argv[i] || "--attempt_guard" == argv[i]){// TODO @Andy make nd true when agcount is decremented to 0
      // any valid flag needed? 
    if(i+1< argc && '-' != argv[i+1][0]){// is it not argv[i+1]  
        ++i;
          OPTIONS.ag_count=std::stoi(argv[i],nullptr,0);}
    else
        OPTIONS.ag_count = INT32_MAX;
  }else if("-rhl" ==argv[i] || "--r_history" == argv[i]){
        OPTIONS.result_history =true;
          if(i+1< argc && '-' != argv[i+1][0]){// is it not argv[i+1]  
            ++i;
            OPTIONS.hl_count=std::stoi(argv[i],nullptr,0);}
          else
            OPTIONS.hl_count = INT32_MAX;
  }else if("-ra" ==argv[i] || "--result_all" == argv[i]){
        OPTIONS.result_all=true;
        // TODO figure out a way to define property 
  }else if("--r_assume" ==argv[i]){
        OPTIONS.r_assume=true;
        //todo figure out to define the property
  }else if("--output" ==argv[i] || "-o" ==argv[i]){
        OPTIONS.output =true;
        //todo figure out to output 
  }
  }
}
  
  
  
  /*
  else if ("-t" == argv[i] || "--trace" == argv[i]) {
    if (i+1<argc && '-' != argv[i+1][0]) {
      ++i;
      OPTIONS.trace_file_path = argv[i];
    }else
      OPTIONS.trace_file_path = DEFAULT;// give the default path
      // TODO function to create trace files for each walk and path to be specified from cli 
  }
  else if ("-s" == argv[i] || "--seed" == argv[i]) {
    if (i+1<argc && '-' != argv[i+1][0]) {
      ++i;
      OPTIONS.trace_file = argv[i];
    } else {
      std::cerr << "invalid argument!! :: -s/--seed requires a parameter to follow, but none was found !!\n" << std::flush;
      exit(EXIT_FAILURE);
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
}
} // namespace options
} // namespace romp
*/