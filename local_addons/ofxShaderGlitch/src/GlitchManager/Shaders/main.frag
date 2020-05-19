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

uniform float u_maxContinuity;

#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/utils.glsl"
// Masks
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/cells.glsl"
#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/patterns.glsl"

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

int convertAngleToDirection(float theta){
    
    int offset = -1;
    // direction vertical neighboor
    if( ( theta >= PI_3_8 && theta < PI_5_8 ) || (theta >= -PI_5_8 && theta < - PI_3_8) )offset = 0;
//    // direction horizontal neighboor
    if( ( theta >= -PI_8 && theta < PI_8 )  || (theta >= PI_7_8 && theta < PI_7_8 + PI_4) )offset = 1;
    //first diagonal //
    if( (theta >= PI_8 && theta < PI_3_8 )  || (theta >= -PI_7_8 && theta < -PI_5_8) )offset = 2;
    //second diagonal \\
    if( ( theta >= -PI_3_8 && theta < -PI_8 ) || ( theta >= PI_3_8 && theta < PI_7_8) )offset = 3;
    
    return offset;
}


float getStrength(sampler2DRect tex, vec2 uv){
    
    float strength = 0.;
    if(uv.x>0.&&uv.x<u_resImg.x&&uv.y>0.&&uv.y<u_resImg.y){
        
        int i,j;
        float colX = 0.;
        float colY = 0.;
        for(i = -1; i <=1; i++){
            for(j = -1; j <=1; j++){
                colX += GREYTex(tex, uv + vec2(i,j)) * (Gx[i+1][j+1]);
                colY += GREYTex(tex, uv + vec2(i,j)) * (Gy[i+1][j+1]);
            }
        }
        
        strength = pow(colX * colX + colY * colY, (.5 * abs(sin(PI_2))));
    }
    return strength;
}

vec2 getAngle(sampler2DRect tex, vec2 uv){
    
    vec2 theta = vec2(0.);
    if(uv.x>0.&&uv.x<u_resImg.x&&uv.y>0.&&uv.y<u_resImg.y){
        
        int i,j;
        float colX = 0.;
        float colY = 0.;
        for(i = -1; i <=1; i++){
            for(j = -1; j <=1; j++){
                colX += GREYTex(tex, uv + vec2(i,j)) * (Gx[i+1][j+1]);
                colY += GREYTex(tex, uv + vec2(i,j)) * (Gy[i+1][j+1]);
            }
        }
        
        theta = convertAngle(atan(colY / colX));
    }
    
    return theta;
}

vec2 getSA(sampler2DRect tex, vec2 uv){
    vec2 strengthTheta = vec2(0.);
    if(uv.x>0.&&uv.x<u_resImg.x&&uv.y>0.&&uv.y<u_resImg.y){
       
       int i,j;
       float colX = 0.;
       float colY = 0.;
       for(i = -1; i <=1; i++){
           for(j = -1; j <=1; j++){
               colX += GREYTex(tex, uv + vec2(i,j)) * (Gx[i+1][j+1]);
               colY += GREYTex(tex, uv + vec2(i,j)) * (Gy[i+1][j+1]);
           }
       }
       
        strengthTheta.x = pow(colX * colX + colY * colY, (.5 * abs(sin(PI_2))));
        strengthTheta.y = convertAngleToDirection(atan(colY / colX) );
        
    }
    
    return strengthTheta;
}
//

// ================

float getLuminance(vec3 rgb){
    return 0.2126*rgb.r + 0.7152*rgb.g + 0.0722*rgb.b;
}

// ================

float doInnerFlip(vec2 uvNorm){
    vec2 inside = uvNorm;
    
    float prop = 0.;
    if(u_gradStart.x < inside.x && inside.x < u_gradFinish.x && u_gradStart.y < inside.y && inside.y < u_gradFinish.y){
        prop = 1.0;
    }
    
    
    return prop;
}

vec4 texFlipV(sampler2DRect tex, vec2 uv, vec2 uvNorms, bool horizontal, bool addGradiant){
    vec4 colors = vec4(0.);
    
    
    vec2 pos = floor(uvNorms * u_amntLinesColumns);
    
    if(u_gradStart.x < uvNorms.x && uvNorms.x < u_gradFinish.x && u_gradStart.y < uvNorms.y && uvNorms.y < u_gradFinish.y){
        
        vec2 newUVs = uv;
        if(mod(pos.y, 2.) == 0.){
            if(horizontal){
                newUVs = vec2(u_resImg - uv);
            }else{
                newUVs = vec2(u_resImg.x - uv.x, uv.y);
            }
        }
        
        colors = texture2DRect(tex, newUVs);
        if(addGradiant)colors = gradiantColor(uvNorms, u_gradStart, u_gradFinish, colors, u_gradiantColor);
    }
    
    return colors;
    
}

vec4 texFlipH(sampler2DRect tex, vec2 uv, vec2 uvNorms, bool vertical, bool addGradiant){
    vec4 colors = vec4(0.);
    
    
    vec2 pos = floor(uvNorms * u_amntLinesColumns);
    
    if(u_gradStart.x < uvNorms.x && uvNorms.x < u_gradFinish.x && u_gradStart.y < uvNorms.y && uvNorms.y < u_gradFinish.y){
        
        vec2 newUVs = uv;
        if(mod(pos.x, 2.) == 0.){
            if(vertical){
                newUVs = vec2(u_resImg - uv);
            }else{
                newUVs = vec2(uv.x, u_resImg.y - uv.y);
            }
        }
        
        colors = texture2DRect(tex, newUVs);
        if(addGradiant)colors = gradiantColor(uvNorms, u_gradStart, u_gradFinish, colors, u_gradiantColor);
    }
    
    return colors;
    
}


// ================
void main( void )
{
    
    vec2 uv_Norm = vec2(gl_TexCoord[0].st / u_resImg);
    
    vec2 uv = lateralSlider(uv_Norm);
    vec4 colors = texture2DRect(u_tex_unit0, gl_TexCoord[0].st);
//    colors = gradiantColor(uv_Norm, u_gradStart, u_gradFinish, colors, u_gradiantColor);
    
    

    float prop = 0.;
    vec3 stretch = vec3(0.);


    if(u_tilingType == 0)prop = LinesCheck(uv_Norm);
    if(u_tilingType == 1)prop = ColumnsCheck(uv_Norm);
    if(u_tilingType == 2)prop = ColOrLineCheck(uv_Norm);
    if(u_tilingType == 3)prop = ColAndLineCheck(uv_Norm);
    if(u_tilingType == 4)prop = CellsCheck(uv_Norm);
    if(u_tilingType == 5)prop = ImageCut(uv_Norm, u_MaskLayers + 1., u_speedLinesColumns * u_time);
    if(u_tilingType == 6 && addWavesDots(uv, 7, u_time * 2.1, u_resImg) >u_thresholdNoise) prop = 1.0f;
    if(u_tilingType == 7)prop = area(uv_Norm, 7., u_time * u_speedLinesColumns.y, .95);
    if(u_tilingType == 8){
        stretch = areaDegrade(uv_Norm, 7., u_time * u_speedLinesColumns.y + u_speedLinesColumns.x, .95);
        prop = stretch.x;

    }
    if(u_tilingType == 9)prop = doInnerFlip(uv_Norm);
    
    

    // ===========================
    //edges
//    float edge = edgeDetection( u_tex_unit0, gl_TexCoord[0].st, (u_maxContinuity == 1.0)?true:false);
//    vec2 edge = processEdge( u_tex_unit0, gl_TexCoord[0].st);
    
    
    // ===========================
    
    
    if(prop > 0.0f){
        
        // ==== glitch ==== //
//        colors = glitchColors(u_tex_unit0, gl_TexCoord[0].st, sin(u_time), 1. - prop);
        // ==== glitch ==== //

        // ==== texture Shift ==== //
//        colors = colorSampleShift(u_tex_unit0, gl_TexCoord[0].st, u_resImg, prop, colors);
//        colors = colorStretch(u_tex_unit0, gl_TexCoord[0].st, u_resImg, prop, stretch.yz, colors);
        
        
        // ==== texture Shift ==== //
        
        // === texture Flip Inbound ==== //
        
//        colors = texFlipV(u_tex_unit0, gl_TexCoord[0].st, uv_Norm, true, true);
        colors = texFlipH(u_tex_unit0, gl_TexCoord[0].st, uv_Norm, true, true);
        
        // === texture Flip Inbound ==== //
        
        // ==== stripes ==== //
//        colors.rgb = stripes(uv_Norm.x, u_time * 4., .5 + .1 * sin(u_time),  true);
        // ==== stripes ==== //
        
    }
    
    gl_FragColor = colors;
}

