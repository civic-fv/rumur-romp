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

#include "TModel.hpp"

#include "../../../common/escape.h"
#include "../../../common/isa.h"
#include "../options.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <ctype.h>
#include <gmpxx.h>
#include <rumur/rumur.h>
// #include <rumur/except.h>
// #include <rumur/Comment.h>
// #include <string>
// #include <utility>
// #include <vector>

// #include <functional>


namespace romp {
using namespace rumur;

// << ========================================================================================== >> 
// <<                               CONSTRUCTORS & DECONSTRUCTORS                                >> 
// << ========================================================================================== >> 

TModel::~TModel() {}



// << ========================================================================================== >> 
// <<                                 PRIVATE HELPER FUNCTIONS                                   >> 
// << ========================================================================================== >> 

// template< typename T >
// std::string int_to_hex( T i )
// {
//   std::stringstream stream;
//   stream << "0x" 
//          << std::setfill ('0') << std::setw(sizeof(T)*2) 
//          << std::hex << i;
//   return stream.str();
// }

// const std::string TModel::gen_new_anon_name() {
//   return "anon"+int_to_hex(anon_id++)+"_t";
// }

const std::string TModel::gen_new_anon_name() {
  char buff[24];
  sprintf(buff, "anon0x%x_t", anon_id++);
  return std::string(buff);
}

const std::string TModel::make_name_unique(const std::string old) {
  char buff[old.size()+24];
  sprintf(buff, (old+"__0x%x_t").c_str(), anon_id++);
  return std::string(buff);
}

void TModel::insert_to_global_top(Ptr<TypeDecl> n) {
  if (cTypeNames.find(n->name) != cTypeNames.end()) {
    n->name = make_name_unique(n->name);
  }
  _top = _children.insert(_top, Ptr<TypeDecl>(n->clone()));
  _top++;
  cTypeNames.insert(n->name);
}

void TModel::insert_to_global_top(Ptr<ConstDecl> n) {
  if (cTypeNames.find(n->name) != cTypeNames.end()) {
    n->name = make_name_unique(n->name);
  }
  _top = _children.insert(_top, Ptr<ConstDecl>(n->clone()));
  _top++;
  cTypeNames.insert(n->name);
}



// << ========================================================================================== >> 
// <<                                  PARSING UTIL FUNCTIONS                                    >> 
// << ========================================================================================== >> 


void TModel::visit_model(Model &n) {
  // std::list<Ptr<Node>> _children;

  // emit_leading_comments(n);

  for (Ptr<Node> &c : n.children) {

    // emit_leading_comments(*c);

    // if this is a rule, first flatten it so we do not have to deal with the
    // hierarchy of rulesets, aliasrules, etc.
    if (auto r = dynamic_cast<Rule *>(c.get())) {
      std::vector<Ptr<Rule>> rs = r->flatten();
      for (Ptr<Rule> &r2 : rs) {
        _children.insert(_children.end(), r2.make());
        dispatch(*r2);
      }

    } else {
      if (auto td = dynamic_cast<TypeDecl *>(c.get())) 
        insert_to_global_top(Ptr<TypeDecl>(td->clone()));
      else if (auto cd = dynamic_cast<ConstDecl *>(c.get())) 
        insert_to_global_top(Ptr<ConstDecl>(cd->clone()));
      else 
        dispatch(*c);
    }
  }
  n.children = std::vector<Ptr<Node>>(_children.begin(), _children.end());
}


// void TModel::visit_constdecl(ConstDecl &n) {}


void TModel::visit_typedecl(TypeDecl &n) {
  // shouldn't have to do anything here.
}


void TModel::visit_typeexprid(TypeExprID &n) {
  // shouldn't need to do anything here
}




void TModel::visit_enum(Enum &n) {
  // shouldn't need to do anything here
}


void TModel::visit_array(Array &n) {
  //TODO: handle the cases of non-TypeExprId TypeExpr values & the nested enums.

  if (auto et_id = dynamic_cast<TypeExprID *>(n.element_type.get())) {
    // do nothing
  } else {
    std::string name = gen_new_anon_name();
    Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(n.element_type->clone()), n.element_type->loc));
    insert_to_global_top(decl);
    n.element_type = Ptr<TypeExprID>(new TypeExprID(name, decl, n.element_type->loc));
  }
  

  // mpz_class count = n.index_type->count();

  // assert(count > 0 && "index type of array does not include undefined");
  // count--;

  // // wrap the array in a struct so that we do not have the awkwardness of
  // // having to emit its type and size on either size of another node
  // *this << "struct " << (pack ? "__attribute__((packed)) " : "") << "{ "
  //       << *n.element_type << " data[" << count.get_str() << "];";

  // The index for this array may be an enum declared inline:
  //
  //   array [enum {A, B}] of foo
  //
  // If so, we need to emit it somehow so that the enum’s members can be
  // referenced later. We define it within this struct to avoid any awkward
  // lexical issues.
  if (auto e = dynamic_cast<const Enum *>(n.index_type.get())) {
    std::string name = gen_new_anon_name();
    Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<Enum>(e->clone()), e->loc));
    insert_to_global_top(decl);
    n.index_type = Ptr<TypeExprID>(new TypeExprID(name, decl, e->loc));
  }

  // *this << " }";
}


void TModel::visit_range(Range &) {
  // shouldn't need to do anything here
}


void TModel::visit_record(Record &n) {
  //TODO: handle the cases of non-TypeExprId TypeExpr values
  std::vector<Ptr<VarDecl>> _fields(n.fields.size());

  for (const Ptr<VarDecl> &f : n.fields) {  // pre output any inplace type definitions
    
    if (auto et_id = dynamic_cast<const TypeExprID *>(f->type.get())) {
      _fields.push_back(Ptr<TypeExprID>(et_id->clone()));
    } else {
      std::string name = gen_new_anon_name();
      Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(f->type->clone()), f->loc));
      insert_to_global_top(decl);
      _fields.push_back(Ptr<TypeExpr>(new TypeExprID(name, decl, f->loc)));
    }
  }
  n.fields = _fields;
}


void TModel::visit_scalarset(Scalarset &n) { 
  // shouldn't need to do anything here
}


void TModel::visit_vardecl(VarDecl &n) {
  // DEBUG: check that no VarDecl is reached that has a non-TypeExprId type! 
  if (auto et_id = dynamic_cast<const TypeExprID *>(n.type.get())) {
    // do nothing
  } else {
    throw new Error("An inline type declaration was reached before being processed by the type handler!!",
                n.loc);
  }
}


void TModel::visit_function(Function &n) {
  //TODO: handle the cases of non-TypeExprID TypeExpr values

  if (auto et_id = dynamic_cast<TypeExprID *>(n.return_type.get())) {
    // do nothing
  } else {
    std::string name = gen_new_anon_name();
    Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(n.return_type->clone()), n.return_type->loc));
    insert_to_global_top(decl);
    n.return_type = Ptr<TypeExprID>(new TypeExprID(name, decl, n.return_type->loc));
  }

  for (Ptr<VarDecl> &p : n.parameters)
    if (auto et_id = dynamic_cast<TypeExprID *>(p->type.get())) {
      // do nothing
    } else {
      std::string name = gen_new_anon_name();
      Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(p->type->clone()), p->type->loc));
      insert_to_global_top(decl);
      p->type = Ptr<TypeExprID>(new TypeExprID(name, decl, p->type->loc));
    }

  std::vector<Ptr<Decl>> _decls(n.decls.size());
  for (Ptr<Decl> &d : n.decls)
    if (auto vd = dynamic_cast<VarDecl *>(d.get())) {
      if (auto et_id = dynamic_cast<TypeExprID *>(vd->type.get())) {
        // do nothing
      } else {
        std::string name = gen_new_anon_name();
        Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(vd->type->clone()), vd->type->loc));
        insert_to_global_top(decl);
        vd->type = Ptr<TypeExprID>(new TypeExprID(name, decl, vd->type->loc));
      }
      _decls.push_back(d);
    } else if (auto td = dynamic_cast<TypeDecl *>(d.get())) {
      insert_to_global_top(Ptr<TypeDecl>(td->clone()));
    } else if (auto cd = dynamic_cast<ConstDecl *>(d.get())) {
      insert_to_global_top(Ptr<ConstDecl>(cd->clone()));
    }
  n.decls = _decls;
}


void TModel::visit_propertyrule(PropertyRule &n) {
  // I shouldn;t need to do anything here
}


void TModel::visit_simplerule(SimpleRule &n) {
  //TODO: handle the cases of non-TypeExprId TypeExpr values

  std::vector<Ptr<Decl>> _decls(n.decls.size());
  for (Ptr<Decl> &d : n.decls)
    if (auto vd = dynamic_cast<VarDecl *>(d.get())) {
      if (auto et_id = dynamic_cast<TypeExprID *>(vd->type.get())) {
        // do nothing
      } else {
        std::string name = gen_new_anon_name();
        Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(vd->type->clone()), vd->type->loc));
        insert_to_global_top(decl);
        vd->type = Ptr<TypeExprID>(new TypeExprID(name, decl, vd->type->loc));
      }
      _decls.push_back(d);
    } else if (auto td = dynamic_cast<TypeDecl *>(d.get())) {
      insert_to_global_top(Ptr<TypeDecl>(td->clone()));
    } else if (auto cd = dynamic_cast<ConstDecl *>(d.get())) {
      insert_to_global_top(Ptr<ConstDecl>(cd->clone()));
    }
  n.decls = _decls;

}


void TModel::visit_startstate(StartState &n) {
  //TODO: handle the cases of non-TypeExprId TypeExpr values

  std::vector<Ptr<Decl>> _decls(n.decls.size());
  for (Ptr<Decl> &d : n.decls)
    if (auto vd = dynamic_cast<VarDecl *>(d.get())) {
      if (auto et_id = dynamic_cast<TypeExprID *>(vd->type.get())) {
        // do nothing
      } else {
        std::string name = gen_new_anon_name();
        Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(vd->type->clone()), vd->type->loc));
        insert_to_global_top(decl);
        vd->type = Ptr<TypeExprID>(new TypeExprID(name, decl, vd->type->loc));
      }
      _decls.push_back(d/* Ptr<VarDecl>(vd->clone()) */);
    } else if (auto td = dynamic_cast<TypeDecl *>(d.get())) {
      insert_to_global_top(Ptr<TypeDecl>(td->clone()));
    } else if (auto cd = dynamic_cast<ConstDecl *>(d.get())) {
      insert_to_global_top(Ptr<ConstDecl>(cd->clone()));
    }
  n.decls = _decls;
}




// TModel &TModel::operator<<(std::string &s) {
//   out << s;
//   return *this;
// }

// TModel &TModel::operator<<(Node &n) {
//   dispatch(n);
//   return *this;
// }

} // namespace romp
