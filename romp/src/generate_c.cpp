#include "generate_c.hpp"
// #include "CLikeGenerator.hpp"
#include "CTypeGenerator.hpp"
#include "ModelSplitter.hpp"
#include "options.h"
#include "resources.h"
#include <cstddef>
#include <iostream>
#include <rumur/rumur.h>
#include <string>
#include <vector>
// #include <streambuf>
#include <strstream>
#include <fstream>

#include "romp_def.hpp"

using namespace rumur;

namespace romp {

class CGenerator : public CLikeGenerator {

private:
  std::vector<std::string> state_vars;
  std::vector<const SimpleRule&> rules;
  std::vector<const PropertyRule&> invariants;
  std::vector<const StartState&> startstates;

public:
  CGenerator(const std::vector<rumur::Comment> &comments_, std::ostream &out_,
             bool pack_)
      : CLikeGenerator(comments_, out_, pack_) {}

  void visit_constdecl(const ConstDecl &n) final {
    *this << indentation() << "const ";

    // if this constant has an explicit type, use that
    if (n.type != nullptr) {
      *this << *n.type;

    } else {

      // otherwise, if it was a previously typedefed enum, use its typedefed
      // name (to avoid later -Wsign-compare warnings on GCC)
      const Ptr<TypeExpr> type = n.value->type();
      auto it = enum_typedefs.find(type->unique_id);
      if (it != enum_typedefs.end()) {
        *this << it->second;

      } else { // fallback on the type of the right hand side
        *this << "__typeof__(" << *n.value << ")";
      }
    }
    *this << " " << n.name << " = " << *n.value << ";";
    emit_trailing_comments(n);
    *this << "\n";
  }

  void visit_function(const Function &n) final {
    *this << indentation() << CodeGenerator::M_FUNCTION__FUNC_ATTRS << "\n"
          << indentation();
    if (n.return_type == nullptr) {
      *this << "void";
    } else {
      *this << *n.return_type;
    }
    *this << " " << n.name << "(";
    if (n.parameters.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Ptr<VarDecl> &p : n.parameters) {
        *this << sep << *p->type << " ";
        // if this is a var parameter, it needs to be a pointer
        if (!p->readonly) {
          (void)is_pointer.insert(p->unique_id);
          *this << "*";
        }
        *this << p->name;
        sep = ", ";
      }
    }
    *this << ") {\n";
    indent();
    for (const Ptr<Decl> &d : n.decls) {
      emit_leading_comments(*d);
      *this << *d;
    }
    for (const Ptr<Stmt> &s : n.body) {
      emit_leading_comments(*s);
      *this << *s;
    }
    dedent();
    *this << "}\n";
  }

  void visit_propertyrule(const PropertyRule &n) final {
    invariants.push_back(n);
    // function prototype
    *this << indentation() << CodeGenerator::M_INVARIANT__FUNC_ATTRS << "\n"
          << indentation() << "bool invariant__" << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        *this << sep;
        if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
          *this << t->name;
        } else {
          *this << value_type;
        }
        *this << " " << q.name;
        sep = ", ";
      }
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

  void visit_simplerule(const SimpleRule &n) final {
    rules.push_back(n);

    *this << indentation() << CodeGenerator::M_RULE_GUARD__FUNC_ATTRS << "\n"
          << indentation() << "bool rule__" << n.name << "__guard(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        *this << sep;
        if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
          *this << t->name;
        } else {
          *this << value_type;
        }
        *this << " " << q.name;
        sep = ", ";
      }
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

    *this << indentation() << CodeGenerator::M_RULE_ACTION__FUNC_ATTRS << "\n"
          << indentation() << "void rule__" << n.name << "__action(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        *this << sep;
        if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
          *this << t->name;
        } else {
          *this << value_type;
        }
        *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") {\n";
    indent();

    // aliases, variables, local types, etc.
    for (const Ptr<AliasDecl> &a : n.aliases) {
      emit_leading_comments(*a);
      *this << *a;
    }
    for (const Ptr<Decl> &d : n.decls) {
      emit_leading_comments(*d);
      *this << *d;
    }

    for (const Ptr<Stmt> &s : n.body) {
      emit_leading_comments(*s);
      *this << *s;
    }

    // clean up any aliases we defined
    for (const Ptr<Decl> &d : n.decls) {
      if (auto a = dynamic_cast<const AliasDecl *>(d.get())) {
        *this << "#undef " << a->name << "\n";
      }
    }
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    leave_scope();
    *this << indentation() << "}\n";
    // scope_level--;
  }

  void visit_startstate(const StartState &n) final {
    startstates.push_back(n);
    
    *this << indentation() << CodeGenerator::M_STARTSTATE__FUNC_ATTRS 
          << " void startstate__" << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        *this << sep;
        if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
          *this << t->name;
        } else {
          *this << value_type;
        }
        *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") {\n";
    indent();

    // aliases, variables, local types, etc.
    for (const Ptr<AliasDecl> &a : n.aliases) {
      emit_leading_comments(*a);
      *this << *a;
    }
    for (const Ptr<Decl> &d : n.decls) {
      emit_leading_comments(*d);
      *this << *d;
    }

    for (const Ptr<Stmt> &s : n.body) {
      emit_leading_comments(*s);
      *this << *s;
    }

    // clean up any aliases we defined
    for (const Ptr<Decl> &d : n.decls) {
      if (auto a = dynamic_cast<const AliasDecl *>(d.get())) {
        *this << "#undef " << a->name << "\n";
      }
    }
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << indentation() << "}\n\n";
  }

  void visit_vardecl(const VarDecl &n) final {
      *this << indentation() << *n.type << " " << n.name << ";";
      emit_trailing_comments(n);
      *this << "\n";
  }


  void gen_state_to_json(const Model &n) {
    *this << "\n" << indentation() << "NLOHMANN_DEFINE_TYPE_INTRUSIVE("
          ROMP_STATE_CLASS_NAME ", ";
    for (auto c : n.children) 
      if (auto v = dynamic_cast<const VarDecl *>(c.get()))
        *this << v->name << ",";
    *this << ")\n";
  }

  void gen_ruleset_array() {
    for (const SimpleRule& rule : rules) {

    }
  }


  void gen_invariant_array() {
    for (const PropertyRule& invar : invariants) {

    }
  }


  void gen_startstate_array() {
    for (const StartState& start : startstates) {

    }
  }

  void dispatch(const Node &n) {
    ModelSplitter splitter;
    splitter.dispatch(n);
    SplitModel split = splitter.get_split_model();

    *this << "\n" << indentation() << "namespace " ROMP_MODEL_NAMESPACE_NAME " {\n";
    indent();


    *this << "\n\n" << indentation() << "/* ======= Model Type Definitions ====== */\n\n";
    CTypeGenerator type_gen(comments, out, pack, 
      [&](const ConstDecl &_n) -> void {visit_constdecl(_n);});
    type_gen.dispatch(split.global_decls);

    *this << "\n" << indentation() << "class " ROMP_STATE_CLASS_NAME " {\n";
    indent();

    *this << "\n\n" << indentation() << "/* ======= Model State Variables ====== */\n\n";
    split.state_var_decl.visit(*this);
    gen_state_to_json(split.state_var_decl);

    *this << "\n\n" << indentation() << "/* ======= Murphi Model Functions ====== */\n\n";
    split.funct_decls.visit(*this);

    *this << "\n\n" << indentation() << "/* ======= Murphi Model Rules (& Invariants) ====== */\n\n";
    split.rule_decls.visit(*this);

    dedent();
    *this << "\n" << indentation() << "};\n";

    dedent();
    *this << "\n" << indentation() << "}\n\n";

    *this << "\n" << indentation() << "namespace " ROMP_CALLER_NAMESPACE_NAME " {\n";
    indent();

    *this << "\n\n" << indentation() << "/* ======= Romp Callable Lists ====== */\n\n";
    gen_ruleset_array();
    gen_invariant_array();
    gen_startstate_array();

    dedent();
    *this << "\n" << indentation() << "}\n\n";
  }

  virtual ~CGenerator() = default;
};

} // namespace

void generate_c(const Node &n, const std::vector<Comment> &comments, bool pack,
                std::ostream &out) {

  // write the static prefix to the beginning of the source file
  for (size_t i = 0; i < resources_c_prefix_c_len; i++)
    out << (char)resources_c_prefix_c[i];


  romp::CGenerator gen(comments, out, pack);
  gen.dispatch(n);



  // out << buffer.rdbuf() << "\n\n";
}
