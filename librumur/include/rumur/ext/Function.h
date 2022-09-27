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
struct RUMUR_API_WITH_RTTI IBuiltInFunction : public Function {

  IBuiltInFunction *clone() const override;
  ~IBuiltInFunction() = default;

  visit(BaseTraversal& visitor) override;
  visit(ConstBaseTraversal& visitor) const override;

  void validate() const override;
  void update() override;

protected:
  IBuiltInFunction(); // this is a pseudo abstract parent class
};


/**
 * @brief A matching function call for IBuiltInFunction.
 */
struct RUMUR_API_WITH_RTTI IBuiltInFunctionCall : public FunctionCall {

  IBuiltInFunctionCall *clone() const override;
  ~IBuiltInFunctionCall() = default;

  visit(BaseTraversal& visitor) override;
  visit(ConstBaseTraversal& visitor) const override;

  void validate() const override;
  void update() override;

protected:
  IBuiltInFunctionCall(); // this is a pseudo abstract parent class
};

} //namespace ext
} //namespace rumur
