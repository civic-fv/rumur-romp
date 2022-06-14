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
#include <algorithm>

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
namespace romp {

// << =================================== Type Declarations ==================================== >>


class Param {
  id_t id;

};

class IterParam : public Param {

    //TODO: Figure this out later

};

/**
 * @brief Abstract parent of all Param class objects.
 *        Implements the base ideas of programmatic permutations for enabling us to track and call 
 *        the various parametric changes rulesets effect onto Murphi generated functions 
 *        for rules, invariants & start-state expressions. 
 */
class RuleSet {
protected:
  uint32_t max_hash;
public:
  RuleSet(int* signature, int param_c) {
    max_hash = 1;
    for (int i=0; i<2*param_c; i+=2)
      max_hash *= signature[i+1] - signature[i];
  }
};


/**
 * @brief an Abstract Param object for working with the parameters for the rules of a Murphi Model.
 *        It (will later) hold the functionality for performing Ganesh's pseudo symmetric reduction.
 */
class RuleSet_Rule : public RuleSet {


};


/**
 * @brief An Abstract Param object for working with the parameters 
 *        of Murphi model \c Invariant and \c StartState functions.
 *        It differs from that of rules in that it is iterable, and is meant to 
 *        allow you to go through all the options easily in a for-range loop.
 * 
 *        FLAWED AND CURRENTLY NON-FUNCTIONAL
 */
class IterRuleSet : public RuleSet {
private: 
  uint32_t iter = 0;
public:
  IterRuleSet* begin() { iter=0; return this; }
  IterRuleSet* end() { return NULL; }
  IterRuleSet* operator++() { iter++; return ((iter<max_hash) ? this : NULL); }
};


class RandWalker {
public:
  static id_t next_id;
  const id_t id;
  class State; // do not remove me
  class startstate;
  startstate *startstate; //storing the startstate of model 
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
  RandWalker() : id(RandWalker::next_id++) {}

void random_walker_simulation(int thread_count, int randomwalker)
/**
 * @brief implementing randomwalk_parallel simulation which has the threads 
 * and no of randomwalkers specified by the user options .
 * 
 */
{
  startstate* genstartstate()
  /**
   * @brief generate all startstates of the model. 
   * To do - how to get the size of startstate
   * 
   */
  {
    startstate *startstate = new startstate[/*how to get the no of startstate*/];
    for (int i=0;i<size;i++)
      startstate[size] = new startstate(size);
    return startstate;
  }
 if(size<randomwalker)
 /**
  * @brief duplicate the startstate 
  * must get randomwalkers startstates-->to do 
  * 
  */
 {
   startstate *startstate1 = new startstate[size];
   std::copy(std::begin(startstate),std::end(startstate), std::begin(startstate1));
 }


 int main()
{
    const int n = 5;
    int src[] = {1, 3, 5, 7, 9};
 
    int dest[n];
    std::copy(std::begin(src), std::end(src), std::begin(dest));
 
    for (int &i: dest) {
        std::cout << i << ' ';
    }
 
    return 0;
}


}


  void Sim1Step(RandWalker::State state, Rule rule, size_t state_count) {
    for (int s = 0; s < state_count; s++)
      if (rule.Guard(states[s])) {
        rule.Run(states[s]);
        states[i].rule_applied(
            rule.id); // this keeps track of history and other overhead
        // These could possibly be parallelized even better for GPU using
        // shared memory. (not implemented here, could introduce data races)
        for (int i = 0; i < INVARIANTS_SIZE; i++)
          states[s].valid |= State::INVARIANTS[i](
              states[i]); // this will need to change after we figure shit out
        for (int a = 0; a < ASSERTIONS_SIZE; a++)
          states[s].valid |= State::ASSERTIONS[a](
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
