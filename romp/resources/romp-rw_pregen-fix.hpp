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

#ifndef _ROMP_STATE_HISTORY_LENSIZE
#define _ROMP_STATE_HISTORY_LEN 16ul
#endif
#ifndef _ROMP_RULESETS_LEN
#define _ROMP_RULESETS_LEN 6ul
#endif
#ifndef _ROMP_PROPERTIES_LEN 
#define _ROMP_PROPERTIES_LEN 2ul
#endif
#ifndef _ROMP_PROPERTY_RULES_LEN 
#define _ROMP_PROPERTY_RULES_LEN 2ul
#endif
#ifndef _ROMP_LIVENESS_TESTS_LEN
#define _ROMP_LIVENESS_TESTS_LEN 2ul
#endif
#ifndef _ROMP_STARTSTATES_LEN
#define _ROMP_STARTSTATES_LEN 2ul
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
  template<class O> struct ojstream; 
  struct RuleInfo;
  struct RuleSet;
  struct Rule;
  struct PropertyInfo;
  struct Property;
  // struct Property_info;
  // typedef const std::string Property_info;
  struct StartStateInfo;
  struct StartState;
  class IRandWalker;
  // struct StartState_info;
}

namespace __caller__ { // LANGUAGE SERVER SUPPORT ONLY!!
 const ::romp::RuleSet RULE_SETS[_ROMP_RULES_LEN]; // LANGUAGE SERVER SUPPORT ONLY!!
 const ::romp::Property PROPERTIES[_ROMP_PROPERTY_RULES_LEN]; // LANGUAGE SERVER SUPPORT ONLY!!
 const ::romp::StartState STARTSTATES[_ROMP_STARTSTATES_LEN]; // LANGUAGE SERVER SUPPORT ONLY!!
} // LANGUAGE SERVER SUPPORT ONLY !!

namespace __model__ { // LANGUAGE SERVER SUPPORT ONLY!!
  struct __State__ { // LANGUAGE SERVER SUPPORT ONLY !!
    ::romp::IRandWalker* __rw__; // LANGUAGE SERVER SUPPORT ONLY !!
    size_t test1[16]; // LANGUAGE SERVER SUPPORT ONLY !!
  }; // LANGUAGE SERVER SUPPORT ONLY !!
  template<class O> ::romp::ojstream<O>& operator << (::romp::ojstream<O>& json, const __State__& s);
} // LANGUAGE SERVER SUPPORT ONLY !!

#endif