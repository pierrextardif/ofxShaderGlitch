#pragma once

#include "ofMain.h"

#include "ofxShaderGlitch.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
    
    void keyPressed(int key);

		ofImage image;

		ofTrueTypeFont 	font;

		char tempStr[512];
    
    ofxShaderGlitch shaderGlitch;
    
    
    
};

