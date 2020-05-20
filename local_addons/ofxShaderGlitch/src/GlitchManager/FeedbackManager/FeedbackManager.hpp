//
//  FeedbackManager.hpp
//  example
//
//  Created by Pierre Tardif on 20/05/2020.
//

#ifndef FeedbackManager_hpp
#define FeedbackManager_hpp

#include <stdio.h>

#include "ofMain.h"

#include "ofxGui.h"

// for faster live coding
#include "ofxAutoReloadedShader.h"
#include "../../constants.h"

class FeedbackManager{

    public:
    FeedbackManager(){
        setup();
    }
    
    
    void init(glm::vec2 resImg);
    
    void setup();
    
    bool initialised;
    
    // GUI
    ofParameterGroup feedbackGUI;
    ofParameter < bool > activateFeedback;
    
    ofxAutoReloadedShader fShader;
    ofFbo fbo1, fbo2;
        
    int indexFbo;
    void begin();
    void end(glm::vec2 resImg);
    
};
#endif /* FeedbackManager_hpp */
