/*
	Generated code for a romp "parallel random walker" verification tool based off of the Murphi Model described in:
		Original Murphi Model: romp/tests/arbiter.m
	Please build with the following command(s):
		<not-implemented-yet>
*/


#define __romp__GENERATED_CODE


#define _ROMP_STATE_TYPE ::__model__::__Model__


#define _ROMP_STATE_HISTORY_LEN (4ul)


#define _ROMP_THREAD_TO_RW_RATIO (8ul)


#define __model__filename "romp/tests/arbiter.m"


#define __model__filename_contains_space (false)


#pragma region inline_library_includes


#pragma endregion inline_library_includes

namespace __model__ { struct __Model__; } // helpful pre-definition


#pragma region model_prefixes

#include "/home/vinuj/Desktop/Repos/rumur-romp/romp/resources/c_prefix.cpp"

#pragma endregion model_prefixes



#pragma region generated_code


/* ======= Header Comments ====== */


/* ======= Model Type & Const Definitions ====== */

namespace __type__ {
  typedef int range_t;
  typedef unsigned int scalarset_t;
  typedef int enum_backend_t;
  const __typeof__(((int)(4))) n = ((int)(4)); // Number of units
  typedef struct { ::__type__::boolean data[4]; size_t size() {return (size_t)(4);} } reg;

  typedef range_t anon0x0_t; // tk[u] == u receives token

  typedef struct { ::__type__::boolean data[5]; size_t size() {return (size_t)(5);} } anon0x1_t;

  typedef range_t anon0x2_t; // rules of units

  typedef range_t anon0x3_t; // rules of arbiters


}



/* ======= Generated Model & State ====== */

namespace __model__ {

  struct __Model__ {
    ::romp::IRandWalker* __rw__; // reference to associated romp rand walker


    /* ======= Model State Variables ====== */

    ::__type__::reg req; // req[u] == u requests the resource

    ::__type__::reg use; // use[u] == u uses the resource

    ::__type__::reg grant; // grant[u] == access is granted to u

    ::__type__::anon0x1_t tk;

    ::__type__::reg flop; // flop[u] == req[u] at last tk[u]

    ::__type__::reg ntk; // ntk[u] == u has the token

    ::__type__::boolean Req_b; // at least one unit requests the resource

    ::__type__::boolean Use_b; // at least one unit uses the resource

    ::__type__::reg p_req; // the value of "req" at last arbitration request


    /* ======= Murphi Model Functions ====== */

    
    void NewToken(void)  {
      using namespace ::__type__;
      try {
        //-----------------------------------------------------------------------
        // Copyright (C) 1992, 1993 by the Board of Trustees of 		 
        // Leland Stanford Junior University.					 
        //									 
        // This description is provided to serve as an example of the use	 
        // of the Murphi description language and verifier, and as a benchmark	 
        // example for other verification efforts.				 
        //									 
        // License to use, copy, modify, sell and/or distribute this description 
        // and its documentation any purpose is hereby granted without royalty,  
        // subject to the following terms and conditions, provided		 
        //									 
        // 1.  The above copyright notice and this permission notice must	 
        // appear in all copies of this description.				 
        // 									 
        // 2.  The Murphi group at Stanford University must be acknowledged	 
        // in any publication describing work that makes use of this example. 	 
        // 									 
        // Nobody vouches for the accuracy or usefulness of this description	 
        // for any purpose.							 
        //-----------------------------------------------------------------------
        //--------------------------------------------------------------------
        // Filename: 	arbiter.m
        // Content:	Hardware arbiter, travelling token, synchronous control
        //		A design that can Deadlock.
        // Version:	Murphi 2.3
        // Engineer:	Nicolas Halbwachs, 1992
        //--------------------------------------------------------------------
        // Number of units
        ((tk).data[(((int)(0))) - int_VALUE_C(0)]) = (true); // a new token is emitted
        // the current requests are recorded for checking the priority rule
        do {
          for (int v = ((int)(0)); v <= ((n) - ((int)(1))); v++) {
            ((p_req).data[((v)) - int_VALUE_C(0)]) = ((req).data[((v)) - int_VALUE_C(0)]);
          }
        } while (0);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(0) ); }
}


    /* ======= Murphi Model StartStates, Rules & Global Properties ====== */

     void __StartState__startstate2(void){
      using namespace ::__type__;
      try {
        // exclusive access
        /* Doesn't work, why??
         assert forall v : 0..u-1 do !p_req[v] end; -- priority rule
         */
        // reactivity
        // exclusive access
        do {
          for (int u = ((int)(0)); u <= ((n) - ((int)(1))); u++) {
            ((req).data[((u)) - int_VALUE_C(0)]) = (false);
            ((use).data[((u)) - int_VALUE_C(0)]) = (false);
            ((grant).data[((u)) - int_VALUE_C(0)]) = (false);
            ((flop).data[((u)) - int_VALUE_C(0)]) = (false);
            ((tk).data[((u)) - int_VALUE_C(0)]) = (false);
            ((ntk).data[((u)) - int_VALUE_C(0)]) = (false);
            ((p_req).data[((u)) - int_VALUE_C(0)]) = (false);
          }
        } while (0);
        (Req_b) = (false);
        (Use_b) = (false);
        ((tk).data[((n)) - int_VALUE_C(0)]) = (false);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelStartStateError(0) ); }
    }



    
    bool __Rule_guard__start_requesting(const ::__type__::anon0x2_t u) const {
      using namespace ::__type__;
      try {
        return ((!((req).data[((u)) - int_VALUE_C(0)])) && (!((use).data[((u)) - int_VALUE_C(0)])));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__start_requesting(const ::__type__::anon0x2_t u) {
      using namespace ::__type__;
      try {
        ((req).data[((u)) - int_VALUE_C(0)]) = (true);
        if ((!(Req_b)) && (!(Use_b))) {
          ::__model__::__Model__::NewToken();
        }
        (Req_b) = (true);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__start_using(const ::__type__::anon0x2_t u) const {
      using namespace ::__type__;
      try {
        return ((grant).data[((u)) - int_VALUE_C(0)]);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__start_using(const ::__type__::anon0x2_t u) {
      using namespace ::__type__;
      try {
        if (this->__rw__->assertion_handler((!(Use_b)),1u)) throw ::romp::ModelPropertyError(1);

        if (this->__rw__->assertion_handler(((req).data[((u)) - int_VALUE_C(0)]),2u)) throw ::romp::ModelPropertyError(2);

        ((use).data[((u)) - int_VALUE_C(0)]) = (true);
        ((req).data[((u)) - int_VALUE_C(0)]) = (false);
        (Use_b) = (true);
        ((grant).data[((u)) - int_VALUE_C(0)]) = (false);
        (Req_b) = ({ bool res_ = false; for (int v = ((int)(0)); v <= ((n) - ((int)(1))); v++) { res_ |= ((req).data[((v)) - int_VALUE_C(0)]); } res_; });
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__end_using(const ::__type__::anon0x2_t u) const {
      using namespace ::__type__;
      try {
        return ((use).data[((u)) - int_VALUE_C(0)]);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__end_using(const ::__type__::anon0x2_t u) {
      using namespace ::__type__;
      try {
        ((use).data[((u)) - int_VALUE_C(0)]) = (false);
        (Use_b) = ({ bool res_ = false; for (int v = ((int)(0)); v <= ((n) - ((int)(1))); v++) { res_ |= ((use).data[((v)) - int_VALUE_C(0)]); } res_; });
        if (this->__rw__->assertion_handler((!(Use_b)),3u)) throw ::romp::ModelPropertyError(3);

        if ((!(Req_b)) && (!(Use_b))) {
          ((tk).data[(((int)(0))) - int_VALUE_C(0)]) = (true);
        }
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__D_flip_flop(const ::__type__::anon0x3_t u) const {
      using namespace ::__type__;
      try {
        return ((tk).data[((u)) - int_VALUE_C(0)]);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__D_flip_flop(const ::__type__::anon0x3_t u) {
      using namespace ::__type__;
      try {
        ((flop).data[((u)) - int_VALUE_C(0)]) = ((req).data[((u)) - int_VALUE_C(0)]);
        ((tk).data[((u)) - int_VALUE_C(0)]) = (false);
        ((ntk).data[((u)) - int_VALUE_C(0)]) = (true);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__access_granting(const ::__type__::anon0x3_t u) const {
      using namespace ::__type__;
      try {
        return (((ntk).data[((u)) - int_VALUE_C(0)]) && ((flop).data[((u)) - int_VALUE_C(0)]));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(4,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__access_granting(const ::__type__::anon0x3_t u) {
      using namespace ::__type__;
      try {
        ((grant).data[((u)) - int_VALUE_C(0)]) = (true);
        ((ntk).data[((u)) - int_VALUE_C(0)]) = (false);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(4,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__token_passing(const ::__type__::anon0x3_t u) const {
      using namespace ::__type__;
      try {
        return (((ntk).data[((u)) - int_VALUE_C(0)]) && (!((flop).data[((u)) - int_VALUE_C(0)])));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(5,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__token_passing(const ::__type__::anon0x3_t u) {
      using namespace ::__type__;
      try {
        ((tk).data[(((u) + ((int)(1)))) - int_VALUE_C(0)]) = (true);
        ((ntk).data[((u)) - int_VALUE_C(0)]) = (false);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(5,::romp::ModelRuleError::ACTION) ); }
    }


    
    bool __Property___no_token_lost_(void) const {
      using namespace ::__type__;
      try {
        return this->__rw__->invariant_handler((((tk).data[((n)) - int_VALUE_C(0)]) == (false)),0u);
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
#define _ROMP_FUNCTS_LEN (1ul)
  /* the info/metadata about the functions/procedures in the model */
  const ::romp::FunctInfo FUNCT_INFOS[_ROMP_FUNCTS_LEN] = {{"NewToken",{{46,1},{53,5}},"{\"$type\":\"procedure\",\"label\":\"NewToken\",\"params\":[],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[46,1],\"end\":[53,5]}}","procedure NewToken();"},};
/* the number of error statements in the model */
#define _ROMP_ERRORS_LEN (0ul)
  /* the info/metadata about the murphi error statements in the model */
  const ::romp::MErrorInfo ERROR_INFOS[_ROMP_ERRORS_LEN] = {};
/* the number of property statements & rules in the model */
#define _ROMP_PROPERTIES_LEN (4ul)
  /* the info/metadata about the murphi properties in the model */
  const ::romp::PropertyInfo PROPERTY_INFOS[_ROMP_PROPERTIES_LEN] = {{"_no_token_lost_",::romp::PropertyType::INVARIANT,"(tk[n] = false)",0,{{112,1},{112,43}},"{\"$type\":\"property\",\"type\":\"invariant\",\"expr\":\"(tk[n] = false)\",\"label\":\"_no_token_lost_\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[112,1],\"end\":[112,43]}"},{"",::romp::PropertyType::ASSERTION,"(!Use_b)",1,{{66,18},{66,31}},"{\"$type\":\"property\",\"type\":\"assert\",\"label\":\"(!Use_b)\",\"expr\":\"(!Use_b)\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[66,18],\"end\":[66,31]}"},{"",::romp::PropertyType::ASSERTION,"req[u]",2,{{70,18},{70,31}},"{\"$type\":\"property\",\"type\":\"assert\",\"label\":\"req[u]\",\"expr\":\"req[u]\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[70,18],\"end\":[70,31]}"},{"",::romp::PropertyType::ASSERTION,"(!Use_b)",3,{{78,18},{78,31}},"{\"$type\":\"property\",\"type\":\"assert\",\"label\":\"(!Use_b)\",\"expr\":\"(!Use_b)\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[78,18],\"end\":[78,31]}"},};
/* the number of start state rules (before ruleset expansions) in the model */
#define _ROMP_STARTSTATE_INFO_LEN (1ul)
  /* the info/metadata about the startstate rules in the model */
  const ::romp::StartStateInfo STARTSTATE_INFOS[_ROMP_STARTSTATE_INFO_LEN] = {{"startstate2",{{102,1},{110,7}},"{\"$type\":\"startstate\",\"label\":\"startstate2\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[102,1],\"end\":[110,7]}"},};
/* the number of rules in the model, BEFORE ruleset expansion */
#define _ROMP_RULESETS_LEN (6ul)
  /* the info/metadata about the rules in the model */
  const ::romp::RuleInfo RULE_SET_INFOS[_ROMP_RULESETS_LEN] = {{"start_requesting",{{56,4},{62,21}},"{\"$type\":\"rule\",\"expr\":\"((!req[u]) & (!use[u]))\",\"label\":\"start_requesting\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[56,4],\"end\":[62,21]}"},{"start_using",{{63,4},{73,21}},"{\"$type\":\"rule\",\"expr\":\"grant[u]\",\"label\":\"start_using\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[63,4],\"end\":[73,21]}"},{"end_using",{{74,4},{80,21}},"{\"$type\":\"rule\",\"expr\":\"use[u]\",\"label\":\"end_using\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[74,4],\"end\":[80,21]}"},{"D_flip_flop",{{84,4},{88,21}},"{\"$type\":\"rule\",\"expr\":\"tk[u]\",\"label\":\"D_flip_flop\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[84,4],\"end\":[88,21]}"},{"access_granting",{{89,4},{93,21}},"{\"$type\":\"rule\",\"expr\":\"(ntk[u] & flop[u])\",\"label\":\"access_granting\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[89,4],\"end\":[93,21]}"},{"token_passing",{{94,4},{98,21}},"{\"$type\":\"rule\",\"expr\":\"(ntk[u] & (!flop[u]))\",\"label\":\"token_passing\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/arbiter.m\",\"start\":[94,4],\"end\":[98,21]}"},};

}

/* ======= Romp CALLER Lists ====== */
namespace __caller__ {
  typedef ::__model__::__Model__ State_t; // type mask for the generated state object
  /* --- Rules Generated by: `start_requesting` (RuleSet expansions) --- */
   bool __Rule_guard__start_requesting__0(const State_t& s){return s.__Rule_guard__start_requesting((::__type__::anon0x2_t) 0); }
   void __Rule_action__start_requesting__0(State_t& s){s.__Rule_action__start_requesting((::__type__::anon0x2_t) 0); }
   bool __Rule_guard__start_requesting__1(const State_t& s){return s.__Rule_guard__start_requesting((::__type__::anon0x2_t) 1); }
   void __Rule_action__start_requesting__1(State_t& s){s.__Rule_action__start_requesting((::__type__::anon0x2_t) 1); }
   bool __Rule_guard__start_requesting__2(const State_t& s){return s.__Rule_guard__start_requesting((::__type__::anon0x2_t) 2); }
   void __Rule_action__start_requesting__2(State_t& s){s.__Rule_action__start_requesting((::__type__::anon0x2_t) 2); }
   bool __Rule_guard__start_requesting__3(const State_t& s){return s.__Rule_guard__start_requesting((::__type__::anon0x2_t) 3); }
   void __Rule_action__start_requesting__3(State_t& s){s.__Rule_action__start_requesting((::__type__::anon0x2_t) 3); }
  /* --- Rules Generated by: `start_using` (RuleSet expansions) --- */
   bool __Rule_guard__start_using__0(const State_t& s){return s.__Rule_guard__start_using((::__type__::anon0x2_t) 0); }
   void __Rule_action__start_using__0(State_t& s){s.__Rule_action__start_using((::__type__::anon0x2_t) 0); }
   bool __Rule_guard__start_using__1(const State_t& s){return s.__Rule_guard__start_using((::__type__::anon0x2_t) 1); }
   void __Rule_action__start_using__1(State_t& s){s.__Rule_action__start_using((::__type__::anon0x2_t) 1); }
   bool __Rule_guard__start_using__2(const State_t& s){return s.__Rule_guard__start_using((::__type__::anon0x2_t) 2); }
   void __Rule_action__start_using__2(State_t& s){s.__Rule_action__start_using((::__type__::anon0x2_t) 2); }
   bool __Rule_guard__start_using__3(const State_t& s){return s.__Rule_guard__start_using((::__type__::anon0x2_t) 3); }
   void __Rule_action__start_using__3(State_t& s){s.__Rule_action__start_using((::__type__::anon0x2_t) 3); }
  /* --- Rules Generated by: `end_using` (RuleSet expansions) --- */
   bool __Rule_guard__end_using__0(const State_t& s){return s.__Rule_guard__end_using((::__type__::anon0x2_t) 0); }
   void __Rule_action__end_using__0(State_t& s){s.__Rule_action__end_using((::__type__::anon0x2_t) 0); }
   bool __Rule_guard__end_using__1(const State_t& s){return s.__Rule_guard__end_using((::__type__::anon0x2_t) 1); }
   void __Rule_action__end_using__1(State_t& s){s.__Rule_action__end_using((::__type__::anon0x2_t) 1); }
   bool __Rule_guard__end_using__2(const State_t& s){return s.__Rule_guard__end_using((::__type__::anon0x2_t) 2); }
   void __Rule_action__end_using__2(State_t& s){s.__Rule_action__end_using((::__type__::anon0x2_t) 2); }
   bool __Rule_guard__end_using__3(const State_t& s){return s.__Rule_guard__end_using((::__type__::anon0x2_t) 3); }
   void __Rule_action__end_using__3(State_t& s){s.__Rule_action__end_using((::__type__::anon0x2_t) 3); }
  /* --- Rules Generated by: `D_flip_flop` (RuleSet expansions) --- */
   bool __Rule_guard__D_flip_flop__0(const State_t& s){return s.__Rule_guard__D_flip_flop((::__type__::anon0x3_t) 0); }
   void __Rule_action__D_flip_flop__0(State_t& s){s.__Rule_action__D_flip_flop((::__type__::anon0x3_t) 0); }
   bool __Rule_guard__D_flip_flop__1(const State_t& s){return s.__Rule_guard__D_flip_flop((::__type__::anon0x3_t) 1); }
   void __Rule_action__D_flip_flop__1(State_t& s){s.__Rule_action__D_flip_flop((::__type__::anon0x3_t) 1); }
   bool __Rule_guard__D_flip_flop__2(const State_t& s){return s.__Rule_guard__D_flip_flop((::__type__::anon0x3_t) 2); }
   void __Rule_action__D_flip_flop__2(State_t& s){s.__Rule_action__D_flip_flop((::__type__::anon0x3_t) 2); }
   bool __Rule_guard__D_flip_flop__3(const State_t& s){return s.__Rule_guard__D_flip_flop((::__type__::anon0x3_t) 3); }
   void __Rule_action__D_flip_flop__3(State_t& s){s.__Rule_action__D_flip_flop((::__type__::anon0x3_t) 3); }
  /* --- Rules Generated by: `access_granting` (RuleSet expansions) --- */
   bool __Rule_guard__access_granting__0(const State_t& s){return s.__Rule_guard__access_granting((::__type__::anon0x3_t) 0); }
   void __Rule_action__access_granting__0(State_t& s){s.__Rule_action__access_granting((::__type__::anon0x3_t) 0); }
   bool __Rule_guard__access_granting__1(const State_t& s){return s.__Rule_guard__access_granting((::__type__::anon0x3_t) 1); }
   void __Rule_action__access_granting__1(State_t& s){s.__Rule_action__access_granting((::__type__::anon0x3_t) 1); }
   bool __Rule_guard__access_granting__2(const State_t& s){return s.__Rule_guard__access_granting((::__type__::anon0x3_t) 2); }
   void __Rule_action__access_granting__2(State_t& s){s.__Rule_action__access_granting((::__type__::anon0x3_t) 2); }
   bool __Rule_guard__access_granting__3(const State_t& s){return s.__Rule_guard__access_granting((::__type__::anon0x3_t) 3); }
   void __Rule_action__access_granting__3(State_t& s){s.__Rule_action__access_granting((::__type__::anon0x3_t) 3); }
  /* --- Rules Generated by: `token_passing` (RuleSet expansions) --- */
   bool __Rule_guard__token_passing__0(const State_t& s){return s.__Rule_guard__token_passing((::__type__::anon0x3_t) 0); }
   void __Rule_action__token_passing__0(State_t& s){s.__Rule_action__token_passing((::__type__::anon0x3_t) 0); }
   bool __Rule_guard__token_passing__1(const State_t& s){return s.__Rule_guard__token_passing((::__type__::anon0x3_t) 1); }
   void __Rule_action__token_passing__1(State_t& s){s.__Rule_action__token_passing((::__type__::anon0x3_t) 1); }
   bool __Rule_guard__token_passing__2(const State_t& s){return s.__Rule_guard__token_passing((::__type__::anon0x3_t) 2); }
   void __Rule_action__token_passing__2(State_t& s){s.__Rule_action__token_passing((::__type__::anon0x3_t) 2); }
   bool __Rule_guard__token_passing__3(const State_t& s){return s.__Rule_guard__token_passing((::__type__::anon0x3_t) 3); }
   void __Rule_action__token_passing__3(State_t& s){s.__Rule_action__token_passing((::__type__::anon0x3_t) 3); }

/* the number of rules in the model, AFTER ruleset expansion */
#define _ROMP_RULE_COUNT (24ul)


  // All of the rule sets in one place
  const ::romp::RuleSet RULESETS[_ROMP_RULESETS_LEN] = {
		{::__info__::RULE_SET_INFOS[0],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__start_requesting__0,__Rule_action__start_requesting__0,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":0}}]","u:=0"}, ::romp::Rule{__Rule_guard__start_requesting__1,__Rule_action__start_requesting__1,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":1}}]","u:=1"}, ::romp::Rule{__Rule_guard__start_requesting__2,__Rule_action__start_requesting__2,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":2}}]","u:=2"}, ::romp::Rule{__Rule_guard__start_requesting__3,__Rule_action__start_requesting__3,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":3}}]","u:=3"}  	}},
		{::__info__::RULE_SET_INFOS[1],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__start_using__0,__Rule_action__start_using__0,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":0}}]","u:=0"}, ::romp::Rule{__Rule_guard__start_using__1,__Rule_action__start_using__1,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":1}}]","u:=1"}, ::romp::Rule{__Rule_guard__start_using__2,__Rule_action__start_using__2,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":2}}]","u:=2"}, ::romp::Rule{__Rule_guard__start_using__3,__Rule_action__start_using__3,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":3}}]","u:=3"}  	}},
		{::__info__::RULE_SET_INFOS[2],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__end_using__0,__Rule_action__end_using__0,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":0}}]","u:=0"}, ::romp::Rule{__Rule_guard__end_using__1,__Rule_action__end_using__1,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":1}}]","u:=1"}, ::romp::Rule{__Rule_guard__end_using__2,__Rule_action__end_using__2,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":2}}]","u:=2"}, ::romp::Rule{__Rule_guard__end_using__3,__Rule_action__end_using__3,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":3}}]","u:=3"}  	}},
		{::__info__::RULE_SET_INFOS[3],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__D_flip_flop__0,__Rule_action__D_flip_flop__0,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":0}}]","u:=0"}, ::romp::Rule{__Rule_guard__D_flip_flop__1,__Rule_action__D_flip_flop__1,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":1}}]","u:=1"}, ::romp::Rule{__Rule_guard__D_flip_flop__2,__Rule_action__D_flip_flop__2,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":2}}]","u:=2"}, ::romp::Rule{__Rule_guard__D_flip_flop__3,__Rule_action__D_flip_flop__3,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":3}}]","u:=3"}  	}},
		{::__info__::RULE_SET_INFOS[4],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__access_granting__0,__Rule_action__access_granting__0,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":0}}]","u:=0"}, ::romp::Rule{__Rule_guard__access_granting__1,__Rule_action__access_granting__1,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":1}}]","u:=1"}, ::romp::Rule{__Rule_guard__access_granting__2,__Rule_action__access_granting__2,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":2}}]","u:=2"}, ::romp::Rule{__Rule_guard__access_granting__3,__Rule_action__access_granting__3,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":3}}]","u:=3"}  	}},
		{::__info__::RULE_SET_INFOS[5],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__token_passing__0,__Rule_action__token_passing__0,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":0}}]","u:=0"}, ::romp::Rule{__Rule_guard__token_passing__1,__Rule_action__token_passing__1,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":1}}]","u:=1"}, ::romp::Rule{__Rule_guard__token_passing__2,__Rule_action__token_passing__2,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":2}}]","u:=2"}, ::romp::Rule{__Rule_guard__token_passing__3,__Rule_action__token_passing__3,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"u\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(n - 1)\",\"value\":3}}]","u:=3"}  	}}};

  /* --- Property Rule(s) generated by: `_no_token_lost_` (RuleSet expansions) --- */
   bool __Property___no_token_lost___0(const State_t& s) {return s.__Property___no_token_lost_(); }

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
#define _ROMP_PROPERTY_RULES_LEN ((1ul) + ___propRule_assume_count___ + ___propRule_cover_count___ + ___propRule_liveness_count___)

  /* All of the property rules expanded in one place */
const ::romp::Property PROPERTIES[_ROMP_PROPERTY_RULES_LEN] = {
		{__Property___no_token_lost___0,::__info__::PROPERTY_INFOS[0],"[]",""},
};

  /* --- StartState Rule(s) generated by: `startstate2` (RuleSet expansions) --- */
   void __StartState__startstate2__0(State_t& s){ s.__StartState__startstate2(); }


#define _ROMP_STARTSTATES_LEN (1ul) // the number of property rules (after ruleset expansion) in the model

  // All of the rule sets in one place
const ::romp::StartState STARTSTATES[_ROMP_STARTSTATES_LEN] = {{__StartState__startstate2__0,::__info__::STARTSTATE_INFOS[0],0,"[]",""}};


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
