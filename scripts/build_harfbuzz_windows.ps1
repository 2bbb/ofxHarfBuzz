# Build harfbuzz.lib for Windows (MSVC x64)
# Requires: CMake 3.15+, Visual Studio 2019+
param()
Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$ProgressPreference = 'SilentlyContinue'

# ── Tool resolver ─────────────────────────────────────────────────────────────
# Find a tool in PATH; if missing, check known install locations and add to PATH.

function Resolve-Tool {
    param(
        [string]$Name,
        [string[]]$KnownPaths,
        [string]$InstallHint
    )
    if (Get-Command $Name -ErrorAction SilentlyContinue) { return }

    foreach ($dir in $KnownPaths) {
        if (Test-Path (Join-Path $dir "$Name.exe")) {
            Write-Host "==> $Name not in PATH, using: $dir"
            $env:Path = "$dir;$env:Path"
            return
        }
    }

    Write-Error @"
ERROR: '$Name' not found in PATH or known install locations.
$InstallHint
After installing, restart PowerShell (or open a new terminal) and re-run.
"@
    exit 1
}

# ── Prerequisite checks ───────────────────────────────────────────────────────

# cmake: winget installs to Program Files\CMake\bin; VS bundles its own copy too
Resolve-Tool cmake `
    -KnownPaths @(
        "C:\Program Files\CMake\bin",
        "C:\Program Files (x86)\CMake\bin"
    ) `
    -InstallHint "Install cmake with:  winget install Kitware.CMake"

# ── Build ─────────────────────────────────────────────────────────────────────

$HB_VERSION = "10.4.0"
$SCRIPT_DIR = Split-Path -Parent $MyInvocation.MyCommand.Path
$OUT_DIR = Join-Path $SCRIPT_DIR "..\libs\harfbuzz"
$TMP = $env:TEMP

$srcDir = "$TMP\harfbuzz-$HB_VERSION"
if (-not (Test-Path $srcDir)) {
    $archive = "$TMP\harfbuzz.zip"
    Write-Host "==> Downloading HarfBuzz $HB_VERSION..."
    curl.exe -sL "https://github.com/harfbuzz/harfbuzz/archive/refs/tags/$HB_VERSION.zip" -o $archive
    Expand-Archive -Path $archive -DestinationPath $TMP -Force
    Remove-Item $archive
} else {
    Write-Host "==> HarfBuzz source already exists, skipping download."
}

$buildDir = "$TMP\hb-build-vs"
Write-Host "==> Configuring..."
cmake -B $buildDir -S $srcDir `
    -A x64 `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL `
    -DBUILD_SHARED_LIBS=OFF `
    -DHB_HAVE_GLIB=OFF -DHB_HAVE_GOBJECT=OFF -DHB_HAVE_ICU=OFF `
    -DHB_HAVE_FREETYPE=OFF -DHB_BUILD_TESTS=OFF -DHB_BUILD_UTILS=OFF

Write-Host "==> Building..."
cmake --build $buildDir --config Release -- /maxCpuCount

$libDst = Join-Path $OUT_DIR "lib\vs"
New-Item -ItemType Directory -Force -Path $libDst | Out-Null
Copy-Item "$buildDir\Release\harfbuzz.lib" "$libDst\"

$incDst = Join-Path $OUT_DIR "include"
New-Item -ItemType Directory -Force -Path $incDst | Out-Null
Get-ChildItem "$srcDir\src\hb*.h" | Copy-Item -Destination $incDst

Write-Host "Done: $libDst\harfbuzz.lib"
