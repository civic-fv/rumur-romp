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

#define ROMP_STATE_TYPE "::" ROMP_MODEL_NAMESPACE "::" ROMP_STATE_CLASS_NAME

#define ROMP_TYPE_NAMESPACE "__type__"

#define ROMP_CALLABLE_RULE_TYPE "::romp::Rule";

#define ROMP_CALLABLE_INVARIANT_TYPE "::romp::Invariant"

#define ROMP_CALLABLE_STARTSTATE_TYPE "::romp::StartState"

#define ROMP_MODEL_EXCEPTION "::romp::ModelException"

#define ROMP_MODEL_TEST_EXCEPTION "::romp::ModelException_Test"

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

#ifdef _ROMP_ENABLE_CUSTOM_RUMUR_TYPE_TRAVERSAL

#endif

#endif
