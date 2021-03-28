# ------------------------------------------------------------
# This file is the main CMakeLists of url of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2021 headcode.space e.U.  
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------


project(hcs-url C CXX)
cmake_minimum_required(VERSION 3.13.4)

cmake_policy(SET CMP0075 NEW)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${HCS_CMAKE}")
include(conan)

if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif ()
message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")

ENABLE_TESTING()


# ------------------------------------------------------------
# Project Version

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")


# ------------------------------------------------------------
# Prime CMake includes

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(CheckLibraryExists)

include(run-gcovr)


# ------------------------------------------------------------
# Compiler

add_definitions(-DVERSION_MAJOR=${VERSION_MAJOR})
add_definitions(-DVERSION_MINOR=${VERSION_MINOR})
add_definitions(-DVERSION_PATCH=${VERSION_PATCH})
add_definitions(-DVERSION=\"${VERSION}\")
add_definitions(-D_GNU_SOURCE)
add_definitions(-D_DEFAULT_SOURCE)

# ---- fix C++ standard used ----

set(CMAKE_CXX_STANDARD 17)
if ((CMAKE_COMPILER_IS_GNUCC) OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"))
    set(COMPILER_WARNINGS "-Wall -Wextra -pedantic -pedantic-errors")
    set(COMPILER_FLAGS_DEBUG "-O0 -g -ggdb3 -fno-builtin -fPIC")
    set(COMPILER_FLAGS_RELEASE "-O3 -Werror -Werror -fno-builtin -fPIC")
    set(LINKER_FLAGS "-rdynamic")
endif ()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    message(STATUS "Turning on compiler command export for CLang: ${CMAKE_BINARY_DIR}/compile_commands.json")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions("DEBUG")
endif ()


# ---- callgrind integration ----

option(CALLGRIND_ENABLED "enable callgrind" off)
if (CALLGRIND_ENABLED)
    message(STATUS "callgrind enabled")
    message(WARNING "callgrind enabled: make package will create defective package!")
    set(VALGRIND_NEEDED "1")
    set(CALLGRIND_OPTIONS "")
else ()
    message(STATUS "callgrind disabled")
endif ()

# ---- final compiler settings ----

set(CMAKE_C_FLAGS_DEBUG "${COMPILER_WARNINGS} ${COMPILER_FLAGS_DEBUG} ${COMPILER_FLAGS_PROFILING}")
set(CMAKE_CXX_FLAGS_DEBUG "${COMPILER_WARNINGS} ${COMPILER_FLAGS_DEBUG} ${COMPILER_FLAGS_PROFILING}")
set(CMAKE_C_FLAGS_RELEASE "${COMPILER_WARNINGS} ${COMPILER_FLAGS_RELEASE} ${COMPILER_FLAGS_PROFILING}")
set(CMAKE_CXX_FLAGS_RELEASE "${COMPILER_WARNINGS} ${COMPILER_FLAGS_RELEASE} ${COMPILER_FLAGS_PROFILING}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${LINKER_FLAGS_PROFILING}")


# ------------------------------------------------------------
# Doxygen

find_package(Doxygen)
if (DOXYGEN_FOUND STREQUAL "NO")
    message(FATAL_ERROR "Doxygen not found. Please get a copy http://www.doxygen.org")
endif ()

configure_file(${CMAKE_SOURCE_DIR}/Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)
add_custom_target(doxygen ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        COMMENT "Doxygen running ...")
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES api-doc)

add_custom_target(doc)
add_dependencies(doc doxygen)
install(DIRECTORY ${CMAKE_BINARY_DIR}/html
        DESTINATION share/doc/hcs-url-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
        COMPONENT documentation
)


# ------------------------------------------------------------ 
# Workload

add_subdirectory(src)
option(BUILD_TESTING "enable testing" off)
if (BUILD_TESTING)
    message(STATUS "Building tests")
    add_subdirectory(test)
else()
    message(STATUS "Building without tests")
    message(STATUS "To enable tests invoke cmake with \"-D BUILD_TESTING=on\" arguments.")
endif ()

add_subdirectory(tools)


# ------------------------------------------------------------
# Installing

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/headcode/url DESTINATION include/headcode COMPONENT header)

# ---- CPack global definitions ----

set(CPACK_PACKAGE_NAME "hcs-url")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})

set(CPACK_PACKAGE_DESCRIPTION "URL parsing and resolving library in C++17.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A small C++17 library parsing and resolving URLs (and URIs).")
set(CPACK_PACKAGE_CONTACT "Oliver Maurhart <oliver.maurhart@headcode.space>")
set(CPACK_PACKAGE_VENDOR "headcode.space")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-${CMAKE_SYSTEM_PROCESSOR}")

set(CPACK_SOURCE_IGNORE_FILES "./build/*;/.git/;/.idea/;/.scannerwork/;/.sonar/;/sonar-project.properties")
set(CPACK_SOURCE_GENERATOR "TGZ")


if ("${CPACK_GENERATOR}" STREQUAL "DEB")

    set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
    set(PACKAGE_DEB_CONTROL_DIR "tools/package/deb")
    include(cmake/deb-package.cmake)

    # only works inside a GitLab CI/CD runner
    set(GITLAB_PACKAGE_BASE_URL "https://gitlab.com/api/v4/projects/25468434/packages/generic/hcs-url")
    set(PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}.deb")
    set(PACKAGE_FILE "${CMAKE_BINARY_DIR}/${PACKAGE_FILE_NAME}")
    include(gitlab-package-push)

endif()

if ("${CPACK_GENERATOR}" STREQUAL "RPM")

    set(CPACK_RPM_PACKAGE_REQUIRES "")
    set(PACKAGE_RPM_CONTROL_DIR "tools/package/rpm")
    include(cmake/rpm-package.cmake)

    # only works inside a GitLab CI/CD runner
    set(GITLAB_PACKAGE_BASE_URL "https://gitlab.com/api/v4/projects/25468434/packages/generic/hcs-url")
    set(PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}.rpm")
    set(PACKAGE_FILE "${CMAKE_BINARY_DIR}/${PACKAGE_FILE_NAME}")
    include(gitlab-package-push)

endif()

include(CPack)
