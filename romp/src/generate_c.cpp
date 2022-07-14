#include "generate_c.hpp"
#include "../../common/escape.h"
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

template< typename T >
std::string int_to_hex( T i ) {
  std::stringstream stream;
  stream << "0x" 
        //  << std::setfill ('0') << std::setw(sizeof(T)*2) 
         << std::hex << i;
  return stream.str();
}

namespace romp {

class CGenerator : public CLikeGenerator {

private:
  static const std::unordered_set<std::string> RESERVED_VAR_NAMES; //{ROMP_RESERVED_NAMES};
  // std::vector<std::string> state_vars;
  std::vector<rumur::Ptr<rumur::SimpleRule>> rules;
  std::vector<rumur::Ptr<rumur::PropertyRule>> property_rules;
  std::vector<rumur::Ptr<rumur::StartState>> startstates;
  std::strstream prop_info_list;
  std::strstream error_info_list;
  id_t next_property_rule_id = 0u;
  id_t next_property_id = 0u;
  id_t next_cover_id = 0u;
  id_t next_liveness_id = 0u;
  id_t next_assert_id = 0u;
  id_t next_assume_id = 0u;
  id_t next_error_id = 0u;
  enum {GLOBAL, RULE, FUNCT} inType = GLOBAL;
  union {Rule& rule; Function& funct; nullptr_t null;} inside;

public:
  CGenerator(const std::vector<rumur::Comment> &comments_, std::ostream &out_,
             bool pack_)
      : CLikeGenerator(comments_, out_, pack_) 
  { 
    inside.null = nullptr;
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
    inside.funct = n;
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
    *this << indentation() << "} catch (::std::exception& ex) { ::std::throw_with_nested( " 
                          ROMP_MAKE_MODEL_EXCEPTION("[func/proc] " + n.name, n) " ); }\n";

    dedent();
    *this << "}\n"; 
    inType = GLOBAL;
    inside.null = nullptr;
  }

  void visit_propertyrule(const PropertyRule &n) final {
    inType = RULE;
    inside.rule = n;
    id_t prop_id = next_property_rule_id++;
    property_rules.push_back(Ptr<const PropertyRule>::make(n));
    // function prototype
    *this << indentation() << CodeGenerator::M_PROPERTY__FUNC_ATTRS << "\n"
          << indentation() << "bool " ROMP_PROPERTYRULE_PREFIX << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        *this << sep // ;
              << "const " // quantifier parameters should never be edited
              << *(q.decl->type) << " " << q.name;
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

    *this << indentation() << "return ";
    std::string prop_type = "[property] ";
    switch (n.property.category) {
      case Property::ASSERTION:
        *this << ROMP_PROPERTY_HANDLER_INVAR "("
              " (" << *n.property.expr << "), " 
              << next_property_id++ << "u"
              ");";
        prop_type = "[invariant] ";
        break;
      case Property::ASSUMPTION:
        *this << ROMP_PROPERTY_HANDLER_ASSUME "("
              " (" << *n.property.expr << "), " 
              << next_property_id++ << "u"
              ");";
        break;
      case Property::COVER:
        *this << ROMP_PROPERTY_HANDLER_COVER "("
              " (" << *n.property.expr << "), " 
              << next_cover_id++ << "u, "
              << next_property_id++ << "u"
              ");";
        prop_type = "[cover] ";
        break;
      case Property::LIVENESS:
        *this << ROMP_PROPERTY_HANDLER_LIVENESS "("
              " (" << *n.property.expr << "), " 
              << next_liveness_id++ << "u, "
              << next_property_id++ << "u"
              ");";
        prop_type = "[liveness] ";
        break;
    }
    *this << "(" << *n.property.expr << ");\n";

    // clean up any aliases we defined
    for (const Ptr<AliasDecl> &a : n.aliases) {
      *this << "#undef " << a->name << "\n";
    }

    dedent();
    *this << indentation() << "} catch (::std::exception& ex) { ::std::throw_with_nested( " 
                          ROMP_MAKE_MODEL_EXCEPTION(prop_type + n.name, n) " ); }\n";

    dedent();
    *this << indentation() << "}\n";
    inType = GLOBAL;
    inside.null = nullptr;
  }

  void visit_simplerule(const SimpleRule &n) final {
    inType = RULE;
    inside.rule = n;
    rules.push_back(Ptr<const SimpleRule>(&n));

    *this << indentation() << CodeGenerator::M_RULE_GUARD__FUNC_ATTRS << "\n"
          << indentation() << "bool " ROMP_RULE_GUARD_PREFIX << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        *this << sep // ;
              << "const "  // quantifier parameters should never be edited
              << *(q.decl->type) << " " << q.name;
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
    *this << indentation() << "} catch (::std::exception& ex) { ::std::throw_with_nested( " 
                          ROMP_MAKE_MODEL_EXCEPTION("[rule guard] " + n.name, n) " ); }\n";

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
        *this << sep // ;
              << "const " // quantifier parameters should never be edited
              << *(q.decl->type) << " " << q.name;
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
    *this << indentation() << "} catch (::std::exception& ex) { ::std::throw_with_nested( " 
                          ROMP_MAKE_MODEL_EXCEPTION("[rule action] " + n.name, n) " ); }\n";

    dedent();
    *this << indentation() << "}\n";
    inType = GLOBAL;
    inside.null = nullptr;
  }

  void visit_startstate(const StartState &n) final {
    inType = RULE;
    inside.rule = n;
    startstates.push_back(Ptr<const StartState>(&n));
    
    *this << indentation() << CodeGenerator::M_STARTSTATE__FUNC_ATTRS 
          << " void " ROMP_STARTSTATE_PREFIX << n.name << "(";

    // parameters
    if (n.quantifiers.empty()) {
      *this << "void";
    } else {
      std::string sep;
      for (const Quantifier &q : n.quantifiers) {
        *this << sep // ;
              << "const " // quantifier parameters should never be edited
              << *(q.decl->type) << " " << q.name;
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
    *this << indentation() << "} catch (::std::exception& ex) { ::std::throw_with_nested( " 
                          ROMP_MAKE_MODEL_EXCEPTION("[startstate] " + n.name, n) " ); }\n";

    dedent();
    *this << indentation() << "}\n\n";
    inType = GLOBAL;
    inside.null = nullptr;
  }

  void visit_vardecl(const VarDecl &n) final {
    if (CGenerator::RESERVED_VAR_NAMES.find(n.name) != CGenerator::RESERVED_VAR_NAMES.end())
      throw Error("``"+n.name+"`` is a reserved name!",n.loc);
    *this << indentation() << *n.type << " " << n.name << ";";
    emit_trailing_comments(n);
    *this << "\n";
  }


  void gen_state_to_json(std::vector<Ptr<VarDecl>> &children) {
    *this << "\n" << indentation() << "NLOHMANN_DEFINE_TYPE_INTRUSIVE("
          ROMP_STATE_CLASS_NAME;
    for (const Ptr<const VarDecl> &c : children) 
      if (auto v = dynamic_cast<const VarDecl *>(c.get()))
        *this << "," << v->name;
    *this << ")\n";
  }

  void gen_ruleset_array() {
    std::strstream ruleset_array;
    ruleset_array << indentation() << ROMP_CALLER_RULESETS_DECL " = {";
    std::string rs_sep = "\n\t\t";
    id_t info_id = 0u;
    for (const Ptr<const SimpleRule>& rule : rules) {
      ruleset_array << rs_sep << ROMP_MAKE_RULESET_STRUCT_HEADER(info_id);
      size_t rule_c = 0;
      *this << indentation() << "/* --- Rules Generated by: `" << rule->name << "` (RuleSet expansions) --- */\n";
      SigPerm sigs(rule);
      std::strstream _sig_str;
      std::string r_sep = "";
      for (const Sig& sig : sigs) {
        _sig_str << sig;
        std::string sig_str(_sig_str.str());
        std::string _guard = ROMP_RULE_GUARD_PREFIX + rule->name + "__" + int_to_hex(rule_c);
        std::string _action = ROMP_RULE_ACTION_PREFIX + rule->name + "__" + int_to_hex(rule_c);
        ruleset_array << r_sep << ROMP_MAKE_RULE_STRUCT(_guard,_action,info_id,sig.to_json(),sig.to_string());
        *this << indentation()
              << CodeGenerator::M_RULE_GUARD__FUNC_ATTRS
              << " bool"
              << _guard
              << "(const State_t& s) throw (" ROMP_MODEL_EXCEPTION_TYPE ") {"
              "return s." ROMP_RULE_GUARD_PREFIX << sig_str << "; }\n"
              << indentation()
              << CodeGenerator::M_RULE_ACTION__FUNC_ATTRS
              << " void"
              << _action
              << "(State_t& s) throw (" ROMP_MODEL_EXCEPTION_TYPE ") {"
              "s." ROMP_RULE_ACTION_PREFIX << sig_str << "; }\n";
        // rule_c++;
        r_sep = ", ";
      }
      ruleset_array << indentation() << "\t" ROMP_MAKE_RULESET_STRUCT_FOOTER();
      rs_sep = ",\n\t\t";
      ++info_id;
    }
    // *this << "\n#undef " ROMP_RULESETS_LEN "\n"; 
    // *this << "\n#define " ROMP_RULESETS_LEN " " << rules.size() << "\n"; 
    *this << "\n\n" << indentation() << "// All of the rule sets in one place\n" 
          << ruleset_array.str() << "};\n\n";
  }


  void gen_property_array() {
    std::strstream prop_list;
    prop_list << ROMP_CALLER_PROPERTIES_DECL " = {";
    id_t _lid = 0u;
    size_t count = 0u;
    std::string sep = "";
    for (const Ptr<const PropertyRule>& prop : property_rules) {
      *this << indentation() << "/* --- Property Rule(s) generated by: `" << prop->name << "` (RuleSet expansions) --- */\n";
      int tmp = 0;
      SigPerm sigs(prop);
      for (auto& sig : sigs) {
        std::string _check = ROMP_PROPERTYRULE_PREFIX + prop->name + "__" + int_to_hex();
        out << indentation() 
            << CodeGenerator::M_PROPERTY__FUNC_ATTRS
            << " bool "
            << _check
            << "(const State_t& s) throw (" ROMP_MODEL_EXCEPTION_TYPE ") {"
               "return s." ROMP_PROPERTYRULE_PREFIX << sig << "; }\n";
        prop_list << sep << ROMP_MAKE_PROPERTY_STRUCT(_check,info_id,sig.to_json(),sig.to_string());
        sep = ", ";
      }
      count += sigs.size();
    }
    *this << "\n\n#define " ROMP_PROPERTY_RULES_LEN " (" << count <<  "ul) // the number of property rules (after ruleset expansion) in the model\n"; 
    *this << "\n" << indentation() << "// All of the rule sets in one place\n" 
          << prop_list.str() << "};\n\n";
  }


  void gen_startstate_array() {
    std::strstream prop_list;
    prop_list << ROMP_CALLER_STARTSTATES_DECL " = {";
    id_t info_id = 0u;
    size_t count = 0u;
    std::string sep = "";
    for (const Ptr<const StartState>& startstate : startstates) {
      *this << indentation() << "/* --- StartState Rule(s) generated by: `" << startstate->name << "` (RuleSet expansions) --- */\n";
      int tmp = 0;
      SigPerm sigs(startstate);
      for (auto& sig : sigs) {
        std::string _init = ROMP_PROPERTYRULE_PREFIX + startstate->name + "__" + int_to_hex(count);
        out << indentation() 
            << CodeGenerator::M_STARTSTATE__FUNC_ATTRS
            << " void "
            << _init
            << "(State_t& s) throw (" ROMP_MODEL_EXCEPTION_TYPE ") {"
               "return s." ROMP_PROPERTYRULE_PREFIX << sig << "; }\n";
        prop_list << sep << ROMP_MAKE_STARTSTATE_STRUCT(_init,info_id,sig.to_json(),sig.to_string());
        sep = ", ";
        ++count;
      }
    }
    *this << "\n\n#define " ROMP_STARTSTATE_RULES_LEN " (" << count <<  "ul) // the number of property rules (after ruleset expansion) in the model\n"; 
    *this << "\n" << indentation() << "// All of the rule sets in one place\n" 
          << prop_list.str() << "};\n\n";
  }

  void visit_propertystmt(const PropertyStmt &n) {
    id_t id = next_property_id++;
    switch (n.property.category) {

    case Property::ASSERTION:
      prop_info_list << ROMP_MAKE_PROPERTY_INFO_STRUCT(n,next_assert_id++,n->message,ROMP_PROPERTY_TYPE_ASSERT,to_json(n,"assert")) ",";
      *this << indentation() << "if (not " << ROMP_ASSERTION_HANDLER(n,prop_id) << ")\n";
      indent();
      *this << indentation() << "failed_assertion(\""
            << escape(n.message == "" ? n.property.expr->to_string() : n.message)
            << "\");\n";
      dedent();
      // *this << indentation() << "}";
      break;

    case Property::ASSUMPTION:
      *this << indentation() << "if (!" << *n.property.expr << ") {\n";
      indent();
      *this << indentation() << "failed_assumption(\""
            << escape(n.message == "" ? n.property.expr->to_string() : n.message)
            << "\");\n";
      dedent();
      *this << indentation() << "}";
      break;

    case Property::COVER:
      *this << indentation() << "if " << *n.property.expr << " {\n";
      indent();
      *this << indentation() << "cover(\""
            << escape(n.message == "" ? n.property.expr->to_string() : n.message)
            << "\");\n";
      dedent();
      *this << indentation() << "}";
      break;

    case Property::LIVENESS:
      throw Error("liveness properties are not supported as statements only global rules!", n.loc);
      *this << indentation() << "if " << *n.property.expr << " {\n";
      indent();
      *this << indentation() << "liveness(\""
            << escape(n.message == "" ? n.property.expr->to_string() : n.message)
            << "\");\n";
      dedent();
      *this << indentation() << "}";
      break;
    }

    emit_trailing_comments(n);
    *this << "\n";
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

    *this << indentation() << "typedef " << value_type << "range_t;\n"
          << indentation() << "typedef " << value_type << "scalarset_t;\n";

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

    *this << "\n" << indentation() << "/* ======= Model State Variables ====== */\n\n";
    // split.state_var_decl.visit(*this);
    for (const Ptr<const VarDecl> &var : sorter.state_var_decls)
      *this << *var << "\n"; 
    gen_state_to_json(sorter.state_var_decls);

    *this << "\n" << indentation() << "/* ======= Murphi Model Functions ====== */\n\n"; // << std::flush;
    // split.funct_decls.visit(*this);
    for (const Ptr<const Function> &funct : sorter.funct_decls)
      *this << *funct << "\n";

    *this << "\n" << indentation() << "/* ======= Murphi Model Rules, StartStates & Invariants ====== */\n\n"; // << std::flush;
    // split.rule_decls.visit(*this);
    for (const Ptr<const Rule> &_r : sorter.rule_decls) {
      std::vector<Ptr<Rule>> rs = _r->flatten();
      for (const Ptr<const Rule> &r2 : rs)
        *this << *r2 << "\n";
    }
    dedent();
    *this << "\n" << indentation() << "};\n"; // << std::flush;

    dedent();
    *this << indentation() << "}\n\n";
    
    *this << indentation() << "/* ======= Murphi Model Infos & MetaData ====== */\n"; // << std::flush;
    *this << indentation() << "namespace " ROMP_INFO_NAMESPACE_NAME " {\n";
    indent();
    *this << "#define " ROMP_FUNCTS_LEN " (" << sorter.funct_decls.size()  "ul) // the number of functions & procedures in the model\n"
          << indentation() << "// the info/metadata about the functions/procedures in the model\n"
          << indentation() << sorter.funct_info_list.str() << '];\n'
          << "#define " ROMP_ERRORS_LEN " (" << next_error_id << "ul) // the number of error statements in the model\n"
          << indentation() << "// the info/metadata about the murphi error statements in the model\n"
          << indentation() << error_info_list.str() << '};\n'
          << "#define " ROMP_PROPERTY_INFOS_LEN " (" << next_property_id <<  "ul) // the number of property statements & rules in the model\n"
          // << "#define " ROMP_PROPERTY_RULES_LEN " (" << property_rules.size() <<  "ul) // the number of property rules (after ruleset expansion) in the model\n"
          << indentation() << "// the info/metadata about the murphi properties in the model\n"
          << indentation() << sorter.prop_info_list.str() << prop_info_list.str() << '};\n'
          << "#define " ROMP_STARTSTATE_INFOS_LEN " (" << startstates.size() <<  "ul) // the number of start state rules (before ruleset expansions) in the model\n"
          // << "#define " ROMP_STARTSTATE_RULES_LEN " (" << startstates.size() <<  "ul) // the number of start state rules (after ruleset expansions) in the model\n"
          << indentation() << "// the info/metadata about the startstate rules in the model\n"
          << indentation() << sorter.startstate_info_list.str() << '};\n'
          << "#define " ROMP_RULESETS_LEN " (" << rules.size()  "ul) // the number of rules (before ruleset expansion) in the model\n"
          << indentation() << "// the info/metadata about the rules in the model\n"
          << indentation() << sorter.rule_info_list.str() << '};\n';
    dedent();
    *this << "\n" << indentation() << "}\n\n";

    *this << indentation() << "/* ======= Romp CALLER Lists ====== */\n";
    *this << indentation() << "namespace " ROMP_CALLER_NAMESPACE_NAME " {\n";
    indent();

    *this << indentation() << "typedef " ROMP_STATE_TYPE " State_t; // type mask for the generated state object\n";

    // gen_ruleset_array();
    // gen_property_array();
    // gen_startstate_array();

    dedent();
    *this << "\n" << indentation() << "}\n\n"; // << std::flush;
  }

  virtual ~CGenerator() = default;

  // CGenerator& operator << (std::ostream& os) {
  //   out << os;
  //   return *this;
  // }

  std::string to_json(const ErrorStmt& rule) {
    std::strstream buf;
    buf << "{\"$type\":\"error-statement\",";
    buf << "\"label\":\"" << escape(rule.message) << "\","
            "\"loc\":{\"$type\":\"location\","
                      "\"file\":\"" << *rule.loc.begin.filename << "\","
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
        << "\"label\":\"" << escape(prop.message == "" ? prop.property.expr->to_string() : prop.message) << "\","
        << "\"expr\":\"" << escape(prop.property.expr->to_string()) << "\","
            "\"loc\":{\"$type\":\"location\","
                      "\"file\":\"" << *prop.loc.begin.filename << "\","
    //                   "\"inside\":\"";
    // // TODO ...
    // buf <<            "\","
                      "\"start\":["<< prop.loc.begin.line << "," << prop.loc.begin.column << "],"  
                      "\"end\":["<< prop.loc.end.line << "," << prop.loc.end.column << "]}";
    return buf.str();
  }

  CGenerator& operator << (const char* str) { out << str; return *this; }
  CGenerator& operator << (const std::string str) { out << str; return *this; }
  CGenerator& operator << (const int val) { out << val; return *this; }
  CGenerator& operator << (const rumur::Node& n) { dispatch(n); return *this; }
  CGenerator& operator << (const Sig& sig) { out << sig; return *this; }
  CGenerator& operator << (const SigParam& param) { out << param; return *this; }
};

const std::unordered_set<std::string> CGenerator::RESERVED_VAR_NAMES{ROMP_RESERVED_NAMES};


std::ostream& operator << (std::ostream& out, const Sig& sig) {
  out << sig.perm.rule->name << "(";
  const char* sep = "";
  for (const SigParam& param : sig) {
    out << sep << param;
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

std::string trim(const std::string &s)
{
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }
 
    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
 
    return std::string(start, end + 1);
}

void generate_c(const Node &n, const std::vector<Comment> &comments, bool pack,
                std::ostream &out, const std::string& in_filename, const std::string& build_cmds) {

  out << ROMP_GENERATED_FILE_PREFACE("\tGenerated code for a romp \"parallel random walker\" verification tool based off of the Murphi Model described in:\n"
                                     "\t\tOriginal Murphi Model: " + in_filename + "\n"
                                     "\tPlease build with the following command:\n") "\n";

  out << "\n#define __romp__GENERATED_CODE\n\n";
  out << "\n#define _ROMP_STATE_TYPE " ROMP_STATE_TYPE "\n\n";
  out << "\n#define __model__filename \"" << in_filename << "\"\n\n";
  auto _tmp = trim(in_filename);
  auto _count = std::count(_tmp.begin(), _tmp.end(), ' ');
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
