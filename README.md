# CS530 Assignment 2 - Limited SIC/XE Assembler (`lxe`)

## Team Information

Team member | Class account / RedId<br />
Hector Anaya  CSSC2505  826734851<br />
Emiliano Nolasco CSSC2545 130310255<br />
Ethan Fudge cssc2519 827570933<br />


## Project Summary

This repository contains a **limited two-pass assembler for the XE variant of the SIC/XE machine**, written in C++ and built with `g++`/`make`. Per the project handout, The executable is named **`lxe`**. 

This assembler reads one or more `.sic` source files from the command line and performs a **Pass 1** parsing the source code and computing the addresse. It then then performs a **Pass 2** to generate:

- a **listing file** with the `.l` extension, and
- a **symbol/literal table file** with the `.st` extension.

Per the assignment scope: this project focuses on the assembler passes and listing/symbol table generation rather than emitting a separate object program file.

## Assignment Scope Addressed

This project is intended to support the following core requirements that are based off the handout and the current codebase :

- two-pass assembly flow
- XE instruction handling using an external opcode table
- listing file generation (`.l`)
- symbol/literal table generation (`.st`)
- command-line processing of one or more source files
- support for key SIC/XE addressing and assembler features used in the provided test cases




### Features implemented in the uploaded code

From the current source files, the assembler includes logic for:

- **instruction formats 1, 2, 3, and 4**
- **simple, immediate, indirect, and indexed addressing**
- **PC-relative and base-relative displacement calculation**
- **extended format (`+`) instructions**
- **literal handling** for values such as `=C'EOF'` & `=X'05'`
- **literal pool emission** through `LTORG` & `END`
- **program block support** through the `USE` directive
- **block finalization** & block start address calculation
- **register instruction encoding** for format 2 operations
- **listing output with object-code field when applicable**

### Directives visible in the current snapshot


The uploaded code explicitly handles or recognizes the following directives in its current form:

- `START`
- `END`
- `USE`
- `LTORG`
- `BASE`
- `NOBASE`
- `WORD`
- `BYTE`
- `RESW`
- `RESB`

### Scope notes

- The assignment handout excludes full `ORG` support and does not require full `EQU` implementation.
- The current repository includes **partial expression-handling utilities** (for example, arithmetic-style operands such as `BUFEND-BUFFER`), but this area should still be treated as limited/non-central relative to the required scope.
- The assembler produces **listing and table output**, not a standalone object program file.

## High-Level Design

The repository follows a modular design centered around a classic two-pass assembler pipeline.

### Main control flow

1. **`ixe.cpp`** receives one or more source file names from the command line.
2. **`Pass1`** reads and parses the source, computes addresses, tracks blocks, collects literals, and stores line metadata for later use.
3. **`Pass2`** reads the intermediate in-memory representation from Pass 1, resolves addresses, encodes instructions, and writes `.l` and `.st` files.
4. Shared helper/data classes provide opcode lookup, label metadata, block metadata, token storage, and literal records.

### Design artifact included with the project

The file **`Tentative Assembler Diagram.drawio`** serves as the design artifact included with this repository. It captures the intended class/module relationships at a high level and can be submitted alongside the rest of the project materials as part of the required software design documentation.

## Core Source Files

### Driver

- **`ixe.cpp`**  
  Program entry point. Validates command-line arguments, runs Pass 1 and Pass 2 for each input file, and clears shared tables between files.

### Pass implementation

- **`Pass1.h` / `Pass1.cpp`**  
  Defines the `Bits`, `Operand`, and `SourceLine` structures and implements the first assembler pass. Responsibilities include parsing source lines, recognizing addressing-mode prefixes, calculating location counters, handling comments/blank lines, collecting literals, and preparing intermediate line records for Pass 2.

- **`Pass2.h` / `Pass2.cpp`**  
  Implements the second assembler pass. Responsibilities include literal conversion, address resolution, instruction encoding for formats 1-4, base-register handling, listing-file generation, and symbol/literal table output.

### Opcode and instruction metadata

- **`OpCode.h` / `OpCode.cpp`**  
  Static opcode-table manager. Loads mnemonic/opcode metadata from the external opcode file and provides opcode validation and lookup.

- **`Code.h` / `Code.cpp`**  
  Small data class for opcode metadata: hexadecimal opcode value, instruction format, and length.

- **`opcodes.txt`**  
  External opcode table used to populate the mnemonic lookup structure. The uploaded file contains the instruction mnemonic, opcode hex value, instruction format, and length fields used by the assembler.

### Symbol, literal, and block data types

- **`Label.h` / `Label.cpp`**  
  Stores label metadata such as address, flag, and block number.

- **`LiteralEntry.h`**  
  Structure representing literal records, including raw literal text, hex content, assigned address, length, and block number.

- **`BData.h` / `BData.cpp`**  
  Encapsulates per-block location counter, start address, length, and block number.

- **`PBlocks.h` / `PBlocks.cpp`**  
  Manages program blocks, current block selection, and final block start-address assignment.

### Additional supporting files

- **`Tokens.h` / `Tokens.cpp`**  
  Defines a token class used during parsing and line bookkeeping.

- **`Error.h` / `Error.cpp`**  
  Placeholder error container used by the token/error infrastructure.

## Repository Contents by Artifact Type

### Source, headers, and build files

- `ixe.cpp`
- `Pass1.cpp`, `Pass1.h`
- `Pass2.cpp`, `Pass2.h`
- `OpCode.cpp`, `OpCode.h`
- `Code.cpp`, `Code.h`
- `Label.cpp`, `Label.h`
- `LiteralEntry.h`
- `BData.cpp`, `BData.h`
- `PBlocks.cpp`, `PBlocks.h`
- `Tokens.cpp`, `Tokens.h`
- `Error.cpp`, `Error.h`
- `Makefile`
- `README.md`

### Test source programs and checked-in reference artifacts

- `copy.sic`, `copy.l`, `copy.st`, `copy.out`
- `blocks.sic`, `blocks.l`, `blocks.st`, `blocks.out`
- `work.sic`, `work.l`, `work.st`, `work.out`
- `P2sample.sic`, `P2sample.l`, `P2sample.st`, `P2.out`

### Documentation and design files

- `Assignment 2.Spring 2026 (2).pdf`
- `CS 530 Course Syllabus - Spring 2026 (1).pdf`
- `Tentative Assembler Diagram.drawio`

### Generated / development artifacts included in the upload

- `lxe` - prebuilt executable present in the uploaded snapshot
- `a.out` - console/debug output artifact from development

## Build Instructions

### Environment

This project is intended for a GNU/Linux environment using:

- `g++`
- `make`
- standard C++ library headers (`iostream`, `fstream`, `sstream`, `vector`, `unordered_map`, `iomanip`, `algorithm`, etc.)

### Build commands

```bash
make clean
make
```

This should produce the executable:

```bash
./lxe
```

## Runtime Notes Before Executing

The current uploaded source snapshot contains one important path assumption:

- **`OpCode.cpp` currently opens `data/opcodes.txt`**
- the uploaded opcode file is present as **`opcodes.txt` in the project root**

To run the current source without changing code, create the expected folder/file path first:

```bash
mkdir -p data
cp opcodes.txt data/opcodes.txt
```

If you prefer, you can instead update the file path in `OpCode.cpp` to match the location of `opcodes.txt` in your final submission layout.

## How to Run

The program accepts one or more `.sic` files as command-line arguments.

### Single file

```bash
./lxe copy.sic
```

### Multiple files

```bash
./lxe copy.sic blocks.sic work.sic P2sample.sic
```

If no source file is supplied, `ixe.cpp` prints an error/usage message and terminates.

## Input and Output Conventions

### Input

Input files are SIC/XE source files using the fixed-field style shown in the textbook-style examples included with the assignment. The uploaded test set includes examples with comments, literals, blocks, immediate operands, indirect operands, indexed operands, and extended-format instructions.

### Output

In the current uploaded code snapshot, Pass 2 writes generated files into an **`output/`** directory using the base name of the source file:

- `output/<name>.l`
- `output/<name>.st`

The repository also includes checked-in root-level `.l` and `.st` files, which appear to be saved development/reference outputs.

## Included Test Programs

| Test File | Purpose / Features Exercised | Included Reference Artifacts |
| --- | --- | --- |
| `P2sample.sic` | Format 4 instructions, `BASE`, indexed addressing, literal use, large displacements | `P2sample.l`, `P2sample.st`, `P2.out` |
| `blocks.sic` | `USE` program blocks, literals across blocks, extended instructions | `blocks.l`, `blocks.st`, `blocks.out` |
| `work.sic` | COPY-style workload used during development, literals, block switching, I/O instructions | `work.l`, `work.st`, `work.out` |
| `copy.sic` | COPY-style assembler test with literals, block use, expressions, and subroutine-style flow | `copy.l`, `copy.st`, `copy.out` |

### Debug/trace artifacts

The `.out` files and `a.out` are not assembler input files. They appear to be development-time console/debug traces captured while testing the parser and the two assembler passes.

## Snapshot Verification Notes

To understand the current state of the uploaded repository, the source snapshot was rebuilt and compared against the checked-in `.l/.st` artifacts after providing the expected `data/opcodes.txt` path.

### Observed status of the uploaded snapshot

| Case | Comparison to checked-in `.l/.st` artifacts |
| --- | --- |
| `P2sample.sic` | matched the uploaded reference outputs |
| `blocks.sic` | matched the uploaded reference outputs |
| `work.sic` | matched the uploaded reference outputs |
| `copy.sic` | did **not** fully match the uploaded reference outputs |

### Practical interpretation

The repository is clearly far along and includes working evidence for several representative test cases, but the `copy.sic` case should be re-verified before final turn-in if that file is intended to be part of the official test set submitted with the project.

## Software Design / SDD Notes

The assignment requires software design documentation. For this repository, the most relevant design artifact already included is:

- **`Tentative Assembler Diagram.drawio`**

A good final submission should include that file (or an exported PDF/image version of it) as the SDD/design artifact and should explain:

- the two-pass architecture
- the role of the opcode table
- the way program blocks are managed
- how literals are collected and emitted
- how listing and symbol-table files are produced

## Tools and Technologies Used

The uploaded files provide direct evidence of the following tools/technologies:

- **C++** as the implementation language
- **GNU `g++`** as the compiler
- **`make`** via the included `Makefile`
- **C++ standard library** containers and stream libraries
- **diagrams.net / draw.io** for the design diagram (`.drawio` file)
- **PDF course documents** used as assignment/syllabus references

If your team also used source control, an IDE, a debugger, or other tooling, add them here before submission.

## Team Organization and Reflection

The assignment specifically asks for documentation describing how the team was organized, how effectively the team worked together, and areas for improvement. The uploaded files do not record that information in detail, so this section is written as a polished template to complete before final submission.

### Suggested completion template

**Organization of work**  
The project was organized around the major components of a two-pass assembler: requirements review, Pass 1 development, Pass 2 code-generation logic, testing against provided SIC/XE examples, and documentation/design work. Team members coordinated changes so that opcode handling, symbol/literal tracking, program-block support, and listing-file generation worked together as a single pipeline.

**What worked well**  
The modular file layout made it easier to separate responsibilities. Breaking the project into passes and helper classes (`OpCode`, `Code`, `PBlocks`, `Label`, and literal support) reduced the amount of logic concentrated in any single file and made debugging more manageable.

**Areas for improvement**  
For a future iteration, the team could improve consistency between repository layout and runtime assumptions (for example, the opcode-file path), increase explicit error reporting, and keep one authoritative set of regression outputs for every included test case.

## Closing Notes

This repository demonstrates a substantial SIC/XE assembler implementation with a clear two-pass structure, modular support classes, multiple test programs, checked-in listing/table artifacts, and an included design diagram. The project already contains most of the material expected for the assignment package; the main remaining work is to ensure the final submission metadata, runtime packaging, and test verification are completely aligned with the course handout.
