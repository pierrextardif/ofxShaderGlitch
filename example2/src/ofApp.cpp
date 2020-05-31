#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setupPresetsManager()
{
	//optional folders. sames as defaults
	presetsManager.setPath_GlobalFolder("ofxPresetsManager");//main container folder
	presetsManager.setPath_KitFolder("presets");//from kit/livePanel presets
	presetsManager.setPath_PresetsFolder("archive");//for browser file presets

	//-

	ofParameterGroup g{ "GLITCH PARAMS" };
	g.add(shaderGlitch.glitch.typeEffect);
	g.add(shaderGlitch.glitch.typeTiling);

	//added ofParametegrGroup to presetsManager
	//define desired trigged keys
	presetsManager.add(g, { '1', '2', '3', '4', '5', '6', '7', '8' });
	presetsManager.setup();

	//customize layout
	presetsManager.setVisible_PresetClicker(true);
	int pad = 20;
	presetsManager.setPosition_PresetClicker(pad, ofGetHeight() - 50 - pad, 50);

	//final startup refresh
	presetsManager.refresh();

}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);


	input = 0;
	//image.loadImage("Textures/'David'_by_Michelangelo_Fir_JBU002.jpg");
	image.loadImage("Textures/david.jpg");

	font.loadFont(ofToDataPath("Fonts/DIN.otf"), 8);

	// GlitchShader
	shaderGlitch.setup();

	setupPresetsManager();
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (input == 1) {
		vidGrabber.update();
		if (vidGrabber.isFrameNew()) {
			t = vidGrabber.getTexture();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	shaderGlitch.begin();

	if (input == 0)image.draw(0, 0);
	if (input == 1)t.draw(0, 0);

	shaderGlitch.end();


	// draw the FPS
	sprintf(tempStr, "%4.1f", ofGetFrameRate());
	ofVec2f pos(ofGetWidth() - 30, ofGetHeight() - 5);
	ofSetColor(0);
	font.drawString(tempStr, pos.x + 1, pos.y + 1);
	ofSetColor(255);
	font.drawString(tempStr, pos.x, pos.y);

}

void ofApp::keyPressed(int key) {
	if (key == 'i')switchInput();
}

void ofApp::switchInput() {
	input += 1;
	if (input > 1)input = 0;

	if (input == 1 && !vidGrabber.isInitialized()) {
		vidGrabber.setDeviceID(0);
		vidGrabber.setDesiredFrameRate(60);
		vidGrabber.initGrabber(1920, 1080);

	}


}
