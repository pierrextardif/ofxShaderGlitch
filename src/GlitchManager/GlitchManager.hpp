//
//  GlitchManager.hpp
//  CellTypes
//
//  Created by Pierre Tardif on 27/03/2020.
//

#ifndef GlitchManager_hpp
#define GlitchManager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxGui.h"

// for faster live coding
#include "ofxAutoReloadedShader.h"

// Cells
#include "CellManager/CellManager.hpp"

// Feedback
#include "FeedbackManager/FeedbackManager.hpp"

class GlitchManager{
    
    public :
    GlitchManager(){
        setup();
    }
    
    void setup();
    void setup(glm::vec2 sizeImg);
    void begin();
    void end();
    
    bool guiON;
    ofParameterGroup typeTiling, typeEffect;
    ofParameter < int > typeT, typeE;
    ofParameter < ofVec2f > amntLinesColumns;
    ofParameter < ofVec2f > speedMoves;
    ofParameter < ofVec4f > props;
    ofParameter < float > alphaGradiant;
    ofParameter < ofColor > gradiantColor;
    ofParameter < ofColor > backgroundColor;
    ofParameter < bool > continuous;
    ofParameter < float > thresholdNoise;
    
    void initGui();
    void drawGUI(){if(guiON)gui.draw();}
    
    
    ofxPanel gui;
    
    glm::vec2 resImg;
    ofxAutoReloadedShader shader, gaussian;
    CellManager cells;
    FeedbackManager feedbackEdge;
    
    ofFbo f;
    bool nonMaxAndContinuity;
    
    ofParameter < bool > bEnable;
	ofParameterGroup params;

};

#endif /* GlitchManager_hpp */
