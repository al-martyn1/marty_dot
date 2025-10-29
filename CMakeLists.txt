cmake_minimum_required(VERSION 3.21)
project(marty_dot CXX)

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


set(MODULE_ROOT "${CMAKE_CURRENT_LIST_DIR}")

file(GLOB sources "${MODULE_ROOT}/*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Sources" FILES ${sources})

file(GLOB_RECURSE headers "${MODULE_ROOT}/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Headers" FILES ${headers})

file(GLOB_RECURSE all_docs "${MODULE_ROOT}/*.md" "${MODULE_ROOT}/*.txt")
list(FILTER all_docs EXCLUDE REGEX ".*CMakeLists\.txt$")
list(FILTER all_docs EXCLUDE REGEX ".*_enums/.*")
list(FILTER all_docs EXCLUDE REGEX ".*examples/.*")
list(FILTER all_docs EXCLUDE REGEX ".*md_/.*")
list(FILTER all_docs EXCLUDE REGEX ".*_md/.*")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Docs" FILES ${all_docs})

file(GLOB_RECURSE all_docs_src "${MODULE_ROOT}/*.md_" "${MODULE_ROOT}/_*.md")
list(FILTER all_docs_src EXCLUDE REGEX ".*md_/.*")
list(FILTER all_docs_src EXCLUDE REGEX ".*_md/.*")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Docs.src" FILES ${all_docs_src})


add_library(${PROJECT_NAME} OBJECT ${sources} ${headers} ${all_docs} ${all_docs_src})
add_library(marty::dot ALIAS ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
