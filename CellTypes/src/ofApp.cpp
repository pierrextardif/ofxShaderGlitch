#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	
//#ifdef TARGET_OPENGLES
//	autoShader.load("Shaders/LiveShader");
//	cout << "Loading mobile shader" << endl;
//#else
//	autoShader.load("../../src/Shaders/LiveShaderDesktop");
//	cout << "Loading desktop shader" << endl;
//#endif
//
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();

	image.loadImage("Textures/IMG_0824.jpg");
//    resImg = ofVec2f(image.getWidth(), image.getHeight());

	font.loadFont(ofToDataPath( "Fonts/DIN.otf"), 8 );
    
    autoShader.setMillisBetweenFileCheck(200);
    
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    cam.update();
   
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// draw to the FBO
//	ofEnableAlphaBlending();
//	fbo.begin();
	ofClear(0, 0, 0, 0);
    glitch.begin();
    glitch.setUniform1f("u_time", ofGetElapsedTimef() );
        image.draw(0,0);
    glitch.end();
//	fbo.end();
//	ofDisableAlphaBlending();

	// draw the FBO to screen
//	fbo.draw(0, 0);

	// draw the FPS
	sprintf(tempStr, "%4.1f", ofGetFrameRate() );
	ofVec2f pos( ofGetWidth()-30, ofGetHeight()-5 );
	ofSetColor(0);
	font.drawString( tempStr, pos.x + 1, pos.y + 1 );
	ofSetColor( 255 );
	font.drawString( tempStr, pos.x, pos.y );
    
    glitch.drawGUI();
}

void ofApp::keyPressed(int key) {
    if(key == 'g')glitch.guiON = !glitch.guiON;
}
