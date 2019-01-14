cmake_minimum_required(VERSION 3.0.2)
project(Smartcar_Tests)

set(REPOSITORY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(UNIT_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/ut)
set(SMARTCAR_LIB ${REPOSITORY_ROOT}/src)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Coverage
list(APPEND CMAKE_MODULE_PATH ${REPOSITORY_ROOT}/CmakeModules)
include(CodeCoverage)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
set(GTEST_RUN_FLAGS --gtest_color=yes --gtest_repeat=5 --gtest_shuffle)

# Download and unpack googletest at configure time
configure_file(test_libs/CMakeLists.txt googletest-download/CMakeLists.txt)
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

# Utility functions
function(configure_test testExecutable)
    # Link against gtest library
    target_link_libraries(${testExecutable} gtest gmock_main)
    # Create test name as the capitalized form of the test executable
    string(TOUPPER ${testExecutable} testName)
    # Add executable to test suite
    add_test(${testName} ${testExecutable} ${GTEST_RUN_FLAGS})
    # Add tests to coverage reports
    setup_target_for_coverage(NAME ${testName} EXECUTABLE ${testExecutable} DEPENDENCIES ${testExecutable})
endfunction(configure_test)

# Common headers
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/test_libs/gmock-gtest
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/
    ${SMARTCAR_LIB}/runtime
)

# SRF08 sensor test
add_executable(srf08_test
    ${SMARTCAR_LIB}/sensors/distance/ultrasound/i2c/SRF08.cpp
    ${UNIT_TESTS}/SRF08_test.cpp
)
target_include_directories(srf08_test PUBLIC
    ${SMARTCAR_LIB}/sensors/distance/ultrasound/i2c
)
configure_test(srf08_test)

# SR04 sensor test
add_executable(sr04_test
    ${SMARTCAR_LIB}/sensors/distance/ultrasound/ping/SR04.cpp
    ${UNIT_TESTS}/SR04_test.cpp
)
target_include_directories(sr04_test PUBLIC
    ${SMARTCAR_LIB}/sensors/distance/ultrasound/ping
)
configure_test(sr04_test)

# Median calculator test
add_executable(utilities_test
    ${UNIT_TESTS}/Utilities_test.cpp
)
target_include_directories(utilities_test PUBLIC
    ${SMARTCAR_LIB}/utilities
)
configure_test(utilities_test)

# InfraredAnalogSensor test
add_executable(infraredanalogsensor_test
    ${SMARTCAR_LIB}/sensors/distance/infrared/analog/InfraredAnalogSensor.cpp
    ${UNIT_TESTS}/InfraredAnalogSensor_test.cpp
)
target_include_directories(infraredanalogsensor_test PUBLIC
    ${SMARTCAR_LIB}/sensors/distance/infrared/analog
)
configure_test(infraredanalogsensor_test)

# SHARP Infrared sensors test
add_executable(sharpsensors_test
    ${SMARTCAR_LIB}/sensors/distance/infrared/analog/sharp/GP2D120.cpp
    ${SMARTCAR_LIB}/sensors/distance/infrared/analog/sharp/GP2Y0A02.cpp
    ${SMARTCAR_LIB}/sensors/distance/infrared/analog/sharp/GP2Y0A21.cpp
    ${SMARTCAR_LIB}/sensors/distance/infrared/analog/InfraredAnalogSensor.cpp
    ${UNIT_TESTS}/SharpSensors_test.cpp
)
target_include_directories(sharpsensors_test PUBLIC
    ${SMARTCAR_LIB}/sensors/distance/infrared/analog/sharp
)
configure_test(sharpsensors_test)

# DirectionlessOdometer test
add_executable(directionlessodometer_test
    ${SMARTCAR_LIB}/sensors/odometer/interrupt/DirectionlessOdometer.cpp
    ${UNIT_TESTS}/DirectionlessOdometer_test.cpp
)
target_include_directories(directionlessodometer_test PUBLIC
    ${SMARTCAR_LIB}/sensors/odometer/interrupt
)
configure_test(directionlessodometer_test)

# DirectionalOdometer test
add_executable(directionalodometer_test
    ${SMARTCAR_LIB}/sensors/odometer/interrupt/DirectionlessOdometer.cpp
    ${SMARTCAR_LIB}/sensors/odometer/interrupt/DirectionalOdometer.cpp
    ${UNIT_TESTS}/DirectionalOdometer_test.cpp
)
target_include_directories(directionalodometer_test PUBLIC
    ${SMARTCAR_LIB}/sensors/odometer/interrupt
)
configure_test(directionalodometer_test)

# GY50 (Gyroscope) test
add_executable(gy50_test
    ${SMARTCAR_LIB}/sensors/heading/gyroscope/GY50.cpp
    ${UNIT_TESTS}/GY50_test.cpp
)
target_include_directories(gy50_test PUBLIC
    ${SMARTCAR_LIB}/sensors/heading/gyroscope
)
configure_test(gy50_test)

# BrushlessMotor test
add_executable(brushedmotor_test
    ${SMARTCAR_LIB}/motor/analog/pwm/BrushedMotor.cpp
    ${UNIT_TESTS}/BrushedMotor_test.cpp
)
target_include_directories(brushedmotor_test PUBLIC
    ${SMARTCAR_LIB}/motor/analog/pwm/
)
configure_test(brushedmotor_test)

# ServoMotor test
add_executable(servomotor_test
    ${SMARTCAR_LIB}/motor/digital/servo/ServoMotor.cpp
    ${UNIT_TESTS}/ServoMotor_test.cpp
)
target_include_directories(servomotor_test PUBLIC
    ${SMARTCAR_LIB}/motor/digital/servo
)
configure_test(servomotor_test)

# AckermanControl test
add_executable(ackermancontrol_test
    ${SMARTCAR_LIB}/control/ackerman/AckermanControl.cpp
    ${UNIT_TESTS}/AckermanControl_test.cpp
)
target_include_directories(ackermancontrol_test PUBLIC
    ${SMARTCAR_LIB}/control/ackerman
    ${SMARTCAR_LIB}/motor
)
configure_test(ackermancontrol_test)

# DifferentialControl test
add_executable(differentialcontrol_test
    ${SMARTCAR_LIB}/control/differential/DifferentialControl.cpp
    ${UNIT_TESTS}/DifferentialControl_test.cpp
)
target_include_directories(differentialcontrol_test PUBLIC
    ${SMARTCAR_LIB}/control/differential
    ${SMARTCAR_LIB}/motor
)
configure_test(differentialcontrol_test)

# SimpleCar test
add_executable(simplecar_test
    ${SMARTCAR_LIB}/car/simple/SimpleCar.cpp
    ${UNIT_TESTS}/SimpleCar_test.cpp
)
target_include_directories(simplecar_test PUBLIC
    ${SMARTCAR_LIB}/car/simple
    ${SMARTCAR_LIB}/control
)
configure_test(simplecar_test)

# HeadingCar test
add_executable(headingcar_test
    ${SMARTCAR_LIB}/car/heading/HeadingCar.cpp
    ${SMARTCAR_LIB}/car/simple/SimpleCar.cpp
    ${UNIT_TESTS}/HeadingCar_test.cpp
)
target_include_directories(headingcar_test PUBLIC
    ${SMARTCAR_LIB}/car/heading
    ${SMARTCAR_LIB}/control
    ${SMARTCAR_LIB}/sensors/heading
)
configure_test(headingcar_test)

# DistanceCar test
add_executable(distancecar_test
    ${SMARTCAR_LIB}/car/distance/DistanceCar.cpp
    ${SMARTCAR_LIB}/car/simple/SimpleCar.cpp
    ${UNIT_TESTS}/DistanceCar_test.cpp
)
target_include_directories(distancecar_test PUBLIC
    ${SMARTCAR_LIB}/car/distance
    ${SMARTCAR_LIB}/control
    ${SMARTCAR_LIB}/sensors/odometer
)
configure_test(distancecar_test)

# SmartCar test
add_executable(smartcar_test
    ${SMARTCAR_LIB}/car/smart/SmartCar.cpp
    ${SMARTCAR_LIB}/car/heading/HeadingCar.cpp
    ${SMARTCAR_LIB}/car/distance/DistanceCar.cpp
    ${SMARTCAR_LIB}/car/simple/SimpleCar.cpp
    ${UNIT_TESTS}/SmartCar_test.cpp
)
target_include_directories(smartcar_test PUBLIC
    ${SMARTCAR_LIB}/car/smart/
    ${SMARTCAR_LIB}/control
    ${SMARTCAR_LIB}/sensors/odometer
    ${SMARTCAR_LIB}/sensors/heading
)
configure_test(smartcar_test)
