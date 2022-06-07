/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file Model.hpp
 *
 * @brief   A Class that is a intermediary representation of the model,
 *        used to allow for generating the Cpp romp tools
 *        in a way that allows full context of the model.
 *
 * @date 2022/05/13
 * @version 0.1
 */

#include "TModel.hpp"

#include "../../../common/escape.h"
#include "../../../common/isa.h"
#include "../options.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <ctype.h>
#include <gmpxx.h>
#include <rumur/rumur.h>
// #include <rumur/except.h>
// #include <rumur/Comment.h>
#include <string>
#include <utility>
#include <vector>
// #include <functional>


namespace romp {
using namespace rumur;

// << ========================================================================================== >> 
// <<                               CONSTRUCTORS & DECONSTRUCTORS                                >> 
// << ========================================================================================== >> 

TModel::~TModel() {}



// << ========================================================================================== >> 
// <<                                 PRIVATE HELPER FUNCTIONS                                   >> 
// << ========================================================================================== >> 

const std::string TModel::gen_new_anon_name() {
  char buff[24];
  sprintf(buff, "anon0x%x_t", anon_id++);
  return std::string(buff);
}



// << ========================================================================================== >> 
// <<                                  PARSING UTIL FUNCTIONS                                    >> 
// << ========================================================================================== >> 


void TModel::visit_model(const Model &n) {
  _model = &n; // save the toplevel of the model for later.

  emit_leading_comments(n);

  for (const Ptr<Node> &c : n.children) {

    emit_leading_comments(*c);

    // if this is a rule, first flatten it so we do not have to deal with the
    // hierarchy of rulesets, aliasrules, etc.
    if (auto r = dynamic_cast<const Rule *>(c.get())) {
      std::vector<Ptr<Rule>> rs = r->flatten();
      for (const Ptr<Rule> &r2 : rs)
        *this << *r2 << "\n";

    } else {
      *this << *c << "\n";
    }
  }
}


void TModel::visit_constdecl(const ConstDecl &n) {
  // *this << indentation() << "const ";

  // // if this constant has an explicit type, use that
  // if (n.type != nullptr) {
  //   *this << *n.type;

  // } else {

  //   // otherwise, if it was a previously typedefed enum, use its typedefed
  //   // name (to avoid later -Wsign-compare warnings on GCC)
  //   const Ptr<TypeExpr> type = n.value->type();
  //   auto it = enum_typedefs.find(type->unique_id);
  //   if (it != enum_typedefs.end()) {
  //     *this << it->second;

  //   } else { // fallback on the type of the right hand side
  //     *this << "__typeof__(" << *n.value << ")";
  //   }
  // }
  // *this << " " << n.name << " = " << *n.value << ";";
  // emit_trailing_comments(n);
  // *this << "\n";
}


void TModel::visit_typedecl(const TypeDecl &n) {
  // If we are typedefing something that is an enum, save this for later lookup.
  // See CGenerator/HGenerator::visit_constdecl for the purpose of this.
  // if (auto e = dynamic_cast<const Enum *>(n.value.get()))
  //   enum_typedefs[e->unique_id] = n.name;

  // preprocess the type declaration and just typedef the anonymous type
  *this << indentation() << *n.value << "\n";

  *this << indentation() << "typedef " << cTypeName[n.value->unique_id] << " " << n.name << ";";
  emit_trailing_comments(n);
  *this << "\n"; 

  // remap the type to its defined name
  cTypeName[n.value->unique_id] = std::string(n.name);
  // set typedecl to refer to its typedecl
  cTypeName[n.unique_id] = std::string(n.name);
}


void TModel::visit_typeexprid(const TypeExprID &n) {
  if (typeIds.find(n.referent->unique_id) == typeIds.end())  // probs don't need until actually genning code.
    throw new rumur::Error("Undefined Type ID!! (type `" + n.name + "` not defined at this point)", n.loc);
  *this << cTypeName[n.referent->unique_id] << " ";
}




void TModel::visit_enum(const Enum &n) {
  *this << "typedef enum { ";
  for (const std::pair<std::string, location> &m : n.members) {
    *this << m.first << ", ";
  }
  const std::string anon_name = gen_new_anon_name();
  *this << "} " << anon_name << ";";
  cTypeName[n.unique_id] = anon_name;
}


void TModel::visit_array(const Array &n) {
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
  if (auto e = dynamic_cast<const Enum *>(n.index_type.get())) {
    *this << " " << *e << ";";
  }

  *this << " }";
}


void TModel::visit_range(const Range &) {
  *this << value_type;
}


void TModel::visit_record(const Record &n) {
  for (const Ptr<VarDecl> &f : n.fields) {  // pre output any inplace type definitions
    *this << *(f->get_type());
  }
  *this << "struct " << (pack ? "__attribute__((packed)) " : "") << "{\n";
  indent();
  for (const Ptr<VarDecl> &f : n.fields) {
    *this << indentation() << cTypeName[f->get_type()->unique_id] << " " << f->name << ";\n";
  }
  dedent();
  *this << indentation() << "}";
}


void TModel::visit_scalarset(const Scalarset &) { *this << value_type; }


void TModel::visit_vardecl(const VarDecl &n) {
  // *this << indentation() << *n.type << " " << n.name << ";";
  // emit_trailing_comments(n);
  // *this << "\n";
}


void TModel::visit_function(const Function &n) {
  // // scope_level++;
  // *this << indentation() << romp::CODEGEN_CONFIG::M_FUNCTION__FUNC_ATTRS << "\n"
  //       << indentation();
  // if (n.return_type == nullptr) {
  //   *this << "void";
  // } else {
  //   *this << *n.return_type;
  // }
  // *this << " " << n.name << "(";
  // if (n.parameters.empty()) {
  //   *this << "void";
  // } else {
  //   std::string sep;
  //   for (const Ptr<VarDecl> &p : n.parameters) {
  //     *this << sep << *p->type << " ";
  //     // if this is a var parameter, it needs to be a pointer
  //     if (!p->readonly) {
  //       (void)is_pointer.insert(p->unique_id);
  //       *this << "*";
  //     }
  //     *this << p->name;
  //     sep = ", ";
  //   }
  // }
  // *this << ") {\n";
  // enter_scope();
  // indent();
  // for (const Ptr<Decl> &d : n.decls) {
  //   emit_leading_comments(*d);
  //   *this << *d;
  // }
  // for (const Ptr<Stmt> &s : n.body) {
  //   emit_leading_comments(*s);
  //   *this << *s;
  // }
  // dedent();
  // leave_scope();
  // *this << "}\n";
  // // scope_level--;
}

void TModel::visit_propertyrule(const PropertyRule &n) {
  // // scope_level++;
  // // function prototype
  // *this << indentation() << romp::CODEGEN_CONFIG::M_RULE_GUARD__FUNC_ATTRS
  //       << "\n"
  //       << indentation() << "bool " << n.name << "(";

  // // parameters
  // if (n.quantifiers.empty()) {
  //   *this << "void";
  // } else {
  //   std::string sep;
  //   for (const Quantifier &q : n.quantifiers) {
  //     *this << sep;
  //     if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
  //       *this << t->name;
  //     } else {
  //       *this << value_type;
  //     }
  //     *this << " " << q.name;
  //     sep = ", ";
  //   }
  // }

  // *this << ") {\n";
  // enter_scope();
  // indent();

  // // any aliases this property uses
  // for (const Ptr<AliasDecl> &a : n.aliases) {
  //   *this << *a;
  // }

  // *this << indentation() << "return " << *n.property.expr << ";\n";

  // // clean up any aliases we defined
  // for (const Ptr<AliasDecl> &a : n.aliases) {
  //   *this << "#undef " << a->name << "\n";
  // }

  // dedent();
  // *this << "}\n";
  // leave_scope();
  // // scope_level--;
}

void TModel::visit_simplerule(const SimpleRule &n) {
  // // scope_level++;
  // *this << indentation() << "bool guard_" << n.name << "(";

  // // parameters
  // if (n.quantifiers.empty()) {
  //   *this << "void";
  // } else {
  //   std::string sep;
  //   for (const Quantifier &q : n.quantifiers) {
  //     *this << sep;
  //     if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
  //       *this << t->name;
  //     } else {
  //       *this << value_type;
  //     }
  //     *this << " " << q.name;
  //     sep = ", ";
  //   }
  // }

  // *this << ") {\n";
  // enter_scope();
  // indent();

  // // any aliases that are defined in an outer scope
  // for (const Ptr<AliasDecl> &a : n.aliases) {
  //   *this << *a;
  // }

  // *this << indentation() << "return ";
  // if (n.guard == nullptr) {
  //   *this << "true";
  // } else {
  //   *this << *n.guard;
  // }
  // *this << ";\n";

  // // clean up aliases
  // for (const Ptr<AliasDecl> &a : n.aliases) {
  //   *this << "#undef " << a->name << "\n";
  // }

  // dedent();
  // leave_scope();
  // *this << indentation() << "}\n\n";

  // *this << indentation() << "void rule_" << n.name << "(";

  // // parameters
  // if (n.quantifiers.empty()) {
  //   *this << "void";
  // } else {
  //   std::string sep;
  //   for (const Quantifier &q : n.quantifiers) {
  //     *this << sep;
  //     if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
  //       *this << t->name;
  //     } else {
  //       *this << value_type;
  //     }
  //     *this << " " << q.name;
  //     sep = ", ";
  //   }
  // }

  // *this << ") {\n";
  // enter_scope();
  // indent();

  // // aliases, variables, local types, etc.
  // for (const Ptr<AliasDecl> &a : n.aliases) {
  //   emit_leading_comments(*a);
  //   *this << *a;
  // }
  // for (const Ptr<Decl> &d : n.decls) {
  //   emit_leading_comments(*d);
  //   *this << *d;
  // }

  // for (const Ptr<Stmt> &s : n.body) {
  //   emit_leading_comments(*s);
  //   *this << *s;
  // }

  // // clean up any aliases we defined
  // for (const Ptr<Decl> &d : n.decls) {
  //   if (auto a = dynamic_cast<const AliasDecl *>(d.get())) {
  //     *this << "#undef " << a->name << "\n";
  //   }
  // }
  // for (const Ptr<AliasDecl> &a : n.aliases) {
  //   *this << "#undef " << a->name << "\n";
  // }

  // dedent();
  // leave_scope();
  // *this << indentation() << "}\n";
  // // scope_level--;
}

void TModel::visit_startstate(const StartState &n) {
  // // scope_level++;
  // *this << indentation() << "void startstate_" << n.name << "(";

  // // parameters
  // if (n.quantifiers.empty()) {
  //   *this << "void";
  // } else {
  //   std::string sep;
  //   for (const Quantifier &q : n.quantifiers) {
  //     *this << sep;
  //     if (auto t = dynamic_cast<const TypeExprID *>(q.type.get())) {
  //       *this << t->name;
  //     } else {
  //       *this << value_type;
  //     }
  //     *this << " " << q.name;
  //     sep = ", ";
  //   }
  // }

  // *this << ") {\n";
  // enter_scope();
  // indent();

  // // aliases, variables, local types, etc.
  // for (const Ptr<AliasDecl> &a : n.aliases) {
  //   emit_leading_comments(*a);
  //   *this << *a;
  // }
  // for (const Ptr<Decl> &d : n.decls) {
  //   emit_leading_comments(*d);
  //   *this << *d;
  // }

  // for (const Ptr<Stmt> &s : n.body) {
  //   emit_leading_comments(*s);
  //   *this << *s;
  // }

  // // clean up any aliases we defined
  // for (const Ptr<Decl> &d : n.decls) {
  //   if (auto a = dynamic_cast<const AliasDecl *>(d.get())) {
  //     *this << "#undef " << a->name << "\n";
  //   }
  // }
  // for (const Ptr<AliasDecl> &a : n.aliases) {
  //   *this << "#undef " << a->name << "\n";
  // }

  // dedent();
  // leave_scope();
  // *this << indentation() << "}\n\n";
}


// << ==================================== Mostly Useless ====================================== >> 

// ...
void TModel::visit_add(const Add &n) {}

void TModel::visit_aliasdecl(const AliasDecl &n) {}

void TModel::visit_aliasrule(const AliasRule &) {
  // this is unreachable because generate_c is only ever called with a Model
  // and visit_model flattens all rules
  assert(!"unreachable");
  __builtin_unreachable();
}

void TModel::visit_aliasstmt(const AliasStmt &n) {}

void TModel::visit_and(const And &n) {}

void TModel::visit_assignment(const Assignment &n) {}

void TModel::visit_band(const rumur::Band &n) {}

void TModel::visit_bnot(const rumur::Bnot &n) {}

void TModel::visit_bor(const rumur::Bor &n) {}

void TModel::visit_clear(const Clear &n) {}

void TModel::visit_div(const Div &n) {}

void TModel::visit_element(const Element &n) {}

void TModel::visit_eq(const Eq &n) {}

void TModel::visit_errorstmt(const ErrorStmt &n) {}

void TModel::visit_exists(const Exists &n) {}

void TModel::visit_exprid(const ExprID &n) {}

void TModel::visit_field(const Field &n) {}

void TModel::visit_for(const For &n) {}

void TModel::visit_forall(const Forall &n) {}

void TModel::visit_functioncall(const FunctionCall &n) {}

void TModel::visit_geq(const Geq &n) {}

void TModel::visit_gt(const Gt &n) {}

void TModel::visit_if(const If &n) {}

void TModel::visit_ifclause(const IfClause &n) {}

void TModel::visit_implication(const Implication &n) {}

void TModel::visit_isundefined(const IsUndefined &) {}

void TModel::visit_leq(const Leq &n) {}

void TModel::visit_lsh(const rumur::Lsh &n) {}

void TModel::visit_lt(const Lt &n) {}

void TModel::visit_mod(const Mod &n) {}

void TModel::visit_mul(const Mul &n) {}

void TModel::visit_negative(const Negative &n) {}

void TModel::visit_neq(const Neq &n) {}

void TModel::visit_not(const Not &n) {}

void TModel::visit_number(const Number &n) {}

void TModel::visit_or(const Or &n) {}

void TModel::visit_procedurecall(const ProcedureCall &n) {}

void TModel::visit_property(const Property &) {
  // this is unreachable because generate_c is only ever called with a Model
  // and nothing that contains a Property descends into it
  assert(!"unreachable");
  __builtin_unreachable();
}

void TModel::visit_propertystmt(const PropertyStmt &n) {

  // switch (n.property.category) {

  // case Property::ASSERTION:
  //   *this << indentation() << "if (!" << *n.property.expr << ") {\n";
  //   indent();
  //   *this << indentation() << "failed_assertion(\""
  //         << escape(n.message == "" ? n.property.expr->to_string() : n.message)
  //         << "\");\n";
  //   dedent();
  //   *this << indentation() << "}";
  //   break;

  // case Property::ASSUMPTION:
  //   *this << indentation() << "if (!" << *n.property.expr << ") {\n";
  //   indent();
  //   *this << indentation() << "failed_assumption(\""
  //         << escape(n.message == "" ? n.property.expr->to_string() : n.message)
  //         << "\");\n";
  //   dedent();
  //   *this << indentation() << "}";
  //   break;

  // case Property::COVER:
  //   *this << indentation() << "if " << *n.property.expr << " {\n";
  //   indent();
  //   *this << indentation() << "cover(\""
  //         << escape(n.message == "" ? n.property.expr->to_string() : n.message)
  //         << "\");\n";
  //   dedent();
  //   *this << indentation() << "}";
  //   break;

  // case Property::LIVENESS:
  //   *this << indentation() << "if " << *n.property.expr << " {\n";
  //   indent();
  //   *this << indentation() << "liveness(\""
  //         << escape(n.message == "" ? n.property.expr->to_string() : n.message)
  //         << "\");\n";
  //   dedent();
  //   *this << indentation() << "}";
  //   break;
  // }

  // emit_trailing_comments(n);
  // *this << "\n";
}

void TModel::visit_put(const Put &n) {}

void TModel::visit_quantifier(const Quantifier &n) {}

void TModel::visit_return(const Return &n) {}

void TModel::visit_rsh(const rumur::Rsh &n) {}

void TModel::visit_ruleset(const Ruleset &) {
  // this is unreachable because generate_c is only ever called with a Model
  // and all rule are flattened during visit_model
  assert(!"unreachable");
  __builtin_unreachable();
}

void TModel::visit_sub(const Sub &n) {}

void TModel::visit_switch(const Switch &n) {}

void TModel::visit_switchcase(const SwitchCase &n) {}

void TModel::visit_ternary(const Ternary &n) {}

// void TModel::visit_typedecl(const TypeDecl &n) {}

// void TModel::visit_typeexprid(const TypeExprID &n) {}

void TModel::visit_undefine(const Undefine &n) {}

void TModel::visit_while(const While &n) {}

void TModel::visit_xor(const rumur::Xor &n) {}



TModel &TModel::operator<<(const std::string &s) {
  out << s;
  return *this;
}

TModel &TModel::operator<<(const Node &n) {
  dispatch(n);
  return *this;
}


// write out a single line comment, accounting for the fact that '\' is an
// escape leader in C that we should suppress if it is escaping the trailing new
// line
static void write_content(const rumur::Comment &c, std::ostream &out) {
  assert(!c.multiline);

  // detect if we have a trailing backslash
  size_t i = 0;
  for (; i < c.content.size(); ++i) {
    if (c.content[i] == '\\') {
      bool remainder_is_space = true;
      for (size_t j = i + 1; j < c.content.size(); ++j) {
        if (!isspace(c.content[j])) {
          remainder_is_space = false;
          break;
        }
      }
      if (remainder_is_space) {
        break;
      }
      ++i; // skip escape
    }
  }

  out << c.content.substr(0, i);
}


size_t TModel::emit_leading_comments(const Node &n) {
  size_t count = 0;
  size_t i = 0;
  for (const rumur::Comment &c : comments) {
    // has this not yet been printed?
    if (!emitted[i]) {
      // does this precede the given node?
      if (c.loc.end.line < n.loc.begin.line ||
          (c.loc.end.line == n.loc.begin.line &&
           c.loc.end.column <= n.loc.begin.column)) {

        // do some white space adjustment for multiline comments
        if (c.multiline) {
          *this << indentation() << "/*";
          bool dropping = false;
          for (const char &b : c.content) {
            if (b == '\n') {
              *this << "\n" << indentation() << " ";
              dropping = true;
            } else if (dropping) {
              if (!isspace(b)) {
                out << b;
                dropping = false;
              }
            } else {
              out << b;
            }
          }
          *this << "*/\n";

        } else { // single line comments can be emitted simpler
          *this << indentation() << "//";
          write_content(c, out);
          *this << "\n";
        }

        emitted[i] = true;
      }
    }
    ++i;
  }
  return count;
}

size_t TModel::drop_comments(const position &pos) {
  size_t count = 0;
  size_t i = 0;
  for (const rumur::Comment &c : comments) {
    // does this precede the given node?
    if (c.loc.end.line < pos.line ||
        (c.loc.end.line == pos.line && c.loc.end.column <= pos.column)) {
      // mark it as emitted so it will be skipped in future
      emitted[i] = true;
    }
    ++i;
  }
  return count;
}

size_t TModel::emit_trailing_comments(const Node &n) {
  size_t count = 0;
  size_t i = 0;
  for (const rumur::Comment &c : comments) {
    if (!emitted[i] && !c.multiline && c.loc.begin.line == n.loc.end.line) {
      *this << " //";
      write_content(c, out);
      emitted[i] = true;
      ++count;
    }
    ++i;
  }
  return count;
}

} // namespace romp
