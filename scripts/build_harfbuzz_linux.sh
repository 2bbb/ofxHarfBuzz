#!/bin/bash
set -euo pipefail

HB_VERSION=10.4.0
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT_DIR="${SCRIPT_DIR}/../libs/harfbuzz"

if [ ! -d "/tmp/harfbuzz-${HB_VERSION}" ]; then
  curl -L "https://github.com/harfbuzz/harfbuzz/releases/download/${HB_VERSION}/harfbuzz-${HB_VERSION}.tar.xz" \
    -o /tmp/harfbuzz.tar.xz
  tar xf /tmp/harfbuzz.tar.xz -C /tmp
fi

cmake -B /tmp/hb-build-linux64 -S /tmp/harfbuzz-${HB_VERSION} \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF \
  -DHB_HAVE_GLIB=OFF -DHB_HAVE_GOBJECT=OFF -DHB_HAVE_ICU=OFF \
  -DHB_HAVE_FREETYPE=OFF -DHB_BUILD_TESTS=OFF -DHB_BUILD_UTILS=OFF

cmake --build /tmp/hb-build-linux64 -j$(nproc)

mkdir -p "${OUT_DIR}/lib/linux64"
cp /tmp/hb-build-linux64/libharfbuzz.a "${OUT_DIR}/lib/linux64/"

# headers (already present from macOS build, but copy if missing)
mkdir -p "${OUT_DIR}/include"
find /tmp/harfbuzz-${HB_VERSION}/src -maxdepth 1 -name "hb*.h" \
  -exec cp {} "${OUT_DIR}/include/" \;

echo "Done: ${OUT_DIR}/lib/linux64/libharfbuzz.a"
lipo -info "${OUT_DIR}/lib/linux64/libharfbuzz.a" 2>/dev/null || file "${OUT_DIR}/lib/linux64/libharfbuzz.a"
