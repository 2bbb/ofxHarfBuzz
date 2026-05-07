#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"

enum class ofxHbTextDirection {
    LeftToRight,
    RightToLeft,
    TopToBottom,
    BottomToTop
};

struct ofxHbGlyph {
    uint32_t  glyphId = 0;
    uint32_t  cluster = 0;
    glm::vec2 advance = {0, 0};  // HarfBuzz 26.6 fixed-point → float 変換済み
    glm::vec2 offset  = {0, 0};
};

struct ofxHbShapeOptions {
    std::string        language  = "und";
    std::string        script    = "";
    ofxHbTextDirection direction = ofxHbTextDirection::LeftToRight;
    std::vector<std::string> features;
};

struct ofxHbGlyphRun {
    std::vector<ofxHbGlyph> glyphs;
    ofxHbShapeOptions        options;
    float                    fontSize = 12.0f;
};
