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

  public:
    // function attributes to prepend before the definition of any murphi function's C/C++ function.
    static std::string M_FUNCTION__FUNC_ATTRS;
    // function attributes to prepend before the definition of any murphi simple rule's guard C/C++ function.
    static std::string M_RULE_GUARD__FUNC_ATTRS;
    // function attributes to prepend before the definition of any murphi simple rule's action C/C++ function.
    static std::string M_RULE_ACTION__FUNC_ATTRS;
    // function attributes to prepend before the definition of any murphi start_state's C/C++ function.
    static std::string M_STARTSTATE__FUNC_ATTRS;
    // function attributes to prepend before the definition of any murphi invariant's (property-rule) C/C++ function.
    static std::string M_INVARIANT__FUNC_ATTRS;

  private:
    size_t indent_level = 0; // current indentation level

    

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
