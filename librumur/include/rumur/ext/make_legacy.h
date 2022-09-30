#pragma once

#include <rumur/ext/traverse.h>
#include <typeinfo> 

#ifndef RUMUR_API_WITH_RTTI
#define RUMUR_API_WITH_RTTI __attribute__((visibility("default")))
#endif

namespace rumur {
namespace ext {

/**
 * @brief Convert a model with extended (ext) AST nodes to a legacy AST.
 *        This allows older model checkers to easily support newer Murphi syntax.
 */
Ptr<Node> make_legacy(const Ptr<Node> &n);

/**
 * @brief abstract interface for extended AST nodes to allow for them to be easily replaced by 
 *        legacy compliant AST nodes.
 * @tparam LT the legacy node that 
 */
// template<class LT>
struct RUMUR_API_WITH_RTTI IExtNode {
  // static_assert(not std::is_base_of<IExtNode, LT>::value, "LT (Legacy node Type) cannot be an extended node!");

  /**
   * @brief Convert an extended AST node to a legacy AST node.
   * @return Ptr to legacy conversion of node 
   */
  virtual Ptr<Node> make_legacy() const = 0;
  // virtual Ptr<LT> make_legacy() const = 0;

};

} //namespace ext
} //namespace rumur
