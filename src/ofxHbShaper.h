#pragma once
#include <string>
#include "ofxHbTypes.h"

class ofxHbFont;

class ofxHbShaper {
public:
    ofxHbGlyphRun shape(const std::string& utf8,
                        ofxHbFont& font,
                        const ofxHbShapeOptions& opts = {});
};
