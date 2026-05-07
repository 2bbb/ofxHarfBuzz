#include "ofApp.h"

static std::string findFont() {
    // macOS
    if (std::filesystem::exists("/System/Library/Fonts/Helvetica.ttc"))
        return "/System/Library/Fonts/Helvetica.ttc";
    // Ubuntu CI (fonts-liberation)
    if (std::filesystem::exists("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
        return "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf";
    // Windows
    if (std::filesystem::exists("C:/Windows/Fonts/arial.ttf"))
        return "C:/Windows/Fonts/arial.ttf";
    return "";
}

void ofApp::setup() {
    auto path = findFont();
    if (path.empty()) {
        ofLogError("testApp") << "No system font found";
        ofExit(1);
    }

    ofxHbFace face;
    if (!face.load(path)) {
        ofLogError("testApp") << "Failed to load font: " << path;
        ofExit(1);
    }

    ofxHbFont font;
    font.create(face, 24.0f);

    ofxHbShaper shaper;

    // ASCII shaping
    auto run = shaper.shape("Hello, World!", font);
    if (run.glyphs.empty()) {
        ofLogError("testApp") << "No glyphs for ASCII text";
        ofExit(1);
    }
    ofLogNotice("testApp") << "ASCII: " << run.glyphs.size() << " glyphs, first id=" << run.glyphs[0].glyphId;

    // Direction/script options
    ofxHbShapeOptions opts;
    opts.direction = ofxHbTextDirection::LeftToRight;
    auto run2 = shaper.shape("fi fl ffi", font, opts);
    if (run2.glyphs.empty()) {
        ofLogError("testApp") << "No glyphs for ligature text";
        ofExit(1);
    }
    ofLogNotice("testApp") << "Ligature: " << run2.glyphs.size() << " glyphs";

    ofLogNotice("testApp") << "All tests passed.";
    ofExit(0);
}
