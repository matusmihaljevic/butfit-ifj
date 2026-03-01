# IFJ24 Compiler

**Authors:** Adam Bojnanský, Jaroslav Podmajerský, Kristián Pribila, Matúš Mihaljevič

## Description

This project implements a compiler for the IFJ24 (subset of the Zig programming language) imperative language. The compiler reads source code from standard input, performs lexical and syntax analysis, builds an abstract syntax tree (AST), runs semantic checks, and generates IFJcode24 output to standard output.

The implementation is written in C and is organized into separate modules for scanning, parsing, precedence analysis, semantic analysis, symbol table handling, and code generation.

## Features

- Full compiler pipeline: scanner -> parser/AST -> semantic analysis -> code generation.
- Lexical analysis with a finite-state scanner, including keywords, identifiers, literals, operators, and comments.
- Parsing of IFJ24 language constructs (declarations, functions, assignments, conditions, loops, return statements).
- Precedence-based expression analysis for binary operations and relations.
- Semantic checks for undefined/redefined symbols, type compatibility, function calls, return values, and scope rules.
- Generation of target IFJcode24 instructions (including prologue, function bodies, expressions, control flow).
- Support for IFJ24 built-in functions such as `readi32`, `readf64`, `readstr`, `write`, `i2f`, `f2i`, `length`, `concat`, `substring`, `strcmp`, `ord`, and `chr`.
- Defined compiler error codes for lexical, syntax, semantic, and internal errors.

## Extensions

- **Integrated semantic and codegen support for richer control flow**: The compiler supports `if/else`, `while`, and `for`-style iteration handling in both semantic analysis and IFJcode24 generation, including checks for `break`/`continue` placement and function return correctness.

## Build

```bash
make
```

This builds the compiler binary:

```bash
./ifj2024
```

## Usage

Compile an IFJ24 source file from stdin and print generated IFJcode24 to stdout:

```bash
./ifj2024 < input.ifj24 > output.code24
```

Run generated code in the IFJcode24 interpreter:

```bash
./tests/ic24int output.code24
```

## Testing

- Lexer unit tests are available via CMake + GoogleTest in `lexer_test.cpp`.
- Integration-like test inputs are available in `tests/` (`test*.ifj24`, `input*.txt`) with helper script `tests/test.sh`.

## Project Structure

- `scanner.c/.h` - lexical analyzer
- `parser.c/.h`, `precedence_analysis.c/.h` - syntax and expression parsing
- `ast.c/.h` - abstract syntax tree structures
- `semantic.c/.h` - semantic analysis and checks
- `symtable.c/.h`, `stack.c/.h`, `LL_buin_fn.c/.h` - semantic support data structures
- `codegen.c/.h`, `codegen_definitions.h` - IFJcode24 generation
- `dstring.c/.h` - dynamic string utilities
- `error.c/.h` - error reporting and compiler exit codes
