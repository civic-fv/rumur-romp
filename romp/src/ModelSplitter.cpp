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

#include "ModelSplitter.hpp"

#include "../../common/escape.h"
#include "../../common/isa.h"
#include "options.h"
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
#include <typeinfo>


namespace romp {
using namespace rumur;

// << ========================================================================================== >> 
// <<                               CONSTRUCTORS & DECONSTRUCTORS                                >> 
// << ========================================================================================== >> 

ModelSplitter::~ModelSplitter() {}


// << ========================================================================================== >> 
// <<                               SPLIT MODEL FACTORY FUNCTION                                 >> 
// << ========================================================================================== >> 

// SplitModel ModelSplitter::split_model(const Model &n) {
//   sort_model(n.children);
//   return (SplitModel){Model(global_decls, (global_decls.size() > 0) ? global_decls[0]->loc : n.loc),
//                       Model(state_var_decls, (state_var_decls.size() > 0) ? state_var_decls[0]->loc : n.loc),
//                       Model(funct_decls, (funct_decls.size() > 0) ? funct_decls[0]->loc : n.loc),
//                       Model(rule_decls, (rule_decls.size() > 0) ? rule_decls[0]->loc : n.loc)};
// }

// void ModelSplitter::dispatch(const Node &n) {  // bad code all around
//   loc = location(n.loc);
//   try {
//     auto m = dynamic_cast<const Model&>(n);
//     sort_model(m.children);
//   } catch (std::bad_cast &ex) {
//     throw Error("!! Expected a Model !!", n.loc);
//   }    
// }



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

// const std::string ModelSplitter::gen_new_anon_name() {
//   return "anon"+int_to_hex(anon_id++)+"_t";
// }

const std::string ModelSplitter::gen_new_anon_name() {
  char buff[24];
  sprintf(buff, "anon0x%x_t", anon_id++);
  return std::string(buff);
}

void ModelSplitter::make_name_unique(std::string &name) {
  if (cTypeNames.find(name) == cTypeNames.end())
    return;
  char buff[name.size()+24];
  sprintf(buff, (name+"__0x%x").c_str(), anon_id++);
  name = std::string(buff);
}

void ModelSplitter::insert_to_global_decls(Ptr<TypeDecl> n) {
  make_name_unique(n->name);
  global_decls.push_back(n);
  cTypeNames.insert(n->name);
}

void ModelSplitter::insert_to_global_decls(Ptr<ConstDecl> n) {
  make_name_unique(n->name);
  global_decls.push_back(n);
  cTypeNames.insert(n->name);
}


void ModelSplitter::sort_model(const std::vector<Ptr<Node>> &children) {
  for (const Ptr<Node> &_c : children) {
  // for (const Ptr<Node> &c : children) {
    Ptr<Node> c(_c->clone());

    if (const auto _td = dynamic_cast<const TypeDecl *>(c.get())) {
      Ptr<TypeDecl> td(_td->clone());
      dispatch(*td);
      insert_to_global_decls(td);
    } else if (const auto _cd = dynamic_cast<const ConstDecl *>(c.get())) {
      Ptr<ConstDecl> cd(_cd->clone());
      insert_to_global_decls(cd);
    } else if (const auto _vd = dynamic_cast<const VarDecl *>(c.get())) {
      Ptr<VarDecl> vd(_vd->clone());
      dispatch(*vd);
      state_var_decls.push_back(vd);
    } else if (const auto _f = dynamic_cast<const Function *>(c.get())) {
      Ptr<Function> f(_f->clone());
      funct_decls.push_back(f);
      dispatch(*f);
    } else if (const auto _r = dynamic_cast<const Rule *>(c.get())) {
      Ptr<Rule> r(_r->clone());
      rule_decls.push_back(r);
      dispatch(*r);
    } else 
        // throw Error("Unexpected item in the global scope!!", _c->loc);
        throw Error("Unexpected item in the global scope!!", c->loc);
  }
}



// << ========================================================================================== >> 
// <<                                  PARSING UTIL FUNCTIONS                                    >> 
// << ========================================================================================== >> 


// void ModelSplitter::visit_model(Model &n) {
//   sort_model(n.children);
// }


void ModelSplitter::visit_constdecl(ConstDecl &n) {
  // shouldn't have to do anything here.
}


void ModelSplitter::visit_typedecl(TypeDecl &n) {
  // shouldn't have to do anything here.
}


void ModelSplitter::visit_typeexprid(TypeExprID &n) {
  // shouldn't need to do anything here
}




void ModelSplitter::visit_enum(Enum &n) {
  // shouldn't need to do anything here
}


void ModelSplitter::visit_array(Array &n) {
  //TODO: handle the cases of non-TypeExprId TypeExpr values & the nested enums.

  if (auto et_id = dynamic_cast<TypeExprID *>(n.element_type.get())) {
    // do nothing
  } else {
    dispatch(*(n.element_type));
    std::string name = gen_new_anon_name();
    Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(n.element_type->clone()), n.element_type->loc));
    insert_to_global_decls(decl);
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

  // if (auto e = dynamic_cast<const Enum *>(n.index_type.get())) {
  //   std::string name = gen_new_anon_name();
  //   Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<Enum>(e->clone()), e->loc));
  //   insert_to_global_decls(decl);
  //   // n.index_type = Ptr<TypeExprID>(new TypeExprID(name, decl, e->loc));
  //   n.index_type = Ptr<TypeExpr>(new IndType(*e));
  // } else
   if (auto it_id = dynamic_cast<TypeExprID *>(n.index_type.get())) {
    // n.index_type = Ptr<IndType>(new IndType(*(it_id->referent->value)));
  } else {
    dispatch(*(n.index_type));
    std::string name = gen_new_anon_name();
    Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(n.index_type->clone()), n.index_type->loc));
    insert_to_global_decls(decl);
    n.index_type = Ptr<TypeExprID>(new TypeExprID(name, decl, n.index_type->loc));
    // n.index_type = Ptr<IndType>(new IndType(*(n.index_type)));
  }
  // *this << " }";
}


void ModelSplitter::visit_range(Range &) {
  // shouldn't need to do anything here
}


void ModelSplitter::visit_record(Record &n) {
  //TODO: handle the cases of non-TypeExprId TypeExpr values
  std::vector<Ptr<VarDecl>> _fields(n.fields.size());

  for (Ptr<VarDecl> &f : n.fields) {  // pre output any inplace type definitions
    
    if (auto et_id = dynamic_cast<TypeExprID *>(f->type.get())) {
      _fields.push_back(Ptr<VarDecl>(f));
    } else {
      dispatch(*(f->type));
      std::string name = gen_new_anon_name();
      Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(f->type), f->loc));
      insert_to_global_decls(decl);
      _fields.push_back(Ptr<VarDecl>(new VarDecl(f->name, 
                                          Ptr<TypeExprID>(new TypeExprID(name, decl, f->loc)),
                                           f->loc)));
    }
  }
  n.fields = _fields;
}


void ModelSplitter::visit_scalarset(Scalarset &n) { 
  // shouldn't need to do anything here
}


void ModelSplitter::visit_vardecl(VarDecl &n) {
  // DEBUG: check that no VarDecl is reached that has a non-TypeExprId type! 
  if (auto et_id = dynamic_cast<const TypeExprID *>(n.type.get())) {
    // do nothing
  } else {
    dispatch(*(n.type));
    std::string name = gen_new_anon_name();
    Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(n.type), n.type->loc));
    insert_to_global_decls(decl);
    n.type = Ptr<TypeExprID>(new TypeExprID(name, decl, n.type->loc));
    // throw Error("An inline type declaration was reached before being processed by the type handler!!"
    //                 " :: `"+n.type->to_string()+"`",
    //             n.loc);
  }
}


void ModelSplitter::visit_function(Function &n) {
  //TODO: handle the cases of non-TypeExprID TypeExpr values

  if (auto et_id = dynamic_cast<TypeExprID *>(n.return_type.get())) {
    // do nothing
  } else {
    
    std::string name = gen_new_anon_name();
    Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(n.return_type->clone()), n.return_type->loc));
    insert_to_global_decls(decl);
    n.return_type = Ptr<TypeExprID>(new TypeExprID(name, decl, n.return_type->loc));
  }

  for (Ptr<VarDecl> &p : n.parameters)
    if (auto et_id = dynamic_cast<TypeExprID *>(p->type.get())) {
      // do nothing
    } else {
      dispatch(*(p->type));
      std::string name = gen_new_anon_name();
      Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(p->type->clone()), p->type->loc));
      insert_to_global_decls(decl);
      p->type = Ptr<TypeExprID>(new TypeExprID(name, decl, p->type->loc));
    }

  std::vector<Ptr<Decl>> _decls(n.decls.size());
  for (Ptr<Decl> &d : n.decls)
    if (auto vd = dynamic_cast<VarDecl *>(d.get())) {
      if (auto et_id = dynamic_cast<TypeExprID *>(vd->type.get())) {
        // do nothing
      } else {
        dispatch(*(vd->type));
        std::string name = gen_new_anon_name();
        Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(vd->type->clone()), vd->type->loc));
        insert_to_global_decls(decl);
        vd->type = Ptr<TypeExprID>(new TypeExprID(name, decl, vd->type->loc));
      }
      _decls.push_back(d);
    } else if (auto td = dynamic_cast<TypeDecl *>(d.get())) {
      insert_to_global_decls(Ptr<TypeDecl>(td->clone()));
    } else if (auto cd = dynamic_cast<ConstDecl *>(d.get())) {
      insert_to_global_decls(Ptr<ConstDecl>(cd->clone()));
    }
  n.decls = _decls;
}

void ModelSplitter::visit_quantifier(Quantifier& q) {
  if (q.type != nullptr) {
    if (q.type->is_simple()) {
      if (q.from != nullptr)
        q.from = Ptr<Number>::make(q.type->lower_bound(),q.type->loc);
      if (q.to != nullptr)
        q.to = Ptr<Number>::make(q.type->upper_bound(), q.type->loc);
      if (q.step != nullptr) {
        // if (auto sc = dynamic_cast<Scalarset*>(q.type->resolve().get())) {
        //   mpz_class temp = (((q.from->constant_fold()) - (q.to->constant_fold())) / (sc->count()));
        //   q.step = Ptr<Number>::make(
        //             temp.get_str(),
        //             q.type->loc);
        // } else 
          q.step = Ptr<Number>::make("1",q.type->loc);
      }
    }
    if (auto _td = dynamic_cast<TypeExprID *>(q.type.get())) {
      // do nothing
    } else {
      dispatch(*(q.type));
      std::string name = gen_new_anon_name();
      Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(q.type), q.type->loc));
      insert_to_global_decls(decl);
      q.type = Ptr<TypeExprID>(new TypeExprID(name, decl, q.type->loc));
    }
    
  } else if (q.from != nullptr && q.to != nullptr) {
    auto range = Ptr<Range>::make(q.from, q.to, q.loc);
    std::string name = gen_new_anon_name();
    Ptr<TypeDecl> decl(new TypeDecl(name, range, q.loc));
    insert_to_global_decls(decl);
    q.type = Ptr<TypeExprID>(new TypeExprID(name, decl, q.loc));
  } else 
    throw Error("the Quantifier has broken bounds!!", q.loc);
}


void ModelSplitter::visit_propertyrule(PropertyRule &n) {
  for (Quantifier &q : n.quantifiers) 
    dispatch(q);
}


void ModelSplitter::visit_simplerule(SimpleRule &n) {
  //TODO: handle the cases of non-TypeExprId TypeExpr values
  for (Quantifier &q : n.quantifiers) 
    dispatch(q);
    // if (q.type != nullptr) 
    //   if (auto _td = dynamic_cast<TypeExprID *>(q.type.get())) {
    //     // do nothing
    //   } else {
    //     dispatch(*(q.type));
    //     std::string name = gen_new_anon_name();
    //     Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(q.type), q.type->loc));
    //     insert_to_global_decls(decl);
    //     q.type = Ptr<TypeExprID>(new TypeExprID(name, decl, q.type->loc));
    // }

  std::vector<Ptr<Decl>> _decls(n.decls.size());
  for (Ptr<Decl> &d : n.decls)
    if (auto vd = dynamic_cast<VarDecl *>(d.get())) {
      if (auto _ = dynamic_cast<TypeExprID *>(vd->type.get())) {
        // do nothing
      } else {
         dispatch(*(vd->type));
        std::string name = gen_new_anon_name();
        Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(vd->type), vd->type->loc));
        insert_to_global_decls(decl);
        vd->type = Ptr<TypeExprID>(new TypeExprID(name, decl, vd->type->loc));
      }
      _decls.push_back(d);
    } else if (auto td = dynamic_cast<TypeDecl *>(d.get())) {
      insert_to_global_decls(Ptr<TypeDecl>(td));
    } else if (auto cd = dynamic_cast<ConstDecl *>(d.get())) {
      insert_to_global_decls(Ptr<ConstDecl>(cd));
    }
  n.decls = _decls;

}

void ModelSplitter::visit_ruleset(rumur::Ruleset &n) {
  for (Quantifier &q : n.quantifiers) 
    if (q.type != nullptr)
      dispatch(q); 
      // if (auto _td = dynamic_cast<TypeExprID *>(q.type.get())) {
      //   // do nothing
      // } else {
      //   dispatch(*(q.type));
      //   std::string name = gen_new_anon_name();
      //   Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(q.type), q.type->loc));
      //   insert_to_global_decls(decl);
      //   q.type = Ptr<TypeExprID>(new TypeExprID(name, decl, q.type->loc));
      // }
  for (auto _r : n.rules)
    dispatch(*_r);
}


void ModelSplitter::visit_aliasrule(rumur::AliasRule &n) {
  for (auto _r : n.rules)
    dispatch(*_r);
}


void ModelSplitter::visit_startstate(StartState &n) {
  //TODO: handle the cases of non-TypeExprId TypeExpr values
  for (Quantifier &q : n.quantifiers) 
  if (q.type != nullptr) 
    if (auto _td = dynamic_cast<TypeExprID *>(q.type.get())) {
      // do nothing
    } else {
      dispatch(*(q.type));
      std::string name = gen_new_anon_name();
      Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(q.type), q.type->loc));
      insert_to_global_decls(decl);
      q.type = Ptr<TypeExprID>(new TypeExprID(name, decl, q.type->loc));
  }
  std::vector<Ptr<Decl>> _decls(n.decls.size());
  for (Ptr<Decl> &d : n.decls)
    if (auto vd = dynamic_cast<VarDecl *>(d.get())) {
      if (auto et_id = dynamic_cast<TypeExprID *>(vd->type.get())) {
        // do nothing
      } else {
         dispatch(*(vd->type));
        std::string name = gen_new_anon_name();
        Ptr<TypeDecl> decl(new TypeDecl(name, Ptr<TypeExpr>(vd->type->clone()), vd->type->loc));
        insert_to_global_decls(decl);
        vd->type = Ptr<TypeExprID>(new TypeExprID(name, decl, vd->type->loc));
      }
      _decls.push_back(d/* Ptr<VarDecl>(vd->clone()) */);
    } else if (auto td = dynamic_cast<TypeDecl *>(d.get())) {
      insert_to_global_decls(Ptr<TypeDecl>(td->clone()));
    } else if (auto cd = dynamic_cast<ConstDecl *>(d.get())) {
      insert_to_global_decls(Ptr<ConstDecl>(cd->clone()));
    }
  n.decls = _decls;
}



void ModelSplitter::__throw_unreachable_error(const Node &n) {
  throw Error("The Model Splitter & Organizer visited an unsupported syntactic element!!", n.loc);
}


// ModelSplitter &ModelSplitter::operator<<(std::string &s) {
//   out << s;
//   return *this;
// }

// ModelSplitter &ModelSplitter::operator<<(Node &n) {
//   dispatch(n);
//   return *this;
// }

} // namespace romp
