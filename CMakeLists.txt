cmake_minimum_required(VERSION 2.8.8)
project(SimpleProject)

enable_language(CXX)

add_subdirectory(llvm)
add_library(llvm INTERFACE)
target_sources(llvm INTERFACE)
target_include_directories(llvm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/llvm/include ${CMAKE_CURRENT_BINARY_DIR}/llvm/include)

# CFLAGS := $(shell ../llvm/build/bin/llvm-config --cxxflags --ldflags --libs bitreader codegen selectiondag)

add_executable(jill
        main.cpp
        )
set_target_properties(jill PROPERTIES COMPILE_FLAGS -std=c++14)

target_link_libraries(jill llvm LLVMCore LLVMSelectionDAG)
