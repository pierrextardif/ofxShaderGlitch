#version 120
#extension GL_ARB_texture_rectangle : enable

#define M_PI 3.1415926535897932

uniform sampler2DRect       u_tex_unit0;
uniform float               u_time;
uniform vec2                u_resImg;
uniform int                 u_tilingType;
uniform vec2                u_speedLinesColumns;
uniform vec2                u_amntLinesColumns;

uniform vec2                u_gradStart;
uniform vec2                u_gradFinish;
uniform float               u_alphaGradiant;

uniform float               u_thresholdNoise;
uniform float               u_continuousMosh;

// Colors
uniform vec4                u_BackGrndColor;
uniform vec4                u_gradiantColor;




#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/utils.glsl"
// Masks
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/cells.glsl"

// Effects
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/glitch.glsl"
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/EdgeDetection.glsl"
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/Gradient.glsl"

vec3 gradColor = u_gradiantColor.rgb;
vec3 invertGradColor = 1.0 - gradColor;

vec3 backgroundColor = u_BackGrndColor.rgb;
vec3 invertBackground = 1.0 - backgroundColor;

vec2 lateralSlider(vec2 uv){
    
    float verticalID = floor(u_amntLinesColumns.y * uv.y);
    
    float pseudoNoise = fract(sin(verticalID + u_time / 100.0)*40.0) * 0.04;
    
    
        
    return vec2(mod(uv.x + pseudoNoise, 1.0), uv.y);
}


// ================

float area(vec2 uv, float amnt, float u_time, float threshold){
    float col = 0.;
    
    float k = 0.;
    
    
    float speed = u_time * 0.05;
    if(u_continuousMosh != 1.)speed = floor(u_time);
    
    for(k=1.; k <= amnt + 1.; k += .5){
        float levelZoom = k + amnt * sin(floor(k* 12.22));
        vec2 currentUV = uv * levelZoom;
        currentUV += (uv + currentUV) * ( k * k);
        currentUV.x += 60. * sin( speed );
        
        vec2 id = floor(currentUV);
        if((Hash21(id) > threshold * (1. - k / amnt) ) && col >= .2 * ( k - amnt / 4. ) ) {
            col += .2;
        }
    }
    
    col = mod(col, 1.);
    
//    if(col > .5){
//        col = 1.;
//    }else{
//        col = 0.;
//    }
    
    
    return col;
}


// ================
vec4 colorSampleShift(sampler2DRect tex, vec2 uv, vec2 imgSize, float intensityShift, vec4 originalColor){
    
    uv = mod(uv + intensityShift * imgSize, imgSize);
    return mix(texture2DRect(tex, uv), originalColor, 1. - intensityShift * .7);
}


vec4 colorStretch(sampler2DRect tex, vec2 uv, vec2 imgSize, float intensityShift, vec4 originalColor){
    
    uv.y = uv.y * 3.2 * intensityShift;
    return mix(texture2DRect(tex, uv), originalColor, intensityShift);
}
// ================

void main( void )
{
    
    vec2 uv_Norm = vec2(gl_TexCoord[0].st / u_resImg);
    
    vec2 uv = lateralSlider(uv_Norm);
    vec4 colors = texture2DRect(u_tex_unit0, gl_TexCoord[0].st);
    

    float prop = 0.0f;
    
    if(u_tilingType == 0)prop = LinesCheck(uv_Norm);
    if(u_tilingType == 1)prop = ColumnsCheck(uv_Norm);
    if(u_tilingType == 2)prop = ColOrLineCheck(uv_Norm);
    if(u_tilingType == 3)prop = ColAndLineCheck(uv_Norm);
    if(u_tilingType == 4)prop = CellsCheck(uv_Norm);
    if(u_tilingType == 5)prop = ImageCut(uv_Norm, u_MaskLayers + 1., u_speedLinesColumns * u_time);
    if(u_tilingType == 6 && addWavesDots(uv, 7, u_time * 2.1, u_resImg) >u_thresholdNoise) prop = 1.0f;
    if(u_tilingType == 7)prop = area(uv_Norm, 7., u_time * u_speedLinesColumns.y, .9);

    
    if(prop > 0.0f){
    
//        float newR = texture2DRect(u_tex_unit0, gl_TexCoord[0].st * 1.04f ).r;
//        float newG = texture2DRect(u_tex_unit0, gl_TexCoord[0].st * 1.03f ).g;
//        float newB = texture2DRect(u_tex_unit0, gl_TexCoord[0].st * 1.02f ).b;
        
        
//        colors = u_BackGrndColor;
        
//        colors = glitchColors( u_tex_unit0, gl_TexCoord[0].st, sin(u_time), prop);
        
        
//
//
//        colors.rgb = mix(colors.rgb, colShifts, prop);
//        colors.g = mix(colors.g, colShifts, prop);
//        colors.b = mix(colors.b, colShifts, prop);
        
        
        
//        fTotalSum = mix( fTotalSum, texture2DRect( ImageTexture,
        //                         vec2( gl_TexCoord[0].s, gl_TexCoord[0].t)), 0.5);
        
        
//        vec4 edges = isOnEdgeCpp(gl_TexCoord[0].st, u_resImg, u_tex_unit0);
        
        // ===========================
        //edges
//        vec4 edge = edgeDetection( u_tex_unit0, gl_TexCoord[0].st * 1.03f, u_resImg);
//        if(edge.r == 1.0 && edge.g == 1.0 && edge.b == 1.0 )colors.rgb = vec3(1.0);
        // ===========================

        
        
//        if(averageEdge > 0.9)
//        float thresholdEdge = 0.5f;
//        if( edges.r > thresholdEdge && edges.g > thresholdEdge && edges.b > thresholdEdge )colors.rgb =  edges.rgb;
//        colors = mix(edges, colors, 0.5);

    }else{
        
    }
    
//    colors = glitchColors( u_tex_unit0, gl_TexCoord[0].st, sin(u_time), prop);

    // ==== texture Shift ====
    
    colors = gradiantColor(uv_Norm, u_gradStart, u_gradFinish, colors, u_gradiantColor);
    
    
    if(prop > 0.){
//        colors = colorSampleShift(u_tex_unit0, gl_TexCoord[0].st, u_resImg, prop, colors);
        colors = colorStretch(u_tex_unit0, gl_TexCoord[0].st, u_resImg, prop, colors);
    }
    
    // ==== texture Shift ====
    
    if(colors.r < .02){
        if(prop > 0.0f)
        {
            colors *= u_BackGrndColor;
        }else{
            colors = vec4(invertBackground, 1.0);
        }
    }
    
//    if(colors.r <  .02)
        
    
    gl_FragColor = colors;
}

