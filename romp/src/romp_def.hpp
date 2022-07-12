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

#define ROMP_UTIL_NAMESPACE_NAME "romp"

#define ROMP_UTIL_NAMESPACE "::" ROMP_UTIL_NAMESPACE_NAME

#define ROMP_RAND_WALKER_TYPE ROMP_UTIL_NAMESPACE "::" "IRandWalker"

#define ROMP_STATE_RAND_WALKER_VAR "__rw__"

#define ROMP_RAND_WALKER_REFERENCE  "" ROMP_RAND_WALKER_TYPE "& " ROMP_STATE_RAND_WALKER_VAR "; // reference to associated romp rand walker"

#define ROMP_MODEL_NAMESPACE_NAME "__model__"

#define ROMP_CALLER_NAMESPACE_NAME "__caller__"

#define ROMP_INFO_NAMESPACE_NAME "__info__"

#define ROMP_STATE_CLASS_NAME "__Model__"

#define ROMP_STATE_TYPE "::" ROMP_MODEL_NAMESPACE_NAME "::" ROMP_STATE_CLASS_NAME

#define ROMP_TYPE_NAMESPACE "__type__"

// #define ROMP_CALLABLE_RULE_DECL "const ::romp::Rule ";
#define ROMP_RULESETS_LEN "ROMP_RULESETS_LEN"
#define ROMP_CALLABLE_RULESETS_DECL "const " ROMP_UTIL_NAMESPACE "::RuleSet RULESETS["ROMP_RULESETS_LEN"]";

#define ROMP_PROPERTIES_LEN "ROMP_PROPERTIES_LEN"
#define ROMP_CALLABLE_PROPERTIES_DECL "const " ROMP_UTIL_NAMESPACE "::Property PROPERTIES["ROMP_PROPERTIES_LEN"]"

#define ROMP_STARTSTATES_LEN "ROMP_STARTSTATES_LEN"
#define ROMP_CALLABLE_STARTSTATES_DECL "const " ROMP_UTIL_NAMESPACE "::StartState STARTSTATES["ROMP_STARTSTATES_LEN"]"

// #define ROMP_STARTSTATES_LEN "ROMP_ERRORS_LEN"
// #define ROMP_CALLABLE_STARTSTATES_DECL "const " ROMP_UTIL_NAMESPACE "::ErrorInfo STARTSTATES["ROMP_STARTSTATES_LEN"]"

#define ROMP_MODEL_EXCEPTION_TYPE "" ROMP_UTIL_NAMESPACE "::" "ModelError"

#define ROMP_MODEL_TEST_EXCEPTION_TYPE "" ROMP_UTIL_NAMESPACE "::" "ModelPropertyError"

#define ROMP_JSON_TYPE "nlohmann::json"

#define ROMP_SHOW_TYPE_OPTION_EXPR "options.SHOW_TYPE"

#define ROMP_GENERATED_FILE_PREFACE(p) \
        "/*\n" p "\n*/\n"

#define ROMP_PREDEFINED_TYPES "boolean"

#define ROMP_STARTSTATE_PREFIX "__StartState__"
#define ROMP_RULE_GUARD_PREFIX "__Rule_guard__"
#define ROMP_RULE_ACTION_PREFIX "__Rule_action__"
#define ROMP_PROPERTYRULE_PREFIX "__Property__"

#define ROMP_MAKE_LOCATION_STRUCT(_name,_loc) "{\"" << escape(_name) << "\","\
                                  "{" << (const int)_loc.begin.line << "," << (const int)_loc.begin.column << "},"\
                                  "{" << (const int)_loc.end.line << "," << (const int)_loc.end.column << "}}"

#define ROMP_MAKE_RULESET_STRUCT_HEADER(_name,_n) "{" << (_name) << ", " ROMP_MAKE_LOCATION_STRUCT((_name),(_n).loc) ", {" 
#define ROMP_MAKE_RULESET_STRUCT_FOOTER(...) "}}" 
#define ROMP_MAKE_RULE_STRUCT(_guard, _action, _params) "{" + (_guard) + "," + (_action) + ",\"" + _params + "\"}"


#define ROMP_MAKE_MODEL_EXCEPTION(_name, _n) ROMP_MODEL_EXCEPTION_TYPE "(" \
                                             "\"Exception caught! see trace for details...\", " ROMP_MAKE_LOCATION_STRUCT((_name),(_n).loc) ")"

#define ROMP_PROPERTY_TYPE_BASE ROMP_UTIL_NAMESPACE "::PropertyType"
#define ROMP_PROPERTY_TYPE_INVAR ROMP_PROPERTY_TYPE_BASE "::INVARIANT"
#define ROMP_PROPERTY_TYPE_ASSERT ROMP_PROPERTY_TYPE_BASE "::ASSERTION"
#define ROMP_PROPERTY_TYPE_ASSUME ROMP_PROPERTY_TYPE_BASE "::ASSUMPTION"
#define ROMP_PROPERTY_TYPE_COVER ROMP_PROPERTY_TYPE_BASE "::COVER"
#define ROMP_PROPERTY_TYPE_LIVENESS ROMP_PROPERTY_TYPE_BASE "::LIVENESS"

#define ROMP_PROPERTY_HANDLER_BASE "this->" ROMP_RAND_WALKER_VAR
#define ROMP_PROPERTY_HANDLER_INVAR ROMP_PROPERTY_HANDLER_BASE "." "invariant_handler"
#define ROMP_PROPERTY_HANDLER_ASSERT ROMP_PROPERTY_HANDLER_BASE "." "assertion_handler"
#define ROMP_PROPERTY_HANDLER_ASSUME ROMP_PROPERTY_HANDLER_BASE "." "assumption_handler"
#define ROMP_PROPERTY_HANDLER_COVER ROMP_PROPERTY_HANDLER_BASE "." "cover_handler"
#define ROMP_PROPERTY_HANDLER_LIVENESS ROMP_PROPERTY_HANDLER_BASE "." "liveness_handler"
#define ROMP_ERROR_HANDLER ROMP_PROPERTY_HANDLER_BASE "." "error_handler"

#define ROMP_MAKE_MODEL_TEST_EXCEPTION(_label, _type, _n) ROMP_MODEL_TEST_EXCEPTION_TYPE "(" \
                                                          (_type) "," << (_n).property

#define ROMP_RESERVED_NAMES "" ROMP_STATE_CLASS_NAME, "to_json", \
                            ROMP_MODEL_NAMESPACE_NAME, ROMP_CALLER_NAMESPACE_NAME, ROMP_UTIL_NAMESPACE_NAME, \
                            "boolean", ROMP_STATE_RAND_WALKER_VAR, "std", ROMP_INFO_NAMESPACE_NAME

#endif
