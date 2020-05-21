//
//  FeedbackManager.cpp
//  example
//
//  Created by Pierre Tardif on 20/05/2020.
//

#include "FeedbackManager.hpp"


void FeedbackManager::setup(){
    initialised = false;
    
    feedbackGUI.setName("feedback");
    feedbackGUI.add(activateFeedback.set("feedback ON /OFF", true));
    
}


//--------------------------------------------------------------
void FeedbackManager::init(glm::vec2 resImg){
    
    if(!initialised){
        
        initialised = true;
        
        fbo1.allocate(resImg.x, resImg.y);
        fbo2.allocate(resImg.x, resImg.y);
        
        // shader load
        string path = PATHTOSHADERS;
        path.append( "Feedback/main");
        
        cout << "path = " << path << endl;
        fShader.load(path);
        
    }
}


//--------------------------------------------------------------
void FeedbackManager::begin(){
    if(activateFeedback){
        fbo1.begin();
       
               
    }
}


//--------------------------------------------------------------
void FeedbackManager::end(glm::vec2 resImg){
    if(activateFeedback){
        fbo1.end();
        fbo2.begin();
        
            fShader.begin();
            fShader.setUniform2f("u_resImg", resImg);
            fShader.setUniform1f("u_time", ofGetElapsedTimef());

            fShader.setUniformTexture("u_texFeedback", fbo2.getTexture(), 1);
            fbo1.draw(0,0);
            fShader.end();
        
        fbo2.end();
        fbo2.draw(0,0);
        
    }
}
