meta:
	ADDON_NAME = ofxHarfBuzz
	ADDON_DESCRIPTION = HarfBuzz text shaping wrapper for openFrameworks
	ADDON_AUTHOR = 2bbb
	ADDON_TAGS = "text" "shaping" "typography"
	ADDON_URL = http://github.com/2bbb/ofxHarfBuzz

common:
	ADDON_INCLUDES = src
	ADDON_INCLUDES += libs/harfbuzz/include
	# ofxHarfBuzz.cpp is the unity-build entry for Xcode; exclude individual TUs from make
	ADDON_SOURCES_EXCLUDE = src/ofxHbFace.cpp
	ADDON_SOURCES_EXCLUDE += src/ofxHbFont.cpp
	ADDON_SOURCES_EXCLUDE += src/ofxHbShaper.cpp

osx:
	ADDON_LIBS = libs/harfbuzz/lib/osx/libharfbuzz.a

linux64:
	ADDON_LIBS = libs/harfbuzz/lib/linux64/libharfbuzz.a

linuxaarch64:
	ADDON_LIBS = libs/harfbuzz/lib/linuxaarch64/libharfbuzz.a

vs:
	ADDON_LIBS = libs/harfbuzz/lib/vs/harfbuzz.lib

msys2:

emscripten:

android:
