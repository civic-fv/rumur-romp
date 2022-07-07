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

class RandWalker {
public:
  static id_t next_id;
  const id_t id;
  const unsigned int init_rand_seed;
  unsigned int rand_seed;
  State_t state;
  size_t fuel;
  bool valid;
  // tripped thing
  ModelError* tripped = nullptr;
  // how many rules have tried to be applied to this state
  size_t level = 0;
  // array of integers representing the rul ID's applied to this state (treated
  // as a circular buffer array)
  id_t history[STATE_HISTORY_SIZE];
  

  RandWalker(State_t startstate, unsigned int rand_seed, size_t fuel/* =DEFAULT_FUEL */) 
    : state(startstate), 
      rand_seed(rand_seed),
      init_rand_seed(rand_seed),
      fuel(fuel),
      id(RandWalker::next_id++) 
  {} 

  /**
   * @brief to pick a rule in random for simulation step
   * 
   */
  RuleSet& rand_ruleset(){
    //returns a  
  }
  /**
   * @brief to pick a rule in random for simulation step
   * 
   */
  Rule& rand_rule(const RuleSet& rs){
    //returns a  
  
  /**
   * @brief call if rule is applied to store what rule made the change in the
   * history circular buffer.
   * @param id the id of the rule that was applied.
   */
  void rule_applied(id_t id) {
    history[level % STATE_HISTORY_SIZE] = id;
    level++;
  }
  
  void sim1Step() {
    //TODO: store the mutated state in the history <-- we don't store the old state we store an id_t referring to the rule applied
    RuleSet& rs= rand_ruleset();
    Rule& r= rand_rule(rs);
    fuel--;
    try {
      if (r.guard(state) == true) //how to pass this rules and guard ?? for every state ?
          r.action(state);
      for (int i = 0; i < INVARIANTS_SIZE; i++)
          if (::__caller__::INVARIANTS[i].check(state) == false) {
              valid = false;
              tripped = new ModelPropertyError(PropertyType::INVARIANT, "", INVARIANTS[i].loc, INVARIANT_INFOS[i].expression);
          }           
    } catch(ModelError& ex) {
      valid = false;
      tripped = ex; // need to look into this one, probs broken with std::nested_exceptions
      // TODO: handle error data
    }
  }

  ~RandWalker() {
    if (tripped != nullptr) delete tripped;
  }
}; //? END class RandomWalker

id_t RandWalker::next_id = 0u;


/**
 * @brief generate all startstates of the model. 
 * To do - how to get the size of startstate
 * 
 */
std::vector<State_t> gen_startstates() throw (ModelError) {
  std::vector<State_t> states;
  for (int i=0; i<STARTSTATES_SIZE; i++) {
    states.push_back(State_t());
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
std::vector<RandWalker> gen_random_walkers(size_t rw_count, unsigned int root_seed, size_t fuel) throw (ModelError) {
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
 * @brief helper function rand_choice 
 * 
 */
template<typename T>
T rand_choice(unsigned int &rand_seed, T min, T max,string seed_input) {
    //to be fed in from the  commandline 
    int seed;
    if(seed_input.length() != 0)
    {
        seed =stoi(seed_input);
    else 
        srand(time(0));
    }
    //not done fully
    seed = (((seed ^ (seed >> 3)) >> 12) & 0xffff) | ((seed & 0x7fff) << 16); // modifies the seed
    int choice = seed % (maxWeight-min+1) + min;  // generates the random number
    cout<<"seed is"<<seed<<endl;
    //cout<<"choice is "<<choice<<endl;
    return 0;
}


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
  } catch (const ModelError& ex) {
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
    ::__caller__::STARTSTATES[rand_choice(_seed_cpy, 0ul, STARTSTATES_SIZE)].initialize(start_state);
  } catch (const ModelError& ex) {
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
//       for (int i = 0; i < INVARIANTS_SIZE; i++)
//         states[s].valid |= State::INVARIANTS[i](
//             states[i]); // this will need to change after we figure shit out
//       for (int a = 0; a < ASSERTIONS_SIZE; a++)
//         states[s].valid |= State::ASSERTIONS[a](
//             states[i]); // this will need to change after we figure shit out
//     }
// }


// << ========================================================================================== >> 
// <<                                         ROMP CODE                                          >> 
// << ========================================================================================== >> 
}
// template<class S, class R>
