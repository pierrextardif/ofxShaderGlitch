#pragma once

#include "ofMain.h"

#include "ofxShaderGlitch.h"

//#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);

	//source
	void switchInput();
	ofImage image;
	ofVideoGrabber vidGrabber;
	ofTexture t;

	//fps infos
	ofTrueTypeFont 	font;
	char tempStr[512];

	//--

	ofxShaderGlitch shaderGlitch;
	int input;
	bool bShowGui = false;

	//ofxPanel gui;
	//ofParameterGroup params;
};

