#ifndef OPTAGS_HPP_
#define OPTAGS_HPP_

#include "IAssignOpTag.hpp"
#include "IBinaryOpTag.hpp"
#include "IUnaryOpTag.hpp"

namespace OpTags {
// binary
static const IBinaryOpTag& Add();
static const IBinaryOpTag& Sub();
static const IBinaryOpTag& Mul();
static const IBinaryOpTag& Div();
static const IBinaryOpTag& Mod();
static const IBinaryOpTag& Lt();
static const IBinaryOpTag& Le();
static const IBinaryOpTag& Gt();
static const IBinaryOpTag& Ge();
static const IBinaryOpTag& Eq();
static const IBinaryOpTag& Ne();
static const IBinaryOpTag& And();
static const IBinaryOpTag& Or();
static const IBinaryOpTag& Xor();

// unary
static const IUnaryOpTag& Neg();
static const IUnaryOpTag& Plus();
static const IUnaryOpTag& Not();

// assign
static const IAssignOpTag& RefAssign();  // =
static const IAssignOpTag& CopyAssign(); // :=
} // namespace OpTags

#endif // OPTAGS_HPP_
