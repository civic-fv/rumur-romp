/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp-rw.hpp
 *
 * @brief The primary simulation/random walk functions of romp-rw.
 *
 * @date 2022/05/11
 * @version 0.1
 */

#ifndef __romp__GENERATED_CODE
#include "c_prefix.cpp" // FOR PRE-CODEGEN LANGUAGE SUPPORT ONLY !!
#endif

// << =================================== Type Declarations ==================================== >>

namespace romp {

/**
 * @brief helper function rand_choice 
 * 
 */
template<typename T>
T rand_choice(unsigned int &seed, T min, T max) {
    //not done fully
    seed = (((seed ^ (seed >> 3)) >> 12) & 0xffff) | ((seed & 0x7fff) << 16); // modifies the seed
    int choice = seed % (maxWeight-min+1) + min;  // generates the random number
    return choice;
}

class RandWalker : public ::romp::IRandWalker {
private:
  static id_t next_id;
  const id_t id;
  const unsigned int init_rand_seed;
  unsigned int rand_seed;
  State_t state;
  size_t _fuel = OPTIONS.depth;
  bool _valid = true;
  bool _is_error = false;
  json_file_t* json;
  // tripped thing
  IModelError* tripped = nullptr;
  // how many rules have tried to be applied to this state
  size_t level = 0;
  // array of integers representing the rul ID's applied to this state (treated
  // as a circular buffer array)
  id_t history[_ROMP_STATE_HISTORY_LEN];
#ifdef __ROMP__DO_MEASURE
  unsigned int init_time;
  unsigned int start_time;
  unsigned int active_time = 0;
#endif
  
  static void init_state(RandWalker* rw) noexcept {
    try {
      // TODO: (ANDREW) generate startstate randomly from rand_seed
      if (OPTIONS.do_trace)
        *rw->json << "{\"$type\":\"init\",\"startstate\":"<< "todo" << ",\"state\":"<< state <<"}";
    } catch (const IModelError& me) {
       rw->__handle_exception<IModelError>(me);
    } catch (const std::exception& ex) {
      rw->__handle_exception<std::exception>(ex);
    } catch (...) {
      std::cerr << "unknown non std::exception was thrown while initializing a Random Walker!\n" << std::flush;
    }
    if (rw->json != nullptr) delete rw->json;
#ifdef __ROMP__DO_MEASURE
    active_time += time(NULL) - start_time;
#endif
  }

  template<typename T>
  void __handle_exception(const T& er) noexcept {
    if (OPTIONS.do_trace) {
       *json << "],\"error-trace\":[" << er << "]";
    } 
    tripped = new T(er);
    valid = false;
  }

  void trace_metadata_out() const {
    *json << ",\"metadata\":{\"model\":\"" __model__filename "\",\"seed\":" << init_rand_seed << ",\"max-depth\":" << OPTIONS.depth <<"}";
  }

public:
  size_t fuel() { return _fuel; }
  bool is_valid() { return _valid; }
  size_t is_error() { return _is_error; }
  
  void (/* RandWalker:: */*sim1Step)();

  RandWalker(State_t startstate, unsigned int rand_seed_) 
    : state(startstate), 
      rand_seed(rand_seed_),
      init_rand_seed(rand_seed_),
#ifdef __ROMP__DO_MEASURE
      init_time(time(NULL)),
#endif
      id(RandWalker::next_id++) 
  { 
    state.__rw__ = this; /* provide a semi-hidden reference to this random walker for calling the property handlers */ 
    if (OPTIONS.do_trace) {
      json = new json_file_t(OPTIONS.trace_dir + '/' + std::to_string(init_rand_seed) + ".json");
      sim1Step = sim1Step_trace;
      *json << "{\"$type\":\"romp-trace\"";
      trace_metadata_out();
      *json << ",\"trace\":[";
    } else {
      sim1Step = sim1Step_no_trace;
    }
    init_state(this);
  } 

  ~RandWalker() { *json << "}"; json->out.close(); delete json; if (tripped != nullptr) delete tripped; }


private:

  /**
   * @brief to pick a rule in random for simulation step
   */
  const RuleSet& rand_ruleset(){
    return ::__caller__::RULE_SETS[rand_choice<size_t>(rand_seed,0ul,_ROMP_RULESETS_LEN)]; 
  }
  /**
   * @brief to pick a rule in random for simulation step
   */
  const Rule& rand_rule(const RuleSet& rs){
    return rs.rules[rand_choice<size_t>(rand_seed,0ul,rs.rules.size())];  
  }
  
  /**
   * @brief call if rule is applied to store what rule made the change in the
   * history circular buffer.
   * @param id the id of the rule that was applied.
   */
  void rule_applied(id_t id) {
    history[level % _ROMP_STATE_HISTORY_LEN] = id;
    level++;
  }

  void sim1Step_trace() noexcept {
#ifdef __ROMP__DO_MEASURE
    start_time = time(NULL);
#endif
    const RuleSet& rs= rand_ruleset();
    const Rule& r= rand_rule(rs);
    bool pass = false;
    try {  
      if ((pass = r.guard(state)) == true) {
        r.action(state);
        --_fuel;
        for (const Property& prop : ::__caller__::PROPERTIES)
          prop.check(state); 
      } else {
        *json << ",{\"$type\":\"rule-failed\",\"rule\":" << r << "}";
#ifdef __ROMP__DO_MEASURE
        active_time += time(NULL)-start_time;
#endif
        return;
      }      
    } catch(IModelError& me) {
      __handle_exception<IModelError>(me);
    } catch (std::exception& ex) {
      __handle_exception<std::exception>(ex);
    } catch (...) {
      std::cerr << "unexpected UNKNOWN exception outside of model \t[dev-error]\n";
    }
  }

  void sim1Step_no_trace() noexcept {
    
    //TODO: store the mutated state in the history <-- we don't store the old state we store an id_t referring to the rule applied
    const RuleSet& rs= rand_ruleset();
    const Rule& r= rand_rule(rs);
    try {
      if (r.guard(state) == false)
        return;
      r.action(state);
      --_fuel;
      for (const Property& prop : ::__caller__::PROPERTIES)
          prop.check(state)
                     
    } catch(IModelError& me) {
      __handle_exception<IModelError>(me);
    } catch (std::exception& ex) {
      __handle_exception<std::exception>(ex);
    } catch (...) {
      std::cerr << "unexpected UNKNOWN exception outside of model \t[dev-error]\n";
    }
  }

  // called when trying to print the results of the random walker when it finishes (will finish up trace file if nessasary too)
  //  the calling context should ensure that the RandWalker is not being used else where & safe output to the ostream 
  friend std::ostream& operator << (std::ostream& out, const RandWalker& rw) {
#ifdef __ROMP__DO_MEASURE
    unsigned int total_time = (time(NULL)-rw.init_time);
#endif
    if (OPTIONS.do_trace && rw.json != nullptr) {
      if (rw._valid) // if it didn't end in an error we need to: 
        *rw->json << "]" // close trace
                  << ",\"error-trace\":[]"; // output empty error-trace
      *rw.json << ",\"results\":{\"depth\":"<< OPTIONS.depth-rw._fuel <<",\"valid\":" << rw._valid << ",\"is-error\":"<< rw._is_error
#ifdef __ROMP__DO_MEASURE
                                  << ",\"active-time\":" << rw.active_time << ",\"total-time\":"
#else
                                  << ",\"active-time\":null,\"total-time\":null" 
#endif
                                <<"}" // closes results object
               << "}" // closes top level trace object
    }
    //TODO write result plain text string (not json) to "out" (the variable in func parameter)
    out << "TODO" // ...
        << "TODO" // ...
        << "TODO" // ...
        << "TODO" // ...
#ifdef __ROMP__DO_MEASURE
        << "TODO" // active time
        << "TODO" // total time
        << "TODO" // states discovered (TODO)
#endif
        << "TODO" /* ... */;

    return out;
  }

  bool assertion_handler(bool expr, id_t prop_id) {

  }

}; //? END class RandomWalker

id_t RandWalker::next_id = 0u;


/**
 * @brief generate all startstates of the model. 
 * To do - how to get the size of startstate
 * 
 */
std::vector<State_t> gen_startstates() throw (IModelError) {
  std::vector<State_t> states;
  for (int i=0; i<_ROMP_STARTSTATES_LEN; i++) {
    states.push_back(State_t{});
    ::__caller__::STARTSTATES[i].initialize(states[states.size()-1]);
  }
  return states;
}

/**
 * @brief to generate random seeds for the no of random-walkers
 * rand is generated using UNIX timestamp 
 * @param root_seed the parent seed for generating the random seeds.
 */
unsigned int genrandomseed(unsigned int &root_seed) {
  return rand_choice(root_seed, 1u, UINT32_MAX);
}

/**
 * @brief generate all startstates of the model. 
 * To do - how to get the size of startstate
 * 
 */
std::vector<RandWalker> gen_random_walkers(size_t rw_count, unsigned int root_seed, size_t fuel) throw (IModelError) {
  std::vector<RandWalker> rws;
  auto startstates = gen_startstates();
  for(int i=0; i<rw_count; i++) {
    auto copied_startstate = State_t(startstates[i%startstates.size()]);
    RandWalker rw1(copied_startstate, get_random_seed(root_seed), fuel);
  }
  return rws;
}

// example of how to get a copy of an object in C++:
//   use the copy constructor (always has the signature)
//    ClassName(const ClassName &obj)
//  this means your's should look like:
//    auto copied_state = State_t(startstates[i%startstates.size()]);




/**
 * @brief implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param rw_count the number of \c RandWalker 's to use.
 * @param rand_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 * @param thread_count the max number of threads to use to accomplish all said random walks.
 */
void launch_OpenMP(size_t rw_count, unsigned int rand_seed, size_t fuel, size_t thread_count) {
  std::vector<RandWalker> rws;
  try {
    rws = gen_random_walkers(rw_count, rand_seed, fuel);
  } catch (const IModelError& ex) {
    std::cerr << "\nModel raised an error when initializing our start state(s)!! (message below)\n"
               << ex.what << std::endl;
  }
  //TODO: launch the random walkers !!
}


/**
 * @brief (NOT YET IMPLEMENTED) \n
 *        Implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param rand_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 */
void launch_CUDA(unsigned int rand_seed, size_t fuel);


/**
 * @brief (NOT YET IMPLEMENTED) \n
 *        Implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param rand_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 */
void launch_SYCL(unsigned int rand_seed, size_t fuel);


/**
 * @brief (NOT YET IMPLEMENTED) \n
 *        Implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param rand_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 */
void launch_OpenMPI(unsigned int rand_seed, size_t fuel);


/**
 * @brief Launches a single RandWalker (basically jst a simulation).
 * @param rand_seed the random seed
 * @param fuel the max number of rules to try to apply
 */
void launch_single(unsigned int rand_seed, size_t fuel) {
  /* ----> to do how to obtain the total no of rule set (treating rules as singleton rules )*/
  unsigned int _seed_cpy = rand_seed;
  State_t start_state;
  try {
    ::__caller__::STARTSTATES[rand_choice(_seed_cpy, 0ul, _ROMP_STARTSTATES_LEN)].initialize(start_state);
  } catch (const IModelError& ex) {
    std::cerr << "\nModel raised an error when initializing our start state!! (message below)\n"
               << ex.what() << std::endl;
    return;
  }
  RandWalker* rw = new RandWalker(start_state,rand_seed);
  while( rw->is_valid() && rw->fuel() > 0)
    rw->sim1Step();

  std::cout << "Single ROMP RESULT:\n" << *rw << std::endl;  // example of writing one RW's results to cout
}


// void Sim1Step(RandWalker::State state, Rule rule, size_t state_count) {
//   for (int s = 0; s < state_count; s++)
//     if (rule.Guard(states[s])) {
//       rule.Run(states[s]);
//       states[i].rule_applied(
//           rule.id); // this keeps track of history and other overhead
//       // These could possibly be parallelized even better for GPU using
//       // shared memory. (not implemented here, could introduce data races)
//       for (int i = 0; i < _ROMP_INVARIANTS_LEN; i++)
//         states[s].valid |= State::INVARIANTS[i](
//             states[i]); // this will need to change after we figure shit out
//       for (int a = 0; a < _ROMP_ASSERTIONS_LEN; a++)
//         states[s].valid |= State::ASSERTIONS[a](
//             states[i]); // this will need to change after we figure shit out
//     }
// }


// << ========================================================================================== >> 
// <<                                         ROMP CODE                                          >> 
// << ========================================================================================== >> 
}
// template<class S, class R>
