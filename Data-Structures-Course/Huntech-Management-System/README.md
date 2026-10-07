# Huntech Management System

A high-performance management system designed for coordinating hunters and squads. The system utilizes advanced data structures to ensure efficient data handling, rapid queries, and optimal resource management.

## System Architecture
- **Data Structures**: 
  - **AVL Trees**: Managed squad and hunter indexing for O(log n) performance.
  - **DSU (Disjoint Set Union)**: Implemented with path compression for efficient squad merging and capability aggregation.
  - **HashMap**: Custom implementation for O(1) average-time hunter lookups.
- **Memory Management**: Utilizes modern C++ memory management techniques (`shared_ptr`, `weak_ptr`) to ensure system stability and prevent resource leaks.

## Technical Stack
- **Language**: C++17
- **Performance**: Optimized for O(log n) time complexity on core operations.

## Build Instructions
To compile the system, use the following G++ command:
`g++ --std=c++17 -DNDEBUG -Wall -pedantic-errors -Werror *.cpp -I.`
