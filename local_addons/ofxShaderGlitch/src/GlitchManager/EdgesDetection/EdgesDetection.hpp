//
//  EdgesDetection.hpp
//  CellTypes
//
//  Created by Pierre Tardif on 28/03/2020.
//

#ifndef EdgesDetection_hpp
#define EdgesDetection_hpp

#include <stdio.h>
#include "ofxGui.h"
#include "ofxAutoReloadedShader.h"

class EdgesDetection{
    public :
    EdgesDetection(){
        setup();
    }
    
    void setup();
    void initGui();
    
    ofParameterGroup edgeGroup;
    ofParameter< ofVec2f > thickness;
    
    void addUniforms();
    ofxAutoReloadedShader* shader;
    
    
    
};

#endif /* EdgesDetection_hpp */
