/*
	Generated code for a romp "parallel random walker" verification tool based off of the Murphi Model described in:
		Original Murphi Model: romp/tests/cache3.m
	Please build with the following command(s):
		<not-implemented-yet>
*/


#define __romp__GENERATED_CODE


#define _ROMP_STATE_TYPE ::__model__::__Model__


#define _ROMP_STATE_HISTORY_LEN (4ul)


#define _ROMP_THREAD_TO_RW_RATIO (8ul)


#define __model__filename "romp/tests/cache3.m"


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
  const __typeof__(((int)(1))) HomeCount = ((int)(1)); // number of homes.
  const __typeof__(((int)(2))) ProcCount = ((int)(2)); // number of processors.
  const __typeof__(((int)(1))) AddressCount = ((int)(1)); // number of addresses
  const __typeof__(((int)(1))) ValueCount = ((int)(1)); // number of data values
  const __typeof__((ProcCount)) DirMax = (ProcCount); // Maximum number of dir
  const __typeof__(((DirMax) + ((int)(3)))) NetMax = ((DirMax) + ((int)(3))); // Network capacity.  Must be at least DirMax to
  typedef range_t Home;

  typedef range_t Proc;

  typedef range_t Node;

  typedef range_t Address;

  typedef range_t Value;

  typedef enum { Cache_Read, Cache_Read_Ex, Cache_Promote, Uncache, Write_Back, Update, Data, Invalidate, Uncache_Ack, Write_Back_Ack, Inv_Ack, Fw_Cache_R, Fw_Cache_R_Ex, Ack_Fw_Cache_Ex, Cache_Promote_Ack, } Message_Type;

  typedef struct {
      //---------------------------------------------------------------------
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
      //-----------------------------------------------------------------------
      //
      // File : cache3.m
      //
      // Cache Coherence Protocol on a General Network (message order
      // is NOT preserved).
      //
      // David L. Dill at Stanford University.   Initial protocol design
      //  by Andreas Nowatzyk, and the initial description was derived
      //  from a description by Ken McMillan.  Modified by Norris Ip
      //
      // Note from Dill: If I had to do it over again, I would model the
      // network as a 2D array indexed by source and destination instead
      // of a single array with all the messages in arbitrary order.
      //
      //  The invariant is also an unnecessarily complicated mess!
      //
      //-----------------------------------------------------------------------
      //
      // Status:  No violations of verification conditions, but it does
      // not necessarily implement an appropriate memory model.
      //
      //------------------------------------------------------------------------
      // 
      // Note:
      //
      // For verification, it is convenient to split nodes into two
      // parts: "homes", which have the Memory, Directory and Remote Access
      // System (RAS) and "procs" (processors), which have the Cache and
      // Pending Message Module (PMM).
      //
      //--------------------------------------------- March 4, 1993 ------------
      // number of homes.
      // number of processors.
      // number of addresses
      // number of data values
      // Maximum number of dir
      // entries that can be kept
      // Network capacity.  Must be at least DirMax to
      // allow for invalidations on Cache_Promote,
      // Cache_Read_Ex.
      // Kludge! so I don't have to redo all message field types.
      // request for shared copy
      // request for master copy
      // request for changing shared copy
      // to master copy
      // request for removing shared copy
      // request for removing master copy
      // update data memory w/ new value
      // reply with data
      // request invalidate data at cache
      // acknowledge for uncache data
      // acknowledge for write back
      // acknowledge for invalidate
      // forwarded shared copy request
      // to remote
      // forwarded master copy request
      // to remote
      // acknowledge for forwarded
      // master copy
      // grant permission to change to
      // master copy
      ::__type__::Message_Type MType;
      ::__type__::Node Source;
      ::__type__::Node Destination;
      ::__type__::Node Aux; // Local in Fw_Cache_R, Fw_Cache_R_Ex, Home in Data.
      ::__type__::Address Address;
      ::__type__::Value Value;
    } Message;

  typedef struct {
