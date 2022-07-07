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
// #include <stdio.h>
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
#include "romp-json.hpp" // FOR PRE-CODEGEN LANGUAGE SUPPORT ONLY !! 
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
    std::string model_obj;
    file_position start;
    file_position end;
  };
  template<class O>
  ojstream<O>& operator << (ojstream<O>& json, const location& loc) { 
    return (json << "{\"$type\":\"location\","
            << "\"file\":\"" __model__filename "\","
            << (loc.model_obj != "") ? "\"inside\":\""+loc.model_obj+"\"," : ""
            << "start" << ':' << loc.start << ',' 
            << "\"end\":" << loc.end
            << '}'); 
  }
  std::ostream& operator << (std::ostream& out, const location& loc) { 
    out << ((__model__filename_contains_space) ? "\""__model__filename "\":" : __model__filename ":") 
        << loc.start << '-' << loc.end;
    if (loc.model_obj != "")
      out << " in \"" << loc.model_obj << "\"";
    return out; 
  }


  struct ModelError : public std::logic_error {
    const location loc;
    const std::string msg;
    ModelError(const std::string msg_, const location loc_) : /* std::logic_error(), */ msg(msg_), loc(loc_) {}
    virtual ModelError* clone() { return new ModelError(*this); }
    const char* what() const noexcept {
      std::strstream out;
      this->what(out);
      return out.str();
    }
    virtual void what(std::ostream& out) const noexcept {
      out << loc << " :: " << msg;
    }
  };

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

  struct ModelPropertyError : public ModelError {
    const std::string test_str;
    const PropertyType prop_type;
    ModelPropertyError(const PropertyType prop_type, const std::string test_str_, const std::string msg_, const location loc_)
      : ModelError(msg_,loc_), test_str(test_str_), prop_type(prop_type_) {}
    void what(std::ostream& out) const noexcept {
      out << loc << " :: " << prop_type << " \"" << msg << "\" " << test_str;
    } 
  };
  

  template<class O> void __jprint_exception(ojstream<O> out& json, const std::exception& ex) noexcept;
  template<class O> ojstream<O>& operator << (ojstream<O>& json, const ModelPropertyError& ex) noexcept { 
    json << "{\"$type\":\"romp::ModelPropertyError\","
             "\"loc\":" << ex.loc << ","
             "\"property:\":{" 
                 "\"$type\":\"property\",\"property-type\":\"" << ex.prop_type << "\","
                 "\"label\":\"" << ex.msg "\","
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
  template<class O> void __jprint_exception(ojstream<O> out& json, const std::exception& ex) noexcept {
    try {
        std::rethrow_if_nested(ex);
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
    out << prefix << ex.what() << '\n';
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


  struct Rule {
    bool (*guard)(const State_t&) throw (ModelError);
    void (*action)(State_t&) throw (ModelError);
    const char* params_json;
  };

  struct RuleSet {
    std::string name;
    location loc;
    std::vector<Rule> rules;
  }; 

  struct Invariant_info {
    id_t id;
    id_t ruleset_id;
    location& loc;
    std::string expression;
  };

  struct Invariant {
    bool (*check)(const State_t&) throw (ModelError);
  };

  struct StartState {
    void (*initialize)(State_t&) throw (ModelError);
    id_t id;
    id_t ruleset_id;
    location& loc;
  };

  // static void failed_assertion_(const char *message) {
  //   fprintf(stderr, "failed assertion: %s\n", message);
  //   exit(EXIT_FAILURE);
  // }

  // void (*failed_assertion)(const char *) = failed_assertion_;

  // static void failed_assumption_(const char *message) {
  //   fprintf(stderr, "failed assumption: %s\n", message);
  //   exit(EXIT_FAILURE);
  // }

  // void (*failed_assumption)(const char *) = failed_assumption_;

  // static void error_(const char *message) {
  //   fprintf(stderr, "error: %s\n", message);
  //   exit(EXIT_FAILURE);
  // }

  // void (*error)(const char *) throw (ModelError) = error_;

  // static void cover_(const char *message __attribute__((unused))) {}

  // void (*cover)(const char *) throw (ModelError) = cover_;

  // static void liveness_(const char *message __attribute__((unused))) {}

  // void (*liveness)(const char *) throw (ModelError) = liveness_;

}

namespace __type__ {
  typedef bool boolean; // builtin type mask for Murphi's boolean
}
