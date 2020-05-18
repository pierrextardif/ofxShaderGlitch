#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);

	image.loadImage("Textures/'David'_by_Michelangelo_Fir_JBU002.jpg");
//    image.loadImage("Textures/IMG_0824.jpg");

	font.loadFont(ofToDataPath( "Fonts/DIN.otf"), 8 );
    
    // GlitchShader
    shaderGlitch.setup();
    
}

//--------------------------------------------------------------
void ofApp::update()
{
   
}

//--------------------------------------------------------------
void ofApp::draw()
{
    shaderGlitch.begin();
        image.draw(0,0);
    shaderGlitch.end();

	// draw the FPS
	sprintf(tempStr, "%4.1f", ofGetFrameRate() );
	ofVec2f pos( ofGetWidth()-30, ofGetHeight()-5 );
	ofSetColor(0);
	font.drawString( tempStr, pos.x + 1, pos.y + 1 );
	ofSetColor( 255 );
	font.drawString( tempStr, pos.x, pos.y );
    
}

void ofApp::keyPressed(int key) {
//    if(key == 'g')glitch.guiON = !glitch.guiON;
}
