/* this code was generated by Murphi2C */

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* built-in boolean type */
typedef bool boolean;

static void failed_assertion_(const char *message) {
  fprintf(stderr, "failed assertion: %s\n", message);
  exit(EXIT_FAILURE);
}

void (*failed_assertion)(const char *) = failed_assertion_;

static void failed_assumption_(const char *message) {
  fprintf(stderr, "failed assumption: %s\n", message);
  exit(EXIT_FAILURE);
}

void (*failed_assumption)(const char *) = failed_assumption_;

static void error_(const char *message) {
  fprintf(stderr, "error: %s\n", message);
  exit(EXIT_FAILURE);
}

void (*error)(const char *) = error_;

static void cover_(const char *message __attribute__((unused))) {}

void (*cover)(const char *) = cover_;

static void liveness_(const char *message __attribute__((unused))) {}

void (*liveness)(const char *) = liveness_;

// various printf wrappers to deal with the user having passed --value-type
static __attribute__((unused)) void print_int     (int v)      { printf("%d",          v); }
static __attribute__((unused)) void print_unsigned(unsigned v) { printf("%u",          v); }
static __attribute__((unused)) void print_short   (short v)    { printf("%hd",         v); }
static __attribute__((unused)) void print_long    (long v)     { printf("%ld",         v); }
static __attribute__((unused)) void print_int8_t  (int8_t v)   { printf("%" PRId8 , v); }
static __attribute__((unused)) void print_uint8_t (uint8_t v)  { printf("%" PRIu8 , v); }
static __attribute__((unused)) void print_int16_t (int16_t v)  { printf("%" PRId16, v); }
static __attribute__((unused)) void print_uint16_t(uint16_t v) { printf("%" PRIu16, v); }
static __attribute__((unused)) void print_int32_t (int32_t v)  { printf("%" PRId32, v); }
static __attribute__((unused)) void print_uint32_t(uint32_t v) { printf("%" PRIu32, v); }
static __attribute__((unused)) void print_int64_t (int64_t v)  { printf("%" PRId64, v); }
static __attribute__((unused)) void print_uint64_t(uint64_t v) { printf("%" PRIu64, v); }

// wrappers for producing literal expressions of value type
#define int_VALUE_C(v)      (v)
#define unsigned_VALUE_C(v) (v ## u)
#define short_VALUE_C(v)    ((short)(v))
#define long_VALUE_C(v)     (v ## l)
#define int8_t_VALUE_C(v)   INT8_C(v)
#define uint8_t_VALUE_C(v)  UINT8_C(v)
#define int16_t_VALUE_C(v)  INT16_C(v)
#define uint16_t_VALUE_C(v) UINT16_C(v)
#define int32_t_VALUE_C(v)  INT32_C(v)
#define uint32_t_VALUE_C(v) UINT32_C(v)
#define int64_t_VALUE_C(v)  INT64_C(v)
#define uint64_t_VALUE_C(v) UINT64_C(v)


// configuration parameters --
const __typeof__(((int)(4))) NODE_NUM = ((int)(4));

const __typeof__(((int)(2))) DATA_NUM = ((int)(2));

// type decl --
typedef int NODE;

typedef int DATA;

typedef enum { I, S, E, } CACHE_STATE;

typedef struct {
  CACHE_STATE State;
  DATA Data;
} CACHE;

typedef enum { Empty, ReqS, ReqE, Inv, InvAck, GntS, GntE, } MSG_CMD;

typedef struct {
  MSG_CMD Cmd;
  DATA Data;
} MSG;

// state variables --
struct { CACHE data[4]; } Cache; // Caches

struct { MSG data[4]; } Chan1; // Channels for Req*

struct { MSG data[4]; } Chan2; // Channels for Gnt* and Inv

struct { MSG data[4]; } Chan3; // Channels for InvAck

struct { boolean data[4]; } InvSet; // Nodes to be invalidated

struct { boolean data[4]; } ShrSet; // Nodes having S or E copies

boolean ExGntd; // E copy has been granted

MSG_CMD CurCmd; // Current request command

NODE CurPtr; // Current request node

DATA MemData; // Memory data

DATA AuxData; // Latest value of cache line

// Initial States --
void startstate_init(DATA d) {
  //
  // All nodes: init all cmd channels to be empty, Cache States I, 
  // the set of nodes to be invalidated is empty
  // and nodes having S or E copies empty
  // 
  do {
    for (int i = 0; i <= (NODE_NUM); i++) {
      (((Chan1).data[((i)) - int_VALUE_C(0)]).Cmd) = (Empty);
      (((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) = (Empty);
      (((Chan3).data[((i)) - int_VALUE_C(0)]).Cmd) = (Empty);
      (((Cache).data[((i)) - int_VALUE_C(0)]).State) = (I);
      ((InvSet).data[((i)) - int_VALUE_C(0)]) = (false);
      ((ShrSet).data[((i)) - int_VALUE_C(0)]) = (false);
    }
  } while (0);
  (ExGntd) = (false);
  (CurCmd) = (Empty);
  (MemData) = (d);
  (AuxData) = (d);
}


// State Transitions --
//------------------------------------------
bool guard_SendReqS(NODE i) {
  return (((((Chan1).data[((i)) - int_VALUE_C(0)]).Cmd) == (Empty)) && ((((Cache).data[((i)) - int_VALUE_C(0)]).State) == (I)));
}

void rule_SendReqS(NODE i) {
  // Any node with cmd req channel empty and cache I can request ReqS
  (((Chan1).data[((i)) - int_VALUE_C(0)]).Cmd) = (ReqS); // raises "ReqS" semaphore
}

//--------------------
bool guard_SendReqE(NODE i) {
  return (((((Chan1).data[((i)) - int_VALUE_C(0)]).Cmd) == (Empty)) && (((((Cache).data[((i)) - int_VALUE_C(0)]).State) == (I)) || ((((Cache).data[((i)) - int_VALUE_C(0)]).State) == (S))));
}

void rule_SendReqE(NODE i) {
  // Any node with cmd req channel empty and cache I/S can request ReqE
  (((Chan1).data[((i)) - int_VALUE_C(0)]).Cmd) = (ReqE); // raises "ReqE" semaphore
}

//------------------------------------------
bool guard_RecvReqS(NODE i) {
  return (((CurCmd) == (Empty)) && ((((Chan1).data[((i)) - int_VALUE_C(0)]).Cmd) == (ReqS)));
}

void rule_RecvReqS(NODE i) {
  // For any node that is waiting with ReqS requested, with CurCmd Empty 
  // we set CurCmd to ReqS on behalf of node i (setting CurPtr to point to it).
  // Then void Chan1 empty.
  // Now Set the nodes to be invalidated to the nodes having S or E copies.
  // prep action of dir ctrlr
  (CurCmd) = (ReqS);
  (CurPtr) = (i); // who sent me ReqS
  (((Chan1).data[((i)) - int_VALUE_C(0)]).Cmd) = (Empty); // drain its cmd
  do {
    for (int j = 0; j <= (NODE_NUM); j++) {
      ((InvSet).data[((j)) - int_VALUE_C(0)]) = ((ShrSet).data[((j)) - int_VALUE_C(0)]); // inv = nodes with S/E
    }
  } while (0);
}

//--------------------
bool guard_RecvReqE(NODE i) {
  return (((CurCmd) == (Empty)) && ((((Chan1).data[((i)) - int_VALUE_C(0)]).Cmd) == (ReqE)));
}

void rule_RecvReqE(NODE i) {
  // For any node that is waiting with ReqE requested, with CurCmd Empty 
  // we set CurCmd to ReqE on behalf of node i (setting CurPtr to point to it).
  // Then void Chan1 empty.
  // Now Set the nodes to be invalidated to the nodes having S or E copies.
  (CurCmd) = (ReqE);
  (CurPtr) = (i); // who sent me ReqE
  (((Chan1).data[((i)) - int_VALUE_C(0)]).Cmd) = (Empty); // drain its cmd
  do {
    for (int j = 0; j <= (NODE_NUM); j++) {
      ((InvSet).data[((j)) - int_VALUE_C(0)]) = ((ShrSet).data[((j)) - int_VALUE_C(0)]); // inv = nodes with S/E
    }
  } while (0);
}

//------------------------------------------
bool guard_SendInv(NODE i) {
  return ((((((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) == (Empty)) && (((InvSet).data[((i)) - int_VALUE_C(0)]) == (true))) && (((CurCmd) == (ReqE)) || (((CurCmd) == (ReqS)) && ((ExGntd) == (true)))));
}

void rule_SendInv(NODE i) {
  // For every node with Chan2 Cmd empty and InvSet true (node to be invalidated)
  // and if CurCmd is ReqE or (ReqS with ExGnt true), then
  // void Chan2 Cmd to Inv, and remove node i from InvSet (invalidation already set out)
  // Gnt* and Inv channel
  // DC: curcmd = E
  // DC: curcmd = S & ExGntd
  (((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) = (Inv); // fill Chan2 with Inv
  ((InvSet).data[((i)) - int_VALUE_C(0)]) = (false);
}

//--------------------
//
// When a node gets invalidated, it acks, and when it was E
// then the node (i) coughs up its cache data into Chan3
// Then cache state is I and undefine Cache Data
// 
bool guard_SendInvAck(NODE i) {
  return (((((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) == (Inv)) && ((((Chan3).data[((i)) - int_VALUE_C(0)]).Cmd) == (Empty)));
}

void rule_SendInvAck(NODE i) {
  (((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) = (Empty);
  (((Chan3).data[((i)) - int_VALUE_C(0)]).Cmd) = (InvAck);
  if ((((Cache).data[((i)) - int_VALUE_C(0)]).State) == (E)) {
    (((Chan3).data[((i)) - int_VALUE_C(0)]).Data) = (((Cache).data[((i)) - int_VALUE_C(0)]).Data);
  }
  (((Cache).data[((i)) - int_VALUE_C(0)]).State) = (I);
  memset(&(((Cache).data[((i)) - int_VALUE_C(0)]).Data), 0, sizeof((((Cache).data[((i)) - int_VALUE_C(0)]).Data)));
}

//--------------------
bool guard_RecvInvAck(NODE i) {
  return (((((Chan3).data[((i)) - int_VALUE_C(0)]).Cmd) == (InvAck)) && ((CurCmd) != (Empty)));
}

void rule_RecvInvAck(NODE i) {
  (((Chan3).data[((i)) - int_VALUE_C(0)]).Cmd) = (Empty);
  ((ShrSet).data[((i)) - int_VALUE_C(0)]) = (false);
  if ((ExGntd) == (true)) {
    (ExGntd) = (false);
    (MemData) = (((Chan3).data[((i)) - int_VALUE_C(0)]).Data);
    memset(&(((Chan3).data[((i)) - int_VALUE_C(0)]).Data), 0, sizeof((((Chan3).data[((i)) - int_VALUE_C(0)]).Data)));
  }
}

//------------------------------------------
bool guard_SendGntS(NODE i) {
  return (((((CurCmd) == (ReqS)) && ((CurPtr) == (i))) && ((((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) == (Empty))) && ((ExGntd) == (false)));
}

void rule_SendGntS(NODE i) {
  (((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) = (GntS);
  (((Chan2).data[((i)) - int_VALUE_C(0)]).Data) = (MemData);
  ((ShrSet).data[((i)) - int_VALUE_C(0)]) = (true);
  (CurCmd) = (Empty);
  memset(&(CurPtr), 0, sizeof((CurPtr)));
}

//--------------------
bool guard_SendGntE(NODE i) {
  return ((((((CurCmd) == (ReqE)) && ((CurPtr) == (i))) && ((((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) == (Empty))) && ((ExGntd) == (false))) && ({ bool res_ = true; for (int j = 0; j <= (NODE_NUM); j++) { res_ &= (((ShrSet).data[((j)) - int_VALUE_C(0)]) == (false)); } res_; }));
}

void rule_SendGntE(NODE i) {
  // nodes having S or E status
  (((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) = (GntE);
  (((Chan2).data[((i)) - int_VALUE_C(0)]).Data) = (MemData);
  ((ShrSet).data[((i)) - int_VALUE_C(0)]) = (true);
  (ExGntd) = (true);
  (CurCmd) = (Empty);
  memset(&(CurPtr), 0, sizeof((CurPtr)));
}

//--------------------
bool guard_RecvGntS(NODE i) {
  return ((((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) == (GntS));
}

void rule_RecvGntS(NODE i) {
  (((Cache).data[((i)) - int_VALUE_C(0)]).State) = (S);
  (((Cache).data[((i)) - int_VALUE_C(0)]).Data) = (((Chan2).data[((i)) - int_VALUE_C(0)]).Data);
  (((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) = (Empty);
  memset(&(((Chan2).data[((i)) - int_VALUE_C(0)]).Data), 0, sizeof((((Chan2).data[((i)) - int_VALUE_C(0)]).Data)));
}

//--------------------
bool guard_RecvGntE(NODE i) {
  return ((((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) == (GntE));
}

void rule_RecvGntE(NODE i) {
  (((Cache).data[((i)) - int_VALUE_C(0)]).State) = (E);
  (((Cache).data[((i)) - int_VALUE_C(0)]).Data) = (((Chan2).data[((i)) - int_VALUE_C(0)]).Data);
  (((Chan2).data[((i)) - int_VALUE_C(0)]).Cmd) = (Empty);
  memset(&(((Chan2).data[((i)) - int_VALUE_C(0)]).Data), 0, sizeof((((Chan2).data[((i)) - int_VALUE_C(0)]).Data)));
}

//--------------------
bool guard_Store(NODE i, DATA d) {
  return ((((Cache).data[((i)) - int_VALUE_C(0)]).State) == (E));
}

void rule_Store(NODE i, DATA d) {
  // for every node i
  // for every data d
  // if node is in E
  (((Cache).data[((i)) - int_VALUE_C(0)]).Data) = (d); // store d into Cache[i].Data
  (AuxData) = (d); // Also update latest cache line value
}

// The node in E can get any "D" value
//------------------------------------------
//-- Invariant properties ----
bool CtrlProp(void) {
  return ({ bool res_ = true; for (int i = 0; i <= (NODE_NUM); i++) { res_ &= ({ bool res_ = true; for (int j = 0; j <= (NODE_NUM); j++) { res_ &= (!((i) != (j)) || ((!((((Cache).data[((i)) - int_VALUE_C(0)]).State) == (E)) || ((((Cache).data[((j)) - int_VALUE_C(0)]).State) == (I))) && (!((((Cache).data[((i)) - int_VALUE_C(0)]).State) == (S)) || (((((Cache).data[((j)) - int_VALUE_C(0)]).State) == (I)) || ((((Cache).data[((j)) - int_VALUE_C(0)]).State) == (S)))))); } res_; }); } res_; });
}

bool DataProp(void) {
  return ((!((ExGntd) == (false)) || ((MemData) == (AuxData))) && ({ bool res_ = true; for (int i = 0; i <= (NODE_NUM); i++) { res_ &= (!((((Cache).data[((i)) - int_VALUE_C(0)]).State) != (I)) || ((((Cache).data[((i)) - int_VALUE_C(0)]).Data) == (AuxData))); } res_; }));
}

