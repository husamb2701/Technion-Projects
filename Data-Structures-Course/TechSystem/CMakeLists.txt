cmake_minimum_required(VERSION 3.28)
project(wet1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(wet1
    main26a1.cpp
    TechSystem26a1.cpp
    TechSystem26a1.h
    wet1util.h
        AVL.h
        Course.h
        Student.cpp
        Student.h)
