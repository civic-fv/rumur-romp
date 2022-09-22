#pragma once

#include <rumur/ext/TypeExpr.h>
#include <rumur/Function.h>


#ifndef RUMUR_API_WITH_RTTI
#define RUMUR_API_WITH_RTTI __attribute__((visibility("default")))
#endif

namespace rumur {
namespace ext {

  /**
   * @brief A type of function that is dependent on the values in a TypeExpr.
   *        This should never be revealed in syntax but is useful for 
   *        making new syntax compatible with legacy systems. 
   * 
   *        WARNING: No validation is done when reached during normal AST traversal 
   *                 in the builtin validate operation.
   *                 Therefore it's exprs, stmts, decls, etc. must be validated 
   *                  from BuiltInFunctionCall.
   *        CONTRACT: All built in functions must be 100% pure.
   */
  struct RUMUR_API_WITH_RTTI IBuiltInFunction : Function {

    IBuiltInFunction() = 0; // this is a abstract parent class
    IBuiltInFunction *clone();

    visit(BaseTraversal& visitor);
    visit(ConstBaseTraversal& visitor) const;

    virtual TypeExpr& dependent_on() = 0;
  };

  /**
   * @brief A type of function that is dependent on the values in a TypeExpr.
   *        This should never be revealed in syntax but is useful for 
   *        making new syntax compatible with legacy systems. 
   * 
   *        WARNING: No validation is done when reached during normal AST traversal 
   *                 in the builtin validate operation.
   *                 Therefore it's exprs, stmts, decls, etc. must be validated 
   *                  from BuiltInFunctionCall.
   *        CONTRACT: All built in functions must be 100% pure.
   */
  struct RUMUR_API_WITH_RTTI IBuiltInProcedure : Function {

    IBuiltInProcedure() = 0; // this is a abstract parent class
    IBuiltInProcedure *clone() override = 0;

    visit(BaseTraversal& visitor);
    visit(ConstBaseTraversal& visitor) const;

    virtual TypeExpr& dependent_on() = 0;
  };

} //namespace ext
} //namespace rumur
