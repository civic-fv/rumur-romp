/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp-rw_pregen-fix.hpp
 * 
 * @brief just a bunch of things to get the language support working on our files without doing code gen.
 * 
 * @date 2022/06/29
 * @version 0.1
 */
#pragma once

#ifndef __romp__GENERATED_CODE

#ifndef STATE_HISTORY_SIZE
#define STATE_HISTORY_SIZE 16ul
#endif
#ifndef RULES_SIZE
#define RULES_SIZE 6ul
#endif
#ifndef INVARIANTS_SIZE 
#define INVARIANTS_SIZE 2ul
#endif
#ifndef STARTSTATES_SIZE
#define STARTSTATES_SIZE 2ul
#endif
#ifndef _ROMP_STATE_TYPE 
#define _ROMP_STATE_TYPE ::__model__::__State__
#endif

// pre-declaration of the state object romp will generate.
namespace __model__ {
  struct __State__;
}
#define __model__filename "<model-file>"
#define __model__filename_contains_space false

namespace romp {
  struct RuleSet;
  struct Rule;
  struct Invariant;
  // struct Invariant_info;
  struct StartState;
  // struct StartState_info;
}

namespace __caller__ { // LANGUAGE SERVER SUPPORT ONLY!!
 ::romp::RuleSet RULE_SETS[RULES_SIZE]; // LANGUAGE SERVER SUPPORT ONLY!!
 ::romp::Invariant INVARIANTS[INVARIANTS_SIZE]; // LANGUAGE SERVER SUPPORT ONLY!!
//  ::romp::Invariant_info INVARIANT_INFOS[INVARIANTS_SIZE]; // LANGUAGE SERVER SUPPORT ONLY!!
 ::romp::StartState STARTSTATES[STARTSTATES_SIZE]; // LANGUAGE SERVER SUPPORT ONLY!!
} // LANGUAGE SERVER SUPPORT ONLY !!

namespace __model__ { // LANGUAGE SERVER SUPPORT ONLY!!
 struct __State__ { // LANGUAGE SERVER SUPPORT ONLY !!
    size_t test1[16]; // LANGUAGE SERVER SUPPORT ONLY !!
 }; // LANGUAGE SERVER SUPPORT ONLY !!
} // LANGUAGE SERVER SUPPORT ONLY !!

#endif
