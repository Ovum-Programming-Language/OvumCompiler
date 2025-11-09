#ifndef PREPROCESSOR_PREPROCESSORERROR_HPP_
#define PREPROCESSOR_PREPROCESSORERROR_HPP_

#include <stdexcept>
#include <string>

namespace ovum::compiler::preprocessor {

class PreprocessorError : public std::runtime_error {
public:
  explicit PreprocessorError(const std::string& msg) : std::runtime_error(msg) {
  }
};

class FileNotFoundError : public PreprocessorError {
public:
  FileNotFoundError(const std::string& file) : PreprocessorError("File not found: " + file) {
  }
};

class FileReadError : public PreprocessorError {
public:
  FileReadError(const std::string& file, const std::string& reason) :
      PreprocessorError("Failed to read " + file + ": " + reason) {
  }
};

class CycleDetectedError : public PreprocessorError {
public:
  CycleDetectedError(const std::string& cycle) : PreprocessorError("Cycle detected in dependencies: " + cycle) {
  }
};

class InvalidImportError : public PreprocessorError {
public:
  InvalidImportError(const std::string& msg) : PreprocessorError("Invalid import: " + msg) {
  }
};

class InvalidDirectiveError : public PreprocessorError {
public:
  explicit InvalidDirectiveError(const std::string& msg) : PreprocessorError("Invalid directive: " + msg) {
  }
};

class UnmatchedDirectiveError : public PreprocessorError {
public:
  explicit UnmatchedDirectiveError(const std::string& msg) : PreprocessorError("Unmatched directive: " + msg) {
  }
};

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSOR_PREPROCESSORERROR_HPP_
