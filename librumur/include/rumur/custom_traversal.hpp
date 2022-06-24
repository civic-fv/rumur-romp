/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file custom_traversal.hpp
 * 
 * @brief This is a collection of macros that allows for the creation of custom traversals of
 *        rumur's Murphi AST, for easy quick definitions of minimal restricted traversal classes.
 * 
 * @date 2022/06/23
 * @version 0.1
 */

#ifndef _romp_src_custom_traversal_hpp
#define _romp_src_custom_traversal_hpp

#include <rumur/except.h>
#include <string>

namespace rumur {
struct TraverseError : public Error {} 
typedef void (ConstBaseTraversal::*custom_const_traverse_unimplemented_visit_handler_t)(const Node& n, const char* type_name);
typedef void (BaseTraversal::*custom_traverse_unimplemented_visit_handler_t)(const Node& n, const char* type_name);
}

/**
 * @brief (NOT YET IMPLIMENTED)
 *        creates default definitions of the various visit methods for the types you list 
 *        in the parenthesis and makes them call a setable
 */
#define CREATE_CUSTOM_CONST_TRAVERSAL()


#endif
