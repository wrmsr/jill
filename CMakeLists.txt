cmake_minimum_required(VERSION 2.8.8)
project(SimpleProject)

enable_language(CXX)

####

# add_subdirectory(llvm)
# add_library(llvm INTERFACE)
# target_sources(llvm INTERFACE)
# target_include_directories(llvm INTERFACE
#         ${CMAKE_CURRENT_SOURCE_DIR}/llvm/include
#         ${CMAKE_CURRENT_BINARY_DIR}/llvm/include
#         )

####

# CFLAGS := $(shell ../llvm/build/bin/llvm-config --cxxflags --ldflags --libs bitreader codegen selectiondag)

# -I/Users/spinlock/src/wrmsr/jill/llvm/include
# -I/Users/spinlock/src/wrmsr/jill/build/llvm/include

# -fPIC
# -fvisibility-inlines-hidden
# -Wall
# -W
# -Wno-unused-parameter
# -Wwrite-strings
# -Wcast-qual
# -Wmissing-field-initializers
# -pedantic
# -Wno-long-long
# -Wcovered-switch-default
# -Wnon-virtual-dtor
# -Wdelete-non-virtual-dtor
# -std=c++11
# -g

# -fno-exceptions
# -fno-rtti
# -D__STDC_CONSTANT_MACROS
# -D__STDC_FORMAT_MACROS
# -D__STDC_LIMIT_MACROS

# -L/Users/spinlock/src/wrmsr/jill/build/llvm/lib
# -Wl,-search_paths_first
# -Wl,-headerpad_max_install_names

# -lLLVMSelectionDAG
# -lLLVMCodeGen
# -lLLVMTarget
# -lLLVMScalarOpts
# -lLLVMInstCombine
# -lLLVMInstrumentation
# -lLLVMProfileData
# -lLLVMObject
# -lLLVMMCParser
# -lLLVMTransformUtils
# -lLLVMMC
# -lLLVMBitWriter
# -lLLVMAnalysis
# -lLLVMBitReader
# -lLLVMCore
# -lLLVMSupport

# add_executable(jill
#         main.cpp
#         )

# set_target_properties(jill PROPERTIES COMPILE_FLAGS -std=c++14)

# target_link_libraries(jill llvm LLVMCore LLVMBitReader LLVMCodeGen LLVMSelectionDAG)

####

# find_package(LLVM REQUIRED CONFIG)
#
# message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
# message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#
# include_directories(${LLVM_INCLUDE_DIRS})
# add_definitions(${LLVM_DEFINITIONS})

add_executable(jill
        backend.cpp
        backend.hpp
        block.cpp
        branch.cpp
        const.cpp
        function.cpp
        instruction.cpp
        loadstore.cpp
        main.cpp
        name.cpp
        printinst.cpp
        sections.cpp
        types.cpp
        util.cpp
        )

target_compile_options(jill PRIVATE -std=c++14 -fno-exceptions -fno-rtti)

target_include_directories(jill PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# llvm_map_components_to_libnames(LLVM_LIBS support core irreader analysis transformutils codegen)
# target_link_libraries(jill ${LLVM_LIBS})
