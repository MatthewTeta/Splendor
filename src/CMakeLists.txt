cmake_minimum_required(VERSION 3.13)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)
 
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # GIT_TAG cdf28903a7781f97ba94d30b79c3a4b0c97ccce7
  GIT_TAG "v5.0.0"
)
 
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
 
# ------------------------------------------------------------------------------

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
set(CMAKE_GENERATOR Ninja)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

include_directories(${CMAKE_SOURCE_DIR})
file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/*.cpp)
# set(SOURCE_FILES main.cpp Pieces.hpp Board.hpp)

# Add main.cpp file of the project root directory as a source file
# set(SOURCE_FILES main.cpp Pieces.hpp Board.hpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(Splendor ${SOURCE_FILES})

target_link_libraries(Splendor
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component)

set_target_properties(Splendor
            PROPERTIES
                LINKER_LANGUAGE
                    CXX
        )
