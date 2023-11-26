cmake_minimum_required(VERSION 3.13)

project(Splendor
            VERSION
                1.0
            DESCRIPTION
                "Splendor Game Simulator"
            LANGUAGES
                CXX
        )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

set(SOURCE_FILES ./src/main.cpp)

# Add main.cpp file of the project root directory as a source file
# set(SOURCE_FILES main.cpp Pieces.hpp Board.hpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(Splendor ${SOURCE_FILES})
set_target_properties(Splendor
            PROPERTIES
                LINKER_LANGUAGE
                    CXX
            )
