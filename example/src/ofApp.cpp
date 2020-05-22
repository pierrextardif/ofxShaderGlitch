#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    input = 0;
    image.loadImage("Textures/'David'_by_Michelangelo_Fir_JBU002.jpg");
    
	font.loadFont(ofToDataPath( "Fonts/DIN.otf"), 8 );
    
    // GlitchShader
    shaderGlitch.setup();
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    if(input == 1){
        vidGrabber.update();
        if(vidGrabber.isFrameNew()){
            t = vidGrabber.getTexture();
        }
    }
    
   
}

//--------------------------------------------------------------
void ofApp::draw()
{
    shaderGlitch.begin();
    
        if(input == 0)image.draw(0,0);
        if(input == 1)t.draw(0,0);
        
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
    if(key == 'i')switchInput();
}

void ofApp::switchInput(){
    input += 1;
    if( input > 1)input = 0;
    
    if(input == 1 && !vidGrabber.isInitialized()){
        vidGrabber.setDeviceID(0);
        vidGrabber.setDesiredFrameRate(60);
        vidGrabber.initGrabber(1920, 1080);
        
    }
    
    
}
