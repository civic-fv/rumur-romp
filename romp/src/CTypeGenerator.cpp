/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file CTypeGenerator.cpp
 * 
 * @brief An AST traversing structure that can only handle generating Types.
 * 
 * @date 2022/06/10
 * @version 0.1
 */

#include "CTypeGenerator.hpp"
#include "options.h"
#include "romp_def.hpp"
#include <rumur/rumur.h>

using namespace rumur;

namespace romp {


void CTypeGenerator::visit_typedecl(const TypeDecl &n) {
  if (CLikeGenerator::reserved_type_names.find(n.name) != CLikeGenerator::reserved_type_names.end())
    throw Error("Tried to define a type with a reserved/built-in type name!!", n.loc);
  if (emitted_tDecls.find(n.name) != emitted_tDecls.end())
    throw Error("Tried to redefine an already defined type!", n.loc);

  // If we are typedefing something that is an enum, save this for later lookup.
  // See CGenerator/HGenerator::visit_constdecl for the purpose of this.
  if (auto e = dynamic_cast<const Enum *>(n.value.get()))
    enum_typedefs[e->unique_id] = n.name;

  *this << indentation() << "typedef " << *n.value << " " << n.name << ";";
  emit_trailing_comments(n);
  *this << "\n";

  emitted_tDecls.insert(n.name);
  emit_json_converter(n.name, n.value);
  *this << "\n";
}


void CTypeGenerator::visit_array(const Array &n) {

  // check_type_ref(n, n.element_type);
  // check_type_ref(n, n.index_type);


  mpz_class count = n.index_type->count();

  assert(count > 0 && "index type of array does not include undefined");
  count--;

  // wrap the array in a struct so that we do not have the awkwardness of
  // having to emit its type and size on either size of another node
  *this << "struct " << (pack ? "__attribute__((packed)) " : "") << "{ "
        << *n.element_type 
        << " data[" << count.get_str() << "];";

  // // The index for this array may be an enum declared inline:
  // //
  // //   array [enum {A, B}] of foo
  // //
  // // If so, we need to emit it somehow so that the enum’s members can be
  // // referenced later. We define it within this struct to avoid any awkward
  // // lexical issues.
  // if (auto e = dynamic_cast<const Enum *>(n.index_type.get())) {
  //   *this << " " << *e << ";";
  // }
  *this << " }";

  //TODO: insert to_string or to_json method here.

}

void CTypeGenerator::emit_json_converter(const std::string &name, const Array &te) {
  std::string et_str = "";
  if (auto et = dynamic_cast<const TypeExprID *>(te.element_type.get()))
    et_str += et->referent->name;
  else
    et_str += value_type;
  *this << indentation() << "void to_json(" ROMP_JSON_TYPE "& j, const " << name << "& data) { "
           "if (" << ROMP_SHOW_TYPE_OPTION_EXPR << ") {" 
              "j = " ROMP_JSON_TYPE "{{\"type\",\"" << name << te.to_string() << "\"},"
                            "{\"value\", std::vector<" << et_str << ">(std::begin(data), std::end(data))}};"
           "} else {to_json(j, std::vector<" << et_str << ">(std::begin(data), std::end(data))));} }\n";
}


void CTypeGenerator::visit_enum(const Enum &n) {
  *this << "enum { ";
  for (const std::pair<std::string, location> &m : n.members) {
    *this << m.first << ", ";
  }
  *this << "}";
}

void CTypeGenerator::emit_json_converter(const std::string &name, const Enum &te) {
  *this << indentation() << "NLOHMANN_JSON_SERIALIZE_ENUM( " << name << ", { ";
  for (auto &m : te.members)
    *this << "{" << m.first << "," 
          "((" ROMP_SHOW_TYPE_OPTION_EXPR ") ? ("
          " \"" << name << "::" << m.first << "\" "
          ") : (\"" << m.first << "\")},";
  *this << " })\n";
}


void CTypeGenerator::visit_range(const Range &) { *this << value_type; }


void CTypeGenerator::emit_json_converter(const std::string &name, const Range &te) {
  *this << indentation() << "void to_json(" ROMP_JSON_TYPE "& j, const " << name << "& data) { "
        "if (" ROMP_SHOW_TYPE_OPTION_EXPR ") {" 
          "j = " ROMP_JSON_TYPE "{{\"type\",\"" << name << ": " << te.to_string() << "\"}, "
                          "{\"value\",(" << value_type << ")data}};"
        "} else {"
          "to_json(j,(" << value_type << ")data);} }\n";
}


void CTypeGenerator::visit_record(const Record &n) {
  *this << "struct " << (pack ? "__attribute__((packed)) " : "") << "{\n";
  indent();
  indent();
  for (const Ptr<VarDecl> &f : n.fields) {
    check_type_ref(*f, f->type);
    emit_leading_comments(*f);
    *this << indentation() << *(f->type) << " " << f->name << ";";
    emit_trailing_comments(*f);
    *this << "\n";
  }
  dedent();
  *this << indentation() << "}";
  dedent();
}


void CTypeGenerator::emit_json_converter(const std::string &name, const Record &te) {
  std::string conv_str = ROMP_JSON_TYPE "{";
  for (const Ptr<VarDecl> &f : te.fields)
    conv_str += "{\"" + f->name + "\", data." + f->name + "},";
  conv_str += "}";
  *this << indentation() << "void to_json(" ROMP_JSON_TYPE "& j, const " << name << "& data) { "
        "j = (" ROMP_SHOW_TYPE_OPTION_EXPR ") ? (" 
            ROMP_JSON_TYPE "{{\"type\",\"" << name << ": " << te.to_string() << "\"},"
            "{\"value\"," << conv_str << "}}"
          ") : (" << conv_str << "); }";
}


void CTypeGenerator::visit_scalarset(const Scalarset &) { *this << value_type; }


void CTypeGenerator::emit_json_converter(const std::string &name, const Scalarset &te) {
  *this << indentation() << "void to_json(" ROMP_JSON_TYPE "& j, const " << name << "& data) { "
        "j = (" ROMP_SHOW_TYPE_OPTION_EXPR ") ? (" 
          ROMP_JSON_TYPE "{{\"type\",\"" << name << ": " << te.to_string() << "\"}, "
                          "{\"value\",(" << value_type << ")data}} "
          ") : ( to_json((" << value_type << ")data) ); }\n";
}


void CTypeGenerator::emit_json_converter(const std::string &name, const TypeExprID &te) {
  *this << indentation() << "void to_json(" ROMP_JSON_TYPE "& j, const " << name << "& data) { "
        "to_json(j,(" << te.referent->name << ")data); "
        "}\n";
}


void CTypeGenerator::emit_json_converter(const std::string &name, const Ptr<TypeExpr> &te) {
  if (auto _a = dynamic_cast<const Array *>(te.get()))
    emit_json_converter(name, *_a);
  else if (auto _e = dynamic_cast<const Enum *>(te.get()))
    emit_json_converter(name, *_e);
  else if (auto _ra = dynamic_cast<const Range *>(te.get()))
    emit_json_converter(name, *_ra);
  else if (auto _re = dynamic_cast<const Record *>(te.get()))
    emit_json_converter(name, *_re);
  else if (auto _s = dynamic_cast<const Scalarset *>(te.get()))
    emit_json_converter(name, *_s);
  else if (auto _id = dynamic_cast<const TypeExprID *>(te.get()))
    emit_json_converter(name, *_id);
  else
    throw Error("Unrecognized Type!!", te->loc);
}

void CTypeGenerator::__throw_unreachable_error(const Node &n) {
  throw Error("The CType Code generator encountered an unsupported syntactic object during generation!!", n.loc);
}

}
