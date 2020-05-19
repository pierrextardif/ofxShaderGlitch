//
//  GlitchManager.cpp
//  CellTypes
//
//  Created by Pierre Tardif on 27/03/2020.
//

#include "GlitchManager.hpp"

void GlitchManager::setup(){
    
    initGui();
    
    shader.load("../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/main");
    shader.setMillisBetweenFileCheck(200);
    
    cells.shader = &shader;
    
    gui.add(cells.cellGroup);
    
    
    resImg = ofVec2f(ofGetWidth(), ofGetHeight());
    
    // edge detection : gaussian
    f.allocate(resImg.x, resImg.y);
    gaussian.load("../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/Gaussian/gaussian");
    
    //edge detection non maximum / continuity
    nonMaxAndContinuity = false;

}

//--------------------------------------------------------------
void GlitchManager::initGui(){
    
    guiON = false;
    gui.setup();
    
    typeTiling.setName("tylingType");
    
    typeTiling.add(type.set("type of tyling", 0, 0, 9));
    typeTiling.add(speedMoves.set("Speed", {1,1}, {-10, -10}, {10,10}));
    typeTiling.add(amntLinesColumns.set("amnt of Lines // Columns", {2,2}, {1,1}, {300,300}));
    gui.add(props.set("testGradSize", ofVec4f(0.25, 0.25, 0.75, 0.75), ofVec4f(0,0,0,0), ofVec4f(1,1,1,1)));
    gui.add(alphaGradiant.set("alphaGradiant", 0.8, 0, 1));
    gui.add(gradiantColor.set("gradiantColor", ofColor(200, 255, 0)));
    gui.add(continuousMosh.set("continuous Mosh", false));
    
    gui.add(typeTiling);
}

//--------------------------------------------------------------
void GlitchManager::begin(){
    
    
    f.begin();
    ofClear(0);
    gaussian.begin();
    
}

//--------------------------------------------------------------
void GlitchManager::end(){
    gaussian.end();
    f.end();
    
    
    shader.begin();
    shader.setUniform2f("u_resImg", resImg);
    shader.setUniform1i("u_tilingType", type);
    shader.setUniform2f("u_speedLinesColumns", speedMoves->x, speedMoves->y);
    shader.setUniform2f("u_amntLinesColumns", amntLinesColumns->x, amntLinesColumns->y);
    shader.setUniform2f("u_gradStart", props->x, props->y);
    shader.setUniform2f("u_gradFinish", props->z, props->w);
    shader.setUniform1f("u_alphaGradiant", alphaGradiant);
    shader.setUniform4f("u_gradiantColor", gradiantColor->r / 255., gradiantColor->g  / 255., gradiantColor->b  / 255., gradiantColor->a / 255.);
    shader.setUniform1f("u_continuousMosh", (continuousMosh == true) ? 1.:0.);
    shader.setUniform1f("u_maxContinuity", (nonMaxAndContinuity == true) ? 1.:0.);
    shader.setUniform1f("u_time", ofGetElapsedTimef());
    cells.addUniforms();
    
    f.draw(0,0);
    shader.end();
}

//--------------------------------------------------------------
void GlitchManager::setUniform1f(string name, float floatName){
    shader.setUniform1f(name, floatName);
}
