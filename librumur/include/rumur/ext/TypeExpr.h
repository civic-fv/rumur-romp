#pragma once

#include <rumur/TypeExpr.h>
#include <unordered_map>

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


struct RUMUR_API_WITH_RTTI ScalarsetUnion : public Scalarset {

  /// map of the NAME of a scalarset or single Enum value in this union
  /// NOTE: this only has a value after symbol-resolution and finalization
  std::unordered_map<std::string,ScalarsetUnionMember> members;
  // the list of types to union
  std::vector<Ptr<TypeExpr>> decl_list;

  ScalarsetUnion(const std::vector<Ptr<TypeExpr>>& decl_list_, const location& loc_);
  ScalarsetUnion *clone() const;
  virtual ~ScalarsetUnion() = default;

  void visit(BaseTraversal& visitor);
  void visit(ConstBaseTraversal& visitor) const;

  void update() override;
  void validate() const override;
  bool is_useful() const override;
  bool contains(const TypeExpr &other) const;
  std::string to_string() const override;
};


struct RUMUR_API_WITH_RTTI Multiset : public Array {

  Ptr<Expr>& size() const;

  Multiset(const Ptr<Expr>& size_, const Ptr<TypeExpr>& element_type_, const location& loc_);
  Multiset *clone() const;
  ~Multiset() = default;

  void visit(BaseTraversal& visitor);
  void visit(ConstBaseTraversal& visitor) const;

  void validate() const;
  std::string to_string() const;  
};

} //namespace ext
} //namespace rumur
