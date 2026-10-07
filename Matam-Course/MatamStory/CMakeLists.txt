cmake_minimum_required(VERSION 3.28)
project(MatamStory)

set(CMAKE_CXX_STANDARD 14)

add_executable(MatamStory 
    main.cpp 
    MatamStory.cpp 
    MatamStory.h 
    Event.cpp
    Event.h
    Player.cpp
    Player.h 
    Utilities.cpp
    Utilities.h	
)
