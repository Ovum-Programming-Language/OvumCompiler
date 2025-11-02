#ifndef OPTAGS_HPP_
#define OPTAGS_HPP_

#include "IAssignOpTag.hpp"
#include "IBinaryOpTag.hpp"
#include "IUnaryOpTag.hpp"

namespace OpTags {
// binary
const IBinaryOpTag& Add();
const IBinaryOpTag& Sub();
const IBinaryOpTag& Mul();
const IBinaryOpTag& Div();
const IBinaryOpTag& Mod();
const IBinaryOpTag& Lt();
const IBinaryOpTag& Le();
const IBinaryOpTag& Gt();
const IBinaryOpTag& Ge();
const IBinaryOpTag& Eq();
const IBinaryOpTag& Ne();
const IBinaryOpTag& And();
const IBinaryOpTag& Or();
const IBinaryOpTag& Xor();

// unary
const IUnaryOpTag& Neg();
const IUnaryOpTag& Plus();
const IUnaryOpTag& Not();

// assign
const IAssignOpTag& RefAssign();  // =
const IAssignOpTag& CopyAssign(); // :=
} // namespace OpTags

#endif // OPTAGS_HPP_
