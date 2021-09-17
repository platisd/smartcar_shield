# Inspired by Lars Bilke's https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake
include(CMakeParseArguments)

# Check prereqs
find_program(GCOV_EXECUTABLE gcov REQUIRED)
find_program(LCOV_EXECUTABLE lcov REQUIRED)
find_program(GENHTML_EXECUTABLE genhtml REQUIRED)

if(CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3)
        message(FATAL_ERROR "Clang version must be 3.0.0 or greater")
    endif()
elseif(MSVC)
    message(FATAL_ERROR "Unsupported C++ compiler")
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Code coverage results with an optimised (non-Debug) build may be misleading")
endif()

# Enables coverage information generation on a target
function (target_enable_coverage target scope)
    target_compile_options(${target} ${scope} -g -O0 -fprofile-arcs -ftest-coverage)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(${target} ${scope} gcov)
    else()
        target_link_options(${target} ${scope} --coverage)
    endif()
endfunction()

# Creates a target ${target}_coverage to
function(setup_coverage_target executable)
    set(coverageTarget "${executable}_coverage")

    set(exclusionArgs)
    foreach (exclude IN LISTS COVERAGE_EXCLUDES)
        list(APPEND exclusionArgs --exclude "${exclude}")
    endforeach ()

    # Setup target
    add_custom_target(${coverageTarget}
        # Reset counters
        COMMAND ${LCOV_EXECUTABLE} --zerocounters --directory . --quiet
        # Create baseline to make sure untouched files show up in the report
        COMMAND ${LCOV_EXECUTABLE} --capture --directory .
            --output-file ${coverageTarget}.base --initial
            ${exclusion_args}

        # Invoke test executable
        COMMAND "$<TARGET_FILE:${executable}>"

        # Capture lcov counters
        COMMAND ${LCOV_EXECUTABLE} --capture --directory .
            --output-file ${coverageTarget}.info ${exclusionArgs}
        # Add baseline counters
        COMMAND ${LCOV_EXECUTABLE} --add-tracefile ${coverageTarget}.base
            --add-tracefile ${coverageTarget}.info
            --output-file ${coverageTarget}.total ${exclusionArgs}
        # Generate report
        COMMAND ${GENHTML_EXECUTABLE} --output-directory ${coverageTarget}
            ${PROJECT_BINARY_DIR}/${coverageTarget}.total
        # Cleanup lcov outputs
        COMMAND ${CMAKE_COMMAND} -E remove ${coverageTarget}.base
            ${coverageTarget}.info ${coverageTarget}.total

        # Notify user of output location
        COMMAND ${CMAKE_COMMAND} -E echo "Open ./${coverageTarget}/index.html in your browser to view the coverage report."
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${executable}
        COMMENT "Processing code coverage counters and generating report."
    )
endfunction()
