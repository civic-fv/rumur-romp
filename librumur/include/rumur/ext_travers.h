#pragma once

#include <rumur/traverse.h>

#ifndef RUMUR_API_WITH_RTTI
#define RUMUR_API_WITH_RTTI __attribute__((visibility("default")))
#endif

namespace rumur {

class RUMUR_API_WITH_RTTI BaseExtTraversal {

public:
  virtual void visit_scalarsetunion(ScalarsetUnion& n) = 0;
   
};

class RUMUR_API_WITH_RTTI ExtTraversal : public BaseTraversal, public BaseExtTraversal {
public:
  void visit_scalarsetunion(ScalarsetUnion& n) override;

};

class RUMUR_API_WITH_RTTI ConstBaseExtTraversal {
public:
  virtual void visit_scalarsetunion(const ScalarsetUnion& n) = 0;

};

class RUMUR_API_WITH_RTTI ConstExtTraversal : public ConstBaseTraversal, public ConstBaseExtTraversal {
public:
  void visit_scalarsetunion(ScalarsetUnion& n) override;

};

class RUMUR_API_WITH_RTTI ConstExtExprTraversal : public ConstExprTraversal, public ConstBaseExtTraversal {
public:
  void visit_scalarsetunion(ScalarsetUnion& n) override;

};

class RUMUR_API_WITH_RTTI ConstExtStmtTraversal : public ConstStmtTraversal, public ConstBaseExtTraversal {
public:
  void visit_scalarsetunion(ScalarsetUnion& n) override;

};

class RUMUR_API_WITH_RTTI ConstExtTypeTraversal : public ConstTypeTraversal, public ConstBaseExtTraversal {
public:
  

};

} // namespace rumur
