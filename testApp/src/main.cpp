#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"

int main() {
    auto window = std::make_shared<ofAppNoWindow>();
    ofSetupOpenGL(window, 0, 0, OF_WINDOW);
    ofRunApp(std::make_shared<ofApp>());
}
