# Task Manager System (C++)

A task management system implemented in C++ as part of an academic project. The system allows managing employees, assigning tasks, and organizing them based on priority and type.

## Project Structure & Implementation

- **`SortedList.h`**: A custom, template-based sorted linked list. Designed to be generic and support modern C++ iteration (range-based loops) and functional features (`filter`, `apply`).
- **`Task` & `Person` Modules**: Represent the core entities. Implemented using efficient string handling (via `std::move`) and proper constructor initialization to ensure performance and prevent memory leaks.
- **`TaskManager`**: The central controller managing the collection of employees. It handles task assignment, completion, and priority bumping, utilizing the custom `SortedList` for data organization.

## Key Technical Highlights

- **Exception Safety**: The `SortedList` implementation follows strong exception guarantees, ensuring no memory leaks occur if an allocation fails during copying.
- **Modern C++ Conventions**: 
    - Used `std::move` for efficient object transfer.
    - Implemented `constexpr` for static constants.
    - Added `explicit` constructors to prevent accidental implicit type conversions.
- **Generic Programming**: Designed list operations (`filter`/`apply`) to work with any callable object (function pointers, functors, or lambdas).

## Tech Stack
- **Language**: C++17
- **Build System**: CMake
- **Analytics**: Fully compliant with Clang-Tidy standards.
