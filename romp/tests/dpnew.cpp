/*
	Generated code for a romp "parallel random walker" verification tool based off of the Murphi Model described in:
		Original Murphi Model: romp/tests/dpnew.m
	Please build with the following command(s):
		<not-implemented-yet>
*/


#define __romp__GENERATED_CODE


#define _ROMP_STATE_TYPE ::__model__::__Model__


#define _ROMP_STATE_HISTORY_LEN (4ul)


#define _ROMP_THREAD_TO_RW_RATIO (8ul)


#define __model__filename "romp/tests/dpnew.m"


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
  const __typeof__(((int)(3))) N = ((int)(3)); // Number of philosophers.
  const __typeof__(((int)(1))) option = ((int)(1)); // options as shown in the header
  typedef range_t Phil_ID;

  typedef enum { left, right, } side;

  typedef enum { think, take, eat, release, } anon0x0_t;

  typedef struct { ::__type__::boolean data[2]; size_t size() {return (size_t)(2);} } anon0x1_t;

  typedef struct {
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
      //----------------------------------------------------------------
      // Engineer:    C. Norris Ip                                    --
      //                                                              --
      // File:        dpnew.m						--
      //                                                              --
      //	Dining Philosophers					--
      //                                                              --
      //      DeadLock retaining options				--
      //      1)   use Rule "stop thinking and sit"                   --
      //		 Rule "start eating if got both forks"		--
      //               Rule "take fork"				--
      //      DeadLock removal options				--
      //	2) get both forks at the same time			--
      //           use Rule "stop thinking and sit"                   --
      //		 Rule "take both fork"				--
      //      3) sit N-1 philosophers only				--
      //         the one used in Hoare's book                         --
      //           use Rule "stop thinking and sit if ok"          	--
      //		 Rule "start eating if got both forks"		--
      //               Rule "take fork"				--
      //                                                              --
      // Date Created:        10 DEC 92                               --
      // Last modified:       14 Feb 93                               --
      //                                                              --
      //----------------------------------------------------------------
      // Number of philosophers.
      // options as shown in the header
      ::__type__::anon0x0_t Status;
      ::__type__::anon0x1_t Got_Forks;
    } anon0x2_t;

  typedef struct { ::__type__::anon0x2_t data[3]; size_t size() {return (size_t)(3);} } anon0x3_t;


}



/* ======= Generated Model & State ====== */

namespace __model__ {

  struct __Model__ {
    ::romp::IRandWalker* __rw__; // reference to associated romp rand walker


    /* ======= Model State Variables ====== */

    ::__type__::anon0x3_t Philosophers;


    /* ======= Murphi Model Functions ====== */


    /* ======= Murphi Model StartStates, Rules & Global Properties ====== */

     void __StartState__startstate2(void){
      using namespace ::__type__;
      try {
        // only if not all others want to eat already
        // cannot get fork, have to wait
        // cannot get fork, have to wait
        // cannot get fork, have to wait 
        /*
         Startstate and invariant
         */
        do {
          for (int ID = ((int)(0)); ID <= ((N) - ((int)(1))); ID++) {
            (((Philosophers).data[((ID)) - int_VALUE_C(0)]).Status) = (think);
            do {
              for (enum_backend_t s = left; s <= right; s++) {
                ((((Philosophers).data[((ID)) - int_VALUE_C(0)]).Got_Forks).data[((s)) - int_VALUE_C(0)]) = (false);
              }
            } while (0);
          }
        } while (0);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelStartStateError(0) ); }
    }



    
    bool __Rule_guard__stop_thinking_and_sit(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return ((((option) == ((int)(1))) || ((option) == ((int)(2)))) && (((p()).Status) == (think)));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__stop_thinking_and_sit(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        ((p()).Status) = (take);
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__stop_thinking_and_sit_if_ok(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return ((((option) == ((int)(3))) && (((p()).Status) == (think))) && ({ bool res_ = false; for (int ID1 = ((int)(0)); ID1 <= ((N) - ((int)(1))); ID1++) { res_ |= (((ID1) != (ID)) && ((((Philosophers).data[((ID1)) - int_VALUE_C(0)]).Status) == (think))); } res_; }));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__stop_thinking_and_sit_if_ok(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        ((p()).Status) = (take);
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__take_left_fork(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return (((((option) == ((int)(1))) || ((option) == ((int)(3)))) && (((p()).Status) == (take))) && ((((p()).Got_Forks).data[((left)) - int_VALUE_C(0)]) == (false)));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__take_left_fork(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        if ((!((((Philosophers).data[((((ID) == ((N) - ((int)(1)))) ? ((int)(0)) : ((ID) + ((int)(1))))) - int_VALUE_C(0)]).Got_Forks).data[((right)) - int_VALUE_C(0)]))) {
          (((p()).Got_Forks).data[((left)) - int_VALUE_C(0)]) = (true);
        } else {
        }
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__take_right_fork(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return (((((option) == ((int)(1))) || ((option) == ((int)(3)))) && (((p()).Status) == (take))) && ((((p()).Got_Forks).data[((right)) - int_VALUE_C(0)]) == (false)));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__take_right_fork(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        if ((!((((Philosophers).data[((((ID) == ((int)(0))) ? ((N) - ((int)(1))) : ((ID) - ((int)(1))))) - int_VALUE_C(0)]).Got_Forks).data[((left)) - int_VALUE_C(0)]))) {
          (((p()).Got_Forks).data[((right)) - int_VALUE_C(0)]) = (true);
        } else {
        }
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__start_eating_if_got_both_forks(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return ((((option) == ((int)(1))) || ((option) == ((int)(3)))) && ({ bool res_ = true; for (enum_backend_t s = left; s <= right; s++) { res_ &= (((p()).Got_Forks).data[((s)) - int_VALUE_C(0)]); } res_; }));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(4,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__start_eating_if_got_both_forks(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        ((p()).Status) = (eat);
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(4,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__take_both_fork(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return ((((option) == ((int)(2))) && (((p()).Status) == (take))) && ({ bool res_ = true; for (enum_backend_t s = left; s <= right; s++) { res_ &= (!(((p()).Got_Forks).data[((s)) - int_VALUE_C(0)])); } res_; }));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(5,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__take_both_fork(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        if ((!((((Philosophers).data[((((ID) == ((int)(0))) ? ((N) - ((int)(1))) : ((ID) - ((int)(1))))) - int_VALUE_C(0)]).Got_Forks).data[((left)) - int_VALUE_C(0)])) && (!((((Philosophers).data[((((ID) == ((N) - ((int)(1)))) ? ((int)(0)) : ((ID) + ((int)(1))))) - int_VALUE_C(0)]).Got_Forks).data[((right)) - int_VALUE_C(0)]))) {
          do {
            for (enum_backend_t s = left; s <= right; s++) {
              (((p()).Got_Forks).data[((s)) - int_VALUE_C(0)]) = (true);
            }
          } while (0);
          ((p()).Status) = (eat);
        } else {
        }
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(5,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__finished_eating(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return (((p()).Status) == (eat));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(6,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__finished_eating(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        ((p()).Status) = (release);
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(6,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__release_left_fork(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return (((p()).Status) == (release));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(7,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__release_left_fork(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        (((p()).Got_Forks).data[((left)) - int_VALUE_C(0)]) = (false);
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(7,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__release_right_fork(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return (((p()).Status) == (release));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(8,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__release_right_fork(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        (((p()).Got_Forks).data[((right)) - int_VALUE_C(0)]) = (false);
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(8,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__start_thinking(const ::__type__::Phil_ID ID) const {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        return ((((p()).Status) == (release)) && ({ bool res_ = true; for (enum_backend_t s = left; s <= right; s++) { res_ &= ((((p()).Got_Forks).data[((s)) - int_VALUE_C(0)]) == (false)); } res_; }));
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(9,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__start_thinking(const ::__type__::Phil_ID ID) {
      using namespace ::__type__;
      try {
#define p() ((Philosophers).data[((ID)) - int_VALUE_C(0)])
        ((p()).Status) = (think);
#undef p
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(9,::romp::ModelRuleError::ACTION) ); }
    }


    
    bool __Property__All_eating_philosophers_have_both_forks_available_(void) const {
      using namespace ::__type__;
      try {
        return this->__rw__->invariant_handler(({ bool res_ = true; for (int ID = ((int)(0)); ID <= ((N) - ((int)(1))); ID++) { res_ &= (!((((Philosophers).data[((ID)) - int_VALUE_C(0)]).Status) == (eat)) || ({ bool res_ = true; for (enum_backend_t s = left; s <= right; s++) { res_ &= ((((Philosophers).data[((ID)) - int_VALUE_C(0)]).Got_Forks).data[((s)) - int_VALUE_C(0)]); } res_; })); } res_; }),0u);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelPropertyError(0) ); }
    }

    
    bool __Property__A_fork_is_used_by_upto_one_philosophers_only(void) const {
      using namespace ::__type__;
      try {
        return this->__rw__->invariant_handler((({ bool res_ = true; for (int ID = ((int)(0)); ID <= ((N) - ((int)(1))); ID++) { res_ &= (!((((Philosophers).data[((ID)) - int_VALUE_C(0)]).Got_Forks).data[((left)) - int_VALUE_C(0)]) || (!((((Philosophers).data[((((ID) == ((N) - ((int)(1)))) ? ((int)(0)) : ((ID) + ((int)(1))))) - int_VALUE_C(0)]).Got_Forks).data[((right)) - int_VALUE_C(0)]))); } res_; }) && ({ bool res_ = true; for (int ID = ((int)(0)); ID <= ((N) - ((int)(1))); ID++) { res_ &= (!((((Philosophers).data[((ID)) - int_VALUE_C(0)]).Got_Forks).data[((right)) - int_VALUE_C(0)]) || (!((((Philosophers).data[((((ID) == ((int)(0))) ? ((N) - ((int)(1))) : ((ID) - ((int)(1))))) - int_VALUE_C(0)]).Got_Forks).data[((left)) - int_VALUE_C(0)]))); } res_; })),1u);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelPropertyError(1) ); }
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
#define _ROMP_PROPERTIES_LEN (2ul)
  /* the info/metadata about the murphi properties in the model */
  const ::romp::PropertyInfo PROPERTY_INFOS[_ROMP_PROPERTIES_LEN] = {{"All_eating_philosophers_have_both_forks_available_",::romp::PropertyType::INVARIANT,"forall ID : Phil_ID do ((Philosophers[ID].Status = eat) -> forall s : side do Philosophers[ID].Got_Forks[s] endforall) endforall",0,{{202,1},{209,6}},"{\"$type\":\"property\",\"type\":\"invariant\",\"expr\":\"forall ID : Phil_ID do ((Philosophers[ID].Status = eat) -> forall s : side do Philosophers[ID].Got_Forks[s] endforall) endforall\",\"label\":\"All_eating_philosophers_have_both_forks_available_\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[202,1],\"end\":[209,6]}"},{"A_fork_is_used_by_upto_one_philosophers_only",::romp::PropertyType::INVARIANT,"(forall ID : Phil_ID do (Philosophers[ID].Got_Forks[left] -> (!Philosophers[((ID = (N - 1)) ? 0 : (ID + 1))].Got_Forks[right])) endforall & forall ID : Phil_ID do (Philosophers[ID].Got_Forks[right] -> (!Philosophers[((ID = 0) ? (N - 1) : (ID - 1))].Got_Forks[left])) endforall)",1,{{212,1},{223,6}},"{\"$type\":\"property\",\"type\":\"invariant\",\"expr\":\"(forall ID : Phil_ID do (Philosophers[ID].Got_Forks[left] -> (!Philosophers[((ID = (N - 1)) ? 0 : (ID + 1))].Got_Forks[right])) endforall & forall ID : Phil_ID do (Philosophers[ID].Got_Forks[right] -> (!Philosophers[((ID = 0) ? (N - 1) : (ID - 1))].Got_Forks[left])) endforall)\",\"label\":\"A_fork_is_used_by_upto_one_philosophers_only\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[212,1],\"end\":[223,6]}"},};
/* the number of start state rules (before ruleset expansions) in the model */
#define _ROMP_STARTSTATE_INFO_LEN (1ul)
  /* the info/metadata about the startstate rules in the model */
  const ::romp::StartStateInfo STARTSTATE_INFOS[_ROMP_STARTSTATE_INFO_LEN] = {{"startstate2",{{192,1},{200,4}},"{\"$type\":\"startstate\",\"label\":\"startstate2\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[192,1],\"end\":[200,4]}"},};
/* the number of rules in the model, BEFORE ruleset expansion */
#define _ROMP_RULESETS_LEN (10ul)
  /* the info/metadata about the rules in the model */
  const ::romp::RuleInfo RULE_SET_INFOS[_ROMP_RULESETS_LEN] = {{"stop_thinking_and_sit",{{67,3},{74,6}},"{\"$type\":\"rule\",\"expr\":\"(((option = 1) | (option = 2)) & (p.Status = think))\",\"label\":\"stop_thinking_and_sit\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[67,3],\"end\":[74,6]}"},{"stop_thinking_and_sit_if_ok",{{76,3},{88,6}},"{\"$type\":\"rule\",\"expr\":\"(((option = 3) & (p.Status = think)) & exists ID1 : Phil_ID do ((ID1 != ID) & (Philosophers[ID1].Status = think)) endexists)\",\"label\":\"stop_thinking_and_sit_if_ok\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[76,3],\"end\":[88,6]}"},{"take_left_fork",{{90,3},{103,6}},"{\"$type\":\"rule\",\"expr\":\"((((option = 1) | (option = 3)) & (p.Status = take)) & (p.Got_Forks[left] = false))\",\"label\":\"take_left_fork\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[90,3],\"end\":[103,6]}"},{"take_right_fork",{{105,3},{118,6}},"{\"$type\":\"rule\",\"expr\":\"((((option = 1) | (option = 3)) & (p.Status = take)) & (p.Got_Forks[right] = false))\",\"label\":\"take_right_fork\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[105,3],\"end\":[118,6]}"},{"start_eating_if_got_both_forks",{{120,3},{129,6}},"{\"$type\":\"rule\",\"expr\":\"(((option = 1) | (option = 3)) & forall s : side do p.Got_Forks[s] endforall)\",\"label\":\"start_eating_if_got_both_forks\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[120,3],\"end\":[129,6]}"},{"take_both_fork",{{131,3},{151,6}},"{\"$type\":\"rule\",\"expr\":\"(((option = 2) & (p.Status = take)) & forall s : side do (!p.Got_Forks[s]) endforall)\",\"label\":\"take_both_fork\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[131,3],\"end\":[151,6]}"},{"finished_eating",{{153,3},{158,6}},"{\"$type\":\"rule\",\"expr\":\"(p.Status = eat)\",\"label\":\"finished_eating\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[153,3],\"end\":[158,6]}"},{"release_left_fork",{{160,3},{165,6}},"{\"$type\":\"rule\",\"expr\":\"(p.Status = release)\",\"label\":\"release_left_fork\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[160,3],\"end\":[165,6]}"},{"release_right_fork",{{167,3},{172,6}},"{\"$type\":\"rule\",\"expr\":\"(p.Status = release)\",\"label\":\"release_right_fork\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[167,3],\"end\":[172,6]}"},{"start_thinking",{{174,3},{183,6}},"{\"$type\":\"rule\",\"expr\":\"((p.Status = release) & forall s : side do (p.Got_Forks[s] = false) endforall)\",\"label\":\"start_thinking\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/dpnew.m\",\"start\":[174,3],\"end\":[183,6]}"},};

}

/* ======= Romp CALLER Lists ====== */
namespace __caller__ {
  typedef ::__model__::__Model__ State_t; // type mask for the generated state object
  /* --- Rules Generated by: `stop_thinking_and_sit` (RuleSet expansions) --- */
   bool __Rule_guard__stop_thinking_and_sit__0(const State_t& s){return s.__Rule_guard__stop_thinking_and_sit((::__type__::Phil_ID) 0); }
   void __Rule_action__stop_thinking_and_sit__0(State_t& s){s.__Rule_action__stop_thinking_and_sit((::__type__::Phil_ID) 0); }
   bool __Rule_guard__stop_thinking_and_sit__1(const State_t& s){return s.__Rule_guard__stop_thinking_and_sit((::__type__::Phil_ID) 1); }
   void __Rule_action__stop_thinking_and_sit__1(State_t& s){s.__Rule_action__stop_thinking_and_sit((::__type__::Phil_ID) 1); }
   bool __Rule_guard__stop_thinking_and_sit__2(const State_t& s){return s.__Rule_guard__stop_thinking_and_sit((::__type__::Phil_ID) 2); }
   void __Rule_action__stop_thinking_and_sit__2(State_t& s){s.__Rule_action__stop_thinking_and_sit((::__type__::Phil_ID) 2); }
  /* --- Rules Generated by: `stop_thinking_and_sit_if_ok` (RuleSet expansions) --- */
   bool __Rule_guard__stop_thinking_and_sit_if_ok__0(const State_t& s){return s.__Rule_guard__stop_thinking_and_sit_if_ok((::__type__::Phil_ID) 0); }
   void __Rule_action__stop_thinking_and_sit_if_ok__0(State_t& s){s.__Rule_action__stop_thinking_and_sit_if_ok((::__type__::Phil_ID) 0); }
   bool __Rule_guard__stop_thinking_and_sit_if_ok__1(const State_t& s){return s.__Rule_guard__stop_thinking_and_sit_if_ok((::__type__::Phil_ID) 1); }
   void __Rule_action__stop_thinking_and_sit_if_ok__1(State_t& s){s.__Rule_action__stop_thinking_and_sit_if_ok((::__type__::Phil_ID) 1); }
   bool __Rule_guard__stop_thinking_and_sit_if_ok__2(const State_t& s){return s.__Rule_guard__stop_thinking_and_sit_if_ok((::__type__::Phil_ID) 2); }
   void __Rule_action__stop_thinking_and_sit_if_ok__2(State_t& s){s.__Rule_action__stop_thinking_and_sit_if_ok((::__type__::Phil_ID) 2); }
  /* --- Rules Generated by: `take_left_fork` (RuleSet expansions) --- */
   bool __Rule_guard__take_left_fork__0(const State_t& s){return s.__Rule_guard__take_left_fork((::__type__::Phil_ID) 0); }
   void __Rule_action__take_left_fork__0(State_t& s){s.__Rule_action__take_left_fork((::__type__::Phil_ID) 0); }
   bool __Rule_guard__take_left_fork__1(const State_t& s){return s.__Rule_guard__take_left_fork((::__type__::Phil_ID) 1); }
   void __Rule_action__take_left_fork__1(State_t& s){s.__Rule_action__take_left_fork((::__type__::Phil_ID) 1); }
   bool __Rule_guard__take_left_fork__2(const State_t& s){return s.__Rule_guard__take_left_fork((::__type__::Phil_ID) 2); }
   void __Rule_action__take_left_fork__2(State_t& s){s.__Rule_action__take_left_fork((::__type__::Phil_ID) 2); }
  /* --- Rules Generated by: `take_right_fork` (RuleSet expansions) --- */
   bool __Rule_guard__take_right_fork__0(const State_t& s){return s.__Rule_guard__take_right_fork((::__type__::Phil_ID) 0); }
   void __Rule_action__take_right_fork__0(State_t& s){s.__Rule_action__take_right_fork((::__type__::Phil_ID) 0); }
   bool __Rule_guard__take_right_fork__1(const State_t& s){return s.__Rule_guard__take_right_fork((::__type__::Phil_ID) 1); }
   void __Rule_action__take_right_fork__1(State_t& s){s.__Rule_action__take_right_fork((::__type__::Phil_ID) 1); }
   bool __Rule_guard__take_right_fork__2(const State_t& s){return s.__Rule_guard__take_right_fork((::__type__::Phil_ID) 2); }
   void __Rule_action__take_right_fork__2(State_t& s){s.__Rule_action__take_right_fork((::__type__::Phil_ID) 2); }
  /* --- Rules Generated by: `start_eating_if_got_both_forks` (RuleSet expansions) --- */
   bool __Rule_guard__start_eating_if_got_both_forks__0(const State_t& s){return s.__Rule_guard__start_eating_if_got_both_forks((::__type__::Phil_ID) 0); }
   void __Rule_action__start_eating_if_got_both_forks__0(State_t& s){s.__Rule_action__start_eating_if_got_both_forks((::__type__::Phil_ID) 0); }
   bool __Rule_guard__start_eating_if_got_both_forks__1(const State_t& s){return s.__Rule_guard__start_eating_if_got_both_forks((::__type__::Phil_ID) 1); }
   void __Rule_action__start_eating_if_got_both_forks__1(State_t& s){s.__Rule_action__start_eating_if_got_both_forks((::__type__::Phil_ID) 1); }
   bool __Rule_guard__start_eating_if_got_both_forks__2(const State_t& s){return s.__Rule_guard__start_eating_if_got_both_forks((::__type__::Phil_ID) 2); }
   void __Rule_action__start_eating_if_got_both_forks__2(State_t& s){s.__Rule_action__start_eating_if_got_both_forks((::__type__::Phil_ID) 2); }
  /* --- Rules Generated by: `take_both_fork` (RuleSet expansions) --- */
   bool __Rule_guard__take_both_fork__0(const State_t& s){return s.__Rule_guard__take_both_fork((::__type__::Phil_ID) 0); }
   void __Rule_action__take_both_fork__0(State_t& s){s.__Rule_action__take_both_fork((::__type__::Phil_ID) 0); }
   bool __Rule_guard__take_both_fork__1(const State_t& s){return s.__Rule_guard__take_both_fork((::__type__::Phil_ID) 1); }
   void __Rule_action__take_both_fork__1(State_t& s){s.__Rule_action__take_both_fork((::__type__::Phil_ID) 1); }
   bool __Rule_guard__take_both_fork__2(const State_t& s){return s.__Rule_guard__take_both_fork((::__type__::Phil_ID) 2); }
   void __Rule_action__take_both_fork__2(State_t& s){s.__Rule_action__take_both_fork((::__type__::Phil_ID) 2); }
  /* --- Rules Generated by: `finished_eating` (RuleSet expansions) --- */
   bool __Rule_guard__finished_eating__0(const State_t& s){return s.__Rule_guard__finished_eating((::__type__::Phil_ID) 0); }
   void __Rule_action__finished_eating__0(State_t& s){s.__Rule_action__finished_eating((::__type__::Phil_ID) 0); }
   bool __Rule_guard__finished_eating__1(const State_t& s){return s.__Rule_guard__finished_eating((::__type__::Phil_ID) 1); }
   void __Rule_action__finished_eating__1(State_t& s){s.__Rule_action__finished_eating((::__type__::Phil_ID) 1); }
   bool __Rule_guard__finished_eating__2(const State_t& s){return s.__Rule_guard__finished_eating((::__type__::Phil_ID) 2); }
   void __Rule_action__finished_eating__2(State_t& s){s.__Rule_action__finished_eating((::__type__::Phil_ID) 2); }
  /* --- Rules Generated by: `release_left_fork` (RuleSet expansions) --- */
   bool __Rule_guard__release_left_fork__0(const State_t& s){return s.__Rule_guard__release_left_fork((::__type__::Phil_ID) 0); }
   void __Rule_action__release_left_fork__0(State_t& s){s.__Rule_action__release_left_fork((::__type__::Phil_ID) 0); }
   bool __Rule_guard__release_left_fork__1(const State_t& s){return s.__Rule_guard__release_left_fork((::__type__::Phil_ID) 1); }
   void __Rule_action__release_left_fork__1(State_t& s){s.__Rule_action__release_left_fork((::__type__::Phil_ID) 1); }
   bool __Rule_guard__release_left_fork__2(const State_t& s){return s.__Rule_guard__release_left_fork((::__type__::Phil_ID) 2); }
   void __Rule_action__release_left_fork__2(State_t& s){s.__Rule_action__release_left_fork((::__type__::Phil_ID) 2); }
  /* --- Rules Generated by: `release_right_fork` (RuleSet expansions) --- */
   bool __Rule_guard__release_right_fork__0(const State_t& s){return s.__Rule_guard__release_right_fork((::__type__::Phil_ID) 0); }
   void __Rule_action__release_right_fork__0(State_t& s){s.__Rule_action__release_right_fork((::__type__::Phil_ID) 0); }
   bool __Rule_guard__release_right_fork__1(const State_t& s){return s.__Rule_guard__release_right_fork((::__type__::Phil_ID) 1); }
   void __Rule_action__release_right_fork__1(State_t& s){s.__Rule_action__release_right_fork((::__type__::Phil_ID) 1); }
   bool __Rule_guard__release_right_fork__2(const State_t& s){return s.__Rule_guard__release_right_fork((::__type__::Phil_ID) 2); }
   void __Rule_action__release_right_fork__2(State_t& s){s.__Rule_action__release_right_fork((::__type__::Phil_ID) 2); }
  /* --- Rules Generated by: `start_thinking` (RuleSet expansions) --- */
   bool __Rule_guard__start_thinking__0(const State_t& s){return s.__Rule_guard__start_thinking((::__type__::Phil_ID) 0); }
   void __Rule_action__start_thinking__0(State_t& s){s.__Rule_action__start_thinking((::__type__::Phil_ID) 0); }
   bool __Rule_guard__start_thinking__1(const State_t& s){return s.__Rule_guard__start_thinking((::__type__::Phil_ID) 1); }
   void __Rule_action__start_thinking__1(State_t& s){s.__Rule_action__start_thinking((::__type__::Phil_ID) 1); }
   bool __Rule_guard__start_thinking__2(const State_t& s){return s.__Rule_guard__start_thinking((::__type__::Phil_ID) 2); }
   void __Rule_action__start_thinking__2(State_t& s){s.__Rule_action__start_thinking((::__type__::Phil_ID) 2); }

/* the number of rules in the model, AFTER ruleset expansion */
#define _ROMP_RULE_COUNT (30ul)


  // All of the rule sets in one place
  const ::romp::RuleSet RULESETS[_ROMP_RULESETS_LEN] = {
		{::__info__::RULE_SET_INFOS[0],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__stop_thinking_and_sit__0,__Rule_action__stop_thinking_and_sit__0,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__stop_thinking_and_sit__1,__Rule_action__stop_thinking_and_sit__1,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__stop_thinking_and_sit__2,__Rule_action__stop_thinking_and_sit__2,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}},
		{::__info__::RULE_SET_INFOS[1],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__stop_thinking_and_sit_if_ok__0,__Rule_action__stop_thinking_and_sit_if_ok__0,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__stop_thinking_and_sit_if_ok__1,__Rule_action__stop_thinking_and_sit_if_ok__1,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__stop_thinking_and_sit_if_ok__2,__Rule_action__stop_thinking_and_sit_if_ok__2,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}},
		{::__info__::RULE_SET_INFOS[2],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__take_left_fork__0,__Rule_action__take_left_fork__0,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__take_left_fork__1,__Rule_action__take_left_fork__1,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__take_left_fork__2,__Rule_action__take_left_fork__2,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}},
		{::__info__::RULE_SET_INFOS[3],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__take_right_fork__0,__Rule_action__take_right_fork__0,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__take_right_fork__1,__Rule_action__take_right_fork__1,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__take_right_fork__2,__Rule_action__take_right_fork__2,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}},
		{::__info__::RULE_SET_INFOS[4],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__start_eating_if_got_both_forks__0,__Rule_action__start_eating_if_got_both_forks__0,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__start_eating_if_got_both_forks__1,__Rule_action__start_eating_if_got_both_forks__1,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__start_eating_if_got_both_forks__2,__Rule_action__start_eating_if_got_both_forks__2,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}},
		{::__info__::RULE_SET_INFOS[5],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__take_both_fork__0,__Rule_action__take_both_fork__0,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__take_both_fork__1,__Rule_action__take_both_fork__1,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__take_both_fork__2,__Rule_action__take_both_fork__2,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}},
		{::__info__::RULE_SET_INFOS[6],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__finished_eating__0,__Rule_action__finished_eating__0,::__info__::RULE_SET_INFOS[6],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__finished_eating__1,__Rule_action__finished_eating__1,::__info__::RULE_SET_INFOS[6],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__finished_eating__2,__Rule_action__finished_eating__2,::__info__::RULE_SET_INFOS[6],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}},
		{::__info__::RULE_SET_INFOS[7],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__release_left_fork__0,__Rule_action__release_left_fork__0,::__info__::RULE_SET_INFOS[7],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__release_left_fork__1,__Rule_action__release_left_fork__1,::__info__::RULE_SET_INFOS[7],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__release_left_fork__2,__Rule_action__release_left_fork__2,::__info__::RULE_SET_INFOS[7],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}},
		{::__info__::RULE_SET_INFOS[8],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__release_right_fork__0,__Rule_action__release_right_fork__0,::__info__::RULE_SET_INFOS[8],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__release_right_fork__1,__Rule_action__release_right_fork__1,::__info__::RULE_SET_INFOS[8],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__release_right_fork__2,__Rule_action__release_right_fork__2,::__info__::RULE_SET_INFOS[8],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}},
		{::__info__::RULE_SET_INFOS[9],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__start_thinking__0,__Rule_action__start_thinking__0,::__info__::RULE_SET_INFOS[9],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":0}}]","ID:=0"}, ::romp::Rule{__Rule_guard__start_thinking__1,__Rule_action__start_thinking__1,::__info__::RULE_SET_INFOS[9],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":1}}]","ID:=1"}, ::romp::Rule{__Rule_guard__start_thinking__2,__Rule_action__start_thinking__2,::__info__::RULE_SET_INFOS[9],"[{\"$type\":\"kv-pair\",\"key\":\"ID\",\"value\":{\"$type\":\"range-value\",\"type\":\"0..(N - 1)\",\"value\":2}}]","ID:=2"}  	}}};

  /* --- Property Rule(s) generated by: `All_eating_philosophers_have_both_forks_available_` (RuleSet expansions) --- */
   bool __Property__All_eating_philosophers_have_both_forks_available___0(const State_t& s) {return s.__Property__All_eating_philosophers_have_both_forks_available_(); }
  /* --- Property Rule(s) generated by: `A_fork_is_used_by_upto_one_philosophers_only` (RuleSet expansions) --- */
   bool __Property__A_fork_is_used_by_upto_one_philosophers_only__0(const State_t& s) {return s.__Property__A_fork_is_used_by_upto_one_philosophers_only(); }

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
#define _ROMP_PROPERTY_RULES_LEN ((2ul) + ___propRule_assume_count___ + ___propRule_cover_count___ + ___propRule_liveness_count___)

  /* All of the property rules expanded in one place */
const ::romp::Property PROPERTIES[_ROMP_PROPERTY_RULES_LEN] = {
		{__Property__All_eating_philosophers_have_both_forks_available___0,::__info__::PROPERTY_INFOS[0],"[]",""},
		{__Property__A_fork_is_used_by_upto_one_philosophers_only__0,::__info__::PROPERTY_INFOS[1],"[]",""},
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
