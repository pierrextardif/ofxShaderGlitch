
/// ofxPresetsManager.h 
///
/// ofxPresetsManager 
/// by moebiussurfing, 2020.
///
/// this addon is based in the original ofxGuiPresetSelector addon 
/// by Nicola Pisanti, MIT License, 2016
/// https://github.com/npisanti/ofxGuiPresetSelector
/// all modifications and new features by moebiussurfing
/// my idea is to allow use ofParameterGroup's as managed content instead of ofxPanel

///--

///TODO:
///	+	could make tween when changing params (?)
///	+	check if if destructor is calling exit makes the app crash. for now, exit() must be called by manually
#pragma once


///----------------------------------------------
///
///	DEFINES
///
///browser system
#define INCLUDE_FILE_BROWSER_IM_GUI
///BUG: seems to make exceptions when multiple ImGui/ofxPresetsManager instances...
///
///debug errors
#define INCLUDE_DEBUG_ERRORS
///
///disable save settings
///#define DEBUG_BLOCK_SAVE_SETTINGS//enable this bc sometimes there's crashes on exit
///
///measure performance ofxTimeMeasurements
///#define TIME_SAMPLE_MEASURES 
///
///----------------------------------------------


#include "ofMain.h"

//--

//internal control
#include "ofxGui.h"

//--

//browser system
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
#include "ofxImGui.h"
#endif

//--

//optional to debug not located files or others
#ifdef INCLUDE_DEBUG_ERRORS
#include "ofxDEBUG_errors.h"
#endif

//--

//optional to debug performance or delay when loading files or presets on hd or memory modes
#ifdef TIME_SAMPLE_MEASURES
#include "ofxTimeMeasurements.h"
#else
#define TS_START_NIF
#define TS_STOP_NIF
#define TS_START_ACC
#define TS_STOP_ACC
#define TS_START
#define TS_STOP
#define TSGL_START
#define TSGL_STOP
#endif

//--

//-------------------------------

#pragma mark - DEFINE_DATA_TYPES

//-

#define NUM_OF_PRESETS 8

//only one it's implemented! can't add more than one parameter group!
#define NUM_MAX_GUIS 1

//---

class ofxPresetsManager : public ofBaseApp
{

private:

#ifdef INCLUDE_DEBUG_ERRORS
	ofxDEBUG_errors errorsDEBUG;
#endif

	//-

	//settings paths
	std::string groupName;//get from ofParameterGroup name
	//TODO:
	//std::string groupName2;//get from ofParameterGroup name

	//all folder names must go without '/'
	std::string path_GLOBAL_Folder;//top parent folder for all other subfolders
	std::string path_Control;//path for app state session settings
	std::string path_Kit_Folder;//path for kit of favorite presets. live kit
	std::string path_PresetsFolder;//path for browse other presets. archive kit
	std::string path_Prefix;//to add to file names

	//TODO:
	//must store settings
	//customize browser path-free (out of global path)
	bool bCustomBrowserPath = false;
	std::string path_BrowserPathFree = "";

	std::string browser_PresetName;

	std::string gui_LabelName = "NO_NAME";//default gui panel name

	//-

private:

	//app settings

	//this loads selected preset, autosave/autoload...gui states
	void load_ControlSettings();
	//public://TEST crash?
	void save_ControlSettings();

	//-

private:

	ofxPanel gui_InternalControl;
	ofParameter<bool> MODE_MemoryLive;
	ofParameter<bool> loadToMemory;
	ofParameter<bool> saveFromMemory;
	void load_AllKit_ToMemory();
	void save_AllKit_FromMemory();

	//randomizer
	//ofParameterGroup params_Randomizer;
	ofParameter<bool> bRandomize;
	ofParameter<bool> ENABLE_RandomizeTimer;
	ofParameter<float> randomizeSpeedF;
	int randomizeSpeed;//real time dureation
	uint32_t randomizerTimer;
	int randomize_MAX_DURATION = 5000;

	////TODO:
	//ofParameterGroup group_TARGET;
	//void addGroup_TARGET(ofParameterGroup &g);	
	//vector<ofParameterGroup> groupsMem;

	//data
	ofXml settingsArray[NUM_OF_PRESETS];

	//--

#pragma mark - OF

public:

	ofxPresetsManager();
	~ofxPresetsManager();

	//-

#pragma mark - API

public:

	//--

	//must be called after params group has been added!
	void setup();
	void setup(std::string name);//to set gui panel name header label

	void exit();

	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);

	void windowResized(int w, int h);

	//-

	void drawPresetClicker();//user clickeable box panel preset selector

	//-

#pragma mark - DIFFERENT_DATA_TYPES_METHODS

	void add(ofParameterGroup params, int numPresets = 8);//add a param group for preset saving
	void add(ofParameterGroup params, initializer_list<int> keysList);//adds and activate keys to switch

	//--

private:

#pragma mark - CALLBACKS

	bool DISABLE_CALLBACKS = true;//to avoid startup crashes and objects are not initialized properly
	bool bIsDoneLoad = false;

public:

	ofParameter<bool> DONE_load;//easy callback to know (in ofApp) that preset LOAD is done 
	ofParameter<bool> DONE_save;//easy callback to know (in ofApp) that preset SAVE is done

public:

	bool isDoneLoad()//easy callback to ofApp integration 
	{
		if (bIsDoneLoad)
		{
			bIsDoneLoad = false;
			return true;
		}
		return false;
	}

	////to check in update() as callback
	//bool isDoneLoad()
	//{
	//    bool isDone = false;
	//    if (DONE_load)
	//    {
	//        DONE_load = false;
	//        isDone = true;
	//    }
	//    else
	//    {
	//        isDone = false;
	//    }
	//    return isDone;
	//}

	//--

#pragma mark - API

public:

	//API

	//-

	void setPosition_DEBUG(int x, int y);

	float getPresetClicker_Width()
	{
		return clicker_cellSize * (keys[0].size() + 1);
	}
	float getPresetClicker_BoxSize()
	{
		return clicker_cellSize;
	}
	float getPresetClicker_Height()
	{
		return clicker_cellSize;
	}

	//-

	void setModeKeySave(int key);//set the key you have to hold for saving, default is OF_KEY_CONTROL
	void setModeKeySwap(int key);//set the key you have to hold for swap, default is OF_KEY_ALT

	//set keys active
	void setEnableKeys(bool active)
	{
		bKeys = active;
		ENABLE_Keys = active;
	}

	void setToggleEnableKeys()
	{
		ENABLE_Keys = !ENABLE_Keys;
		bKeys = ENABLE_Keys;
	}

	bool isKeysEnabled()
	{
		return ENABLE_Keys;
	}

	//-

	void load_Next()
	{
		PRESET_selected++;
		if (PRESET_selected > numPresets_OfFavorites)
		{
			PRESET_selected = numPresets_OfFavorites;
		}
	}

	void load_Previous()
	{
		PRESET_selected--;
		if (PRESET_selected < 1)
		{
			PRESET_selected = 1;
		}
	}

	void loadPreset(int p);//load preset by code from ofApp
	int getNumPresets()
	{
		return numPresets_OfFavorites;
	}

	int getCurrentPreset()//get index of selected preset
	{
		return PRESET_selected;
	}

	//-

	void setVisible_GroupName(bool b)//disabler for minimal design
	{
		SHOW_GroupName = b;
	}

	//--

	//randomizer
public:
	void setModeRandomizerPreset(bool b)
	{
		ENABLE_RandomizeTimer = b;
	}
	void setToggleRandomizerPreset()
	{
		ENABLE_RandomizeTimer = !ENABLE_RandomizeTimer;
	}
	void randomizeBang()
	{
		bRandomize = true;
	}

	//--

private:

	//draws group name into clicker boxes panel
	bool SHOW_GroupName = true;

	//--

	//used when preset has not changed but we like to retrig
	bool bMustTrig = false;

public:

	bool isMustTrig()//trig on select preset or not
	{
		if (bMustTrig)
		{
			bMustTrig = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	//-

	//browser
private:
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	//load presets from preset folder, not from favorites presets folders
	void browser_PresetLoad(string name);
	void browser_PresetSave(string name);
	bool browser_FilesRefresh();
#endif

	//-

#pragma mark - GUI

	//-

public:

	//API

	//BUG: 
	//workflow 
	//to solve auto load fail because the sorting of xml autoSave after preset selector tag
	void refresh()
	{
		windowResized(ofGetWidth(), ofGetHeight());

		////browser
		//browser_FilesRefresh();

		//-

		ofLogNotice("ofxPresetsManager") << "Refresh()";
		PRESET_selected_PRE = -1;
		PRESET_selected = PRESET_selected;
		ofLogNotice("ofxPresetsManager") << "Preset " << PRESET_selected;
	}

	void setVisible_Help(bool b)
	{
		debugClicker = b;
	}

#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	void setPosition_GUI_Browser(int x, int y)
	{
		ImGui_Position = ofVec2f(x, y);
	}
	void setSize_GUI_Browser(int w, int h)
	{
		ImGui_Size = ofVec2f(w, h);
	}
	void setVisible_GUI_Browser(bool b)
	{
		SHOW_Browser = b;
	}
#endif

	void setPosition_GUI_InternalControl(int x, int y)
	{
		guiPos_InternalControl = ofVec2f(x, y);
		gui_InternalControl.setPosition(guiPos_InternalControl.x, guiPos_InternalControl.y);
	}

	void setVisible_GUI_Internal(bool visible)
	{
		SHOW_Gui_Internal = visible;
	}

	bool isVisible_GUI_Internal()
	{
		return SHOW_Gui_Internal;
	}

	void setToggleVisible_GUI_Internal()
	{
		SHOW_Gui_Internal = !SHOW_Gui_Internal;
	}

	void setPosition_PresetClicker(int x, int y, int _cellSize)
	{
		clicker_Pos.x = x;
		clicker_Pos.y = y;
		clicker_cellSize = _cellSize;
	}

	void setVisible_PresetClicker(bool visible)
	{
		SHOW_ClickPanel = visible;
	}

	bool isVisible_PresetClicker()
	{
		return SHOW_ClickPanel;
	}

	//--

#pragma mark - SETTINGS

	void setPath_ControlSettings(string str)//main addon settings
	{
		path_Control = str;
	}

	void setPath_KitFolder(string folder);//path folder for favorite/live presets
	void setPath_PresetsFolder(string folder);//path folder for kit for the browser
	void setPath_GlobalFolder(string folder);//path for root container folder

	void setAutoLoad(bool b)
	{
		autoLoad = b;
	}

	void setAutoSave(bool b)
	{
		autoSave = b;
	}

	void setAutoSaveTimer(bool b)
	{
		bAutosaveTimer = b;
	}

	//--

public:

	//from 1 to 8. (indexed vector vars starts from 0)
	ofParameter<int> PRESET_selected;

private:

	int PRESET_selected_PRE = -1;

	//ofParameter<int> PRESET2_selected;
	//int selected2_PRE = -1;

	//--

public:

	//switch on or off the control with the keys
	void setToggleKeysControl(bool active);

	//-

private:

	//engine

	//save to a preset
	void save(int presetIndex, int guiIndex = 0);
	void save(int presetIndex, string guiName);

	//loads an already saved preset
	void load(int presetIndex, int guiIndex = 0);
	void load(int presetIndex, string guiName);

	//get the last loaded preset
	int getPresetIndex(int guiIndex) const;
	int getPresetIndex(string guiName) const;

	int getGuiIndex(string name) const;
	string getPresetName(string guiName, int presetIndex);

	//--

	ofParameter<bool> SHOW_Gui_Internal;

	bool debugClicker = true;//?

	//-

	//browser
#ifdef INCLUDE_FILE_BROWSER_IM_GUI

public:
	ofParameter<bool> bImGui_mouseOver;

private:
	void browser_Setup();

	ofxImGui::Gui gui_Browser;

	ofParameter<bool> MODE_Browser_NewPreset;

	bool SHOW_ImGui;
	//bool bImGui_mouseOver;
	bool bImGui_mouseOver_PRE;

	bool browser_draw_ImGui();
	bool browser_draw_ImGui_Browser();
	void browser_draw_ImGui_MenuBar();
	void browser_draw_ImGui_MenuFile();
	void browser_draw_ImGui_User(ofxImGui::Settings &settings);

	//TODO:
	//void gui_saveToFile(const std::string &filename, ofAbstractParameter &parameter);
	//void gui_loadFromFile(const std::string &filename, ofAbstractParameter &parameter);
	//void gui_SaveAsSettings();

	//layout
	void browser_ImGui_theme();
	//ofxImGui::Settings mainSettings;
	ofParameter<glm::vec2> ImGui_Position;//ImGui browser panel position. must move by gui!  
	ofParameter<glm::vec2> ImGui_Size;//not used yet

	////TODO: 
	////DEBUG:
	//void groupDebug(ofParameterGroup &group);

	//-

public:
	bool isMouseOver()
	{
		bool b;
		if (!SHOW_Browser) b = false;
		else b = bImGui_mouseOver;
		return b;
	}

	//bool isMouseOver_Changed()
	//{
	//	if (bMouseOver_Changed)
	//	{
	//		//bMouseOver_Changed = false;//reset
	//		return true;
	//	}
	//}

private:
	bool bMouseOver_Changed = false;

	//--

	//browser presets
	//bool MODE_newPreset = false;
	string textInput_New = "";//user input text

	//files
	std::vector<std::string> fileNames;
	std::vector<ofFile> files;
	int currentFile = 0;
	string textInput_temp = "";
	bool bFilesError = false;

	//--

	//helpers
public:
	void doGetFavsFromBrowser()
	{
		ofLogNotice("ofxPresetsManager") << "doGetFavsFromBrowser()";

		//browser path
		string browser_path;
		browser_path = path_GLOBAL_Folder + "/" + path_PresetsFolder;

		//browser number of files

		//iterate all presets
		for (int i = 0; i < numPresets_OfFavorites; i++)
		{
			std::string pathSrc;
			std::string pathDst;

			pathSrc = getPresetName(groups[0].getName(), i);
			boost::filesystem::path bPath(pathSrc);

			//string pathFolder = ofToString(bPath.parent_path());
			string fileName = ofToString(bPath.filename().generic_string());
			pathDst = browser_path + "/" + fileName;

			ofLogNotice("ofxPresetsManager") << "pathSrc: " << pathSrc;
			ofLogNotice("ofxPresetsManager") << "pathDst: " << pathDst;

			ofFile file;
			file.copyFromTo(pathSrc, pathDst, true, true);//relative, overwrite

		}

		//--

		//refresh files
		browser_FilesRefresh();
	}

#endif//end browser

	//----


	//layout
	ofVec2f guiPos_InternalControl = ofVec2f(500, 500);
	int clicker_cellSize = 80;
	ofVec2f clicker_Pos = ofVec2f(500, 500);

	//--

	//A. ofParameterGroup
	std::vector<ofParameterGroup> groups;
	//to store multiple group targets. 
	//when using only one ofParameterGroup, there's only one group element!
	//ofParameterGroup params_gui;

	//--

	std::vector<int> lastIndices;//? this seems to be the last selected of any group(?)
	//TODO:
	//lastIndices it's the gui box clicked only, not important.. ?

	std::vector<int> presets;//? this seems to be the number of presets each group(?)

	//--

	void addKeysListeners();
	void removeKeysListeners();

	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);

	bool bKeys;//enabled keys
	vector<vector<int>> keys;//trigger keys for all presets
	bool keysNotActivated;

	//save
	int modeKeySave;//save mod key
	bool bKeySave;//save mod key state

	//TODO:
	//swap selected preset swith the clicked (i.e: 4 -> 7  &  4 <- 7)
	int modKeySwap;//swap mod key 
	bool bKeySwap;//swap mod key state

	bool lastMouseButtonState;
	void mousePressed(int x, int y);

	std::vector<int> newIndices;//? this seems to be the number of the groups(? )

	int numPresets_OfFavorites;//amount of box-clickable handled presets on current favorites/kit [8]

	//---

	//helpers

	void doCloneRight(int pIndex);//clone from selected preset to all others to the right
	void doCloneAll();//clone all presets from the current selected

	void doLoad(int pIndex);//engine loader. not used? bc bug on bLoad trigger..
	void doSave(int pIndex);//engine saver starting at 0

	//TODO:
	//for multiple groups new feature..
	//void doSave2(int pIndex);

	//-

	//font to label clicker boxes
	ofTrueTypeFont myFont;
	string myTTF;//gui font for all gui theme
	int sizeTTF;

	//-

	//void groupDebug(ofParameterGroup &group);

	//-

	//parameters

	//-

	//control panel settings/states

	void Changed_Params_Control(ofAbstractParameter &e);

	//-

	//control presetsManager params
	//to select presets, clone, save..

	ofParameterGroup params_Control;
	ofParameter<bool> SHOW_ClickPanel;
#ifdef INCLUDE_FILE_BROWSER_IM_GUI
	ofParameter<bool> SHOW_MenuTopBar;
	ofParameter<bool> SHOW_Browser;
#endif

	ofParameter<bool> bSave;
	//ofParameter<bool> bLoad;//buggy

	ofParameter<bool> autoSave;
	ofParameter<bool> autoLoad;

	ofParameter<bool> bCloneRight;
	ofParameter<bool> bCloneAll;

	//internal groups
	ofParameterGroup params_Favorites;
	ofParameterGroup params_Gui;
	ofParameterGroup params_Options;
	ofParameterGroup params_Tools;

	ofParameter<glm::vec2> Gui_Internal_Position;
	ofParameter<bool> ENABLE_Keys;

	//--

	//timer autosave
	ofParameter<bool> bAutosaveTimer;
	uint64_t timerLast_Autosave = 0;
	int timeToAutosave = 9000;

	//bool DISABLE_Callbacks = false;
	//updating some params before save will trigs also the group callbacks
	//so we disable this callbacks just in case params updatings are required
	//in this case we will need to update gui position param

	//--

	bool ENABLE_KeysArrowBrowse = true;//allow browse by arrows keys by default

public:

	void setEnableKeysArrowBrowse(bool b)
	{
		ENABLE_KeysArrowBrowse = b;
	}
};
