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
#include "NestedError.hpp"
#include "type_traverse.hpp"
#include "romp_def.hpp"
#include <cstddef>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <strstream>

namespace romp {
class CGenerator;
class SigPerm;
struct Sig;
struct SigParam;
struct QuantExpansion;


//TODO fix this mess !!
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


// << ========================================================================================== >> 
// <<                                            Sig                                             >> 
// << ========================================================================================== >> 
#pragma region class_Sig
struct Sig {
  size_t index;
  std::vector<const SigParam&> params;
  SigParam_Iter_t begin() const { return params.begin(); }
  SigParam_Iter_t end() const { return params.end(); }
  std::string to_string() const {

  }
  // std::string to_json() const {  
  //   std::strstream buf;
  //   buf << "{\"$type\":\""+ perm.rule_type +"\","
  //           "\"id\":\"" + perm.rule->name + "\","
  //           "\"quantifiers\":[";
  //   std::string sep;
  //   for (int i=0; i<perm.param_count; ++i) {
  //     buf << sep << "{\"$type\":\"quantifier\","
  //                   "\"id:\"" << perm.quantifiers[i]->name << "\","
  //                   "\"value\":" << params[i] << "}";
  //     sep = ",";
  //   }
  //   buf << "]}";
  //   return buf.str(); 
  // }
private:
  const SigPerm& perm;
  friend std::ostream& operator << (std::ostream& out, const Sig& sig);
  friend CGenerator& CGenerator::operator << (const Sig& sig);
};
#pragma endregion class_Sig


// << ========================================================================================== >> 
// <<                                         SigParam                                           >> 
// << ========================================================================================== >> 
#pragma region class_SigParam
struct SigParam {
  const mpz_class value;
  const std::string value_str;
  const std::string str_rep;
  const std::string json_rep;
  const QuantExpansion& qe;
  const std::string& to_string() const {return str_rep;}
  const std::string& to_json() const {return json_rep;}
private:
  friend struct QuantExpansion;
  static const std::string to_string(const std::string value_str&, const QuantExpansion& qe) { 
    return "(::" ROMP_TYPE_NAMESPACE "::" + qe.type_id + ") " + value_str; }
  static const std::string to_json() const { return "{\"type\":\"" + qe.type->to_string() + "\","
                                                      "\"value\":""\"" + value_str +"\"}"; }
  friend std::ostream& operator << (std::ostream& out, const SigParam& param);
  friend CGenerator& CGenerator::operator << (const SigParam& param);
};
#pragma endregion class_SigParam


// << ========================================================================================== >> 
// <<                                      QuantExpansion                                        >> 
// << ========================================================================================== >> 
#pragma region class_QuantExpansion
struct QuantExpansion {
  vec_MTypeVals_t values;
  const rumur::Ptr<const rumur::TypeExpr> type;
  const std::string type_id;
  mpz_class start;
  mpz_class stop;
  mpz_class step = 1_mpz;
  size_t size() const { return (_size > 0_mpz) ? _size : (_size = ((abs(stop-start)+1_mpz)/abs(step))); }
  QuantExpansion_Iter_t begin() const { values.begin(); }
  QuantExpansion_Iter_t end() const { values.end(); }
  QuantExpansion(const Quantifier& q) 
    : type(q.decl->type->resolve())
  {
    if (auto _tid = dynamic_cast<const rumur::TypeExprID*>(q.decl->type.get()))
      type_id = "::" ROMP_TYPE_NAMESPACE "::" + _tid->name;
    else
      throw Error("Unprocessed anonymous type found in ruleset quantifier!!\t[dev-error]",q.type->loc);
    if (q.type == nullptr)
      resolve_quantifier_bounds(q);
    else
      resolve_quantifier_type(q);
  }
private:
  mpz_class _size = 0_mpz;
  void resolve_quantifier_bounds(const rumur::Quantifier& q) {
    // mpz_class start_mpz, stop_mpz, step_mpz;
    try { 
      start/* _mpz */ = q.from->constant_fold();
      stop/* _mpz */ = q.to->constant_fold();
      if (q.step != nullptr)
        step/* _mpz */ = q.step->constant_fold();
    } catch (rumur::Error& er) { 
      throw rumur::NestedError("You can't have a Ruleset's Quantifier dependent on a variable or a subset of an Enum !!", q.loc, er); 
    }
    if (not start/* _mpz */.fits_slong_p())
      throw Error("Couldn't resolve the value of the Expression.", q.from->loc);
    if (not stop/* _mpz */.fits_slong_p())
      throw Error("Couldn't resolve the value of the Expression.", q.to->loc);
    if (q.step != nullptr && not step/* _mpz */.fits_slong_p())
      throw Error("Couldn't resolve the value of the Expression (step).", q.step->loc);
    // start = start_mpz.get_ui();
    // stop = stop_mpz.get_ui();
    // if (q.step != nullptr)
    //   step = step_mpz.get_ui();
    values = std::vector<SigParam>(qe.size());
    for (mpz_class i = start; i<=stop, i += step)
      qe.values.push_back(SigParam{
                            i,
                            i.get_str()
                          });
  }
  void resolve_quantifier_type(const rumur::Quantifier& q_) {
    class type_trav : public rumur::ConstBaseTypeTraversal {
      const rumur::Quantifier& q;
      QuantExpansion& qe;
    public:
      type_dispatcher(const rumur::Quantifier& q__, QuantExpansion& parent_) 
        : q(q__), qe(parent_),
          ConstBaseTypeTraversal("Not a supported TypeExpr for bounding a quantifier (it may be undefined or too complex) !!") 
      {}
      void visit_array(const rumur::Array& n) { unsupported_traversal("rumur::Array"); }
      void visit_record(const rumur::Record& n) { unsupported_traversal("rumur::Record"); }
      void visit_typeexprid(const rumur::TypeExprID& n) { unsupported_traversal("undefined rumur::TypeExprID");; }
      void visit_enum(const rumur::Enum& n) { 
        qe.start = mpz_class(0ul);
        qe.stop = mpz_class(n.members.size()) - 1_mpz;
        qe._size = n.members.size();
        qe.values = std::vector<SigParam>(n.members.size());
        for (int i=0; i<n.members.size(), ++i)
          qe.values.push_back(SigParam{
                                mpz_class(i),
                                qe.type_id + "::" + n.members[i].first
                              });
      }
      void visit_range(const rumur::Range& n) {
        qe.start = n.min->constant_fold();
        qe.stop = n.max->constant_fold();
        qe.values = std::vector<SigParam>(qe.size());
        for (mpz_class i = qe.start; i<=qe.stop, i += qe.step)
          qe.values.push_back(SigParam{
                                i,
                                i.get_str()
                              });
      }
      void visit_scalarset(const rumur::Scalarset& n) {
        qe.start = 0_mpz;
        qe.stop = n.bound->constant_fold() - 1_mpz;
        qe.values = std::vector<SigParam>(qe.size());
        for (mpz_class i = qe.start; i<=qe.stop, i += qe.step)
          qe.values.push_back(SigParam{
                                i,
                                i.get_str()
                              });
      }
    };
    type_trav tt(q_, *this);
    try {
      tt.dispatch(*(q_.type->resolve()));
    } catch (rumur:Error& er) {
      throw NestedError("Could not resolve the bounds of the Type based Quantifier !!", q_.loc, er);
    }
  }
};
#pragma endregion class_QuantExpansion

// << ========================================================================================== >> 
// <<                                          SigPerm                                           >> 
// << ========================================================================================== >> 
#pragma region class_SigPerm
class SigPerm {
private:

protected:
  static quant_vals_cache_t quant_vals_cache;
  vec_quant_vals_iter_t quant_vals_iter;
  

public:
  const rumur::Ptr<const rumur::Rule> rule; // the "ruleset" we are creating all the signatures for
  const std::string rule_type;
  const std::vector<const rumur::Quantifier>& quantifiers;
  const size_t size;
  const size_t param_count;


// << ============================= Constructors & Deconstructor =============================== >> 
private:
protected:
  SigPerm(const rumur::Rule rule_, const char* rule_type_) 
    : rule_type(rule_type_), rule(&rule_), quant_vals_iter(rule_.quantifiers.size()), 
      param_count(rule_.quantifiers.size()), quantifiers(rule_.quantifiers)
  {
    add_quants(rule.quantifiers);
  }
public:
  SigPerm(const rumur::SimpleRule rule_) : SigPerm(rule_, "Rule") {}
  SigPerm(const rumur::StartState rule_) : SigPerm(rule_, "StartState") {}
  SigPerm(const rumur::PropertyRule rule_) : SigPerm(rule_, "Invariant") {}

// << =================================== Member Functions ===================================== >> 
private:
  void add_quants(const std::vector<const rumur::Ptr<const rumur::Quantifier>>& qs) {
    for (auto q : qs)
      add_quant(*q);
  }

  void add_quant(const rumur::Quantifier& q) {
    if (auto _tid = dynamic_cast<const rumur::TypeExprID*>(q.decl->type.get())) {
      quant_vals_cache_iter_t qe_i = SigPerm::quant_vals_cache.find(_tid->name);
      QuantExpansion& qe;
      if (qe_i == SigPerm::quant_vals_cache.end()) {
        SigPerm::add_quant(_tid->name, q);
        qe = SigPerm::quant_vals_cache[_tid->name];
      } else
        qe = *qe_i; 
      quant_vals.push_back(qe->begin());
      size *= qe.size();
    } else
      throw Error("Unprocessed anonymous type found in ruleset quantifier!!\t[dev-error]", q.type->loc);
  }

  static void add_quant(const std::string& name, const rumur::Quantifier& q) {
    SigPerm::quant_vals_cache.insert(name, QuantExpansion(q));
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


protected:

public:
  
  // << ========================================================================================== >> 
  // <<                                     SigPerm::Iterator                                      >> 
  // << ========================================================================================== >> 

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
#pragma endregion class_SigPerm



}
