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
#include "../../common/escape.h"

namespace romp {

  // << ========================================================================================== >> 
  // <<                                            SIG                                             >> 
  // << ========================================================================================== >> 

  std::vector<std::shared_ptr<const SigParam>>::iterator Sig::begin() const { return params.begin(); }
  std::vector<std::shared_ptr<const SigParam>>::iterator Sig::end() const { return params.end(); }
  // std::vector<const SigParam&>::iterator Sig::begin() const { return params.begin(); }
  // std::vector<const SigParam&>::iterator Sig::end() const { return params.end(); }

  Sig::Sig(size_t index_, std::vector<std::shared_ptr<const SigParam>> params_, const SigPerm& perm_)
    : index(index_), params(params_), perm(perm_) {}
  // Sig::Sig(size_t index_, std::vector<const SigParam&> params_, const SigPerm& perm_)
  //   : index(index_), params(params_), perm(perm_) {}

  std::string Sig::to_string() const {
    if (perm.param_count > 0) {
    std::strstream buf;
    // buf << "quantifiers(";
    std::string sep = "";
      for (int i=0; i<perm.param_count; ++i) {
        buf << sep << perm.quantifiers[i].name << ":=" << escape(params[i]->value_str);
        // buf << sep << perm.quantifiers[i].name << ":=" << escape(params[i].value_str);
        sep = "; ";
      }
      // buf << ')';
    return buf.str(); 
    } else { return ""; }
  }

  std::string Sig::to_json() const {  
    if (perm.param_count > 0) {
    std::strstream buf;
    // buf << "\"quantifiers\":";
    std::string sep = "";
      buf << '[';
      for (int i=0; i<perm.param_count; ++i) {
        buf << sep << "{\"$type\":\"kv-pair\",\"key\":\"" << perm.quantifiers[i].name << "\",\"value\":"
                  << params[i].to_json() << '}';
        sep = ",";
      }
      buf << ']';
      return buf.str(); 
    } else { return "null"; }
  }

  // << ========================================================================================== >> 
  // <<                                         SigParam                                           >> 
  // << ========================================================================================== >> 

  const std::string& SigParam::to_string() const {return str_rep;}
  const std::string& SigParam::to_json() const {return json_rep;}

  const std::string SigParam::to_string(const std::string& value_str, const QuantExpansion& qe) { 
    return "(::" ROMP_TYPE_NAMESPACE "::" + qe.type_id + ") " + value_str; 
  }

  const std::string SigParam::to_json(const std::string& value_str, const QuantExpansion& qe, const std::string json_val_type) {
    // if (auto _tid = dynamic_cast<const rumur::TypeExprID*>(qe.type.get())) {
    //   return "{\"type\":\"" + _tid->referent->name + ": " + _tid->referent->value->to_string() + "\","
    //                 "\"value\":""\"" + value_str +"\"}";
    return "{\"$type\":\""+ escape(json_val_type) + "\",\"type\":\"" + escape(qe.type->resolve()->to_string()) + "\",\"value\":" + value_str +"}"; 
  }

  // << ========================================================================================== >> 
  // <<                                      QuantExpansion                                        >> 
  // << ========================================================================================== >> 

  size_t QuantExpansion::size() const { return _size.get_ui() ; }

  std::vector<std::shared_ptr<const SigParam>>::iterator QuantExpansion::begin() const { values.begin(); }
  std::vector<std::shared_ptr<const SigParam>>::iterator QuantExpansion::end() const { values.end(); }
  // std::vector<const SigParam>::iterator QuantExpansion::begin() const { values.begin(); }
  // std::vector<const SigParam>::iterator QuantExpansion::end() const { values.end(); }

  QuantExpansion::QuantExpansion(const rumur::Quantifier& q) 
    : type(q.decl->type)
  {
    if (auto _tid = dynamic_cast<const rumur::TypeExprID*>(q.decl->type.get()))
      type_id = _tid->referent->name;
    else
      throw rumur::Error("Unprocessed anonymous type found in ruleset quantifier!!\t[dev-error]",q.type->loc);
    if (q.type == nullptr)
      resolve_quantifier_bounds(q);
    else
      resolve_quantifier_type(q);
    _size = ((abs(stop-start)+1_mpz)/abs(step));
  }

  void QuantExpansion::resolve_quantifier_bounds(const rumur::Quantifier& q) {
    // mpz_class start_mpz, stop_mpz, step_mpz;
    try { 
      start/* _mpz */ = q.from->constant_fold();
      stop/* _mpz */ = q.to->constant_fold();
      if (q.step != nullptr)
        step/* _mpz */ = q.step->constant_fold();
    } catch (rumur::Error& er) { 
      std::throw_with_nested(rumur::Error("You can't have a Ruleset's Quantifier dependent on a variable or a subset of an Enum !!", q.loc)); 
    }
    if (not start/* _mpz */.fits_slong_p())
      throw rumur::Error("Couldn't resolve the value of the Expression.", q.from->loc);
    if (not stop/* _mpz */.fits_slong_p())
      throw rumur::Error("Couldn't resolve the value of the Expression.", q.to->loc);
    if (q.step != nullptr && not step/* _mpz */.fits_slong_p())
      throw rumur::Error("Couldn't resolve the value of the Expression (step).", q.step->loc);
    // start = start_mpz.get_ui();
    // stop = stop_mpz.get_ui();
    // if (q.step != nullptr)
    //   step = step_mpz.get_ui();
    values = std::vector<std::shared_ptr<const SigParam>>();
    for (mpz_class i = start; i<=stop; i += step)
      values.push_back(std::shared_ptr<const SigParam>(new SigParam{
                            i,
                            i.get_str(),
                            SigParam::to_string(i.get_str(), *this),
                            SigParam::to_json(i.get_str(), *this, "quantifier-value"),
                            *this
                          }));
  }

  void QuantExpansion::resolve_quantifier_type(const rumur::Quantifier& q_) {
    class type_trav : public rumur::ConstBaseTypeTraversal {
      const rumur::Quantifier& q;
      QuantExpansion& qe;
    public:
      type_trav(const rumur::Quantifier& q__, QuantExpansion& parent_) 
        : q(q__), qe(parent_),
          ConstBaseTypeTraversal("Not a supported TypeExpr for bounding a quantifier (it may be undefined or too complex) !!") 
      {}
      void visit_array(const rumur::Array& n) { unsupported_traversal(n,"rumur::Array"); }
      void visit_record(const rumur::Record& n) { unsupported_traversal(n,"rumur::Record"); }
      void visit_typeexprid(const rumur::TypeExprID& n) { unsupported_traversal(n,"undefined rumur::TypeExprID");; }
      void visit_enum(const rumur::Enum& n) { 
        qe.start = mpz_class(0ul);
        qe.stop = mpz_class(n.members.size()) - 1_mpz;
        qe._size = n.members.size();
        qe.values = std::vector<std::shared_ptr<const SigParam>>(qe.size());
        for (int i=0; i<n.members.size(); ++i) {
          std::string val = qe.type_id + "::" + n.members[i].first;
          qe.values.push_back(std::shared_ptr<const SigParam>(new SigParam{
                                mpz_class(i),
                                val,
                                "::" ROMP_TYPE_NAMESPACE "::" + val,
                                SigParam::to_json(escape("\""+n.members[i].first+"\""), qe, "enum-value"),
                                // "{\"type\":\"" + qe.type_id + "\","
                                //   "\"value\":\"" + n.members[i].first + "\"}",
                                qe
                              }));
        }
      }
      void visit_range(const rumur::Range& n) {
        qe.start = n.min->constant_fold();
        qe.stop = n.max->constant_fold();
        qe.values = std::vector<std::shared_ptr<const SigParam>>(qe.size());
        for (mpz_class i = qe.start; i<=qe.stop; i += qe.step)
          qe.values.push_back(std::shared_ptr<const SigParam>(new SigParam{
                                i,
                                i.get_str(),
                                SigParam::to_string(i.get_str(), qe),
                                SigParam::to_json(i.get_str(), qe, "range-value"),
                                qe
                              }));
      }
      void visit_scalarset(const rumur::Scalarset& n) {
        qe.start = 0_mpz;
        qe.stop = n.bound->constant_fold() - 1_mpz;
        qe.values = std::vector<std::shared_ptr<const SigParam>>(qe.size());
        for (mpz_class i = qe.start; i<=qe.stop; i += qe.step)
          qe.values.push_back(std::shared_ptr<const SigPerm>(new SigParam{
                                i,
                                i.get_str(),
                                SigParam::to_string(i.get_str(), qe),
                                SigParam::to_json(i.get_str(), qe, "scalarset-value"),
                                qe
                              }));
      }
    };
    type_trav tt(q_, *this);
    try {
      tt.dispatch(*(q_.type->resolve()));
    } catch (rumur::Error& er) {
      std::throw_with_nested(rumur::Error("Could not resolve the bounds of the Type based Quantifier !!", q_.loc));
    }
  }


  // << ========================================================================================== >> 
  // <<                                          SigPerm                                           >> 
  // << ========================================================================================== >> 

  SigPerm::SigPerm(const rumur::Ptr<const rumur::Rule> rule_, const char* rule_type_) 
    : rule_type(rule_type_), rule(rule_), quant_vals(rule_->quantifiers.size()), 
      param_count(rule_->quantifiers.size()), quantifiers(rule_->quantifiers)
  {
    add_quants(rule->quantifiers);
  }

  void SigPerm::add_quants(const std::vector<rumur::Quantifier>& qs) {
    for (const auto& q : qs)
      add_quant(q);
  }

  void SigPerm::add_quant(const rumur::Quantifier& q) {
    if (auto _tid = dynamic_cast<const rumur::TypeExprID*>(q.decl->type.get())) {
      auto qe_i = SigPerm::quant_vals_cache.find(_tid->referent->name);
      // const QuantExpansion& qe = qe_i->second;
      if (qe_i == SigPerm::quant_vals_cache.end()) {
        SigPerm::add_quant(_tid->name, q);
        quant_vals.push_back(SigPerm::quant_vals_cache[_tid->referent->name]);
      } else
        quant_vals.push_back(qe_i->second); 
      // quant_vals.push_back(qe);
      this->_size *= qe_i->second->size();
    } else
      throw rumur::Error("Unprocessed anonymous type found in ruleset quantifier!!\t[dev-error]", q.type->loc);
  }

  void SigPerm::add_quant(const std::string& name, const rumur::Quantifier& q) {
    SigPerm::quant_vals_cache.insert(std::make_pair(name, std::shared_ptr<const QuantExpansion>(new QuantExpansion(q))));
  }

  std::vector<size_t> SigPerm::get_init_param_iters() const {
    std::vector<size_t> param_iters(param_count);
    for (size_t i=0; i<param_count; ++i)
      param_iters.push_back(0);
    return param_iters;
  }
  // std::vector<std::vector<const SigParam>::iterator> SigPerm::get_init_param_iters() const {
  //   std::vector<std::vector<const SigParam>::iterator> param_iters(param_count);
  //   for (auto quant_val : quant_vals)
  //     param_iters.push_back(quant_val->begin());
  //   return param_iters;
  // }

  size_t SigPerm::size() const { return _size; }
  SigPerm::Iterator SigPerm::begin() const { return Iterator(*this, get_init_param_iters()); }
  SigPerm::Iterator SigPerm::end() const { return Iterator(*this); }

  // std::string SigPerm::to_string() const {
  // 
  // }
  


  // << ========================================================================================== >> 
  // <<                                     SigPerm::Iterator                                      >> 
  // << ========================================================================================== >> 

  const Sig SigPerm::Iterator::operator * () const { return *sig_ptr; }
  const Sig* SigPerm::Iterator::operator -> () const { return sig_ptr; }
  // prefix iterator
  SigPerm::Iterator& operator ++ (SigPerm::Iterator& it) { it.increment_item(); return it; } 
  // postfix iterator
  SigPerm::Iterator operator ++ (SigPerm::Iterator& it, int n) { SigPerm::Iterator cpy(it); ++it; return cpy; }
  /* friend */ bool operator == (const SigPerm::Iterator& l, const SigPerm::Iterator& r) { return  (l.perm.rule->name) ==  (r.perm.rule->name) && l.index == r.index; }
  /* friend */ bool operator != (const SigPerm::Iterator& l, const SigPerm::Iterator& r) { return  (l.perm.rule->name) !=  (r.perm.rule->name) || l.index != r.index; }
  std::string SigPerm::Iterator::to_string() const { sig_ptr->to_string(); }

  void SigPerm::Iterator::increment_item() {
    increment_param_iters();
    if (index == perm.size()) return;
    Sig* old = sig_ptr;
    std::vector<const SigParam&> params(perm.param_count);
    for (size_t i=0; i<perm.size(); ++i)
      params.push_back(perm.quant_vals[i].values[param_iters[i]]);
    // for (auto param_i : param_iters)
    //   params.push_back(*param_i);
    sig_ptr = new Sig(++index, params, perm);
    delete old;
  }
  void SigPerm::Iterator::increment_param_iters() { increment_param_iters(perm.param_count-1); }
  void SigPerm::Iterator::increment_param_iters(size_t level) {
    if (level < 0) { index = perm.size(); return; }
    param_iters[level]++;
    if (param_iters[level] < perm.quant_vals[level].size()) return;
    param_iters[level] = 0;
    increment_param_iters(--level);
  }

  SigPerm::Iterator::Iterator(const SigPerm& perm_, std::vector<size_t> param_iters_) 
      : index(0ul), perm(perm_), param_iters(param_iters_)
    {}
  // SigPerm::Iterator::Iterator(const SigPerm& perm_, std::vector<std::vector<const SigParam&>::iterator> param_iters_) 
  //     : index(0ul), perm(perm_), param_iters(param_iters_)
  //   {}
  SigPerm::Iterator::Iterator(const SigPerm& perm_) : index(perm_.size()), perm(perm_) {}
  SigPerm::Iterator::~Iterator() { delete sig_ptr; }

}
