//
//  ofxShaderGlitch.hpp
//  Gradient
//
//  Created by Pierre Tardif on 10/05/2020.
//

///	TODO:
///	+	reset per zone
///	+	ndi addon
///	+	

#ifndef ofxShaderGlitch_hpp
#define ofxShaderGlitch_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofEvents.h"

// ---- dependency ---- //
#include "ofxAutoReloadedShader.h"

// ---- custom ---- //
#include "GlitchManager/GlitchManager.hpp"

#include "ofxPresetsManager.h"

#include "ofxGui.h"

class ofxShaderGlitch{
    public :
    ofxShaderGlitch(){
        
    }
    
    void setup();
    void exit();
    void keyPressed(ofKeyEventArgs &keyArgs);
        
    // shader
    GlitchManager glitch;
    void begin();
    void end();
    void drawGUI();

	//presetsManager
	ofxPresetsManager presetsManager;

	void setupPresetsManager();
	ofParameterGroup params;
	ofxPanel gui;
	bool bVisibleGui = false;
	void setVisibleGui(bool b) {
		bVisibleGui = b;
		presetsManager.setVisible_GUI_Browser(bVisibleGui);
		presetsManager.setVisible_PresetClicker(bVisibleGui);
	}
	void setToggleVisibleGui() {
		bVisibleGui = !bVisibleGui;
		presetsManager.setVisible_GUI_Browser(bVisibleGui);
		presetsManager.setVisible_PresetClicker(bVisibleGui);
	}
};

#endif /* ofxShaderGlitch_hpp */
