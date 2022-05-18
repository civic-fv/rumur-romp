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

#include <rumur/rumur.h>

#include <string>
#include <stack>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace romp {

/**
 * @brief   A Class that is a intermediary representation of the model,
 *        used to allow for generating the Cpp romp tools
 *        in a way that allows full context of the model.
 *          It allows the following:
 *        \li Keeping track of all types (generating names for anonymous types)
 *        \li Keeping track of all "variables" respecting scope.
 *        \li Keeping track of all functions, procedures, & rules.
 *        \li allowing lookup of all above mentioned items.
 *        \li preserving a notion of scope.
 */
class IModel : public rumur::ConstBaseTraversal {

  // << ===================================== Class Members ====================================== >> 
public: // ---- Public Class Members ---- //


private: // ---- Private Class Members ---- //

  int scope_level = 0; // keep track of what level of scope we are in durring processing
  std::stack<std::string> scope_str;
  const rumur::Node& root;
  const rumur::Model* _model = nullptr;

  std::vector<rumur::VarDecl&> state_vars;
  std::unordered_set<std::string> typeIds;
  std::unordered_map<std::string,rumur::TypeExpr&> typeExprs;


  // << ============================= Constructors & Deconstructors ============================== >> 
public:
  IModel(const rumur::Node& root) : root(root) { scope_str.push(""); }
  ~IModel();


  // << ========================== Public/External Function Functions ============================ >> 
public:




  // << =========================== Internal/Private Helper Functions ============================ >> 
private:

  void add_new_type(std::string& name, rumur::TypeExpr& value);
  void add_new_anon_type(rumur::TypeExpr& value);


  // << =========================== Model Processing Helper Functions ============================ >> 
public:
  // - most useful ---- 
  void visit_model(const rumur::Model &n) final;
  void visit_constdecl(const ConstDecl &n) final;
  void visit_typedecl(const rumur::TypeDecl &n) final;
  void visit_typeexprid(const rumur::TypeExprID &n) final;
  void visit_vardecl(const VarDecl &n) final;
  void visit_function(const Function &n) final;
  void visit_propertyrule(const PropertyRule &n) final;
  void visit_simplerule(const SimpleRule &n) final;
  void visit_startstate(const StartState &n) final;

  // - mostly ignored ---- 
  void visit_add(const rumur::Add &n) final;
  void visit_aliasdecl(const rumur::AliasDecl &n) final;
  void visit_aliasrule(const rumur::AliasRule &) final;
  void visit_aliasstmt(const rumur::AliasStmt &n) final;
  void visit_and(const rumur::And &n) final;
  void visit_array(const rumur::Array &n) final;
  void visit_assignment(const rumur::Assignment &n) final;
  void visit_band(const rumur::Band &n) final;
  void visit_bnot(const rumur::Bnot &n) final;
  void visit_bor(const rumur::Bor &n) final;
  void visit_clear(const rumur::Clear &n) final;
  void visit_div(const rumur::Div &n) final;
  void visit_element(const rumur::Element &n) final;
  void visit_enum(const rumur::Enum &n) final;
  void visit_eq(const rumur::Eq &n) final;
  void visit_errorstmt(const rumur::ErrorStmt &n) final;
  void visit_exists(const rumur::Exists &n) final;
  void visit_exprid(const rumur::ExprID &n) final;
  void visit_field(const rumur::Field &n) final;
  void visit_implication(const rumur::Implication &n) final;
  void visit_isundefined(const rumur::IsUndefined &) final;
  void visit_for(const rumur::For &n) final;
  void visit_forall(const rumur::Forall &n) final;
  void visit_functioncall(const rumur::FunctionCall &n) final;
  void visit_geq(const rumur::Geq &n) final;
  void visit_gt(const rumur::Gt &n) final;
  void visit_if(const rumur::If &n) final;
  void visit_ifclause(const rumur::IfClause &n) final;
  void visit_leq(const rumur::Leq &n) final;
  void visit_lsh(const rumur::Lsh &n) final;
  void visit_lt(const rumur::Lt &n) final;
  void visit_mod(const rumur::Mod &n) final;
  void visit_mul(const rumur::Mul &n) final;
  void visit_negative(const rumur::Negative &n) final;
  void visit_neq(const rumur::Neq &n) final;
  void visit_not(const rumur::Not &n) final;
  void visit_number(const rumur::Number &n) final;
  void visit_or(const rumur::Or &n) final;
  void visit_procedurecall(const rumur::ProcedureCall &n) final;
  void visit_property(const rumur::Property &) final;
  void visit_propertystmt(const rumur::PropertyStmt &n) final;
  void visit_put(const rumur::Put &n) final;
  void visit_quantifier(const rumur::Quantifier &n) final;
  void visit_range(const rumur::Range &) final;
  void visit_record(const rumur::Record &n) final;
  void visit_return(const rumur::Return &n) final;
  void visit_rsh(const rumur::Rsh &n) final;
  void visit_ruleset(const rumur::Ruleset &) final;
  void visit_scalarset(const rumur::Scalarset &) final;
  void visit_sub(const rumur::Sub &n) final;
  void visit_switch(const rumur::Switch &n) final;
  void visit_switchcase(const rumur::SwitchCase &n) final;
  void visit_ternary(const rumur::Ternary &n) final;
  void visit_undefine(const rumur::Undefine &n) final;
  void visit_while(const rumur::While &n) final;
  void visit_xor(const rumur::Xor &n) final;


  
};

} // namespace romp
