#version 120
#extension GL_ARB_texture_rectangle : enable

#define M_PI 3.1415926535897932

uniform sampler2DRect       u_tex_unit0;
uniform vec2                u_resImg;

// ==== Gaussian filtering ==== //
const int kernelSize = 5;
float gaussianKernel[kernelSize * kernelSize] = float[]( 2.,   4.,  5.,  4.,  2.,
                                                         4.,   9., 12.,  9.,  4.,
                                                         5.,  12., 15., 12.,  5.,
                                                         2.,   4.,  5.,  4.,  2.,
                                                         4.,   9., 12.,  9.,  4.);

vec4 Gaussian5_5(sampler2DRect tex, vec2 uv){
    
    uv -= vec2(2);
    
    vec4 col;
    int i = 0;
    for(i=0; i < gaussianKernel.length(); i += 1){
        
        vec2 offset = vec2(mod(float(i),5.) - 2., floor(float(i) / 5.) - 2.);
        col += texture2DRect(tex, uv + offset) * gaussianKernel[i];
    }
    
    col /= 159.;
    
    return col;
}

// ==== Gaussian filtering ==== //

void main( void )
{
    gl_FragColor = Gaussian5_5(u_tex_unit0, gl_TexCoord[0].st);
}

