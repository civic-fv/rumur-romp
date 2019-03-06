#pragma once

#include <cstddef>
#include <iostream>
#include "location.hh"
#include <memory>
#include <rumur/Expr.h>
#include <rumur/Node.h>
#include <rumur/Ptr.h>

namespace rumur {

struct Property : public Node {

  enum Category {
    DISABLED,
    ASSERTION,
    ASSUMPTION,
    COVER,
  };

  Category category;
  Ptr<Expr> expr;

  Property(Category category_, const Ptr<Expr> &expr_,
    const location &loc_);
  Property *clone() const final;
  virtual ~Property() = default;

  bool operator==(const Node &other) const final;
};

}
