//
//  GlitchShader.cpp
//  SegmentationJJF
//
//  Created by Pierre Tardif on 25/03/2020.
//

#include "GlitchShader.hpp"

void GlitchShader::setup(){
    
    initGui();
    
//    shader.load("../../src/JJFTracker/GlitchShader/shader/shader.vert",
//                "../../src/JJFTracker/GlitchShader/shader/shader.frag");
}

void GlitchShader::initGui(){
    
    guiON = false;
    gui.setup();
    shaderControl.setName("glitch");
    
    shaderControl.add(amount.set("amount", 0, 0, 10));
    shaderControl.add(angle.set("angle", 1, 0,M_PI));
    shaderControl.add(seed.set("seed", 0, -0.2,0.2));
    shaderControl.add(iterationsGlitch.set("iterationsGlitch", 1,1, 12));
    
    gui.add(shaderControl);
}

void GlitchShader::addUniforms(ofxAutoReloadedShader* s){
    
    s->setUniform1f("amount", amount);
    s->setUniform1f("angle", angle);
    s->setUniform1f("seed", seed);
    s->setUniform1i("iterationsGlitch", iterationsGlitch);
}
