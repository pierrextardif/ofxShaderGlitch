#define ON_BIN_APP_SHADERS
//#define LOCALADDON

#ifdef ON_BIN_APP_SHADERS
    #define PATHTOSHADERS "Shaders/ofxShaderGlitch/"
#else
#ifdef LOCALADDON
    #define PATHTOSHADERS "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/"
#else
    #define PATHTOSHADERS "../../../../../addons/ofxShaderGlitch/src/GlitchManager/Shaders/"
#endif
#endif