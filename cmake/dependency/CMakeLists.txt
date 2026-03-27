cmake_minimum_required(VERSION 3.26)
project(DkybDependencyBuild LANGUAGES NONE)

execute_process(
    COMMAND git config --global url.https://github.com/.insteadOf git@github.com:
)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake-common/cmake/DkybDependency.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake-common/cmake/DkybDependencyRegistry.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake-common/cmake/DkybDependencyRunner.cmake)
