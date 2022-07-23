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
#include <iostream>
// #include <stdlib.h>
#include <string>
#include <string.h>  // required for memset
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <type_traits>
#include <thread>



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
  struct Options;
  extern Options options;
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

  namespace options {

    /**
     * @brief the number of concurrent threads a system supports \n
     *        \b NOTE: if \c 0 then number is unknown & user must provide with \c -ptn / \c --threads flags.
     */
    const unsigned int __system_thread_count = ::std::thread::hardware_concurrency();
    unsigned int get_default_thread_count() {
      switch (__system_thread_count) {
      case 0: return 0;
      case 1: 
      case 2:
        return 1;
      default:
        return __system_thread_count - 2;
      }
    }

    struct Options {
      size_t history_length = 4;
      bool do_trace = false;
      unsigned int threads =  get_default_thread_count(); 
      size_t depth = INT32_MAX;      
      unsigned int random_walkers = threads*_ROMP_THREAD_TO_RW_RATIO; 
      unsigned int
          rand_seed = time(NULL); 
      std::string seed_str; 
      bool do_single = false;
      bool liveness = false;
      size_t lcount = INT16_MAX;
      size_t cover_count = INT16_MAX; 
      size_t attempt_limit= 0; // disabled if 0/NULL
      std::string trace_dir = "./traces/"; // path for the trace file to be created during each walk
      bool deadlock =
          false; // separate bool for each property or consider having a valid bool
                // . depends on how property will be designed
      bool result = false; // result output
      bool result_all = false;
      bool r_assume = false;
      bool complete_on_cover= false;
      bool output_results = false;
      std::string result_out_file = "results.txt";
      bool even_start = false;
    };
  }
  options::Options OPTIONS;

  typedef _ROMP_STATE_TYPE State_t;

  static std::string octal(char c) {
    char buffer[sizeof("\\000")];
    snprintf(buffer, sizeof(buffer), "\\%03o", c);
    return buffer;
  }

  std::string escape_str(const std::string &s) {
    std::string out;
    for (const char &c : s) {
      if (iscntrl(c) || c == '\\' || c == '\"') {
        out += "\\" + octal(c);
      } else {
        out += c;
      }
    }
    return out;
  }

  struct IModelError;
  template<class O> void __jprint_exception(ojstream<O>& json, const std::exception& ex) noexcept;

  template <class O>
  struct ojstream {
    static_assert(std::is_base_of<std::ostream, O>::value, "O must inherit from std::ostream");
    O out;
  protected: ojstream(O out_) : out(out_) {}
    // int ex_level = 0;
  public:
    ~ojstream() { out << ']}\n' << std::flush; out.close(); } // probs move this to random walker
    template<typename... Args> ojstream(Args &&...args) 
      : out(O(std::forward<Args>(args)...)) {
      out << "{\"$type\":\"romp-trace\""; // probs move this to random walker
    }
    // template<typename T> 
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
    ojstream<O>& operator << (const bool val) { out << (val) ? "true" : "false"; return *this; }
    ojstream<O>& operator << (const IModelError& me) noexcept;
    ojstream<O>& operator << (const std::exception& ex) noexcept { 
      // if (ex_level++ == 0) out << "],\"error-trace\":["
      out << "{\"$type\":\"exception\","
              "\"what\":\"" << escape_str(ex.what()) << "\"}";
      __jprint_exception(*this,ex);
      // if (--ex_level == 0) out << "],";
      return *this; 
    }
    char* str() { if () }
  };


  typedef ojstream<std::ofstream> json_file_t;
  typedef ojstream<std::stringstream> json_str_t;

  // template<> char* json_str_t::str() { return out.str(); }

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

  struct IModelError : public std::logic_error {
    IModelError() : std::logic_error("this is a model error (you should never see this)") {}
    const char* what() const noexcept {
      std::stringstream out;
      this->what(out);
      return out.str();
    }
    virtual void what(std::ostream& out) const noexcept = 0;
    virtual void to_json(json_file_t& json) const noexcept = 0;
    virtual void to_json(json_str_t& json) const noexcept = 0;
  };
  template<class O>
  ojstream<O>& ojstream<O>::operator << (const IModelError& me) noexcept { 
    // if (ex_level++ == 0) out << "],\"error-trace\":["
    me.to_json(out); 
    __jprint_exception(*this,me);
    // if (--ex_level == 0) out << ']';
    return *this; 
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
    std::string label;
    PropertyType type;
    std::string expr;
    // size_t prop_id;
    id_t id;
    location loc;
    std::string json_h; // missing closing '}' so you can insert other fields if necessary
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const PropertyInfo& pi) noexcept { return (json << pi.json_h << '}'); }
  std::ostream& operator << (std::ostream& out, const PropertyInfo& pi) noexcept { return (out << pi.type << " \"" << pi.label << "\" " << pi.expr << " @(" << pi.loc << ")"); }

   struct Property {
    void (*check)(const State_t&) throw (IModelError);
    const PropertyInfo& info;
    const std::string quant_json;
    const std::string quant_str;
  };

  struct ModelPropertyError : public IModelError {
    ModelPropertyError(const Property& prop_) : isProp(true) { data.prop = &prop_; }
    ModelPropertyError(const PropertyInfo& info_) : isProp(false) { data.info = &info_; }
    ModelPropertyError(id_t id) : isProp(false) { data.info = &(::__info__::PROPERTY_INFOS[id]); }
    void what(std::ostream& out) const noexcept { if (isProp) out << *data.prop; else out << *data.info; }
    virtual void to_json(json_file_t& json) const noexcept { _to_json(json); }
    virtual void to_json(json_str_t& json) const noexcept { _to_json(json); }
  private:
    union {const Property* prop; const PropertyInfo* info;} data;
    const bool isProp;
    template<class O>
    void _to_json(ojstream<O>& json) const noexcept { 
      json << "{\"$type\":\"model-error\","
               "\"type\":\"property\","
              //  "\"what\":\""<< escape_str(what()) << "\","
               "\"inside\":";
      if (isProp) json << *data.prop; else json << *data.info;
      json << '}';
    } 
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
    bool (*guard)(const State_t&) throw (IModelError);
    void (*action)(State_t&) throw (IModelError);
    const RuleInfo& info;
    const std::string quant_json;
    const std::string quant_str;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& jso  n, const Rule& r) noexcept { return (json << r.info.json_h << ",\"quantifiers\":" << r.quant_json << '}'); }
  std::ostream& operator << (std::ostream& out, const Rule& r) noexcept { 
    out << "rule \"" << r.info.label << "\" ";
    if (r.quant_str.size() > 0)
      out << " Quantifiers(" << r.quant_str << ") ";
    return (out << "@(" << r.info.loc << ')');
  }

  struct RuleSet {
    const RuleInfo& info;
    std::vector<Rule> rules;
  }; 

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const RuleSet& rs) noexcept { return (json << rs.info); }
  std::ostream& operator << (std::ostream& out, const RuleSet& rs) noexcept { return (out << rs.info); }

  struct ModelRuleError : public IModelError {
    enum Where {UNKNOWN,GUARD,ACTION};
    ModelRuleError(const Rule& rule_) : isRule(true), where(UNKNOWN) { data.rule = &rule_; }
    ModelRuleError(const RuleInfo& info_) : isRule(false), where(UNKNOWN) { data.info = &info_; }
    ModelRuleError(const RuleSet& rule_set_) : isRule(false), where(UNKNOWN) { data.info = &(rule_set_.info); }
    ModelRuleError(id_t id) : isRule(false), where(UNKNOWN) { data.info = &(::__info__::RULE_SET_INFOS[id]); }
    ModelRuleError(id_t id, Where where_) : isRule(false), where(where_) { data.info = &(::__info__::RULE_SET_INFOS[id]); }
    void what(std::ostream& out) const noexcept { if (isRule) out << *data.rule; else out << *data.info; }
    virtual void to_json(json_file_t& json) const noexcept { _to_json(json); }
    virtual void to_json(json_str_t& json) const noexcept { _to_json(json); }
  private:
    union {const Rule* rule; const RuleInfo* info;} data;
    const bool isRule;
    const Where where;
    template<class O>
    void _to_json(ojstream<O>& json) const noexcept { 
      json << "{\"$type\":\"model-error\","
               "\"type\":\"rule" << ((where==UNKNOWN) ? "" : ((where==GUARD) ? "-guard" : "-action")) << "\","
              //  "\"what\":\""<< escape_str(what()) << "\","
               "\"inside\":";
      if (isRule) json << *data.rule; else json << *data.info;
      json << '}'; 
    } 
  };
  
  struct StartStateInfo {
    const std::string label;
    const location loc;
    const std::string json_h;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const StartStateInfo& si) noexcept { return (json << si.json_h << '}'); }
  std::ostream& operator << (std::ostream& out, const StartStateInfo& si) noexcept { return (out << "startstate \""<< si.label << "\" @(" << si.loc << ")"); }

  struct StartState {
    void (*initialize)(State_t&) throw (IModelError);
    const StartStateInfo& info;
    const id_t id;
    const std::string quant_json;
    const std::string quant_str;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const StartState& s) noexcept { 
    return (json << s.info.json_h 
                 << ",\"id\":" << s.id
                 << ",\"quantifiers\":" << s.quant_json << '}'); 
  }
  std::ostream& operator << (std::ostream& out, const StartState& s) noexcept {
    out << "startstate \"" << s.info.label << "\" ";
    if (s.quant_str.size() > 0)
      out << "-- id: " << s.id 
          << " -- Quantifiers(" << s.quant_str << ") ";
    return (out << "@(" << s.info.loc << ')');
  }

  struct ModelStartStateError : public IModelError {
    ModelStartStateError(const StartState& rule_) : isStartState(true) { data.rule = &rule_; }
    ModelStartStateError(const StartStateInfo& info_) : isStartState(false) { data.info = &info_; }
    ModelStartStateError(id_t id) : isStartState(false) { data.info = &(::__info__::STARTSTATE_INFOS[id]); }
    void what(std::ostream& out) const noexcept { if (isStartState) out << *data.rule; else out << *data.info; }
    virtual void to_json(json_file_t& json) const noexcept { _to_json(json); }
    virtual void to_json(json_str_t& json) const noexcept { _to_json(json); }
  private:
    union {const StartState* rule; const StartStateInfo* info;} data;
    const bool isStartState;
    template<class O>
    void _to_json(ojstream<O>& json) const noexcept { 
      json << "{\"$type\":\"model-error\","
               "\"type\":\"startstate\","
              //  "\"what\":\""<< escape_str(what()) << "\","
               "\"inside\":";
      if (isStartState) json << *data.rule; else json << *data.info;
      json << '}'; 
    } 
  };

  struct MErrorInfo {
    const std::string label;
    location loc;
    bool isFunct;
    const std::string json;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const MErrorInfo& ei) noexcept { return (json << ei.json_h); }
  std::ostream& operator << (std::ostream& out, const MErrorInfo& ei) noexcept { return (out << "error \""<< ei.label << "\" @(" << ei.loc << ")"); }
  
  struct ModelMErrorError : public IModelError {
    ModelMErrorError(const MErrorInfo& info_) : info(info_) {}
    ModelMErrorError(id_t id) : info(::__info__::ERROR_INFOS[id]) {}
    void what(std::ostream& out) const noexcept { out << info; }
    virtual void to_json(json_file_t& json) const noexcept { _to_json(json); }
    virtual void to_json(json_str_t& json) const noexcept { _to_json(json); }
  private:
    const MErrorInfo& info;
    template<class O>
    void _to_json(ojstream<O>& json) const noexcept { 
      json << "{\"$type\":\"model-error\","
               "\"type\":\"error-statement\","
              //  "\"what\":\"" << escape_str(what()) << "\","
               "\"inside\":" << info << '}'; 
    } 
  };

  struct FunctInfo {
    const std::string label;
    location loc;
    const std::string json;
    const std::string signature;
  };

  template<class O> ojstream<O>& operator << (ojstream<O>& json, const FunctInfo& fi) noexcept { return (json << fi.json_h); }
  std::ostream& operator << (std::ostream& out, const FunctInfo& fi) noexcept { return (out << fi.signature << " @(" << fi.loc << ")"); }
  
  struct ModelFunctError : public IModelError {
    ModelFunctError(const FunctInfo& info_) : info(info_) {}
    ModelFunctError(id_t id) : info(::__info__::FUNCT_INFOS[id]) {}
    void what(std::ostream& out) const noexcept { out << info; }
    virtual void to_json(json_file_t& json) const noexcept { _to_json(json); }
    virtual void to_json(json_str_t& json) const noexcept { _to_json(json); }
  private:
    const FunctInfo& info;
    template<class O>
    void _to_json(ojstream<O>& json) const noexcept { 
      json << "{\"$type\":\"model-error\","
               "\"type\":\"function\","
              //  "\"what\":\"" << escape_str(what()) << "\","
               "\"inside\":" << info << '}';
    } 
  };


  template<class O> void __jprint_exception(ojstream<O>& json, const std::exception& ex) noexcept {
    try {
        std::rethrow_if_nested(ex);
    } catch(const IModelError& mod_ex) {
      json << ',' << mod_ex;
    } catch(const std::exception& ex) {
      json << ',' << ex;
    } catch(...) {
      json << ",{\"$type\":\"trace-error\",\"what\":\"unknown non-exception type thrown !!\"}";
    }
  }
 
#define __romp__nested_exception__print_prefix '|'

  void __fprint_exception(std::ostream& out, const std::exception& ex/* , const size_t level */) noexcept;

  std::ostream& operator << (std::ostream& out, const IModelError& ex) noexcept {
    ex.what(out);
    out << '\n';
    __fprint_exception(out, ex);
    return out;
  }

  std::ostream& operator << (std::ostream& out, const std::exception& ex) noexcept {
    out << ex.what() << '\n';
    __fprint_exception(out, ex);
    return out;
  }

  void __fprint_exception(std::ostream& out, const std::exception& ex/* , const size_t level */) noexcept {
    // out << std::string(level, __romp__nested_exception__print_prefix);
    try {
        std::rethrow_if_nested(ex);
    } catch(const IModelError& mod_ex) {
      out << __romp__nested_exception__print_prefix << mod_ex;
    } catch(const std::exception& ex) {
      out << __romp__nested_exception__print_prefix << ex;
    } catch(...) {}
  }


  // struct PrettyPrinter {
  //   std::string
  // }

  // std::ostream& operator << (std::ostream& out, const State_t& state) {

  // }


  class IRandWalker {
    /**
     * @brief the handler the model will call when it reaches an inline `assert`/`invariant` statement
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the model calling this function should return an error or not.  ( \c true = DO throw, \c false = do NOT throw)
     */
    virtual bool assertion_handler(bool eval_expr, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call when it reaches a global rule `assert`/`invariant`
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the caller should report an error. ( \c true = DO throw, \c false = do NOT throw)
     */
    virtual bool invariant_handler(bool eval_expr, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call when it reaches an `assumption` statement
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - whether or not the caller should report an error. ( \c true = DO throw, \c false = do NOT throw)
     */
    virtual bool assumption_handler(bool eval_expr, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call when a `cover` statement is reached
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param cover_id the id given to this specific `cover` statement (value based upon the order it was processed by the code gen tool)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the caller should report an error. ( \c true = DO throw, \c false = do NOT throw)
     */
    virtual bool cover_handler(bool eval_expr, id_t cover_id, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call when a `liveness` statement is reached
     * @param eval_expr the expression to be evaluated (technically it will be evaluated before reaching this)
     * @param liveness_id the id given to this specific `liveness` statement (value based upon the order it was processed by the code gen tool)
     * @param prop_id the id given to the property (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the caller should report an error. ( \c true = DO throw, \c false = do NOT throw)
     */
    virtual bool liveness_handler(bool eval_expr, id_t liveness_id, id_t prop_id) = 0;
    /**
     * @brief the handler the model will call it reaches an `error` statement
     * @param error_id the id given to the error statement (value based upon the order it was processed by the code gen tool)
     * @return \c bool - the negation of whether or not the caller should report an error. ( \c true = DO throw, \c false = do NOT throw)
     */
    virtual bool error_handler(id_t error_id) = 0;
    friend State_t;
  };

}

namespace __type__ {
  typedef bool boolean; // builtin type mask for Murphi's boolean
}
