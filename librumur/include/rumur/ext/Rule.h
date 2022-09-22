#pragma once

#include <rumur/Rule.h>
#include <rumur/ext/TypeExpr.h>

#ifndef RUMUR_API_WITH_RTTI
#define RUMUR_API_WITH_RTTI __attribute__((visibility("default")))
#endif

namespace rumur {
namespace ext {

struct RUMUR_API_WITH_RTTI ChooseRule : public Ruleset {

  ChooseRule(const std::vector<MultisetVarQuantifier>& quantifiers_,
              const std::vector<Ptr<Rule>>& rules_, const location& loc_);
  ChooseRule *clone() const override;
  ~ChooseRule() = default;

  void visit(BaseTraversal& visitor) override;
  void visit(ConstBaseTraversal& visitor) const override;

  void update() override;
  void validate() const override;
};

} //namespace ext
} //namespace rumur
