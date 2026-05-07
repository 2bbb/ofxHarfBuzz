#pragma once
#include "hb.h"

class ofxHbFace;

class ofxHbFont {
public:
    ~ofxHbFont();
    void create(ofxHbFace& face, float sizePixels);
    hb_font_t* get() const;

private:
    hb_font_t* font_ = nullptr;
};
