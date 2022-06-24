/**
 * @proj romp
 * @author Andrew Osterhout
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp_def.hpp
 * 
 * @brief A bunch of definitions for configuration 
 *        and other details in the romp codegen tool.
 * 
 * @date 2022/05/11
 * @version 0.1
 */

#ifndef _romp_src_romp_def_hpp
#define _romp_src_romp_def_hpp

#define ROMP_MODEL_NAMESPACE_NAME "__model__"

#define ROMP_CALLER_NAMESPACE_NAME "__caller__"

#define ROMP_STATE_CLASS_NAME "__State__"

#define ROMP_STATE_TYPE "::" ROMP_MODEL_NAMESPACE "::" ROMP_STATE_CLASS_NAME

#define ROMP_TYPE_NAMESPACE "__type__"

#define ROMP_CALLABLE_RULE_TYPE "::romp::Rule";

#define ROMP_CALLABLE_INVARIANT_TYPE "::romp::Invariant";

#define ROMP_CALLABLE_STARTSTATE_TYPE "::romp::StartState";

#define ROMP_MODEL_EXCEPTION "::romp::ModelException"

#define ROMP_MODEL_EXCEPTION_TYPE ROMP_MODEL_EXCEPTION "*"

#define ROMP_RESERVED_NAMES "rules", "invariants", "startstates", "__State__", "RandWalker", "to_json", \
                            "__model__", "__caller__", "romp" , "boolean"

#define ROMP_JSON_TYPE "nlohmann::json"

#define ROMP_SHOW_TYPE_OPTION_EXPR "options.SHOW_TYPE"

#define ROMP_GENERATED_FILE_PREFACE(p) \
        "/*" p "*/"

#define ROMP_PREDEFINED_TYPES "boolean"

#ifdef _ROMP_ENABLE_CUSTOM_RUMUR_TYPE_TRAVERSAL
#include <rumur/rumur.h>
namespace rumur {
class ConstBaseTypeTraversal : public BaseConstTraversal {
  std::string msg;
  ConstBaseTypeTraversal(std::string& msg_) : msg(msg_) {}
protected:
  void unsupported_traversal(const Node& n, const char* type_name) const { throw Error(" [" + type_name +"] :: " + msg, n.loc); }
public:
  void visit_add(const Add& n) { unsupported_traversal(n,"rumur::Add"); }
  void visit_aliasdecl(const AliasDecl& n) { unsupported_traversal(n,"rumur::AliasDecl"); }
  void visit_aliasrule(const AliasRule& n) { unsupported_traversal(n,"rumur::AliasRule"); }
  void visit_aliasstmt(const AliasStmt& n) { unsupported_traversal(n,"rumur::AliasStmt"); }
  void visit_and(const And& n) { unsupported_traversal(n,"rumur::And"); }
//   void visit_array(const Array& n) { unsupported_traversal(n,"rumur::Array"); }
  void visit_assignment(const Assignment& n) { unsupported_traversal(n,"rumur::Assignment"); }
  void visit_band(const Band& n) { unsupported_traversal(n,"rumur::Band"); }
  void visit_bnot(const Bnot& n) { unsupported_traversal(n,"rumur::Bnot"); }
  void visit_bor(const Bor& n) { unsupported_traversal(n,"rumur::Bor"); }
  void visit_clear(const Clear& n) { unsupported_traversal(n,"rumur::Clear"); }
  void visit_constdecl(const ConstDecl& n) { unsupported_traversal(n,"rumur::ConstDecl"); }
  void visit_div(const Div& n) { unsupported_traversal(n,"rumur::Div"); }
  void visit_element(const Element& n) { unsupported_traversal(n,"rumur::Element"); }
//   void visit_enum(const Enum& n) { unsupported_traversal(n,"rumur::Enum"); }
  void visit_eq(const Eq& n) { unsupported_traversal(n,"rumur::Eq"); }
  void visit_errorstmt(const ErrorStmt& n) { unsupported_traversal(n,"rumur::ErrorStmt"); }
  void visit_exists(const Exists& n) { unsupported_traversal(n,"rumur::Exists"); }
  void visit_exprid(const ExprID& n) { unsupported_traversal(n,"rumur::ExprID"); }
  void visit_field(const Field& n) { unsupported_traversal(n,"rumur::Field"); }
  void visit_for(const For& n) { unsupported_traversal(n,"rumur::For"); }
  void visit_forall(const Forall& n) { unsupported_traversal(n,"rumur::Forall"); }
  void visit_function(const Function& n) { unsupported_traversal(n,"rumur::Function"); }
  void visit_functioncall(const FunctionCall& n) { unsupported_traversal(n,"rumur::FunctionCall"); }
  void visit_geq(const Geq& n) { unsupported_traversal(n,"rumur::Geq"); }
  void visit_gt(const Gt& n) { unsupported_traversal(n,"rumur::Gt"); }
  void visit_if(const If& n) { unsupported_traversal(n,"rumur::If"); }
  void visit_ifclause(const IfClause& n) { unsupported_traversal(n,"rumur::IfClause"); }
  void visit_implication(const Implication& n) { unsupported_traversal(n,"rumur::Implication"); }
  void visit_isundefined(const IsUndefined& n) { unsupported_traversal(n,"rumur::IsUndefined"); }
  void visit_leq(const Leq& n) { unsupported_traversal(n,"rumur::Leq"); }
  void visit_lsh(const Lsh& n) { unsupported_traversal(n,"rumur::Lsh"); }
  void visit_lt(const Lt& n) { unsupported_traversal(n,"rumur::Lt"); }
  void visit_model(const Model& n) { unsupported_traversal(n,"rumur::Model"); }
  void visit_mod(const Mod& n) { unsupported_traversal(n,"rumur::Mod"); }
  void visit_mul(const Mul& n) { unsupported_traversal(n,"rumur::Mul"); }
  void visit_negative(const Negative& n) { unsupported_traversal(n,"rumur::Negative"); }
  void visit_neq(const Neq& n) { unsupported_traversal(n,"rumur::Neq"); }
  void visit_not(const Not& n) { unsupported_traversal(n,"rumur::Not"); }
  void visit_number(const Number& n) { unsupported_traversal(n,"rumur::Number"); }
  void visit_or(const Or& n) { unsupported_traversal(n,"rumur::Or"); }
  void visit_procedurecall(const ProcedureCall& n) { unsupported_traversal(n,"rumur::ProcedureCall"); }
  void visit_property(const Property& n) { unsupported_traversal(n,"rumur::Property"); }
  void visit_propertyrule(const PropertyRule& n) { unsupported_traversal(n,"rumur::PropertyRule"); }
  void visit_propertystmt(const PropertyStmt& n) { unsupported_traversal(n,"rumur::PropertyStmt"); }
  void visit_put(const Put& n) { unsupported_traversal(n,"rumur::Put"); }
  void visit_quantifier(const Quantifier& n) { unsupported_traversal(n,"rumur::Quantifier"); }
//   void visit_range(const Range& n) { unsupported_traversal(n,"rumur::Range"); }
//   void visit_record(const Record& n) { unsupported_traversal(n,"rumur::Record"); }
  void visit_return(const Return& n) { unsupported_traversal(n,"rumur::Return"); }
  void visit_rsh(const Rsh& n) { unsupported_traversal(n,"rumur::Rsh"); }
  void visit_ruleset(const Ruleset& n) { unsupported_traversal(n,"rumur::Ruleset"); }
//   void visit_scalarset(const Scalarset& n) { unsupported_traversal(n,"rumur::Scalarset"); }
  void visit_simplerule(const SimpleRule& n) { unsupported_traversal(n,"rumur::SimpleRule"); }
  void visit_startstate(const StartState& n) { unsupported_traversal(n,"rumur::StartState"); }
  void visit_sub(const Sub& n) { unsupported_traversal(n,"rumur::Sub"); }
  void visit_switch(const Switch& n) { unsupported_traversal(n,"rumur::Switch"); }
  void visit_switchcase(const SwitchCase& n) { unsupported_traversal(n,"rumur::SwitchCase"); }
  void visit_ternary(const Ternary& n) { unsupported_traversal(n,"rumur::Ternary"); }
  void visit_typedecl(const TypeDecl& n) { unsupported_traversal(n,"rumur::TypeDecl"); }
//   void visit_typeexprid(const TypeExprID& n) { unsupported_traversal(n,"rumur::TypeExprID"); }
  void visit_undefine(const Undefine& n) { unsupported_traversal(n,"rumur::Undefine"); }
  void visit_vardecl(const VarDecl& n) { unsupported_traversal(n,"rumur::VarDecl"); }
  void visit_while(const While& n) { unsupported_traversal(n,"rumur::While"); }
  void visit_xor(const Xor& n) { unsupported_traversal(n,"rumur::Xor"); }
};
}
#endif

#endif
