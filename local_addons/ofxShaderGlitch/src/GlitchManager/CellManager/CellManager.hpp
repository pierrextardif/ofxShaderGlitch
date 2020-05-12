//
//  CellManager.hpp
//  CellTypes
//
//  Created by Pierre Tardif on 27/03/2020.
//

#ifndef CellManager_hpp
#define CellManager_hpp

#include <stdio.h>
#include "ofxGui.h"
#include "ofxAutoReloadedShader.h"

class CellManager{
    public :
    CellManager(){
        setup();
    }
    
    void setup();
    void initGui();
    void update();
    
    ofParameterGroup cellGroup;
    ofParameter< ofColor > backgroundColor;
    ofParameter< ofVec2f > amountOfCells;
    ofParameter< ofVec2f > offset;
    ofParameter< int > layers;
    ofParameter < float > thresholdNoise;
    
    void addUniforms();
    ofxAutoReloadedShader* shader;
    
    
    
};

#endif /* CellManager_hpp */
