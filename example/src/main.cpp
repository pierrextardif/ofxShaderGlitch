#include "ofMain.h"
#include "ofApp.h"
#ifdef TARGET_OPENGLES
#include "ofGLES2Renderer.h"
#endif
//========================================================================
int main( ){

	ofSetLogLevel(OF_LOG_VERBOSE);
	#ifdef TARGET_OPENGLES
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLES2Renderer()));
	#endif
	ofSetupOpenGL(1920, 2000, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
