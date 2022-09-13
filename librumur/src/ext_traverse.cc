
#include <rumur/ext_traverse.h>

namespace rumur {

void ExtTraversal::visit_scalarsetunion(ScalarsetUnion& n) {
  for (auto m : n.members) 
    dispatch(*m);
}
void ConstExtTraversal::visit_scalarsetunion(const ScalarsetUnion& n) {
  for (const auto m : n.members) 
    dispatch(*m);
}
void ConstExtExprTraversal::visit_scalarsetunion(const ScalarsetUnion& n) {
  for (const auto m : n.members) 
    dispatch(*m);
}
void ConstExtStmtTraversal::visit_scalarsetunion(const ScalarsetUnion& n) {
  for (const auto m : n.members) 
    dispatch(*m);
}
// void ConstExtTypeTraversal::visit_scalarsetunion(const ScalarsetUnion& n) {  //not used for a type expr
//   for (const auto m : n.members) 
//     dispatch(*m);
// }



} // namespace rumur
