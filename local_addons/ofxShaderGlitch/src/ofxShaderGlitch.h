//
//  ofxShaderGlitch.hpp
//  Gradient
//
//  Created by Pierre Tardif on 10/05/2020.
//

#ifndef ofxShaderGlitch_hpp
#define ofxShaderGlitch_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofEvents.h"

 // ---- dependency ---- //
#include "ofxAutoReloadedShader.h"

// ---- custom ---- //
#include "GlitchManager/GlitchManager.hpp"


class ofxShaderGlitch{
    public :
    ofxShaderGlitch(){
        
    }
    
    void setup();
    void keyPressed(ofKeyEventArgs &keyArgs);
    
    
    // shader
    GlitchManager glitch;
    void begin();
    void end();
    void drawGUI();

};

#endif /* ofxShaderGlitch_hpp */
