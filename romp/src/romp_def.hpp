/**
 * @proj romp
 * @author Andrew Osterhout
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp_def.hpp
 * 
 * @brief A bunch of definitions for configuration 
 *        and other details in the romp codegen tool.
 * 
 * @date 2022/05/11
 * @version 0.1
 */

#ifndef _romp_src_romp_def_hpp
#define _romp_src_romp_def_hpp

#define ROMP_RESERVED_NAMES "rule_sets", "invariants", "start_states", "State", "RandWalker"

#define ROMP_JSON_TYPE "nlohmann::json"

#define ROMP_SHOW_TYPE_OPTION_EXPR "options.SHOW_TYPE"



// << ========================================================================================== >> 
// <<                                 SIMULATION CLASS CODE GEN                                  >> 
// << ========================================================================================== >> 

// #define __romp__Simulation_start \
//   "class Simulation\n"\
//   "{\n"\
//   "public:\n"\
//   "  State state;"\
//   "  /*<list* of callable rules>*/\n"\
//   "  /*<list* of callable invariants>*/\n"\
//   "  /*<list* of callable startstate gens>*/\n"\
//   "  \n"

// #define __romp__Simulation_Constructor \
//   "  \n"\
//   "  /* Class Constructor(s) */\n"\
//   "  \n"\
//   "  /* Class Deconstructor */\n"\
//   "  \n"

// #define __romp__Simulation_Methods \
//   "  \n"\
//   "  /* Helper methods */\n"

// #define __romp__Simulation_end \
//   __romp__Simulation_Constructor\
//   __romp__Simulation_Methods\
//   "\n"\
//   "};\n\n"


// << ========================================================================================== >> 
// <<                                   STATE CLASS CODE GEN                                     >> 
// << ========================================================================================== >> 

#define __romp__State_start \
  "  class Simulation::State\n"\
  "  {\n"\
  "  public:\n"\
  "    \n"

#define __romp__State_Constructor \
  "    \n"\
  "    /* Class Constructor(s) */\n"\
  "    \n"\
  "    /* Class Deconstructor */\n"\
  "    \n"

#define __romp__State_Methods \
  "    \n"\
  "    /* Helper methods */\n"

#define __romp__State_end \
  __romp__State_Constructor\
  __romp__State_Methods\
  "  \n"\
  "  };\n\n"


#endif
