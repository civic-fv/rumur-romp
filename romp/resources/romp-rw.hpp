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
  json_file_t* json;
  // tripped thing
  IModelError* tripped = nullptr;
  // how many rules have tried to be applied to this state
  size_t level = 0;
  // array of integers representing the rul ID's applied to this state (treated
  // as a circular buffer array)
  id_t history[_ROMP_STATE_HISTORY_LEN];
  
  static init_state(unsigned int& seed, json_file_t* json) noexcept {
    try {
      // TODO: (ANDREW) generate startstate randomly from rand_seed
    } catch (const IModelError& me) {
       __handle_init_exception<IModelError>(json,me);
    } catch (const std::exception& ex) {
      __handle_init_exception<std::exception>(json,ex);
    } catch (...) {
      std::cerr << "unknown non std::exception was thrown while initializing a Random Walker!\n" << std::flush;
    }
    if (json != nullptr) delete json;
  }

  template<typename T>
  static __handle_init_exception(json_file_t& json, const T&) noexcept {
    //TODO: (AJANTHA) this function
     *json << ",{\"$type\":\"romp-trace\",\"metadata\":" << /*metadata*/ << "}";
    if (OPTIONS.do_trace) {
        *json << ",{\"$trace\":\"Random walker\",\"$type\":\"init"\"startstate:"/*ptr to startstate*/ << "}";
    } else {
      //TODO something else
      // to call in sim1step_notrace 
    }
  }

public:
  size_t fuel;
  bool valid;
  void (*RandWalker::sim1Step)();

  RandWalker(State_t startstate, unsigned int rand_seed_, size_t fuel/* =DEFAULT_FUEL */) 
    : state(startstate), 
      rand_seed(rand_seed_),
      init_rand_seed(rand_seed_),
      fuel(fuel),
      id(RandWalker::next_id++) 
  { 
    state.__rw__ = *this; /* provide a semi-hidden reference to this random walker for calling the property handlers */ 
    if (OPTIONS.do_trace) {
      json = new json_file_t(OPTIONS.trace_dir + '/' + std::to_string(rand_seed_) + ".json");
      sim1Step = sim1Step_trace;
           *json << ",{\"$type\":\"romp-trace\",\"metadata\":" << /*metadata*/ << "}";
           *json << ",{\"$trace\":\"Random walker\",\"$type\":\"init"\"startstate:"<</*ptr to startstate*/ << "state:"\<</**/}";

      //TODO: put in the initial json info
      //TODO: put in the startstate object into the trace section
    } else {
      sim1Step = sim1Step_no_trace;
    }
  } 

  ~RandWalker() { *json << "]}"; json->out.close(); delete json; if (tripped != nullptr) delete tripped; }


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
    //TODO: store the mutated state in the history <-- we don't store the old state we store an id_t referring to the rule applied
    const RuleSet& rs= rand_ruleset();
    const Rule& r= rand_rule(rs);
    bool pass = false;
    try {  
      if ((pass = r.guard(state)) == true)
        r.action(state);
      else {
        *json << ",{\"$type\":\"rule-failed\",\"rule\":" << r << "}";
        return;
      }
      for (const Property& prop : ::__caller__::PROPERTIES)
          if (prop.check(state) == false) {
              valid = false;
              tripped = new ModelPropertyError(prop);
          }        
    } catch(IModelError& me) {
      valid = false;
      tripped = me/* .clone() */; // need to look into this one, probs broken with std::nested_exceptions
      // TODO: handle error data
    } catch (std::exception& ex) {
      std::cerr << "unexpected exception outside of model \t[dev-error]\n" << ex.what() << std::endl;
    } catch (...) {
      std::cerr << "unexpected UNKNOWN exception outside of model \t[dev-error]\n";
    }
  }

  void sim1Step_no_trace() noexcept {
    
    //TODO: store the mutated state in the history <-- we don't store the old state we store an id_t referring to the rule applied
    const RuleSet& rs= rand_ruleset();
    const Rule& r= rand_rule(rs);
    fuel--;
    try {
      if (r.guard(state) == true)
        r.action(state);

      for (const Property& prop : ::__caller__::PROPERTIES)
          if (prop.check(state) == false) {
              valid = false;
              tripped = new ModelPropertyError(prop);
          }        
    } catch(IModelError& me) {
      valid = false;
      tripped = me/* .clone() */; // need to look into this one, probs broken with std::nested_exceptions
      // TODO: handle error data
    } catch (std::exception& ex) {
      std::err << "unexpected exception outside of model \t[dev-error]\n" << ex.what() << std::endl;
    } catch (...) {
      std::err << "unexpected UNKNOWN exception outside of model \t[dev-error]\n";
    }
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
    rws = gen_random_walkers(rw_count, root_seed, fuel);
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
    std::error << "\nModel raised an error when initializing our start state!! (message below)\n"
               << ex.what << std::endl;
    return;
  }
  RandWalker* rw = new RandWalker(start_state,rand_seed,fuel);
  while( rw->valid && rw->fuel > 0)
    rw->sim1step();

  //TODO: call sim1Step as RandWalker (rw) method
  //TODO: check if rw is still valid to run (check valid parameter & fuel level) -> make a decision
  //TODO: send rw off to have it's results presented to the user
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
