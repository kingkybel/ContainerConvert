cmake_minimum_required(VERSION 3.26)
project(ContainerConvert
        VERSION 1.0.0
        DESCRIPTION "A header only for conversion of standard containers"
        HOMEPAGE_URL "https://github.com/kingkybel/ContainerConvert")

include(GNUInstallDirs)
include(CTest)
enable_testing()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-common/cmake/DkybBuildSettings.cmake)
dkyb_apply_common_settings(
        EXTRA_CXX_FLAGS "-DBOOST_ALLOW_DEPRECATED_HEADERS" "-DBOOST_BIND_GLOBAL_PLACEHOLDERS"
)

execute_process(
        COMMAND bash -c "printf \"%s\" $(cat /etc/os-release | grep -E \"^ID=[^\\n]*|^VERSION_ID=[^\\n]*\" | cut -f2 -d= | tr -d '\"')"
        OUTPUT_VARIABLE LINUX_VERSION
)
message(STATUS "Linux version is '${LINUX_VERSION}'")

set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(
        ${Boost_INCLUDE_DIRS}
        BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/include
        system
)
link_directories(
        /usr/local/lib
        ${Boost_LIBRARY_DIR_RELEASE}
        system
)

add_library(DirectedGraphs INTERFACE)
# Adding the install interface generator expression makes sure that the include
# files are installed to the proper location (provided by GNUInstallDirs)
target_include_directories(
        DirectedGraphs
        INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(DirectedGraphs INTERFACE cxx_std_23)

add_subdirectory(test)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=* -p ${CMAKE_SOURCE_DIR}/build")

file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

# Install the header files directly under 'include/dkyb'
install(FILES ${HEADER_FILES}
        DESTINATION include/dkyb)
