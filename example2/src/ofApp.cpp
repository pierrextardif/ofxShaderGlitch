#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
	//ofSetLogLevel(OF_LOG_VERBOSE);

	indexInput = 0;

	indexImage = 0;
	image.loadImage("Textures/TheDavidFace.png");
	//image.loadImage("Textures/david.jpg");
	//image.loadImage("Textures/jjfGOPRO.png");

	font.loadFont(ofToDataPath("Fonts/DIN.otf"), 8);

	//GlitchShader
	shaderGlitch.setup();
	shaderGlitch.setVisibleGui(bShowGui);

	//params = shaderGlitch.glitch.params;
	//gui.setup(params);

}

//--------------------------------------------------------------
void ofApp::update()
{
	if (indexInput == 1) {
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
	{
		//cout << ofGetUsingArbTex() << endl;

		//draw scene
		if (indexInput == 0) {
			ofRectangle r(0, 0, image.getWidth(), image.getHeight());
			r.scaleTo(ofGetWindowRect());
			image.draw(r.x, r.y, r.width, r.height);
		}
		else if (indexInput == 1) {
			//t.draw(0, 0);
			vidGrabber.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
	}
	shaderGlitch.end();

	//--

	//gui

	if (bShowGui) {
		//draw the FPS
		sprintf(tempStr, "%4.1f", ofGetFrameRate());
		ofVec2f pos(ofGetWidth() - 30, ofGetHeight() - 5);
		ofSetColor(0);
		if (font.isLoaded())font.drawString(tempStr, pos.x + 1, pos.y + 1);
		else ofDrawBitmapString(tempStr, pos.x + 1, pos.y + 1);

		ofSetColor(255);
		if (font.isLoaded())font.drawString(tempStr, pos.x, pos.y);
		else ofDrawBitmapString(tempStr, pos.x, pos.y);

		////shaderGlitch.drawGUI();//use addon internal gui panel
		//gui.draw();//use external ofApp gui panel
	}

	shaderGlitch.drawGUI();//use addon internal gui panel
}

void ofApp::keyPressed(int key) {
	//input camera or image
	if (key == 'i') switchInput();

	//gui
	if (key == OF_KEY_TAB) {
		bShowGui = !bShowGui;

		shaderGlitch.setVisibleGui(bShowGui);
		//shaderGlitch.setToggleVisibleGui();
	}
	
	//browse images
	if (key == OF_KEY_DOWN) {
		indexImage++;
		if (indexImage == 3) indexImage = 0;

		switch (indexImage)
		{
		case 0:
			image.loadImage("Textures/TheDavidFace.png");
			break;
		case 1:
			image.loadImage("Textures/david.jpg");
			break;
		case 2:
			image.loadImage("Textures/jjfGOPRO.png");
			break;
		}
	}
}

void ofApp::switchInput() {
	indexInput += 1;
	if (indexInput > 1)indexInput = 0;

	if (indexInput == 1 && !vidGrabber.isInitialized()) {
		int _d = 0;
		vidGrabber.setDeviceID(_d);
		//vidGrabber.setDesiredFrameRate(60);
		vidGrabber.initGrabber(1920, 1080);
	}
}
