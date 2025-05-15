# SQL-Database in C++

### Author: Samuel Mekonnen

## 🗂 Project Overview

This is a lightweight SQL-like database engine implemented in C++. It features a command-line interface, custom tokenizer, parser, and binary file-backed table storage. It supports simple SQL commands including `SELECT`, `INSERT`, and conditional queries using a custom-built relational engine.

---

## ✅ Features

- [x] Interactive SQL command line interface
- [x] Batch command execution via `commands.txt`
- [x] Binary file-based table storage (e.g., `student.bin`, `employee.bin`)
- [x] Tokenizer and parser with support for basic SQL syntax
- [x] Shunting Yard and Reverse Polish Notation (RPN) evaluation
- [x] Error handling with specific messages
- [x] Modular design using STL and custom components
- [x] Unit tests using GoogleTest

---

## 📁 File Structure

```bash
.
├── CMakeLists.txt           # Build configuration
├── main.cpp                 # Entry point: handles CLI and batch execution
├── commands.txt             # Batch SQL commands
├── tables.txt               # Table index/config
├── includes/                # All logic: tokenizer, parser, sql engine
├── _tests/                  # Unit test files using GoogleTest
├── *.bin                    # Binary table storage files
└── *.txt                    # Table field descriptions and selected outputs
