/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file SigPerm.hpp
 * 
 * @brief Implements a iterator class that can produce lists of the various permutations
 *        in the quantifiers of rumur's AST Rule's, so that we can call them in our output.
 * 
 *        Note: Note Optimized to be used in more than one file in a compilation (single file construction)
 * 
 * @date 2022/06/22
 * @version 0.1
 */

#pragma once


#include <rumur/rumur.h>
#include <cstddef>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <strstream>

namespace romp {

class SigPerm;
struct Sig;
struct SigParam;
struct QuantExpansion;

using vec_MTypeVals_t = ::std::vector<const ::romp::SigParam>;
using QuantExpansion_Iter_t = ::romp::vec_MTypeVals_t::iterator ;
using SigParam_Iter_t = ::romp::vec_MTypeVals_t::iterator ;
using quant_vals_cache_t = ::std::unordered_map<::std::string, const ::romp::QuantExpansion>> ;
using quant_vals_cache_iter_t = ::romp::quant_vals_cache_t::iterator ;
using vec_quant_vals_t = ::std::vector<const ::romp::quant_vals_cache_iter_t> ;
using quant_vals_iter_t = ::romp::vec_quant_vals_t::iterator ;
// typedef ::std::vector<const ::romp::Sig::Param> vec_MTypeVals_t;
// typedef ::romp::vec_MTypeVals_t::iterator QuantExpansion_Iter_t;
// typedef ::romp::vec_MTypeVals_t::iterator SigParam_Iter_t;
// typedef ::std::unordered_map<::std::string, const ::romp::QuantExpansion>> quant_vals_cache_t;
// typedef ::romp::quant_vals_cache_t::iterator quant_vals_cache_iter_t;
// typedef ::std::vector<const ::romp::quant_vals_cache_iter_t> vec_quant_vals_t;
// typedef ::romp::vec_quant_vals_t::iterator quant_vals_iter_t;


struct Sig {
  size_t index;
  std::vector<const Param> params;
  SigParam_Iter_t begin() const { return params.begin(); }
  SigParam_Iter_t end() const { return params.end(); }
  std::string to_string() const {
    std::strstream buf;
    buf << perm.rule->name << "(";
    std::string sep;
    //TODO: figure out what ot do here (probs manual json output)
  }
private:
  const SigPerm& parent;

};

struct SigParam {
  rumur::Ptr<const rumur::TypeExprID> type;
  std::string type_id;
  size_t value;
  std::string value_str;
  std::string to_string() const { return "(" + type->to_string() + ") " + value_str; }
private:
  const QuantExpansion& qe;
};

struct QuantExpansion {
  vec_MTypeVals_t values;
  const rumur::Ptr<const rumur::TypeExpr> type;
  size_t start;
  size_t stop;
  size_t step = 1;
  size_t size() const { return (_size > 0) ? _size : (_size = (stop-start)/step); }
  QuantExpansion_Iter_t begin() const { values.begin(); }
  QuantExpansion_Iter_t end() const { values.end(); }
  QuantExpansion(const Quantifier& q) {
    if (q.type == nullptr) {
      start = q.from->constant_fold().get_ui();
      stop = q.to->constant_fold().get_ui();
      step = (q.step == nullptr) ? 1ul : q.step->constant_fold().get_ui();
    } else {
      start = 
    }
  }
private:
  size_t _size = 0;
};

class SigPerm {
// << =================================== Member Variables ===================================== >> 
private:

protected:
  static quant_vals_cache_t quant_vals_cache;
  vec_quant_vals_iter_t quant_vals_iter;
  

public:
  const rumur::Ptr<const rumur::Rule> rule; // the "ruleset" we are creating all the signatures for
  const size_t size;
  const unsigned int param_count;


// << ============================= Constructors & Deconstructor =============================== >> 
private:
protected:
public:
  SigPerm(const rumur::Ptr<const rumur::Rule> rule_) 
    : rule(&rule_), quant_vals_iter(rule.quantifiers.size()), param_count(rule.quantifiers.size())
  {
    add_quants(rule.quantifiers);
  }


// << =================================== Member Functions ===================================== >> 
private:
  void add_quants(const std::vector<const rumur::Ptr<const rumur::Quantifier>>& qs) {
    for (auto q : qs)
      add_quant(*q);
  }

  void add_quant(const rumur::Quantifier& q) {
    quant_vals_cache_iter_t qe_i = SigPerm::quant_vals_cache.find(q.type->to_string());
    QuantExpansion& qe;
    if (qe_i == SigPerm::quant_vals_cache.end()) {
      SigPerm::add_quant(q);
      qe = SigPerm::quant_vals_cache[q.type->to_string()];
    } else
      qe = *qe_i; 
    quant_vals.push_back(qe->begin());
    size *= qe.size();
  }

  static void add_quant(const rumur::Quantifier& q) {
    //TODO: actually extrapolate all possible values for a specific quantifier
  }
  
  std::vector<QuantExpansion_Iter_t> get_init_param_iters() {
    std::vector<QuantExpansion_Iter_t> param_iters(param_count);
    for (auto quant_val : quant_vals)
      param_iters.push_back(quant_val);
    return param_iters;
  }

protected:
  


public:
  size_t size() const {}
  Iterator begin() const { return Iterator(*this); }
  Iterator end() const { return Iterator(size(), *this); }


// << =============================== Nested Classes & Structs ================================= >> 
protected:

public:
  


  struct Iterator {
    const Sig operator*() const { return *sig_ptr; }
    const Sig const* operator->() const { return sig_ptr; }
    // prefix iterator
    Iterator& operator ++ () { increment_item(); return *this; } 
    // postfix iterator
    Iterator operator ++ () { Iterator cpy = *this; ++(*this); return cpy; }
    friend bool operator == (const Iterator& l, const Iterator& r) { return  (l.perm.rule.name) ==  (r.perm.rule.name) && l.index == r.index; }
    friend bool operator != (const Iterator& l, const Iterator& r) { return  (l.perm.rule.name) !=  (r.perm.rule.name) || l.index != r.index; }
    std::string to_string() const { sig_ptr->to_string(); }
  protected:
    void increment_item() {
      increment_param_iters();
      if (index == perm.size) return;
      Sig* old = sig_ptr;
      vec_MTypeVals_t params(perm.param_count);
      for (auto param_i : param_iters)
        params.push_back(*param_i);
      sig_ptr = new Sig{++index, params, perm};
      delete old;
    }
    void increment_param_iters(size_t level=perm.param_count-1) {
      if (level < 0) { index = perm.size; return; }
      param_iters[level]++;
      if (param_iters[level] != perm.quant_vals[level].end()) return;
      param_iters[level] = perm.quant_vals[level].begin();
      increment_param_iters(--level);
    }
  private:
    size_t index;
    Sig* sig_ptr;
    const SigPerm& perm;
    std::vector<QuantExpansion_Iter_t> param_iters;
    // construct a "begin" iterator (ONLY)
    Iterator(const SigPerm& perm) 
      : index(0ul), perm(perm), param_iters(perm.get_init_param_iters()) 
    {}
    // construct an "end" iterator (ONLY)
    Iterator(size_t index, const SigPerm& perm) : index(perm.size), perm(perm) {}
    ~Iterator() { delete sig_ptr; }
  };
};


}
