/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file c_prefix.cpp
 * 
 * @brief The header for romp generated C code (prefixes the generated code)
 * 
 * @date 2022/07/06
 * @version 0.1
 */

// #include <assert.h>
// #include <stdbool.h>
#include <cstddef>
#include <cstdio>
// #include <stdlib.h>
#include <string>
#include <fstream>
#include <strstream>
#include <exception>
#include <vector>
#include <memory>
#include <utility>
#include <type_traits>



// << =============================== Preprocessor Declarations ================================ >>

#ifndef __romp__GENERATED_CODE
#include "romp-rw_pregen-fix.hpp" // FOR PRE-CODEGEN LANGUAGE SUPPORT ONLY !! 
// #include "romp-json.hpp" // FOR PRE-CODEGEN LANGUAGE SUPPORT ONLY !! 
#endif


// various printf wrappers to deal with the user having passed --value-type
// static __attribute__((unused)) void print_int     (int v)      { printf("%d",          v); }
// static __attribute__((unused)) void print_unsigned(unsigned v) { printf("%u",          v); }
// static __attribute__((unused)) void print_short   (short v)    { printf("%hd",         v); }
// static __attribute__((unused)) void print_long    (long v)     { printf("%ld",         v); }
// static __attribute__((unused)) void print_int8_t  (int8_t v)   { printf("%" PRId8 , v); }
// static __attribute__((unused)) void print_uint8_t (uint8_t v)  { printf("%" PRIu8 , v); }
// static __attribute__((unused)) void print_int16_t (int16_t v)  { printf("%" PRId16, v); }
// static __attribute__((unused)) void print_uint16_t(uint16_t v) { printf("%" PRIu16, v); }
// static __attribute__((unused)) void print_int32_t (int32_t v)  { printf("%" PRId32, v); }
// static __attribute__((unused)) void print_uint32_t(uint32_t v) { printf("%" PRIu32, v); }
// static __attribute__((unused)) void print_int64_t (int64_t v)  { printf("%" PRId64, v); }
// static __attribute__((unused)) void print_uint64_t(uint64_t v) { printf("%" PRIu64, v); }

// wrappers for producing literal expressions of value type
#define int_VALUE_C(v)      (v)
#define unsigned_VALUE_C(v) (v ## u)
#define short_VALUE_C(v)    ((short)(v))
#define long_VALUE_C(v)     (v ## l)
#define int8_t_VALUE_C(v)   INT8_C(v)
#define uint8_t_VALUE_C(v)  UINT8_C(v)
#define int16_t_VALUE_C(v)  INT16_C(v)
#define uint16_t_VALUE_C(v) UINT16_C(v)
#define int32_t_VALUE_C(v)  INT32_C(v)
#define uint32_t_VALUE_C(v) UINT32_C(v)
#define int64_t_VALUE_C(v)  INT64_C(v)
#define uint64_t_VALUE_C(v) UINT64_C(v)


namespace romp {
  struct RuleInfo; struct PropertyInfo; struct StartStateInfo; struct MErrorInfo; struct FunctInfo;
  struct RuleSet; struct StartState; struct Property;
}

namespace __caller__ {
  extern const ::romp::RuleSet RULESETS[];
  extern const ::romp::StartState STARTSTATES[];
  extern const ::romp::Property PROPERTIES[];
}

namespace __info__ { // LANGUAGE SERVER SUPPORT ONLY!!
  extern const ::romp::RuleInfo RULE_SET_INFOS[];
  extern const ::romp::PropertyInfo PROPERTY_INFOS[]; 
  extern const ::romp::StartStateInfo STARTSTATE_INFOS[]; 
  extern const ::romp::MErrorInfo ERROR_INFOS[]; 
  extern const ::romp::FunctInfo FUNCT_INFOS[]; 
}

namespace romp {

  typedef _ROMP_STATE_TYPE State_t;

  template <class O>
  struct ojstream {
    static_assert(std::is_base_of<std::ostream, O>::value, "O must inherit from std::ostream");
    O out;
  protected: ojstream(O out_) : out(out_) {}
  public:
    ~ojstream() { out.close(); }
    template<typename... Args> ojstream(Args &&...args) 
      : out(O(std::forward<Args>(args)...)) {}
    template<typename T> 
    // friend ojstream<O>& operator << (ojstream<O>& json, const T& val);
    ojstream<O>& operator << (const std::string& str) { out << str; return *this; }
    ojstream<O>& operator << (const char* str) { out << str; return *this; }
    ojstream<O>& operator << (const unsigned char val) { out << val; return *this; }
    ojstream<O>& operator << (const char val) { out << val; return *this; }
    ojstream<O>& operator << (const unsigned long val) { out << val; return *this; }
    ojstream<O>& operator << (const long val) { out << val; return *this; }
    ojstream<O>& operator << (const unsigned int val) { out << val; return *this; }
    ojstream<O>& operator << (const int val) { out << val; return *this; }
    ojstream<O>& operator << (const unsigned short val) { out << val; return *this; }
    ojstream<O>& operator << (const short val) { out << val; return *this; }
    ojstream<O>& operator << (const unsigned short val) { out << val; return *this; }
    ojstream<O>& operator << (const short val) { out << val; return *this; }
    ojstream<O>& operator << (const bool val) { out << (val) ? "true" : "false"; return *this; }
  };

  typedef ojstream<std::ofstream> json_file_t;
  typedef ojstream<std::strstream> json_str_t;

  template<> char* json_str_t::str() { return this->str(); }

  struct file_position {
    size_t row;
    size_t col;
  };
  template<class O>
  ojstream<O>& operator << (ojstream<O>& json, const file_position& fp) { 
    return (json << '[' << fp.row << ',' << fp.col << ']'); 
  }
  std::ostream& operator << (std::ostream& out, const file_position& fp) { return (out << fp.row << ',' << fp.col); }

  struct location {
    // std::string model_obj;
    file_position start;
    file_position end;
  };
  template<class O>
  ojstream<O>& operator << (ojstream<O>& json, const location& loc) { 
    return (json << "{\"$type\":\"location\","
            << "\"file\":\"" __model__filename "\","
            // << (loc.model_obj != "") ? "\"inside\":\""+loc.model_obj+"\"," : ""
            << "start" << ':' << loc.start << ',' 
            << "\"end\":" << loc.end
            << '}'); 
  }
  std::ostream& operator << (std::ostream& out, const location& loc) { 
    out << ((__model__filename_contains_space) ? "\""__model__filename "\":" : __model__filename ":") 
        << loc.start << '-' << loc.end;
    // if (loc.model_obj != "")
    //   out << " in ``" << loc.model_obj << "``";
    return out; 
  }

  // struct IInfo {
  //   virtual void to_json(json_file_t& json) { this->to_json(json.out); }
  //   virtual void to_json(json_str_t& json) { this->to_json(json.out); }
  //   virtual void to_string(std::ostream& out) = 0;
  // protected:
  //   virtual void to_json(std::ostream& json) = 0;
  // };

  typedef enum { INVARIANT, ASSERTION, ASSUMPTION, COVER, LIVENESS } PropertyType;
  std::ostream& operator << (std::ostream& out, const PropertyType& val) { 
    switch (val) { 
      case PropertyType::ASSERTION: return out << "assert"; 
      case PropertyType::ASSUMPTION: return out << "assume";
      case PropertyType::COVER: return out << "cover";
      case PropertyType::LIVENESS: return out << "liveness";
      case PropertyType::INVARIANT: return out << "invariant";
      default: return out << "UNKNOWN";}
   }
  template<class O> ojstream<O>& operator << (ojstream<O>& json, const PropertyType& val) { json.out << val; return json; }

  struct PropertyInfo {
    PropertyType type;
    std::string label;
    std::string expr;
    // size_t prop_id;
    id_t id;
    location loc;
    std::string json_h; // missing closing '}' so you can insert other fields if necessary
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const PropertyInfo& pi) noexcept { return (json << pi.json_h << '}'); }
  std::ostream& operator << (std::ostream& out, const PropertyInfo& pi) noexcept { return (out << pi.type << " \"" << pi.name << "\" " << pi.expr << " @(" << pi.loc << ")"); }

   struct Property {
    bool (*check)(const State_t&) throw (ModelError);
    const PropertyInfo& info;
    const std::string quant_json;
    const std::string quant_str;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const Property& p) noexcept { return (json << p.info.json_h << ",\"quantifiers\":" << p.quants_json << '}'); }
  std::ostream& operator << (std::ostream& out, const Property& p) noexcept { 
    out << p.info.type << " \"" << p.info.label << "\" ";
    if (p.quant_str.size() > 0)
      out << " Quantifiers(" << p.quant_str << ") ";
    return (out << "@(" << p.info.loc << ')'); 
  }

  struct RuleInfo {
    std::string label;
    location loc;
    std::string json_h; 
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const RuleInfo& ri) noexcept { return (json << ri.json_h << '}'); }
  std::ostream& operator << (std::ostream& out, const RuleInfo& ri) noexcept { return (out << "rule \""<< ri.label << "\" @(" << ri.loc << ")"); }

  struct Rule {
    bool (*guard)(const State_t&) throw (ModelError);
    void (*action)(State_t&) throw (ModelError);
    RuleInfo& info;
    const std::string quant_json;
    const std::string quant_str;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const Rule& r) noexcept { return (json << r.info.json_h << ",\"quantifiers\":" << r.quant_json << '}'); }
  std::ostream& operator << (std::ostream& out, const Rule& r) noexcept { 
    out << "rule \"" << r.info.label << "\" ";
    if (r.quant_str.size() > 0)
      out << " Quantifiers(" << r.quant_str << ") ";
    return (out << "@(" << r.info.loc << ')');
  }

  struct RuleSet {
    RuleInfo& info;
    std::vector<Rule> rules;
  }; 

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const RuleSet& rs) noexcept { return (json << rs.info); }
  std::ostream& operator << (std::ostream& out, const RuleSet& rs) noexcept { return (out << rs.info); }

  struct StartStateInfo {
    const std::string label;
    location loc;
    const std::string json_h;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const StartStateInfo& si) noexcept { return (json << si.json_h << '}'); }
  std::ostream& operator << (std::ostream& out, const StartStateInfo& si) noexcept { return (out << "startstate \""<< si.label << "\" @(" << si.loc << ")"); }

  struct StartState {
    void (*initialize)(State_t&) throw (ModelError);
    StartStateInfo& info;
    const std::string quant_json;
    const std::string quant_str;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const StartState& s) noexcept { return (json << s.info.json_h << ",\"quantifiers\":" << s.quant_json << '}'); }
  std::ostream& operator << (std::ostream& out, const StartState& s) noexcept {
    out << "startstate \"" << s.info.label << "\" ";
    if (s.quant_str.size() > 0)
      out << " Quantifiers(" << s.quant_str << ") ";
    return (out << "@(" << s.info.loc << ')');
  }

  struct MErrorInfo {
    const std::string label;
    location loc;
    bool isFunct;
    const std::string json;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const MErrorInfo& ei) noexcept { return (json << ei.json_h); }
  std::ostream& operator << (std::ostream& out, const MErrorInfo& ei) noexcept { return (out << "error \""<< ei.label << "\" @(" << ei.loc << ")"); }
  
  struct FunctInfo {
    const std::string label;
    location loc;
    const std::string json;
    const std::string signature;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const FunctInfo& fi) noexcept { return (json << ei.json_h); }
  std::ostream& operator << (std::ostream& out, const FunctInfo& fi) noexcept { return (out << fi.signature << " @(" << ei.loc << ")"); }

  enum ModelObjType { RULE, STARTSTATE, PROPERTY, FUNCTION, ERROR };

  struct ModelError : public std::logic_error {
    const ModelObjType type;
    const id_t id;
    const id_t qid;
    ModelError(ModelObjType type_, id_t id_, id_t qid_) : type(type_), id(id_), qid(quid_) {}
    virtual ModelError* clone() { return new ModelError(*this); }
    const char* what() const noexcept {
      std::strstream out;
      this->what(out);
      return out.str();
    }
    virtual void what(std::ostream& out) const noexcept {
      switch (type) {
        case ModelObjType::RULE:
          out << ::__info__::RULE_SET_INFOS[id];
          return;
        case ModelObjType::STARTSTATE:
          out << ::__info__::STARTSTATE_INFOS[id];
          return;
        case ModelObjType::PROPERTY:
          out << ::__info__::STARTSTATE_INFOS[id];
          return;
        
      }
    }
  };

  struct ModelPropertyError : public ModelError {
    const PropertyInfo& prop_info;
    ModelPropertyError(const PropertyInfo& prop_)
      : ModelError("",prop_.info.loc), prop(prop_) {}
    virtual ModelError* clone() { return new ModelPropertyError(*this); }
    void what(std::ostream& out) const noexcept {
      out << prop;
    } 
  };
  

  template<class O> void __jprint_exception(ojstream<O>& json, const std::exception& ex) noexcept;
  template<class O> ojstream<O>& operator << (ojstream<O>& json, const ModelPropertyError& ex) noexcept { 
    json << "{\"$type\":\"romp::ModelPropertyError\","
             "\"loc\":" << ex.loc << ","
             "\"property:\":{" 
                 "\"$type\":\"property\",\"property-type\":\"" << ex.prop_type << "\","
                 "\"label\":\"" << ex.prop.info.name "\","
                 "\"test-expr\":\"" << ex.test_str << "\"},"
                 "\"what\":\""; ex.what(json.out); json << '"';
    __jprint_exception(json, ex);
    return (json << '}');
  }
  template<class O> ojstream<O>& operator << (ojstream<O>& json, const ModelError& ex) noexcept { 
    json << "{\"$type\":\"romp::ModelError\","
             "\"loc\":" << ex.loc << ","
             "\"message\":\"" << ex.msg << "\","
             "\"what\":\""; ex.what(json.out); json << '"';
    __jprint_exception(json, ex);
    return (json << '}');
  }
  template<class O> ojstream<O>& operator << (ojstream<O>& json, const std::runtime_error& ex) noexcept {
    json << "{\"$type\":\"std::runtime_error\","
             "\"what\":\"" << me.what() << "\"";
    __jprint_exception(json, ex);
    return (json << '}');
  }
  template<class O> ojstream<O>& operator << (ojstream<O>& json, const std::logic_error& ex) noexcept {
    json << "{\"$type\":\"std::logic_error\","
             "\"what\":\"" << me.what() << "\"";
    __jprint_exception(json, ex);
    return (json << '}');
  }
  template<class O> ojstream<O>& operator << (ojstream<O>& json, const std::exception& ex) noexcept {
    json << "{\"$type\":\"std::exception\","
             "\"what\":\"" << me.what() << "\"";
    __jprint_exception(json, ex);
    return (json << '}');
  }
  template<class O> void __jprint_exception(ojstream<O>& json, const std::exception& ex) noexcept {
    try {
        std::rethrow_if_nested(ex);
        json << ",\"cause\":null";
    } catch(const ModelPropertyError& mod_test_ex) {
      json << ",\"cause\":" << mod_test_ex;
    } catch(const ModelError& mod_ex) {
      json << ",\"cause\":" << mod_ex;
    } catch(const std::runtime_error& re) {
      json << ",\"cause\":" << re;
    } catch(const std::logic_error& le) {
      json << ",\"cause\":" << le;
    } catch(const std::exception& ex) {
      json << ",\"cause\":" << ex;
    } catch(...) {
      json << ",\"cause\":\"unknown\"";
    }
  }
 
#define __romp__nested_exception__print_prefix '|'

  void fprint_exception(std::ostream& out, const ModelError& ex, const size_t level) noexcept;
  void fprint_exception(std::ostream& out, const std::exception& ex, const size_t level) noexcept;

  void fprint_exception(std::ostream& out, const ModelError& ex, const size_t level) noexcept {
    out << std::string(level, __romp__nested_exception__print_prefix);
    ex.what(out);
    out << '\n';
    try {
        std::rethrow_if_nested(ex);
    } catch(const ModelError& mod_ex) {
      fprint_exception(out, mod_ex, level+1);
    } catch(const std::exception& ex) {
      fprint_exception(out, ex, level+1);
    } catch(...) {}
  }

  void fprint_exception(std::ostream& out, const std::exception& ex, const size_t level) noexcept {
    out << std::string(level, __romp__nested_exception__print_prefix)
        << ex.what() << '\n';
    try {
        std::rethrow_if_nested(ex);
    } catch(const ModelError& mod_ex) {
      fprint_exception(out, mod_ex, prefix + __romp__nested_exception__print_prefix);
    } catch(const std::exception& ex) {
      fprint_exception(out, ex, prefix + __romp__nested_exception__print_prefix);
    } catch(...) {}
  }

  std::ostream& operator << (std::ostream& out, const ModelError& ex) noexcept {
    fprint_exception(out, ex, 0);
    return out;
  }

  std::ostream& operator << (std::ostream& out, const std::exception& ex) noexcept {
    fprint_exception(out, ex, 0);
    return out;
  }


  class IRandWalker {
    /**
     * @brief the handler the model will call when it reaches an inline `assert`/`invariant` statement
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the model calling this function should return an error or not.  ( \c true - do NOT, \c false DO)
     */
    virtual bool assertion_handler(bool eval_expr, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call when it reaches a global rule `assert`/`invariant`
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the caller should report an error. ( \c true - do NOT, \c false DO)
     */
    virtual bool invariant_handler(bool eval_expr, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call when it reaches an `assumption` statement
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - whether or not the caller should report an error. ( \c true - do NOT, \c false DO)
     */
    virtual bool assumption_handler(bool eval_expr, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call when a `cover` statement is reached
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param cover_id the id given to this specific `cover` statement (value based upon the order it was processed by the code gen tool)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the caller should report an error. ( \c true - do NOT, \c false DO)
     */
    virtual bool cover_handler(bool eval_expr, id_t cover_id, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call when a `liveness` statement is reached
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param liveness_id the id given to this specific `liveness` statement (value based upon the order it was processed by the code gen tool)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the caller should report an error. ( \c true - do NOT, \c false DO)
     */
    virtual bool liveness_handler(bool eval_expr, id_t liveness_id, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call it reaches an `error` statement
     * @param error_id the id given to the error statement (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the caller should report an error. ( \c true - DO, \c false do NOT)
     */
    virtual bool error_handler(id_t error_id) = 0;
    friend State_t;
  };

}

namespace __type__ {
  typedef bool boolean; // builtin type mask for Murphi's boolean
}
