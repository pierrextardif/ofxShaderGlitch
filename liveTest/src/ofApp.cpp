#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	
#ifdef TARGET_OPENGLES
	autoShader.load("Shaders/LiveShader");
	cout << "Loading mobile shader" << endl;
#else
	autoShader.load("../../src/Shaders/LiveShaderDesktop");
	cout << "Loading desktop shader" << endl;	
#endif
	
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();

	image.loadImage("Textures/jjfGOPRO.png");
    resImg = ofVec2f(ofGetWidth(), ofGetHeight());

	font.loadFont(ofToDataPath( "Fonts/DIN.otf"), 8 );
    
    autoShader.setMillisBetweenFileCheck(200);
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// draw to the FBO
	ofEnableAlphaBlending();	
	fbo.begin();
	ofClear(0, 0, 0, 0);
		autoShader.begin();
		autoShader.setUniform1f("u_time", ofGetElapsedTimef() );
        autoShader.setUniform2f("u_resImg", resImg.x, resImg.y);
        gShader.addUniforms(&autoShader);
		
            image.draw(0,0, ofGetWidth(), ofGetHeight() );
		autoShader.end();
	fbo.end();
	ofDisableAlphaBlending();

	// draw the FBO to screen
	fbo.draw(0, 0);

	// draw the FPS
	sprintf(tempStr, "%4.1f", ofGetFrameRate() );
	ofVec2f pos( ofGetWidth()-30, ofGetHeight()-5 );
	ofSetColor(0);
	font.drawString( tempStr, pos.x + 1, pos.y + 1 );
	ofSetColor( 255 );
	font.drawString( tempStr, pos.x, pos.y );
    
    gShader.drawGUI();
}

void ofApp::keyPressed(int key) {
    if(key == 'g')gShader.guiON = !gShader.guiON;
}
