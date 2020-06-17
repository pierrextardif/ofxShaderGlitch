#define ON_BIN_APP_SHADERS	//this alone to put shaders files into bin/data
//#define LOCALADDON	//uncommented to used addon into /local_addons
//both above commented to use shader files into addon folder

#ifdef ON_BIN_APP_SHADERS
    #define PATHTOSHADERS "Shaders/ofxShaderGlitch/"
#else
#ifdef LOCALADDON
    #define PATHTOSHADERS "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/"
#else
    #define PATHTOSHADERS "../../../../../addons/ofxShaderGlitch/src/GlitchManager/Shaders/"
#endif
#endif