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
    int choice = seed % (max-min+1) + min;  // generates the random number
    return choice;
}

class RandWalker : public ::romp::IRandWalker {
private:
  static id_t next_id;
  const id_t id;
  size_t start_id;
  const unsigned int init_rand_seed;
  unsigned int rand_seed;
  State_t state;
  size_t _fuel = OPTIONS.depth;
  bool _valid = true;
  bool _is_error = false;
  json_file_t* json;
  // tripped thing
  IModelError* tripped = nullptr;
  IModelError* tripped_inside = nullptr;
  struct History {
    const Rule* rule;
  };
  size_t history_level = 1;
  size_t history_size = OPTIONS.history_length;
  History* history = new History[OPTIONS.history_length];
  size_t history_start = 1;
  /**
   * @brief call if rule is applied to store what rule made the change in the
   * history circular buffer.
   * @param r reference to the rule that was applied
   */
  void add_to_history(const Rule& r) {
    history[history_level%history_size] = History{&r};
    ++history_level;
    if (history_level >= history_size)
      ++history_start;
  }
#ifdef __ROMP__DO_MEASURE
  time_t init_time;
  time_t start_time;
  time_t active_time = 0;
#endif
  
  void init_state() noexcept {
    if (OPTIONS.do_even_start) {
      rand_choice(rand_seed,0ul,_ROMP_STARTSTATES_LEN); // burn one rand option for consistency
      start_id = id % _ROMP_STARTSTATES_LEN;
    } else if (OPTIONS.start_id) {
      rand_choice(rand_seed,0ul,_ROMP_STARTSTATES_LEN); // burn one rand option for consistency
      start_id = OPTIONS.start_id;
    } else
      start_id = rand_choice(rand_seed,0ul,_ROMP_STARTSTATES_LEN);
    const StartState& startstate = ::__caller__::STARTSTATES[start_id];
#ifdef __ROMP__DO_MEASURE
    start_time = time(NULL);
#endif
    try {
      startstate.initialize(state);
      if (OPTIONS.do_trace)
        *json << "{\"$type\":\"init\",\"startstate\":" << startstate << ",\"state\":" << state <<"}";
    } catch (const IModelError& me) {
       __handle_exception<StartState,IModelError>(startstate,me); return;
    } catch (const std::exception& ex) {
      __handle_exception<StartState,std::exception>(startstate,ex); return;
    } catch (...) {
      std::cerr << "unknown non std::exception was thrown while initializing a Random Walker (id:"<<id<<")!\t[dev-error]\n" << std::flush;
    }
    // if (json != nullptr) delete json;
#ifdef __ROMP__DO_MEASURE
    active_time += time(NULL) - start_time;
#endif
  }

  template<typename R, typename E>
  void __handle_exception(const R& r, const E& er) noexcept {
    if (OPTIONS.do_trace) {
       *json << "],\"error-trace\":[" << er << "]";
    } 
    tripped_inside = r.make_error();
#ifdef __ROMP__DO_MEASURE
    active_time += time(NULL)-start_time;
#endif
  }
 

  void trace_metadata_out() const {
    *json << ",\"metadata\":{\"model\":\"" __model__filename "\",\"seed\":" << init_rand_seed << ",\"max-depth\":" << OPTIONS.depth <<"}";
  }

public:
  const std::function<void()> sim1Step;
  size_t fuel() { return _fuel; }
  bool is_valid() { return _valid; }
  size_t is_error() { return _is_error; }
  

  RandWalker(unsigned int rand_seed_) 
    : rand_seed(rand_seed_),
      init_rand_seed(rand_seed_),
      sim1Step(((OPTIONS.do_trace) 
                  ? std::function<void()>([this](){sim1Step_trace();}) 
                  : std::function<void()>([this](){sim1Step_no_trace();}))),
      id(RandWalker::next_id++) 
  { 
    state.__rw__ = this; /* provide a semi-hidden reference to this random walker for calling the property handlers */ 
#ifdef __ROMP__DO_MEASURE
    init_time = time(NULL);
#endif
    init_state();
    if (OPTIONS.do_trace) {
      json = new json_file_t(OPTIONS.trace_dir + std::to_string(init_rand_seed) + ".json");
      // sim1Step = std::function<void()>([this]() {sim1Step_trace();});
      *json << "{\"$type\":\"romp-trace\"";
      trace_metadata_out();
      *json << ",\"trace\":[";
    } else {
      // sim1Step = std::function<void()>([this]() {sim1Step_no_trace();});
    }
  } 

  ~RandWalker() { 
    if (json != nullptr) delete json; 
    if (tripped != nullptr) delete tripped;
    if (tripped_inside != nullptr) delete tripped_inside; 
    if (history != nullptr) delete[] history; 
  }


private:

  /**
   * @brief to pick a rule in random for simulation step
   */
  const RuleSet& rand_ruleset(){
    return ::__caller__::RULESETS[rand_choice<size_t>(rand_seed,0ul,_ROMP_RULESETS_LEN)]; 
  }
  /**
   * @brief to pick a rule in random for simulation step
   */
  const Rule& rand_rule(const RuleSet& rs){
    return rs.rules[rand_choice<size_t>(rand_seed,0ul,rs.rules.size())];  
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
        add_to_history(r);
      } else {
        *json << ",{\"$type\":\"rule-failed\",\"rule\":" << r << "}";
      }      
    } catch(IModelError& me) {
      __handle_exception<Rule,IModelError>(r,me); return;
    } catch (std::exception& ex) {
      __handle_exception<Rule,std::exception>(r,ex); return;
    } catch (...) {
      std::cerr << "unexpected UNKNOWN exception inside of model (rule) \t[dev-error]\n";
    }
    if (pass)
      for (const auto& prop : ::__caller__::PROPERTIES)
        try {
          if (prop.check(state)) { // if tripped
            tripped = new ModelPropertyError(prop);
            break;
          }
        } catch(IModelError& me) {
          __handle_exception<Property,IModelError>(prop,me); return;
        } catch (std::exception& ex) {
          __handle_exception<Property,std::exception>(prop,ex); return;
        } catch (...) {
          std::cerr << "unexpected UNKNOWN exception inside of model (property-rule) \t[dev-error]\n";
        }
#ifdef __ROMP__DO_MEASURE
    active_time += time(NULL)-start_time;
#endif
  }

  void sim1Step_no_trace() noexcept {
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
        add_to_history(r);
      }      
    } catch(IModelError& me) {
      __handle_exception<Rule,IModelError>(r,me); return;
    } catch (std::exception& ex) {
      __handle_exception<Rule,std::exception>(r,ex); return;
    } catch (...) {
      std::cerr << "unexpected UNKNOWN exception inside of model (rule) \t[dev-error]\n";
    }
    if (pass)
      for (const auto& prop : ::__caller__::PROPERTIES)
        try {
          if (prop.check(state)) { // if tripped
            tripped = new ModelPropertyError(prop);
            break;
          }
        } catch(IModelError& me) {
          __handle_exception<Property,IModelError>(prop,me); return;
        } catch (std::exception& ex) {
          __handle_exception<Property,std::exception>(prop,ex); return;
        } catch (...) {
          std::cerr << "unexpected UNKNOWN exception inside of model (property-rule) \t[dev-error]\n";
        }
#ifdef __ROMP__DO_MEASURE
    active_time += time(NULL)-start_time;
#endif             
  }

  // called when trying to print the results of the random walker when it finishes (will finish up trace file if nessasary too)
  //  the calling context should ensure that the RandWalker is not being used else where & safe output to the ostream 
  friend std::ostream& operator << (std::ostream& out, const RandWalker& rw) {
#ifdef __ROMP__DO_MEASURE
    time_t total_time = (time(NULL)-rw.init_time);
#endif
    if (OPTIONS.do_trace && rw.json != nullptr) {
      if (rw._valid) // if it didn't end in an error we need to: 
        *rw.json << "]" // close trace
                  << ",\"error-trace\":[]"; // output empty error-trace
      *rw.json << ",\"results\":{\"depth\":"<< OPTIONS.depth-rw._fuel <<",\"valid\":" << rw._valid << ",\"is-error\":"<< rw._is_error
#ifdef __ROMP__DO_MEASURE
                                  << ",\"active-time\":" << rw.active_time << ",\"total-time\":" << total_time
#else
                                  << ",\"active-time\":null,\"total-time\":null" 
#endif
                                << "}" // closes results object
               << "}"; // closes top level trace object
      rw.json->out.flush();
    }
    if (not rw._is_error && not OPTIONS.result_all) return out; // don't output non-error state unless --report-all
#ifdef __romp__ENABLE_assume_property
    if (not rw._is_error && rw.tripped != nullptr && not OPTIONS.r_assume) return out; // don't output assumption violations unless --report-assume
#endif
    out << "\n====== BEGIN :: Report of Walk #" << rw.id << " ======"
        << "\n  RandSeed: " << rw.init_rand_seed
        << "\n  StartState: " << ::__caller__::STARTSTATES[rw.start_id]
        << "\n  Fuel level: " << rw._fuel
        << "\n\nTrace lite:"
        << "\n  " << ((OPTIONS.do_trace) ? 
                          "see \"" + OPTIONS.trace_dir + std::to_string(rw.init_rand_seed) + ".json\" for full trace." 
                        : "use the --trace/-t option to generate a full & detailed trace." ) 
        << "\n  # of rules applied: " << rw.history_level+1
        << "\n  History:\n"; // how to get for thr rule vs ruleset
    if (rw.history_start > 1)
      out << "    ... forgotten past ...\n";
    for (size_t i=rw.history_start; i<=rw.history_level; ++i)
      out << "    (" << i+1 <<") " << *(rw.history[i%rw.history_size].rule) << "\n";
    out << "\nFinal State value:" << rw.state << "\n"
        << "\nProperty/Error Report:"
        << "\n  Still a ``valid'' State?: " << (rw._valid ? "true" : "false") //    is it a valid state
        << "\n  In an ``Error State''?: " << (rw._is_error ? "true" : "false") //    is it a valid state
        << "\n  Property Violated: " << *rw.tripped;
    if (rw.tripped_inside != nullptr)
      out << "\n       While Inside: " << *rw.tripped_inside;
        
#ifdef __ROMP__DO_MEASURE
    out << "TODO" //  states discovered (TODO)
        << "TODO" //  runtime info sub-header
        << "TODO" //      active time
        << "TODO" // metrics header
        << "TODO" //      total time
#endif
    out << "\n======= END :: Report of Walk #" << rw.id << " =======\n" << std::flush;

    return out;
  }

  bool error_handler(id_t error_id) {
    tripped = new ModelMErrorError(error_id);
    _valid = false;
    _is_error = true;
    return true;
  }

  bool assertion_handler(bool expr, id_t prop_id) {
    if (expr) return false;
    tripped = new ModelPropertyError(prop_id);
    _valid = false;
    _is_error = true;
    return true;
  }
  bool invariant_handler(bool expr, id_t prop_id) {
    if (expr) return false;
    // no need to store exception in tripped if a property rule the catch will give us a better error dump
    // invar_handler is only called from a property rule
    _valid = false;
    _is_error = true;
    return true;
  }
#ifdef __romp__ENABLE_assume_property
  bool assumption_handler(bool expr, id_t prop_id) {
    if (expr) return false;
    tripped = new ModelPropertyError(prop_id);
    _valid = false;
    _is_error = false; // this is what makes an assumption different from an assertion
    return true;
  }
#else
  bool assumption_handler(bool expr, id_t prop_id) {
    return false;  // don't do anything if the assume property is nto enabled
  }
#endif
#ifdef __romp__ENABLE_cover_property
  bool cover_handler(bool expr, id_t cover_id, id_t prop_id) {
    return false;  // TODO actually handle this as described in the help page
  }
#else
  bool cover_handler(bool expr, id_t cover_id, id_t prop_id) {
    return false;  // don't throw anything if cover is not enabled
  }
#endif
#ifdef __romp__ENABLE_liveness_property
  bool liveness_handler(bool expr, id_t liveness_id, id_t prop_id) {
    return false;  // TODO actually handle this as described in the help page
  }
#else
  bool liveness_handler(bool expr, id_t liveness_id, id_t prop_id) {
    // no need to store exception in tripped if a property rule the catch will give us a better error dump
    return false;  // don't throw anything if liveness is not enabled
  }
#endif


}; //? END class RandomWalker

id_t RandWalker::next_id = 0u;

/**
 * @brief to generate random seeds for the no of random-walkers
 * rand is generated using UNIX timestamp 
 * @param root_seed the parent seed for generating the random seeds.
 */
unsigned int gen_random_seed(unsigned int &root_seed) {
  return rand_choice(root_seed, 1u, UINT32_MAX);
}

/**
 * @brief generate all startstates of the model. 
 * To do - how to get the size of startstate
 * 
 */
std::vector<RandWalker> gen_random_walkers(unsigned int root_seed)   {
  std::vector<RandWalker> rws;
  for(int i=0; i<OPTIONS.random_walkers; i++) {
    rws.push_back(RandWalker(gen_random_seed(root_seed)));
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
void launch_OpenMP(unsigned int root_seed) {
  std::cout << "\n\t!! NOT YET IMPLEMENTED !!\n" << std::endl; return; //!! temp, remove when finished !! 
  
  // std::vector<RandWalker> rws;
  // try {
  //   rws = gen_random_walkers(rw_count, root_seed, fuel);
  // } catch (const IModelError& ex) {
  //   std::cerr << "\nModel raised an error when initializing our start state(s)!! (message below)\n"
  //             << ex << std::endl;
  // }
  //TODO: launch the random walkers !!
}


/**
 * @brief (NOT YET IMPLEMENTED) \n
 *        Implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param root_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 */
void launch_CUDA(unsigned int root_seed);


/**
 * @brief (NOT YET IMPLEMENTED) \n
 *        Implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param root_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 */
void launch_SYCL(unsigned int root_seed);


/**
 * @brief (NOT YET IMPLEMENTED) \n
 *        Implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param root_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 */
void launch_OpenMPI(unsigned int root_seed);


/**
 * @brief Launches a single RandWalker (basically jst a simulation).
 * @param rand_seed the random seed
 * @param fuel the max number of rules to try to apply
 */
void launch_single(unsigned int rand_seed) {
  RandWalker* rw = new RandWalker(rand_seed);
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
