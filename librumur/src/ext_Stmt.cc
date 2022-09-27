
#include <rumur/ext/Stmt.h>


namespace rumur {
namespace ext {

MultisetAdd::MultisetAdd(const Ptr<Expr>& value_, const Ptr<Exp>& multiset_, const location& loc_)
  : multiset(multiset_), value(value_), loc(loc_) {}
MultisetAdd *MultisetAdd::clone() const { return new MultisetAdd(*this); }

void MultisetAdd::visit(BaseTraversal &visitor) { visitor.visit_multisetadd(*this); }
void MultisetAdd::visit(ConstBaseTraversal &visitor) const { visitor.visit_multisetadd(*this); }

void MultisetAdd::validate() const {
  if (const auto ms = dynamic_cast<const Multiset*>(multiset->type().get())) {
    if (not value->type()->coerces_to(*ms->element_type))
      throw Error("the type of expr=`" + value->to_string() +"` can't be coerced to type=`"
                  + ms->element_type->to_string() +"`", value->loc);
  } else
    throw Error("not of a multiset type as expected", multiset->loc);
}
void MultisetAdd::update() {
  //TODO
}
Ptr<AliasStmt> MultisetAdd::make_legacy() const {
  //TODO
}

// << ------------------------------------------------------------------------------------------ >> 

MultisetRemove::MultisetRemove(const Ptr<Expr>& index_, const Ptr<Expr>& multiset_, const location& loc_)
  : index(index_), multiset(multiset_), loc(loc_) {}
MultisetRemove *MultisetRemove::clone() const { return new MultisetRemove(*this); }

void MultisetRemove::visit(BaseTraversal &visitor) { visitor.visit_multisetremove(*this); }
void MultisetRemove::visit(ConstBaseTraversal &visitor) const { visitor.visit_multisetremove(*this); }

void MultisetRemove::validate() const {
  if (const auto ms = dynamic_cast<const Multiset*>(multiset->type().get())) {
    if (not index->type()->equal_to(*ms->index_type))
      throw Error("index not of valid type for specified multiset", index->loc);
  } else
    throw Error("not of a multiset type as expected", multiset->loc);
}
void MultisetRemove::update() {
  //TODO
}

Ptr<AliasStmt> MultisetRemove::make_legacy() const {
  throw Error("`MultisetRemove` is not supported in legacy model-checkers! (try MultisetRemovePred instead)", loc);
}

// << ------------------------------------------------------------------------------------------ >> 

MultisetRemovePred::MultisetRemovePred(const MultisetQuantifier& ms_quantifier_, const Ptr<Expr>& pred_, const location& loc_)
  : ms_quantifier(ms_quantifier_), pred(pred_), loc(loc_) {}
MultisetRemovePred *MultisetRemovePred::clone() const { return new MultisetRemovePred(*this); }

void MultisetRemovePred::visit(BaseTraversal &visitor) { visitor.visit_multisetremovepred(*this); }
void MultisetRemovePred::visit(ConstBaseTraversal &visitor) const { visitor.visit_multisetremovepred(*this); }

void MultisetRemovePred::validate() const {
  if (not pred->is_boolean())
    throw Error("expresion does not evaluate to a boolean", pred->loc);
  if (not pred->is_pure())
    throw Error("expression is not pure", pred->loc);
}
void MultisetRemovePred::update() {
  //TODO
}

Ptr<AliasStmt> MultisetRemovePred::make_legacy() const {
  //TODO
}


} //namespace ext
} //namespace rumur
