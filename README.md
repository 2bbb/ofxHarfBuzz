# ofxHarfBuzz

openFrameworks addon wrapping [HarfBuzz](https://harfbuzz.github.io/) for Unicode text shaping.

## What it does

Takes a UTF-8 string + font + options and returns a list of glyph IDs with positions (advances and offsets). Handles ligatures, kerning, Arabic/Hebrew bidirectional text, CJK, and OpenType features correctly — things `ofTrueTypeFont` does not do.

## Classes

### `ofxHbFace`
RAII wrapper around `hb_face_t`. Loads font bytes from disk.
```cpp
ofxHbFace face;
face.load("fonts/NotoSansCJK.ttf");

hb_face_t*                   face.get();
const std::vector<uint8_t>&  face.getFontData();  // raw bytes (share with Skia)
```

### `ofxHbFont`
RAII wrapper around `hb_font_t`. Created from a face at a given pixel size.
```cpp
ofxHbFont font;
font.create(face, 48.0f);
```

### `ofxHbShaper`
Stateless shaper. Calls `hb_shape()` and converts the result to `ofxHbGlyphRun`.
```cpp
ofxHbShaper shaper;
ofxHbGlyphRun run = shaper.shape("こんにちは", font);
for (auto& g : run.glyphs) {
    // g.glyphId, g.advance, g.offset, g.cluster
}
```

## Types

```cpp
struct ofxHbGlyph {
    uint32_t  glyphId;   // glyph index in the font
    uint32_t  cluster;   // byte offset in source UTF-8
    glm::vec2 advance;   // pen advance (x, y) in pixels — 26.6 fixed converted to float
    glm::vec2 offset;    // glyph placement offset from pen position
};

struct ofxHbShapeOptions {
    std::string        language  = "und";                        // BCP 47 e.g. "ja", "ar"
    std::string        script    = "";                           // ISO 15924 e.g. "Latn", "Arab"
    ofxHbTextDirection direction = ofxHbTextDirection::LeftToRight;
    std::vector<std::string> features;                          // e.g. {"liga=1", "kern=1"}
};

struct ofxHbGlyphRun {
    std::vector<ofxHbGlyph> glyphs;
    ofxHbShapeOptions        options;
    float                    fontSize;
};
```

## Building the library

```bash
# macOS universal (arm64 + x86_64)
bash scripts/build_harfbuzz_macos.sh

# Linux x64
bash scripts/build_harfbuzz_linux.sh

# Windows — run in PowerShell
.\scripts\build_harfbuzz_windows.ps1
```

Requires CMake 3.15+. No ICU, GLib, or FreeType dependency.  
Output: `libs/harfbuzz/lib/{osx,linux64,vs}/` + `libs/harfbuzz/include/`  
`libs/` is git-ignored; run the script once per machine.

## CI

HarfBuzz is built from source on the first CI run and cached thereafter.

[![CI](https://github.com/2bbb/ofxHarfBuzz/actions/workflows/ci.yml/badge.svg)](https://github.com/2bbb/ofxHarfBuzz/actions/workflows/ci.yml)

Platforms: macOS (Make + Xcode), Linux x64, Windows x64

## Requirements

- openFrameworks 0.12.0+
- HarfBuzz 10.4.0

## License

MIT
