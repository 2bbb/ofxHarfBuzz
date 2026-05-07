#pragma once
#include <filesystem>
#include <vector>
#include "hb.h"

class ofxHbFace {
public:
    ~ofxHbFace();
    bool load(const std::filesystem::path& path);
    hb_face_t* get() const;
    const std::vector<uint8_t>& getFontData() const;

private:
    hb_face_t* face_ = nullptr;
    std::vector<uint8_t> fontData_;
};
