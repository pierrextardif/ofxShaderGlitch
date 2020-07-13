//
//  ofxShaderGlitch.hpp
//  Gradient
//
//  Created by Pierre Tardif on 10/05/2020.
//

///	TODO:
///	+	reset per zone
///	+	make play toggle public to expose to gui
///	+	make 2 public groups to split into external gui

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

//#define USE_GUI_INTERNAL
#ifdef USE_GUI_INTERNAL
#include "ofxGui.h"
#endif

class ofxShaderGlitch {
public:

	ofxShaderGlitch() {
		//setPath_GlobalFolder("ofxShaderGlitch");
		//NOTE: this above line will force create the folder if do not exist at startup,
		//and it's not the desired behaviour if we are using the addon in another addon like ofxFboFxHelper
		//bc we will have an empty folder.. So:

		path_GLOBAL_Folder = "ofxShaderGlitch";

		bEnabledKeys = false;
	};		

	~ofxShaderGlitch() {};

	void setup();
	void exit();
	
	void keyPressed(ofKeyEventArgs &keyArgs);
	bool bEnabledKeys;
	void setKeysEnabled(bool b) {
		bEnabledKeys = b;
		presetsManager.setEnableKeys(b);
	}

	//shader
//private:
public:
	GlitchManager glitch;//TODO: would like to make public only required params!

public:
	ofParameterGroup params_Control;

	//TODO:
	ofParameterGroup getParamGroup() {
		return glitch.params;
	}
	ofParameterGroup getParamGroup_Tiling() {
		return glitch.typeTiling;
	}
	ofParameterGroup getParamGroup_TypeEffect() {
		return glitch.typeEffect;
	}
	ofParameterGroup getParamGroup_Control() {
		return params_Control;
	}

	//--------------------------------------------------------------
	void setLogLevel(ofLogLevel level)
	{
		ofSetLogLevel("ofxShaderGlitch", level);
	}
	
	//-

public:
	void begin();
	void end();

	//TODO:
	//optional to debug..
	void update(ofEventArgs & args);

#ifdef USE_GUI_INTERNAL
public:
	void drawGUI();
private:
	void refreshGUI();
#endif

private:
	string path_GLOBAL_Folder;

public:
	void setPath_GlobalFolder(string folder);//path for root container folder

	//presetsManager
public:
	ofxPresetsManager presetsManager;
	void setupPresetsManager();
	ofParameterGroup params;
	void Changed_Params(ofAbstractParameter &e);

private:
#ifdef USE_GUI_INTERNAL
	ofxPanel gui;//internal gui
#endif
	bool bVisibleGui = false;

public:
	//--------------------------------------------------------------
	void setVisibleGui(bool b) {
		bVisibleGui = b;
		//presetsManager.setVisible_GUI_ImGui(bVisibleGui);
		presetsManager.setVisible_PresetClicker(bVisibleGui);
	}
	//--------------------------------------------------------------
	void setToggleVisibleGui() {
		bVisibleGui = !bVisibleGui;
		//presetsManager.setVisible_GUI_ImGui(bVisibleGui);
		presetsManager.setVisible_PresetClicker(bVisibleGui);
	}

	//--

private:
	//--------------------------------------------------------------
	void CheckFolder(string _path)
	{
		ofLogNotice(__FUNCTION__) << _path;

		ofDirectory dataDirectory(ofToDataPath(_path, true));

		//check if folder path exist
		if (!dataDirectory.isDirectory())
		{
			ofLogError(__FUNCTION__) << "FOLDER NOT FOUND! TRYING TO CREATE...";

			//try to create folder
			bool b = dataDirectory.createDirectory(ofToDataPath(_path, true));

			//debug if creation has been succeded
			if (b) ofLogNotice(__FUNCTION__) << "CREATED '" << _path << "'  SUCCESSFULLY!";
			else ofLogError(__FUNCTION__) << "UNABLE TO CREATE '" << _path << "' FOLDER!";
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "OK! LOCATED FOLDER: '" << _path << "'";//nothing to do
		}
	}
};

#endif /* ofxShaderGlitch_hpp */
