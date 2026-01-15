# Ovum Compiler (ovumc)

The official compiler implementation for the [Ovum Programming Language](https://github.com/Ovum-Programming-Language/OvumLanguage), written in C++. This compiler transforms Ovum source code (`.ovum` files) into executable bytecode for the Ovum Virtual Machine.

## About Ovum

Ovum is a strongly statically typed, single-threaded programming language focused on safety, clarity, and performance. It features:

- **Memory safety** via garbage collection
- **Immutability by default** with explicit mutation using `var`
- **Pure functions** (`pure`) that are side-effect free and cacheable
- **Interface-based polymorphism** (no class inheritance)
- **Kotlin-like syntax** with minimal, predictable constructs
- **JIT compilation** with garbage collection for performance

## Prerequisites

- **CMake** 3.25 or higher
- **C++23** compatible compiler (GCC, Clang, or MSVC)
- **Git** (for dependency management)

## Quick Start

### Automated Installation

The easiest way to build and install the compiler:

```bash
./install.sh
```

This script will:
1. Configure CMake with Release build settings
2. Build the `ovumc` executable
3. Create a symbolic link in your home directory
4. Optionally install to `/usr/bin` (Linux) or provide Windows instructions

### Manual Build

1. **Configure the build:**
   ```bash
   cmake -S . -B build
   ```

2. **Build the compiler:**
   ```bash
   cmake --build build --target ovumc
   ```

3. **Build and run tests:**
   ```bash
   cmake --build build --target ovumc_tests
   ctest --test-dir build
   ```

## Usage

Once built, the `ovumc` compiler can be used to compile Ovum source files:

```bash
# Compile an Ovum source file
ovumc -m input.ovum -o output.oil -I ./ -D SOME_FLAG

# Show help
ovumc -h
```

## Development

### Building for Development

For development builds with debug information:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Running Tests

The project includes comprehensive tests using Google Test:

```bash
# Run all tests
ctest --test-dir build

# Run specific test executable
./build/ovumc_tests.exe  # Windows
./build/tests/ovumc_tests  # Linux/macOS
```

### Code Quality

The project includes:
- **Clang-format** configuration (`.clang-format`)
- **Clang-tidy** configuration (`.clang-tidy`)
- **CI/CD pipeline** with automated testing

## Contributing

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for details on:

- Code style and formatting
- Testing requirements
- Pull request process
- Issue reporting

## Related Projects

- **[OvumLanguage](https://github.com/Ovum-Programming-Language/OvumLanguage)** - Main language repository with documentation and specifications
- **[OvumVM](https://github.com/Ovum-Programming-Language/OvumVM)** - Virtual Machine implementation
- **[OvumExamples](https://github.com/Ovum-Programming-Language/OvumExamples)** - Examples of code in Ovum with bytecode

## License

This project is licensed under the GPL-3.0 License - see the [LICENSE](LICENSE) file for details.


## Support

- **Issues**: Report bugs and request features on [GitHub Issues](https://github.com/Ovum-Programming-Language/OvumCompiler/issues)
- **Documentation**: Visit the [Ovum Language Documentation](https://github.com/Ovum-Programming-Language/OvumDocs) for language documentation or view it [online](https://ovum-programming-language.github.io/OvumDocs/).

