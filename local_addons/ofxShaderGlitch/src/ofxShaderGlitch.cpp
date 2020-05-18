//
//  ofxShaderGlitch.cpp
//  Gradient
//
//  Created by Pierre Tardif on 10/05/2020.
//

#include "ofxShaderGlitch.h"


void ofxShaderGlitch::setup(){
    
    ofAddListener(ofEvents().keyPressed, this, &ofxShaderGlitch::keyPressed);
}

//--------------------------------------------------------------
void ofxShaderGlitch::begin(){
    glitch.begin();
    addUniforms();
}

//--------------------------------------------------------------
void ofxShaderGlitch::end(){
    glitch.end();
    drawGUI();
}

//--------------------------------------------------------------
void ofxShaderGlitch::addUniforms(){
 
//    glitch.setUniform1f("u_time", sin(ofGetElapsedTimef()) );

}

//--------------------------------------------------------------
void ofxShaderGlitch::drawGUI(){
    glitch.drawGUI();
}

//--------------------------------------------------------------
void ofxShaderGlitch::keyPressed(ofKeyEventArgs &keyArgs){
    if(keyArgs.key == 'g')glitch.guiON = !glitch.guiON;
    if(keyArgs.key == 'c'){
        glitch.nonMaxAndContinuity = !glitch.nonMaxAndContinuity;
        cout << " maxContinuity = " << ofToString(glitch.nonMaxAndContinuity) << endl;
    }
}
