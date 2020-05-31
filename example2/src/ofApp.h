#pragma once

#include "ofMain.h"

#include "ofxShaderGlitch.h"
#include "ofxPresetsManager.h"

class ofApp : public ofBaseApp {

public:

	//presetsManager
	ofxPresetsManager presetsManager;
	void setupPresetsManager();

	void setup();
	void update();
	void draw();
	void exit();

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

