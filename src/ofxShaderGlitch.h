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

#include "ofxGui.h"

class ofxShaderGlitch {
public:
	ofxShaderGlitch() {
		setPath_GlobalFolder("ofxShaderGlitch");
	};
	~ofxShaderGlitch() {};

	void setup();
	void exit();
	void keyPressed(ofKeyEventArgs &keyArgs);

	//shader
public:
	GlitchManager glitch;//TODO: would like to make public only required params!

public:
	void begin();
	void end();
	void drawGUI();

private:
	void refreshGUI();
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
	ofxPanel gui;//internal gui
	bool bVisibleGui = false;

public:
	//--------------------------------------------------------------
	void setVisibleGui(bool b) {
		bVisibleGui = b;
		presetsManager.setVisible_GUI_Browser(bVisibleGui);
		presetsManager.setVisible_PresetClicker(bVisibleGui);
	}
	//--------------------------------------------------------------
	void setToggleVisibleGui() {
		bVisibleGui = !bVisibleGui;
		presetsManager.setVisible_GUI_Browser(bVisibleGui);
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
