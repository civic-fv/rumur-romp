/**
 * @proj romp
 * @author Andrew Osterhout
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp_def.hpp
 * 
 * @brief A bunch of definitions for configuration 
 *        and other details in the romp codegen tool.
 * 
 * @date 2022/05/11
 * @version 0.1
 */

#ifndef _romp_src_romp_def_hpp
#define _romp_src_romp_def_hpp

#define ROMP_MODEL_NAMESPACE_NAME "__model__"

#define ROMP_CALLER_NAMESPACE_NAME "__caller__"

#define ROMP_STATE_CLASS_NAME "__State__"

#define ROMP_STATE_TYPE "::" ROMP_MODEL_NAMESPACE_NAME "::" ROMP_STATE_CLASS_NAME

#define ROMP_TYPE_NAMESPACE "__type__"

// #define ROMP_CALLABLE_RULE_DECL "const ::romp::Rule ";
#define ROMP_CALLABLE_RULESET_DECL "const ::romp::RuleSet RULESETS[]";

#define ROMP_CALLABLE_INVARIANT_DECL "const ::romp::Invariant INVARIANTS[]"

#define ROMP_CALLABLE_STARTSTATE_DECL "const ::romp::StartState STARTSTATES[]"

#define ROMP_MODEL_EXCEPTION_TYPE "::romp::ModelException"

#define ROMP_MODEL_TEST_EXCEPTION_TYPE "::romp::ModelException_Test"

#define ROMP_RESERVED_NAMES "rules", "invariants", "startstates", "__State__", "RandWalker", "to_json", \
                            "__model__", "__caller__", "romp" , "boolean"

#define ROMP_JSON_TYPE "nlohmann::json"

#define ROMP_SHOW_TYPE_OPTION_EXPR "options.SHOW_TYPE"

#define ROMP_GENERATED_FILE_PREFACE(p) \
        "/*\n" p "\n*/\n"

#define ROMP_PREDEFINED_TYPES "boolean"

#define ROMP_STARTSTATE_PREFIX "__StartState__"
#define ROMP_RULE_GUARD_PREFIX "__Rule_guard__"
#define ROMP_RULE_ACTION_PREFIX "__Rule_action__"
#define ROMP_PROPERTYRULE_PREFIX "__Invariant__"

#define ROMP_MAKE_LOCATION_STRUCT(_name,_loc) "{\"" << escape(_name) << "\","\
                                  "{" << _loc.begin.line << "," << _loc.begin.column << "},"\
                                  "{" << _loc.end.line << "," << _loc.end.column << "}}"

#define ROMP_MAKE_RULESET_STRUCT_HEADER(_name,_loc) "{" << _name << ", " ROMP_MAKE_LOCATION_STRUCT(_name,_loc) ", {" 
#define ROMP_MAKE_RULESET_STRUCT_FOOTER(...) "}}" 
#define ROMP_MAKE_RULE_STRUCT(_guard, _action, _params) "{" + _guard + "," + _action + "," + _params + "}"


#define ROMP_MAKE_MODEL_EXCEPTION(_msg, _name, _loc) ROMP_MODEL_EXCEPTION_TYPE "(" \
                                               "\"Exception caught\", " ROMP_MAKE_LOCATION_STRUCT(_name,_loc) ")

#define ROMP_TRY_HEADER << "try {"
#define ROMP_TRY_FOOTER(n) << "} catch (const std::exception& ex) { " \
                           "throw " ROMP_MAKE_MODEL_EXCEPTION("Exception caught", n.name, n.loc) "); }\n";

#ifdef _ROMP_ENABLE_CUSTOM_RUMUR_TYPE_TRAVERSAL

#endif

#endif
