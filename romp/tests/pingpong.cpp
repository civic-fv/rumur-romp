/*
	Generated code for a romp "parallel random walker" verification tool based off of the Murphi Model described in:
		Original Murphi Model: romp/tests/pingpong.m
	Please build with the following command(s):
		<not-implemented-yet>
*/


#define __romp__GENERATED_CODE


#define _ROMP_STATE_TYPE ::__model__::__Model__


#define _ROMP_STATE_HISTORY_LEN (4ul)


#define _ROMP_THREAD_TO_RW_RATIO (8ul)


#define __model__filename "romp/tests/pingpong.m"


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
  typedef range_t player_t;

  typedef struct {
      //-----------------------------------------------------------------------
      // Copyright (C) 1992, 1993 by the Board of Trustees of 		  
      // Leland Stanford Junior University.					  
      //									  
      // This description is provided to serve as an example of the use	  
      // of the Murphi description language and verifier, and as a benchmark	  
      // example for other verification efforts.
      //	
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
      //------------------------------------------------------------------------
      /* pingpong.m
       * Ralph Melton, 8/30/92.
       * Done to test Murphi and
       * because it's an important toy problem that we don't already have.
       */
      ::__type__::boolean hasball;
      ::__type__::boolean gotball;
    } anon0x0_t;

  typedef struct { ::__type__::anon0x0_t data[2]; size_t size() {return (size_t)(2);} } anon0x1_t;


}



/* ======= Generated Model & State ====== */

namespace __model__ {

  struct __Model__ {
    ::romp::IRandWalker* __rw__; // reference to associated romp rand walker


    /* ======= Model State Variables ====== */

    ::__type__::anon0x1_t Players;


    /* ======= Murphi Model Functions ====== */


    /* ======= Murphi Model StartStates, Rules & Global Properties ====== */

     void __StartState__startstate2(const ::__type__::player_t p){
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
        //------------------------------------------------------------------------
        /* pingpong.m
         * Ralph Melton, 8/30/92.
         * Done to test Murphi and
         * because it's an important toy problem that we don't already have.
         */
#define ping() ((Players).data[((p)) - int_VALUE_C(0)])
#define pong() ((Players).data[((((int)(1)) - (p))) - int_VALUE_C(0)])
        /* Yes, a startstate within an alias and a ruleset. */
        ((ping()).hasball) = (true);
        ((ping()).gotball) = (false);
        memset(&(pong()), 0, sizeof((pong())));
#undef ping
#undef pong
      } catch (...) { ::std::throw_with_nested( ::romp::ModelStartStateError(0) ); }
    }



    
    bool __Rule_guard__Get_ball(const ::__type__::player_t p) const {
      using namespace ::__type__;
      try {
#define ping() ((Players).data[((p)) - int_VALUE_C(0)])
#define pong() ((Players).data[((((int)(1)) - (p))) - int_VALUE_C(0)])
        return ((ping()).gotball);
#undef ping
#undef pong
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Get_ball(const ::__type__::player_t p) {
      using namespace ::__type__;
      try {
#define ping() ((Players).data[((p)) - int_VALUE_C(0)])
#define pong() ((Players).data[((((int)(1)) - (p))) - int_VALUE_C(0)])
        ((ping()).hasball) = (true);
        ((ping()).gotball) = (false);
#undef ping
#undef pong
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Keep_ball(const ::__type__::player_t p) const {
      using namespace ::__type__;
      try {
#define ping() ((Players).data[((p)) - int_VALUE_C(0)])
#define pong() ((Players).data[((((int)(1)) - (p))) - int_VALUE_C(0)])
        return ((ping()).hasball);
#undef ping
#undef pong
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Keep_ball(const ::__type__::player_t p) {
      using namespace ::__type__;
      try {
#define ping() ((Players).data[((p)) - int_VALUE_C(0)])
#define pong() ((Players).data[((((int)(1)) - (p))) - int_VALUE_C(0)])
#undef ping
#undef pong
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Pass_ball(const ::__type__::player_t p) const {
      using namespace ::__type__;
      try {
#define ping() ((Players).data[((p)) - int_VALUE_C(0)])
#define pong() ((Players).data[((((int)(1)) - (p))) - int_VALUE_C(0)])
        return ((ping()).hasball);
#undef ping
#undef pong
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Pass_ball(const ::__type__::player_t p) {
      using namespace ::__type__;
      try {
#define ping() ((Players).data[((p)) - int_VALUE_C(0)])
#define pong() ((Players).data[((((int)(1)) - (p))) - int_VALUE_C(0)])
        ((ping()).hasball) = (false);
        ((pong()).gotball) = (true);
#undef ping
#undef pong
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::ACTION) ); }
    }


    
    bool __Property__Only_one_ball_in_play_(void) const {
      using namespace ::__type__;
      try {
        return this->__rw__->invariant_handler(({ bool res_ = true; for (int p = ((int)(0)); p <= ((int)(1)); p++) { res_ &= (!((!((((Players).data[((p)) - int_VALUE_C(0)]).hasball) && (((Players).data[((p)) - int_VALUE_C(0)]).gotball))) && ((((Players).data[((p)) - int_VALUE_C(0)]).hasball) || (((Players).data[((p)) - int_VALUE_C(0)]).gotball))) || ({ bool res_ = true; for (int q = ((int)(0)); q <= ((int)(1)); q++) { res_ &= (!((((Players).data[((q)) - int_VALUE_C(0)]).hasball) || (((Players).data[((q)) - int_VALUE_C(0)]).gotball)) || ((p) == (q))); } res_; })); } res_; }),0u);
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
  const ::romp::PropertyInfo PROPERTY_INFOS[_ROMP_PROPERTIES_LEN] = {{"Only_one_ball_in_play_",::romp::PropertyType::INVARIANT,"forall p : player_t do (((!(Players[p].hasball & Players[p].gotball)) & (Players[p].hasball | Players[p].gotball)) -> forall q : player_t do ((Players[q].hasball | Players[q].gotball) -> (p = q)) endforall) endforall",0,{{72,1},{79,6}},"{\"$type\":\"property\",\"type\":\"invariant\",\"expr\":\"forall p : player_t do (((!(Players[p].hasball & Players[p].gotball)) & (Players[p].hasball | Players[p].gotball)) -> forall q : player_t do ((Players[q].hasball | Players[q].gotball) -> (p = q)) endforall) endforall\",\"label\":\"Only_one_ball_in_play_\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/pingpong.m\",\"start\":[72,1],\"end\":[79,6]}"},};
/* the number of start state rules (before ruleset expansions) in the model */
#define _ROMP_STARTSTATE_INFO_LEN (1ul)
  /* the info/metadata about the startstate rules in the model */
  const ::romp::StartStateInfo STARTSTATE_INFOS[_ROMP_STARTSTATE_INFO_LEN] = {{"startstate2",{{61,5},{66,8}},"{\"$type\":\"startstate\",\"label\":\"startstate2\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/pingpong.m\",\"start\":[61,5],\"end\":[66,8]}"},};
/* the number of rules in the model, BEFORE ruleset expansion */
#define _ROMP_RULESETS_LEN (3ul)
  /* the info/metadata about the rules in the model */
  const ::romp::RuleInfo RULE_SET_INFOS[_ROMP_RULESETS_LEN] = {{"Get_ball",{{39,5},{45,8}},"{\"$type\":\"rule\",\"expr\":\"ping.gotball\",\"label\":\"Get_ball\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/pingpong.m\",\"start\":[39,5],\"end\":[45,8]}"},{"Keep_ball",{{47,5},{51,8}},"{\"$type\":\"rule\",\"expr\":\"ping.hasball\",\"label\":\"Keep_ball\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/pingpong.m\",\"start\":[47,5],\"end\":[51,8]}"},{"Pass_ball",{{53,5},{59,8}},"{\"$type\":\"rule\",\"expr\":\"ping.hasball\",\"label\":\"Pass_ball\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/pingpong.m\",\"start\":[53,5],\"end\":[59,8]}"},};

}

/* ======= Romp CALLER Lists ====== */
namespace __caller__ {
  typedef ::__model__::__Model__ State_t; // type mask for the generated state object
  /* --- Rules Generated by: `Get_ball` (RuleSet expansions) --- */
   bool __Rule_guard__Get_ball__0(const State_t& s){return s.__Rule_guard__Get_ball((::__type__::player_t) 0); }
   void __Rule_action__Get_ball__0(State_t& s){s.__Rule_action__Get_ball((::__type__::player_t) 0); }
   bool __Rule_guard__Get_ball__1(const State_t& s){return s.__Rule_guard__Get_ball((::__type__::player_t) 1); }
   void __Rule_action__Get_ball__1(State_t& s){s.__Rule_action__Get_ball((::__type__::player_t) 1); }
  /* --- Rules Generated by: `Keep_ball` (RuleSet expansions) --- */
   bool __Rule_guard__Keep_ball__0(const State_t& s){return s.__Rule_guard__Keep_ball((::__type__::player_t) 0); }
   void __Rule_action__Keep_ball__0(State_t& s){s.__Rule_action__Keep_ball((::__type__::player_t) 0); }
   bool __Rule_guard__Keep_ball__1(const State_t& s){return s.__Rule_guard__Keep_ball((::__type__::player_t) 1); }
   void __Rule_action__Keep_ball__1(State_t& s){s.__Rule_action__Keep_ball((::__type__::player_t) 1); }
  /* --- Rules Generated by: `Pass_ball` (RuleSet expansions) --- */
   bool __Rule_guard__Pass_ball__0(const State_t& s){return s.__Rule_guard__Pass_ball((::__type__::player_t) 0); }
   void __Rule_action__Pass_ball__0(State_t& s){s.__Rule_action__Pass_ball((::__type__::player_t) 0); }
   bool __Rule_guard__Pass_ball__1(const State_t& s){return s.__Rule_guard__Pass_ball((::__type__::player_t) 1); }
   void __Rule_action__Pass_ball__1(State_t& s){s.__Rule_action__Pass_ball((::__type__::player_t) 1); }

/* the number of rules in the model, AFTER ruleset expansion */
#define _ROMP_RULE_COUNT (6ul)


  // All of the rule sets in one place
  const ::romp::RuleSet RULESETS[_ROMP_RULESETS_LEN] = {
		{::__info__::RULE_SET_INFOS[0],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Get_ball__0,__Rule_action__Get_ball__0,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"p\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..1\",\"value\":0}}]","p:=0"}, ::romp::Rule{__Rule_guard__Get_ball__1,__Rule_action__Get_ball__1,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"p\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..1\",\"value\":1}}]","p:=1"}  	}},
		{::__info__::RULE_SET_INFOS[1],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Keep_ball__0,__Rule_action__Keep_ball__0,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"p\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..1\",\"value\":0}}]","p:=0"}, ::romp::Rule{__Rule_guard__Keep_ball__1,__Rule_action__Keep_ball__1,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"p\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..1\",\"value\":1}}]","p:=1"}  	}},
		{::__info__::RULE_SET_INFOS[2],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Pass_ball__0,__Rule_action__Pass_ball__0,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"p\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..1\",\"value\":0}}]","p:=0"}, ::romp::Rule{__Rule_guard__Pass_ball__1,__Rule_action__Pass_ball__1,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"p\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..1\",\"value\":1}}]","p:=1"}  	}}};

  /* --- Property Rule(s) generated by: `Only_one_ball_in_play_` (RuleSet expansions) --- */
   bool __Property__Only_one_ball_in_play___0(const State_t& s) {return s.__Property__Only_one_ball_in_play_(); }

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
		{__Property__Only_one_ball_in_play___0,::__info__::PROPERTY_INFOS[0],"[]",""},
};

  /* --- StartState Rule(s) generated by: `startstate2` (RuleSet expansions) --- */
   void __StartState__startstate2__0(State_t& s){ s.__StartState__startstate2((::__type__::player_t) 0); }
   void __StartState__startstate2__1(State_t& s){ s.__StartState__startstate2((::__type__::player_t) 1); }


#define _ROMP_STARTSTATES_LEN (2ul) // the number of property rules (after ruleset expansion) in the model

  // All of the rule sets in one place
const ::romp::StartState STARTSTATES[_ROMP_STARTSTATES_LEN] = {{__StartState__startstate2__0,::__info__::STARTSTATE_INFOS[0],0,"[{\"$type\":\"kv-pair\",\"key\":\"p\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..1\",\"value\":0}}]","p:=0"}, {__StartState__startstate2__1,::__info__::STARTSTATE_INFOS[0],1,"[{\"$type\":\"kv-pair\",\"key\":\"p\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..1\",\"value\":1}}]","p:=1"}};


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
