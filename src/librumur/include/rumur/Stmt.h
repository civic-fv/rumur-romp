#pragma once

#include <iostream>
#include "location.hh"
#include <rumur/Expr.h>
#include <rumur/Node.h>
#include <string>
#include <vector>

namespace rumur {

class Stmt : public Node {

 public:
  using Node::Node;

  Stmt() = delete;
  Stmt(const Stmt&) = default;
  Stmt(Stmt&&) = default;
  Stmt &operator=(const Stmt&) = default;
  Stmt &operator=(Stmt&&) = default;
  virtual ~Stmt() { }
  virtual Stmt *clone() const = 0;
  virtual void generate(std::ostream &out) const = 0;

};

static inline std::ostream &operator<<(std::ostream &out, const Stmt &s) {
  s.generate(out);
  return out;
}

class Assert : public Stmt {

 public:
  Expr *expr;
  std::string message;

  Assert() = delete;
  Assert(Expr *expr_, const std::string &message_, const location &loc_);
  Assert(const Assert &other);
  Assert &operator=(Assert other);
  friend void swap(Assert &x, Assert &y) noexcept;
  Assert *clone() const final;
  virtual ~Assert();

  void generate(std::ostream &out) const final;
  bool operator==(const Node &other) const final;
};

class Assignment : public Stmt {

 public:
  Lvalue *lhs;
  Expr *rhs;

  Assignment() = delete;
  Assignment(Lvalue *lhs_, Expr *rhs_, const location &loc_);
  Assignment(const Assignment &other);
  Assignment &operator=(Assignment other);
  friend void swap(Assignment &x, Assignment &y) noexcept;
  Assignment *clone() const final;
  virtual ~Assignment();

  void generate(std::ostream &out) const final;
  bool operator==(const Node &other) const final;
};

class ErrorStmt : public Stmt {

  public:
   std::string message;

   ErrorStmt() = delete;
   ErrorStmt(const std::string &message_, const location &loc_);
   ErrorStmt(const ErrorStmt &other);
   ErrorStmt &operator=(ErrorStmt other);
   friend void swap(ErrorStmt &x, ErrorStmt &y) noexcept;
   ErrorStmt *clone() const final;
   virtual ~ErrorStmt() { }

   void generate(std::ostream &out) const final;
   bool operator==(const Node &other) const final;
};

class For : public Stmt {

 public:
  Quantifier *quantifier;
  std::vector<Stmt*> body;

  For() = delete;
  For(Quantifier *quantifier_, std::vector<Stmt*> &&body_, const location &loc_);
  For(const For &other);
  For &operator=(For other);
  friend void swap(For &x, For &y) noexcept;
  virtual ~For();
  For *clone() const final;

  void generate(std::ostream &out) const final;
  bool operator==(const Node &other) const final;
};

class IfClause : public Node {

 public:
  Expr *condition;
  std::vector<Stmt*> body;

  IfClause() = delete;
  IfClause(Expr *condition_, std::vector<Stmt*> &&body_, const location &loc_);
  IfClause(const IfClause &other);
  IfClause &operator=(IfClause other);
  friend void swap(IfClause &x, IfClause &y) noexcept;
  virtual ~IfClause();
  IfClause *clone() const final;

  bool operator==(const Node &other) const final;
};

class If : public Stmt {

 public:
  std::vector<IfClause> clauses;

  If() = delete;
  If(std::vector<IfClause> &&clauses_, const location &loc_);
  If(const If &other);
  If &operator=(If other);
  friend void swap(If &x, If &y) noexcept;
  virtual ~If() { };
  If *clone() const final;

  void generate(std::ostream &out) const final;
  bool operator==(const Node &other) const final;
};

}
