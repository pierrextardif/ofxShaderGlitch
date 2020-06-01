dependencies:
ofxSerializer

description:

The addon auto creates a json file that is auto saved and loaded.
\data\ofxWindowApp\AppWindow.json

position
size: height, width
window_mode


usage:

ofApp.h:

#include "ofxWindowApp.h"

class ofApp : public ofBaseApp
{

public:

	ofxWindowApp WindowApp;
	
	..