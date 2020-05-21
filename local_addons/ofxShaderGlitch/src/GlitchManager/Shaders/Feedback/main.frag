#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect       u_tex_unit0;
uniform sampler2DRect       u_texFeedback;
uniform vec2                u_resImg;
uniform float               u_time;

#pragma include "../../../local_addons/ofxShaderGlitch/src/GlitchManager/Shaders/utils.glsl"


// ================


float highThreshold = .35;
float lowThreshold = .1;

float GREYCol(vec3 rgb){
    return .3086 * rgb.r + .6094 * rgb.g + 0.0820 * rgb.b;
    
}

float GREYTex(sampler2DRect tex, vec2 uv){
//    vec3 rgb = Gaussian5_5(tex, uv).rgb;
    vec3 rgb = texture2DRect(tex, uv).rgb;
    return GREYCol(rgb);
    
}

mat3 Gx = mat3( -1.0, 0.0, 1.0,
                -2.0, 0.0, 2.0,
                -1.0, 0.0, 1.0 );

mat3 Gy =  mat3( 1.0,  2.0,  1.0,
                 0.0,  0.0,  0.0,
                -1.0, -2.0, -1.0 );

vec2  offsetTheta(float val){
    
    vec2 dir = vec2(0.);
    if(val == 0.)dir = vec2(0., 1.);
    if(val == 1.)dir = vec2(1., 0.);
    if(val == 2.)dir = vec2(1., -1.);
    if(val == 3.)dir = vec2(1., 1.);
    
    return dir;
}

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


vec2 convertAngle(float theta){
    
    vec2 offset = vec2(0.);
    // direction vertical neighboor
    if( ( theta >= PI_3_8 && theta < PI_5_8 ) || (theta >= -PI_5_8 && theta < - PI_3_8) )offset = vec2(0., 1.);
//    // direction horizontal neighboor
    if( ( theta >= -PI_8 && theta < PI_8 )  || (theta >= PI_7_8 && theta < PI_7_8 + PI_4) )offset = vec2(1., 0.);
    //first diagonal //
    if( (theta >= PI_8 && theta < PI_3_8 )  || (theta >= -PI_7_8 && theta < -PI_5_8) )offset = vec2(-1., -1.);
    //second diagonal \\
    if( ( theta >= -PI_3_8 && theta < -PI_8 ) || ( theta >= PI_3_8 && theta < PI_7_8) )offset = vec2(1., -1.);
    
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

bool dirRed(vec2 uvDir){
    bool hDir = false;
    // direction is vertical so we take the horizontal direction to get a ray
    if(uvDir.y == 0.)hDir = true;
    return hDir;
}

bool dirGreen(vec2 uvDir){
    bool vDir = false;
    // direction is horizontal so we take the vertical direction to get a ray
    if(uvDir.x == 0.)vDir = true;
    return vDir;
}


bool dirBlue(vec2 uvDir){
    bool fDiagDir = false;
    // direction is first diag so we take the second diag to get a ray
    if( (uvDir.x == -1. && uvDir.y == 1. ) || (uvDir.x == 1. && uvDir.y == -1. ) )fDiagDir = true;
    return fDiagDir;
}

bool dirTurquoise(vec2 uvDir){
    bool sDiagDir = false;
    // direction is first diag so we take the second diag to get a ray
    if( (uvDir.x == -1. && uvDir.y == -1.) || (uvDir.x == 1. && uvDir.y == 1.) )sDiagDir = true;
    return sDiagDir;
}

vec3 edgeFeedback(sampler2DRect tex, vec2 uv){
    
    vec3 col = vec3(0.);
    int i,j;
    
    vec4 decreaseRatio = vec4(.96, .94, .98, .94);
    
    
    for(i = -1; i <= 1; i += 1){
        for(j = -1; j <= 1; j += 1){

            vec2 newUVs = uv + vec2(i,j);
            vec4 c = texture2DRect(tex, newUVs);

            if(c.b > 0. && c.g == c.b && dirTurquoise(vec2(i,j))){
                col.g = col.b = c.g * decreaseRatio.w;
            }
            
            
            if(c.r > 0. && dirRed(vec2(i,j)) ){
                col.r = c.r * decreaseRatio.x;
            }
            if(c.g > 0. && dirGreen(vec2(i,j)) ){
                col.g = c.g * decreaseRatio.y;
            }
            if(c.b > 0. && dirBlue(vec2(i,j)) ){
                col.b = c.b * decreaseRatio.z;
            }
            
        }
    }
    
    return col;
    
}

// ================
void main( void )
{
    vec4 colors = texture2DRect(u_tex_unit0, gl_TexCoord[0].st);
    colors.rgb += edgeFeedback(u_texFeedback, gl_TexCoord[0].st);
    
    gl_FragColor = colors;
}

 
