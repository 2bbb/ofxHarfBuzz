#include "ofxHbFace.h"
#include <fstream>
#include "ofLog.h"

ofxHbFace::~ofxHbFace() {
    if(face_) hb_face_destroy(face_);
}

bool ofxHbFace::load(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if(!file) {
        ofLogError("ofxHbFace::load") << "cannot open: " << path;
        return false;
    }
    auto size = file.tellg();
    fontData_.resize(size);
    file.seekg(0);
    file.read(reinterpret_cast<char*>(fontData_.data()), size);

    hb_blob_t* blob = hb_blob_create(
        reinterpret_cast<const char*>(fontData_.data()),
        static_cast<unsigned int>(fontData_.size()),
        HB_MEMORY_MODE_READONLY,
        nullptr, nullptr
    );
    face_ = hb_face_create(blob, 0);
    hb_blob_destroy(blob);

    if(hb_face_get_upem(face_) == 0) {
        ofLogError("ofxHbFace::load") << "invalid font face: " << path;
        hb_face_destroy(face_);
        face_ = nullptr;
        fontData_.clear();
        return false;
    }
    return true;
}

hb_face_t* ofxHbFace::get() const {
    return face_;
}

const std::vector<uint8_t>& ofxHbFace::getFontData() const {
    return fontData_;
}
