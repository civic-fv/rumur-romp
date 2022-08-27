--------------------------------------------------------------------------
-- Copyright (C) 1992 by the Board of Trustees of 			  
-- Leland Stanford Junior University.					  
--									  
-- This description is provided to serve as an example of the use	  
-- of the Murphi description language and verifier, and as a benchmark	  
-- example for other verification efforts.				  
--									  
-- License to use, copy, modify, sell and/or distribute this description  
-- and its documentation any purpose is hereby granted without royalty,   
-- subject to the following terms and conditions, provided		  
--									  
-- 1.  The above copyright notice and this permission notice must	  
-- appear in all copies of this description.				  
-- 									  
-- 2.  The Murphi group at Stanford University must be acknowledged	  
-- in any publication describing work that makes use of this example. 	  
-- 									  
-- Nobody vouches for the accuracy or usefulness of this description	  
-- for any purpose.							  
--------------------------------------------------------------------------

--------------------------------------------------------------------------
-- NOTE: This model with a "bug" inserted into it, 
--        might not compile with CMurphi 
--        or traditional/old/original Murphi
--       This is because the getRand function relies 
--        on bitwise AND, OR & NOT, which og spec does not have.
--       It is designed to work with the parser and AST from the rumur-lib 
--------------------------------------------------------------------------

--------------------------------------------------------------------------
--
--                                                                        
-- File:        muxn.m                                                    
--                                                                        
-- Content:     Peterson's algorithm (mutual exclusion for n-processes)   
--                                                                        
-- Summary of result:                                                     
--          1)  No bug is discovered 					  
--          2)  Details of result can be found at the end of this file.   
--                                                                        
-- References: 						       	       	  
-- Peterson, G.L.,  Myths about the mutual exclusion problem,             
-- Information processing letters, Vol 12, No 3, 1981.                    
--                                                                        
-- Date created:         28 Oct 92                                        
-- Last Modified:        17 Feb 93                                        
--                                                                        
--------------------------------------------------------------------------

Const
  N: 7;
  S: 3; -- the length of the bug sub sequence
  N_fact: (N*(N-1)*(N-2)*(N-3)*(N-4)*(N-4)*(N-5)*(N-6)); -- change this every time you change N
  N_pow_N: (N*N*N*N*N*N*N); -- change this every time you change N

Type
  Seed: N_fact..N_pow_N -- a seed type (unsigned short)
  --  The scalarset is used for symmetry, which is implemented in Murphi 1.5
  --  and not upgraded to Murphi 2.0 yet
  pid: scalarset (N);
  pid_r: 0..(N-1);
  -- pid: 1..N;
  priority: 0..N;
  label_t: Enum{L0, -- : non critical section; j := 1; while j<n do
		L1,  -- : Beginwhile Q[i] := j  
		L2,  -- : turn[j] := i   (asking the other process to take turn / update the turn system)
		L3,  -- : wait until (forall k != i, Q[k] < j) or turn[j] != i ; j++; Endwhile  (wait for your turn) <-- you get lock here
		L4   -- : critical section; Q[i] := 0  (your turn)
 		};
Var
  P: Array [ pid ] Of label_t;       -- stores the current state of each process
  Q: Array [ pid ] Of priority;      -- stores what priority each process has
  turn: Array [ priority ] Of pid;   -- maps each priority to a process
  localj: Array [ pid ] Of priority; -- maps each process to it's current priority

  -- locking history buffer
  lock_hist: Array [ pid_r ] Of pid_r;
  bug: Array [ pid_r ] Of pid_r; -- where we store first locking order to use as bug seq
  lh_size: pid_r;  -- start and end of circ buffer
  lh_is_full: boolean;  -- if we circling yet (aka need to move start/lh_i forward)

-- generate a random number 
function getRand(var seed: Seed;): pid_r
var r: Seed;
Begin
  r := seed % N;
  seed := seed / N;
  return r;
End;
-- function getRand(var seed: Seed): priority
-- type temp_t: -2147483647..2147483647;
-- var a,b,q: Seed;
--     t1,t2,t3,t4: temp_t;
-- Begin
--   a := seed; b := (seed / 2);
--   t1 := a | b; t3:= !a; t4 := !b; t2 := t3 | t4; 
--   q := t1 & t2; -- calculate xor the long way
--   seed := (((q) / 4) & 255) | ((seed & 127) * 8);
--   return seed % N;
-- End;


Ruleset i: pid  Do

  Rule "L0 execute inc j and while"
    P[i] = L0  ==>
  Begin
    localj[i] := 1;
    P[i] := L1; 
  End;

  Rule "L1 execute assign Qi j"
    P[i] = L1  ==>
  Begin
    Q[i] := localj[i];
    P[i] := L2;
  End;

  Rule "L2 execute assign TURNj i"
    P[i] = L2  ==>
  Begin
    turn[localj[i]]  := i;
    P[i] := L3; 
  End;

  Rule "L3 execute wait until"
    P[i] = L3  ==>  -- any process at L3 can do this but only 
  Begin
    If ( Forall k: pid Do
           ( k!=i ) -> ( Q[k]<localj[i] )  -- if all other process has a lower/worse priority
         End --forall
       | ( turn[localj[i]] != i ) ) -- ?!? if our current processes priority is not associated with our process ?!?
    Then                            -- ?!?  |-> this probs means if it is not our turn (this exists to keep the always action below from executing past range bounds)
      localj[i] := localj[i] + 1;  -- always increase local priority (i.e the for loop from wikipedia)
      If ( localj[i]<N )
      Then
        P[i] := L1; -- go update the Q again
      Else          -- when localj[i] = N -> it is now our turn 
        P[i] := L4; -- this assigns the lock to (us)
      End; --If 
    End; --If
  End;

  Rule "L4 execute critical and assign Qi 1"
    P[i] = L4  ==>
  Begin
    Q[i] := 1;
    P[i] := L0;
    -- we have fully reached the lock (update history)
    lock_hist[lh_size] := i;
    lh_size := (lh_size + 1) % N;
    If lh_size = 0
    Then
      lh_is_full := True;
    End
  End; 

End; --Ruleset


RuleSet rand_seed: Seed Do
  Startstate "init"
  var seed: Seed;
  Begin
    For i:pid Do
      P[i] := L0;
      Q[i] := 0;
    End; --For

    For i: priority Do
      Undefine turn[i];
    End; --For

    Clear localj;

    -- hist bug stuff
    seed := rand_seed;
    lh_is_full := False;
    Clear lock_hist;
    For i: pid_r Do
      bug[i] := getRand(seed);
    End;
  End;
End;

Invariant "only one P has lock"
  ! Exists i1: pid Do
    Exists i2: pid Do
      ( i1 != i2
      & P[i1] = L4 -- critical
      & P[i2] = L4 -- critical
      )
    End  --Exists
    End; --Exists


Invariant (!(lh_is_full) | (
            Exists b_i: 0..(N-S) Do
              Exists lh_i: 0..(N-S) Do
                  Forall j: 0..(S-1) Do
                    bug[b_i+j] = lock_hist[lh_i+j]
                  EndForall
              EndExists
            EndExists));




/******************

Summary of Result (using release 2.3):

1) 3 processes

   breath-first search
   29 bits (4 bytes) per state
   771 states with a max of about 54 states in queue
   2313 rules fired
   0.73s in sun sparc 2 station

2) 5 processes

   breath-first search
   63 bits (8 bytes) per state
   576,551 states with a max of about 22,000 states in queue
   2,882,755 rules fired
   1201.66s in sun sparc 2 station

2.73S

* 3 processes (sparc  2 station)
	* The size of each state is 35 bits (rounded up to 5 bytes).

	BFS -nosym
	882 states, 2646 rules fired in 0.73s.

	BFS -sym1
	172 states, 516 rules fired in 0.36s.

* 5 processes (sparc 2 station)
	* The size of each state is 63 bits (rounded up to 8 bytes).

	BFS -sym1
	6770 states, 33850 rules fired in 22.55s.
	249 states max in the queue.

	BFS -nosym
	628868 states, 3144340 rules fired in 758.92s.
	25458 states max in the queue.

gamma2.9S on theforce.stanford.edu 

  	5 proc
	-O4 compile 119.7s 2.7Mbytes
	    (24 bytes per states)
	-sym2,3,4  6770 states, 33850 rules 14.35s

	6 proc
	-O4 compile 120.2s 2.7Mbytes
	    (28 bytes per states)
        -sym2,3,4  35,159 states, 210954 rules 117.45s 

Release 2.9S (Sparc 20, cabbage.stanford.edu)

   7 processes
         * The size of each state is 232 bits (rounded up to 32 bytes).
      -b * The size of each state is 101 bits (rounded up to 16 bytes).

         163298 states, 1143086 rules fired in 341.93s.
      -b 163298 states, 1143086 rules fired in 378.04s.
      -c 163298 states, 1143086 rules fired in 292.42s.

******************/

