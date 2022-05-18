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

#ifdef __romp__PREBUILD
#include <cstddef>
#include <functional>
#include <string>
#endif

// << =============================== Preprocessor DEclarations
// ================================ >>

#define STATE_HISTORY_SIZE 16
#ifndef RULES_SIZE
#define RULES_SIZE 0
#endif
#ifndef INVARIANTS_SIZE
#define RULE_COUNT 0
#endif
namespace romp {

// << =================================== Type Declarations
// ==================================== >>

class Sim {
public:
  static id_t next_id;
  const id_t id;
  class State; // do not remove me
  State *state;
  // was this state tripped
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
  Sim() : id(Sim::next_id++) {}

  void Sim1Step(Sim::State state, Rule rule, size_t state_count) {
    for (int s = 0; s < state_count; s++)
      if (rule.Guard(states[s])) {
        rule.Run(states[s]);
        states[i].rule_applied(
            rule.id); // this keeps track of history and other overhead
        // These could possibly be parrallelized even better for GPU using
        // shared memory. (not implimented here, could introduce data races)
        for (int i = 0; i < INVARIANTS_SIZE; i++)
          states[s].valid |= INVARIANTS[i](
              states[i]); // this will need to change after we figure shit out
        for (int a = 0; a < ASSERTIONS_SIZE; a++)
          states[s].valid |= ASSERTIONS[a](
              states[i]); // this will need to change after we figure shit out
      }
  }
}
}; // namespace romp

Sim::next_id = 0;

template <class SIG> class Rule {
public:
  std::function<bool(S)> Guard;
  std::function<void(S)> Run;
};

// << ================================ Extern Predeclarations
// ================================== >>
#ifdef __romp__PREBUILD
extern Rule RULES[];
extern std::function<bool(ProtoState)> INVARIANTS[];
extern std::function<bool(ProtoState)> ASSERTIONS[];
extern ::State * ::GenStartStates();
#endif

// stuct RS {
//   function<bool(State,Params)> guard;
//   function<void(State,Params)> action;
//   Param_t **params;
//   Type Param_t;
// };

// <<
// ==========================================================================================
// >>
// <<                                         ROMP CODE >>
// <<
// ==========================================================================================
// >>

// template<class S, class R>
