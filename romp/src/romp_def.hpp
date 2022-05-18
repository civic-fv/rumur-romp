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

#pragma once

#include <rumur/Node.h>
#include <rumur/Function.h>
#include <string>
#include <vector>


namespace romp
{
  // A struct for keeping track of the current scope
  struct Scope {
    size_t id;
    int level;
    rumur::Node* node;
    std::string name;
    Scope(rumur::Node* node, std::string name)
      : id(node->unique_id), level(-1), node(node), name(name)
    {}
    Scope(rumur::Node* node)
      : Scope(node, static_cast<rumur::Function*>(node)->name)
    {}
  };

  struct entity_ref
  {
    const std::string name;
    entity_ref(std::string name) : name(name) {}
  };

  struct guard_ref : public entity_ref
  {

  };

  struct action_ref : public entity_ref
  {

  };

  struct startstate_ref : public guard_ref
  {

  };

  class invariant_ref : public entity_ref
  {

  };


  /**
   * @brief A singleton class that holds the config options for the 
   */
  class CODEGEN_CONFIG
  {
    public:
      static std::string M_CONSTANT__VAR_ATTRS;

      static std::string M_TYPEDEF__TYPE_VAR_ATTRS;

      static std::string M_FUNCTION__FUNC_ATTRS;
      static std::string M_RULE_GUARD__FUNC_ATTRS;
      static std::string M_RULE_ACTION__FUNC_ATTRS;
      static std::string M_STARTSTATE__FUNC_ATTRS;
      static std::string M_INVARIANT__FUNC_ATTRS;

  };
  

}

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
