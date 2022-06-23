/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file SigPerm.cpp
 * 
 * @brief Implements a iterator class that can produce lists of the various permutations
 *        in the quantifiers of rumur's AST Rule's, so that we can call them in our output.
 * 
 * @date 2022/06/22
 * @version 0.1
 */

#include "SigPerm.hpp"

namespace romp {
  using namespace rumur;


// << ========================================================================================== >> 
// <<                                          SigPerm                                           >> 
// << ========================================================================================== >> 
#pragma region SigPerm__code

static void SigPerm::add_quant(const Quantifier& q) {

}

SigPerm::SigPerm(const Rule& rule) : rule(rule) {

}

SigPerm::Iterator SigPerm::begin() const {

}

SigPerm::Iterator SigPerm::end() const {

}


#pragma endregion SigPerm__code


// << ========================================================================================== >> 
// <<                                       SigPerm::Sig                                        >> 
// << ========================================================================================== >> 
#pragma region SigPerm_Item__code


std::string SigPerm::Sig::to_string() {

}


#pragma endregion SigPerm_Item__code


// << ========================================================================================== >> 
// <<                                     SigPerm::Iterator                                      >> 
// << ========================================================================================== >> 
#pragma region SigPerm_Iterator__code





std::string SigPerm::Iterator::to_string() { return sig_ptr->to_string(); }

#pragma endregion SigPerm_Iterator__code

}
