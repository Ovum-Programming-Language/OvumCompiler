#ifndef STRUCTURALVALIDATOR_HPP_
#define STRUCTURALVALIDATOR_HPP_

#include "WalkVisitor.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"

class StructuralValidator : public WalkVisitor {
public:
  explicit StructuralValidator(IDiagnosticSink& sink) : sink_(sink) {
  }

  void Visit(Module& node) override;
  void Visit(FunctionDecl& node) override;
  void Visit(ClassDecl& node) override;
  void Visit(CallDecl& node) override;
  void Visit(MethodDecl& node) override;
  void Visit(DestructorDecl& node) override;

  void Visit(Call& node) override;
  void Visit(Binary& node) override;
  void Visit(Unary& node) override;
  void Visit(FieldAccess& node) override;
  void Visit(IndexAccess& node) override;
  void Visit(NamespaceRef& node) override;
  void Visit(SafeCall& node) override;

private:
  IDiagnosticSink& sink_;
};

#endif // STRUCTURALVALIDATOR_HPP_
