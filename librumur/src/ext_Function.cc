
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

void IBuiltInFunction::visit(BaseTraversal& visitor) { visitor.visit_BuiltInFunction(*this); }
void IBuiltInFunction::visit(ConstBaseTraversal& visitor) const { visitor.visit_BuiltInFunction(*this); }

// << ------------------------------------------------------------------------------------------ >> 


} //namespace ext
} //namespace rumur
