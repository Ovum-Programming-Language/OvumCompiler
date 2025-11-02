#include "PreprocessorUnitTestSuite.hpp"

PreprocessorUnitTestSuite::TestResult PreprocessorUnitTestSuite::RunSingleTest(const std::filesystem::path& test_file_path) {
  TestResult result;
  result.test_name = test_file_path.filename().string();
  auto [wait_error, include_num] = ParseTestDirectives(test_file_path);
  result.expected_include_num = include_num;
  PreprocessingParameters params;
  params.main_file = test_file_path;
  params.include_paths = { std::filesystem::path(TEST_DATA_DIR) / "preprocessor" / "for_import" };
  Preprocessor preprocessor(params);
    
  auto process_result = preprocessor.Process();
    
  if (wait_error) {
    if (!process_result.has_value()) {
      result.passed = true;
      result.error_message = "Expected error occurred: " + 
          GetErrorString(process_result.error());
    } else {
      result.passed = false;
      result.error_message = "Expected error but preprocessing succeeded";
    }
  } else {
    if (!process_result.has_value()) {
      result.passed = false;
      result.error_message = "Unexpected error: " + 
          GetErrorString(process_result.error());
    } else {
      result.included_numbers = ExtractIncludedNumbers(process_result.value());
      result.passed = ValidateIncludedNumbers(result.included_numbers, include_num);
          
        if (!result.passed) {
          result.error_message = "INCLUDE validation failed. Expected max: " + 
              std::to_string(include_num) + ", Got: [";
          for (size_t i = 0; i < result.included_numbers.size(); ++i) {
            if (i > 0) result.error_message += ", ";
            result.error_message += std::to_string(result.included_numbers[i]);
          }
          result.error_message += "]";
        }
      }
    }
    
    return result;
}
std::pair<bool, int> PreprocessorUnitTestSuite::ParseTestDirectives(const std::filesystem::path& file_path) {
  std::ifstream file(file_path);
  std::string line;
  bool wait_error = false;
  int include_num = 0;
  
  while (std::getline(file, line)) {
    size_t start = line.find_first_not_of(" \t");
    if (start == std::string::npos) continue;
    
    std::string directive = line.substr(start);
    
    if (directive.rfind("EXPECTERROR", 0) == 0) {
      wait_error = true;
      continue;
    }
    if (directive.rfind("INCLUDENUM", 0) == 0) {
      size_t num_start = directive.find_first_of("0123456789");
      if (num_start != std::string::npos) {
          include_num = std::stoi(directive.substr(num_start));
      }
      break;
    }
    
    if (directive[0] != '#' && !directive.empty()) {
      break;
    }
  }
  
  return {wait_error, include_num};
}

std::vector<int> PreprocessorUnitTestSuite::ExtractIncludedNumbers(const std::vector<TokenPtr>& tokens) {
    std::vector<int> included_numbers;
    
    for (size_t i = 0; i < tokens.size(); ++i) {
      if (IsIncludeToken(tokens[i])) {
        if (i + 1 < tokens.size() && IsNumberToken(tokens[i + 1])) {
          int number = ExtractNumberFromToken(tokens[i + 1]);
          included_numbers.push_back(number);
          i++;
        }
      }
    }
    
    return included_numbers;
}

bool PreprocessorUnitTestSuite::ValidateIncludedNumbers(const std::vector<int>& included_numbers, int max_include) {
  for (int num : included_numbers) {
    if (num > max_include) {
      return false;
    }
  }
  return true;
}

bool PreprocessorUnitTestSuite::IsIncludeToken(const TokenPtr& token) {
  return token->GetLexeme() == "INCLUDE";
}

bool PreprocessorUnitTestSuite::IsNumberToken(const TokenPtr& token) {
  return token->GetStringType() == "LITERAL:Int";
}

int PreprocessorUnitTestSuite::ExtractNumberFromToken(const TokenPtr& token) {
  return std::stoi(token->GetLexeme());
}

std::string PreprocessorUnitTestSuite::GetErrorString(const PreprocessorError& error) {
  return error.what();
}