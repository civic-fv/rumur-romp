/*
	Generated code for a romp "parallel random walker" verification tool based off of the Murphi Model described in:
		Original Murphi Model: romp/tests/mcslock2.m
	Please build with the following command(s):
		<not-implemented-yet>
*/


#define __romp__GENERATED_CODE


#define _ROMP_STATE_TYPE ::__model__::__Model__


#define _ROMP_STATE_HISTORY_LEN (4ul)


#define _ROMP_THREAD_TO_RW_RATIO (8ul)


#define __model__filename "romp/tests/mcslock2.m"


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
  const __typeof__(((int)(3))) N = ((int)(3));
  typedef scalarset_t pid;

  typedef enum { L0, L1, L2, L3, L4, L5, L6, L7, L8, L9, L10, L11, L12, } label_t;

  typedef struct {
      //------------------------------------------------------------------------
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
      //------------------------------------------------------------------------
      //------------------------------------------------------------------------
      //
      //                                                                        
      // File:        mcslock2.m                                                
      //                                                                        
      // Content:     MCS distributed list-based queuing lock                   
      //              without atomic compare_and_swap operations                
      //                                                                        
      // Summary of result:                                                     
      //          1)  No bug is discovered 					  
      //          2)  Details of result can be found at the end of this file.   
      //                                                                        
      // References: 						       	       	  
      // J.M. Mellor-Crummey and M.L. Scott,					  
      // Algorithms for Scalable Synchronization on Shared-Memory Multiprocessors,
      // ACM transactions on computer systems, Vol 9, No 1, 1991.               
      //                                                                        
      // Date created:         28 Oct 92                                        
      // Last Modified:        17 Feb 93                                        
      //                                                                        
      //------------------------------------------------------------------------
      //  The scalarset is used for symmetry, which is implemented in Murphi 1.5
      //  and not upgraded to Murphi 2.0 yet
      // : non critical section; 
      // : acquire => R->next := nil
      // :            predecessor := fetch_and_store( L,R )
      // :  	     if (predecessor != nil)
      // : 		R->locked := true
      // : 		predecessor->next := R
      // : 		repeat while I->locked
      // : critical section; 
      // : release => if (R->next = nil)
      // : 		old_tail = fetch_and_store( L,nil )
      // : 		if (old_tail = R)
      // :			return
      // :		usuper := fetch_and_store ( L, old_tail)
      // :		repeat while R->next = nil
      // :		if (usuper != nil)
      // :			usuper->next := R-> next
      // : 		else
      // :			R->next->locked := false
      // :	    else
      // :		R->next->locked := false
      ::__type__::boolean nil;
      ::__type__::pid p;
    } ptqnode;

  typedef struct {
      ::__type__::ptqnode next;
      ::__type__::boolean locked;
    } qnode;

  typedef struct { ::__type__::label_t data[3]; size_t size() {return (size_t)(3);} } anon0x0_t;

  typedef struct { ::__type__::qnode data[3]; size_t size() {return (size_t)(3);} } anon0x1_t;

  typedef struct {
      ::__type__::ptqnode pred;
      ::__type__::ptqnode tail;
      ::__type__::ptqnode usuper;
    } anon0x2_t;

  typedef struct { ::__type__::anon0x2_t data[3]; size_t size() {return (size_t)(3);} } anon0x3_t;


}



/* ======= Generated Model & State ====== */

namespace __model__ {

  struct __Model__ {
    ::romp::IRandWalker* __rw__; // reference to associated romp rand walker


    /* ======= Model State Variables ====== */

    ::__type__::anon0x0_t P;

    ::__type__::anon0x1_t R;

    ::__type__::anon0x3_t local;

    ::__type__::ptqnode L;


    /* ======= Murphi Model Functions ====== */

    
    void setNextNil(const ::__type__::pid i)  {
      using namespace ::__type__;
      try {
        ((((R).data[((i)) - int_VALUE_C(0)]).next).nil) = (true);
        memset(&((((R).data[((i)) - int_VALUE_C(0)]).next).p), 0, sizeof(((((R).data[((i)) - int_VALUE_C(0)]).next).p)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(0) ); }
}

    
    void setNext(const ::__type__::pid i, const ::__type__::pid n)  {
      using namespace ::__type__;
      try {
        ((((R).data[((i)) - int_VALUE_C(0)]).next).nil) = (false);
        ((((R).data[((i)) - int_VALUE_C(0)]).next).p) = (n);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(1) ); }
}

    
    void setPredNil(const ::__type__::pid i)  {
      using namespace ::__type__;
      try {
        ((((local).data[((i)) - int_VALUE_C(0)]).pred).nil) = (true);
        memset(&((((local).data[((i)) - int_VALUE_C(0)]).pred).p), 0, sizeof(((((local).data[((i)) - int_VALUE_C(0)]).pred).p)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(2) ); }
}

    
    void setPred(const ::__type__::pid i, const ::__type__::pid n)  {
      using namespace ::__type__;
      try {
        ((((local).data[((i)) - int_VALUE_C(0)]).pred).nil) = (false);
        ((((local).data[((i)) - int_VALUE_C(0)]).pred).p) = (n);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(3) ); }
}

    
    void setTailNil(const ::__type__::pid i)  {
      using namespace ::__type__;
      try {
        ((((local).data[((i)) - int_VALUE_C(0)]).tail).nil) = (true);
        memset(&((((local).data[((i)) - int_VALUE_C(0)]).tail).p), 0, sizeof(((((local).data[((i)) - int_VALUE_C(0)]).tail).p)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(4) ); }
}

    
    void setTail(const ::__type__::pid i, const ::__type__::pid n)  {
      using namespace ::__type__;
      try {
        ((((local).data[((i)) - int_VALUE_C(0)]).tail).nil) = (false);
        ((((local).data[((i)) - int_VALUE_C(0)]).tail).p) = (n);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(5) ); }
}

    
    void setUsuperNil(const ::__type__::pid i)  {
      using namespace ::__type__;
      try {
        ((((local).data[((i)) - int_VALUE_C(0)]).usuper).nil) = (true);
        memset(&((((local).data[((i)) - int_VALUE_C(0)]).usuper).p), 0, sizeof(((((local).data[((i)) - int_VALUE_C(0)]).usuper).p)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(6) ); }
}

    
    void setUsuper(const ::__type__::pid i, const ::__type__::pid n)  {
      using namespace ::__type__;
      try {
        ((((local).data[((i)) - int_VALUE_C(0)]).usuper).nil) = (false);
        ((((local).data[((i)) - int_VALUE_C(0)]).usuper).p) = (n);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(7) ); }
}

    
    void setLockNil(void)  {
      using namespace ::__type__;
      try {
        ((L).nil) = (true);
        memset(&((L).p), 0, sizeof(((L).p)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(8) ); }
}

    
    void setLock(const ::__type__::pid i)  {
      using namespace ::__type__;
      try {
        ((L).nil) = (false);
        ((L).p) = (i);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(9) ); }
}


    /* ======= Murphi Model StartStates, Rules & Global Properties ====== */

     void __StartState__startstate1(void){
      using namespace ::__type__;
      try {
        //If
        //If
        //If
        //If
        //If
        //If
        //If
        //If
        //If
        //If
        //Ruleset
        do {
          for (int i = 0; i <= (N); i++) {
            ((P).data[((i)) - int_VALUE_C(0)]) = (L0);
            ::__model__::__Model__::setNextNil((i));
            (((R).data[((i)) - int_VALUE_C(0)]).locked) = (false);
            ::__model__::__Model__::setPredNil((i));
            ::__model__::__Model__::setTailNil((i));
            ::__model__::__Model__::setUsuperNil((i));
          }
        } while (0); //For
        ::__model__::__Model__::setLockNil();
      } catch (...) { ::std::throw_with_nested( ::romp::ModelStartStateError(0) ); }
    }



    
    bool __Rule_guard__execute_non_crit_and_assign_next_nil(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L0));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_non_crit_and_assign_next_nil(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        ::__model__::__Model__::setNextNil((i));
        ((P).data[((i)) - int_VALUE_C(0)]) = (L1);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_assign_pred_FandS_L_I(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L1));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_assign_pred_FandS_L_I(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        if (((L).nil)) {
          ::__model__::__Model__::setPredNil((i));
        } else {
          ::__model__::__Model__::setPred((i), ((L).p));
        }
        ::__model__::__Model__::setLock((i));
        ((P).data[((i)) - int_VALUE_C(0)]) = (L2);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_if_pred_nil(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L2));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_if_pred_nil(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        if (((((local).data[((i)) - int_VALUE_C(0)]).pred).nil)) {
          ((P).data[((i)) - int_VALUE_C(0)]) = (L6);
        } else {
          ((P).data[((i)) - int_VALUE_C(0)]) = (L3);
        }
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_assign_Ilocked_true(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L3));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_assign_Ilocked_true(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        (((R).data[((i)) - int_VALUE_C(0)]).locked) = (true);
        ((P).data[((i)) - int_VALUE_C(0)]) = (L4);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_assign_prednext_I(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L4));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(4,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_assign_prednext_I(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        ::__model__::__Model__::setNext(((((local).data[((i)) - int_VALUE_C(0)]).pred).p), (i));
        ((P).data[((i)) - int_VALUE_C(0)]) = (L5);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(4,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_repeat_while_Ilocked(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L5));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(5,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_repeat_while_Ilocked(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        if ((!(((R).data[((i)) - int_VALUE_C(0)]).locked))) {
          ((P).data[((i)) - int_VALUE_C(0)]) = (L6);
        }
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(5,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_crit_and_if_Inext_nil(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L6));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(6,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_crit_and_if_Inext_nil(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        if (((((R).data[((i)) - int_VALUE_C(0)]).next).nil)) {
          ((P).data[((i)) - int_VALUE_C(0)]) = (L7);
        } else {
          ((P).data[((i)) - int_VALUE_C(0)]) = (L12);
        }
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(6,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_assign_oldtail_FandS_L_nil(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L7));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(7,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_assign_oldtail_FandS_L_nil(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        if (((L).nil)) {
          ::__model__::__Model__::setTailNil((i));
        } else {
          ::__model__::__Model__::setTail((i), ((L).p));
        }
        ::__model__::__Model__::setLockNil();
        ((P).data[((i)) - int_VALUE_C(0)]) = (L8);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(7,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_if_tail_eq_I(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L8));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(8,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_if_tail_eq_I(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        if (((((local).data[((i)) - int_VALUE_C(0)]).tail).p) == (i)) {
          ((P).data[((i)) - int_VALUE_C(0)]) = (L0);
        } else {
          ((P).data[((i)) - int_VALUE_C(0)]) = (L9);
        }
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(8,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_assign_usuper_FandS_L_tail(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L9));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(9,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_assign_usuper_FandS_L_tail(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        ::__type__::ptqnode murphilocal;
        (murphilocal) = (((local).data[((i)) - int_VALUE_C(0)]).tail);
        if (((L).nil)) {
          ::__model__::__Model__::setUsuperNil((i));
        } else {
          ::__model__::__Model__::setUsuper((i), ((L).p));
        }
        if (((murphilocal).nil)) {
          if (this->__rw__->error_handler(0u))
            throw ::romp::ModelMErrorError(0);

          ::__model__::__Model__::setLockNil();
        } else {
          ::__model__::__Model__::setLock(((murphilocal).p));
        }
        ((P).data[((i)) - int_VALUE_C(0)]) = (L10);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(9,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_repeat_while_Inext_nil(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L10));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(10,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_repeat_while_Inext_nil(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        if ((!((((R).data[((i)) - int_VALUE_C(0)]).next).nil))) {
          ((P).data[((i)) - int_VALUE_C(0)]) = (L11);
        }
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(10,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L11));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(11,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        if ((!((((local).data[((i)) - int_VALUE_C(0)]).usuper).nil))) {
          (((R).data[(((((local).data[((i)) - int_VALUE_C(0)]).usuper).p)) - int_VALUE_C(0)]).next) = (((R).data[((i)) - int_VALUE_C(0)]).next);
        } else {
          (((R).data[(((((R).data[((i)) - int_VALUE_C(0)]).next).p)) - int_VALUE_C(0)]).locked) = (false);
        }
        ((P).data[((i)) - int_VALUE_C(0)]) = (L0);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(11,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__execute_assign_Inextlocked_false(const ::__type__::pid i) const {
      using namespace ::__type__;
      try {
        return (((P).data[((i)) - int_VALUE_C(0)]) == (L12));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(12,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__execute_assign_Inextlocked_false(const ::__type__::pid i) {
      using namespace ::__type__;
      try {
        (((R).data[(((((R).data[((i)) - int_VALUE_C(0)]).next).p)) - int_VALUE_C(0)]).locked) = (false);
        ((P).data[((i)) - int_VALUE_C(0)]) = (L0);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(12,::romp::ModelRuleError::ACTION) ); }
    }


    
    bool __Property__property2(void) const {
      using namespace ::__type__;
      try {
        return this->__rw__->invariant_handler((!({ bool res_ = false; for (int i1 = 0; i1 <= (N); i1++) { res_ |= ({ bool res_ = false; for (int i2 = 0; i2 <= (N); i2++) { res_ |= ((((i1) != (i2)) && (((P).data[((i1)) - int_VALUE_C(0)]) == (L6))) && (((P).data[((i2)) - int_VALUE_C(0)]) == (L6))); } res_; }); } res_; })),0u);
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
#define _ROMP_FUNCTS_LEN (10ul)
  /* the info/metadata about the functions/procedures in the model */
  const ::romp::FunctInfo FUNCT_INFOS[_ROMP_FUNCTS_LEN] = {{"setNextNil",{{94,1},{98,5}},"{\"$type\":\"procedure\",\"label\":\"setNextNil\",\"params\":[{\"$type\":\"param\",\"id\":\"i\",\"type\":\"pid\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[94,1],\"end\":[98,5]}}","procedure setNextNil(var i: pid);"},{"setNext",{{100,1},{104,5}},"{\"$type\":\"procedure\",\"label\":\"setNext\",\"params\":[{\"$type\":\"param\",\"id\":\"i\",\"type\":\"pid\"},{\"$type\":\"param\",\"id\":\"n\",\"type\":\"pid\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[100,1],\"end\":[104,5]}}","procedure setNext(var i,n: pid);"},{"setPredNil",{{106,1},{110,5}},"{\"$type\":\"procedure\",\"label\":\"setPredNil\",\"params\":[{\"$type\":\"param\",\"id\":\"i\",\"type\":\"pid\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[106,1],\"end\":[110,5]}}","procedure setPredNil(var i: pid);"},{"setPred",{{112,1},{116,5}},"{\"$type\":\"procedure\",\"label\":\"setPred\",\"params\":[{\"$type\":\"param\",\"id\":\"i\",\"type\":\"pid\"},{\"$type\":\"param\",\"id\":\"n\",\"type\":\"pid\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[112,1],\"end\":[116,5]}}","procedure setPred(var i,n: pid);"},{"setTailNil",{{118,1},{122,5}},"{\"$type\":\"procedure\",\"label\":\"setTailNil\",\"params\":[{\"$type\":\"param\",\"id\":\"i\",\"type\":\"pid\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[118,1],\"end\":[122,5]}}","procedure setTailNil(var i: pid);"},{"setTail",{{124,1},{128,5}},"{\"$type\":\"procedure\",\"label\":\"setTail\",\"params\":[{\"$type\":\"param\",\"id\":\"i\",\"type\":\"pid\"},{\"$type\":\"param\",\"id\":\"n\",\"type\":\"pid\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[124,1],\"end\":[128,5]}}","procedure setTail(var i,n: pid);"},{"setUsuperNil",{{130,1},{134,5}},"{\"$type\":\"procedure\",\"label\":\"setUsuperNil\",\"params\":[{\"$type\":\"param\",\"id\":\"i\",\"type\":\"pid\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[130,1],\"end\":[134,5]}}","procedure setUsuperNil(var i: pid);"},{"setUsuper",{{136,1},{140,5}},"{\"$type\":\"procedure\",\"label\":\"setUsuper\",\"params\":[{\"$type\":\"param\",\"id\":\"i\",\"type\":\"pid\"},{\"$type\":\"param\",\"id\":\"n\",\"type\":\"pid\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[136,1],\"end\":[140,5]}}","procedure setUsuper(var i,n: pid);"},{"setLockNil",{{142,1},{146,5}},"{\"$type\":\"procedure\",\"label\":\"setLockNil\",\"params\":[],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[142,1],\"end\":[146,5]}}","procedure setLockNil();"},{"setLock",{{148,1},{152,5}},"{\"$type\":\"procedure\",\"label\":\"setLock\",\"params\":[{\"$type\":\"param\",\"id\":\"i\",\"type\":\"pid\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[148,1],\"end\":[152,5]}}","procedure setLock(var i: pid);"},};
/* the number of error statements in the model */
#define _ROMP_ERRORS_LEN (1ul)
  /* the info/metadata about the murphi error statements in the model */
  const ::romp::MErrorInfo ERROR_INFOS[_ROMP_ERRORS_LEN] = {{"tail cannot be nil at this point.",{{268,7},{268,48}},0,"{\"$type\":\"error-statement\",\"label\":\"tail cannot be nil at this point.\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[268,7],\"end\":[268,48]}"},};
/* the number of property statements & rules in the model */
#define _ROMP_PROPERTIES_LEN (1ul)
  /* the info/metadata about the murphi properties in the model */
  const ::romp::PropertyInfo PROPERTY_INFOS[_ROMP_PROPERTIES_LEN] = {{"property2",::romp::PropertyType::INVARIANT,"(!exists i1 : pid do exists i2 : pid do (((i1 != i2) & (P[i1] = L6)) & (P[i2] = L6)) endexists endexists)",0,{{322,1},{330,8}},"{\"$type\":\"property\",\"type\":\"invariant\",\"expr\":\"(!exists i1 : pid do exists i2 : pid do (((i1 != i2) & (P[i1] = L6)) & (P[i2] = L6)) endexists endexists)\",\"label\":\"property2\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[322,1],\"end\":[330,8]}"},};
/* the number of start state rules (before ruleset expansions) in the model */
#define _ROMP_STARTSTATE_INFO_LEN (1ul)
  /* the info/metadata about the startstate rules in the model */
  const ::romp::StartStateInfo STARTSTATE_INFOS[_ROMP_STARTSTATE_INFO_LEN] = {{"startstate1",{{309,1},{320,4}},"{\"$type\":\"startstate\",\"label\":\"startstate1\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[309,1],\"end\":[320,4]}"},};
/* the number of rules in the model, BEFORE ruleset expansion */
#define _ROMP_RULESETS_LEN (13ul)
  /* the info/metadata about the rules in the model */
  const ::romp::RuleInfo RULE_SET_INFOS[_ROMP_RULESETS_LEN] = {{"execute_non_crit_and_assign_next_nil",{{156,3},{162,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L0)\",\"label\":\"execute_non_crit_and_assign_next_nil\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[156,3],\"end\":[162,6]}"},{"execute_assign_pred_FandS_L_I",{{164,3},{176,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L1)\",\"label\":\"execute_assign_pred_FandS_L_I\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[164,3],\"end\":[176,6]}"},{"execute_if_pred_nil",{{178,3},{188,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L2)\",\"label\":\"execute_if_pred_nil\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[178,3],\"end\":[188,6]}"},{"execute_assign_Ilocked_true",{{190,3},{196,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L3)\",\"label\":\"execute_assign_Ilocked_true\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[190,3],\"end\":[196,6]}"},{"execute_assign_prednext_I",{{198,3},{204,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L4)\",\"label\":\"execute_assign_prednext_I\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[198,3],\"end\":[204,6]}"},{"execute_repeat_while_Ilocked",{{206,3},{214,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L5)\",\"label\":\"execute_repeat_while_Ilocked\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[206,3],\"end\":[214,6]}"},{"execute_crit_and_if_Inext_nil",{{216,3},{226,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L6)\",\"label\":\"execute_crit_and_if_Inext_nil\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[216,3],\"end\":[226,6]}"},{"execute_assign_oldtail_FandS_L_nil",{{228,3},{240,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L7)\",\"label\":\"execute_assign_oldtail_FandS_L_nil\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[228,3],\"end\":[240,6]}"},{"execute_if_tail_eq_I",{{242,3},{252,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L8)\",\"label\":\"execute_if_tail_eq_I\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[242,3],\"end\":[252,6]}"},{"execute_assign_usuper_FandS_L_tail",{{254,3},{274,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L9)\",\"label\":\"execute_assign_usuper_FandS_L_tail\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[254,3],\"end\":[274,6]}"},{"execute_repeat_while_Inext_nil",{{276,3},{284,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L10)\",\"label\":\"execute_repeat_while_Inext_nil\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[276,3],\"end\":[284,6]}"},{"execute_if_usuper_neq_nil_then_assign_usupernext_Inext",{{286,3},{297,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L11)\",\"label\":\"execute_if_usuper_neq_nil_then_assign_usupernext_Inext\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[286,3],\"end\":[297,6]}"},{"execute_assign_Inextlocked_false",{{299,3},{305,6}},"{\"$type\":\"rule\",\"expr\":\"(P[i] = L12)\",\"label\":\"execute_assign_Inextlocked_false\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/mcslock2.m\",\"start\":[299,3],\"end\":[305,6]}"},};

}

/* ======= Romp CALLER Lists ====== */
namespace __caller__ {
  typedef ::__model__::__Model__ State_t; // type mask for the generated state object
  /* --- Rules Generated by: `execute_non_crit_and_assign_next_nil` (RuleSet expansions) --- */
   bool __Rule_guard__execute_non_crit_and_assign_next_nil__0(const State_t& s){return s.__Rule_guard__execute_non_crit_and_assign_next_nil((::__type__::pid) 0); }
   void __Rule_action__execute_non_crit_and_assign_next_nil__0(State_t& s){s.__Rule_action__execute_non_crit_and_assign_next_nil((::__type__::pid) 0); }
   bool __Rule_guard__execute_non_crit_and_assign_next_nil__1(const State_t& s){return s.__Rule_guard__execute_non_crit_and_assign_next_nil((::__type__::pid) 1); }
   void __Rule_action__execute_non_crit_and_assign_next_nil__1(State_t& s){s.__Rule_action__execute_non_crit_and_assign_next_nil((::__type__::pid) 1); }
   bool __Rule_guard__execute_non_crit_and_assign_next_nil__2(const State_t& s){return s.__Rule_guard__execute_non_crit_and_assign_next_nil((::__type__::pid) 2); }
   void __Rule_action__execute_non_crit_and_assign_next_nil__2(State_t& s){s.__Rule_action__execute_non_crit_and_assign_next_nil((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_assign_pred_FandS_L_I` (RuleSet expansions) --- */
   bool __Rule_guard__execute_assign_pred_FandS_L_I__0(const State_t& s){return s.__Rule_guard__execute_assign_pred_FandS_L_I((::__type__::pid) 0); }
   void __Rule_action__execute_assign_pred_FandS_L_I__0(State_t& s){s.__Rule_action__execute_assign_pred_FandS_L_I((::__type__::pid) 0); }
   bool __Rule_guard__execute_assign_pred_FandS_L_I__1(const State_t& s){return s.__Rule_guard__execute_assign_pred_FandS_L_I((::__type__::pid) 1); }
   void __Rule_action__execute_assign_pred_FandS_L_I__1(State_t& s){s.__Rule_action__execute_assign_pred_FandS_L_I((::__type__::pid) 1); }
   bool __Rule_guard__execute_assign_pred_FandS_L_I__2(const State_t& s){return s.__Rule_guard__execute_assign_pred_FandS_L_I((::__type__::pid) 2); }
   void __Rule_action__execute_assign_pred_FandS_L_I__2(State_t& s){s.__Rule_action__execute_assign_pred_FandS_L_I((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_if_pred_nil` (RuleSet expansions) --- */
   bool __Rule_guard__execute_if_pred_nil__0(const State_t& s){return s.__Rule_guard__execute_if_pred_nil((::__type__::pid) 0); }
   void __Rule_action__execute_if_pred_nil__0(State_t& s){s.__Rule_action__execute_if_pred_nil((::__type__::pid) 0); }
   bool __Rule_guard__execute_if_pred_nil__1(const State_t& s){return s.__Rule_guard__execute_if_pred_nil((::__type__::pid) 1); }
   void __Rule_action__execute_if_pred_nil__1(State_t& s){s.__Rule_action__execute_if_pred_nil((::__type__::pid) 1); }
   bool __Rule_guard__execute_if_pred_nil__2(const State_t& s){return s.__Rule_guard__execute_if_pred_nil((::__type__::pid) 2); }
   void __Rule_action__execute_if_pred_nil__2(State_t& s){s.__Rule_action__execute_if_pred_nil((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_assign_Ilocked_true` (RuleSet expansions) --- */
   bool __Rule_guard__execute_assign_Ilocked_true__0(const State_t& s){return s.__Rule_guard__execute_assign_Ilocked_true((::__type__::pid) 0); }
   void __Rule_action__execute_assign_Ilocked_true__0(State_t& s){s.__Rule_action__execute_assign_Ilocked_true((::__type__::pid) 0); }
   bool __Rule_guard__execute_assign_Ilocked_true__1(const State_t& s){return s.__Rule_guard__execute_assign_Ilocked_true((::__type__::pid) 1); }
   void __Rule_action__execute_assign_Ilocked_true__1(State_t& s){s.__Rule_action__execute_assign_Ilocked_true((::__type__::pid) 1); }
   bool __Rule_guard__execute_assign_Ilocked_true__2(const State_t& s){return s.__Rule_guard__execute_assign_Ilocked_true((::__type__::pid) 2); }
   void __Rule_action__execute_assign_Ilocked_true__2(State_t& s){s.__Rule_action__execute_assign_Ilocked_true((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_assign_prednext_I` (RuleSet expansions) --- */
   bool __Rule_guard__execute_assign_prednext_I__0(const State_t& s){return s.__Rule_guard__execute_assign_prednext_I((::__type__::pid) 0); }
   void __Rule_action__execute_assign_prednext_I__0(State_t& s){s.__Rule_action__execute_assign_prednext_I((::__type__::pid) 0); }
   bool __Rule_guard__execute_assign_prednext_I__1(const State_t& s){return s.__Rule_guard__execute_assign_prednext_I((::__type__::pid) 1); }
   void __Rule_action__execute_assign_prednext_I__1(State_t& s){s.__Rule_action__execute_assign_prednext_I((::__type__::pid) 1); }
   bool __Rule_guard__execute_assign_prednext_I__2(const State_t& s){return s.__Rule_guard__execute_assign_prednext_I((::__type__::pid) 2); }
   void __Rule_action__execute_assign_prednext_I__2(State_t& s){s.__Rule_action__execute_assign_prednext_I((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_repeat_while_Ilocked` (RuleSet expansions) --- */
   bool __Rule_guard__execute_repeat_while_Ilocked__0(const State_t& s){return s.__Rule_guard__execute_repeat_while_Ilocked((::__type__::pid) 0); }
   void __Rule_action__execute_repeat_while_Ilocked__0(State_t& s){s.__Rule_action__execute_repeat_while_Ilocked((::__type__::pid) 0); }
   bool __Rule_guard__execute_repeat_while_Ilocked__1(const State_t& s){return s.__Rule_guard__execute_repeat_while_Ilocked((::__type__::pid) 1); }
   void __Rule_action__execute_repeat_while_Ilocked__1(State_t& s){s.__Rule_action__execute_repeat_while_Ilocked((::__type__::pid) 1); }
   bool __Rule_guard__execute_repeat_while_Ilocked__2(const State_t& s){return s.__Rule_guard__execute_repeat_while_Ilocked((::__type__::pid) 2); }
   void __Rule_action__execute_repeat_while_Ilocked__2(State_t& s){s.__Rule_action__execute_repeat_while_Ilocked((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_crit_and_if_Inext_nil` (RuleSet expansions) --- */
   bool __Rule_guard__execute_crit_and_if_Inext_nil__0(const State_t& s){return s.__Rule_guard__execute_crit_and_if_Inext_nil((::__type__::pid) 0); }
   void __Rule_action__execute_crit_and_if_Inext_nil__0(State_t& s){s.__Rule_action__execute_crit_and_if_Inext_nil((::__type__::pid) 0); }
   bool __Rule_guard__execute_crit_and_if_Inext_nil__1(const State_t& s){return s.__Rule_guard__execute_crit_and_if_Inext_nil((::__type__::pid) 1); }
   void __Rule_action__execute_crit_and_if_Inext_nil__1(State_t& s){s.__Rule_action__execute_crit_and_if_Inext_nil((::__type__::pid) 1); }
   bool __Rule_guard__execute_crit_and_if_Inext_nil__2(const State_t& s){return s.__Rule_guard__execute_crit_and_if_Inext_nil((::__type__::pid) 2); }
   void __Rule_action__execute_crit_and_if_Inext_nil__2(State_t& s){s.__Rule_action__execute_crit_and_if_Inext_nil((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_assign_oldtail_FandS_L_nil` (RuleSet expansions) --- */
   bool __Rule_guard__execute_assign_oldtail_FandS_L_nil__0(const State_t& s){return s.__Rule_guard__execute_assign_oldtail_FandS_L_nil((::__type__::pid) 0); }
   void __Rule_action__execute_assign_oldtail_FandS_L_nil__0(State_t& s){s.__Rule_action__execute_assign_oldtail_FandS_L_nil((::__type__::pid) 0); }
   bool __Rule_guard__execute_assign_oldtail_FandS_L_nil__1(const State_t& s){return s.__Rule_guard__execute_assign_oldtail_FandS_L_nil((::__type__::pid) 1); }
   void __Rule_action__execute_assign_oldtail_FandS_L_nil__1(State_t& s){s.__Rule_action__execute_assign_oldtail_FandS_L_nil((::__type__::pid) 1); }
   bool __Rule_guard__execute_assign_oldtail_FandS_L_nil__2(const State_t& s){return s.__Rule_guard__execute_assign_oldtail_FandS_L_nil((::__type__::pid) 2); }
   void __Rule_action__execute_assign_oldtail_FandS_L_nil__2(State_t& s){s.__Rule_action__execute_assign_oldtail_FandS_L_nil((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_if_tail_eq_I` (RuleSet expansions) --- */
   bool __Rule_guard__execute_if_tail_eq_I__0(const State_t& s){return s.__Rule_guard__execute_if_tail_eq_I((::__type__::pid) 0); }
   void __Rule_action__execute_if_tail_eq_I__0(State_t& s){s.__Rule_action__execute_if_tail_eq_I((::__type__::pid) 0); }
   bool __Rule_guard__execute_if_tail_eq_I__1(const State_t& s){return s.__Rule_guard__execute_if_tail_eq_I((::__type__::pid) 1); }
   void __Rule_action__execute_if_tail_eq_I__1(State_t& s){s.__Rule_action__execute_if_tail_eq_I((::__type__::pid) 1); }
   bool __Rule_guard__execute_if_tail_eq_I__2(const State_t& s){return s.__Rule_guard__execute_if_tail_eq_I((::__type__::pid) 2); }
   void __Rule_action__execute_if_tail_eq_I__2(State_t& s){s.__Rule_action__execute_if_tail_eq_I((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_assign_usuper_FandS_L_tail` (RuleSet expansions) --- */
   bool __Rule_guard__execute_assign_usuper_FandS_L_tail__0(const State_t& s){return s.__Rule_guard__execute_assign_usuper_FandS_L_tail((::__type__::pid) 0); }
   void __Rule_action__execute_assign_usuper_FandS_L_tail__0(State_t& s){s.__Rule_action__execute_assign_usuper_FandS_L_tail((::__type__::pid) 0); }
   bool __Rule_guard__execute_assign_usuper_FandS_L_tail__1(const State_t& s){return s.__Rule_guard__execute_assign_usuper_FandS_L_tail((::__type__::pid) 1); }
   void __Rule_action__execute_assign_usuper_FandS_L_tail__1(State_t& s){s.__Rule_action__execute_assign_usuper_FandS_L_tail((::__type__::pid) 1); }
   bool __Rule_guard__execute_assign_usuper_FandS_L_tail__2(const State_t& s){return s.__Rule_guard__execute_assign_usuper_FandS_L_tail((::__type__::pid) 2); }
   void __Rule_action__execute_assign_usuper_FandS_L_tail__2(State_t& s){s.__Rule_action__execute_assign_usuper_FandS_L_tail((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_repeat_while_Inext_nil` (RuleSet expansions) --- */
   bool __Rule_guard__execute_repeat_while_Inext_nil__0(const State_t& s){return s.__Rule_guard__execute_repeat_while_Inext_nil((::__type__::pid) 0); }
   void __Rule_action__execute_repeat_while_Inext_nil__0(State_t& s){s.__Rule_action__execute_repeat_while_Inext_nil((::__type__::pid) 0); }
   bool __Rule_guard__execute_repeat_while_Inext_nil__1(const State_t& s){return s.__Rule_guard__execute_repeat_while_Inext_nil((::__type__::pid) 1); }
   void __Rule_action__execute_repeat_while_Inext_nil__1(State_t& s){s.__Rule_action__execute_repeat_while_Inext_nil((::__type__::pid) 1); }
   bool __Rule_guard__execute_repeat_while_Inext_nil__2(const State_t& s){return s.__Rule_guard__execute_repeat_while_Inext_nil((::__type__::pid) 2); }
   void __Rule_action__execute_repeat_while_Inext_nil__2(State_t& s){s.__Rule_action__execute_repeat_while_Inext_nil((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_if_usuper_neq_nil_then_assign_usupernext_Inext` (RuleSet expansions) --- */
   bool __Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__0(const State_t& s){return s.__Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext((::__type__::pid) 0); }
   void __Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__0(State_t& s){s.__Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext((::__type__::pid) 0); }
   bool __Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__1(const State_t& s){return s.__Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext((::__type__::pid) 1); }
   void __Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__1(State_t& s){s.__Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext((::__type__::pid) 1); }
   bool __Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__2(const State_t& s){return s.__Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext((::__type__::pid) 2); }
   void __Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__2(State_t& s){s.__Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext((::__type__::pid) 2); }
  /* --- Rules Generated by: `execute_assign_Inextlocked_false` (RuleSet expansions) --- */
   bool __Rule_guard__execute_assign_Inextlocked_false__0(const State_t& s){return s.__Rule_guard__execute_assign_Inextlocked_false((::__type__::pid) 0); }
   void __Rule_action__execute_assign_Inextlocked_false__0(State_t& s){s.__Rule_action__execute_assign_Inextlocked_false((::__type__::pid) 0); }
   bool __Rule_guard__execute_assign_Inextlocked_false__1(const State_t& s){return s.__Rule_guard__execute_assign_Inextlocked_false((::__type__::pid) 1); }
   void __Rule_action__execute_assign_Inextlocked_false__1(State_t& s){s.__Rule_action__execute_assign_Inextlocked_false((::__type__::pid) 1); }
   bool __Rule_guard__execute_assign_Inextlocked_false__2(const State_t& s){return s.__Rule_guard__execute_assign_Inextlocked_false((::__type__::pid) 2); }
   void __Rule_action__execute_assign_Inextlocked_false__2(State_t& s){s.__Rule_action__execute_assign_Inextlocked_false((::__type__::pid) 2); }

/* the number of rules in the model, AFTER ruleset expansion */
#define _ROMP_RULE_COUNT (39ul)


  // All of the rule sets in one place
  const ::romp::RuleSet RULESETS[_ROMP_RULESETS_LEN] = {
		{::__info__::RULE_SET_INFOS[0],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_non_crit_and_assign_next_nil__0,__Rule_action__execute_non_crit_and_assign_next_nil__0,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_non_crit_and_assign_next_nil__1,__Rule_action__execute_non_crit_and_assign_next_nil__1,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_non_crit_and_assign_next_nil__2,__Rule_action__execute_non_crit_and_assign_next_nil__2,::__info__::RULE_SET_INFOS[0],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[1],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_assign_pred_FandS_L_I__0,__Rule_action__execute_assign_pred_FandS_L_I__0,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_assign_pred_FandS_L_I__1,__Rule_action__execute_assign_pred_FandS_L_I__1,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_assign_pred_FandS_L_I__2,__Rule_action__execute_assign_pred_FandS_L_I__2,::__info__::RULE_SET_INFOS[1],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[2],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_if_pred_nil__0,__Rule_action__execute_if_pred_nil__0,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_if_pred_nil__1,__Rule_action__execute_if_pred_nil__1,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_if_pred_nil__2,__Rule_action__execute_if_pred_nil__2,::__info__::RULE_SET_INFOS[2],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[3],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_assign_Ilocked_true__0,__Rule_action__execute_assign_Ilocked_true__0,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_assign_Ilocked_true__1,__Rule_action__execute_assign_Ilocked_true__1,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_assign_Ilocked_true__2,__Rule_action__execute_assign_Ilocked_true__2,::__info__::RULE_SET_INFOS[3],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[4],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_assign_prednext_I__0,__Rule_action__execute_assign_prednext_I__0,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_assign_prednext_I__1,__Rule_action__execute_assign_prednext_I__1,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_assign_prednext_I__2,__Rule_action__execute_assign_prednext_I__2,::__info__::RULE_SET_INFOS[4],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[5],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_repeat_while_Ilocked__0,__Rule_action__execute_repeat_while_Ilocked__0,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_repeat_while_Ilocked__1,__Rule_action__execute_repeat_while_Ilocked__1,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_repeat_while_Ilocked__2,__Rule_action__execute_repeat_while_Ilocked__2,::__info__::RULE_SET_INFOS[5],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[6],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_crit_and_if_Inext_nil__0,__Rule_action__execute_crit_and_if_Inext_nil__0,::__info__::RULE_SET_INFOS[6],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_crit_and_if_Inext_nil__1,__Rule_action__execute_crit_and_if_Inext_nil__1,::__info__::RULE_SET_INFOS[6],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_crit_and_if_Inext_nil__2,__Rule_action__execute_crit_and_if_Inext_nil__2,::__info__::RULE_SET_INFOS[6],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[7],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_assign_oldtail_FandS_L_nil__0,__Rule_action__execute_assign_oldtail_FandS_L_nil__0,::__info__::RULE_SET_INFOS[7],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_assign_oldtail_FandS_L_nil__1,__Rule_action__execute_assign_oldtail_FandS_L_nil__1,::__info__::RULE_SET_INFOS[7],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_assign_oldtail_FandS_L_nil__2,__Rule_action__execute_assign_oldtail_FandS_L_nil__2,::__info__::RULE_SET_INFOS[7],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[8],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_if_tail_eq_I__0,__Rule_action__execute_if_tail_eq_I__0,::__info__::RULE_SET_INFOS[8],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_if_tail_eq_I__1,__Rule_action__execute_if_tail_eq_I__1,::__info__::RULE_SET_INFOS[8],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_if_tail_eq_I__2,__Rule_action__execute_if_tail_eq_I__2,::__info__::RULE_SET_INFOS[8],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[9],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_assign_usuper_FandS_L_tail__0,__Rule_action__execute_assign_usuper_FandS_L_tail__0,::__info__::RULE_SET_INFOS[9],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_assign_usuper_FandS_L_tail__1,__Rule_action__execute_assign_usuper_FandS_L_tail__1,::__info__::RULE_SET_INFOS[9],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_assign_usuper_FandS_L_tail__2,__Rule_action__execute_assign_usuper_FandS_L_tail__2,::__info__::RULE_SET_INFOS[9],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[10],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_repeat_while_Inext_nil__0,__Rule_action__execute_repeat_while_Inext_nil__0,::__info__::RULE_SET_INFOS[10],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_repeat_while_Inext_nil__1,__Rule_action__execute_repeat_while_Inext_nil__1,::__info__::RULE_SET_INFOS[10],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_repeat_while_Inext_nil__2,__Rule_action__execute_repeat_while_Inext_nil__2,::__info__::RULE_SET_INFOS[10],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[11],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__0,__Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__0,::__info__::RULE_SET_INFOS[11],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__1,__Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__1,::__info__::RULE_SET_INFOS[11],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__2,__Rule_action__execute_if_usuper_neq_nil_then_assign_usupernext_Inext__2,::__info__::RULE_SET_INFOS[11],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}},
		{::__info__::RULE_SET_INFOS[12],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__execute_assign_Inextlocked_false__0,__Rule_action__execute_assign_Inextlocked_false__0,::__info__::RULE_SET_INFOS[12],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":0}}]","i:=0"}, ::romp::Rule{__Rule_guard__execute_assign_Inextlocked_false__1,__Rule_action__execute_assign_Inextlocked_false__1,::__info__::RULE_SET_INFOS[12],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":1}}]","i:=1"}, ::romp::Rule{__Rule_guard__execute_assign_Inextlocked_false__2,__Rule_action__execute_assign_Inextlocked_false__2,::__info__::RULE_SET_INFOS[12],"[{\"$type\":\"kv-pair\",\"key\":\"i\",\"value\":{\"$type\":\"scalarset-value\",\"type\":\"scalarset(N)\",\"value\":2}}]","i:=2"}  	}}};

  /* --- Property Rule(s) generated by: `property2` (RuleSet expansions) --- */
   bool __Property__property2__0(const State_t& s) {return s.__Property__property2(); }

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
		{__Property__property2__0,::__info__::PROPERTY_INFOS[0],"[]",""},
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
