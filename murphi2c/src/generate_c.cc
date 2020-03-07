#include <cassert>
#include <cstddef>
#include "CLikeGenerator.h"
#include "../../common/escape.h"
#include "generate_c.h"
#include <gmpxx.h>
#include <iostream>
#include "resources.h"
#include <rumur/rumur.h>
#include <string>
#include <utility>
#include <vector>

using namespace rumur;

namespace {

class CGenerator : public CLikeGenerator {

 public:
  CGenerator(std::ostream &out_, bool pack_): CLikeGenerator(out_, pack_) { }

  void visit_aliasdecl(const AliasDecl &n) final {
    *this << "#define " << n.name << " " << *n.value << "\n";
  }

  void visit_aliasstmt(const AliasStmt &n) final {
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << *a;
    }
    for (const Ptr<Stmt> &s : n.body) {
      *this << *s;
    }
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }
  }

  void visit_array(const Array &n) final {
    mpz_class count = n.index_type->count();

    assert(count > 0 && "index type of array does not include undefined");
    count--;

    // wrap the array in a struct so that we do not have the awkwardness of
    // having to emit its type and size on either size of another node
    *this << "struct " << (pack ? "__attribute__((packed)) " : "") << "{ "
      << *n.element_type << " data[" << count.get_str() << "];";

    // The index for this array may be an enum declared inline:
    //
    //   array [enum {A, B}] of foo
    //
    // If so, we need to emit it somehow so that the enum’s members can be
    // referenced later. We define it within this struct to avoid any awkward
    // lexical issues.
    if (auto e = dynamic_cast<const Enum*>(n.index_type.get())) {
      *this << " " << *e << ";";
    }

     *this <<" }";
  }

  void visit_assignment(const Assignment &n) final {
    *this << indentation() << *n.lhs << " = " << *n.rhs << ";\n";
  }

  void visit_clear(const Clear &n) final {
    *this << indentation() << "memset(&" << *n.rhs << ", 0, sizeof(" << *n.rhs
      << "));\n";
  }

  void visit_constdecl(const ConstDecl &n) final {
    *this << indentation() << "const ";
    if (n.type == nullptr) {
      *this << "int64_t";
    } else {
      *this << *n.type;
    }
    *this << " " << n.name << " = " << *n.value << ";\n";
  }

  void visit_enum(const Enum &n) final {
    *this << "enum { ";
    for (const std::pair<std::string, location> &m : n.members) {
      *this << m.first << ", ";
    }
    *this << "}";
  }

  void visit_errorstmt(const ErrorStmt &n) final {
    *this << indentation() << "error(\"" << escape(n.message) << "\");\n";
  }

  void visit_for(const For &n) final {

    // open a scope to make all of this appear as a single statement to any
    // enclosing code
    *this << indentation() << "do {\n";
    indent();

    // if the type of the quantifier is an enum defined inline, we need to
    // define this in advance because C does not permit this to be defined
    // within the for loop initialiser
    if (auto e = dynamic_cast<const Enum*>(n.quantifier.type.get())) {
      *this << indentation() << *e << ";\n";
    }

    *this << indentation() << n.quantifier << " {\n";
    indent();
    for (const Ptr<Stmt> &s : n.body) {
      *this << *s;
    }
    dedent();
    *this << indentation() << "}\n";

    dedent();
    *this << indentation() << "} while (0);\n";
  }

  void visit_function(const Function &n) final {
    *this << indentation();
    if (n.return_type == nullptr) {
      *this << "void";
    } else {
      *this << *n.return_type;
    }
    *this << " " << n.name << "(";
    bool first = true;
    for (const Ptr<VarDecl> &p : n.parameters) {
      if (!first) {
        *this << ", ";
      }
      *this << *p->type << " ";
      // if this is a var parameter, it needs to be a pointer
      if (!p->readonly) {
        *this << "*" << p->name << "_";
      } else {
        *this << p->name;
      }
      first = false;
    }
    *this << ") {\n";
    indent();
    // provide aliases of var parameters under their original name
    for (const Ptr<VarDecl> &p : n.parameters) {
      if (!p->readonly) {
        *this << "#define " << p->name << " (*" << p->name << "_)\n";
      }
    }
    for (const Ptr<Decl> &d : n.decls) {
      *this << *d;
    }
    for (const Ptr<Stmt> &s : n.body) {
      *this << *s;
    }
    // clean up var aliases
    for (const Ptr<VarDecl> &p : n.parameters) {
      if (!p->readonly) {
        *this << "#undef " << p->name << "\n";
      }
    }
    dedent();
    *this << "}\n";
  }

  void visit_if(const If &n) final {
    bool first = true;
    for (const IfClause &c : n.clauses) {
      if (first) {
        *this << indentation();
      } else {
        *this << " else ";
      }
      dispatch(c);
      first = false;
    }
    *this << "\n";
  }

  void visit_ifclause(const IfClause &n) final {
    if (n.condition != nullptr) {
      *this << "if " << *n.condition << " ";
    }
    *this << "{\n";
    indent();
    for (const Ptr<Stmt> &s : n.body) {
      *this << *s;
    }
    dedent();
    *this << indentation() << "}";
  }

  void visit_model(const Model &n) final {

    // constants, types and variables
    for (const Ptr<Decl> &d : n.decls) {
      *this << *d;
    }

    *this << "\n";

    // functions and procedures
    for (const Ptr<Function> &f : n.functions) {
      *this << *f << "\n";
    }

    // flatten the rules so we do not have to deal with the hierarchy of
    // rulesets, aliasrules, etc.
    std::vector<Ptr<Rule>> flattened;
    for (const Ptr<Rule> &r : n.rules) {
      std::vector<Ptr<Rule>> rs = r->flatten();
      flattened.insert(flattened.end(), rs.begin(), rs.end());
    }

    // startstates, rules, invariants
    for (const Ptr<Rule> &r : flattened) {
      *this << *r << "\n";
    }
  }

  void visit_propertyrule(const PropertyRule &n) final {

    // function prototype
    *this << indentation() << "bool " << n.name << "(";

    // parameters
    bool first = true;
    for (const Quantifier &q : n.quantifiers) {
      if (!first) {
        *this << ", ";
      }
      if (auto t = dynamic_cast<const TypeExprID*>(q.type.get())) {
        *this << t->name;
      } else {
        *this << "int64_t";
      }
      *this << " " << q.name;
      first = false;
    }

    *this << ") {\n";
    indent();

    // any aliases this property uses
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << *a;
    }

    *this << indentation() << "return " << *n.property.expr << ";\n";

    // clean up any aliases we defined
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << "}\n";
  }

  void visit_propertystmt(const PropertyStmt &n) final {

    switch (n.property.category) {

      case Property::ASSERTION:
        *this << indentation() << "if (!" << *n.property.expr << ") {\n";
        indent();
        *this << indentation() << "failed_assertion(\""
          << escape(n.message == "" ? n.property.expr->to_string() : n.message)
          << "\");\n";
        dedent();
        *this << indentation() << "}\n";
        break;

      case Property::ASSUMPTION:
        *this << indentation() << "if (!" << *n.property.expr << ") {\n";
        indent();
        *this << indentation() << "failed_assumption(\""
          << escape(n.message == "" ? n.property.expr->to_string() : n.message)
          << "\");\n";
        dedent();
        *this << indentation() << "}\n";
        break;

      case Property::COVER:
        *this << indentation() << "if " << *n.property.expr << " {\n";
        indent();
        *this << indentation() << "cover(\""
          << escape(n.message == "" ? n.property.expr->to_string() : n.message)
          << "\");\n";
        dedent();
        *this << indentation() << "}\n";
        break;

      case Property::LIVENESS:
        *this << indentation() << "if " << *n.property.expr << " {\n";
        indent();
        *this << indentation() << "liveness(\""
          << escape(n.message == "" ? n.property.expr->to_string() : n.message)
          << "\");\n";
        dedent();
        *this << indentation() << "}\n";
        break;

    }
  }

  void visit_put(const Put &n) final {
    *this << indentation() << "printf(";
    if (n.expr == nullptr) {
      *this << "\"%s\\n\", \"" << escape(n.value) << "\")";
    } else {
      *this << "\"%\" PRId64 \"\\n\", " << *n.expr << ")";
    }
    *this << ";\n";
  }

  void visit_quantifier(const Quantifier &n) final {

    if (n.type == nullptr) {
      bool down_count = n.from->constant() && n.to->constant()
        && n.to->constant_fold() < n.from->constant_fold();
      *this << "for (int64_t " << n.name << " = " << *n.from << "; " << n.name
        << " " << (down_count ? ">=" : "<=") << " " << *n.to << "; " << n.name
        << " += ";
      if (n.step == nullptr) {
        *this << "1";
      } else {
        *this << *n.step;
      }
      *this << ")";
      return;
    }

    const Ptr<TypeExpr> resolved = n.type->resolve();

    if (auto e = dynamic_cast<const Enum*>(resolved.get())) {
      if (e->members.empty()) {
        // degenerate loop
        *this << "for (int " << n.name << " = 0; " << n.name << " < 0; "
          << n.name << "++)";
      } else {
        // common case
        *this << "for (__typeof__(" << e->members[0].first << ") " << n.name
          << " = " << e->members[0].first << "; " << n.name << " <= "
          << e->members[e->members.size() - 1].first << "; " << n.name << "++)";
      }
      return;
    }

    if (auto r = dynamic_cast<const Range*>(resolved.get())) {
      *this << "for (int64_t " << n.name << " = " << *r->min << "; " << n.name
        << " <= " << *r->max << "; " << n.name << "++)";
      return;
    }

    if (auto s = dynamic_cast<const Scalarset*>(resolved.get())) {
      *this << "for (int64_t " << n.name << " = 0; " << n.name << " <= "
        << *s->bound << "; " << n.name << "++)";
      return;
    }

    assert(!"missing case in visit_quantifier()");
  }

  void visit_range(const Range&) final {
    *this << "int64_t";
  }

  void visit_record(const Record &n) final {
    *this << "struct " << (pack ? "__attribute__((packed)) " : "") << "{\n";
    indent();
    for (const Ptr<VarDecl> &f : n.fields) {
      *this << *f;
    }
    dedent();
    *this << indentation() << "}";
  }

  void visit_return(const Return &n) final {
    *this << indentation() << "return";
    if (n.expr != nullptr) {
      *this << " " << *n.expr;
    }
    *this << ";\n";
  }

  void visit_ruleset(const Ruleset&) final {
    // this is unreachable because generate_c is only ever called with a Model
    // and all rule are flattened during visit_model
    assert(!"unreachable");
    __builtin_unreachable();
  }

  void visit_scalarset(const Scalarset&) final {
    *this << "int64_t";
  }

  void visit_simplerule(const SimpleRule &n) final {
    *this << indentation() << "bool guard_" << n.name << "(";

    // parameters
    bool first = true;
    for (const Quantifier &q : n.quantifiers) {
      if (!first) {
        *this << ", ";
      }
      if (auto t = dynamic_cast<const TypeExprID*>(q.type.get())) {
        *this << t->name;
      } else {
        *this << "int64_t";
      }
      *this << " " << q.name;
      first = false;
    }

    *this << ") {\n";
    indent();

    // any aliases that are defined in an outer scope
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << *a;
    }

    *this << indentation() << "return ";
    if (n.guard == nullptr) {
      *this << "true";
    } else {
      *this << *n.guard;
    }
    *this << ";\n";

    // clean up aliases
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << indentation() << "}\n\n";

    *this << indentation() << "void rule_" << n.name << "(";

    // parameters
    first = true;
    for (const Quantifier &q : n.quantifiers) {
      if (!first) {
        *this << ", ";
      }
      if (auto t = dynamic_cast<const TypeExprID*>(q.type.get())) {
        *this << t->name;
      } else {
        *this << "int64_t";
      }
      *this << " " << q.name;
      first = false;
    }

    *this << ") {\n";
    indent();

    // aliases, variables, local types, etc.
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << *a;
    }
    for (const Ptr<Decl> &d : n.decls) {
      *this << *d;
    }

    for (const Ptr<Stmt> &s : n.body) {
      *this << *s;
    }

    // clean up any aliases we defined
    for (const Ptr<Decl> &d : n.decls) {
      if (auto a = dynamic_cast<const AliasDecl*>(d.get())) {
        *this << "#undef " << a->name << "\n";
      }
    }
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << indentation() << "}\n";
  }

  void visit_startstate(const StartState &n) final {
    *this << indentation() << "void startstate_" << n.name << "(";

    // parameters
    bool first = true;
    for (const Quantifier &q : n.quantifiers) {
      if (!first) {
        *this << ", ";
      }
      if (auto t = dynamic_cast<const TypeExprID*>(q.type.get())) {
        *this << t->name;
      } else {
        *this << "int64_t";
      }
      *this << " " << q.name;
      first = false;
    }

    *this << ") {\n";
    indent();

    // aliases, variables, local types, etc.
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << *a;
    }
    for (const Ptr<Decl> &d : n.decls) {
      *this << *d;
    }

    for (const Ptr<Stmt> &s : n.body) {
      *this << *s;
    }

    // clean up any aliases we defined
    for (const Ptr<Decl> &d : n.decls) {
      if (auto a = dynamic_cast<const AliasDecl*>(d.get())) {
        *this << "#undef " << a->name << "\n";
      }
    }
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << indentation() << "}\n\n";
  }

  void visit_switch(const Switch &n) final {

    // Rumur permits switch statements with non-constant case expressions, while
    // C’s switch statements do not support this. To deal with this discrepancy,
    // we emit switch statements as more flexible if-then-else blocks instead.

    // make the variable declaration and if-then-else block appear as a single
    // statement to any enclosing code
    *this << indentation() << "do {\n";
    indent();

    // we need to declare a temporary for the expression here because it may not
    // be pure, so we cannot necessarily safely emit it repeatedly in the
    // if-then-else conditions
    *this << indentation() << "__typeof__(" << *n.expr << ") res_ = " << *n.expr
      << ";\n";

    bool first = true;
    for (const SwitchCase &c : n.cases) {
      if (first) {
        *this << indentation();
      } else {
        *this << " else ";
      }
      if (!c.matches.empty()) {
        std::string sep = "";
        *this << "if (";
        for (const Ptr<Expr> &m : c.matches) {
          *this << sep << "res_ == " << *m;
          sep = " || ";
        }
        *this << ") ";
      }
      *this << "{\n";
      indent();
      for (const Ptr<Stmt> &s : c.body) {
        *this << *s;
      }
      dedent();
      *this << indentation() << "}";
      first = false;
    }
    *this << "\n";

    dedent();
    *this << indentation() << "} while (0);\n";
  }

  void visit_switchcase(const SwitchCase &n) final {
    if (n.matches.empty()) {
      *this << indentation() << "default:\n";
    } else {
      for (const Ptr<Expr> &m : n.matches) {
        *this << indentation() << "case " << *m << ":\n";
      }
    }
    indent();
    for (const Ptr<Stmt> &s : n.body) {
      *this << *s;
    }
    *this << indentation() << "break;\n";
    dedent();
  }

  void visit_typedecl(const TypeDecl &n) final {
    *this << indentation() << "typedef " << *n.value << " " << n.name << ";\n";
  }

  void visit_typeexprid(const TypeExprID &n) final {
    *this << n.name;
  }

  void visit_vardecl(const VarDecl &n) final {
    *this << indentation() << *n.type << " " << n.name << ";\n";
  }

  void visit_while(const While &n) final {
    *this << indentation() << "while " << *n.condition << " {\n";
    indent();
    for (const Ptr<Stmt> &s : n.body) {
      *this << *s;
    }
    dedent();
    *this << indentation() << "}\n";
  }

  virtual ~CGenerator() = default;
};

}

void generate_c(const Node &n, bool pack, std::ostream &out) {

  // write the static prefix to the beginning of the source file
  for (size_t i = 0; i < resources_c_prefix_c_len; i++)
    out << (char)resources_c_prefix_c[i];

  CGenerator gen(out, pack);
  gen.dispatch(n);
}
