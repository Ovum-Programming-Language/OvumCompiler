#include "OpTags.hpp"

#include <string_view>

namespace ovum::compiler::parser {

namespace {

struct BinaryAdd : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "add";
  }
};

struct BinarySub : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "sub";
  }
};

struct BinaryMul : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "mul";
  }
};

struct BinaryDiv : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "div";
  }
};

struct BinaryMod : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "mod";
  }
};

struct BinaryLt : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "lt";
  }
};

struct BinaryLe : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "le";
  }
};

struct BinaryGt : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "gt";
  }
};

struct BinaryGe : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "ge";
  }
};

struct BinaryEq : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "eq";
  }
};

struct BinaryNe : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "ne";
  }
};

struct BinaryAnd : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "and";
  }
};

struct BinaryOr : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "or";
  }
};

struct BinaryXor : IBinaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "xor";
  }
};

struct UnaryNeg : IUnaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "neg";
  }
};

struct UnaryPlus : IUnaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "plus";
  }
};

struct UnaryNot : IUnaryOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "not";
  }
};

struct AssignRef : IAssignOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return "=";
  }
};

struct AssignCopy : IAssignOpTag {
  [[nodiscard]] std::string_view Name() const override {
    return ":=";
  }
};

} // namespace

namespace OpTags {

const IBinaryOpTag& Add() {
  static BinaryAdd t;
  return t;
}

const IBinaryOpTag& Sub() {
  static BinarySub t;
  return t;
}

const IBinaryOpTag& Mul() {
  static BinaryMul t;
  return t;
}

const IBinaryOpTag& Div() {
  static BinaryDiv t;
  return t;
}

const IBinaryOpTag& Mod() {
  static BinaryMod t;
  return t;
}

const IBinaryOpTag& Lt() {
  static BinaryLt t;
  return t;
}

const IBinaryOpTag& Le() {
  static BinaryLe t;
  return t;
}

const IBinaryOpTag& Gt() {
  static BinaryGt t;
  return t;
}

const IBinaryOpTag& Ge() {
  static BinaryGe t;
  return t;
}

const IBinaryOpTag& Eq() {
  static BinaryEq t;
  return t;
}

const IBinaryOpTag& Ne() {
  static BinaryNe t;
  return t;
}

const IBinaryOpTag& And() {
  static BinaryAnd t;
  return t;
}

const IBinaryOpTag& Or() {
  static BinaryOr t;
  return t;
}

const IBinaryOpTag& Xor() {
  static BinaryXor t;
  return t;
}

const IUnaryOpTag& Neg() {
  static UnaryNeg t;
  return t;
}

const IUnaryOpTag& Plus() {
  static UnaryPlus t;
  return t;
}

const IUnaryOpTag& Not() {
  static UnaryNot t;
  return t;
}

const IAssignOpTag& RefAssign() {
  static AssignRef t;
  return t;
}

const IAssignOpTag& CopyAssign() {
  static AssignCopy t;
  return t;
}

} // namespace OpTags

} // namespace ovum::compiler::parser
