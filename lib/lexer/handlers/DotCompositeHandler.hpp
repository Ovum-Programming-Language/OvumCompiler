#ifndef DOTCOMPOSITEHANDLER_HPP_
#define DOTCOMPOSITEHANDLER_HPP_

#include "Handler.hpp"
#include "NumberHandler.hpp"
#include "OperatorHandler.hpp"
#include "PunctHandler.hpp"

class DotCompositeHandler : public Handler {
public:
  DotCompositeHandler();

  OptToken Scan(SourceCodeWrapper& wrapper) override;

private:
  std::unique_ptr<NumberHandler> num_;
  std::unique_ptr<OperatorHandler> op_;
  std::unique_ptr<PunctHandler> punct_;
};

#endif // DOTCOMPOSITEHANDLER_HPP_
