//
//  ofxShaderGlitch.cpp
//  Gradient
//
//  Created by Pierre Tardif on 10/05/2020.
//

#include "ofxShaderGlitch.h"


void ofxShaderGlitch::exit() {
	presetsManager.exit();//only required to store some gui/class settings.
}

void ofxShaderGlitch::setup() {
	//presetsManager
	setupPresetsManager();

	ofAddListener(ofEvents().keyPressed, this, &ofxShaderGlitch::keyPressed);
}

//--------------------------------------------------------------
void ofxShaderGlitch::begin() {
	glitch.begin();
}

//--------------------------------------------------------------
void ofxShaderGlitch::end() {
	glitch.end();
	
	//drawGUI();
}

//--------------------------------------------------------------
void ofxShaderGlitch::drawGUI() {
	//glitch.drawGUI();

	if(bVisibleGui) gui.draw();
}

//--------------------------------------------------------------
void ofxShaderGlitch::keyPressed(ofKeyEventArgs &keyArgs) {
	if (keyArgs.key == 'g')glitch.guiON = !glitch.guiON;
	if (keyArgs.key == 'c') {
		glitch.nonMaxAndContinuity = !glitch.nonMaxAndContinuity;
		//cout << " maxContinuity = " << ofToString(glitch.nonMaxAndContinuity) << endl;
	}
	if (keyArgs.key == ' ') {
		presetsManager.setToggleRandomizerPreset();
	}
}

//--------------------------------------------------------------
void ofxShaderGlitch::setupPresetsManager()
{
	////optional folders. same as defaults. look on /data folder
	//presetsManager.setPath_GlobalFolder("ofxPresetsManager");//main container folder
	//presetsManager.setPath_KitFolder("presets");//from kit/livePanel presets
	//presetsManager.setPath_PresetsFolder("archive");//for browser file presets

	//-

	//prepare the group and add subgroups that you want presetize:
	params = glitch.params;
	gui.setup(params);

	//added ofParametegrGroup to presetsManager
	//define desired trigged keys
	presetsManager.add(params, { '1', '2', '3', '4', '5', '6', '7', '8' });
	presetsManager.setup();

	//customize layout
	presetsManager.setVisible_PresetClicker(true);
	int pad = 20;
	presetsManager.setPosition_PresetClicker(pad, ofGetHeight() - 50 - pad, 50);

	//final startup refresh
	presetsManager.refresh();

	//nothing more!
	//all presets are stored on runtime to xml files.
	//there's a kind of faster 'memory mode' that stores presets on a vector, and to xml files only on exit. 
	//update, draw and autosave its handled by the addon. only must call exit method.
	//api can disable trigger keys, hide/show gui and customize layout...etc
}