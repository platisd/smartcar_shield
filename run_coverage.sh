#!/bin/bash
caller_dir=$PWD

smartcar_test_dir="${TRAVIS_BUILD_DIR}/test"
coverage_report_dir="${TRAVIS_BUILD_DIR}/coverage-reports"

# Specify which directories are to be excluded from coverage report
coverage_ignore_dirs="\
    '/usr/*' \
"

# Run coverage for ins-node and ins-server
eval mkdir -p $coverage_report_dir
eval lcov --directory . --capture --output-file coverage.info
eval lcov --remove coverage.info $coverage_ignore_dirs --output-file $coverage_report_dir/cpp_coverage.info
eval lcov --list coverage.info

# Go back to the initial directory when you are done
cd $caller_dir
