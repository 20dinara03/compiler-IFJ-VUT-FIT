# IFJ22 Compiler  
🚀 Implementation of a compiler for the IFJ22 imperative language, translating it into IFJcode22 intermediate code.  

## 📌 Project Description  
This project is part of the **IFJ and IAL** courses at FIT VUT. The goal is to develop a compiler for the **IFJ22 language**, generating **IFJcode22**, which is then interpreted by the provided `ic22int` interpreter.  

### 🔹 **Features of the Compiler:**  
- ✅ **Lexical analysis** using finite state automata  
- ✅ **Syntactic analysis** via **recursive descent** or **LL-table** parsing  
- ✅ **Precedence parsing** for expressions  
- ✅ **Semantic analysis** (type checking, function redefinition, parameter validation)  
- ✅ **Generation of IFJcode22 intermediate code**  
- ✅ **Symbol table implementation** (**BVS** or **TRP**)  
- ✅ **Testing with the official `ic22int` interpreter**  

## 📂 Project Structure  
```bash
ifj22-compiler/
├── src/ # Source code of the compiler
│ ├── main.c # Main entry point
│ ├── scanner.c # Lexical analyzer
│ ├── parser.c # Syntax analyzer
│ ├── symtable.c # Symbol table (BVS/TRP)
│ ├── generator.c # IFJcode22 code generator
│ ├── error.c # Error handling
│ ├── utils.c # Utility functions
├── tests/ # Test cases
├── docs/ # Documentation
├── Makefile # Build script
└── README.md # This file
```
## 🛠 **Usage**  
### 🔹 **Compilation**  
The project can be compiled using `Makefile`:  
```sh
make
```
### 🔹 **Running the Compiler**
The compiler reads an IFJ22 program from the standard input and generates IFJcode22:
```sh
./ifj22 < program.ifj > program.ifjcode
```
### 🔹 **Running the Generated Code in the Interpreter**
```sh
ic22int program.ifjcode
```
## 📜 **Supported Features**
- ✅ Lexical Analysis (identifiers, keywords, literals)
- ✅ Syntax Analysis (language structure, function definitions, expressions)
- ✅ Semantic Analysis (type checking, redefinitions, program errors)
- ✅ Intermediate Code Generation (IFJcode22 instructions)
- ✅ Symbol Table Implementation (BVS/TRP)
- ✅ Testing with ic22int interpreter

## 📖 **Documentation**
Detailed information about the implementation, algorithms, and structures can be found in docs/documentation.pdf.

## 👥 **Authors and Maintainers**
*Mark Barzali -- [@Drakorgaur](github.com/Drakorgaur)*  
*Oleksandr Turytsia -- [@turytsia](https://github.com/turytsia)*  
*Dinara Garipova -- [@20dinara03](https://github.com/20dinara03)*  
*Kateryna Zdebska -- [@zdebska](https://github.com/zdebska)*  

## 📅 **Project Deadline**: December 7, 2022
