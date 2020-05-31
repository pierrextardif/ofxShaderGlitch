#pragma once

#include "ofMain.h"

#include "ofxShaderGlitch.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
    
        void keyPressed(int key);
        
        
        void switchInput();
		ofImage image;
        ofVideoGrabber vidGrabber;
        ofTexture t;

        // fps infos
		ofTrueTypeFont 	font;
		char tempStr[512];
    
    
    int input;
    ofxShaderGlitch shaderGlitch;
    
    
    
};

