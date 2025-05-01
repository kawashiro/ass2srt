#!/bin/sh

set -e

MXE_PATH="$1"
BUILD_DIR="/tmp/ass2srt_win_build"
CMAKE_BIN="$MXE_PATH/usr/x86_64-pc-linux-gnu/bin/cmake"
STRIP_BIN="$MXE_PATH/usr/bin/i686-w64-mingw32.static-strip"

if [ -z "$MXE_PATH" ] || [ -z "$2" ]; then
    echo "usage: ./build-win-dist.sh <MXE_PATH> <OUT_FILE.zip>"
    exit 2
fi
OUT_FILE=$(realpath "$2")

"$CMAKE_BIN" -S . -B "$BUILD_DIR" -DASS2SRT_BUILD_UI=true -DCMAKE_BUILD_TYPE=Release -DMXE_PATH="$MXE_PATH" -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-mingw64.cmake
"$CMAKE_BIN" --build "$BUILD_DIR"

"$STRIP_BIN" "$BUILD_DIR/ass2srt.exe"
"$STRIP_BIN" "$BUILD_DIR/ass2srt-gui.exe"

cp README.md "$BUILD_DIR/README.md"
cp LICENSE "$BUILD_DIR/LICENSE"

rm -f "$OUT_FILE"
cd "$BUILD_DIR"
zip -9 "$OUT_FILE" ass2srt.exe ass2srt-gui.exe README.md LICENSE

cd ..
rm -r "$BUILD_DIR"
