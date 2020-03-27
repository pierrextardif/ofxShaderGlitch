#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "GlitchManager/GlitchManager.hpp"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
    
    void keyPressed(int key);
		
		
        ofVideoGrabber cam;    
		ofxAutoReloadedShader autoShader;
		ofFbo fbo;

		ofImage image;

		ofTrueTypeFont 	font;

		char tempStr[512];
    
        GlitchManager glitch;
};

