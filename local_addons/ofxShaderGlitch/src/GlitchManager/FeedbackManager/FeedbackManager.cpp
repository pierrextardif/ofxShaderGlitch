//
//  FeedbackManager.cpp
//  example
//
//  Created by Pierre Tardif on 20/05/2020.
//

#include "FeedbackManager.hpp"


void FeedbackManager::setup(){
    initialised = false;
    indexFbo = 0;
    
    feedbackGUI.setName("feedback");
    feedbackGUI.add(activateFeedback.set("feedback ON /OFF", false));
    
}

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

void FeedbackManager::begin(){
    if(activateFeedback){
        fbo1.begin();
//        if(indexFbo == 1)fbo2.begin();
       
               
    }
}

void FeedbackManager::end(glm::vec2 resImg){
    if(activateFeedback){
        
//        if(indexFbo == 0){
            fbo1.end();
            fbo2.begin();
//        }
        
//        if(indexFbo == 1){
//            fbo2.end();
//            fbo1.begin();
//        }
        
        
        fShader.begin();
        fShader.setUniform2f("u_resImg", resImg);
        fShader.setUniform1f("u_time", ofGetElapsedTimef());

//        if(indexFbo == 0){
            fShader.setUniformTexture("u_texFeedback", fbo2.getTexture(), 1);
            fbo1.draw(0,0);
//        }
//        if(indexFbo == 1){
//            fShader.setUniformTexture("u_texFeedback", fbo2.getTexture(), 1);
//            fbo1.draw(0,0);
//        }
        fShader.end();
        
//        if(indexFbo == 0){
            fbo2.end();
//            fbo1 = fbo2;
//        }
//        if(indexFbo == 1){
//            fbo1.end();
//            fbo2 = fbo1;
//        }
        
        fbo2.draw(0,0);
        
        indexFbo += 1;
        if(indexFbo == 2)indexFbo = 0;
    }
}


/*
 
 
 i = 0
 f0.begin();
 pre process
 f0.end();
 
 f1.begin();
 shader.begin();
 f0.draw(0,0);
 shader.end();
 f1.end();
 f1.draw(0,0);
 
 ////
 
 i = 0
 f0.begin();
 proprocess
 f0.end();
 
 f1.begin();
 shader.begin();
 shader.setUniformTexture(feedbackTexture);
 preprocessResult
 shader.end();
 f1.end();
 
 f1.draw(0,0);
 
 
 i = 1
 f1.begin();
 f1.end();
 
 
 */
