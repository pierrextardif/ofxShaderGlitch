#version 120
#extension GL_ARB_texture_rectangle : enable

#define M_PI 3.1415926535897932

// ==== global ==== //
uniform sampler2DRect       u_tex_unit0;
uniform float               u_time;
uniform vec2                u_resImg;

uniform int                 u_tilingType;
uniform int                 u_effectType;

// speed
uniform vec2                u_speedLinesColumns;
uniform vec2                u_amntLinesColumns;

uniform vec2                u_gradStart;
uniform vec2                u_gradFinish;
uniform float               u_alphaGradiant;

uniform float               u_thresholdNoise;
uniform float               u_continuous;
uniform float               u_MaskLayers;

// Colors
uniform vec4                u_BackGrndColor;
uniform vec4                u_gradiantColor;

uniform float               u_maxContinuity;




// ===== import elements ==== //
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/utils.glsl"
// Effects
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/glitch.glsl"
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/EdgeDetection.glsl"
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/Gradient.glsl"
// Masks
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/cells.glsl"
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/patterns.glsl"


vec4 invertGradColor = vec4(1.0 - u_gradiantColor.rgb, u_gradiantColor.a);
vec4 invertBackground = vec4(1.0 - u_BackGrndColor.rgb, u_BackGrndColor.a);

void main( void )
{
    
    vec2 uv_Norm = vec2(gl_TexCoord[0].st / u_resImg);
    
    vec4 colors = texture2DRect(u_tex_unit0, gl_TexCoord[0].st);
    

    // ==== feedback ====
    if(u_tilingType == -1)colors.rgb = edgeTracking( u_tex_unit0, gl_TexCoord[0].st);

    float prop = 0.;
    vec3 stretch = vec3(0.);
    

    if(u_tilingType == 0)prop = LinesCheck(uv_Norm);
    if(u_tilingType == 1)prop = ColumnsCheck(uv_Norm);
    if(u_tilingType == 2)prop = ColOrLineCheck(uv_Norm);
    if(u_tilingType == 3)prop = ColAndLineCheck(uv_Norm);
    if(u_tilingType == 4)prop = CellsCheck(uv_Norm);
    if(u_tilingType == 5)prop = ImageCut(uv_Norm, u_MaskLayers + 1., u_speedLinesColumns * u_time);
    if(u_tilingType == 6 && addWavesDots(gl_TexCoord[0].st, 7, u_time * 2.1, u_resImg) >u_thresholdNoise) prop = 1.0f;
    if(u_tilingType == 7)prop = area(uv_Norm, 7., u_time * u_speedLinesColumns.y, .95);
    if(u_tilingType == 8){
        stretch = areaDegrade(uv_Norm, 7., u_time * u_speedLinesColumns.y + u_speedLinesColumns.x, .95);
        prop = stretch.x;

    }
    if(u_tilingType == 9)prop = doInnerFlip(uv_Norm);
    if(u_tilingType == 10)prop = 1.;
    
    

    // ===========================
    //edges
    float edge = edgeDetection( u_tex_unit0, gl_TexCoord[0].st, (u_maxContinuity == 1.0)?true:false);
    vec3 edgeColor = edgeTracking( u_tex_unit0, gl_TexCoord[0].st);
    
    // ===========================
    


    
    if(prop > 0.0f){
                
        // ==== glitch ==== //
        if(u_effectType == 0)colors = glitchColors(u_tex_unit0, gl_TexCoord[0].st, sin(u_time), 1. - prop);
        // ==== glitch ==== //

        // ==== texture Shift ==== //
        if(u_effectType == 1)colors = colorSampleShift(u_tex_unit0, gl_TexCoord[0].st, u_resImg, prop, colors);
        if(u_effectType == 2)colors = colorStretch(u_tex_unit0, gl_TexCoord[0].st, u_resImg, prop, stretch.yz, colors);
        
        
        // ==== texture Shift ==== //
        
        // === texture Flip Inbound ==== //
        
        if(u_effectType == 3)colors = texFlipV(u_tex_unit0, gl_TexCoord[0].st, uv_Norm, true, true);
        if(u_effectType == 4)colors = texFlipH(u_tex_unit0, gl_TexCoord[0].st, uv_Norm, true, true);
        if(u_effectType == 5)colors = texture2DRect(u_tex_unit0, lateralSlider(uv_Norm) * u_resImg);
        
        // === texture Flip Inbound ==== //
        
        // ==== stripes ==== //
        if(u_effectType == 6)colors.rgb = stripes((u_continuous == 1.)?uv_Norm.x:uv_Norm.y, u_time * 4., .5 + .1 * sin(u_time),  true);
        // ==== stripes ==== //
        
        if(u_effectType == 7)colors = u_BackGrndColor;
        if(u_effectType == 8 || u_effectType == 9)colors = gradiantColor(uv_Norm, u_gradStart, u_gradFinish, colors, u_gradiantColor);
        
        
        // ==== lateral shift ==== //
        if(u_effectType == 10)colors = lateralShift(u_tex_unit0, gl_TexCoord[0].st, uv_Norm, u_time * u_speedLinesColumns.y, .5 + .1 * sin(u_time), (u_continuous == 1.)?true:false);
        // ==== lateral shift ==== //
        
    }else{

        if(u_effectType == 7)colors = invertBackground;
        if(u_effectType == 9)colors = gradiantColor(uv_Norm, u_gradStart, u_gradFinish, colors, vec4(1. - u_gradiantColor.rgb, u_gradiantColor.a));
    }
    
    gl_FragColor = colors;
}

