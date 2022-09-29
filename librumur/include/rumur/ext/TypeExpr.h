#pragma once

#include <rumur/ext/un-ext.h>
#include <rumur/TypeExpr.h>
#include <unordered_map>
#include <unordered_set>

#ifndef RUMUR_API_WITH_RTTI
#define RUMUR_API_WITH_RTTI __attribute__((visibility("default")))
#endif

namespace rumur {
namespace ext {

struct RUMUR_API_WITH_RTTI ScalarsetUnionMember {
  Ptr<TypeExpr> value;
  mpz_class min;
  mpz_class max;
};


struct RUMUR_API_WITH_RTTI ScalarsetUnion : public Scalarset, public IExtNode<Scalarset> {

  // the list of types to union
  std::vector<Ptr<TypeExpr>> decl_list;
  /// map of the NAME of a scalarset or single Enum value in this union
  /// NOTE: this only has a value after symbol-resolution and a call to `update()`
  /// WARNING: meant for implementing legacy code adaptations and the contains method ONLY!
  ///          do not use in your implementation unless you know what you are doing,
  ///          it gets overwritten every time `update()` is called.
  std::unordered_map<std::string,ScalarsetUnionMember> members;

  ScalarsetUnion(const std::vector<Ptr<TypeExpr>>& decl_list_, const location& loc_);
  ScalarsetUnion *clone() const;
  virtual ~ScalarsetUnion() = default;

  void visit(BaseTraversal& visitor);
  void visit(ConstBaseTraversal& visitor) const;

  void count() const override;

  void update() override;
  void validate() const override;
  bool is_useful() const override;
  bool contains(const TypeExpr &other) const;
  std::string to_string() const override;

  Ptr<Scalarset> make_legacy() const override;
private:
  Ptr<Add> gen_legacy_bound(std::unordered_set<std::string>& handled) const;
};


struct RUMUR_API_WITH_RTTI Multiset : public Array, public IExtNode<Array> {

  Ptr<Expr>& size;
  //NOTE: Multiset's by default have nullptr as their index_type, 
  //      make_legacy will return an Array that has a proper index_type
  // Ptr<TypeExpr>& element_type;

  Multiset(const Ptr<Expr>& size_, const Ptr<TypeExpr>& element_type_, const location& loc_);
  Multiset *clone() const;
  ~Multiset() = default;

  void visit(BaseTraversal& visitor);
  void visit(ConstBaseTraversal& visitor) const;

  void validate() const override;
  void update() override;
  std::string to_string() const;

  Ptr<Array> make_legacy() const override;
};

} //namespace ext
} //namespace rumur
