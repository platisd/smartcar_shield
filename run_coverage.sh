#!/bin/bash
caller_dir=$PWD

script_dir=$(dirname "$0")
smartcar_test_dir="${script_dir}/test"
coverage_report_dir="${script_dir}/coverage-reports"

# Specify which directories are to be excluded from coverage report
coverage_ignore_dirs="\
    '/usr/*' \
    '*/test/*' \
    '*/external/*'\
"

# Run coverage
eval mkdir -p $coverage_report_dir
eval lcov --directory . --capture --output-file coverage.info
eval lcov --remove coverage.info $coverage_ignore_dirs --output-file $coverage_report_dir/cpp_coverage.info
eval lcov --list $coverage_report_dir/cpp_coverage.info

# Go back to the initial directory when you are done
cd $caller_dir
