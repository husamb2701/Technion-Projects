# Graphic Matrix & Movie Simulator (C++)

A C++ object-oriented project designed to simulate core graphical transformations using mathematical matrices and sequentially manage them as "Movie" frames. This was developed as part of an academic assignment focusing on low-level memory management and operator overloading.

## Project Structure & My Implementation

The test suite (`tests.cpp`), the basic error-handling skeleton (`Utilities`), and the core architectural requirements were provided by the university course staff. 

My work focused entirely on designing and safely implementing the inner logic, dynamic memory allocations, and mathematical operations for the two main modules:

### 1. The `Matrix` Module
- **Memory Management:** Implemented the Rule of 3 (Custom Copy Constructor, Assignment Operator, and Destructor) to handle dynamic 1D arrays representing 2D matrices, preventing memory leaks.
- **Operator Overloading:** Deeply overloaded mathematical operators (`+`, `-`, `*`, `+=`, `-=`, `*=`, and unary `-`) to support both Matrix-Matrix and Matrix-Scalar operations.
- **Algorithms:** Implemented matrix rotation (clockwise/counter-clockwise), transposition, Frobenius Norm calculation, and a recursive algorithm for Determinant calculation.

### 2. The `MataMvidia` (Movie) Module
- **Dynamic Frame Management:** Managed a dynamically allocated array of `Matrix` objects representing sequential movie frames.
- **Safe Concatenation:** Implemented deep-copy concatenation operators (`+=`, `+`) to merge individual frames or entire movies together, ensuring old memory is safely freed and reallocated without leaks or dangling pointers.

## Tech Stack
- **Language:** C++ (Strictly compiled with `-std=c++17`, `-Wall`, `-Werror`)
- **Build Tool:** CMake
- **Memory & Syntax:** Fully validated against Clang-Tidy standards.
