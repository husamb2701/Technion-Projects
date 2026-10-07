# TechSystem

A management system for students and courses, implemented using AVL trees.

## Project Overview
The system provides efficient management of students and academic courses using an AVL tree data structure to ensure optimal time complexity for all operations.

## Core Components
- **TechSystem**: The main system logic managing the interaction between students and courses.
- **AVL Tree**: A generic AVL tree implementation ensuring balanced operations.
- **Entities**: Data classes for `Student` and `Course`, managed via `shared_ptr` to ensure consistent data ownership.

## Complexity
The system adheres to strict time complexity requirements as specified in the project documentation. Theoretical analysis is provided in `dry.pdf`.

## Compilation
To compile the project, use the following command:
`g++ --std=c++17 -DNDEBUG -Wall -pedantic-errors -Werror *.cpp -I.`
