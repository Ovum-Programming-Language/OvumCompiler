#ifndef PARSER_OPTAGS_HPP_
#define PARSER_OPTAGS_HPP_

#include "IAssignOpTag.hpp"
#include "IBinaryOpTag.hpp"
#include "IUnaryOpTag.hpp"

namespace ovum::compiler::parser {

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
const IBinaryOpTag& BitwiseAnd();  // &
const IBinaryOpTag& BitwiseOr();   // |
const IBinaryOpTag& LeftShift();   // <<
const IBinaryOpTag& RightShift();  // >>

// unary
const IUnaryOpTag& Neg();
const IUnaryOpTag& Plus();
const IUnaryOpTag& Not();
const IUnaryOpTag& BitwiseNot();   // ~
const IUnaryOpTag& Unwrap();       // ! (postfix, for nullable unwrap)

// assign
const IAssignOpTag& RefAssign();  // =
const IAssignOpTag& CopyAssign(); // :=
} // namespace OpTags

} // namespace ovum::compiler::parser

#endif // PARSER_OPTAGS_HPP_
