#version 120
#extension GL_ARB_texture_rectangle : enable

#define M_PI 3.1415926535897932

uniform sampler2DRect       u_tex_unit0;
uniform float               u_time;
uniform vec2                u_resImg;
uniform int                 u_tilingType;
uniform vec2                u_speedLinesColumns;
uniform vec2                u_amntLinesColumns;

//#pragma include "./cells.glsl"
#pragma include "../../src/GlitchManager/Shaders/cells.glsl"
// ====================================================== //
float LinesCheck(vec2 uv_Norm){
    
    float insideLine = 0.0;
    float hPos = fract(u_amntLinesColumns.x * uv_Norm.x);
    
    float verticalID = floor(u_amntLinesColumns.x * uv_Norm.x);
    if(verticalID == floor(fract(u_time * u_speedLinesColumns.x) * u_amntLinesColumns.x))insideLine = 1.0;
    
    return insideLine;
}
float ColumnsCheck(vec2 uv_Norm){
    
    float insideColumn = 0.0;
    float hPos = fract(u_amntLinesColumns.y * uv_Norm.y);
    
    float horizontalID = floor(u_amntLinesColumns.y * uv_Norm.y);
    if(horizontalID == floor(fract(u_time * u_speedLinesColumns.y) * u_amntLinesColumns.y))insideColumn = 1.0;
    return insideColumn;
}

float ColOrLineCheck(vec2 uv_Norm){
    return min( (LinesCheck(uv_Norm) + ColumnsCheck(uv_Norm) ), 1.0f);
}

float ColAndLineCheck(vec2 uv_Norm){
    return (LinesCheck(uv_Norm) * ColumnsCheck(uv_Norm));

}

void main( void )
{

    vec4 colors = texture2DRect( u_tex_unit0, gl_TexCoord[0].st );
    
    
    vec2 uv_Norm = vec2(gl_TexCoord[0].st / u_resImg);

    float prop = 0.0f;
    
    if(u_tilingType == 0)prop = LinesCheck(uv_Norm);
    if(u_tilingType == 1)prop = ColumnsCheck(uv_Norm);
    if(u_tilingType == 2)prop = ColOrLineCheck(uv_Norm);
    if(u_tilingType == 3)prop = ColAndLineCheck(uv_Norm);
    if(u_tilingType == 4)prop = CellsCheck(uv_Norm);
    
    if(prop > 0.0f){
        float newR = texture2DRect(u_tex_unit0, gl_TexCoord[0].st * 1.04f ).r;
        float newG = texture2DRect(u_tex_unit0, gl_TexCoord[0].st * 1.03f ).g;
        float newB = texture2DRect(u_tex_unit0, gl_TexCoord[0].st * 1.02f ).b;
        
        colors.r = mix(colors.r, newR, prop);
        colors.g = mix(colors.g, newG, prop);
        colors.b = mix(colors.b, newB, prop);
    }
    
    gl_FragColor = colors;
}

