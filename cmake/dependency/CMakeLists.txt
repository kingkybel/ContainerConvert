cmake_minimum_required(VERSION 3.26)
project(DkybDependencyBuild LANGUAGES NONE)

execute_process(
    COMMAND git config --global url.https://github.com/.insteadOf git@github.com:
)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake-common/cmake/DkybDependency.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake-common/cmake/DkybDependencyRegistry.cmake)

set(_gtest_build_target "")
if(DKYB_DEPENDENCY_NAME STREQUAL "typetraits")
    message(STATUS "Ensuring gtest is built before typetraits")
    dkyb_build_and_cache(gtest 1.17.0)
    dkyb_dependency_lookup(gtest 1.17.0 BUILD_TARGET _gtest_build_target)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake-common/cmake/DkybDependencyRunner.cmake)

if(_gtest_build_target)
    add_dependencies(dkyb_dependency_runner ${_gtest_build_target})
endif()
