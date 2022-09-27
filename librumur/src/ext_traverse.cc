
#include <rumur/traverse.h>
#include <rumur/ext/traverse.h>
#include <casssert>

namespace rumur {

// << ========================================================================================== >> 
// <<      IMPLEMENTATION OF EXT SYNTAX VISITATION METHODS NODES FOR (CONST)BASETRAVERSAL        >> 
// << ========================================================================================== >> 


void BaseTraversal::visit_ChooseRule(ext::ChooseRule& n) {
  visit_ruleset(*n.make_legacy());
}
void BaseTraversal::visit_ismember(ext::IsMember& n) {
  visit_or(*n.make_legacy());
}
void BaseTraversal::visit_multiset(ext::Multiset& n) {
  visit_array(*n.make_legacy());
}
void BaseTraversal::visit_multisetadd(ext::MultisetAdd& n) {
  visit_aliasstmt(*n.make_legacy());
}
void BaseTraversal::visit_multisetcount(ext::MultisetCount& n) {
  visit_add(*n.make_legacy());
}
void BaseTraversal::visit_multisetelement(ext::MultisetElement& n) {
  visit_element(*n.make_legacy());
}
void BaseTraversal::visit_multisetremovepred(ext::MultisetRemovePred& n) {
  visit_aliasstmt(*n.make_legacy());
}
void BaseTraversal::visit_multisetquantifier(ext::MultisetQuantifier& n) {
  visit_quantifier(*n.make_legacy());
}
void BaseTraversal::visit_scalarsetunion(ext::ScalarsetUnion& n) {
  visit_scalarset(*n.make_legacy());
}
void BaseTraversal::visit_sucast(ext::SUCast& n) {
  visit_add(*n.make_legacy());
}

void BaseTraversal::visit_BuiltInFunction(ext::IBuiltInFunction& n) {
  visit_function(*n.make_legacy());
}


// << ------------------------------------------------------------------------------------------ >> 


void ConstBaseTraversal::visit_ChooseRule(const ext::ChooseRule& n) {
  visit_ruleset(*n.make_legacy());
}
void ConstBaseTraversal::visit_ismember(const ext::IsMember& n) {
  visit_or(*n.make_legacy());
}
void ConstBaseTraversal::visit_multiset(const ext::Multiset& n) {
  visit_array(*n.make_legacy());
}
void ConstBaseTraversal::visit_multisetadd(const ext::MultisetAdd& n) {
  visit_aliasstmt(*n.make_legacy());
}
void ConstBaseTraversal::visit_multisetcount(const ext::MultisetCount& n) {
  visit_add(*n.make_legacy());
}
void ConstBaseTraversal::visit_multisetelement(const ext::MultisetElement& n) {
  visit_element(*n.make_legacy());
}
void ConstBaseTraversal::visit_multisetremovepred(const ext::MultisetRemovePred& n) {
  visit_aliasstmt(*n.make_legacy());
}
void ConstBaseTraversal::visit_multisetquantifier(const ext::MultisetQuantifier& n) {
  visit_quantifier(*n.make_legacy());
}
void ConstBaseTraversal::visit_scalarsetunion(const ext::ScalarsetUnion& n) {
  visit_scalarset(*n.make_legacy());
}
void ConstBaseTraversal::visit_sucast(const ext::SUCast& n) {
  visit_add(*n.make_legacy());
}

void ConstBaseTraversal::visit_BuiltInFunction(const ext::IBuiltInFunction& n) {
  visit_function(*n.make_legacy());
}



namespace ext {

// << ========================================================================================== >> 
// <<               IMPLEMENTATION FOR PURPOSE BUILT EXT SYNTAX TRAVERSAL CLASSES                >> 
// << ========================================================================================== >> 

void ExtBaseTraversal::visit_chooserule(ChooseRule& n) {
  assert(false && "`visit_chooserule` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_chooserule(ChooseRule& n) {
  for (auto &mq : n.ms_quantifiers)
    dispatch(mq);
  for (auto &r : n.rules)
    dispatch(*r);
}
void ConstExtBaseTraversal::visit_chooserule(const ChooseRule& n) {
  assert(false && "`visit_chooserule` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
}
void ConstExtTraversal::visit_chooserule(const ChooseRule& n) {
  for (auto &mq : n.ms_quantifiers)
    dispatch(mq);
  for (auto &r : n.rules)
    dispatch(*r);
}
void ConstExtExprTraversal::visit_chooserule(const ChooseRule& n) {
  assert(false && "`visit_chooserule` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtExprTraversal)");
}
void ConstExtStmtTraversal::visit_chooserule(const ChooseRule& n) {
  for (auto &mq : n.ms_quantifiers)
    dispatch(mq);
  for (auto &r : n.rules)
    dispatch(*r);
}
void ConstExtTypeTraversal::visit_chooserule(const ChooseRule& n) {
  for (auto &mq : n.ms_quantifiers)
    dispatch(mq);
  for (auto &r : n.rules)
    dispatch(*r);
}

// << ------------------------------------------------------------------------------------------ >> 

void ExtBaseTraversal::visit_ismember(IsMember& n) {
  assert(false && "`visit_ismember` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_ismember(IsMember& n) {
  dispatch(*n.designator);
  dispatch(*n.type);
}
void ConstExtBaseTraversal::visit_ismember(const IsMember& n) {
  assert(false && "`visit_ismember` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
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

void ExtBaseTraversal::visit_multiset(Multiset& n) {
  assert(false && "`visit_multiset` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_multiset(Multiset& n) {
  dispatch(*n.index_type);
  dispatch(*n.element_type);
}
void ConstExtBaseTraversal::visit_multiset(const Multiset& n) {
  assert(false && "`visit_multiset` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
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

void ExtBaseTraversal::visit_multisetadd(MultisetAdd& n) {
  assert(false && "`visit_multisetadd` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)")
}
void ExtTraversal::visit_multisetadd(MultisetAdd& n) {
  dispatch(*n.multiset);
  dispatch(*n.value);
}
void ConstExtBaseTraversal::visit_multisetadd(const MultisetAdd& n) {
  assert(false && "`visit_multisetadd` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
}
void ConstExtTraversal::visit_multisetadd(const MultisetAdd& n) {
  dispatch(*n.multiset);
  dispatch(*n.value);
}
void ConstExtExprTraversal::visit_multisetadd(const MultisetAdd& n) {
  dispatch(*n.multiset);
  dispatch(*n.value);
}
void ConstExtStmtTraversal::visit_multisetadd(const MultisetAdd& n) {
  assert(false && "`visit_multisetadd` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtStmtTraversal)");
}
void ConstExtTypeTraversal::visit_multisetadd(const MultisetAdd& n) {
  dispatch(*n.multiset);
  dispatch(*n.value);
}


// << ------------------------------------------------------------------------------------------ >> 


void ExtBaseTraversal::visit_multisetcount(MultisetCount& n) {
  assert(false && "`visit_multisetcount` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_multisetcount(MultisetCount& n) {
  dispatch(n.ms_quantifier);
  dispatch(*n.condition);
}
void ConstExtBaseTraversal::visit_multisetcount(const MultisetCount& n) {
  assert(false && "`visit_multisetcount` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
}
void ConstExtTraversal::visit_multisetcount(const MultisetCount& n) {
  dispatch(n.ms_quantifier);
  dispatch(*n.condition);
}
void ConstExtExprTraversal::visit_multisetcount(const MultisetCount& n) {
  assert(false && "`visit_multisetcount` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtExprTraversal)");
}
void ConstExtStmtTraversal::visit_multisetcount(const MultisetCount& n) {
  dispatch(n.ms_quantifier);
  dispatch(*n.condition);
}
void ConstExtTypeTraversal::visit_multisetcount(const MultisetCount& n) {  //not used for a type expr
  dispatch(n.ms_quantifier);
  dispatch(*n.condition);
}


// << ------------------------------------------------------------------------------------------ >> 


void ExtBaseTraversal::visit_multisetelement(MultisetElement& n) {
  assert(false && "`visit_multisetelement` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_multisetelement(MultisetElement& n) {
  dispatch(*n.array);
  dispatch(*n.index);
}
void ConstExtBaseTraversal::visit_multisetelement(const MultisetElement& n) {
  assert(false && "`visit_multisetelement` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
}
void ConstExtTraversal::visit_multisetelement(const MultisetElement& n) {
  dispatch(*n.array);
  dispatch(*n.index);
}
void ConstExtExprTraversal::visit_multisetelement(const MultisetElement& n) {
  assert(false && "`visit_multisetelement` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtExprTraversal)");
}
void ConstExtStmtTraversal::visit_multisetelement(const MultisetElement& n) {
  dispatch(*n.array);
  dispatch(*n.index);
}
void ConstExtTypeTraversal::visit_multisetelement(const MultisetElement& n) {  //not used for a type expr
  dispatch(*n.array);
  dispatch(*n.index);
}


// << ------------------------------------------------------------------------------------------ >> 


void ExtBaseTraversal::visit_multisetremove(MultisetRemove& n) {
  assert(false && "`visit_multisetremove` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_multisetremove(MultisetRemove& n) {
  dispatch(*n.index);
  dispatch(*n.multiset);
}
void ConstExtBaseTraversal::visit_multisetremove(const MultisetRemove& n) {
  assert(false && "`visit_multisetremove` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
}
void ConstExtTraversal::visit_multisetremove(const MultisetRemove& n) {
  dispatch(*n.index);
  dispatch(*n.multiset);
}
void ConstExtExprTraversal::visit_multisetremove(const MultisetRemove& n) {
  dispatch(*n.index);
  dispatch(*n.multiset);
}
void ConstExtStmtTraversal::visit_multisetremove(const MultisetRemove& n) {
  assert(false && "`visit_multisetremove` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtStmtTraversal)");
}
void ConstExtTypeTraversal::visit_multisetremove(const MultisetRemove& n) { 
  dispatch(*n.index);
  dispatch(*n.multiset);
}


// << ------------------------------------------------------------------------------------------ >> 


void ExtBaseTraversal::visit_multisetremovepred(MultisetRemovePred& n) {
  assert(false && "`visit_multisetremovepred` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_multisetremovepred(MultisetRemovePred& n) {
  dispatch(n.ms_quantifier);
  dispatch(*n.pred);
}
void ConstExtBaseTraversal::visit_multisetremovepred(const MultisetRemovePred& n) {
  assert(false && "`visit_multisetremovepred` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
}
void ConstExtTraversal::visit_multisetremovepred(const MultisetRemovePred& n) {
  dispatch(n.ms_quantifier);
  dispatch(*n.pred);
}
void ConstExtExprTraversal::visit_multisetremovepred(const MultisetRemovePred& n) {
  dispatch(n.ms_quantifier);
  dispatch(*n.pred);
}
void ConstExtStmtTraversal::visit_multisetremovepred(const MultisetRemovePred& n) {
  assert(false && "`visit_multisetremovepred` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtStmtTraversal)");
}
void ConstExtTypeTraversal::visit_multisetremovepred(const MultisetRemovePred& n) { 
  dispatch(n.ms_quantifier);
  dispatch(*n.pred);
}


// << ------------------------------------------------------------------------------------------ >> 


void ExtBaseTraversal::visit_multisetquantifier(MultisetQuantifier& n) {
  assert(false && "`visit_multisetquantifier` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_multisetquantifier(MultisetQuantifier& n) {
  dispatch(*n.multiset);
}
void ConstExtBaseTraversal::visit_multisetquantifier(const MultisetQuantifier& n) {
  assert(false && "`visit_multisetquantifier` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
}
void ConstExtTraversal::visit_multisetquantifier(const MultisetQuantifier& n) {
  dispatch(*n.multiset);
}
void ConstExtExprTraversal::visit_multisetquantifier(const MultisetQuantifier& n) {
  dispatch(*n.multiset);
}
void ConstExtStmtTraversal::visit_multisetquantifier(const MultisetQuantifier& n) {
  assert(false && "`visit_multisetquantifier` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtStmtTraversal)");
}
void ConstExtTypeTraversal::visit_multisetquantifier(const MultisetQuantifier& n) { 
  dispatch(*n.multiset);
}


// << ------------------------------------------------------------------------------------------ >> 


void ExtBaseTraversal::visit_scalarsetunion(ScalarsetUnion& n) {
  assert(false && "`visit_scalarsetunion` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_scalarsetunion(ScalarsetUnion& n) {
  for (auto m : n.decl_list) 
    dispatch(*m);
}
void ConstExtBaseTraversal::visit_scalarsetunion(const ScalarsetUnion& n) {
  assert(false && "`visit_scalarsetunion` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
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


void ExtBaseTraversal::visit_sucast(SUCast& n) {
  assert(false && "`visit_sucast` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ExtBaseTraversal)");
}
void ExtTraversal::visit_sucast(SUCast& n) { // OPTIONAL to override
  visit_add(*n.make_legacy());
}
void ConstExtBaseTraversal::visit_sucast(const SUCast& n) {
  assert(false && "`visit_sucast` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
}
void ConstExtTraversal::visit_sucast(const SUCast& n) {  // OPTIONAL to override
  dispatch(*n.rhs);
}
void ConstExtExprTraversal::visit_sucast(const SUCast& n) {  // OPTIONAL to override
  assert(false && "`visit_sucast` needs to be overriden in implementing classes! "
                  "(from: rumur::ext::ConstExtBaseTraversal)");
}
void ConstExtStmtTraversal::visit_sucast(const SUCast& n) {  // OPTIONAL to override
  dispatch(*n.rhs);
}
void ConstExtTypeTraversal::visit_sucast(const SUCast& n) {  // OPTIONAL to override
  dispatch(*n.rhs);
}

// << ------------------------------------------------------------------------------------------ >> 


} // namespace ext
} // namespace rumur
