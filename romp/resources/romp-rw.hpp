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
#include "c_prefix.c"
#endif

#include <cstddef>
#include <string>
#include <vector>

// << =============================== Preprocessor Declarations ================================ >>


#ifndef STATE_HISTORY_SIZE
#define STATE_HISTORY_SIZE 16
#endif
#ifndef RULES_SIZE
#define RULES_SIZE 0
#endif
#ifndef INVARIANTS_SIZE 
#define INVARIANTS_SIZE 2
#endif
#ifndef STARTSTATES_SIZE
#define STARTSTATES_SIZE 0
#endif
#ifndef _ROMP_STATE_TYPE 
#define _ROMP_STATE_TYPE ::__model__::__State__
#endif

// << =================================== Type Declarations ==================================== >>
#ifndef __romp__GENERATED_CODE
namespace __model__ { // LANGUAGE SERVER SUPPORT ONLY!!
 class __State__ { // LANGUAGE SERVER SUPPORT ONLY !!
    size_t test1[16];
 };
}
#endif

namespace romp {

// typedef void* Param;

typedef struct {
  bool (*guard)(::__model__::__State__);
  void (*action)(::__model__::__State__);
} Rule;

typedef struct {
  bool (*check)(::__model__::__State__);
} Invariant;

typedef struct {
  void (*initialize)(::__model__::__State__);
} StartState;

}

#ifndef __romp__GENERATED_CODE
namespace __caller__ { // LANGUAGE SERVER SUPPORT ONLY!!
 ::romp::Rule* RULES[RULES_SIZE]; // LANGUAGE SERVER SUPPORT ONLY!!
 ::romp::Invariant INVARIANTS[INVARIANTS_SIZE]; // LANGUAGE SERVER SUPPORT ONLY!!
 ::romp::StartState STARTSTATES[STARTSTATES_SIZE]; // LANGUAGE SERVER SUPPORT ONLY!!
}
#endif


namespace romp {

class RandWalker {
public:
  static id_t next_id;
  const id_t id;
  const unsigned int rand_seed;
  _ROMP_STATE_TYPE state;
  
  bool valid;
  // tripped thing
  std::string tripped;
  // how many rules have tried to be applied to this state
  int level;
  // how many rules have actually been applied to this state
  int true_level;
  // array of intigers representing the rul ID's applied to this state (treated
  // as a circular buffer array)
  int history[STATE_HISTORY_SIZE];

  /**
   * @brief call if rule is applied to store what rule made the change in the
   * history circular buffer.
   * @param id the id of the rule that was applied.
   */
  void rule_applied(int id) {
    history[true_level % STATE_HISTORY_SIZE] = id;
    true_level++;
  }
  RandWalker(_ROMP_STATE_TYPE startstate, unsigned int rand_seed_) 
    : state(startstate), 
      rand_seed(rand_seed_),
      id(RandWalker::next_id++) 
  {}

}; //? END class RandomWalker

id_t RandWalker::next_id = 0u;


/**
 * @brief generate all startstates of the model. 
 * To do - how to get the size of startstate
 * 
 */
std::vector<_ROMP_STATE_TYPE> gen_startstates() {
  std::vector<_ROMP_STATE_TYPE> states;
  for (int i=0; i<STARTSTATES_SIZE; i++) {
    states.push_back(_ROMP_STATE_TYPE());
    ::__caller__::STARTSTATES[i].initialize(states[states.size()-1]);
  }
  return states;
}

/**
 * @brief to generate randomseeds for the no of random-walkers
 * rand is generated using UNIX timestamp 
 */
unsigned int get_random_seed(unsigned int root_seed){
  return rand_seed(root_seed); // this should just gen a rand int between 0 and max unsigned int
}

/**
 * @brief generate all startstates of the model. 
 * To do - how to get the size of startstate
 * 
 */
std::vector<RandWalker> gen_random_walkers(size_t rw_count, unsigned int root_seed) {
  std::vector<RandWalker> rws;
  auto startstates = gen_startstates();
  for(int i=0; i<rw_count; i++) {
    auto copied_startstate = _ROMP_STATE_TYPE(startstates[i%startstates.size()]);
    RandWalker rw1(copied_startstate, get_random_seed(root_seed));
  }
  return rws;
}

// example of how to get a copy of an object in C++:
//   use the copy constructor (always has the signature)
//    ClassName(const ClassName &obj)
//  this means your's should look like:
//    auto copied_state = _ROMP_STATE_TYPE(startstates[i%startstates.size()]);


/**
 * @brief to generate randomseeds for the no of random-walkers
 * rand is generated using UNIX timestamp 
 */
std::vector<unsigned int> genrandomseed(){
  rand_seed *rand_seed = new rand_seed[randomwalker];
  srand(time(NULL));
  for(int i=0;i<randomwalker;i++)
      rand_seed[randomwalker] = new rand_seed(randomwalker);
  return rand_seed;
}

/**
 * @brief implementing randomwalk_parallel simulation which has the threads 
 * and no of random-walkers specified by the user options .
 */
void launch_OpenMP(int thread_count, int rw_count) {
  
  
}

void launch_CUDA();

void launch_SYCL();

void launch_OpenMPI();

void launch_single() {

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
