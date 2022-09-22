
#include <rumur/traverse.h>
#include <rumur/ext/traverse.h>
#include <casssert>

namespace rumur {

// << ========================================================================================== >> 
// <<      IMPLEMENTATION OF EXT SYNTAX VISITATION METHODS NODES FOR (CONST)BASETRAVERSAL        >> 
// << ========================================================================================== >> 

void BaseTraversal::visit_chooserule(ext::ChooseRule& n) {
  assert((n.aliases.size() <= 0) 
          && "ChooseRule was not updated before visitation");
  visit_ruleset(n);
}
void BaseTraversal::visit_ismember(ext::IsMember& n) {
  assert((n.lhs != nullptr && n.rhs != nullptr) 
          && "IsMember was not updated before visitation!");
  visit_or(n);
}
void BaseTraversal::visit_multiset(ext::Multiset& n) {
  // nothing to assert for multiset it's truly just an array in disguise under the hood
  visit_array(n);
}
void BaseTraversal::visit_multisetvarquantifier(ext::MultisetVarQuantifier& n) {
  assert((n.to != nullptr)
          && "MultisetVarQuantifier was not updated before visitation");
  visit_quantifier(n);
}
void BaseTraversal::visit_scalarsetunion(ext::ScalarsetUnion& n) {
  assert((n.bound != nullptr) 
          && "ScalarSetUnion was not updated before visitation!");
  visit_scalarset(n);
}
void BaseTraversal::visit_sucast(ext::SUCast& n) {
  assert((n.rhs != nullptr) 
          && "SUCast was not updated before visitation!");
  visit_add(n);
}

void BaseTraversal::visit_BuiltInFunction(ext::IBuiltInFunction& n) {
  assert((n.body.size() > 0) 
          && "child of IBuiltInFunction was not updated before visitation!");
  visit_function(n);
}


// << ------------------------------------------------------------------------------------------ >> 

void ConstBaseTraversal::visit_chooserule(const ext::ChooseRule& n) {
  assert((n.aliases.size() <= 0) 
          && "ChooseRule was not updated before visitation");
  visit_ruleset(n);
}
void ConstBaseTraversal::visit_ismember(const ext::IsMember& n) {
  assert((n.lhs != nullptr && n.rhs != nullptr) 
          && "IsMember was not updated before visitation!");
  visit_or(n);
}
void ConstBaseTraversal::visit_multiset(const ext::Multiset& n) {
  // nothing to assert for multiset it's truly just an array in disguise under the hood
  visit_array(n);
}
void ConstBaseTraversal::visit_multisetvarquantifier(const ext::MultisetVarQuantifier& n) {}
void ConstBaseTraversal::visit_scalarsetunion(const ext::ScalarsetUnion& n) {
  assert((n.bound != nullptr) 
          && "ScalarSetUnion was not updated before visitation!");
  visit_scalarset(n);
}
void ConstBaseTraversal::visit_sucast(const ext::SUCast& n) {
  assert((n.rhs != nullptr) 
          && "SUCast was not updated before visitation!");
  visit_add(n);
}

void ConstBaseTraversal::visit_BuiltInFunction(const ext::IBuiltInFunction& n) {
  assert((n.body.size() > 0) 
          && "child of IBuiltInFunction was not updated before visitation!");
  visit_function(n);
}



namespace ext {

// << ========================================================================================== >> 
// <<               IMPLEMENTATION FOR PURPOSE BUILT EXT SYNTAX TRAVERSAL CLASSES                >> 
// << ========================================================================================== >> 

void ExtTraversal::visit_ismember(IsMember& n) {
  dispatch(*n.designator);
  dispatch(*n.type);
}
void ConstExtTraversal::visit_ismember(const IsMember& n) {
  dispatch(*n.designator);
  dispatch(*n.type);
}
void ConstExtExprTraversal::visit_ismember(const IsMember& n) {
  assert(false && "`visit_ismember` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtExprTraversal)");
}
void ConstExtStmtTraversal::visit_ismember(const IsMember& n) {
  dispatch(*n.designator);
  dispatch(*n.type);
}
void ConstExtTypeTraversal::visit_ismember(const IsMember& n) {
  dispatch(*n.designator);
  dispatch(*n.type);
}

// << ------------------------------------------------------------------------------------------ >> 


void ExtTraversal::visit_multiset(Multiset& n) {
  dispatch(*n.index_type);
  dispatch(*n.element_type);
}
void ConstExtTraversal::visit_multiset(const Multiset& n) {
  dispatch(*n.index_type);
  dispatch(*n.element_type);
}
void ConstExtExprTraversal::visit_multiset(const Multiset& n) {
  dispatch(*n.index_type);
  dispatch(*n.element_type);
}
void ConstExtStmtTraversal::visit_multiset(const Multiset& n) {
  dispatch(*n.index_type);
  dispatch(*n.element_type);
}
void ConstExtTypeTraversal::visit_multiset(const Multiset& n) {  //not used for a type expr
  assert(false && "`visit_multiset` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtTypeTraversal)");
}

// << ------------------------------------------------------------------------------------------ >> 


void ExtTraversal::visit_scalarsetunion(ScalarsetUnion& n) {
  for (auto m : n.decl_list) 
    dispatch(*m);
}
void ConstExtTraversal::visit_scalarsetunion(const ScalarsetUnion& n) {
  for (const auto m : n.decl_list) 
    dispatch(*m);
}
void ConstExtExprTraversal::visit_scalarsetunion(const ScalarsetUnion& n) {
  for (const auto m : n.decl_list) 
    dispatch(*m);
}
void ConstExtStmtTraversal::visit_scalarsetunion(const ScalarsetUnion& n) {
  for (const auto m : n.decl_list) 
    dispatch(*m);
}
void ConstExtTypeTraversal::visit_scalarsetunion(const ScalarsetUnion& n) {  //not used for a type expr
  assert(false && "`visit_scalarsetunion` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtTypeTraversal)");
}

// << ------------------------------------------------------------------------------------------ >> 


void ExtTraversal::visit_sucast(SUCast& n) { // OPTIONAL to override
  dispatch(*n.target);
  dispatch(*n.lhs);
  if (n.rhs != nullptr) dispatch(*n.rhs);
}
void ConstExtTraversal::visit_sucast(const SUCast& n) {  // OPTIONAL to override
  dispatch(*n.target);
  dispatch(*n.lhs);
  if (n.rhs != nullptr) dispatch(*n.rhs);
}
void ConstExtExprTraversal::visit_sucast(const SUCast& n) {  // OPTIONAL to override
  dispatch(*n.target);
  dispatch(*n.lhs);
  if (n.rhs != nullptr) dispatch(*n.rhs);
}
void ConstExtStmtTraversal::visit_sucast(const SUCast& n) {  // OPTIONAL to override
  dispatch(*n.target);
  dispatch(*n.lhs);
  if (n.rhs != nullptr) dispatch(*n.rhs);
}
void ConstExtTypeTraversal::visit_sucast(const SUCast& n) {  // OPTIONAL to override
  dispatch(*n.target);
  dispatch(*n.lhs);
  if (n.rhs != nullptr) dispatch(*n.rhs);
}

// << ------------------------------------------------------------------------------------------ >> 


} // namespace ext
} // namespace rumur
