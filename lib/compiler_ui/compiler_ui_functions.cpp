#include "argparser/Argument.hpp"
#include "compiler_ui_functions.hpp"

#include <fstream>
#include <memory>

#include <argparser/ArgParser.hpp>

#include "lib/parser/ParserFsm.hpp"
#include "lib/parser/ast/BuilderAstFactory.hpp"
#include "lib/parser/ast/visitors/BytecodeVisitor.hpp"
#include "lib/parser/ast/visitors/LintVisitor.hpp"
#include "lib/parser/ast/visitors/TypeChecker.hpp"
#include "lib/parser/diagnostics/DiagnosticCollector.hpp"
#include "lib/parser/pratt/DefaultOperatorResolver.hpp"
#include "lib/parser/pratt/PrattExpressionParser.hpp"
#include "lib/parser/tokens/token_streams/VectorTokenStream.hpp"
#include "lib/parser/type_parser/QNameTypeParser.hpp"
#include "lib/preprocessor/Preprocessor.hpp"

static void PrintDiagnosticMessage(const ovum::compiler::parser::Diagnostic& diag, std::ostream& out) {
  if (diag.IsSuppressed()) {
    return;
  }

  std::string severity_str = diag.GetSeverity() ? diag.GetSeverity()->Name().data() : "UNKNOWN";

  out << "[" << severity_str << "]";

  if (!diag.GetCode().empty()) {
    out << " " << diag.GetCode();
  }

  out << ": " << diag.GetDiagnosticsMessage();
  const std::optional<ovum::compiler::parser::SourceSpan>& where_opt = diag.GetWhere();

  if (where_opt.has_value()) {
    const ovum::compiler::parser::SourceSpan& where = where_opt.value();

    if (!where.GetSourceId().Path().empty()) {
      out << " \nAt " << where.GetSourceId().Path();
    }

    if (where.GetStart() == where.GetEnd()) {
      out << " line " << where.GetStart().GetLine() << " column " << where.GetStart().GetColumn();
    } else {
      out << " from line " << where.GetStart().GetLine() << " column " << where.GetStart().GetColumn() << " to line "
          << where.GetEnd().GetLine() << " column " << where.GetEnd().GetColumn();
    }
  }

  out << "\n";
}

int32_t StartCompilerConsoleUI(const std::vector<std::string>& args, std::ostream& out, std::ostream& err) {
  const CompositeString default_output_name = "Same as input file but with .oil extension";
  auto is_file = [](std::string& arg) { return std::filesystem::exists(arg); };
  auto is_directory = [](std::string& arg) { return std::filesystem::is_directory(arg); };
  std::string description = "Ovum Compiler that compiles Ovum source code to Ovum Intermediate Language.";
  std::vector<CompositeString> include_dirs;
  std::vector<std::string> define_symbols;
  std::vector<bool> no_lint;

  ArgumentParser::ArgParser arg_parser("ovumc", PassArgumentTypes());
  arg_parser.AddCompositeArgument('m', "main-file", "Path to the main file").AddIsGood(is_file).AddValidate(is_file);
  arg_parser.AddCompositeArgument('o', "output-file", "Path to the output file").Default(default_output_name);
  arg_parser.AddCompositeArgument('I', "include-dirs", "Path to directories where include files are located")
      .AddIsGood(is_directory)
      .AddValidate(is_directory)
      .MultiValue(0)
      .StoreValues(include_dirs);
  arg_parser.AddStringArgument('D', "define-symbols", "Defined symbols").MultiValue(0).StoreValues(define_symbols);
  arg_parser.AddFlag('n', "no-lint", "Disable linter").MultiValue(0).StoreValues(no_lint);
  arg_parser.AddHelp('h', "help", description);

  bool parse_result = arg_parser.Parse(args, {.out_stream = err, .print_messages = true});
  if (!parse_result) {
    err << arg_parser.HelpDescription();
    return 1;
  }

  if (arg_parser.Help()) {
    out << arg_parser.HelpDescription();
    return 0;
  }

  std::filesystem::path main_file = arg_parser.GetCompositeValue("main-file").c_str();
  std::filesystem::path output_file;
  const CompositeString& output_file_value = arg_parser.GetCompositeValue("output-file");

  if (output_file_value == default_output_name) {
    output_file = main_file;
    output_file.replace_extension(".oil");
  } else {
    output_file = output_file_value.c_str();
  }

  std::set<std::filesystem::path> include_paths;

  for (const auto& include_dir : include_dirs) {
    include_paths.emplace(include_dir.c_str());
  }

  std::unordered_set<std::string> predefined_symbols;

  for (const auto& define_symbol : define_symbols) {
    predefined_symbols.emplace(define_symbol.c_str());
  }

  include_paths.emplace(main_file.parent_path());

  ovum::compiler::preprocessor::PreprocessingParameters params{
      .include_paths = include_paths, .predefined_symbols = predefined_symbols, .main_file = main_file};

  ovum::compiler::preprocessor::Preprocessor preprocessor(params);

  auto result = preprocessor.Process();

  if (!result) {
    err << result.error().what() << "\n";
    return 2;
  }

  std::vector<ovum::TokenPtr> tokens = std::move(result.value());

  // Set up parser
  auto factory = std::make_shared<ovum::compiler::parser::BuilderAstFactory>();
  auto type_parser = std::make_unique<ovum::compiler::parser::QNameTypeParser>(*factory);
  auto resolver = std::make_unique<ovum::compiler::parser::DefaultOperatorResolver>();
  auto expr_parser =
      std::make_unique<ovum::compiler::parser::PrattExpressionParser>(std::move(resolver), factory, type_parser.get());
  auto parser =
      std::make_unique<ovum::compiler::parser::ParserFsm>(std::move(expr_parser), std::move(type_parser), factory);

  // Parse
  ovum::compiler::parser::DiagnosticCollector diags;
  ovum::compiler::parser::VectorTokenStream stream(tokens);
  auto module = parser->Parse(stream, diags);

  if (!module) {
    err << "Parsing failed\n";
    return 3;
  }

  // Linting
  if (no_lint.empty() || !no_lint[0]) {
    ovum::compiler::parser::LintVisitor lint_visitor(diags);
    module->Accept(lint_visitor);
  }

  // Type checking
  if (no_lint.size() <= 1 || !no_lint[1]) {
    ovum::compiler::parser::TypeChecker type_checker(diags);
    module->Accept(type_checker);
  }

  // Check all diagnostics
  if (diags.Count() > 0) {
    for (const auto& diag : diags.All()) {
      PrintDiagnosticMessage(diag, out);
    }

    if (diags.ErrorCount() > 0) {
      return 4;
    }
  }

  // Generate bytecode
  std::ofstream output_stream(output_file, std::ios::out | std::ios::trunc);
  if (!output_stream.is_open()) {
    err << "Failed to open output file: " << output_file.string() << "\n";
    return 1;
  }

  ovum::compiler::parser::BytecodeVisitor visitor(output_stream);
  module->Accept(visitor);

  output_stream.close();

  return 0;
}
