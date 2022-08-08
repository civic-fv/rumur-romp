/*
	Generated code for a romp "parallel random walker" verification tool based off of the Murphi Model described in:
		Original Murphi Model: romp/tests/client.m
	Please build with the following command(s):
		<not-implemented-yet>
*/


#define __romp__GENERATED_CODE


#define _ROMP_STATE_TYPE ::__model__::__Model__


#define _ROMP_STATE_HISTORY_LEN (4ul)


#define _ROMP_THREAD_TO_RW_RATIO (8ul)


#define __model__filename "romp/tests/client.m"


#define __model__filename_contains_space (false)


#pragma region inline_library_includes


#pragma endregion inline_library_includes

namespace __model__ { struct __Model__; } // helpful pre-definition


#pragma region model_prefixes

#include "/home/vinuj/Desktop/Repos/rumur-romp/romp/resources/c_prefix.cpp"

#pragma endregion model_prefixes



#pragma region generated_code



/* ======= Model Type & Const Definitions ====== */

namespace __type__ {
  typedef int range_t;
  typedef unsigned int scalarset_t;
  const __typeof__(((int)(2))) clientNUMS = ((int)(2));
  typedef enum { I, T, C, E, } state;

  typedef range_t client;

  typedef struct { ::__type__::state data[2]; size_t size() {return (size_t)(2);} } anon0x0_t;


}



/* ======= Generated Model & State ====== */

namespace __model__ {

  struct __Model__ {
    ::romp::IRandWalker* __rw__; // reference to associated romp rand walker


    /* ======= Model State Variables ====== */

    ::__type__::anon0x0_t n;

    ::__type__::boolean x;


    /* ======= Murphi Model Functions ====== */


    /* ======= Murphi Model StartStates, Rules & Global Properties ====== */

     void __StartState__startstate1(void){
      using namespace ::__type__;
      try {
        do {
          for (int i = ((int)(1)); i <= (clientNUMS); i++) {
            ((n).data[((i)) - int_VALUE_C(1)]) = (I);
          }
        } while (0);
        (x) = (true);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelStartStateError(0) ); }
    }



    
    bool __Rule_guard__Try(const ::__type__::client i) const {
      using namespace ::__type__;
      try {
        return (((n).data[((i)) - int_VALUE_C(1)]) == (I));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Try(const ::__type__::client i) {
      using namespace ::__type__;
      try {
        ((n).data[((i)) - int_VALUE_C(1)]) = (T);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Crit(const ::__type__::client i) const {
      using namespace ::__type__;
      try {
        return ((((n).data[((i)) - int_VALUE_C(1)]) == (T)) && ((x) == (true)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Crit(const ::__type__::client i) {
      using namespace ::__type__;
      try {
        ((n).data[((i)) - int_VALUE_C(1)]) = (C);
        (x) = (false);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Exit(const ::__type__::client i) const {
      using namespace ::__type__;
      try {
        return (((n).data[((i)) - int_VALUE_C(1)]) == (C));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Exit(const ::__type__::client i) {
      using namespace ::__type__;
      try {
        ((n).data[((i)) - int_VALUE_C(1)]) = (E);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Idle(const ::__type__::client i) const {
      using namespace ::__type__;
      try {
        return (((n).data[((i)) - int_VALUE_C(1)]) == (E));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Idle(const ::__type__::client i) {
      using namespace ::__type__;
      try {
        ((n).data[((i)) - int_VALUE_C(1)]) = (I);
        (x) = (true);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::ACTION) ); }
    }


    
    bool __Property__coherence(const ::__type__::client i, const ::__type__::client j) const {
      using namespace ::__type__;
      try {
        return this->__rw__->invariant_handler((!((i) != (j)) || (!(((n).data[((i)) - int_VALUE_C(1)]) == (C)) || (((n).data[((j)) - int_VALUE_C(1)]) != (C)))),0u);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelPropertyError(0) ); }
    }


  };

  //TODO: make the json and stream operators for the state class
template<class O> ::romp::ojstream<O>& operator << (::romp::ojstream<O>& out, const ::__model__::__Model__& s) {return (out << "\"``json state output'' NOT YET IMPLEMENTED\""); }
  ::std::ostream& operator << (::std::ostream& out, const ::__model__::__Model__& s) {return (out << "``plain text state output'' NOT YET IMPLEMENTED"); }
}

/* ======= Murphi Model Infos & MetaData ====== */
namespace __info__ {

/* the number of functions & procedures in the model */
#define _ROMP_FUNCTS_LEN (0ul)
  /* the info/metadata about the functions/procedures in the model */
  const ::romp::FunctInfo FUNCT_INFOS[_ROMP_FUNCTS_LEN] = {};
/* the number of error statements in the model */
#define _ROMP_ERRORS_LEN (0ul)
  /* the info/metadata about the murphi error statements in the model */
  const ::romp::MErrorInfo ERROR_INFOS[_ROMP_ERRORS_LEN] = {};
/* the number of property statements & rules in the model */
#define _ROMP_PROPERTIES_LEN (1ul)
  /* the info/metadata about the murphi properties in the model */
  const ::romp::PropertyInfo PROPERTY_INFOS[_ROMP_PROPERTIES_LEN] = {{"coherence",::romp::PropertyType::INVARIANT,"((i != j) -> ((n[i] = C) -> (n[j] != C)))",0,{{37,1},{38,36}},"{\"$type\":\"property\",\"type\":\"invariant\",\"expr\":\"((i != j) -> ((n[i] = C) -> (n[j] != C)))\",\"label\":\"coherence\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/client.m\",\"start\":[37,1],\"end\":[38,36]}"},};
/* the number of start state rules (before ruleset expansions) in the model */
#define _ROMP_STARTSTATE_INFO_LEN (1ul)
  /* the info/metadata about the startstate rules in the model */
  const ::romp::StartStateInfo STARTSTATE_INFOS[_ROMP_STARTSTATE_INFO_LEN] = {{"startstate1",{{28,1},{34,14}},"{\"$type\":\"startstate\",\"label\":\"startstate1\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/client.m\",\"start\":[28,1],\"end\":[34,14]}"},};
/* the number of rules in the model, BEFORE ruleset expansion */
#define _ROMP_RULESETS_LEN (4ul)
  /* the info/metadata about the rules in the model */
  const ::romp::RuleInfo RULE_SET_INFOS[_ROMP_RULESETS_LEN] = {{"Try",{{11,1},{12,24}},"{\"$type\":\"rule\",\"expr\":\"(n[i] = I)\",\"label\":\"Try\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/client.m\",\"start\":[11,1],\"end\":[12,24]}"},{"Crit",{{14,1},{16,37}},"{\"$type\":\"rule\",\"expr\":\"((n[i] = T) & (x = true))\",\"label\":\"Crit\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/client.m\",\"start\":[14,1],\"end\":[16,37]}"},{"Exit",{{18,1},{20,24}},"{\"$type\":\"rule\",\"expr\":\"(n[i] = C)\",\"label\":\"Exit\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/client.m\",\"start\":[18,1],\"end\":[20,24]}"},{"Idle",{{23,1},{25,24}},"{\"$type\":\"rule\",\"expr\":\"(n[i] = E)\",\"label\":\"Idle\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/client.m\",\"start\":[23,1],\"end\":[25,24]}"},};

}

/* ======= Romp CALLER Lists ====== */
namespace __caller__ {
  typedef ::__model__::__Model__ State_t; // type mask for the generated state object
  /* --- Rules Generated by: `Try` (RuleSet expansions) --- */
   bool __Rule_guard__Try__0(const State_t& s){return s.__Rule_guard__Try((::__type__::client) 1); }
   void __Rule_action__Try__0(State_t& s){s.__Rule_action__Try((::__type__::client) 1); }
   bool __Rule_guard__Try__1(const State_t& s){return s.__Rule_guard__Try((::__type__::client) 2); }
   void __Rule_action__Try__1(State_t& s){s.__Rule_action__Try((::__type__::client) 2); }
  /* --- Rules Generated by: `Crit` (RuleSet expansions) --- */
   bool __Rule_guard__Crit__0(const State_t& s){return s.__Rule_guard__Crit((::__type__::client) 1); }
   void __Rule_action__Crit__0(State_t& s){s.__Rule_action__Crit((::__type__::client) 1); }
   bool __Rule_guard__Crit__1(const State_t& s){return s.__Rule_guard__Crit((::__type__::client) 2); }
   void __Rule_action__Crit__1(State_t& s){s.__Rule_action__Crit((::__type__::client) 2); }
  /* --- Rules Generated by: `Exit` (RuleSet expansions) --- */
   bool __Rule_guard__Exit__0(const State_t& s){return s.__Rule_guard__Exit((::__type__::client) 1); }
   void __Rule_action__Exit__0(State_t& s){s.__Rule_action__Exit((::__type__::client) 1); }
   bool __Rule_guard__Exit__1(const State_t& s){return s.__Rule_guard__Exit((::__type__::client) 2); }
   void __Rule_action__Exit__1(State_t& s){s.__Rule_action__Exit((::__type__::client) 2); }
  /* --- Rules Generated by: `Idle` (RuleSet expansions) --- */
   bool __Rule_guard__Idle__0(const State_t& s){return s.__Rule_guard__Idle((::__type__::client) 1); }
   void __Rule_action__Idle__0(State_t& s){s.__Rule_action__Idle((::__type__::client) 1); }
   bool __Rule_guard__Idle__1(const State_t& s){return s.__Rule_guard__Idle((::__type__::client) 2); }
   void __Rule_action__Idle__1(State_t& s){s.__Rule_action__Idle((::__type__::client) 2); }

/* the number of rules in the model, AFTER ruleset expansion */
#define _ROMP_RULE_COUNT (8ul)


  // All of the rule sets in one place
  const ::romp::RuleSet RULESETS[_ROMP_RULESETS_LEN] = {
		{::__info__::RULE_SET_INFOS[0],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Try__0,__Rule_action__Try__0,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__Try__1,__Rule_action__Try__1,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[1],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Crit__0,__Rule_action__Crit__0,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__Crit__1,__Rule_action__Crit__1,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[2],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Exit__0,__Rule_action__Exit__0,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__Exit__1,__Rule_action__Exit__1,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[3],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Idle__0,__Rule_action__Idle__0,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__Idle__1,__Rule_action__Idle__1,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":2}}]","i:=2"}  	}}};

  /* --- Property Rule(s) generated by: `coherence` (RuleSet expansions) --- */
   bool __Property__coherence__0(const State_t& s) {return s.__Property__coherence((::__type__::client) 1, (::__type__::client) 1); }
   bool __Property__coherence__1(const State_t& s) {return s.__Property__coherence((::__type__::client) 1, (::__type__::client) 2); }
   bool __Property__coherence__2(const State_t& s) {return s.__Property__coherence((::__type__::client) 2, (::__type__::client) 1); }
   bool __Property__coherence__3(const State_t& s) {return s.__Property__coherence((::__type__::client) 2, (::__type__::client) 2); }

#ifdef __romp__ENABLE_assume_property
#define ___propRule_assume_count___ (0ul)
#else
#define ___propRule_assume_count___ (0ul)
#endif
#ifdef __romp__ENABLE_cover_property
#define ___propRule_cover_count___ (0ul)
#else
#endif
#define ___propRule_cover_count___ (0ul)
#ifdef __romp__ENABLE_liveness_property
#define ___propRule_liveness_count___ (0ul)
#else
#define ___propRule_liveness_count___ (0ul)
#endif

/* the number of property rules (after ruleset expansion) in the model */
#define _ROMP_PROPERTY_RULES_LEN ((4ul) + ___propRule_assume_count___ + ___propRule_cover_count___ + ___propRule_liveness_count___)

  /* All of the property rules expanded in one place */
const ::romp::Property PROPERTIES[_ROMP_PROPERTY_RULES_LEN] = {
		{__Property__coherence__0,::__info__::PROPERTY_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":1}},{\"$type\":\"kv-pair\",\"key\":\"j\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":1}}]","i:=1; j:=1"},
		{__Property__coherence__1,::__info__::PROPERTY_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":1}},{\"$type\":\"kv-pair\",\"key\":\"j\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":2}}]","i:=1; j:=2"},
		{__Property__coherence__2,::__info__::PROPERTY_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":2}},{\"$type\":\"kv-pair\",\"key\":\"j\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":1}}]","i:=2; j:=1"},
		{__Property__coherence__3,::__info__::PROPERTY_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":2}},{\"$type\":\"kv-pair\",\"key\":\"j\",\"value\":{\"$type\":\"range-value\",\"type\":\"1..clientNUMS\",\"value\":2}}]","i:=2; j:=2"},
};

  /* --- StartState Rule(s) generated by: `startstate1` (RuleSet expansions) --- */
   void __StartState__startstate1__0(State_t& s){ s.__StartState__startstate1(); }


#define _ROMP_STARTSTATES_LEN (1ul) // the number of property rules (after ruleset expansion) in the model

  // All of the rule sets in one place
const ::romp::StartState STARTSTATES[_ROMP_STARTSTATES_LEN] = {{__StartState__startstate1__0,::__info__::STARTSTATE_INFOS[0],0,"[]",""}};


}



#pragma endregion generated_code


#pragma region romp_rw

#include "/home/vinuj/Desktop/Repos/rumur-romp/romp/resources/romp-rw.hpp"

#pragma endregion romp_rw


#pragma region romp_rw_options

#include "/home/vinuj/Desktop/Repos/rumur-romp/romp/resources/romp-rw-options.hpp"

#pragma endregion romp_rw_options


#pragma region romp_rw_main

#include "/home/vinuj/Desktop/Repos/rumur-romp/romp/resources/romp-rw-main.hpp"

#pragma endregion romp_rw_main
