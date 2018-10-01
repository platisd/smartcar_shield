cmake_minimum_required(VERSION 3.0.2)
project(Smartcar_Tests)

set(REPOSITORY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(UNIT_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/ut)
set(SMARTCAR_LIB ${REPOSITORY_ROOT}/src/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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

add_definitions(-DENABLE_TESTS -DPLATFORM_AGNOSTIC_BUILD)
enable_testing()

# Common headers
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/test_libs/gmock-gtest
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/
    ${SMARTCAR_LIB}/runtime
)

add_executable(srf08_test
    ${SMARTCAR_LIB}/sensors/distance/ultrasound/i2c/SRF08.cpp
    ${UNIT_TESTS}/SRF08_test.cpp
)
target_include_directories(srf08_test PUBLIC
    ${SMARTCAR_LIB}/sensors/distance/ultrasound/i2c
)

target_link_libraries(srf08_test gtest gmock_main)

add_test(SRF08_TEST srf08_test ${GTEST_RUN_FLAGS})

# Add coverage reports
setup_target_for_coverage(NAME SRF08_TEST_coverage EXECUTABLE srf08_test DEPENDENCIES srf08_test)