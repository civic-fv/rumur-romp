#pragma once

#include <rumur/Rule.h>
#include <rumur/ext/un-ext.h>
#include <rumur/ext/TypeExpr.h>
#include <rumur/ext/Expr.h>

#ifndef RUMUR_API_WITH_RTTI
#define RUMUR_API_WITH_RTTI __attribute__((visibility("default")))
#endif

namespace rumur {
namespace ext {


struct RUMUR_API_WITH_RTTI ChooseRule : public Rule, public IExtNode<Ruleset> {

  std::vector<Ptr<Rule>> rules;

  ChooseRule(const std::vector<MultisetQuantifier> &ms_quantifiers_,
             const std::vector<Ptr<Rule>>& rules_, const location& loc_);
  virtual ~ChooseRule() = default;
  ChooseRule *clone() const final;


  void visit(BaseTraversal &visitor) final;
  void visit(ConstBaseTraversal &visitor) const final;

  void validate() const final;
  std::vector<Ptr<Rule>> flatten() const final;

  Ptr<Ruleset> make_legacy() const;
};


} //namespace ext
} //namespace rumur