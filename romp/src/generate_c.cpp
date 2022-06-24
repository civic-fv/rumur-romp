#include "generate_c.hpp"
#include "CLikeGenerator.hpp"
#include "CTypeGenerator.hpp"
#include "ModelSplitter.hpp"
#include "options.h"
#include "resources.h"
#include <cstddef>
#include <iostream>
#include <rumur/rumur.h>
#include <string>
#include <vector>
#include <unordered_set>
// #include <streambuf>
#include <strstream>


#include "romp_def.hpp"

using namespace rumur;

namespace romp {

class CGenerator : public CLikeGenerator {

private:
  // std::vector<std::string> state_vars;
  std::vector<Ptr<const SimpleRule>> rules;
  std::vector<Ptr<const PropertyRule>> invariants;
  std::vector<Ptr<const StartState>> startstates;

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
        *this << " ::"ROMP_TYPE_NAMESPACE"::" << it->second;

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
    *this << ") throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n";

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
    invariants.push_back(Ptr<const PropertyRule>(&n));
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
          *this << *t; //t->name;
        } else {
          *this << value_type;
        }
        *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") const throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n";

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
    *this << indentation() << "}\n";
  }

  void visit_simplerule(const SimpleRule &n) final {
    rules.push_back(Ptr<const SimpleRule>(&n));

    *this << indentation() << CodeGenerator::M_RULE_GUARD__FUNC_ATTRS << "\n"
          << indentation() << "bool rule_guard__" << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        *this << sep;
        if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
          *this << *t; //t->name;
        } else {
          *this << value_type;
        }
        *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") const throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n";

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
          << indentation() << "void rule_action__" << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        *this << sep;
        if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
          *this << *t; //t->name;
        } else {
          *this << value_type;
        }
        *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n";

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
    *this << indentation() << "}\n";
  }

  void visit_startstate(const StartState &n) final {
    startstates.push_back(Ptr<const StartState>(&n));
    
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
          *this << *t; //t->name;
        } else {
          *this << value_type;
        }
        *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n"; 

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


  void gen_state_to_json(std::vector<Ptr<VarDecl>> &children) {
    *this << "\n" << indentation() << "NLOHMANN_DEFINE_TYPE_INTRUSIVE("
          ROMP_STATE_CLASS_NAME;
    for (const Ptr<const VarDecl> &c : children) 
      if (auto v = dynamic_cast<const VarDecl *>(c.get()))
        *this << "," << v->name;
    *this << ")\n";
  }

  void gen_ruleset_array() {
    std::strstream ruleset_array;
    std::strstream ruleset_name_array;
    for (const Ptr<const SimpleRule>& rule : rules) {
      *this << indentation() << "/* --- Ruleset " << rule->name << " (generated) --- */\n\n";
      size_t start;
      size_t stop;
      size_t step;

    }
  }


  void gen_invariant_array() {
    for (const Ptr<const PropertyRule>& invar : invariants) {
      int tmp = 0;
    }
  }


  void gen_startstate_array() {
    for (const Ptr<const StartState>& start : startstates) {
      int tmp = 0;
    }
  }

  void visit_model(const Model &n) {
    ModelSplitter sorter;
    sorter.sort_model(n.children);
    // SplitModel split = splitter.split_model(n);

    *this << "\n\n" << indentation() << "/* ======= Model Type & Const Definitions ====== */\n";
    *this << "\n" << indentation() << "namespace " ROMP_TYPE_NAMESPACE " {\n";
    indent();

    CTypeGenerator type_gen(comments, out, pack, 
      [&](const ConstDecl &_n) -> void {visit_constdecl(_n);});
    for (const Ptr<const Decl> &decl : sorter.global_decls)
      type_gen << *decl;

    dedent();
    *this << "\n" << indentation() << "}\n\n"; // << std::flush;

    // update these to reflect the processes from the type emitter
    enum_typedefs = type_gen.enum_typedefs;
    emitted_tDecls = type_gen.emitted_tDecls;
    is_pointer = type_gen.is_pointer;
    // emitted = type_gen.emitted;

    *this << "\n\n" << indentation() << "/* ======= Generated Model & State ====== */\n";
    *this << "\n" << indentation() << "namespace " ROMP_MODEL_NAMESPACE_NAME " {\n";
    indent();

    *this << "\n" << indentation() << "class " ROMP_STATE_CLASS_NAME " {\n";
    indent();

    *this << "\n" << indentation() << "/* ======= Model State Variables ====== */\n\n";
    // split.state_var_decl.visit(*this);
    for (const Ptr<const VarDecl> &var : sorter.state_var_decls)
      *this << *var << "\n"; 
    gen_state_to_json(sorter.state_var_decls);

    *this << "\n" << indentation() << "/* ======= Murphi Model Functions ====== */\n\n"; // << std::flush;
    // split.funct_decls.visit(*this);
    for (const Ptr<const Function> &funct : sorter.funct_decls)
      *this << *funct << "\n";

    *this << "\n" << indentation() << "/* ======= Murphi Model Rules (& Invariants) ====== */\n\n"; // << std::flush;
    // split.rule_decls.visit(*this);
    for (const Ptr<const Rule> &_r : sorter.rule_decls) {
      std::vector<Ptr<Rule>> rs = _r->flatten();
      for (const Ptr<const Rule> &r2 : rs)
        *this << *r2 << "\n";
    }

    dedent();
    *this << "\n" << indentation() << "};\n"; // << std::flush;

    dedent();
    *this << "\n" << indentation() << "}\n\n";

    *this << "\n" << indentation() << "namespace " ROMP_CALLER_NAMESPACE_NAME " {\n";
    indent();

    *this << "\n\n" << indentation() << "/* ======= Romp Callable Lists ====== */\n\n";
    // gen_ruleset_array();
    // gen_invariant_array();
    // gen_startstate_array();

    dedent();
    *this << "\n" << indentation() << "}\n\n"; // << std::flush;
  }

  virtual ~CGenerator() = default;

  // CGenerator& operator << (std::ostream& os) {
  //   out << os;
  //   return *this;
  // }
};


} // namespace

void output_embedded_code_file(std::ostream& out, const unsigned char* ecf, const size_t ecf_l) {
  for (size_t i = 0; i < ecf_l; i++)
    out << (char) ecf[i];
}

void generate_c(const Node &n, const std::vector<Comment> &comments, bool pack,
                std::ostream &out) {

  out << ROMP_GENERATED_FILE_PREFACE("test text") "\n";
  out << "\n#define __romp__GENERATED_CODE\n\n";

  out << "\n#pragma region inline_library_includes\n\n";
  // write json library to the file
  // output_embedded_code_file(out, resources_lib_nlohmann_json_hpp, resources_lib_nlohmann_json_hpp_len);
  out << "\n#pragma endregion inline_library_includes\n\n" << std::flush;

  out << "\n#pragma region model_prefixes\n\n";
  // write the static prefix to the beginning of the source file
  output_embedded_code_file(out, resources_c_prefix_cpp, resources_c_prefix_cpp_len);
  out << "\n#pragma endregion model_prefixes\n\n" << std::flush;


  out << "\n\n#pragma region generated_code\n\n";
  romp::CGenerator gen(comments, out, pack);
  gen.dispatch(n);
  out << "\n\n#pragma endregion generated_code\n\n" << std::flush;

  out << "\n#pragma region romp_rw\n\n";
  output_embedded_code_file(out, resources_romp_rw_hpp, resources_romp_rw_hpp_len);
  out << "\n#pragma endregion romp_rw\n\n" << std::flush;
  out << "\n#pragma region romp_rw_options\n\n";
  output_embedded_code_file(out, resources_romp_rw_options_hpp, resources_romp_rw_options_hpp_len);
  out << "\n#pragma endregion romp_rw_options\n\n" << std::flush;
  out << "\n#pragma region romp_rw_main\n\n";
  output_embedded_code_file(out, resources_romp_rw_main_hpp, resources_romp_rw_main_hpp_len);
  out << "\n#pragma endregion romp_rw_main\n" << std::flush;


  // out << buffer.rdbuf() << "\n\n";
  // out << std::flush;
}
