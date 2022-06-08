/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file Model.hpp
 *
 * @brief   A Class that is a intermediary representation of the model,
 *        used to allow for generating the Cpp romp tools
 *        in a way that allows full context of the model.
 *
 * @date 2022/05/13
 * @version 0.1
 */

#include <rumur/rumur.h>

#include <string>
// #include <stack>
#include <vector>
#include <list>
// #include <iostream>
// #include <unordered_map>
#include <unordered_set>

#include "../CodeGenerator.h"

namespace romp {

/**
 * @brief   A Class that is a intermediary representation of the model,
 *        used to allow for generating the Cpp romp tools
 *        in a way that allows full context of the model.
 *          It allows the following:
 *        \li Keeping track of all types (generating names for anonymous types)
 *        \li Keeping track of all "variables" respecting scope.
 *        \li Keeping track of all functions, procedures, & rules.
 *        \li allowing lookup of all above mentioned items.
 *        \li preserving a notion of scope.
 */
class TModel : public rumur::BaseTraversal,
               public romp::CodeGenerator {

  // << ===================================== Class Members ====================================== >> 
public: // ---- Public Class Members ---- //


private: // ---- Private Class Members ---- //
  
  const rumur::Node& root;
  const rumur::Ptr<rumur::Model> _model;

  std::vector<rumur::VarDecl&> state_vars;
  
  std::unordered_set<std::string> cTypeNames;
  // std::unordered_map<size_t,std::string> cTypeName;  // might end up not needing this

  std::list<Ptr<Node>> _children;
  std::list<rumur::Ptr<rumur::Node>>::iterator _top;

  id_t anon_id = 0;


  // << ============================= Constructors & Deconstructor =============================== >> 
public:
  TModel(rumur::Node& root) : root(root) { }
  ~TModel();


  // << ========================== Public/External Function Functions ============================ >> 
public:




  // << =========================== Internal/Private Helper Functions ============================ >> 
private:

  const std::string gen_new_anon_name();
  const std::string make_name_unique(const std::string old);
  void insert_to_global_top(Ptr<TypeDecl> n);
  void insert_to_global_top(Ptr<ConstDecl> n);


  // << =========================== Model Processing Helper Functions ============================ >> 
public:
  // - most useful ---- 
  void visit_model(rumur::Model &n) final;
  void visit_array(rumur::Array &n) final;
  void visit_record(rumur::Record &n) final;
  void visit_function(Function &n) final;
  void visit_simplerule(SimpleRule &n) final;
  void visit_startstate(StartState &n) final;

  // - useful for debugging ---- 
  void visit_typedecl(rumur::TypeDecl &n) final;
  void visit_constdecl(ConstDecl &n) final;
  void visit_vardecl(VarDecl &n) final;
  void visit_enum(rumur::Enum &n) final;
  void visit_range(rumur::Range &) final;
  void visit_scalarset(rumur::Scalarset &) final;
  void visit_typeexprid(rumur::TypeExprID &n) final;
  void visit_propertyrule(PropertyRule &n) final;
  

  // void visit_constdecl(ConstDecl &n) final; // pretty sure I don't need this!


  // TModel &operator<<(std::string &s);
  // TModel &operator<<(rumur::Node &n);
  
};

} // namespace romp
