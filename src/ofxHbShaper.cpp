#include "ofxHbShaper.h"
#include "ofxHbFont.h"
#include "hb.h"

static hb_direction_t toHbDirection(ofxHbTextDirection d) {
    switch(d) {
        case ofxHbTextDirection::LeftToRight:  return HB_DIRECTION_LTR;
        case ofxHbTextDirection::RightToLeft:  return HB_DIRECTION_RTL;
        case ofxHbTextDirection::TopToBottom:  return HB_DIRECTION_TTB;
        case ofxHbTextDirection::BottomToTop:  return HB_DIRECTION_BTT;
    }
    return HB_DIRECTION_LTR;
}

ofxHbGlyphRun ofxHbShaper::shape(const std::string& utf8,
                                  ofxHbFont& font,
                                  const ofxHbShapeOptions& opts) {
    hb_buffer_t* buf = hb_buffer_create();

    hb_buffer_add_utf8(buf, utf8.c_str(), -1, 0, -1);
    hb_buffer_set_direction(buf, toHbDirection(opts.direction));

    if(!opts.script.empty()) {
        hb_buffer_set_script(buf,
            hb_script_from_string(opts.script.c_str(), -1));
    }
    if(!opts.language.empty()) {
        hb_buffer_set_language(buf,
            hb_language_from_string(opts.language.c_str(), -1));
    }

    // OpenType features
    std::vector<hb_feature_t> features;
    for(const auto& f : opts.features) {
        hb_feature_t feat;
        if(hb_feature_from_string(f.c_str(), -1, &feat)) {
            features.push_back(feat);
        }
    }

    hb_shape(font.get(), buf, features.data(), static_cast<unsigned int>(features.size()));

    unsigned int count = 0;
    hb_glyph_info_t*     infos     = hb_buffer_get_glyph_infos(buf, &count);
    hb_glyph_position_t* positions = hb_buffer_get_glyph_positions(buf, &count);

    ofxHbGlyphRun run;
    run.options = opts;
    run.glyphs.resize(count);

    constexpr float kFixed = 1.0f / 64.0f;  // 26.6 fixed-point → float
    for(unsigned int i = 0; i < count; ++i) {
        run.glyphs[i].glyphId = infos[i].codepoint;
        run.glyphs[i].cluster = infos[i].cluster;
        run.glyphs[i].advance = {positions[i].x_advance * kFixed,
                                  positions[i].y_advance * kFixed};
        run.glyphs[i].offset  = {positions[i].x_offset  * kFixed,
                                  positions[i].y_offset  * kFixed};
    }

    hb_buffer_destroy(buf);
    return run;
}
