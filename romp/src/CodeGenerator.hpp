/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file CodeGenerator.hpp
 * 
 * @brief The place where dreams come to die
 * 
 * @date 2022/06/10
 * @version 0.1
 */

#pragma once

#include <cstddef>
#include <string>
#include <vector>
// #include <stack>

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
