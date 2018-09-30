cmake_minimum_required(VERSION 3.0.2)
project(Smartcar_Tests)

set(REPOSITORY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SMARTCAR_SOURCE ${REPOSITORY_ROOT}/src)
set(SMARTCAR_HEADERS ${REPOSITORY_ROOT}/src/include)
add_compile_definitions(PLATFORM_AGNOSTIC_BUILD)

# Coverage
list(APPEND CMAKE_MODULE_PATH ${REPOSITORY_ROOT}/CmakeModules)
include(CodeCoverage)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
set(GTEST_RUN_FLAGS --gtest_color=yes --gtest_repeat=5 --gtest_shuffle)

# Download and unpack googletest at configure time
configure_file(test_libs/CMakeLists.txt
googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
include_directories("${gtest_SOURCE_DIR}/include"
    "${gmock_SOURCE_DIR}/include")

add_definitions(-DENABLE_TESTS)
enable_testing()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/test_libs/gmock-gtest
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/
)


# # test DataStore class
# add_executable(test_data_store
#     ${REPOSITORY_ROOT}/include/data_store.hpp
#     ${REPOSITORY_ROOT}/src/data_store.cpp
#     suite_data_store.cpp
# )
# target_link_libraries(test_data_store gtest gmock_main)
#
# add_test(DATA_STORE_TEST test_data_store ${GTEST_RUN_FLAGS})
#
# # Add coverage reports
# setup_target_for_coverage(NAME DATA_STORE_TEST_coverage EXECUTABLE test_data_store DEPENDENCIES test_data_store)
