#!/bin/sh

set -ex

if [ -n "$1" ]; then
    build_dir="$1"
else
    build_dir="./build"
fi
build_src_dir="$build_dir/CMakeFiles/ass2srt_test.dir/src"

old_cwd=$(pwd)
cd "$build_src_dir"
find . -type f -name '*.gcno' | xargs -n1 gcov
cd "$old_cwd"

lcov --capture --directory "$build_src_dir" --output-file "$build_dir/gtest_coverage.info" --include '**/src/**'
genhtml "$build_dir/gtest_coverage.info" --output-directory "$build_dir/coverage_html"

index_path=$(realpath "$build_dir/coverage_html/index.html")
echo "Report generated at: $index_path"
