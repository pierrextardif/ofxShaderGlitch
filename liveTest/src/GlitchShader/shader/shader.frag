#version 330
precision highp float;

uniform sampler2DRect tex0;
uniform sampler2DRect contour;
uniform float iTime;
uniform vec2 resolution;

struct OffsetRGB {
    vec2 red;
    vec2 green;
    vec2 blue;
    
};

////
in vec3 vecProj;
in vec3 normalProj;
////

// ==== import Glitch Shader ==== //

#pragma include "./glitch.glsl"


in vec2 vUv;
out vec4 outputColor;



void main () {
    // uv texture coordinates changes
    vec4 colors = texture(tex0,vUv);

    if(texture(contour, vUv).rgb == vec3(1,1,1)){
        for( int i = 0; i < iterationsGlitch; i++){
            
            float iSpeed = i%2 + mod(i, iTime);
            float iSine = i*sin((iTime + iSpeed) );
            if( i % 2 == 0) iSine = i * cos((iTime + iSpeed));
            vec4 newC = glitchColors( vUv, tex0, iSine);
            colors.rgba = mix(newC, colors, colors - newC);
            
        }
    }
    
    
    outputColor = colors;
}
