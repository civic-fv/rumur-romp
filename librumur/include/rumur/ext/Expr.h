#pragma once

#include <rumur/Expr.h>
#include <rumur/ext/Function.h>

#ifndef RUMUR_API_WITH_RTTI
#define RUMUR_API_WITH_RTTI __attribute__((visibility("default")))
#endif

namespace rumur {
namespace ext {

  struct RUMUR_API_WITH_RTTI BuiltInFunctionCall : public FunctionCall {

  };

  struct RUMUR_API_WITH_RTTI IsMember : public Or {

    Ptr<Expr> designator;
    Ptr<TypeExpr> type;

    IsMember(const Ptr<Expr>& designator_, const Ptr<TypeExpr>& type_, const location& loc_);
    IsMember* clone() const override;
    ~IsMember() = default;

    void visit(BaseTraversal& visitor) override;
    void visit(ConstBaseTraversal& visitor) const override;

    void validate() const override;
    void update() override;

    std::string to_string() const override;
  private:
    void validate_types() const;
  };


  /**
   * @brief A built in AST element, that should never be inserted by the parser,
   *        but should be inserted by the Assignment operator when assigning into
   *        a scalarset union from a scalarset or vise versa.
   *        It is mostly used as a wrapper for an existing AST node to wrap a child in.
   *        WARNING: This is a internal AST tool only & is pretty HACKY, 
   *                 therefore it is not included directly in any traversal tool,
   *                  and will just register as an add operation!
   */
  struct RUMUR_API_WITH_RTTI SUCast : public Add {

    Ptr<TypeExpr> target; // expected to have it's symbols resolved already (HACKY)

    /**
     * @brief \b Constructor: \n Construct a new \c SUCast object.
     *        \b NOTE: all parameters are expected have their symbols resolved before construction (HACKY)
     * @param target_ The SCALARSET UNION type you wish to get to 
     * @param cur_ The expression that will have its value cast to target
     * @param loc_ The location of the parent AST that creates this as a wrapper for an expression.
     */
    SUCast(const Ptr<TypeExpr>& target_, const Ptr<Expr>& cur_, const location& loc_);
    virtual ~SUCast() = default;
    SUCast *clone() const override;
    
    void visit(BaseTraversal& visitor) override;
    void visit(ConstBaseTraversal& visitor) const override;

    Ptr<TypeExpr> type() const override;
    void validate() const override;
    void update() override;  // update is also called from constructor

    std::string to_string() const override;
  
  private:
    void pre_validate() const;
  };


} //namespace ext
} //namespace rumur
