//
//  GlitchManager.cpp
//  CellTypes
//
//  Created by Pierre Tardif on 27/03/2020.
//

#include "GlitchManager.hpp"

void GlitchManager::setup(){
    
    initGui();
    
    shader.load("../../src/GlitchManager/Shaders/main");
    shader.setMillisBetweenFileCheck(200);
    
    cells.shader = &shader;
    gui.add(cells.cellGroup);
    
    resImg = ofVec2f(ofGetWidth(), ofGetHeight());

}

void GlitchManager::initGui(){
    
    guiON = false;
    gui.setup();
    
    typeTiling.setName("tylingType");
    typeTiling.add(type.set("type of tyling", 0, 0, 4));
    typeTiling.add(speedMoves.set("Speed", {1,1}, {0,0}, {10,10}));
    typeTiling.add(amntLinesColumns.set("amnt of Lines // Columns", {2,2}, {1,1}, {300,300}));
    
    gui.add(typeTiling);
}

void GlitchManager::begin(){
    
    shader.begin();
    shader.setUniform2f("u_resImg", resImg);
    shader.setUniform1i("u_tilingType", type);
    shader.setUniform2f("u_speedLinesColumns", speedMoves->x, speedMoves->y);
    shader.setUniform2f("u_amntLinesColumns", amntLinesColumns->x, amntLinesColumns->y);
    cells.addUniforms();
    
}
void GlitchManager::end(){
    shader.end();
}

void GlitchManager::setUniform1f(string name, float floatName){
    shader.setUniform1f(name, floatName);
}
