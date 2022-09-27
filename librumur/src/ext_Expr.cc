
#include "../../common/isa.h"
#include "location.hh"
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <gmpxx.h>
#include <iostream>
#include <limits>
#include <memory>
#include <rumur/Boolean.h>
#include <rumur/Decl.h>
#include <rumur/ext/Expr.h>
#include <rumur/Function.h>
#include <rumur/Ptr.h>
#include <rumur/ext/TypeExpr.h>
#include <rumur/except.h>
#include <rumur/ext/traverse.h>
#include <string>
#include <utility>
#include <vector>
#include <map>

namespace rumur {
namespace ext {

struct SubRangeSet {
  mpz_class abs_min;
  mpz_class abs_max;
  _Range* range_head = nullptr;
  void insert(mpz_class min, mpz_class max) {
    auto _insert [&](_Range* r) -> _Range* { // forward (r never null)
      if (min > r->max) { // case: r comes before range
        auto _r = new _Range{r,min,max,r->next};
        r->next = _r;
        return _r;
      }
      if (min >= r->min) {  // case: r overlaps with range // also case: r->max == min
        if (r->max >= max) // sub-case: r contains all of our range
          max = r->max;   // update max
        return r;
      }
      if (r->next == nullptr)
        return new _Range{r,min,max,nullptr};
      return _insert(r->next);
    };
    min = ((min <= abs_min) ? abs_min : min);
    max = ((max >= abs_max) ? abs_max : max);
    if (range_head == nullptr) {  // base case: empty
      range_head = new _Range{nullptr, min, max, nullptr};
      return;
    }
    _Range* range = _insert(range_head);
    auto _span [&](_Range* r) -> void { // forward (r can be null); !! trims ALL nodes it explores !!
      if (r == nullptr) // base case: END OF RANGE LIST
        return;
      if (max < r->min) // base case: NO OVERLAP
        return;  // do not trim r
      if (max >= r->min) { // case: r overlaps with range
        range->max = r->max; // update with current range with values from this overlapping range
        range->next = r->next;
      }
      if (max > r->max) // case: range completely overlaps with r
        _span(r->next);  // check next range for overlap
      delete r; // trim r
    };
    _span(range->next);  // never call with a node you don't want to loose!!
  }
  struct _Range {
    _Range* prev;
    mpz_class min;
    mpz_class max;
    _Range* next;
  };
  struct iterator {
    _Range* iter;
    _Range& operator*() const { return *iter; }
    _Range* operator->() const { return iter; }
    friend iterator& operator ++ (iterator& i) { i.iter = i.iter.next; return i; }
    friend iterator operator ++ (iterator& i, int v) { iterator cpy{i.iter}; ++i; return cpy; }
    friend bool operator == (const iterator& l, const iterator& r) { return l.iter == r.iter; }
    friend bool operator != (const iterator& l, const iterator& r) { return l.iter != r.iter; }
  };
  iterator* begin() { return iterator{range_head}; }
  iterator* end() { return iterator{nullptr}; }
};

SubRangeSet make_ranges(const ScalarsetUnion* u, const TypeExpr& type) {
  struct MakeRanges : public ConstExtTypeTraversal {
    SubRangeSet ranges;
    const Ptr<ScalarsetUnion> _u;
    make_ranges(const ScalarsetUnion*_u_) : _u(_u_), ranges({0_mpz,_u->count()}) {}
    bool _try_insert(std::string name) {
      auto _m = _u->members.find(name);
      if (_m != _u->members.end()) {
        ranges.insert(_m->second.min,_m->second.max)
        return true;
      }
      return false;
    }
    visit_array(const Array& n) { assert(false && "Array should be unreachable"); }
    visit_enum(const Enum& n) {
      for (const auto mp : n.members)
          _try_insert("_enum_"+mp.first);
    }
    visit_multiset(const Multiset& n) { assert(false && "Multiset should be unreachable"); }
    visit_range(const Range& n) { assert(false && "Range should be unreachable"); }
    visit_record(const Record& n) { assert(false && "Record should be unreachable"); }
    visit_scalarset(const Scalarset& n) { 
      throw Error("scalarsets must be defined with a name to be a member of a union!",n.loc); 
    }
    visit_scalarsetunion(const ScalarsetUnion& n) {
      for (const auto m : n.decl_list)
        dispatch(*m);
    }
    visit_typeexprid(const TypeExprID& n) {
      if (not _try_insert(n.referent->name))
        dispatch(*n.resolve());
    }
  };
  MakeRanges r(u);
  r.dispatch(type);
  return r.ranges;
}


// << ------------------------------------------------------------------------------------------ >> 


IsMember::IsMember(Ptr<Expr> designator_, Ptr<TypeExpr> type_, location loc_)
  : Or(nullptr,nullptr,loc_), designator(designator_), type(type_) {}
IsMember* IsMember::clone() { return new IsMember(*this); }

void IsMember::visit(BaseTraversal& visitor) { visitor.visit_ismember(*this); }
void IsMember::visit(ConstBaseTraversal& visitor) const { visitor.visit_ismember(*this); }

void IsMember::update() {
  // - not necessary since the change to the IExtNode and make_legacy design
  // validate_types();
  // if (const auto _u = dynamic_cast<const ScalarsetUnion*>(designator.type().get())) {
  //   r = make_ranges(_u,*type);
  //   // build the Exprs
  //   auto _build [&](SubRangeSet::_Range* r, Or& p_expr) -> void {
  //     if (r == nullptr) // safety case: def end of expr
  //       return;
  //     p_expr.lhs = Ptr<And>::make(
  //       Ptr<Leq>::make(Ptr<Number>::make(r->min), designator, loc),
  //       Ptr<Leq>::make(designator, Ptr<Number>::make(r->max), loc),
  //       loc
  //     );
  //     if (r->next == nullptr) { // base case: end of expr
  //       p_expr.rhs = rumur::False;
  //       return;
  //     } // else  // case: more ranges to insert into check
  //     p_expr.rhs = Ptr<Or>::make(nullptr,nullptr,loc);
  //     _build(r->next,_rhs);
  //   };
  //   if (ranges.range_head == nullptr)
  //     throw Error("IsMember use error : `" + type->to_string() + "` "
  //                 "has no intersection with the type of `"
  //                 + designator->to_string() +"` !", 
  //                 loc);
  //   _build(r.ranges.range_head, *this);
  // } else 
  //   assert(false && "DEV ERROR : should be unreachable (work done in validate_types)");
}


void IsMember::validate_types() const {
  struct validate_types : public ConstExtTypeTraversal {
    Ptr<ScalarsetUnion> _u;
    make_ranges(ScalarsetUnion*_u_) : _u(_u_), ranges({0_mpz,_u->count()}) {}
    bool _contains(std::string name) {
      return (_u->members.find(name) != _u->members.end())
    }
    visit_array(const Array& n) { throw Error("Arrays cant be members of scalarset unions!",n.loc); }
    visit_enum(const Enum& n) {
      for (const auto mp : e.members)
          _contains("_enum_"+mp.first);
    }
    visit_multiset(const Multiset& n) { throw Error("Multisets cant be members of scalarset unions!",n.loc); }
    visit_range(const Range& n) { throw Error("Ranges's cant be members of scalarset unions!",n.loc); }
    visit_record(const Record& n) { throw Error("Records's cant be members of scalarset unions!",n.loc); }
    visit_scalarset(const Scalarset& n) { throw Error("scalarsets must be defined with a name to be a member of a union!",n.loc); }
    visit_scalarsetunion(const ScalarsetUnion& n) {
      for (const auto m : n.decl_list)
        dispatch(*m);
    }
    visit_typeexprid(const TypeExprID& n) {
      if (n.referent == nullptr)
        throw Error(n.name+" was not resolved \t[dev-error]",n.loc)
      if (not _contains(n.referent->name))
        dispatch(*n.resolve());
    }
  };
  if (const auto _u = dynamic_cast<const ScalarsetUnion*>(designator.type().get())) {
    validate_types _vt(_u);
    _vt.dispatch(*type);
  } else 
    throw Error("IsMember use error : `" + designator->to_string() 
                + "` is not of type scalarset union, as IsMember expects", 
                designator.loc)
}
// this needs to be called after symbol resolution and a call to update()
void IsMember::validate() const {
  pre_validate();
  if (lhs == nullptr || rhs == nullptr)
    throw Error("IsMember was not updated! \t[dev-error]",loc);
  And::validate();
}

std::string IsMember::to_string() const {
  return "IsMember(" + designator->to_string() + ", " + type->to_string() + ')';
}

Ptr<Or> IsMember::make_legacy() const {
  validate_types();
  if (const auto _u = dynamic_cast<const ScalarsetUnion*>(designator.type().get())) {
    auto _or = Ptr<Or>::make(nullptr,nullptr,loc);
    r = make_ranges(_u,*type);
    // build the Exprs
    auto _build [&](SubRangeSet::_Range* r, Or& p_expr) -> void {
      if (r == nullptr) // safety case: def end of expr
        return;
      p_expr.lhs = Ptr<And>::make(
        Ptr<Leq>::make(Ptr<Number>::make(r->min), designator, loc),
        Ptr<Leq>::make(designator, Ptr<Number>::make(r->max), loc),
        loc
      );
      if (r->next == nullptr) { // base case: end of expr
        p_expr.rhs = rumur::False;
        return;
      } // else  // case: more ranges to insert into check
      p_expr.rhs = Ptr<Or>::make(nullptr,nullptr,loc);
      _build(r->next,_rhs);
    };
    if (ranges.range_head == nullptr)
      throw Error("IsMember use error : `" + type->to_string() + "` "
                  "has no intersection with the type of `"
                  + designator->to_string() +"` !", 
                  loc);
    _build(r.ranges.range_head, *_or);
    return _or;
  } // else 
    assert(false && "DEV ERROR : should be unreachable (work done in validate_types)");
}

// << ------------------------------------------------------------------------------------------ >> 

SUCast::SUCast(const Ptr<TypeExpr> &target_, const Ptr<TypeExpr> &cur_, const location &loc_)
  : Add(cur_,nullptr,loc_), target(target_) { update(); }
SUCast *SUCast::clone() const { return new SUCast(*this); }

void SUCast::visit(BaseTraversal& visitor) { visitor.visit_sucast(*this); } 
void SUCast::visit(ConstBaseTraversal& visitor) const { visitor.visit_sucast(*this); }

Ptr<TypeExpr> SUCast::type() const { return target; }
void SUCast::pre_validate() const {
  // one of the two types must be a scalarset union that contains the other type
  result = false;
  if (const auto _t1 = dynamic_cast<const ScalarsetUnion *>(target->resolve().get()))
    result |= _t1->contains(*t2);
  if (const auto _t2 = dynamic_cast<const ScalarsetUnion *>(lhs->type().get()))
    result |= _t2->contains(*t1);
  if (not result)
    throw Error("Can't cast/convert expr=`"
                + rhs->to_string() + "` --to-> type=`" + target->to_string() "`", loc);
}
void SUCast::validate() const {
  pre_validate();
  assert(rhs != nullptr && "DEV ERROR : did not create cast conversion!");
}

mpz_class get_conv_modifier_su(const Scalarset& from, const ScalarsetUnion& to) {
  return to.members[from.name].min;
}
mpz_class get_conv_modifier_us(const ScalarsetUnion& from, const Scalarset& to) {
  return -1_mpz * get_conv_modifier_su(to,from);
}
mpz_class get_conv_modifier_eu(const Enum& from, const ScalarsetUnion& to) {
  return to.members["_enum_"+from.members[0].first].min;
}
mpz_class get_conv_modifier_ue(const ScalarsetUnion& from, const Enum& to) {
  return -1_mpz * get_conv_modifier_eu(to,from);
}
mpz_class get_conv_modifier(const TypeExpr& from, const TypeExpr& to) {
  if (const auto to_u = dynamic_cast<const ScalarsetUnion*>(&to)) {
    if (const auto from_s = dynamic_cast<const Scalarset*>(&from)) {
      // thanks to the coercion check in pre_validate, 
      //  we can treat scalarsets and unions the same for the from parameter,
      //  when the to property is know to be a union type.
      return get_conv_modifier_su(*from_s, *to_u);
    } else if (const auto from_e = dynamic_cast<const Enum*>(&from)) {
      return get_conv_modifier_eu(*from_e, *to_u);
    } // else 
      assert(false && "DEV ERROR : SUCast coming from incompatible type AND not caught earlier! (1)");
  } else if (const auto to_s = dynamic_cast<const Scalarset*>(&to)) {
    if (const auto from_u = dynamic_cast<const ScalarsetUnion*>(&from)) {
      return get_conv_modifier_us(*from_u, *to_s);
    } // else
      assert(false && "DEV ERROR : SUCast coming from incompatible type AND not caught earlier! (2)")
  } else if (const auto to_e = dynamic_cast<const Enum*>(&to)) {
    if (const auto from_u = dynamic_cast<const ScalarsetUnion*>(&from)) {
      return get_conv_modifier_ue(*from_u, *to_e);
    } // else
      assert(false && "DEV ERROR : SUCast coming from incompatible type AND not caught earlier! (3)")
  }
}

void SUCast::update() {
  // NOt necessary since change to ExtNode and make_legacy design
  // pre_validate();
  // rhs = Ptr<Number>::make(get_conv_modifier(
  //                           *rhs->type(), 
  //                           *target->resolve()
  //                         ), loc);
}

std::string SUCast::to_string() const { 
  return "(/*cast*/ " + rhs->to_string() 
          + "/* --to-> `" target->to_string() "`*/)"; 
}

Ptr<Add> SUCast::make_legacy() const {
  return Ptr<Add>::make(rhs,
                        Ptr<Number>::make(get_conv_modifier(
                                              *rhs->type(), 
                                              *target->resolve()
                                          ), loc),
                        loc);
}


// << ------------------------------------------------------------------------------------------ >> 

MultisetElement::MultisetElement(const Ptr<Expr>& designator_, const Ptr<Expr> index_, const location& loc_)
  : Element(designator_,index_,loc_) {}
MultisetElement *MultisetElement::clone() const { return new MultisetElement(*this); }

void MultisetElement::visit(BaseTraversal& visitor) { visitor.visit_multisetelement(*this); }
void MultisetElement::visit(ConstBaseTraversal& visitor) const { visitor.visit_multisetelement(*this); }

void MultisetElement::validate() const {
  const Ptr<TypeExpr> t = array->type()->resolve();

  if (!isa<Multiset>(t))
    throw rumur::Error("multiset index on an expression that is not a multiset", loc);

  auto a = dynamic_cast<const Array &>(*t);

  if (!index->type()->coerces_to(*a.index_type))
    throw rumur::Error("multiset indexed using an expression of incorrect type", loc);
}
void MultisetElement::update() { 
  // do nothing but override the SUCast inserting behavior from element,
  //  since we should never allow any in-appropriate index types in the first place. 
}

// std::string MultisetElement::to_string() const {
//   return array->to_string() + "[" + index->to_string() + "]";
// }

Ptr<Element> MultisetElement::make_legacy() const { return Ptr<Element>(clone()); }


// << ------------------------------------------------------------------------------------------ >> 

MultisetQuantifier::MultisetQuantifier(const std::string& name_, const Ptr<Expr> multiset_, const location& loc_)
  : Quantifier(name_,nullptr,loc_), multiset(multiset_) {}
MultisetQuantifier *MultisetQuantifier::clone() const { new MultisetQuantifier(*this); }

void MultisetQuantifier::visit(BaseTraversal& visitor) { visitor.visit_multisetquantifier(*this); }
void MultisetQuantifier::visit(ConstBaseTraversal& visitor) const { visitor.visit_multisetquantifier(*this); }

void MultisetQuantifier::validate() const {
  if (not isa<Multiset>(multiset->type()))
    throw Error("expected a multiset", multiset->loc);
}
void MultisetQuantifier::update() {
  if (const auto ms = dynamic_cast<const Multiset*>(multiset->type())) {
    assert(ms->index_type != nullptr
            && "DEV ERROR : MultisetQuantifier's multiset has not been updated as expected yet");
    type = ms->index_type;
  }
}

std::string MultisetQuantifier::to_string() const {
  return name + ":" + multiset->to_string();
}

Ptr<Quantifier> MultisetQuantifier::make_legacy() const {
  assert(type != nullptr && "MultisetQuantifier was not ready to convert to Legacy!");
  return Ptr<Quantifier>(clone());
}


// << ------------------------------------------------------------------------------------------ >> 

MultisetCount::MultisetCount(const MultisetQuantifier& ms_quantifier_, const Ptr<Expr>& condition_, const location& loc_)
  : Add(nullptr, nullptr, loc_), 
    ms_quantifier(ms_quantifier_), condition(condition_) 
  {}
MultisetCount *MultisetCount::clone() const { new MultisetCount(*this); }

void MultisetCount::visit(BaseTraversal& visitor) { visitor.visit_multisetcount(*this); }
void MultisetCount::visit(ConstBaseTraversal& visitor) const { visitor.visit_multisetcount(*this); }

void MultisetCount::validate() const {
  if (not condition->is_boolean())
    throw Error("expresion does not evaluate to a boolean", pred->loc);
  if (not condition->is_pure())
    throw Error("expression is not pure", pred->loc);
}
void MultisetCount::update() {
  // nothing to do here
}
void MultisetCount::type() const {
  if (const auto ms = dynamic_cast<const Multiset*>(ms_quantifier.multiset->type().get()))
    return Ptr<Range>::make(Ptr<Number>::make(0_mpz,location()),
                            ms->size(), location());
  // else
    assert(false && "DEV ERROR : MultisetQuantifier did not refer to a multiset!");
}

std::string MultisetCount::to_string() const {
  return "MultisetCount(" + ms_quantifier.to_string() + ", " + condition->to_string() + ")";
}

Ptr<Add> MultisetCount::make_legacy() const {
  //TODO
}


} //namespace ext
} //namespace rumur
