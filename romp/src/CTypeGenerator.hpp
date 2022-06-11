/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file CTypeGenerator.hpp
 * 
 * @brief An AST traversing structure that can only handle generating Types.
 * 
 * @date 2022/06/06
 * @version 0.1
 */

#include "CLikeGenerator.hpp"
// #include <rumur/rumur.h>
#include <unordered_set>


namespace romp {


class CTypeGenerator : public CLikeGenerator {
private:


protected:
  

public:


public:

  void visit_typedecl(const rumur::TypeDecl &n) final;
  void visit_array(const rumur::Array &n) final;
  void visit_enum(const rumur::Enum &n) final;
  void visit_range(const rumur::Range &) final;
  void visit_record(const rumur::Record &n) final;
  void visit_scalarset(const rumur::Scalarset &) final;



private:
  void emit_json_converter(const std::string &name, const rumur::Ptr<rumur::TypeExpr> &te);
  void emit_json_converter(const std::string &name, const rumur::Array &te);
  void emit_json_converter(const std::string &name, const rumur::Enum &te);
  void emit_json_converter(const std::string &name, const rumur::Record &te);
  void emit_json_converter(const std::string &name, const rumur::Range &te);
  void emit_json_converter(const std::string &name, const rumur::Scalarset &te);
  void emit_json_converter(const std::string &name, const rumur::TypeExprID &te);

};

}
