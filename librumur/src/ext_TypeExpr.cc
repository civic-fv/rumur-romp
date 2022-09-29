#include "../../common/isa.h"
#include <cassert>
#include <cstddef>
#include <gmpxx.h>
#include <iostream>
#include <limits.h>
#include <memory>
#include <rumur/Decl.h>
#include <rumur/Expr.h>
#include <rumur/Number.h>
#include <rumur/Ptr.h>
#include <rumur/ext/TypeExpr.h>
#include <rumur/except.h>
#include <rumur/ext/traverse.h>
#include <string>
#include <utility>
#include <vector>
#include <sstream>

namespace rumur {
namespace ext {


ScalarsetUnion::ScalarsetUnion(const std::vector<Ptr<TypeExpr>>& decl_list_, const location &loc_) 
  : Scalarset(Ptr<TypeExprID>::make("_union_none_",nullptr,loc), loc), 
    decl_list(decl_list_), _useful(false) {}
ScalarsetUnion* ScalarsetUnion::clone() const { return new ScalarsetUnion(members,loc); }

void ScalarsetUnion::visit(BaseTraversal &visitor) { visitor.visit_scalarsetunion(*this); }
void ScalarsetUnion::visit(ConstBaseTraversal &visitor) const { visitor.visit_scalarsetunion(*this); }

void ScalarsetUnion::validate() const { 
  for (const auto _m : decl_list) {
    const auto m = _m.value->resolve();
    if (isa<Enum>(m)) {
      if (m->count() <= 0)
        throw Error("ScalarsetUnions require included enums to define at least 1 member!",_m->loc);
      continue;
    } else if (isa<TypeExprID>(_m.value) && isa<Scalarset>(m)) {
      continue;
    }
    throw Error("ScalarsetUnion can only union: enums, and named Scalarsets.", _m.value->loc);
  }
}
bool ScalarsetUnion::is_useful() const {
  bool useful = false;
  for (const auto m : decl_list)
    if (isa<Scalarset>(m.value)) {
      if (isa<ScalarsetUnion>(m.value))
        useful |= m.value->is_useful();
    }
  return (useful && count() > 1_mpz); // @Smattr you might want to tweak this comparison, too
}
void ScalarsetUnion::update() {
  //TODO clean this function up
  mpz_class count;
  auto try_insert [&](name,value) -> bool {
    if (members.find(name) != members.end()) 
      return false;
    members[name] = value;
    return true;
  };
  auto _union_enum [&](Enum& e, mpz_class _sum=0_mpz) -> mpz_class {
    for (auto m : e.members)
        if (try_insert("_enum_"+m.first,ScalarsetUnionMember(_m,_sum,_sum)))
          _sum++;
    return sum;
  };
  auto _union [&](ScalarsetUnion& ssu, mpz_class sum=0_mpz) -> mpz_class {
    for (auto _m : ssu.decl_list) {
      auto _res_t = _m->resolve();
      if (auto _e = dynamic_cast<Enum*>(_res_t.get()))
        _union_enum(*_e,sum);
      if (auto _ssu = dynamic_cast<ScalarsetUnion*>(_res_t.get()))
          _union(*_ssu,sum);  // recursively explore and add members of contained scalarset union with this one.
      if (auto _tid = dynamic_cast<TypeExprID*>(_m.get())) { 
        count = _m->count();
        if (try_insert(_tid->referent->name,ScalarsetUnionMember(_m,sum,sum+count-1_mpz)))
          sum += count;
        continue;
      }
      throw Error("union only operates on Enum and NAMED scalarset types!", _m->loc);
    }
  };
  members.clear();
  bound = Ptr<Number>::make(_union(*this),loc);
}

std::string ScalarsetUnion::to_string() const {
  std::stringstream buf;
  buf << "union {";
  std::string sep;
  for (const auto _m : members) {
    buf << sep << _m.value->to_string();
    sep = ", ";
  }
  buf << '}';
  return buf.str();
}

bool ScalarsetUnion::contains(const TypeExpr& n) const {
  struct Contain : public ConstExtTypeTraversal {
    bool result = false;
    const ScalarsetUnion& u;
    Contain(const ScalarsetUnion& u_) : u(u_) {} 
    void visit_array(const Array &n) final { result = false; }

    void visit_enum(const Enum &n) final {
      auto _ui = u.members.find("_enum_"+n.members[0].first);
      if (_ui == u.members.end() || not n.coerces_to(*_ui->second.value))
        // not a member of the union or comes from a different enum
        return; // result defaults to false
      result = true; // "enum set" is in union
    }

    void visit_multiset(const Multiset& n) { result = false; }
    void visit_range(const Range &n) final { result = false; }
    void visit_record(const Record &n) final { result = false; }

    void visit_scalarset(const Scalarset &n) final {
      if (n.name == "") // unnamed scalarsets can never be in a union
          return; // result defaults to false
      auto _ui = u.members.find(n.name);
      if (_ui == u.members.end())
        return; // result defaults to false
      // the scalarset in the union and `n` are the same if ...
      result = ((n.name == _ui->second.value->name) // names match
                && (n.bound->constant_fold()        // and bounds match
                    == _ui->second.value->bound->constant_fold()));
    }

    void visit_scalarsetunion(const ScalarsetUnion& n) {
      // a union `this` can only contain another scalarset union `n`...
      //  IF it meets the same conditions as a scalarset to be contained in this union
      //  AND `this` contains ALL of the children of `n`
      visit_scalarset(n);
      if (not result) return; // failed scalarset test
      for (const auto m : n.decl_list)
        result &= u.contains(*m);
    }

    void visit_typeexprid(const TypeExprID &n) final { dispatch(*n.referent); }
  };
  Contain c(*this);
  c.dispatch(n);
  return c.result;
}

Ptr<Scalarset> ScalarsetUnion::make_legacy() const {
  std::unordered_set<std::string> handled{name,""}; // ignore self and never add unnamed
  auto res = Ptr<Scalarset>(gen_legacy_bound(handled),loc);
  res->name = name;
  return res;
}

Ptr<Add> ScalarsetUnion::gen_legacy_bound(std::unordered_set<std::string>& handled) const {
  auto _extend = [&](Ptr<Add>& add, int i=0) -> void {
    if (i<decl_list.size()) { // base case end of assets
      // add->rhs = Ptr<Number>::make(0_mpz,location());
      return;
    }
    Ptr<Expr> _bound = nullptr; 
    if (const auto _u = dynamic_cast<const ScalarSetUnion*>(decl_list[i])) {
      if (_u->name == "")
        throw Error("A union can't contain an unnamed union",_u->loc);
      if (handled.find(_u->name) != handled.end()) {
        handled.insert(_u->name);
        _bound = _u->gen_legacy_bound(handled);
      }
    } else if (const auto _e = dynamic_cast<const Enum*>(decl_list[i])) {
      mpz_class count = 0_mpz;
      for (const auto m : _e->members)
        if (handled.find("_enum_"+m.first) != handled.end()) {
          handled.insert("_enum_"+m.first);
          ++count;
      _bound = Ptr<Number>::make(count,_e->loc);
    } else if (const auto _s = dynamic_cast<const Scalarset*>(decl_list[i])) {
      if (_s->name == "")
        throw Error("A union can't contain an unnamed scalarset",_u->loc);
      handled.insert(_s->name);
      _bound = _s->bound;
    }
    add->rhs = Ptr<Add>::make(
                  _bound,
                  Ptr<Number>::make(0_mpz,location()),
                  loc);
    _extend(add->rhs,++i);
  };
  auto bound = Ptr<Add>::make(Ptr<Number>::make(0_mpz,location()),
                              Ptr<Number>::make(0_mpz,location()),
                              loc);
  _extend(bound);
  return bound;
}

// << ------------------------------------------------------------------------------------------ >> 

Multiset::Multiset(const Ptr<Expr> size_, const Ptr<TypeExpr> element_type_, const location loc_)
  : Array(nullptr, element_type_, loc), size(size_) {}

Multiset* Multiset::clone() const { return new Multiset(*this); }

void Multiset::visit(BaseTraversal& visitor) { visitor.visit_multiset(*this); }
void Multiset::visit(ConstBaseTraversal& visitor) const { visitor.visit_multiset(*this); }

void Multiset::update() {
  // nothing to do here (moved to make legacy)
}
void Multiset::validate() const {
  if (size->is_boolean())
    throw Error("Multiset size constraint must be a number not a boolean!", size->loc);
  if (not size->constant())
    throw Error("Multiset size constraint must be constant and inferable at compile time!", size->loc);
}
std::string Multiset::to_string() const {
  return "multiset [" + size->to_string() + "] of " + element_type->to_string();
}

Ptr<Array> Multiset::make_legacy() const { 
  return Ptr<Array>(Ptr<Range>::make(Ptr<Number>::make(1_mpz,location()), size, size->loc),
                    element_type, loc);
}


} //namespace ext
} //namespace rumur
