#!/bin/bash
set -euo pipefail
# Requires an aarch64 host or cross-compilation toolchain.
# On GitHub Actions: use ubuntu-22.04 with runs-on: [self-hosted, arm64]
# or use a QEMU-based aarch64 runner.

HB_VERSION=10.4.0
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT_DIR="${SCRIPT_DIR}/../libs/harfbuzz"

if [ ! -d "/tmp/harfbuzz-${HB_VERSION}" ]; then
  curl -L "https://github.com/harfbuzz/harfbuzz/releases/download/${HB_VERSION}/harfbuzz-${HB_VERSION}.tar.xz" \
    -o /tmp/harfbuzz.tar.xz
  tar xf /tmp/harfbuzz.tar.xz -C /tmp
fi

cmake -B /tmp/hb-build-aarch64 -S /tmp/harfbuzz-${HB_VERSION} \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF \
  -DHB_HAVE_GLIB=OFF -DHB_HAVE_GOBJECT=OFF -DHB_HAVE_ICU=OFF \
  -DHB_HAVE_FREETYPE=OFF -DHB_BUILD_TESTS=OFF -DHB_BUILD_UTILS=OFF

cmake --build /tmp/hb-build-aarch64 -j$(nproc)

mkdir -p "${OUT_DIR}/lib/linuxaarch64"
cp /tmp/hb-build-aarch64/libharfbuzz.a "${OUT_DIR}/lib/linuxaarch64/"

echo "Done: ${OUT_DIR}/lib/linuxaarch64/libharfbuzz.a"
