#include "generate_c.hpp"
#include "nested_escape.hpp"
// #include "../../common/escape.h"
#include "CLikeGenerator.hpp"
#include "CTypeGenerator.hpp"
#include "ModelSplitter.hpp"
#include "SigPerm.hpp"
#include "options.h"
#include "resources.h"
#include <cstddef>
#include <iostream>
#include <rumur/rumur.h>
#include <string>
#include <vector>
#include <unordered_set>
// #include <streambuf>
#include <strstream>
#include <algorithm>
#include <iomanip>
// #include <pair>


#include "romp_def.hpp"

using namespace rumur;


std::string int_to_hex(unsigned long i ) {
  // std::strstream stream;
  // stream << "0x" 
  //       //  << std::setfill ('0') << std::setw(sizeof(T)*2) 
  //        << std::hex << i;
  // return stream.str();
  return std::to_string(i);
}

namespace romp {

class CGenerator : public CLikeGenerator {

private:
  static const std::unordered_set<std::string> RESERVED_VAR_NAMES; //{ROMP_RESERVED_NAMES};
  // std::vector<std::string> state_vars;
  // std::vector<rumur::Ptr<rumur::SimpleRule>> rules;
  // std::vector<rumur::Ptr<rumur::PropertyRule>> property_rules;
  // std::vector<rumur::Ptr<rumur::StartState>> startstates;
  std::strstream prop_info_list;
  std::strstream error_info_list;
  id_t next_property_rule_id = 0u;
  id_t next_rule_id = 0u;
  id_t next_startstate_id = 0u;
  id_t next_funct_id = 0u;
  id_t next_property_id = 0u;
  id_t next_cover_id = 0u;
  id_t next_liveness_id = 0u;
  id_t next_assert_id = 0u;
  id_t next_assume_id = 0u;
  id_t next_error_id = 0u;
  bool processing_global_prop = false;
  enum {GLOBAL, RULE, FUNCT} inType = GLOBAL;

public:
  CGenerator(const std::vector<rumur::Comment> &comments_, std::ostream &out_,
             bool pack_)
      : CLikeGenerator(comments_, out_, pack_) 
  { 
    error_info_list << ROMP_INFO_ERRORS_DECL " = {";
  }

  void visit_constdecl(const ConstDecl &n) final {
    *this << indentation() << "const ";

    // if this constant has an explicit type, use that
    if (n.type != nullptr) {
      *this << *n.type;

    } else {

      // otherwise, if it was a previously typedefed enum, use its typedefed
      // name (to avoid later -Wsign-compare warnings on GCC)
      const Ptr<TypeExpr> type = n.value->type();
      auto it = enum_typedefs.find(type->unique_id);
      if (it != enum_typedefs.end()) {
        *this << " ::"ROMP_TYPE_NAMESPACE"::" << it->second;

      } else { // fallback on the type of the right hand side
        *this << "__typeof__(" << *n.value << ")";
      }
    }
    *this << " " << n.name << " = " << *n.value << ";";
    emit_trailing_comments(n);
    *this << "\n";
  }

  void visit_function(const Function &n) final {
    inType = FUNCT;
    id_t id = next_funct_id++;
    *this << indentation() << CodeGenerator::M_FUNCTION__FUNC_ATTRS << "\n"
          << indentation();
    if (n.return_type == nullptr) {
      *this << "void";
    } else {
      *this << *n.return_type;
    }
    *this << " " << n.name << "(";
    if (n.parameters.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Ptr<VarDecl> &p : n.parameters) {
        *this << sep;
        if (p->readonly)
          *this << "const ";
        *this << *p->type; // << " ";
        // if this is a var parameter, it needs to be a ~~pointer~~ reference
        if (!p->readonly) {
          // (void)is_pointer.insert(p->unique_id);  // no need for pointer shenanigans in C++
          *this << "&"; // "*";
        }
        *this << " " << p->name;
        sep = ", ";
      }
    }
    *this << ") ";
    if (n.is_pure()) *this << "const "; // if function never changes the state mark it as const (allowed in guards and property_rules)
    *this << " throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n";
    
    *this << indentation() << "try {\n";
    indent(); 

    for (const Ptr<Decl> &d : n.decls) {
      emit_leading_comments(*d);
      *this << *d;
    }
    for (const Ptr<Stmt> &s : n.body) {
      emit_leading_comments(*s);
      *this << *s;
    }

    dedent();
    *this << indentation() << "} catch (...) { ::std::throw_with_nested( " 
                          ROMP_MAKE_MODEL_ERROR_FUNCT(n,id) " ); }\n";

    dedent();
    *this << "}\n"; 
    inType = GLOBAL;
  }

  void visit_propertyrule(const PropertyRule &n) final {
    inType = RULE;
    id_t prop_id = next_property_rule_id++;
    // property_rules.push_back(Ptr<const PropertyRule>::make(n));
    // function prototype
    *this << indentation() << CodeGenerator::M_PROPERTY__FUNC_ATTRS << "\n"
          << indentation() << "void " ROMP_PROPERTYRULE_PREFIX << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        if (q.type == nullptr || q.decl == nullptr || q.decl->type == nullptr)
          throw Error("(property-rule) Quantifier is missing type/decl info !! \t[dev-error]",q.loc);
        *this << sep // ;
              << "const " // quantifier parameters should never be edited
              << *(q.type) << " " << q.name;
        // if (auto t = dynamic_cast<const TypeExprID *>(q.type.get()))
        //   *this << *t; //t->name;
        // else
        //   *this << value_type;
        // *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") const throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n";
    
    *this << indentation() << "try {\n";
    indent(); 

    // any aliases this property uses
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << *a;
    }

    processing_global_prop = true;
    const auto stmt = Ptr<PropertyStmt>::make(n.property,n.name,n.loc);
    *this // << indentation() // << "return ";
          << *stmt;
    processing_global_prop = false;
    // *this << "(" << *n.property.expr << ");\n";

    // clean up any aliases we defined
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << indentation() << "} catch (...) { ::std::throw_with_nested( " 
                          ROMP_MAKE_MODEL_ERROR_PROPERTY(n,prop_id) " ); }\n";

    dedent();
    *this << indentation() << "}\n";
    inType = GLOBAL;
  }

  void visit_simplerule(const SimpleRule &n) final {
    inType = RULE;
    // rules.push_back(Ptr<const SimpleRule>(&n));
    id_t id = next_rule_id++;

    *this << indentation() << CodeGenerator::M_RULE_GUARD__FUNC_ATTRS << "\n"
          << indentation() << "bool " ROMP_RULE_GUARD_PREFIX << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        if (q.type == nullptr || q.decl == nullptr || q.decl->type == nullptr)
          throw Error("(rule-guard) Quantifier is missing type/decl info !! \t[dev-error]",q.loc);
        *this << sep // ;
              << "const "  // quantifier parameters should never be edited
              << *(q.type) << " " << q.name;
        /// if (auto t = dynamic_cast<const TypeExprID *>(q.type.get()))
        //   *this << *t; //t->name;
        // else
        //   *this << value_type;
        // *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") const throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n";
    
    *this << indentation() << "try {\n";
    indent(); 

    // any aliases that are defined in an outer scope
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << *a;
    }

    *this << indentation() << "return ";
    if (n.guard == nullptr) {
      *this << "true";
    } else {
      *this << *n.guard;
    }
    *this << ";\n";

    // clean up aliases
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << indentation() << "} catch (...) { ::std::throw_with_nested( "
                                  ROMP_MAKE_MODEL_ERROR_RULE_GUARD(n,id) " ); }\n";

    dedent();
    *this << indentation() << "}\n\n";

    *this << indentation() << CodeGenerator::M_RULE_ACTION__FUNC_ATTRS << "\n"
          << indentation() << "void " ROMP_RULE_ACTION_PREFIX << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        if (q.type == nullptr || q.decl == nullptr || q.decl->type == nullptr)
          throw Error("(rule-action) Quantifier is missing type/decl info !! \t[dev-error]",q.loc);
        *this << sep // ;
              << "const " // quantifier parameters should never be edited
              << *(q.type) << " " << q.name;
        // if (auto t = dynamic_cast<const TypeExprID *>(q.type.get()))
        //   *this << *t; //t->name;
        // else
        //   *this << value_type;
        // *this << " " << q.name;
        *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n";
    
    *this << indentation() << "try {\n";
    indent(); 

    // aliases, variables, local types, etc.
    for (const Ptr<AliasDecl> &a : n.aliases) {
      emit_leading_comments(*a);
      *this << *a;
    }
    for (const Ptr<Decl> &d : n.decls) {
      emit_leading_comments(*d);
      *this << *d;
    }

    for (const Ptr<Stmt> &s : n.body) {
      emit_leading_comments(*s);
      *this << *s;
    }

    // clean up any aliases we defined
    for (const Ptr<Decl> &d : n.decls) {
      if (auto a = dynamic_cast<const AliasDecl *>(d.get())) {
        *this << "#undef " << a->name << "\n";
      }
    }
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << indentation() << "} catch (...) { ::std::throw_with_nested( " 
                          ROMP_MAKE_MODEL_ERROR_RULE_ACTION(n,id) " ); }\n";

    dedent();
    *this << indentation() << "}\n";
    inType = GLOBAL;
  }

  void visit_startstate(const StartState &n) final {
    inType = RULE;
    // startstates.push_back(Ptr<const StartState>(&n));
    id_t id = next_startstate_id++;
    
    *this << indentation() << CodeGenerator::M_STARTSTATE__FUNC_ATTRS 
          << " void " ROMP_STARTSTATE_PREFIX << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const auto q : n.quantifiers) {
        if (q.type == nullptr || q.decl == nullptr || q.decl->type == nullptr)
          throw Error("(startstate) Quantifier is missing type/decl info !! \t[dev-error]",q.loc);
        *this << sep // ;
              << "const " // quantifier parameters should never be edited
              << *(q.type) << " " << q.name;
        /// if (auto t = dynamic_cast<const TypeExprID *>(q.type.get()))
        //   *this << *t; //t->name;
        // else
        //   *this << value_type;
        // *this << " " << q.name;
        sep = ", ";
      }
    }

    *this << ") throw (" ROMP_MODEL_EXCEPTION_TYPE ") {\n";
    indent();
    *this << indentation() << "using namespace ::" ROMP_TYPE_NAMESPACE ";\n"; 

    *this << indentation() << "try {\n";
    indent(); 

    // aliases, variables, local types, etc.
    for (const Ptr<AliasDecl> &a : n.aliases) {
      emit_leading_comments(*a);
      *this << *a;
    }
    for (const Ptr<Decl> &d : n.decls) {
      emit_leading_comments(*d);
      *this << *d;
    }

    for (const Ptr<Stmt> &s : n.body) {
      emit_leading_comments(*s);
      *this << *s;
    }

    // clean up any aliases we defined
    for (const Ptr<Decl> &d : n.decls) {
      if (auto a = dynamic_cast<const AliasDecl *>(d.get())) {
        *this << "#undef " << a->name << "\n";
      }
    }
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << indentation() << "} catch (...) { ::std::throw_with_nested( " 
                          ROMP_MAKE_MODEL_ERROR_STARTSTATE(n, id) " ); }\n";

    dedent();
    *this << indentation() << "}\n\n";
    inType = GLOBAL;
  }

  void visit_vardecl(const VarDecl &n) final {
    if (CGenerator::RESERVED_VAR_NAMES.find(n.name) != CGenerator::RESERVED_VAR_NAMES.end())
      throw Error("``"+n.name+"`` is a reserved name!",n.loc);
    *this << indentation() << *n.type << " " << n.name << ";";
    emit_trailing_comments(n);
    *this << "\n";
  }


  void gen_state_to_json(std::vector<Ptr<VarDecl>> &children) {
    // *this << "\n" << indentation() << "NLOHMANN_DEFINE_TYPE_INTRUSIVE("
    //       ROMP_STATE_CLASS_NAME;
    // for (const Ptr<const VarDecl> &c : children) 
    //   if (auto v = dynamic_cast<const VarDecl *>(c.get()))
    //     *this << "," << v->name;
    // *this << ")\n";
    *this << "\n" << indentation() << "//TODO: make the json and stream operators for the state class\n\n";
  }

  void gen_ruleset_array(const std::vector<rumur::Ptr<rumur::SimpleRule>>& rules) {
    std::strstream ruleset_array;
    ruleset_array << indentation() << ROMP_CALLER_RULESETS_DECL " = {";
    std::string rs_sep = "\n\t\t";
    id_t info_id = 0u;
    // for (const Ptr<SimpleRule>& _rule : rules) {
    for (size_t i=0; i<rules.size(); ++i) {
      // const SimpleRule& rule = *_rule;
      const SimpleRule& rule = *rules[i];
      ruleset_array << rs_sep << ROMP_MAKE_RULESET_STRUCT_HEADER(info_id);
      // size_t rule_c = 0;
      *this << indentation() << "/* --- Rules Generated by: `" << rule.name << "` (RuleSet expansions) --- */\n";
      SigPerm sigs(rule);
      std::strstream _sig_str;
      std::string r_sep = "";
      for (const Sig& sig : sigs) {
        _sig_str << sig;
        std::string sig_str(_sig_str.str() + " "); // sig_str += " ";
        std::string _guard = ROMP_RULE_GUARD_PREFIX + rule.name + "__" + std::to_string(sig.index);  // int_to_hex(sig.index);
        std::string _action = ROMP_RULE_ACTION_PREFIX + rule.name + "__" + std::to_string(sig.index); // int_to_hex(sig.index);
        ruleset_array << r_sep << ROMP_MAKE_RULE_STRUCT(_guard,_action,info_id,sig.to_json(),sig.to_string());
        *this << indentation()
              << CodeGenerator::M_RULE_GUARD__FUNC_ATTRS
              << " bool "
              << _guard
              << "(const State_t& s) throw (" ROMP_MODEL_EXCEPTION_TYPE ") {"
              "return s." ROMP_RULE_GUARD_PREFIX << sig_str << "; }\n"
              << indentation()
              << CodeGenerator::M_RULE_ACTION__FUNC_ATTRS
              << " void "
              << _action
              << "(State_t& s) throw (" ROMP_MODEL_EXCEPTION_TYPE ") {"
              "s." ROMP_RULE_ACTION_PREFIX << sig_str << "; }\n";
        // rule_c++;
        r_sep = ", ";
      }
      ruleset_array << indentation() << "\t" ROMP_MAKE_RULESET_STRUCT_FOOTER();
      rs_sep = ",\n\t\t";
      ++info_id;
      this.out << std::flush;
    }
    // *this << "\n#undef " ROMP_RULESETS_LEN "\n"; 
    // *this << "\n#define " ROMP_RULESETS_LEN " " << rules.size() << "\n"; 
    *this << "\n\n" << indentation() << "// All of the rule sets in one place\n" 
          << ruleset_array.str() << "};\n\n";
    this.out << std::flush;
  }


  void gen_property_array(const std::vector<rumur::Ptr<rumur::PropertyRule>>& property_rules) {
    std::strstream prop_list;
    prop_list << ROMP_CALLER_PROPERTIES_DECL " = {";
    id_t _lid = 0u;
    size_t count = 0u;
    std::string sep = "";
    // for (const Ptr<const PropertyRule>& _prop : property_rules) {
    for (size_t i=0; i<property_rules.size(); ++i) {
      // const PropertyRule& prop = *_prop;
      const PropertyRule& prop = *property_rules[i];
      *this << indentation() << "/* --- Property Rule(s) generated by: `" << prop.name << "` (RuleSet expansions) --- */\n";
      SigPerm sigs(prop);
      for (auto& sig : sigs) {
        std::string _check = ROMP_PROPERTYRULE_PREFIX + prop.name + "__" + std::to_string(sig.index);  // int_to_hex(sig.index);
        out << indentation() 
            << CodeGenerator::M_PROPERTY__FUNC_ATTRS
            << " void "
            << _check
            << "(const State_t& s) throw (" ROMP_MODEL_EXCEPTION_TYPE ") {"
               "return s." ROMP_PROPERTYRULE_PREFIX << sig << "; }\n";
        prop_list << sep << ROMP_MAKE_PROPERTY_STRUCT(_check,_lid,sig.to_json(),sig.to_string());
        sep = ", ";
        ++count;
      }
      ++_lid;
      this.out << std::flush;
    }
    *this << "\n\n#define " ROMP_PROPERTY_RULES_LEN " (" << count <<  "ul) // the number of property rules (after ruleset expansion) in the model\n"; 
    *this << "\n" << indentation() << "// All of the rule sets in one place\n" 
          << prop_list.str() << "};\n\n";
    this.out << std::flush;
  }


  void gen_startstate_array(const std::vector<rumur::Ptr<rumur::StartState>>& startstates) {
    std::strstream prop_list;
    prop_list << ROMP_CALLER_STARTSTATES_DECL " = {";
    id_t info_id = 0u;
    size_t count = 0u;
    std::string sep = "";
    // for (const Ptr<const StartState>& _startstate : startstates) {
    for (size_t i=0; i<startstates.size(); ++i) {
      // const StartState& startstate = *_startstate;
      const StartState& startstate = *startstates[i];
      *this << indentation() << "/* --- StartState Rule(s) generated by: `" << startstate.name << "` (RuleSet expansions) --- */\n";
      SigPerm sigs(startstate);
      for (auto& sig : sigs) {
        std::string _init = ROMP_PROPERTYRULE_PREFIX + startstate.name + "__" + std::to_string(sig.index);  // int_to_hex(sig.index);
        out << indentation() 
            << CodeGenerator::M_STARTSTATE__FUNC_ATTRS
            << " void "
            << _init
            << "(State_t& s) throw (" ROMP_MODEL_EXCEPTION_TYPE ") {"
               "return s." ROMP_STARTSTATE_PREFIX << sig << "; }\n";
        prop_list << sep << ROMP_MAKE_STARTSTATE_STRUCT(_init,info_id,sig.to_json(),sig.to_string());
        sep = ", ";
        ++count;
      }
      ++info_id;
      this.out << std::flush;
    }
    *this << "\n\n#define " ROMP_STARTSTATE_RULES_LEN " (" << count <<  "ul) // the number of property rules (after ruleset expansion) in the model\n"; 
    *this << "\n" << indentation() << "// All of the rule sets in one place\n" 
          << prop_list.str() << "};\n\n";
    this.out << std::flush;
  }

  void visit_propertystmt(const PropertyStmt &n) {
    id_t id = ((not processing_global_prop) ? (next_property_id++) : (next_property_rule_id-1));
    id_t _id = 0u;

    *this << indentation() << "if (";
    indent();

    switch (n.property.category) {
    case Property::ASSERTION:
      *this << ROMP_ASSERTION_HANDLER(n,id) << ")\n"
            << indentation() << "throw " ROMP_MAKE_MODEL_ERROR_PROPERTY(n,id) ";\n";
      if (not processing_global_prop)
        prop_info_list << ROMP_MAKE_PROPERTY_INFO_STRUCT(n,id,n.message,ROMP_PROPERTY_TYPE_ASSERT, to_json(n,"assert")) ",";
      break;

    case Property::ASSUMPTION:
      *this << ROMP_ASSUMPTION_HANDLER(n,id) << ")\n"
            << indentation() << "throw " ROMP_MAKE_MODEL_ERROR_PROPERTY(n,id) ";\n";
      if (not processing_global_prop)
        prop_info_list << ROMP_MAKE_PROPERTY_INFO_STRUCT(n,id,n.message,ROMP_PROPERTY_TYPE_ASSUME, to_json(n,"assume")) ",";
      break;

    case Property::COVER:
      _id = next_cover_id++;
      *this << ROMP_COVER_HANDLER(n,id,_id) << ")\n"
            << indentation() << "throw " ROMP_MAKE_MODEL_ERROR_PROPERTY(n,id) ";\n";
      if (not processing_global_prop)
        prop_info_list << ROMP_MAKE_PROPERTY_INFO_STRUCT(n,_id,n.message,ROMP_PROPERTY_TYPE_COVER, to_json(n,"cover")) ",";
      break;

    case Property::LIVENESS:
      if (not processing_global_prop)
        throw Error("liveness properties are NOT supported as embedded statements only global rules!", n.loc);
      _id = next_liveness_id++;
      *this << ROMP_LIVENESS_HANDLER(n,id,_id) << ")\n"
            << indentation() << "throw " ROMP_MAKE_MODEL_ERROR_PROPERTY(n,id) ";\n";
      // prop_info_list << ROMP_MAKE_PROPERTY_INFO_STRUCT(n,_id,n.message,ROMP_PROPERTY_TYPE_LIVENESS, to_json(n,"liveness")) ",";
      break;
    }
    dedent();

    emit_trailing_comments(n);
    *this << "\n";
  }

  void visit_errorstmt(const ErrorStmt &n) {
    id_t id = next_error_id++;
    *this << indentation() << "if (" ROMP_ERROR_HANDLER(id) ")\n";
    indent();
    *this << indentation() << "throw " ROMP_MAKE_MODEL_ERROR_ERROR(n,id) ";\n";
    dedent();
    emit_trailing_comments(n);
    *this << "\n";
    error_info_list << ROMP_MAKE_ERROR_INFO_STRUCT(n,(inType==FUNCT),to_json(n)) ",";
  }

  void visit_model(const Model &n) {
    ModelSplitter sorter;
    sorter.sort_model(n.children);
    next_property_id = sorter.next_property_id;
    next_cover_id = sorter.next_cover_id;
    next_liveness_id = sorter.next_liveness_id;
    next_assert_id = sorter.next_invar_id;
    next_assume_id = sorter.next_assume_id;
    // prop_info_list = sorter.prop_info_list; // not possible just output this before the one made here
    // SplitModel split = splitter.split_model(n);

    *this << "\n\n" << indentation() << "/* ======= Model Type & Const Definitions ====== */\n";
    *this << "\n" << indentation() << "namespace " ROMP_TYPE_NAMESPACE " {\n";
    indent();

    *this << indentation() << "typedef " << value_type << " range_t;\n"
          << indentation() << "typedef unsigned int scalarset_t;\n";

    CTypeGenerator type_gen(comments, out, pack, 
      [&](const ConstDecl &_n) -> void {visit_constdecl(_n);});
    for (const Ptr<const Decl> &decl : sorter.global_decls)
      type_gen << *decl;

    dedent();
    *this << "\n" << indentation() << "}\n\n"; // << std::flush;

    // update these to reflect the processes from the type emitter
    enum_typedefs = type_gen.enum_typedefs;
    emitted_tDecls = type_gen.emitted_tDecls;
    is_pointer = type_gen.is_pointer;
    // emitted = type_gen.emitted;

    *this << "\n\n" << indentation() << "/* ======= Generated Model & State ====== */\n";
    *this << "\n" << indentation() << "namespace " ROMP_MODEL_NAMESPACE_NAME " {\n";
    indent();

    *this << "\n" << indentation() << "struct " ROMP_STATE_CLASS_NAME " {\n";
    indent();
    *this << indentation() << ROMP_RAND_WALKER_DECL "\n\n";
    *this << "\n" << indentation() << "/* ======= Model State Variables ====== */\n\n";
    // split.state_var_decl.visit(*this);
    for (const Ptr<const VarDecl> &var : sorter.state_var_decls)
      *this << *var << "\n"; 
    gen_state_to_json(sorter.state_var_decls);

    *this << "\n" << indentation() << "/* ======= Murphi Model Functions ====== */\n\n"; // << std::flush;
    // split.funct_decls.visit(*this);
    for (const Ptr<const Function> &funct : sorter.funct_decls)
      *this << *funct << "\n";

    *this << "\n" << indentation() << "/* ======= Murphi Model StartStates, Rules & Global Properties ====== */\n\n"; // << std::flush;
    // split.rule_decls.visit(*this);
    // for (const Ptr<const Rule> &_r : sorter.rule_decls) {
    //   std::vector<Ptr<Rule>> rs = _r->flatten();
    //   for (auto &r2 : rs) {
    //     if (_r->quantifiers.size() < r2->quantifiers.size())
    //       throw Error("rule did not flatten properly\t[dev-error]",_r->loc);
    //     for (int i=0; i<_r->quantifiers.size(); ++i) // bad lazy patch that will not work for nested rulesets!!
    //       r2->quantifiers[i] = _r->quantifiers[i]; // reason: flattening is loosing pointers to quantifier members 
    //     *this << *r2 << "\n";
    //   }
    // }
    for (const auto& _s : sorter.startstate_decls)
      *this << *_s << "\n";
    *this << "\n";
    for (const auto& _r : sorter.rule_decls)
      *this << *_r << "\n";
    *this << "\n";
    for (const auto& _p : sorter.property_decls)
      *this << *_p << "\n";
  
    dedent();
    *this << "\n" << indentation() << "};\n"; // << std::flush;

    dedent();
    *this << indentation() << "}\n\n";
    
    *this << indentation() << "/* ======= Murphi Model Infos & MetaData ====== */\n"; // << std::flush;
    *this << indentation() << "namespace " ROMP_INFO_NAMESPACE_NAME " {\n";
    indent();
    *this << "#define " ROMP_FUNCTS_LEN " (" << sorter.funct_decls.size() << "ul) // the number of functions & procedures in the model\n"
          << indentation() << "// the info/metadata about the functions/procedures in the model\n"
          << indentation() << sorter.funct_info_list.str() << "};\n"
          << "#define " ROMP_ERRORS_LEN " (" << next_error_id << "ul) // the number of error statements in the model\n"
          << indentation() << "// the info/metadata about the murphi error statements in the model\n"
          << indentation() << error_info_list.str() << "};\n"
          << "#define " ROMP_PROPERTY_INFOS_LEN " (" << next_property_id <<  "ul) // the number of property statements & rules in the model\n"
          // << "#define " ROMP_PROPERTY_RULES_LEN " (" << property_rules.size() <<  "ul) // the number of property rules (after ruleset expansion) in the model\n"
          << indentation() << "// the info/metadata about the murphi properties in the model\n"
          << indentation() << sorter.prop_info_list.str() << prop_info_list.str() << "};\n"
          << "#define " ROMP_STARTSTATE_INFOS_LEN " (" << sorter.startstate_decls.size() <<  "ul) // the number of start state rules (before ruleset expansions) in the model\n"
          // << "#define " ROMP_STARTSTATE_RULES_LEN " (" << startstates.size() <<  "ul) // the number of start state rules (after ruleset expansions) in the model\n"
          << indentation() << "// the info/metadata about the startstate rules in the model\n"
          << indentation() << sorter.startstate_info_list.str() << "};\n"
          << "#define " ROMP_RULESETS_LEN " (" << sorter.rule_decls.size() << "ul) // the number of rules (before ruleset expansion) in the model\n"
          << indentation() << "// the info/metadata about the rules in the model\n"
          << indentation() << sorter.rule_info_list.str() << "};\n";
    dedent();
    *this << "\n" << indentation() << "}\n\n"; out << std::flush;

    *this << indentation() << "/* ======= Romp CALLER Lists ====== */\n";
    *this << indentation() << "namespace " ROMP_CALLER_NAMESPACE_NAME " {\n";
    indent();

    *this << indentation() << "typedef " ROMP_STATE_TYPE " State_t; // type mask for the generated state object\n";

    gen_ruleset_array(sorter.rule_decls);
    gen_property_array(sorter.property_decls);
    gen_startstate_array(sorter.startstate_decls);

    dedent();
    *this << "\n" << indentation() << "}\n\n"; // << std::flush;
    out << std::flush;
  }

  virtual ~CGenerator() = default;

  // CGenerator& operator << (std::ostream& os) {
  //   out << os;
  //   return *this;
  // }

  std::string to_json(const ErrorStmt& rule) {
    std::strstream buf;
    buf << "{\"$type\":\"error-statement\",";
    buf << "\"label\":\"" << nEscape(rule.message) << "\","
            "\"loc\":{\"$type\":\"location\","
                      "\"file\":\"" << nEscape(file_path) <<  "\","
    //                   "\"inside\":\"";
    // // TODO ...
    // buf <<            "\","
                      "\"start\":["<< rule.loc.begin.line << "," << rule.loc.begin.column << "],"  
                      "\"end\":["<< rule.loc.end.line << "," << rule.loc.end.column << "]}";
    return buf.str();
  }

  std::string to_json(const PropertyStmt& prop, const std::string prop_type) {
    std::strstream buf;
    buf << "{\"$type\":\"property\","
        << "\"type\":\"" << prop_type << "\","
        << "\"label\":\"" << nEscape(prop.message == "" ? prop.property.expr->to_string() : prop.message) << "\","
        << "\"expr\":\"" << nEscape(prop.property.expr->to_string()) << "\","
            "\"loc\":{\"$type\":\"location\","
                      "\"file\":\"" << nEscape(file_path) << "\","
    //                   "\"inside\":\"";
    // // TODO ...
    // buf <<            "\","
                      "\"start\":["<< prop.loc.begin.line << "," << prop.loc.begin.column << "],"  
                      "\"end\":["<< prop.loc.end.line << "," << prop.loc.end.column << "]}";
    return buf.str();
  }
public:
  CGenerator& operator << (const char* str) { out << str; return *this; }
  CGenerator& operator << (const std::string str) { out << str; return *this; }
  CGenerator& operator << (const int val) { out << val; return *this; }
  CGenerator& operator << (const rumur::Node& n) { dispatch(n); return *this; }
  CGenerator& operator << (const Sig& sig) { out << sig; return *this; }
  friend CGenerator& operator << (CGenerator& gen, const SigParam& param) { gen.out << param; return gen; }
  // CGenerator& operator << (const SigParam& param) { out << param; return *this; }
};

const std::unordered_set<std::string> CGenerator::RESERVED_VAR_NAMES{ROMP_RESERVED_NAMES};


std::ostream& operator << (std::ostream& out, const Sig& sig) {
  out << sig.perm.rule.name << "(";
  std::string sep;
  for (size_t i=0; i<sig.size(); ++i) 
    if (sig.params[i] != nullptr) {
      out << sep << *sig.params[i];
      sep = ", ";
    }
  return (out << ")");
}

std::ostream& operator << (std::ostream& out, const SigParam& param) { return (out << param.to_string()); }



} // namespace

void output_embedded_code_file(std::ostream& out, const unsigned char* ecf, const size_t ecf_l) {
  for (size_t i = 0; i < ecf_l; i++)
    out << (char) ecf[i];
}



void generate_c(const Node &n, const std::vector<Comment> &comments, bool pack,
                std::ostream &out, const std::string& build_cmds) {

  out << ROMP_GENERATED_FILE_PREFACE("\tGenerated code for a romp \"parallel random walker\" verification tool based off of the Murphi Model described in:\n"
                                     "\t\tOriginal Murphi Model: " + file_path + "\n"
                                     "\tPlease build with the following command(s):\n\t\t" + build_cmds + "") "\n";

  out << "\n#define __romp__GENERATED_CODE\n\n";
  out << "\n#define _ROMP_STATE_TYPE " ROMP_STATE_TYPE "\n\n";
  out << "\n#define __model__filename \"" << nEscape(file_path) << "\"\n\n";
  auto _count = std::count(file_path.begin(), file_path.end(), ' ');
  out << "\n#define __model__filename_contains_space " 
      << ((_count > 0) ? "true" : "false") << "\n\n";


  out << "\n#pragma region inline_library_includes\n\n";
  // write json library to the file
  // output_embedded_code_file(out, resources_lib_nlohmann_json_hpp, resources_lib_nlohmann_json_hpp_len);
  out << "\n#pragma endregion inline_library_includes\n\n" << std::flush;

  out << "namespace " ROMP_MODEL_NAMESPACE_NAME " { struct " ROMP_STATE_CLASS_NAME "; } // helpful pre-definition\n\n";

  out << "\n#pragma region model_prefixes\n\n";
  // write the static prefix to the beginning of the source file
  output_embedded_code_file(out, resources_c_prefix_cpp, resources_c_prefix_cpp_len);
  out << "\n#pragma endregion model_prefixes\n\n" << std::flush;


  out << "\n\n#pragma region generated_code\n\n";
  romp::CGenerator gen(comments, out, pack);
  gen.dispatch(n);
  out << "\n\n#pragma endregion generated_code\n\n" << std::flush;

  out << "\n#pragma region romp_rw\n\n";
  output_embedded_code_file(out, resources_romp_rw_hpp, resources_romp_rw_hpp_len);
  out << "\n#pragma endregion romp_rw\n\n" << std::flush;
  out << "\n#pragma region romp_rw_options\n\n";
  output_embedded_code_file(out, resources_romp_rw_options_hpp, resources_romp_rw_options_hpp_len);
  out << "\n#pragma endregion romp_rw_options\n\n" << std::flush;
  out << "\n#pragma region romp_rw_main\n\n";
  output_embedded_code_file(out, resources_romp_rw_main_hpp, resources_romp_rw_main_hpp_len);
  out << "\n#pragma endregion romp_rw_main\n" << std::flush;


  // out << buffer.rdbuf() << "\n\n";
  // out << std::flush;
}
