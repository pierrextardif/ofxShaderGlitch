//
//  GlitchShader.hpp
//  SegmentationJJF
//
//  Created by Pierre Tardif on 25/03/2020.
//

#ifndef GlitchShader_hpp
#define GlitchShader_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxGui.h"


#include "ofxAutoReloadedShader.h"

#define M_PI 3.141592653589793

class GlitchShader{
    public :
    

    ofShader shader;
    
    ofParameterGroup shaderControl;
    GlitchShader(){
        
        setup();
    }
    
    void setup();
    
    void drawGUI(){if(guiON)gui.draw();}
    void begin(ofxAutoReloadedShader* s, ofVec2f res, ofTexture texContour);
    void end();
    // ==== control ==== //
    
    bool guiON;
    ofxPanel gui;
    void initGui();

    
    
    void addUniforms(ofxAutoReloadedShader* s);
    
    ofParameter<float>amount;
    ofParameter<float>angle;
    ofParameter<float>seed;
    ofParameter<float>speed;
    ofParameter<float>seed_x;
    ofParameter<float>seed_y;
    ofParameter<float>distortion_x;
    ofParameter<float>distortion_y;
    ofParameter<float>col_s;
    
    
    ofParameter < int > iterationsGlitch;
    
    
};


#endif /* GlitchShader_hpp */
