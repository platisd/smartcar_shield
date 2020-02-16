#!/bin/bash
caller_dir=$PWD
script_dir=$(dirname "$0")
cd $script_dir

echo "========= clang-tidy ========="
set -xu

fix_errors=""
arg=${1:-n}
if [ "$arg" = "--fix" ] || [ "$arg" = "-fix" ]; then
    fix_errors="-fix"
fi

eval run-clang-tidy-8 \
    -p=${script_dir}/test/build \
    -j$(nproc) \
    -header-filter=src/* \
    files src/* \
    $fix_errors \
    | grep "warning:"

warnings_not_found=$?

cd $caller_dir

[ $warnings_not_found -eq 0 ] && echo "Warnings found! Please fix them" && exit 1 || exit 0
