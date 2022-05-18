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

#include "IModel.hpp"

#include "../../../common/escape.h"
#include "../../../common/isa.h"
#include "../options.h"
#include <cassert>
#include <cstddef>
#include <ctype.h>
#include <gmpxx.h>
#include <rumur/rumur.h>
#include <string>
#include <utility>
#include <vector>

using namespace rumur;

namespace romp {

// << ========================================================================================== >> 
// <<                               CONSTRUCTORS & DECONSTRUCTORS                                >> 
// << ========================================================================================== >> 

IModel::~IModel() {}



// << ========================================================================================== >> 
// <<                                 PRIVATE HELPER FUNCTIONS                                   >> 
// << ========================================================================================== >> 





// << ========================================================================================== >> 
// <<                                  PARSING UTIL FUNCTIONS                                    >> 
// << ========================================================================================== >> 


void IModel::visit_model(const Model &n) {
  _model = &n; // save the toplevel of the model for later.
}


void IModel::visit_constdecl(const ConstDecl &n) {
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


void IModel::visit_typedecl(const TypeDecl &n) {
  
}


void IModel::visit_typeexprid(const TypeExprID &n) {
  assert (typeIds.find(n.name) == typeIds.end() && "Undefined Type ID");
}


void IModel::visit_enum(const Enum &n) {
  
}


void IModel::visit_array(const Array &n) {}


void IModel::visit_range(const Range &) {}


void IModel::visit_record(const Record &n) {}


void IModel::visit_scalarset(const Scalarset &) {}


void IModel::visit_vardecl(const VarDecl &n) {
  // *this << indentation() << *n.type << " " << n.name << ";";
  // emit_trailing_comments(n);
  // *this << "\n";
}


void IModel::visit_function(const Function &n) {
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

void IModel::visit_propertyrule(const PropertyRule &n) {
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

void IModel::visit_simplerule(const SimpleRule &n) {
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

void IModel::visit_startstate(const StartState &n) {
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
void IModel::visit_add(const Add &n) {}

void IModel::visit_aliasdecl(const AliasDecl &n) {}

void IModel::visit_aliasrule(const AliasRule &) {
  // this is unreachable because generate_c is only ever called with a Model
  // and visit_model flattens all rules
  assert(!"unreachable");
  __builtin_unreachable();
}

void IModel::visit_aliasstmt(const AliasStmt &n) {}

void IModel::visit_and(const And &n) {}

void IModel::visit_assignment(const Assignment &n) {}

void IModel::visit_band(const Band &n) {}

void IModel::visit_bnot(const Bnot &n) {}

void IModel::visit_bor(const Bor &n) {}

void IModel::visit_clear(const Clear &n) {}

void IModel::visit_div(const Div &n) {}

void IModel::visit_element(const Element &n) {}

void IModel::visit_eq(const Eq &n) {}

void IModel::visit_errorstmt(const ErrorStmt &n) {}

void IModel::visit_exists(const Exists &n) {}

void IModel::visit_exprid(const ExprID &n) {}

void IModel::visit_field(const Field &n) {}

void IModel::visit_for(const For &n) {}

void IModel::visit_forall(const Forall &n) {}

void IModel::visit_functioncall(const FunctionCall &n) {}

void IModel::visit_geq(const Geq &n) {}

void IModel::visit_gt(const Gt &n) {}

void IModel::visit_if(const If &n) {}

void IModel::visit_ifclause(const IfClause &n) {}

void IModel::visit_implication(const Implication &n) {}

void IModel::visit_isundefined(const IsUndefined &) {}

void IModel::visit_leq(const Leq &n) {}

void IModel::visit_lsh(const Lsh &n) {}

void IModel::visit_lt(const Lt &n) {}

void IModel::visit_mod(const Mod &n) {}

void IModel::visit_mul(const Mul &n) {}

void IModel::visit_negative(const Negative &n) {}

void IModel::visit_neq(const Neq &n) {}

void IModel::visit_not(const Not &n) {}

void IModel::visit_number(const Number &n) {}

void IModel::visit_or(const Or &n) {}

void IModel::visit_procedurecall(const ProcedureCall &n) {}

void IModel::visit_property(const Property &) {
  // this is unreachable because generate_c is only ever called with a Model
  // and nothing that contains a Property descends into it
  assert(!"unreachable");
  __builtin_unreachable();
}

void IModel::visit_propertystmt(const PropertyStmt &n) {

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

void IModel::visit_put(const Put &n) {}

void IModel::visit_quantifier(const Quantifier &n) {}

void IModel::visit_return(const Return &n) {}

void IModel::visit_rsh(const Rsh &n) {}

void IModel::visit_ruleset(const Ruleset &) {
  // this is unreachable because generate_c is only ever called with a Model
  // and all rule are flattened during visit_model
  assert(!"unreachable");
  __builtin_unreachable();
}

void IModel::visit_sub(const Sub &n) {}

void IModel::visit_switch(const Switch &n) {}

void IModel::visit_switchcase(const SwitchCase &n) {}

void IModel::visit_ternary(const Ternary &n) {}

void IModel::visit_typedecl(const TypeDecl &n) {}

void IModel::visit_typeexprid(const TypeExprID &n) {}

void IModel::visit_undefine(const Undefine &n) {}

void IModel::visit_while(const While &n) {}

void IModel::visit_xor(const Xor &n) {}


} // namespace romp
