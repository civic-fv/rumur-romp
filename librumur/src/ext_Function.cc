
#include <cstddef>
#include <memory>
#include <rumur/Decl.h>
#include <rumur/ext/Function.h>
#include <rumur/Ptr.h>
#include <rumur/Stmt.h>
#include <rumur/ext/TypeExpr.h>
#include <rumur/except.h>
#include <rumur/ext/traverse.h>
#include <string>
#include <utility>
#include <vector>

namespace rumur {
namespace ext {

IBuiltInFunction *clone() const { 
  assert(false && "DEV ERROR : You must override `<typename> *clone()` in all child classes of `rumur::ext::IBuiltInFunction` !!"); 
  return nullptr;
}

void IBuiltInFunction::visit(BaseTraversal& visitor) { assert(false && "DEV ERROR : You must override `visit(rumur::BaseTraversal&)` in all child classes of `rumur::ext::IBuiltInFunction` !!"); }
void IBuiltInFunction::visit(ConstBaseTraversal& visitor) const { assert(false && "DEV ERROR : You must override `visit(rumur::ConstBaseTraversal&) const` in all child classes of `rumur::ext::IBuiltInFunction` !!"); }

void IBuiltInFunction::validate() const { assert(false && "DEV ERROR : You must override `validate() const` in all child classes of `rumur::ext::IBuiltInFunction` !!"); }
void IBuiltInFunction::update() { assert(false && "DEV ERROR : You must override `update()` in all child classes of `rumur::ext::IBuiltInFunction` !!"); }

// << ------------------------------------------------------------------------------------------ >> 

IBuiltInFunctionCall *clone() const { 
  assert(false && "DEV ERROR : You must override `<typename> *clone()` in all child classes of `rumur::ext::IBuiltInFunctionCall` !!"); 
  return nullptr;
}

void IBuiltInFunctionCall::visit(BaseTraversal& visitor) { assert(false && "DEV ERROR : You must override `visit(rumur::BaseTraversal&)` in all child classes of `rumur::ext::IBuiltInFunctionCall` !!"); }
void IBuiltInFunctionCall::visit(ConstBaseTraversal& visitor) const { assert(false && "DEV ERROR : You must override `visit(rumur::ConstBaseTraversal&) const` in all child classes of `rumur::ext::IBuiltInFunctionCall` !!"); }

void IBuiltInFunctionCall::validate() const { assert(false && "DEV ERROR : You must override `validate() const` in all child classes of `rumur::ext::IBuiltInFunctionCall` !!"); }
void IBuiltInFunctionCall::update() { assert(false && "DEV ERROR : You must override `update()` in all child classes of `rumur::ext::IBuiltInFunctionCall` !!"); }

} //namespace ext
} //namespace rumur
