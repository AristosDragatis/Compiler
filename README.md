# Interval Arithmetic Compiler

This project implements a basic **compiler for interval arithmetic expressions**, built using **Flex** for lexical analysis and **C++** for parsing, semantic analysis, and AST construction.

It parses arithmetic expressions , performs semantic checks, and generates an Abstract Syntax Tree (AST) that represents the structure of the expression. The grammar supports standard arithmetic operations (`+`, `-`, `*`, `/`) with proper handling of precedence, associativity, and parentheses.

## Features

- Lexical analysis using Flex
- LL(1) Parsing logic in C++
- Abstract Syntax Tree (AST) construction
- Symbol table management
- Handles negative numbers and parentheses

## File Structure

- `lexer.l` – Flex lexer specification
- `parser.cpp` – Main parser, AST builder, and symbol table logic
- `input_.txt` – Input arithmetic expression (user input)
- `tokens.txt` – Tokenized version of the input (output from Flex)

## Requirements

- **Flex** (Fast Lexical Analyzer)
- **g++** (GNU C++ Compiler)

## How to Compile and Run

1. **Generate the lexer using Flex:**

```terminal
flex lexer.l
```
This creates a lex.yy.c file from your lexer.l.

**2. Compile the lexer and parser together:**

```terminal
g++ lex.yy.c parser.cpp -o interval_compiler
```

**3. Prepare input:**
Write your arithmetic expression in input_.txt (e.g., x1 + x2; ).

Run the lexer to tokenize the input and save tokens to tokens.txt.


**4. Run the compiler:**
```terminal
./interval_compiler
```
