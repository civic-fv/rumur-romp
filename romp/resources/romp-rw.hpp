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

// #ifndef __romp__GENERATED_CODE
#include <cstddef>
#include <functional>
#include <string>
#include <iterator>
// #endif

// << =============================== Preprocessor DEclarations
// ================================ >>

#define STATE_HISTORY_SIZE 16
#ifndef RULES_SIZE
#define RULES_SIZE 0
#endif
#ifndef INVARIANTS_SIZE 2
#define RULE_COUNT 0
#endif
#ifndef STARTSTATES_SIZE 2
#define RULE_COUNT 0
#endif

// << =================================== Type Declarations ==================================== >>
namespace __model__ {
 class __State__;
}

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

namespace __caller__ {
 ::romp::Rule* RULES[RULES_SIZE];
 ::romp::Invariant* INVARIANTS[INVARIANTS_SIZE];
 ::romp::StartState* STARTSTATES[STARTSTATES_SIZE];
}


namespace romp {

class RandWalker {
public:
  static id_t next_id;
  const id_t id;
  
  ::__model__::__State__* state;
  // was this state tripped
  bool valid;
  // tripped thing
  std::string tripped;
  // how many rules have tried to be applied to this state
  int level;
  // how many rules have actually been applied to this state
  int true_level;
  // array of integers representing the rul ID's applied to this state (treated
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
  RandWalker() : id(RandWalker::next_id++) {}


  void Sim1Step(RandWalker::__State__ state, Rule rule, size_t state_count) {
    for (int s = 0; s < state_count; s++)
      if (rule.Guard(states[s])) {
        rule.Run(states[s]);
        states[i].rule_applied(
            rule.id); // this keeps track of history and other overhead
        // These could possibly be parallelized even better for GPU using
        // shared memory. (not implemented here, could introduce data races)
        for (int i = 0; i < INVARIANTS_SIZE; i++)
          states[s].valid |= __State__::INVARIANTS[i](
              states[i]); // this will need to change after we figure shit out
        for (int a = 0; a < ASSERTIONS_SIZE; a++)
          states[s].valid |= __State__::ASSERTIONS[a](
              states[i]); // this will need to change after we figure shit out
      }
  }
}
}; // namespace romp

Sim::next_id = 0;




// <<
// ==========================================================================================
// >>
// <<                                         ROMP CODE >>
// <<
// ==========================================================================================
// >>

// template<class S, class R>
