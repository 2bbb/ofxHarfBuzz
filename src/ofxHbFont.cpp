#include "ofxHbFont.h"
#include "ofxHbFace.h"

ofxHbFont::~ofxHbFont() {
    if(font_) hb_font_destroy(font_);
}

void ofxHbFont::create(ofxHbFace& face, float sizePixels) {
    if(font_) hb_font_destroy(font_);
    font_ = hb_font_create(face.get());

    unsigned int upem = hb_face_get_upem(face.get());
    // ppem でサイズを設定 (HarfBuzz はスケールを使ってポジションを返す)
    hb_font_set_ppem(font_, static_cast<unsigned int>(sizePixels),
                             static_cast<unsigned int>(sizePixels));
    // scale: 26.6 fixed-point → 1/64 pixel units
    // upem → sizePixels のスケール (64倍で26.6 fixed)
    int scale = static_cast<int>(sizePixels * 64);
    hb_font_set_scale(font_, scale, scale);
}

hb_font_t* ofxHbFont::get() const {
    return font_;
}
