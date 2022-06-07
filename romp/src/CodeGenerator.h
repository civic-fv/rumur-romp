#pragma once

#include <cstddef>
#include <string>
#include <vector>
// #include <stack>
#include <rumur/Node.h>

#include "romp_def.hpp"

namespace romp {

  // some common supporting code used by code generation functions
  class CodeGenerator {

  private:
    size_t indent_level = 0; // current indentation level
    size_t scope_level = 0; // current scope level
    // std::stack<romp::Scope> scope;

    int next_rule_ref_id = 0;
    int next_invariant_ref_id = 0;
    int next_startstate_ref_id = 0;

    std::vector<guard_ref*> rule_guard_refs;
    std::vector<action_ref*> rule_action_refs;
    std::vector<invariant_ref*> invariant_refs;
    std::vector<startstate_ref*> startstate_refs;
    

  protected:
    // get a white space string for the current indentation level
    std::string indentation() const;

    // increase the current indentation level
    void indent();

    // decrease the current indentation level
    void dedent();

    // get a white space string for the current indentation level
    size_t get_scope_level() const;

    // increase the current scope level
    size_t enter_scope();

    // decrease the current scope level
    size_t leave_scope();

    // // get a white space string for the current indentation level
    // romp::Scope scope_level() const;

    // // increase the current scope level
    // void push_scope(romp::Scope*);

    // // decrease the current scope level
    // romp::Scope pop_scope();

  public:
    // make this class abstract
    virtual ~CodeGenerator() = 0;
  };

}
