#include <rumur/ext/Rule.h>

namespace rumur {
namespace ext {

ChooseRule::ChooseRule(const std::vector<MultisetVarQuantifier>& quantifiers_,
                       const std::vector<Ptr<Rule>>& rules_, const location& loc_)
  : Ruleset(quantifiers_, rules_, loc_) {}
ChooseRule *ChooseRule::clone() const { return new ChooseRule(*this); }

void ChooseRule::visit(BaseTraversal& visitor) { visitor.visit_chooserule(*this); }
void ChooseRule::visit(ConstBaseTraversal& visitor) const { visitor.visit_chooserule(*this); }

void ChooseRule::update() {}
void ChooseRule::validate() const {}

} //namespace ext
} //namespace rumur
