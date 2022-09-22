#pragma once

#include <rumur/ext/IExtTraverse.h>
#include <rumur/traverse.h>
#include <rumur/ext/Expr.h>
#include <rumur/ext/Function.h>
#include <rumur/ext/TypeExpr.h>

#ifndef RUMUR_API_WITH_RTTI
#define RUMUR_API_WITH_RTTI __attribute__((visibility("default")))
#endif

namespace rumur {
namespace ext {

class RUMUR_API_WITH_RTTI ExtTraversal : public Traversal {
public:
  void visit_ismember(IsMember& n) override;
  void visit_multiset(Multiset& n) override;
  void visit_scalarsetunion(ScalarsetUnion& n) override;
  void void visit_sucast(SUCast& n) override;

};

class RUMUR_API_WITH_RTTI ConstExtTraversal : public ConstBaseTraversal {
public:
  void visit_ismember(const IsMember& n) override;
  void visit_multiset(const Multiset& n) override;
  void visit_scalarsetunion(const ScalarsetUnion& n) override;
  void visit_sucast(const SUCast& n) override;

};

class RUMUR_API_WITH_RTTI ConstExtExprTraversal : public ConstExprTraversal {
public:
  void visit_ismember(const IsMember& n) override; // override me in your implementation
  void visit_multiset(const Multiset& n) final;
  void visit_scalarsetunion(const ScalarsetUnion& n) final;
  void visit_sucast(const SUCast& n) override;  // (OPTIONAL) override me in your implementation

};

class RUMUR_API_WITH_RTTI ConstExtStmtTraversal : public ConstStmtTraversal {
public:
  void visit_ismember(const IsMember& n) final;
  void visit_multiset(const Multiset& n) final;
  void visit_scalarsetunion(const ScalarsetUnion& n) final;
  void visit_sucast(const SUCast& n) final;

};

class RUMUR_API_WITH_RTTI ConstExtTypeTraversal : public ConstTypeTraversal {
public:
  void visit_ismember(const IsMember& n) final;
  void visit_multiset(const Multiset& n) override;  // override me in your implementation
  void visit_scalarsetunion(const ScalarsetUnion& n) override;  // override me in your implementation
  void visit_sucast(const SUCast& n) final;

};

} // namespace ext
} // namespace rumur
