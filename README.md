# BitVaram

BitVaram is a non-deterministic, calculus-paradigm-oriented programming language implemented in C++.  
It is based on a primitive bit-level memory model combined with dynamic scoping and an adaptive runtime environment.

The core idea of BitVaram is a flexible definition model where functions and variables can be defined or undefined  
dynamically at runtime, both locally within the running program and globally in the language core.  
This allows programs to adapt their behavior on the fly, enabling different executions of the same code  
to produce different results depending on the current context and state.

## Key Features

- Primitive Memory Model: Operations occur at the bit level for fine-grained control.
- Adaptive Definitions: Dynamic creation and removal of variables and functions globally or locally.
- Dynamic Scope: Variable resolution is determined at runtime, allowing flexible environment influence.
- Registry Memory: Maintains global state and definitions across runs.
- Functional Mechanics: Supports function declarations and calls integrated with memory and scope.

## Project Structure

- src/ — Core C++ source files.
- include/ — Header files.
- docs/ — Design documents and specifications.
- examples/ — Sample programs.
- tests/ — Unit and integration tests.
- LICENSE — License file.

## Building the Project

1. Clone the repository:
   git clone https://github.com/DanilaEfimov/BitVaram.git
   cd BitVaram

2. Create build directory and compile:
   mkdir build
   cd build
   cmake ..
   make

3. The executable (e.g. `bitvaram`) will be generated in the build directory.

## Running

Run the executable to start the BitVaram interpreter or compiler depending on the current implementation.

## Contribution

Contributions and suggestions are welcome. Open issues or pull requests for discussion.

## License

Licensed under the MIT License. See LICENSE for details.
