# CMake setup
cmake_minimum_required(VERSION 3.16)
project(GATM)

# Project setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output dirs
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/debug)
else ()
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/release)
endif ()

# Sources
set(GATM_SOURCE_FILES src/main.cpp src/instruction/Instruction.cpp src/instruction/Instruction.hpp src/Machine.cpp src/Machine.hpp src/instruction/Add.cpp src/instruction/Add.hpp src/instruction/Immediate.cpp src/instruction/Immediate.hpp src/instruction/Sub.cpp src/instruction/Sub.hpp src/instruction/Mul.cpp src/instruction/Mul.hpp src/instruction/Div.cpp src/instruction/Div.hpp src/instruction/JumpZero.cpp src/instruction/JumpZero.hpp src/instruction/Not.cpp src/instruction/Not.hpp src/instruction/Halt.cpp src/instruction/Halt.hpp src/instruction/Copy.cpp src/instruction/Copy.hpp)

# Flags
set(COMPILE_FLAGS "${COMPILE_FLAGS}")
add_definitions(${COMPILE_FLAGS})

# Include package manager
include(cmake/CPM.cmake)

# Load dependencies
CPMAddPackage("gh:onqtam/doctest#2.4.6")
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

# Includes
#include_directories()

# Setup compilation & linking
add_executable(GATM ${GATM_SOURCE_FILES})
add_library(GATM_STATIC STATIC ${GATM_SOURCE_FILES})
target_link_libraries(GATM PUBLIC Threads::Threads OpenMP::OpenMP_CXX )

# Add tests
add_subdirectory(test)