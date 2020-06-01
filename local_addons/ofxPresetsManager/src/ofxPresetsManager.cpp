#include "ofxPresetsManager.h"

#pragma mark - OF

//--------------------------------------------------------------
ofxPresetsManager::ofxPresetsManager()
{
	//subscribed to auto run update and draw without required 'manual calls'
	ofAddListener(ofEvents().update, this, &ofxPresetsManager::update);
	ofAddListener(ofEvents().draw, this, &ofxPresetsManager::draw);

	//-

	DISABLE_CALLBACKS = true;

	//-

	//mainly to measure performance when using hd files vs faster memory vectors
#ifdef TIME_SAMPLE_MEASURES
	//specify a target framerate
	//TIME_SAMPLE_SET_FRAMERATE(fps);
	//TIME_SAMPLE_ENABLE();
	TIME_SAMPLE_SET_AVERAGE_RATE(0.1);
	TIME_SAMPLE_SET_DRAW_LOCATION(TIME_SAMPLE_DRAW_LOC_BOTTOM_LEFT);
	TIME_SAMPLE_SET_PRECISION(4);
	TIME_SAMPLE_GET_INSTANCE()->setEnabled(true);
	TIME_SAMPLE_DISABLE();
#endif

	//--

	//default settings paths

	//top parent folder
	path_GLOBAL_Folder = "ofxPresetsManager";
	//default addon folder

	//default kit folder for live/favorites presets
	path_Kit_Folder = "presets";

	//big browser
	path_PresetsFolder = "archive";
	//default archive presets folder to browse

	//app settings
	path_Control = "settings";

	//to add to all presets filenames
	path_Prefix = "_preset_";

	//--

	bKeys = false;
	keysNotActivated = true;
	lastMouseButtonState = false;

	modeKeySave = OF_KEY_CONTROL;
	bKeySave = false;

	modKeySwap = OF_KEY_ALT;
	bKeySwap = false;

	//--

	//this multidimension is for multiple gui/groups (feature not implemented!)
	groups.reserve(NUM_MAX_GUIS);
	lastIndices.reserve(NUM_MAX_GUIS);
	keys.reserve(NUM_MAX_GUIS);

	//--

	//alternative callbacks to know when preset loading/save is done
	DONE_load.set("DONE LOAD", false);//easy callback to know (in ofApp) that preset LOAD is done
	DONE_save.set("DONE SAVE", false);//easy callback to know (in ofApp) that preset SAVE is done

	//we can use too the easy isDoneLoad() to check in ofApp update() as kind of callback.

	//-

	//control parameters

	PRESET_selected.set("PRESETS", 1, 1, numPresets_OfFavorites);
	//PRESET2_selected.set("PRESETS2", 1, 1, numPresets_OfFavorites);//this multidimension is for multiple gui/groups (feature not implemented!)

#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	MODE_Browser_NewPreset.set("NEW!", false);
	SHOW_Browser.set("SHOW BROWSER", false);
	//SHOW_MenuTopBar.set("SHOW MENU", false);

	browser_PresetName = "NO_NAME_PRESET";//browser loaded preset name

	//radomizer
	//select a random preset (from 1 to 8)
	//params_Randomizer.setName("Randomizer");
	bRandomize.set("RANDOMIZE", false);
	ENABLE_RandomizeTimer.set("MODE TIMER", false);
	randomizeSpeedF.set("SPEED", 0.8f, 0.f, 1.f);
#endif

	bSave.set("SAVE", false);
	//bLoad.set("LOAD", false);

	MODE_MemoryLive.set("MODE MEMORY", false);
	loadToMemory.set("LOAD TO MEMORY", false);
	saveFromMemory.set("SAVE FROM MEMORY", false);
	autoLoad.set("AUTO LOAD", true);
	autoSave.set("AUTO SAVE", true);
	bAutosaveTimer.set("TIMER AUTO SAVE", false);

	SHOW_Gui_Internal.set("SHOW CONTROL GUI ", false);
	SHOW_ClickPanel.set("SHOW CLICK PANEL", false);

	bCloneRight.set("CLONE >", false);
	bCloneAll.set("CLONE ALL", false);

	ENABLE_Keys.set("ENABLE KEYS", true);

	//-

	//layout
	Gui_Internal_Position.set("GUI POSITION",
		glm::vec2(ofGetWidth() * 0.5, ofGetHeight()* 0.5),
		glm::vec2(0, 0),
		glm::vec2(ofGetWidth(), ofGetHeight())
	);

#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	ImGui_Position.set("GUI BROWSER POSITION",
		glm::vec2(ofGetWidth() * 0.5, ofGetHeight()* 0.5),
		glm::vec2(0, 0),
		glm::vec2(ofGetWidth(), ofGetHeight())
	);
	ImGui_Size.set("GUI BROWSER SIZE",
		glm::vec2(100, 0),
		glm::vec2(0, 0),
		glm::vec2(ofGetWidth(), ofGetHeight())
	);
#endif

	//-

	//exclude from xml settings

	//TODO:
	//BUG: 
	//avoid make the group xml empty (when all inside params are excluded!)

	//bLoad.setSerializable(false);
	bSave.setSerializable(false);
	bCloneRight.setSerializable(false);
	bCloneAll.setSerializable(false);
	loadToMemory.setSerializable(false);
	saveFromMemory.setSerializable(false);
	//SHOW_Gui_Internal.setSerializable(false);

	//params_Tools.setSerializable(false);

	//-

	//params groups

	params_Favorites.setName("USER");
	params_Favorites.add(PRESET_selected);
	//params_Favorites.add(PRESET2_selected);

	//params_Favorites.add(bLoad);
	params_Favorites.add(bSave);
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	params_Favorites.add(MODE_Browser_NewPreset);
#endif

	params_Options.setName("OPTIONS");
	params_Options.add(MODE_MemoryLive);
	params_Options.add(loadToMemory);
	params_Options.add(saveFromMemory);
	params_Options.add(autoLoad);
	params_Options.add(autoSave);
	params_Options.add(bAutosaveTimer);

	params_Gui.setName("GUI");
	params_Gui.add(SHOW_Gui_Internal);
	params_Gui.add(SHOW_ClickPanel);
	//params_Gui.add(SHOW_MenuTopBar);
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	params_Gui.add(SHOW_Browser);
#endif
	params_Gui.add(ENABLE_Keys);

	//layout
	params_Gui.add(Gui_Internal_Position);
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	params_Gui.add(ImGui_Position);
	params_Gui.add(ImGui_Size);
#endif

	params_Tools.setName("HELPER TOOLS");
	params_Tools.add(bCloneRight);
	params_Tools.add(bCloneAll);

#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	params_Tools.add(ENABLE_RandomizeTimer);
	params_Tools.add(randomizeSpeedF);
	params_Tools.add(bRandomize);
#endif

	//all nested params for callbacks and storage settings
	params_Control.setName("ofxPresetsManager");
	params_Control.add(params_Favorites);
	params_Control.add(params_Options);
	params_Control.add(params_Gui);
	params_Control.add(params_Tools);

	ofAddListener(params_Control.parameterChangedE(), this, &ofxPresetsManager::Changed_Params_Control);
}

//--------------------------------------------------------------
void ofxPresetsManager::setPosition_DEBUG(int x, int y)
{
	errorsDEBUG.setPosition(x, y);
}

//--------------------------------------------------------------
void ofxPresetsManager::setup(std::string name)///must be called after adding params group
{
	gui_LabelName = name;
	setup();
}

//--------------------------------------------------------------
void ofxPresetsManager::setup()
{
	DISABLE_CALLBACKS = true;

	//-

	//ofSetLogLevel("ofxPresetsManager", OF_LOG_NOTICE);
	ofLogNotice("ofxPresetsManager") << "setup()";

	//-

	//gui font
	myTTF = path_GLOBAL_Folder + "/" + "fonts/overpass-mono-bold.otf";
	sizeTTF = 10;
	bool bLoaded = myFont.load(myTTF, sizeTTF, true, true);

#ifdef INCLUDE_DEBUG_ERRORS
	if (!bLoaded)
	{
		errorsDEBUG.addError(gui_LabelName + " ofxPresetsManager", "setup() myFont", myTTF);
	}
#endif

	//--

	//ofxGui theme

	string pathFont = path_GLOBAL_Folder + "/" + "fonts/overpass-mono-bold.otf";
	//must check this font file is found there
	ofFile file(pathFont);
	if (file.exists())
	{
		ofxGuiSetFont(pathFont, 9);
	}
#ifdef INCLUDE_DEBUG_ERRORS
	else
	{
		errorsDEBUG.addError(gui_LabelName + " ofxPresetsManager", "setup() ofxGui", pathFont);
	}
#endif

	//-

	//colors
	ofxGuiSetDefaultHeight(20);
	ofxGuiSetBorderColor(32);
	ofxGuiSetFillColor(ofColor(48));
	ofxGuiSetTextColor(ofColor::white);
	ofxGuiSetHeaderColor(ofColor(24));

	//control gui
	gui_InternalControl.setup(gui_LabelName);
	gui_InternalControl.add(params_Control);
	gui_InternalControl.setPosition(ofGetWidth() - 210, 10);//default

	//collapse
	//gui_InternalControl.getGroup("ofxPresetsManager").minimize();
	auto &gPanel = gui_InternalControl.getGroup("ofxPresetsManager");
	auto &gGui = gPanel.getGroup("GUI");
	gGui.minimize();

	auto &gGuiPos = gGui.getGroup("GUI POSITION");
	gGuiPos.minimize();
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	auto &gGuiPos1 = gGui.getGroup("GUI BROWSER POSITION");
	gGuiPos1.minimize();
	auto &gGuiPos2 = gGui.getGroup("GUI BROWSER SIZE");
	gGuiPos2.minimize();
#endif

	auto &gOptions = gPanel.getGroup("OPTIONS");
	//gOptions.minimize();
	//gOptions.maximize();
	gPanel.getGroup("HELPER TOOLS").minimize();

	//--

	//browser
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	browser_Setup();
#endif

	//--

	//timer auto save
	timerLast_Autosave = ofGetElapsedTimeMillis();

	//--

	////memory mode
	//load_AllKit_ToMemory();


	//-------

	//STARTUP

	//app settings
	DISABLE_CALLBACKS = false;

	load_ControlSettings();

	//TODO:
	//PRESET_selected_PRE = -1;

	setVisible_GUI_Internal(false);

	//--------


	////TODO
	////moved from add
	////TODO: bug on mixerBlend.. in load_AllKit_ToMemory...
	////gui_LabelName = groups[0].getName();//TODO: one group only

	////memory mode
	//load_AllKit_ToMemory();

}

//--------------------------------------------------------------
void ofxPresetsManager::windowResized(int w, int h)
{
	ofLogNotice("ofxPresetsManager") << "windowResized: " << w << "," << h;

	////use this bc save/load is not working
	//#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	//	ImGui_Size = glm::vec2(250, 0);//width/height
	//	ImGui_Position = glm::vec2(w - ImGui_Size.get().x - 10, 10);
	//#endif
}

//--------------------------------------------------------------
void ofxPresetsManager::update(ofEventArgs & args)
{
	//-

	//randomize timer
	if (ENABLE_RandomizeTimer)
	{
		uint32_t _time = ofGetElapsedTimeMillis();

		if (_time - randomizerTimer > (randomize_MAX_DURATION*(1.f - randomizeSpeedF)))
		{
			bRandomize = true;
		}
	}

	//-

	//plotters
#ifdef TIME_SAMPLE_MEASURES 
	TS_START("load1");
	TS_STOP("load1");
	TS_START("load2");
	TS_STOP("load2");
	TS_START("loadMem");
	TS_STOP("loadMem");
#endif

	//--

	//autosave
	//&& autoLoad? 
#ifndef INCLUDE_FILE_BROWSER_IM_GUI
	if (autoSave && bAutosaveTimer && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
#else
	if (!MODE_Browser_NewPreset && autoSave && bAutosaveTimer && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
#endif
	{
		ofLogNotice("ofxPresetsManager") << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t[AUTOSAVE]";

		//app settings
		save_ControlSettings();

		//save current preset
		doSave(PRESET_selected - 1);
		//doSave2(PRESET2_selected - 1);

		if (!MODE_MemoryLive)
		{
			//MODE A: from hd file
			//not required because the files are already there
		}
		else
		{
			//MODE B: direct from memory
			save_AllKit_FromMemory();
		}

		//-

		//auto save timer
		timerLast_Autosave = ofGetElapsedTimeMillis();
	}
}

//---------------------------------------------------------------------
void ofxPresetsManager::draw(ofEventArgs & args)
{
	//--

	//debug errors
#ifdef INCLUDE_DEBUG_ERRORS
	errorsDEBUG.draw();
#endif

	//--

	//internal ofxGui panel
	if (SHOW_Gui_Internal)
	{
		gui_InternalControl.draw();
	}

	//--

	//user clicker boxes preset selector 
	//(from live kit/favorites)
	if (SHOW_ClickPanel)
	{
		drawPresetClicker();
	}

	//----

	//browser

#ifdef INCLUDE_FILE_BROWSER_IM_GUI

	//draw ImGui
	if (SHOW_Browser)
	{
		browser_draw_ImGui();
		//not working
		//bImGui_mouseOver = browser_draw_ImGui();
	}
#endif
}


//--------------------------------------------------------------
void ofxPresetsManager::drawPresetClicker()
{
	//user trigger boxes clickable selector and save button
	//draws some minimalistic graphics to monitor the active preset
	//when graphics are drawn you can also click on them for saving/loading

	if (!lastMouseButtonState && ofGetMousePressed())
	{
		mousePressed(ofGetMouseX(), ofGetMouseY());
	}
	lastMouseButtonState = ofGetMousePressed();

	//-

	ofPushStyle();
	ofPushMatrix();

	ofTranslate(clicker_Pos);

	//-

	//0. box of all boxes background
	ofFill();
	ofSetColor(0, 128);
	ofDrawRectangle(0, 0,
		clicker_cellSize * (keys[0].size() + 1), clicker_cellSize*groups.size());

	//-

	ofNoFill();
	ofSetColor(ofColor::white);

	for (size_t i = 0; i < keys.size(); ++i)//draw all guis/groups
	{
		//draw any preset box
		size_t k = 0;
		for (; k < keys[i].size(); ++k)
		{
			//1.1 outbox border container
			ofDrawRectangle(clicker_cellSize * k, clicker_cellSize * i, clicker_cellSize, clicker_cellSize);

			//-

			//1.2 label boxes
			if (!myFont.isLoaded())//without ttf font
			{
				ofDrawBitmapString(ofToString((char)keys[i][k]),
					clicker_cellSize*k + 8,
					clicker_cellSize*i + 18);
			}
			else//custom font 
			{
				myFont.drawString(ofToString((char)keys[i][k]),
					clicker_cellSize * k + 0.5 * clicker_cellSize - 0.25 * sizeTTF,
					clicker_cellSize * i + 0.5 * clicker_cellSize + 0.5 * sizeTTF);
			}

			//-

			//2. inner box. double mark current selected preset
			if (lastIndices[i] == k)
			{
				float r = 3.0f;
				float pd = 4.0f;
				int color = 200;

				ofPushStyle();
				ofSetColor(color, 164);

				//-

				//border only
				ofNoFill();
				ofDrawRectRounded(clicker_cellSize * k + pd, clicker_cellSize * i + pd,
					clicker_cellSize - 2 * pd, clicker_cellSize - 2 * pd,
					r);

				//-

				//filled
				ofFill();
				ofSetColor(color, 64);
				ofDrawRectRounded(clicker_cellSize * k + pd, clicker_cellSize * i + pd,
					clicker_cellSize - 2 * pd, clicker_cellSize - 2 * pd,
					r);
				ofNoFill();

				ofPopStyle();
			}
		}

		//-

		//?
		//for (; k < presets[i]; ++k)
		//{
		//	ofDrawRectangle(clicker_cellSize * k, clicker_cellSize * i, 
		//		clicker_cellSize, clicker_cellSize);

		//	if (lastIndices[i] == k)
		//		ofDrawRectangle(clicker_cellSize * k + 4, clicker_cellSize * i + 4, clicker_cellSize - 8, clicker_cellSize - 8);
		//}

		//-

		//3. save button

		//box
		ofDrawRectangle(clicker_cellSize * k, clicker_cellSize * i, clicker_cellSize, clicker_cellSize);

		//-

		//save label
		int ySave = clicker_cellSize * i + 0.5 * clicker_cellSize + 0.5 * sizeTTF;
		if (!myFont.isLoaded())//without ttf font
		{
			ofDrawBitmapString("SAVE",
				clicker_cellSize*k + 8,
				clicker_cellSize*i + 18);
		}
		else//custom font 
		{
			float wx = 0.5f * myFont.getStringBoundingBox("SAVE", 0, 0).width;
			myFont.drawString("SAVE",
				clicker_cellSize * k + 0.5 * clicker_cellSize - wx,
				//clicker_cellSize * k + 0.5 * clicker_cellSize - 1.8f * sizeTTF,
				ySave);
		}
		k++;//?

		//-

		//group kit name
		if (SHOW_GroupName)
		{
			string info = groups[i].getName();

			//double font to improve different background colors
			int gap = 1;
			//int xG = clicker_cellSize * k + 15;

			float strW = myFont.getStringBoundingBox(info, 0, 0).width;
			int xG = -strW - 20;
			ySave = ySave - 2;//little up

			ofSetColor(ofColor::black);//shadow
			if (myFont.isLoaded())
				myFont.drawString(info, xG + gap, ySave + gap);
			else
				ofDrawBitmapString(info, xG + gap, ySave + gap);

			ofSetColor(ofColor::white);//white
			if (myFont.isLoaded())
				myFont.drawString(info, xG, ySave);
			else
				ofDrawBitmapString(info, xG, ySave);
		}

		//-

		bool bLateralPosition = true;

		//4. help info text
		if (debugClicker && ENABLE_Keys)
		{
			string info = "";
			bool bSimpleInfo = true;
			if (bSimpleInfo)
			{
				// keys[i][k]
				info += "[" + ofToString((char)keys[0][0]) + "|";
				info += ofToString((char)keys[0][keys[0].size() - 1]) + "]";
			}
			else
			{
				info += "MOUSE-CLICK OR KEYS [1-8] TO LOAD PRESET\n";
				info += "HOLD [CTRL] TO SAVE/COPY PRESET\n";
			}

			int x;
			int y;
			int gap = 1;
			int pad = 13;

			if (!bLateralPosition)//vertical position below boxes
			{
				x = 0;
				y = (clicker_cellSize + 15) * groups.size();
			}
			else//lateral position right to the boxes
			{
				x = clicker_cellSize * k + pad;
				y = ySave - (bSimpleInfo ? -2 : sizeTTF);
			}

			//double font to improve different background colors
			ofSetColor(ofColor::black);//shadow
			if (myFont.isLoaded())
				myFont.drawString(info, x + gap, y + gap);
			else
				ofDrawBitmapString(info, x + gap, y + gap);

			ofSetColor(ofColor::white);//white
			if (myFont.isLoaded())
				myFont.drawString(info, x, y);
			else
				ofDrawBitmapString(info, x, y);
		}

		ofPopStyle();
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
ofxPresetsManager::~ofxPresetsManager()
{
	//exit();
}

//-

#pragma mark - ENGINE

//--------------------------------------------------------------
string ofxPresetsManager::getPresetName(string gName, int presetIndex)
{
	string strFolder;
	string strFile;
	string strPath;

	strFolder = path_GLOBAL_Folder + "/" + path_Kit_Folder + "/";
	strFile = groupName + path_Prefix + ofToString(presetIndex) + ".xml";
	strPath = strFolder + strFile;

	return strPath;
}

//-

//--------------------------------------------------------------
void ofxPresetsManager::add(ofParameterGroup params, int _num_presets)//main adder
{
	//main adder of a ofParameterGroup for preset management

	ofLogNotice("ofxPresetsManager") << "added group";

	groups.push_back(params);//each enqued group-param handles all (_num_presets) '8' presets

	lastIndices.push_back(0);//?
	newIndices.push_back(0);//?
	presets.push_back(_num_presets);//?

	//-

	//update control gui panel params
	numPresets_OfFavorites = _num_presets;
	PRESET_selected.setMax(numPresets_OfFavorites);

	//TODO:
	//PRESET2_selected.setMax(numPresets_OfFavorites);

	//-

	//path folder and xml presets file names
	groupName = groups[0].getName();//TODO: one group only
	//groupName2 = groups[1].getName();//TODO: one group only

	ofLogNotice("ofxPresetsManager") << "groupName: " << groupName;

	//-

	//temporary name only to debug purposes
	//final label name to gui display will be setted if setup("name") is called 
	gui_LabelName = groups[0].getName();
	//TODO: one group only

	//-

	//memory mode
	load_AllKit_ToMemory();
}

//--------------------------------------------------------------
void ofxPresetsManager::add(ofParameterGroup params, initializer_list<int> keysList)
{
	//adds a ofParameterGroup for preset management with the list of trigger keys

	add(params, keysList.size());

	keys.resize(groups.size());
	int i = groups.size() - 1;

	keys[i].reserve(keysList.size());

	for (const int &key : keysList)
		keys[i].push_back(key);

	if (keysNotActivated)
		addKeysListeners();
}

//-

//--------------------------------------------------------------
void ofxPresetsManager::save(int presetIndex, int guiIndex)
{
	ofLogVerbose("ofxPresetsManager") << "save(" << presetIndex << "," << guiIndex << ")";

	//clamp limiters
	if (guiIndex >= 0 && guiIndex < (int)groups.size()
		&& (presetIndex >= 0) && (presetIndex < NUM_OF_PRESETS))
	{
		ofLogVerbose("ofxPresetsManager") << "DONE_save (1)";

		//it's important if this line is before or after ofSerialize
		DONE_save = true;

		//-

		//MODE A: from hd file

		if (!MODE_MemoryLive)
		{

			TS_START("saveFile1");

			std::string strPath = getPresetName(groups[guiIndex].getName(), presetIndex);
			ofXml settings;
			ofSerialize(settings, groups[guiIndex]);
			settings.save(strPath);

			TS_STOP("saveFile1");
		}

		//-

		//MODE B: direct from memory

		else
		{

			TS_START("saveMem1");

			//ofLogNotice("ofxPresetsManager") << "MEMORY MODE";

			ofSerialize(settingsArray[presetIndex], groups[guiIndex]);

			TS_STOP("saveMem1");
		}
	}
	else
	{
		ofLogError("ofxPresetsManager") << "OUT OF RANGE SAVE";
	}
}

//--------------------------------------------------------------
void ofxPresetsManager::save(int presetIndex, string gName)
{
	ofLogVerbose("ofxPresetsManager") << "save(" << presetIndex << "," << gName << ")";

	int guiIndex = getGuiIndex(gName);

	if (guiIndex >= 0 && guiIndex < (int)groups.size()
		&& (presetIndex >= 0) && (presetIndex < NUM_OF_PRESETS))
	{
		ofLogVerbose("ofxPresetsManager") << "DONE_save (2)";

		//it's important if this line is before or after ofSerialize
		DONE_save = true;

		//-

		//MODE A: from hd file

		if (!MODE_MemoryLive)
		{
			TS_START("saveFile2");//for TimeMeasurements only

			std::string strPath = getPresetName(gName, presetIndex);
			ofXml settings;
			ofSerialize(settings, groups[guiIndex]);
			settings.save(strPath);

			////it's important if this line is before or after ofSerialize
			//DONE_save = true;

			TS_STOP("saveFile2");//for TimeMeasurements only
		}

		//-

		//MODE B: direct from memory

		else
		{
			TS_START("saveMem2");//for TimeMeasurements only

			//ofLogNotice("ofxPresetsManager") << "MEMORY MODE";

			ofSerialize(settingsArray[presetIndex], groups[guiIndex]);

			TS_STOP("saveMem2");//for TimeMeasurements only
		}
	}
	else
	{
		ofLogError("ofxPresetsManager") << "OUT OF RANGE SAVE";
	}

}

//--------------------------------------------------------------
void ofxPresetsManager::load(int presetIndex, int guiIndex)
{
	ofLogVerbose("ofxPresetsManager") << "load(" << presetIndex << "," << guiIndex << ")";

	//TODO:
	//guiIndex is not the preset position, it's for multiplye guis!
	//its the target group, there's only one for all the 8 presets (in only-one-gui-mode)

	//clamp limiters
	if (guiIndex >= 0 && guiIndex < (int)groups.size()
		&& (presetIndex >= 0) && (presetIndex < NUM_OF_PRESETS))
	{
		if (!MODE_MemoryLive)
		{
			//MODE A: from hd file

			TS_START("loadFile1");//for TimeMeasurements only

			std::string strPath = getPresetName(groups[guiIndex].getName(), presetIndex);
			ofXml settings;
			settings.load(strPath);
			ofDeserialize(settings, groups[guiIndex]);

			TS_STOP("loadFile1");//for TimeMeasurements only
		}
		else
		{
			//MODE B: direct from memory

			TS_START("loadMem1");//for TimeMeasurements only

			//ofLogNotice("ofxPresetsManager") << "MEMORY MODE";

			//using xml array
			ofDeserialize(settingsArray[presetIndex], groups[guiIndex]);

			//TODO:
			//ERROR do not loads the group content
			//ofParameterGroup g;
			//g = groupsMem[presetIndex];
			//groups[0] = g;

			TS_STOP("loadMem1");//for TimeMeasurements only
		}

		//-

		//mark selected
		lastIndices[guiIndex] = presetIndex;

		//-

		//callback
		ofLogVerbose("ofxPresetsManager") << "DONE_load (1)";
		DONE_load = true;

		//simple callback
		bIsDoneLoad = true;
	}
	else
	{
		ofLogError("ofxPresetsManager") << "OUT OF RANGE LOAD";
	}
}

//--------------------------------------------------------------
void ofxPresetsManager::load(int presetIndex, string gName)
{
	ofLogVerbose("ofxPresetsManager") << "load(" << presetIndex << "," << gName << ")";

	int guiIndex = getGuiIndex(gName);

	if (guiIndex >= 0 && guiIndex < (int)groups.size()
		&& (presetIndex >= 0) && (presetIndex < NUM_OF_PRESETS))
	{
		if (!MODE_MemoryLive)
		{
			//MODE A: from hd file

			TS_START("loadFile2");//for TimeMeasurements only

			std::string strPath = getPresetName(gName, presetIndex);
			ofXml settings;
			settings.load(strPath);
			ofDeserialize(settings, groups[guiIndex]);

			TS_STOP("loadFile2");//for TimeMeasurements only
		}
		else
		{
			//MODE B: direct from memory

			TS_START("loadMem2");//for TimeMeasurements only

			//ofLogNotice("ofxPresetsManager") << "MEMORY MODE";

			ofDeserialize(settingsArray[presetIndex], groups[guiIndex]);

			TS_STOP("loadMem2");//for TimeMeasurements only
		}

		//-

		//mark selected
		lastIndices[guiIndex] = presetIndex;

		//-

		ofLogVerbose("ofxPresetsManager") << "DONE_load (1)";
		DONE_load = true;
	}
	else
	{
		ofLogError("ofxPresetsManager") << "OUT OF RANGE LOAD";
	}
}

//--------------------------------------------------------------
void ofxPresetsManager::loadPreset(int p)
{
	if (!DISABLE_CALLBACKS)// && (PRESET_selected != PRESET_selected_PRE))
	{
		ofLogNotice("ofxPresetsManager") << "loadPreset(" << ofToString(p) << ")";
		ofLogNotice("ofxPresetsManager") << "-------------------------------------------------------------------------------------------------------";

		if (PRESET_selected > 0 && PRESET_selected <= numPresets_OfFavorites)
		{
			PRESET_selected = p;
			//ofLogNotice("ofxPresetsManager") << ".";

			//PRESET_selected_PRE = PRESET_selected;//TODO:
		}
		else
		{
			ofLogNotice("ofxPresetsManager") << "IGNORE LOAD PRESET";
			//workaround clamp
			PRESET_selected = 1;//set to first as default presets when out of range
		}
	}
}

//--

//engine helpers
//--------------------------------------------------------------
int ofxPresetsManager::getPresetIndex(int guiIndex) const
{
	if (guiIndex > 0 && guiIndex < (int)groups.size())
	{
		return lastIndices[guiIndex];
	}
	else
	{
		return -1;
	}
}

//--------------------------------------------------------------
int ofxPresetsManager::getPresetIndex(string gName) const
{
	int guiIndex = getGuiIndex(gName);

	if (guiIndex > 0 && guiIndex < (int)groups.size())
	{
		return lastIndices[guiIndex];
	}
	else
	{
		return -1;
	}
}

//--------------------------------------------------------------
int ofxPresetsManager::getGuiIndex(string name) const
{
	for (size_t i = 0; i < (int)groups.size(); ++i)
	{
		if (groups[i].getName() == name)
		{
			return i;
		}
	}
	return -1;
}

//--------------------------------------------------------------
void ofxPresetsManager::setModeKeySave(int key)
{
	modeKeySave = key;
}

//--------------------------------------------------------------
void ofxPresetsManager::setModeKeySwap(int key)
{
	modKeySwap = key;
}

#pragma mark - OF_LISTENERS

//----------------------------------------------------------------
void ofxPresetsManager::keyPressed(ofKeyEventArgs &eventArgs)
{
#ifndef INCLUDE_FILE_BROWSER_IM_GUI
	if (bKeys && ENABLE_Keys)
#else
	if (bKeys && ENABLE_Keys && !bImGui_mouseOver)//disable keys when mouse over gui
#endif
	{
		const int &key = eventArgs.key;

		//-

		//mode key for saving with mouse or trigger keys
		if (key == modeKeySave)
		{
			bKeySave = true;
			ofLogVerbose("ofxPresetsManager") << "modeKeySave TRUE" << endl;
			return;
		}

		//mode key for swap with mouse or trigger keys
		else if (key == modKeySwap)
		{
			bKeySwap = true;
			ofLogVerbose("ofxPresetsManager") << "modKeySwap TRUE" << endl;
			return;
		}


		//-

		//TODO: 
		//controlled from outside
		//hide/show control gui
		else if (key == 'G')
		{
			SHOW_Gui_Internal = !SHOW_Gui_Internal;
			setVisible_GUI_Internal(SHOW_Gui_Internal);
			setVisible_GUI_Browser(SHOW_Gui_Internal);
			setVisible_PresetClicker(SHOW_Gui_Internal);
		}

		//if (key == 'g')
		//{
		//	setVisible_PresetClicker(!isVisible_PresetClicker());
		//}

		//-

		//timer to randomize and choice a random preset from the kit
		if (key == 'R')
		{
			setToggleRandomizerPreset();
		}

		//-

		//navigate kit/favorites presets
		if (key == OF_KEY_RIGHT && ENABLE_KeysArrowBrowse)
		{

			//browse presets
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
			if (MODE_Browser_NewPreset)
			{
				currentFile++;
				if (currentFile > files.size() - 1)
				{
					currentFile = files.size() - 1;
				}
				browser_PresetName = fileNames[currentFile];
				ofLogNotice("ofxPresetsManager") << "[>] LOAD" << endl;
				ofLogNotice("ofxPresetsManager") << "Preset Name: " << browser_PresetName;
				browser_PresetLoad(browser_PresetName);
			}
			else
#endif
				//browse favorites
			{
				int i = PRESET_selected;
				i++;
				if (i > NUM_OF_PRESETS)
					i = NUM_OF_PRESETS;
				PRESET_selected = i;
			}
		}

		else if (key == OF_KEY_LEFT && ENABLE_KeysArrowBrowse)
		{
			//browse presets
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
			if (MODE_Browser_NewPreset)
			{
				currentFile--;
				if (currentFile < 0)
				{
					currentFile = 0;
				}
				browser_PresetName = fileNames[currentFile];
				ofLogNotice("ofxPresetsManager") << "[<] LOAD" << endl;
				ofLogNotice("ofxPresetsManager") << "Preset Name: " << browser_PresetName;
				browser_PresetLoad(browser_PresetName);
			}
			else
#endif
				//browse favorites
			{
				int i = PRESET_selected;
				i--;
				if (i < 1)
					i = 1;
				PRESET_selected = i;
			}
		}

		//--

		//main shortcuts:
		//preset selector (like from 1 to 8)

		for (int i = 0; i < keys.size(); ++i)//i=witch group. only performs i=0 bc there's only one implemented group!
		{
			for (int k = 0; k < keys[i].size(); ++k)//performs all registered keys: one for each [8] preset
			{
				if (key == keys[i][k])
				{
					ofLogNotice("ofxPresetsManager") << "Key: " << (char)key;

					if (bKeySave)
					{
						save(k, i);
					}
					else
					{
						ofLogNotice("ofxPresetsManager") << "[" << k << "][" << i << "]";

						PRESET_selected = 1 + k;
					}
					return;
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofxPresetsManager::keyReleased(ofKeyEventArgs &eventArgs)
{
	//mod keys
	if (eventArgs.key == modeKeySave && ENABLE_Keys)
	{
		bKeySave = false;
		ofLogVerbose("ofxPresetsManager") << "modeKeySave FALSE" << endl;
	}
	else if (eventArgs.key == modKeySwap && ENABLE_Keys)
	{
		bKeySwap = false;
		ofLogVerbose("ofxPresetsManager") << "modKeySwap FALSE" << endl;
	}
}

//--------------------------------------------------------------
void ofxPresetsManager::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxPresetsManager::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxPresetsManager::keyReleased);
	keysNotActivated = false;
	bKeys = true;
}

//--------------------------------------------------------------
void ofxPresetsManager::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxPresetsManager::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxPresetsManager::keyReleased);
}

//-----------------------------------------------------
void ofxPresetsManager::mousePressed(int x, int y)
{
	x = x - clicker_Pos.x;
	y = y - clicker_Pos.y;

	int xIndex = x / clicker_cellSize;
	int yIndex = y / clicker_cellSize;
	//yIndex is for when we add more groups feature! (just one group for now)

	//Index -1 for out of boxes
	xIndex = (x > 0) ? xIndex : -1;
	yIndex = (y > 0) ? yIndex : -1;

	//debug mouse out of click cells
	if (false && xIndex != -1 && yIndex != -1)
		ofLogNotice("ofxPresetsManager") << "mousePressed: (" << xIndex << "," << yIndex << ")";

	//-

	//1. key presets buttons
	if (yIndex >= 0 && yIndex < (int)groups.size())
	{
		//avoid outer panel logs. only into the group row levels
		if (xIndex != -1 && yIndex != -1)
			ofLogVerbose("ofxPresetsManager") << "mousePressed: (" << xIndex << "," << yIndex << ")";

		//-

		if (xIndex >= 0 && xIndex < presets[yIndex])//? presets[0] its's the same than numPresets_OfFavorites!
		//if (xIndex >= 0 && xIndex < numPresets_OfFavorites)
		{
			//1. mod controlled by save modeKeySave
			if (bKeySave)
			{
				//save "memory" to the clicked Index
				doSave(xIndex);

				//will auto load
				PRESET_selected = 1 + xIndex;
			}

			//-

			//2. mod controlled by modKeySwap
			else if (bKeySwap)
			{
				int IndexSource = PRESET_selected - 1;
				int IndexDest = xIndex;
				string srcName = getPresetName(groups[0].getName(), IndexSource);
				string dstName = getPresetName(groups[0].getName(), IndexDest);

				//cout << "Source: " << IndexSource + 1 << endl;
				//cout << "Dest  : " << xIndex + 1 << endl;
				//cout << "Source: " << srcName << endl;
				//cout << "Dest  : " << dstName << endl << endl;

				//1. save source preset (from memory) to temp file
				string _pathSrc = "tempSrc.xml";
				ofXml settingsSrc;
				ofSerialize(settingsSrc, groups[0]);
				settingsSrc.save(_pathSrc);

				//2. load destination "from kit" to memory
				std::string _path2 = getPresetName(groups[0].getName(), IndexDest);
				ofXml settings2;
				settings2.load(_path2);
				ofDeserialize(settings2, groups[0]);

				//3. save destination preset (from memory) to temp file
				ofXml settingsDst;
				ofSerialize(settingsDst, groups[0]);

				//4. using files
				//save source (from dest)
				settingsDst.save(srcName);
				settingsSrc.save(dstName);

				//5. delete temp file
				ofFile _file;
				_file.removeFile(_pathSrc);

				//6. auto load source (the same preset was selected befor swap clicked!)
				PRESET_selected = 1 + xIndex;
			}

			//-

			//normal load (not any key modifier pressed)
			else
			{
				//will trig autoload callback on change, calling load(p, 0);
				PRESET_selected = 1 + xIndex;
			}
		}

		//2. last button (save button)
		else if (xIndex == presets[yIndex])
		{
			ofLogNotice("ofxPresetsManager") << "saveButton: (" << yIndex << ")";

			doSave(lastIndices[yIndex]);
			//save( lastIndices[yIndex], yIndex );
		}
	}
}

//----

#pragma mark - ENGINE

//--------------------------------------------------------------
void ofxPresetsManager::doCloneRight(int pIndex)
{
	ofLogNotice("ofxPresetsManager") << "doCloneRight: pIndex: " << pIndex;
	for (int i = pIndex + 1; i < numPresets_OfFavorites; i++)
	{
		save(i, 0);//0 is bc it's the only 1st params group implemented
	}
}

//--------------------------------------------------------------
void ofxPresetsManager::doCloneAll()
{
	ofLogNotice("ofxPresetsManager") << "doCloneAll";// << pIndex;
	for (int i = 0; i < numPresets_OfFavorites; i++)
	{
		save(i, 0);//0 is bc it's the only 1st params group implemented
	}
}

//--------------------------------------------------------------
void ofxPresetsManager::doLoad(int pIndex)
{
	ofLogVerbose("ofxPresetsManager") << "doLoad: pIndex: " << pIndex;
	load(pIndex, 0);//0 is bc it's the only 1st params group implemented
}

//--------------------------------------------------------------
void ofxPresetsManager::doSave(int pIndex)
{
	ofLogVerbose("ofxPresetsManager") << "doSave: pIndex: " << pIndex;
	save(pIndex, 0);//0 is bc it's the only 1st params group implemented
}

//--------------------------------------------------------------
void ofxPresetsManager::setToggleKeysControl(bool active)
{
	bKeys = active;
}

#pragma mark - CALLBACKS

//--------------------------------------------------------------
void ofxPresetsManager::Changed_Params_Control(ofAbstractParameter &e)
{
	if (!DISABLE_CALLBACKS)
	{
		string name = e.getName();

		ofLogNotice("ofxPresetsManager") << "Changed_Params_Control '" << name << "': " << e;

		//-

		if (name == "SAVE" && bSave)
		{
			ofLogNotice("ofxPresetsManager") << "SAVE: " << e;
			bSave = false;
			doSave(PRESET_selected - 1);
		}

		//else if (name == "LOAD" && bLoad)
		//{
		//	ofLogNotice("ofxPresetsManager") << "LOAD: " << e;
		//	bLoad = false;
		//	doLoad(PRESET_selected - 1);
		//}

		//-

		//tools
		else if (name == "CLONE >" && bCloneRight)
		{
			ofLogNotice("ofxPresetsManager") << "CLONE >: " << e;
			bCloneRight = false;
			doCloneRight(PRESET_selected - 1);
		}
		else if (name == "CLONE ALL" && bCloneAll)
		{
			ofLogNotice("ofxPresetsManager") << "CLONE ALL: " << e;
			bCloneAll = false;
			doCloneAll();
		}

		//--

		//randomizer
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
		else if (name == "RANDOMIZE" && bRandomize)
		{
			ofLogNotice("ofxPresetsManager") << "RANDOMIZE: " << e;
			bRandomize = false;

			//avoid random is the same previuous preset (TODO:improve)

			int _r = PRESET_selected;
			while (_r == PRESET_selected)
			{
				_r = (int)ofRandom(1, 9);
			}
			loadPreset(_r);

			//-

			//start timer again
			if (ENABLE_RandomizeTimer)
			{
				randomizerTimer = ofGetElapsedTimeMillis();
			}
		}
		else if (name == "MODE TIMER")
		{
			ofLogNotice("ofxPresetsManager") << "MODE TIMER: " << e;
		}
		else if (name == "SPEED")
		{
			ofLogNotice("ofxPresetsManager") << "SPEED: " << e;
			//randomizeSpeedF;
		}
#endif

		//--

		else if (name == "ENABLE KEYS")
		{
		}

		//--

		else if (name == "GUI POSITION")
		{
			ofLogVerbose("ofxPresetsManager") << "GUI POSITION: " << e;
			//clamp inside window
			float x, y;
			x = ofClamp(Gui_Internal_Position.get().x, 0, ofGetWidth() - 200);
			y = ofClamp(Gui_Internal_Position.get().y, 0, ofGetHeight() - 20);
			gui_InternalControl.setPosition(x, y);
		}

#ifdef INCLUDE_FILE_BROWSER_IM_GUI
		else if (name == "GUI BROWSER POSITION")
		{
			ofLogVerbose("ofxPresetsManager") << "GUI BROWSER POSITION: " << e;

			////clamp inside window
			//float x, y;
			//x = ofClamp(ImGui_Position.get().x, 0, ofGetWidth() - 200);
			//y = ofClamp(ImGui_Position.get().y, 0, ofGetHeight() - 20);
			//ImGui_Position = glm::vec2(x, y);
		}
#endif
		//--

		else if (name == "LOAD TO MEMORY" && loadToMemory)
		{
			ofLogNotice("ofxPresetsManager") << "loadToMemory:" << e;
			loadToMemory = false;
			load_AllKit_ToMemory();
		}

		else if (name == "SAVE FROM MEMORY" && saveFromMemory)
		{
			ofLogNotice("ofxPresetsManager") << "saveFromMemory:" << e;
			saveFromMemory = false;
			save_AllKit_FromMemory();
		}

		else if (name == "MODE MEMORY")
		{
			ofLogNotice("ofxPresetsManager") << "MODE MEMORY: " << e;

			if (MODE_MemoryLive)
			{
				//reload all xml preset files to memory
				load_AllKit_ToMemory();
			}
			else
			{
				//save all xml preset files to disk from memory
				save_AllKit_FromMemory();
			}
		}


		//----


		//1. selected preset NOT CHANGED

		//TODO: 
		//should be nice to add toggle to auto retrig or not behavior 

		else if (name == "PRESETS" && (PRESET_selected == PRESET_selected_PRE))
		{
			ofLogNotice("ofxPresetsManager") << "PRESET NOT Changed: " << e << ". BUT..";
			ofLogNotice("ofxPresetsManager") << "TRIG PRESET!";

			//browser
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
			if (MODE_Browser_NewPreset)
			{
				if (autoLoad)
				{
					load(PRESET_selected - 1, 0);
				}
			}
#endif

			bMustTrig = true;
		}

		//-

		//2. selected preset CHANGED

		else if (name == "PRESETS" && (PRESET_selected != PRESET_selected_PRE))
		{
			ofLogNotice("ofxPresetsManager") << "PRESET: " << e;

			//-

#ifndef INCLUDE_FILE_BROWSER_IM_GUI
			if (autoSave)// && autoLoad)
#else
			if (autoSave && !MODE_Browser_NewPreset)//workflow: browser mode bypasses autosave
#endif
			{
				save(PRESET_selected_PRE - 1, 0);
			}

			//-

			//remember this PRE state to know if changed or not on the next time..
			PRESET_selected_PRE = PRESET_selected;

			//-

			//indexes starts from 0, not from 1 like presets
			int xIndex = PRESET_selected - 1;
			int yIndex = 0;//TODO: one group only for now

			//-

			if (autoLoad)
			{
				load(xIndex, yIndex);
			}
			else
			{
				if (lastIndices.size() > 0
					&& yIndex < lastIndices.size()
					&& xIndex < NUM_OF_PRESETS)
				{
					//mark selector as lastone trigged
					lastIndices[yIndex] = xIndex;
				}
				else
				{
					ofLogError("ofxPresetsManager") << "lastIndices has 0 size!";
				}
			}
		}
		//else
		//{
		//	ofLogError("ofxPresetsManager") << "IGNORED PRESETS CHANGE";
		//}
	}
}

#pragma mark - SETTINGS

//--------------------------------------------------------------
void ofxPresetsManager::load_ControlSettings()
{
	ofXml settings;
	string path = path_GLOBAL_Folder + "/" + path_Control + "/" + "control.xml";
	bool bLoaded = settings.load(path);

#ifdef INCLUDE_DEBUG_ERRORS
	if (!bLoaded)
	{
		errorsDEBUG.addError(gui_LabelName + " ofxPresetsManager", "load_ControlSettings()", path);
	}
#endif

	ofLogNotice("ofxPresetsManager") << "load_ControlSettings: " << path;
	ofLogNotice("ofxPresetsManager") << "load_ControlSettings: PRESET " << PRESET_selected;

	if (bLoaded)
	{
		ofDeserialize(settings, params_Control);
	}
	else
	{
		ofLogError("ofxPresetsManager") << "FILE '" << path << "' NOT FOUND!";
	}
}

//--------------------------------------------------------------
void ofxPresetsManager::save_ControlSettings()
{
#ifndef DEBUG_BLOCK_SAVE_SETTINGS

	ofLogNotice("ofxPresetsManager") << "save_ControlSettings()";

	//TODO:
	//crashes!
	//it seems related to autoSave timer?

	DISABLE_CALLBACKS = true;//?

	//-

	//TODO: 
	//crashes sometimes?

	//get gui position from panel to update the position param
	Gui_Internal_Position = glm::vec2(gui_InternalControl.getPosition());

	//float x, y;
	//x = gui_InternalControl.getPosition().x;
	//y = gui_InternalControl.getPosition().y;
	//Gui_Internal_Position = glm::vec2(x, y);

	//---

//#ifdef INCLUDE_FILE_BROWSER_IM_GUI
//	//BUG: can't get gui settings...
//	//gui_Browser.~Gui.get
//	auto settings = ofxImGui::Settings();
//	auto p = settings.windowPos;
//	auto w = settings.windowSize;
//	ImGui_Position = glm::vec2(p.x, p.y);
//
//	//cout << "pos:" << ofToString(p) << endl;
//	//cout << "size:" << ofToString(w) << endl;
//	//cout << "ImGui_Position:" << ofToString(ImGui_Position.get()) << endl;
//#endif

	//---

	//TODO: 
	//crashes?

	try
	{
		ofLogNotice("ofxPresetsManager") << endl << params_Control.toString() << endl;

		ofXml settingsControl;

		//TODO: 
		//crashes here!
		ofSerialize(settingsControl, params_Control);

		string path = path_GLOBAL_Folder + "/" + path_Control + "/" + "control.xml";
		ofLogNotice("ofxPresetsManager") << "path: " << path;

		//TODO: 
		//crashes here!
		settingsControl.save(path);

		ofLogNotice("ofxPresetsManager") << settingsControl << endl;
	}
	catch (int n)
	{
		ofLogError("ofxPresetsManager") << "CATCH ERROR" << endl;
		throw;
	}

	//---
	//NOTES:
	/*
	//TODO:
	//BUG?
	//BUG: avoid make the HELPER group xml empty!

	//try
	//{
	//	throw 20;
	//}
	//catch (int e)
	//{
	//	cout << "An exception occurred. Exception Nr. " << e << '\n';
	//}

	//try {
	//	// code here
	//}
	//catch (int param) { cout << "int exception"; }
	//catch (char param) { cout << "char exception"; }
	//catch (...) { cout << "default exception"; }

	//try {
	//	try {
	//		// code here
	//	}
	//	catch (int n) {
	//		throw;
	//	}
	//}
	//catch (...) {
	//	cout << "Exception occurred";
	//}
	*/
	//---

	DISABLE_CALLBACKS = false;
#else
	ofLogNotice("ofxPresetsManager") << "[DEBUG] BLOCKED save_ControlSettings()";
#endif
}

//--------------------------------------------------------------
void ofxPresetsManager::setPath_KitFolder(string folder)
{
	path_Kit_Folder = folder;
}

//--------------------------------------------------------------
void ofxPresetsManager::setPath_PresetsFolder(string folder)
{
	path_PresetsFolder = folder;
}

//--------------------------------------------------------------
void ofxPresetsManager::setPath_GlobalFolder(string folder)
{
	path_GLOBAL_Folder = folder;
}

//--

//memory mode 
//(loaded from data vector instead of hd files)
//--------------------------------------------------------------
void ofxPresetsManager::save_AllKit_FromMemory()
{
	ofLogVerbose("ofxPresetsManager") << "save_AllKit_FromMemory()";

	for (int i = 0; i < NUM_OF_PRESETS; i++)
	{
		string strFolder;
		string strFile;
		string strPath;

		strFolder = path_GLOBAL_Folder + "/" + path_Kit_Folder + "/";
		strFile = groupName + path_Prefix + ofToString(i) + ".xml";
		strPath = strFolder + strFile;

		settingsArray[i].save(strPath);
	}

	//debug params
	bool bDEBUG = false;
	if (bDEBUG)
	{
		for (int i = 0; i < NUM_OF_PRESETS; i++)
		{
			ofLogNotice("ofxPresetsManager") << "settingsArray[" << i << "] " << ofToString(settingsArray[i].toString());
		}
	}
}

//--------------------------------------------------------------
void ofxPresetsManager::load_AllKit_ToMemory()
{
	ofLogNotice("ofxPresetsManager") << "load_AllKit_ToMemory()";

	////TODO:
	//groupName = groups[0].getName();
	//ofLogNotice("ofxPresetsManager") << "groups[0].getName(): " << groupName;
	//groupsMem.clear();
	//groupsMem.reserve(NUM_OF_PRESETS);
	//groupsMem.resize(NUM_OF_PRESETS);

	for (int i = 0; i < NUM_OF_PRESETS; i++)
	{
		//TODO:
		//PROBLEM:
		//this is not working because all the groups of the vector are shallow copies,
		//so all are changing together
		//pre-create params inside the group not avoid emptyness
		//ofParameterGroup g = groups[0];
		//groupsMem[i] = g;

		//-

		string pathFolder;
		string pathFilename;
		string pathComplete;
		pathFolder = path_GLOBAL_Folder + "/" + path_Kit_Folder + "/";
		pathFilename = groupName + path_Prefix + ofToString(i) + ".xml";
		pathComplete = pathFolder + pathFilename;

		//load xml file
		ofXml settings;
		bool bLoaded = settings.load(pathComplete);

		//debug
		if (false)
		{
			ofLogNotice("ofxPresetsManager") << "[" << i << "]";
			ofLogNotice("ofxPresetsManager") << "File: " << pathComplete
				<< "\n" << ofToString(settings.toString());
		}

		//-

		////TODO:
		//ofDeserialize(settings, groupsMem[i]);
		//ofLogNotice("ofxPresetsManager") << "ofParameterGroup: " << i << "\n" << ofToString(groupsMem[i]);

		if (bLoaded)
		{
			settingsArray[i] = settings;
		}
#ifdef INCLUDE_DEBUG_ERRORS
		else if (!bLoaded)
		{
			errorsDEBUG.addError("ofxPresetsManager " + gui_LabelName, "load_AllKit_ToMemory() - NOT FOUND OR EMPTY XML FILES:", pathComplete);
			//TODO:
			//maybe we should create not found or empty xml presets with an empty preset..
		}
#endif
	}

	ofLogNotice("ofxPresetsManager") << "-------------------------------------------------------------------------------------------------------";

	//debug params
	bool bDEBUG = false;
	if (bDEBUG)
	{
		for (int i = 0; i < NUM_OF_PRESETS; i++)
		{
			ofLogNotice("ofxPresetsManager") << "settingsArray[" << i << "]\n" << ofToString(settingsArray[i].toString());
		}
	}

	////debug params
	//for (int i = 0; i < NUM_OF_PRESETS; i++)
	//{
	//	ofLogNotice("ofxPresetsManager") << "groupsMem[" << i << "]\n" << ofToString(groupsMem[i]);
	//}
}

////--------------------------------------------------------------
//void ofxPresetsManager::addGroup_TARGET(ofParameterGroup &g)
//{
//	ofLogNotice("ofxPresetsManager") << "addGroup_TARGET:" << "" << endl;
//
//	group_TARGET = g;
//}

//--------------------------------------------------------------
void ofxPresetsManager::exit()
{
	ofLogVerbose("ofxPresetsManager") << "exit()";

	DISABLE_CALLBACKS = true;//?

	//autosave PRESET_selected preset on exit
	if (autoSave)// && autoLoad)
	{
		doSave(PRESET_selected - 1);
		//doSave2(PRESET2_selected - 1);
	}

	//TODO:
	//app settings
	save_ControlSettings();//crashes?

	//destroy callbacks
	removeKeysListeners();

	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxPresetsManager::Changed_Params_Control);

	//-

	//TODO: move above
	//MODE B: direct from memory
	if (MODE_MemoryLive && autoSave)
	{
		save_AllKit_FromMemory();
	}

	//--

	ofRemoveListener(ofEvents().update, this, &ofxPresetsManager::update);
	ofRemoveListener(ofEvents().draw, this, &ofxPresetsManager::draw);
}



//--

#pragma mark - BROWSER_GUI

#ifdef INCLUDE_FILE_BROWSER_IM_GUI

//* browser
//--------------------------------------------------------------
bool ofxPresetsManager::browser_draw_ImGui()
{
	auto mainSettings = ofxImGui::Settings();//?

	this->gui_Browser.begin();

	if (SHOW_ImGui)//not useful..
	{
		//mouse over gui system
		bMouseOver_Changed = false;
		bImGui_mouseOver = false;

		//-

		bImGui_mouseOver = browser_draw_ImGui_Browser();

		//-

		if (bImGui_mouseOver != bImGui_mouseOver_PRE)
		{
			bImGui_mouseOver_PRE = bImGui_mouseOver;
			bMouseOver_Changed = true;
		}

		if (bMouseOver_Changed)
		{
			ofLogVerbose("ofxPresetsManager") << "bImGui_mouseOver: " << (bImGui_mouseOver ? "INSIDE" : "OUTSIDE");
		}
	}

	this->gui_Browser.end();

	return mainSettings.mouseOverGui;
}

//TODO:
//--------------------------------------------------------------
void ofxPresetsManager::browser_draw_ImGui_User(ofxImGui::Settings &settings)
{
	ImGui::SetNextItemWidth(100);
	ofxImGui::AddParameter(this->MODE_Browser_NewPreset);
	//ofxImGui::AddParameter(this->SHOW_Browser);//this

	//-

	//other usefull params from hidden internal panel
	ofxImGui::AddParameter(this->SHOW_Gui_Internal);
	ofxImGui::AddParameter(this->SHOW_ClickPanel);
	ofxImGui::AddParameter(this->SHOW_Browser);
	ofxImGui::AddParameter(this->autoSave);
	//ofxImGui::AddGroup(this->params_Options, settings);//grouped
	//ofxImGui::AddParameter(this->MODE_MemoryLive);
	//ofxImGui::AddParameter(this->PRESET_selected);

	//-

	// send/save browsed preset to favorites

	if (ImGui::Button("TO LIVE"))
	{
		ofLogNotice("ofxPresetsManager") << "TO LIVE: SAVE browsed Preset: " << browser_PresetName;
		ofLogNotice("ofxPresetsManager") << "to favorites Preset: [" << PRESET_selected << "]";

		if (MODE_Browser_NewPreset)
		{
			save(PRESET_selected - 1, 0);
		}
	}

	//-

	//get/copy all [8] presets from favs and send/save to browser folder ("archive")

	ImGui::SameLine();
	if (ImGui::Button("FROM FAVS"))
	{
		ofLogNotice("ofxPresetsManager") << "FROM FAVS";
		doGetFavsFromBrowser();
	}

	//-

	//helpers

	if (ImGui::Button("CLONE ALL"))
	{
		bCloneAll = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("CLONE >"))
	{
		bCloneRight = true;
	}

	//-

	ImGui::SetNextItemWidth(100);
	ofxImGui::AddParameter(this->PRESET_selected);//TODO: customize width to make stretch

	//-

	//randomizer
	ofxImGui::AddParameter(this->ENABLE_RandomizeTimer);
	if (ImGui::TreeNode("RANDOMIZER"))
	{
		if (ImGui::Button("RANDOMIZE"))
		{
			bRandomize = true;
		}

		ImGui::SetNextItemWidth(100);
		//ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5f);
		ofxImGui::AddParameter(this->randomizeSpeedF);

		ImGui::TreePop();
	}
}

//--------------------------------------------------------------
bool ofxPresetsManager::browser_draw_ImGui_Browser()
{
	//-

	ofVec2f pos, size;
	pos = ofVec2f(ImGui_Position.get().x, ImGui_Position.get().y);
	size = ofVec2f(ImGui_Size.get().x, ImGui_Size.get().y);

	auto mainSettings = ofxImGui::Settings();
	mainSettings.windowPos = pos;
	//mainSettings.windowSize = size;

	ImGui::SetNextWindowPos(ofVec2f(pos.x, pos.y), ImGuiCond_Always);

	//-

	//NOTE:
	//seems that window (not tree) is required to allow text input stills inside box...

	string _name;
	_name = groups[0].getName();
	//_name = "PRESET MANAGER";
	//_name = "PRESET MANAGER  [" + groups[0].getName() + "]";

	bool _collapse = false;
	if (ofxImGui::BeginWindow(_name, mainSettings, _collapse))
	{
		ImGui::Text("PRESET MANAGER");

		//--

		browser_draw_ImGui_User(mainSettings);

		//-- 

		{
			int numFilePresets = fileNames.size();

			//0. error when no files detected

			if (numFilePresets == 0)
			{
				int n = 30;
				float a = ofMap(ofGetFrameNum() % n, 0, n, 0.f, 1.f);

				ImGui::PushID(1);
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5, 0.0f, 0.5f, a));

				ImGui::Text("DIR NOT FOUND!");

				ImGui::PopStyleColor(1);
				ImGui::PopID();
			}

			else if (numFilePresets > 0)
			{
				//1. arrow buttons

				static int counter = currentFile;
				float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

				ImGui::PushButtonRepeat(true);

				//--

				//1.1 prev

				if (ImGui::ArrowButton("##left", ImGuiDir_Left))
				{
					if (counter > 0)
					{
						counter--;
						currentFile = counter;
						if (currentFile < files.size())
						{
							browser_PresetName = fileNames[currentFile];
							ofLogNotice("ofxPresetsManager") << "ARROW: browser_PresetName: [" + ofToString(currentFile) + "] " << browser_PresetName;

							//if (MODE_Browser_NewPreset)
							ofLogNotice("ofxPresetsManager") << "LOAD" << endl;
							ofLogNotice("ofxPresetsManager") << "Preset Name: " << browser_PresetName;
							browser_PresetLoad(browser_PresetName);
						}
					}
				}

				//--

				//1.2 next

				ImGui::SameLine(0.0f, spacing);
				if (ImGui::ArrowButton("##right", ImGuiDir_Right))
				{
					if (counter < files.size() - 1)
					{
						counter++;
						currentFile = counter;
						if (currentFile < files.size())
						{
							browser_PresetName = fileNames[currentFile];
							ofLogNotice("ofxPresetsManager") << "ARROW: browser_PresetName: [" + ofToString(currentFile) + "] " << browser_PresetName;

							//if (MODE_Browser_NewPreset)
							ofLogNotice("ofxPresetsManager") << "LOAD Preset Name: " << browser_PresetName;
							browser_PresetLoad(browser_PresetName);
						}
					}
				}

				ImGui::PopButtonRepeat();

				//--

				//1.3 text preview current preset number to total. (1/4)

				ImGui::SameLine();
				//if (numFilePresets > 0)
				{
					ImGui::Text("%d/%d", currentFile + 1, numFilePresets);
					//ImGui::Text("%d/%d", 0, numFilePresets);
				}
			}


			//--

			//3. scrollable list

			if (!fileNames.empty())
			{
				int currentFileIndex = currentFile;
				//ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.9f);
				ImGui::SetNextItemWidth(170);
				if (ofxImGui::VectorCombo(" ", &currentFileIndex, fileNames))
				{
					ofLogNotice("ofxPresetsManager") << "Preset Index: " << ofToString(currentFileIndex);
					if (currentFileIndex < fileNames.size())
					{
						currentFile = currentFileIndex;
						browser_PresetName = fileNames[currentFile];

						ofLogNotice("ofxPresetsManager") << "LOAD Preset Name: " << browser_PresetName;
						browser_PresetLoad(browser_PresetName);
					}
				}
			}

			//--

			//4.2 update

			if (ImGui::Button("UPDATE"))
			{
				//browser_PresetName = textInput_temp;
				//ofLogNotice("ofxPresetsManager") << "UPDATE Preset Name: " << browser_PresetName << endl;

				//0. get filename of selected
				string _currName = files[currentFile].getBaseName();
				ofLogNotice("ofxPresetsManager") << "UPDATE Preset Name: " << _currName;

				//1. delete old file
				files[currentFile].remove();

				//2. save "ovewrite"
				browser_PresetSave(_currName);

				//-

				//workflow

				//3. refresh files
				browser_FilesRefresh();

				//4. reselect last save preset (bc directory sorting changes)
				ofLogNotice("ofxPresetsManager") << "Reload last updated preset:";
				int iNew = -1;//search index for filename
				for (size_t i = 0; i < files.size(); i++)
				{
					string n = files[i].getBaseName();
					if (n == _currName)
					{
						iNew = i;
					}
				}
				if (iNew != -1)
				{
					ofLogNotice("ofxPresetsManager") << "Index [" << iNew << "] " << files[iNew].getBaseName();
					currentFile = iNew;
					browser_PresetName = fileNames[currentFile];
					browser_PresetLoad(browser_PresetName);
				}
				else
				{
					ofLogError("ofxPresetsManager") << "Not found! Bad Index [" << iNew << "]";
				}
			}

			//-

			//4.3 reload

			ImGui::SameLine();
			if (ImGui::Button("RELOAD"))
			{
				ofLogNotice("ofxPresetsManager") << "RELOAD Preset Name: " << browser_PresetName;
				browser_PresetLoad(browser_PresetName);
			}

			//-

			//4.4 delete

			ImGui::SameLine();
			if (ImGui::Button("DELETE"))//current preset
			{
				ofLogNotice("ofxPresetsManager") << "DELETE Preset Name: " << browser_PresetName;
				ofLogNotice("ofxPresetsManager") << "filepath: " << files[currentFile].getAbsolutePath();

				//1. delete file
				files[currentFile].remove();

				//-

				//workflow

				//2. refresh files
				bool b = browser_FilesRefresh();
				if (b)
				{
					currentFile = 0;
					browser_PresetName = fileNames[currentFile];
					browser_PresetLoad(browser_PresetName);
					//ofLogNotice("ofxPresetsManager") << "Load preset [0] " << browser_PresetName;
				}
				else
				{
					ofLogError("ofxPresetsManager") << "Error listing directory!";
				}
			}

			//----

			//5. second panel (new preset)

			if (MODE_Browser_NewPreset)
			{
				//5.1 new preset name

				//ImGui::Text("NEW PRESET:");

				//-

				//5.2 user input text

				//loaded string into char array
				char tab[32];
				strncpy(tab, textInput_New.c_str(), sizeof(tab));
				tab[sizeof(tab) - 1] = 0;

				if (ImGui::InputText("", tab, IM_ARRAYSIZE(tab)))
				{
					ofLogNotice("ofxPresetsManager") << "InputText [tab]:" << ofToString(tab) << endl;
					textInput_New = ofToString(tab);
					ofLogNotice("ofxPresetsManager") << "textInput_New:" << textInput_New << endl;

					//bBlink = true;//not workind. we like to blink when mouse_on_text_input
				}

				//--

				//5.3 save new

				//workflow

				//blink when it's editing a new preset..
				//TODO:
				bool bBlink;
				bBlink = true;
				if (bBlink)
				{
					//ImGui::PushID(1);
					int n = 30;
					float a = ofMap(ofGetFrameNum() % n, 0, n, 0.2f, 0.5f);
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5, 0.0f, 0.5f, a));
				}

				if (ImGui::Button("SAVE"))
				{
					ofLogNotice("ofxPresetsManager") << "textInput_New: " << textInput_New << endl;

					//1. save
					browser_PresetSave(textInput_New);

					//-

					//workflow

					//2. disable new preset mode
					MODE_Browser_NewPreset = false;

					//3. refresh files
					browser_FilesRefresh();

					//4. reselect last save preset (bc directory sorting changes)
					ofLogNotice("ofxPresetsManager") << "Reload last saved preset:";
					int iNew = -1;
					for (size_t i = 0; i < files.size(); i++)
					{
						string n = files[i].getBaseName();
						if (n == textInput_New)
						{
							iNew = i;
						}
					}
					ofLogNotice("ofxPresetsManager") << "Index [" << iNew << "] " << files[iNew].getBaseName();
					currentFile = iNew;
					browser_PresetLoad(textInput_New);
				}

				if (bBlink)
				{
					ImGui::PopStyleColor(1);
				}
			}
		}
	}
	ofxImGui::EndWindow(mainSettings);

	return mainSettings.mouseOverGui;
}

//--------------------------------------------------------------
void ofxPresetsManager::browser_PresetSave(string name)//without xml extension nor path
{
	ofLogNotice("ofxPresetsManager") << "browser_PresetSave: \t\t\t\t" << name << ".xml";

	ofXml settings;
	ofSerialize(settings, groups[0]);

	string _path = path_GLOBAL_Folder + "/" + path_PresetsFolder + "/" + name + ".xml";
	settings.save(_path);
	ofLogNotice("ofxPresetsManager") << "File: " << _path;

	//-

	//ofLogVerbose("ofxPresetsManager") << "DONE_save";
	//DONE_save = true;//TODO:?
}

//--------------------------------------------------------------
void ofxPresetsManager::browser_PresetLoad(string name)//without xml extension nor path
{
	ofLogNotice("ofxPresetsManager") << "browser_PresetLoad: \t\t\t\t" << name << ".xml";

	ofXml settings;
	string _path = path_GLOBAL_Folder + "/" + path_PresetsFolder + "/" + name + ".xml";
	settings.load(_path);
	ofLogNotice("ofxPresetsManager") << "File: " << _path;

	ofDeserialize(settings, groups[0]);

	//-

	//ofLogVerbose("ofxPresetsManager") << "DONE_load";
	//DONE_load = true;//TODO:?
}

//--------------------------------------------------------------
void ofxPresetsManager::browser_Setup()
{
	//font customize 
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	string _name = "overpass-mono-bold.otf";
	string _path = path_GLOBAL_Folder + "/fonts/" + _name;
	io.Fonts->AddFontFromFileTTF(&ofToDataPath(_path)[0], 13.0f);

	//-

	gui_Browser.setup();
	browser_ImGui_theme();
	SHOW_ImGui = true;//not useful
	bImGui_mouseOver.set("mouseOverGui", false);

	//load files structure directory
	bool bLoaded = browser_FilesRefresh();

	//workflow
	if (bLoaded)
	{
		//load first preset
		if (fileNames.size() > 0)
		{
			currentFile = 0;

			browser_PresetName = fileNames[currentFile];
			browser_PresetLoad(browser_PresetName);
		}
	}

}

//--------------------------------------------------------------
bool ofxPresetsManager::browser_FilesRefresh()
{
	ofLogNotice("ofxPresetsManager") << "browser_FilesRefresh()";

	string _path;
	_path = path_GLOBAL_Folder + "/" + path_PresetsFolder;

	//-

	//TODO:
	//if (!bCustomBrowserPath)
	//	_path = path_GLOBAL_Folder + "/" + path_PresetsFolder;
	//else
	//	_path = path_BrowserPathFree;
	//ofLogNotice("ofxPresetsManager") << "Path: " << _path;

	ofDirectory dataDirectory(ofToDataPath(_path, true));

	//-

	//TODO:
	//make above code to function
	//create folder if do not exist!
	if (!dataDirectory.isDirectory())
	{
		ofLogError("ofxPresetsManager") << "FOLDER DOES NOT EXIST!";
		bool b = dataDirectory.createDirectory(ofToDataPath(_path, true));
		if (b)
			ofLogNotice("ofxPresetsManager") << "CREATED FOLDER: " << _path;
		else
			ofLogError("ofxPresetsManager") << "UNABLE TO CREATE FOLDER: " << _path;

	}

	//-

	//ofDirectory dataDirectory;
	//if (!bCustomBrowserPath)
	//	dataDirectory = ofDirectory(ofToDataPath(_path, true));
	//else
	//{
	//	ofDisableDataPath();
	//	dataDirectory = ofDirectory(path_BrowserPathFree);
	//	ofEnableDataPath();
	//	//[error] ofDirectory: listDir:() source directory does not exist : ""&"F:\openFrameworks\addons\ofxPresetsManager\2_presetsManager\bin\data\ofxPresetsManager\presets&"\""
	//	
	//	//"F:\openFrameworks\addons\ofxPresetsManager\2_presetsManager\bin\data\ofxPresetsManager\presets"
	//	//dataDirectory = ofDirectory("F:\openFrameworks\addons\ofxPresetsManager\2_presetsManager\bin\data\ofxPresetsManager\presets");
	//}

	//clear files and filenames vectors
	files.clear();
	fileNames.clear();

	//load all folder files in one call
	files = dataDirectory.getFiles();

	ofLogNotice("ofxPresetsManager") << "Preset files:";
	for (size_t i = 0; i < files.size(); i++)
	{
		fileNames.push_back(files[i].getBaseName());
		ofLogNotice("ofxPresetsManager") << "[" << i << "] " << files[i].getBaseName();
	}

	//-

	//TODO
	//void to go to 1st...

	//1. load same position preset
	//if preset is deleted goes to nextone..
	//should check names because sorting changes..
	if (fileNames.size() > 0)
	{
		//TODO:
		//disabled autoload preset. just list files!

		////workflow
		////clamp selected file to folder limits. select last one (?)
		//if (currentFile > fileNames.size() - 1)
		//	currentFile = fileNames.size() - 1;
		////else if (currentFile > fileNames.size() - 1)

		//browser_PresetName = fileNames[currentFile];
		//browser_PresetLoad(browser_PresetName);

		bFilesError = false;
	}
	else
	{
		ofLogError("ofxPresetsManager") << "BROWSER: FILES NOT FOUND ON FOLDER!";
		bFilesError = true;

		//TODO:
		//disable custom path bc error
		ofLogError("ofxPresetsManager") << "Disable custom path: " << path_BrowserPathFree;
		bCustomBrowserPath = false;

		ofLogError("ofxPresetsManager") << "------------------------";
	}

	//workflow
	////2. always goes to 1st preset 0
	////that's because saving re sort the files
	////and we don't know the position of last saved preset..
	//if (fileNames.size() > 0)
	//{
	//   currentFile = 0;
	//   browser_PresetName = fileNames[currentFile];
	//   browser_PresetLoad(browser_PresetName);
	//}
	//else
	//{
	//   ofLogError("ofxColorManager") << "NOT FOUND ANY FILE PRESET!";
	//}

	return !bFilesError;//true if ok
}

//--------------------------------------------------------------
void ofxPresetsManager::browser_ImGui_theme()
{
	//must be done after setup the gui

	ImGuiStyle *style = &ImGui::GetStyle();

	style->FramePadding = ImVec2(4, 2);
	//style->WindowMinSize = ImVec2(160, 65);
	//style->ItemSpacing = ImVec2(6, 2);
	style->ItemSpacing = ImVec2(6, 4);
	style->ItemInnerSpacing = ImVec2(6, 4);
	style->Alpha = 1.0f;
	style->WindowRounding = 0.0f;
	style->FrameRounding = 0.0f;
	style->IndentSpacing = 6.0f;
	style->ItemInnerSpacing = ImVec2(2, 4);
	style->ColumnsMinSpacing = 50.0f;
	style->GrabMinSize = 14.0f;
	style->GrabRounding = 0.0f;
	style->ScrollbarSize = 12.0f;
	style->ScrollbarRounding = 0.0f;

	//my dark theme

	//ImVec4* colors = ImGui::GetStyle().Colors;
	style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.54f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.26f, 0.68f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.67f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.12f, 0.11f, 0.11f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.75f, 0.77f, 0.79f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.79f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.29f, 0.29f, 0.29f, 0.78f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.44f, 0.44f, 0.44f, 0.25f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.68f, 0.68f, 0.68f, 0.67f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.68f, 0.68f, 0.68f, 0.95f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.81f, 0.79f, 0.79f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
	style->Colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
	style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
	style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	//   //2. defaut dark theme exported
	//   style->WindowMinSize = ImVec2(160, 65);
	//   style->FramePadding = ImVec2(4, 2);
	//   style->ItemSpacing = ImVec2(6, 2);
	//   style->ItemInnerSpacing = ImVec2(6, 4);
	//   style->Alpha = 1.0f;
	//   style->WindowRounding = 0.0f;
	//   style->FrameRounding = 0.0f;
	//   style->IndentSpacing = 6.0f;
	//   style->ItemInnerSpacing = ImVec2(2, 4);
	//   style->ColumnsMinSpacing = 50.0f;
	//   style->GrabMinSize = 14.0f;
	//   style->GrabRounding = 0.0f;
	//   style->ScrollbarSize = 12.0f;
	//   style->ScrollbarRounding = 0.0f;
}

//TODO:
////--------------------------------------------------------------
//void ofxPresetsManager::gui_loadFromFile(const std::string &filename, ofAbstractParameter &parameter)
//{
//	ofXml xml;
//	xml.load(filename);
//	ofDeserialize(xml, parameter);
//}
//
////--------------------------------------------------------------
//void ofxPresetsManager::gui_saveToFile(const std::string &filename, ofAbstractParameter &parameter)
//{
//	ofXml xml;
//	ofSerialize(xml, parameter);
//	xml.save(filename);
//}
//
////--------------------------------------------------------------
//void ofxPresetsManager::gui_SaveAsSettings()
//{
//
//}

////--------------------------------------------------------------
//static void ShowHelpMarker(const char *desc)
//{
//	ImGui::TextDisabled("(?)");
//	if (ImGui::IsItemHovered())
//	{
//		ImGui::BeginTooltip();
//		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
//		ImGui::TextUnformatted(desc);
//		ImGui::PopTextWrapPos();
//		ImGui::EndTooltip();
//	}
//}

#endif


//SNIPPET - GIST:
////NESTED GROUPS
//for (auto parameter : paramsFull)
//{
//    //Group.
//    auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(parameter);
//    if (parameterGroup)
//    {
//        //Recurse through contents.
//        ofxImGui::AddGroup(*parameterGroup, mainSettings);
//        continue;
//    }
//}


////--------------------------------------------------------------
//void ofxPresetsManager::browser_draw_ImGui_MenuBar()
//{
//	if (ImGui::BeginMainMenuBar())
//	{
//		if (ImGui::BeginMenu("File"))
//		{
//			browser_draw_ImGui_MenuFile();
//			ImGui::EndMenu();
//		}
//		ImGui::EndMainMenuBar();
//	}
//}
//
////--------------------------------------------------------------
//void ofxPresetsManager::browser_draw_ImGui_MenuFile()
//{
//	if (ImGui::MenuItem("New"))
//	{
//	}
//	if (ImGui::MenuItem("Open", "l"))
//	{
//		//gui_loadFromFile("settings.xml", params_Control);
//	}
//	if (ImGui::MenuItem("Save", "s"))
//	{
//		//gui_saveToFile("settings.xml", params_Control);
//	}
//	if (ImGui::MenuItem("Save As.."))
//	{
//		//gui_SaveAsSettings();
//	}
//	ImGui::Separator();
//	ImGui::Separator();
//	if (ImGui::MenuItem("Quit", "ESQ"))
//	{
//	}
//}

////TODO: 
////DEBUG:
////--------------------------------------------------------------
//void ofxPresetsManager::groupDebug(ofParameterGroup &group)
//{
//	string str;
//	int x = 200;
//	int y = 100;
//	int pad = 20;
//	int i = 0;
//	int ig = 0;
//
//	for (auto parameter : group)
//	{
//		//Group.
//		auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(parameter);
//		if (parameterGroup)
//		{
//			//Recurse through contents.
//			//ofxImGui::AddGroup(*parameterGroup, settings);
//			groupDebug(*parameterGroup);
//
//			ig++;
//			str = "group : [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x + ig * 300, y + pad * i++);
//			continue;
//		}
//
//		//Parameter, try everything we know how to handle.
//#if OF_VERSION_MINOR >= 10
//		auto parameterVec2f = std::dynamic_pointer_cast<ofParameter<glm::vec2>>(parameter);
//		if (parameterVec2f)
//		{
//			//ofxImGui::AddParameter(*parameterVec2f);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//		auto parameterVec3f = std::dynamic_pointer_cast<ofParameter<glm::vec3>>(parameter);
//		if (parameterVec3f)
//		{
//			//ofxImGui::AddParameter(*parameterVec3f);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//		auto parameterVec4f = std::dynamic_pointer_cast<ofParameter<glm::vec4>>(parameter);
//		if (parameterVec4f)
//		{
//			//ofxImGui::AddParameter(*parameterVec4f);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//#endif
//		auto parameterOfVec2f = std::dynamic_pointer_cast<ofParameter<ofVec2f>>(parameter);
//		if (parameterOfVec2f)
//		{
//			//ofxImGui::AddParameter(*parameterOfVec2f);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//		auto parameterOfVec3f = std::dynamic_pointer_cast<ofParameter<ofVec3f>>(parameter);
//		if (parameterOfVec3f)
//		{
//			//ofxImGui::AddParameter(*parameterOfVec3f);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//		auto parameterOfVec4f = std::dynamic_pointer_cast<ofParameter<ofVec4f>>(parameter);
//		if (parameterOfVec4f)
//		{
//			//ofxImGui::AddParameter(*parameterOfVec4f);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//		auto parameterFloatColor = std::dynamic_pointer_cast<ofParameter<ofFloatColor>>(parameter);
//		if (parameterFloatColor)
//		{
//			//ofxImGui::AddParameter(*parameterFloatColor);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//		auto parameterFloat = std::dynamic_pointer_cast<ofParameter<float>>(parameter);
//		if (parameterFloat)
//		{
//			//ofxImGui::AddParameter(*parameterFloat);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//		auto parameterInt = std::dynamic_pointer_cast<ofParameter<int>>(parameter);
//		if (parameterInt)
//		{
//			//ofxImGui::AddParameter(*parameterInt);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//		auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(parameter);
//		if (parameterBool)
//		{
//			//ofxImGui::AddParameter(*parameterBool);
//			str = "parameter : [" + ofToString(ig) + "] [" + ofToString(i) + "] " + ofToString(parameter->getName());
//			ofDrawBitmapStringHighlight(str, x, y + pad * i++);
//			continue;
//		}
//
//		ofLogWarning(__FUNCTION__) << "Could not create GUI element for parameter " << parameter->getName();
//	}
//}