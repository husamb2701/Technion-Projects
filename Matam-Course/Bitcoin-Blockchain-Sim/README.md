# Blockchain Transaction Ledger Sim (C++)

A customized cryptocurrency transaction ledger implemented from scratch in C++ as part of an academic project. The system manages transaction records using a custom linked list with strict dynamic memory allocations.

## Project Structure & My Implementation

The framework, command-line interface (`main.cpp`), and hashing logic (`Utilities`) were provided by the course staff. 

My work focused entirely on designing and implementing the data structure internals and memory management logic (`BlockChain` and `Transaction` modules):

- **Memory Management (Rule of 3):** Implemented custom Copy Constructor, Copy Assignment Operator, and Destructor to ensure deep copying of heap-allocated nodes, preventing memory leaks (verified cleanly).
- **Singly Linked Blockchain Structure:** Managed node insertions at the head of the chain ($O(1)$ complexity) to maintain a chronological ledger.
- **Ledger Compression:** Implemented an in-place compression algorithm (`BlockChainCompress`) that safely merges sequential blocks from the same sender and receiver, optimizing time complexity and freeing unused dynamic memory.
- **Verification & Persistence:** Implemented utilities to parse transaction logs from files and verify ledger consistency using the provided cryptographic hash function.

## Tech Stack
- **Language:** C++
- **Build Tool:** CMake
- **Analytics:** Fully compliant with Clang-Tidy standards
