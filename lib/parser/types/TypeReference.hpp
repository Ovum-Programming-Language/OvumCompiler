#ifndef TYPEREFERENCE_HPP_
#define TYPEREFERENCE_HPP_

#include <string>
#include <vector>

class TypeReference {
public:
  TypeReference();

  TypeReference(std::string&& name);

  TypeReference(const TypeReference& ref);

  ~TypeReference();

  TypeReference& operator=(const TypeReference& ref);

  TypeReference& operator=(std::string&& name);

  [[nodiscard]] std::string GetName() const;

  [[nodiscard]] std::vector<std::string> GetQualifiedName() const;

private:
  std::vector<std::string> qname;
  bool is_nullable = false;
};

#endif // TYPEREFERENCE_HPP_
