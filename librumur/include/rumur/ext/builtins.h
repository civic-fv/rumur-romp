/**
 * @proj rumurlib
 * @author Andrew Osterhout (github: @osterhoutan & @osterhoutan-UofU)
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file ext/builtins.h
 * 
 * @brief a collection of static builtin functions ast objects that serve as a stdlib 
 *        the for the Murphi model language, inclusion in your model-checker generator 
 *        should always be optional.
 * 
 * @date 2022/09/14
 * @version 0.1
 */

#pragma once

#include <rumur/ext/TypeExpt.h>
#include <rumur/ext/Function.h>
#include <rumur/ext/Expr.h>
#include <rumur/ext/Stmt.h>


#ifndef RUMUR_API_WITH_RTTI
#define RUMUR_API_WITH_RTTI __attribute__((visibility("default")))
#endif

namespace rumur {
namespace ext {

  // nothing to see here, yet..

} //namespace ext
} //namespace rumur
