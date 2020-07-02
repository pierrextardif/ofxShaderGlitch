#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);

	input = 0;
	image.loadImage("Textures/david.jpg");

	font.loadFont(ofToDataPath("Fonts/DIN.otf"), 8);

	//GlitchShader
	shaderGlitch.setup();

	//params = shaderGlitch.glitch.params;
	//gui.setup(params);

}

//--------------------------------------------------------------
void ofApp::update()
{
	if (input == 1) {
		vidGrabber.update();

		//if (vidGrabber.isFrameNew()) {
		//	t = vidGrabber.getTexture();
		//}
	}
}

//--------------------------------------------------------------
void ofApp::exit()
{
	shaderGlitch.exit();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	shaderGlitch.begin();

	//cout << ofGetUsingArbTex() << endl;

	if (input == 0){
		image.draw(0, 0);
	}
	else if (input == 1){
		//t.draw(0, 0);
		vidGrabber.draw(0, 0, ofGetWidth(), ofGetHeight());
	}

	shaderGlitch.end();

	//--

	if (bShowGui) {
		//draw the FPS
		sprintf(tempStr, "%4.1f", ofGetFrameRate());
		ofVec2f pos(ofGetWidth() - 30, ofGetHeight() - 5);
		ofSetColor(0);
		font.drawString(tempStr, pos.x + 1, pos.y + 1);
		ofSetColor(255);
		font.drawString(tempStr, pos.x, pos.y);

		////shaderGlitch.drawGUI();//use addon internal gui panel
		//gui.draw();//use external ofApp gui panel
	}

	shaderGlitch.drawGUI();//use addon internal gui panel
}

void ofApp::keyPressed(int key) {
	if (key == 'i') switchInput();
	if (key == OF_KEY_TAB) {
		bShowGui = !bShowGui;

		shaderGlitch.setVisibleGui(bShowGui);
		//shaderGlitch.setToggleVisibleGui();
	}
}

void ofApp::switchInput() {
	input += 1;
	if (input > 1)input = 0;

	if (input == 1 && !vidGrabber.isInitialized()) {
		int _d = 0;
		vidGrabber.setDeviceID(_d);
		//vidGrabber.setDesiredFrameRate(60);
		vidGrabber.initGrabber(1920, 1080);
	}
}
