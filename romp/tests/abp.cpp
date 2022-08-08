/*
	Generated code for a romp "parallel random walker" verification tool based off of the Murphi Model described in:
		Original Murphi Model: romp/tests/abp.m
	Please build with the following command(s):
		<not-implemented-yet>
*/


#define __romp__GENERATED_CODE


#define _ROMP_STATE_TYPE ::__model__::__Model__


#define _ROMP_STATE_HISTORY_LEN (4ul)


#define _ROMP_THREAD_TO_RW_RATIO (8ul)


#define __model__filename "romp/tests/abp.m"


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
  typedef enum { EMPTY, PACKET, ACK, CORRUPT, } PacketType;

  typedef range_t ValueType;

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
      // appear in all copies of this description.				 
      // 									 
      // 2.  The Murphi group at Stanford University must be acknowledged	 
      // in any publication describing work that makes use of this example. 	 
      // 									 
      // Nobody vouches for the accuracy or usefulness of this description	 
      // for any purpose.							 
      //--------------------------------------------------------------------
      // Filename: 	abp.m
      // Content:	Simplex communication over a lossy channel
      // 		using a 1 bit sequence number
      // Version:	Murphi 2.3
      // Engineer:	Originally by C. Han Yang, 1991
      //		Update to Murphi 2.3 by Ralph Melton, 1993.
      //--------------------------------------------------------------------
      /*
       Simplex communication over a lossy media using a 1 bit sequence number
       */
      ::__type__::PacketType Status;
      ::__type__::ValueType Value;
      ::__type__::ValueType SeqNo;
    } PacketRecord;

  typedef range_t anon0x0_t; // Flage indicating that a "1" has been sent


}



/* ======= Generated Model & State ====== */

namespace __model__ {

  struct __Model__ {
    ::romp::IRandWalker* __rw__; // reference to associated romp rand walker


    /* ======= Model State Variables ====== */

    ::__type__::PacketRecord X2RChannel; // Channel from Transmitter to Receiver

    ::__type__::PacketRecord R2XChannel; // Channel from Receiver to Transmitter

    ::__type__::ValueType Value; // Value to be sent by the Transmitter

    ::__type__::ValueType OldValue; // Value sent by the Transmitter in last message

    ::__type__::anon0x0_t SentOne;

    ::__type__::ValueType ReceivedValue; // Inclusive OR of values received by

    ::__type__::ValueType XSeqNo; // Sequence number sent by the transmitter

    ::__type__::ValueType RSeqNo; // Sequence number next expected by the Receiver


    /* ======= Murphi Model Functions ====== */

    
    void SendPacket(::__type__::PacketRecord& Channel, const ::__type__::PacketType Status, const ::__type__::ValueType Value, const ::__type__::ValueType SeqNo)  {
      using namespace ::__type__;
      try {
        // Receiver
        ((Channel).Status) = (Status);
        ((Channel).Value) = (Value);
        ((Channel).SeqNo) = (SeqNo);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelFunctError(0) ); }
}


    /* ======= Murphi Model StartStates, Rules & Global Properties ====== */

     void __StartState__startstate0(void){
      using namespace ::__type__;
      try {
        // Acknowledgement from Receiver has been corrupted, Send old Value
        // 
        // Last message was properly accepted.  Send new packet
        // Last message was corrupted.  Must resend data.
        // Send acknowledgement without updating the sequence number
        // Packet is accepted
        // Check for duplicated packet
        /*
         The following 2 rules nondeterministically corrupts the channels
         */
        (Value) = ((int)(0));
        (OldValue) = ((int)(0));
        (SentOne) = ((int)(0));
        (XSeqNo) = ((int)(1));
        (RSeqNo) = ((int)(0));
        memset(&(X2RChannel), 0, sizeof((X2RChannel)));
        memset(&(R2XChannel), 0, sizeof((R2XChannel)));
        ((R2XChannel).Status) = (ACK);
        ((R2XChannel).Value) = ((int)(0));
        ((R2XChannel).SeqNo) = ((int)(0));
        (ReceivedValue) = ((int)(0));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelStartStateError(0) ); }
    }



    
    bool __Rule_guard__Generate_Value_0_to_send(void) const {
      using namespace ::__type__;
      try {
        return (((R2XChannel).Status) != (EMPTY));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Generate_Value_0_to_send(void) {
      using namespace ::__type__;
      try {
        (Value) = ((int)(0));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(0,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Generate_Value_1_to_send(void) const {
      using namespace ::__type__;
      try {
        return ((((R2XChannel).Status) != (EMPTY)) && ((SentOne) == ((int)(0))));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Generate_Value_1_to_send(void) {
      using namespace ::__type__;
      try {
        (Value) = ((int)(1));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(1,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Transmitter(void) const {
      using namespace ::__type__;
      try {
        return (((R2XChannel).Status) != (EMPTY));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Transmitter(void) {
      using namespace ::__type__;
      try {
        do {
          __typeof__(((R2XChannel).Status)) res_ = ((R2XChannel).Status);
          if (res_ == (CORRUPT)) {
            ::__model__::__Model__::SendPacket((X2RChannel), (PACKET), (OldValue), (XSeqNo));
          } else if (res_ == (ACK)) {
            if (((R2XChannel).SeqNo) != (XSeqNo)) {
              (XSeqNo) = (((XSeqNo) == ((int)(1))) ? ((int)(0)) : ((int)(1)));
              if ((SentOne) == ((int)(1))) {
                (Value) = ((int)(0));
              }
              (OldValue) = (Value);
              ::__model__::__Model__::SendPacket((X2RChannel), (PACKET), (Value), (XSeqNo));
              if ((Value) == ((int)(1))) {
                (SentOne) = ((int)(1));
              }
            } else {
              ::__model__::__Model__::SendPacket((X2RChannel), (PACKET), (OldValue), (XSeqNo));
            }
          } else {
            if (this->__rw__->error_handler(0u))
              throw ::romp::ModelMErrorError(0);

          }
        } while (0);
        memset(&(R2XChannel), 0, sizeof((R2XChannel)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(2,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Receiver(void) const {
      using namespace ::__type__;
      try {
        return (((X2RChannel).Status) != (EMPTY));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Receiver(void) {
      using namespace ::__type__;
      try {
        do {
          __typeof__(((X2RChannel).Status)) res_ = ((X2RChannel).Status);
          if (res_ == (CORRUPT)) {
            ::__model__::__Model__::SendPacket((R2XChannel), (ACK), ((int)(0)), (RSeqNo));
          } else if (res_ == (PACKET)) {
            if (((X2RChannel).SeqNo) == (RSeqNo)) {
              (RSeqNo) = (((RSeqNo) == ((int)(1))) ? ((int)(0)) : ((int)(1)));
              ::__model__::__Model__::SendPacket((R2XChannel), (ACK), ((int)(0)), (RSeqNo));
              if ((((X2RChannel).Value) == ((int)(1))) && ((ReceivedValue) == ((int)(1)))) {
                if (this->__rw__->error_handler(1u))
                  throw ::romp::ModelMErrorError(1);

              }
              if (((X2RChannel).Value) == ((int)(1))) {
                (ReceivedValue) = ((int)(1));
              }
            } else {
              ::__model__::__Model__::SendPacket((R2XChannel), (ACK), ((int)(0)), (RSeqNo));
            }
          } else {
            if (this->__rw__->error_handler(2u))
              throw ::romp::ModelMErrorError(2);

          }
        } while (0);
        memset(&(X2RChannel), 0, sizeof((X2RChannel)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(3,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Corrupt_X2RChannel(void) const {
      using namespace ::__type__;
      try {
        return ((((X2RChannel).Status) != (EMPTY)) && (((X2RChannel).Status) != (CORRUPT)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(4,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Corrupt_X2RChannel(void) {
      using namespace ::__type__;
      try {
        memset(&(X2RChannel), 0, sizeof((X2RChannel)));
        ((X2RChannel).Status) = (CORRUPT);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(4,::romp::ModelRuleError::ACTION) ); }
    }

    
    bool __Rule_guard__Corrupt_R2XChannel(void) const {
      using namespace ::__type__;
      try {
        return ((((R2XChannel).Status) != (EMPTY)) && (((R2XChannel).Status) != (CORRUPT)));
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(5,::romp::ModelRuleError::GUARD) ); }
    }

    
    void __Rule_action__Corrupt_R2XChannel(void) {
      using namespace ::__type__;
      try {
        memset(&(R2XChannel), 0, sizeof((R2XChannel)));
        ((R2XChannel).Status) = (CORRUPT);
      } catch (...) { ::std::throw_with_nested( ::romp::ModelRuleError(5,::romp::ModelRuleError::ACTION) ); }
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
  const ::romp::FunctInfo FUNCT_INFOS[_ROMP_FUNCTS_LEN] = {{"SendPacket",{{51,1},{57,5}},"{\"$type\":\"procedure\",\"label\":\"SendPacket\",\"params\":[{\"$type\":\"param\",\"id\":\"Channel\",\"type\":\"PacketRecord\"},{\"$type\":\"param\",\"id\":\"Status\",\"type\":\"PacketType\"},{\"$type\":\"param\",\"id\":\"Value\",\"type\":\"ValueType\"},{\"$type\":\"param\",\"id\":\"SeqNo\",\"type\":\"ValueType\"}],\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[51,1],\"end\":[57,5]}}","procedure SendPacket(Channel: PacketRecord; var Status: PacketType; var Value,SeqNo: ValueType);"},};
/* the number of error statements in the model */
#define _ROMP_ERRORS_LEN (3ul)
  /* the info/metadata about the murphi error statements in the model */
  const ::romp::MErrorInfo ERROR_INFOS[_ROMP_ERRORS_LEN] = {{"Invalid Packet Type received at the Transmitter\\n",{{102,7},{102,64}},0,"{\"$type\":\"error-statement\",\"label\":\"Invalid Packet Type received at the Transmitter\\\\n\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[102,7],\"end\":[102,64]}"},{"Receiver has received 2 packets with value 1\\n",{{125,11},{125,65}},0,"{\"$type\":\"error-statement\",\"label\":\"Receiver has received 2 packets with value 1\\\\n\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[125,11],\"end\":[125,65]}"},{"Invalid Packet Type received at the Receiver\\n",{{135,7},{135,61}},0,"{\"$type\":\"error-statement\",\"label\":\"Invalid Packet Type received at the Receiver\\\\n\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[135,7],\"end\":[135,61]}"},};
/* the number of property statements & rules in the model */
#define _ROMP_PROPERTIES_LEN (0ul)
  /* the info/metadata about the murphi properties in the model */
  const ::romp::PropertyInfo PROPERTY_INFOS[_ROMP_PROPERTIES_LEN] = {};
/* the number of start state rules (before ruleset expansions) in the model */
#define _ROMP_STARTSTATE_INFO_LEN (1ul)
  /* the info/metadata about the startstate rules in the model */
  const ::romp::StartStateInfo STARTSTATE_INFOS[_ROMP_STARTSTATE_INFO_LEN] = {{"startstate0",{{160,1},{173,4}},"{\"$type\":\"startstate\",\"label\":\"startstate0\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[160,1],\"end\":[173,4]}"},};
/* the number of rules in the model, BEFORE ruleset expansion */
#define _ROMP_RULESETS_LEN (6ul)
  /* the info/metadata about the rules in the model */
  const ::romp::RuleInfo RULE_SET_INFOS[_ROMP_RULESETS_LEN] = {{"Generate_Value_0_to_send",{{59,1},{64,4}},"{\"$type\":\"rule\",\"expr\":\"(R2XChannel.Status != EMPTY)\",\"label\":\"Generate_Value_0_to_send\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[59,1],\"end\":[64,4]}"},{"Generate_Value_1_to_send",{{66,1},{71,4}},"{\"$type\":\"rule\",\"expr\":\"((R2XChannel.Status != EMPTY) & (SentOne = 0))\",\"label\":\"Generate_Value_1_to_send\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[66,1],\"end\":[71,4]}"},{"Transmitter",{{73,1},{105,4}},"{\"$type\":\"rule\",\"expr\":\"(R2XChannel.Status != EMPTY)\",\"label\":\"Transmitter\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[73,1],\"end\":[105,4]}"},{"Receiver",{{107,1},{138,4}},"{\"$type\":\"rule\",\"expr\":\"(X2RChannel.Status != EMPTY)\",\"label\":\"Receiver\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[107,1],\"end\":[138,4]}"},{"Corrupt_X2RChannel",{{144,1},{150,4}},"{\"$type\":\"rule\",\"expr\":\"((X2RChannel.Status != EMPTY) & (X2RChannel.Status != CORRUPT))\",\"label\":\"Corrupt_X2RChannel\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[144,1],\"end\":[150,4]}"},{"Corrupt_R2XChannel",{{152,1},{158,4}},"{\"$type\":\"rule\",\"expr\":\"((R2XChannel.Status != EMPTY) & (R2XChannel.Status != CORRUPT))\",\"label\":\"Corrupt_R2XChannel\",\"loc\":{\"$type\":\"location\",\"file\":\"romp/tests/abp.m\",\"start\":[152,1],\"end\":[158,4]}"},};

}

/* ======= Romp CALLER Lists ====== */
namespace __caller__ {
  typedef ::__model__::__Model__ State_t; // type mask for the generated state object
  /* --- Rules Generated by: `Generate_Value_0_to_send` (RuleSet expansions) --- */
   bool __Rule_guard__Generate_Value_0_to_send__0(const State_t& s){return s.__Rule_guard__Generate_Value_0_to_send(); }
   void __Rule_action__Generate_Value_0_to_send__0(State_t& s){s.__Rule_action__Generate_Value_0_to_send(); }
  /* --- Rules Generated by: `Generate_Value_1_to_send` (RuleSet expansions) --- */
   bool __Rule_guard__Generate_Value_1_to_send__0(const State_t& s){return s.__Rule_guard__Generate_Value_1_to_send(); }
   void __Rule_action__Generate_Value_1_to_send__0(State_t& s){s.__Rule_action__Generate_Value_1_to_send(); }
  /* --- Rules Generated by: `Transmitter` (RuleSet expansions) --- */
   bool __Rule_guard__Transmitter__0(const State_t& s){return s.__Rule_guard__Transmitter(); }
   void __Rule_action__Transmitter__0(State_t& s){s.__Rule_action__Transmitter(); }
  /* --- Rules Generated by: `Receiver` (RuleSet expansions) --- */
   bool __Rule_guard__Receiver__0(const State_t& s){return s.__Rule_guard__Receiver(); }
   void __Rule_action__Receiver__0(State_t& s){s.__Rule_action__Receiver(); }
  /* --- Rules Generated by: `Corrupt_X2RChannel` (RuleSet expansions) --- */
   bool __Rule_guard__Corrupt_X2RChannel__0(const State_t& s){return s.__Rule_guard__Corrupt_X2RChannel(); }
   void __Rule_action__Corrupt_X2RChannel__0(State_t& s){s.__Rule_action__Corrupt_X2RChannel(); }
  /* --- Rules Generated by: `Corrupt_R2XChannel` (RuleSet expansions) --- */
   bool __Rule_guard__Corrupt_R2XChannel__0(const State_t& s){return s.__Rule_guard__Corrupt_R2XChannel(); }
   void __Rule_action__Corrupt_R2XChannel__0(State_t& s){s.__Rule_action__Corrupt_R2XChannel(); }

/* the number of rules in the model, AFTER ruleset expansion */
#define _ROMP_RULE_COUNT (6ul)


  // All of the rule sets in one place
  const ::romp::RuleSet RULESETS[_ROMP_RULESETS_LEN] = {
		{::__info__::RULE_SET_INFOS[0],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Generate_Value_0_to_send__0,__Rule_action__Generate_Value_0_to_send__0,::__info__::RULE_SET_INFOS[0],"[]",""}  	}},
		{::__info__::RULE_SET_INFOS[1],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Generate_Value_1_to_send__0,__Rule_action__Generate_Value_1_to_send__0,::__info__::RULE_SET_INFOS[1],"[]",""}  	}},
		{::__info__::RULE_SET_INFOS[2],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Transmitter__0,__Rule_action__Transmitter__0,::__info__::RULE_SET_INFOS[2],"[]",""}  	}},
		{::__info__::RULE_SET_INFOS[3],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Receiver__0,__Rule_action__Receiver__0,::__info__::RULE_SET_INFOS[3],"[]",""}  	}},
		{::__info__::RULE_SET_INFOS[4],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Corrupt_X2RChannel__0,__Rule_action__Corrupt_X2RChannel__0,::__info__::RULE_SET_INFOS[4],"[]",""}  	}},
		{::__info__::RULE_SET_INFOS[5],std::vector<::romp::Rule>{::romp::Rule{__Rule_guard__Corrupt_R2XChannel__0,__Rule_action__Corrupt_R2XChannel__0,::__info__::RULE_SET_INFOS[5],"[]",""}  	}}};


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
#define _ROMP_PROPERTY_RULES_LEN ((0ul) + ___propRule_assume_count___ + ___propRule_cover_count___ + ___propRule_liveness_count___)

  /* All of the property rules expanded in one place */
const ::romp::Property PROPERTIES[_ROMP_PROPERTY_RULES_LEN] = {
};

  /* --- StartState Rule(s) generated by: `startstate0` (RuleSet expansions) --- */
   void __StartState__startstate0__0(State_t& s){ s.__StartState__startstate0(); }


#define _ROMP_STARTSTATES_LEN (1ul) // the number of property rules (after ruleset expansion) in the model

  // All of the rule sets in one place
const ::romp::StartState STARTSTATES[_ROMP_STARTSTATES_LEN] = {{__StartState__startstate0__0,::__info__::STARTSTATE_INFOS[0],0,"[]",""}};


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
