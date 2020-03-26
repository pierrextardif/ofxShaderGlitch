#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect 	u_tex_unit0;
uniform float 			u_time;
uniform vec2            u_resImg;

uniform float amount;
uniform float angle;
uniform float seed;
uniform int iterationsGlitch;

 #pragma include "../../src/GlitchShader/shader/glitch.glsl"


bool OutsideSquare(vec2 cellXY, vec2 valSlopes, vec2 offset){
    
    bool OutsideCell = false;
    float cellXSize = cellXY.x;
    float cellYSize = cellXY.y;
    
    float heightSlope = valSlopes.y;
    float slope = valSlopes.x;
    
    slope += offset.x;
    heightSlope += offset.y;
    
    //  Left
    float A = - heightSlope / slope;
    float B = heightSlope;
    // TOP side
    if(     (cellXSize <= slope)&&
            (cellYSize <= A * cellXSize + B)){
        OutsideCell = true;
    }
    A = (1 - heightSlope) / slope;
    B = heightSlope;
    // BOTTOM side
    if(     (cellXSize <= slope)&&
            (cellYSize >= A * cellXSize + B)){
        OutsideCell = true;
    }
    
    slope -= 2 * offset.x;
    heightSlope -= 2 * offset.y;
    // Right Side
    // TOP side
    A = heightSlope / slope;
    B = heightSlope - A;
    
    if( cellXSize >= slope &&
       (cellYSize <= A * cellXSize + B) ){
        OutsideCell = true;
    }
    // BOTTOM side
    A = (heightSlope - 1) / slope;
    B = heightSlope - A;
    
    if( cellXSize >= slope &&
       (cellYSize >= A * cellXSize + B) ){
        OutsideCell = true;
    }
    return OutsideCell;
}
float Hash21(vec2 p){
    p = fract(p * vec2( 123.34, 456.21));
    p += dot(p, p +45.32);
    return fract(p.x*p.y);
}

bool CellCheck(){
    
    bool outsideCell = false;
    vec2 uv_Norm = vec2(gl_TexCoord[0].st / u_resImg);
    
    
    float ratio = 2.0;
    float amountCell = 6.0;
    float cellXSize = fract( amountCell * uv_Norm.x);
    float cellYSize = fract( amountCell * ratio * uv_Norm.y);
    
    float thicknessLine = 0.005;
    if(cellXSize > 1 - thicknessLine || cellYSize > 1 - thicknessLine * ratio || cellXSize < thicknessLine || cellYSize < thicknessLine * ratio){
        outsideCell = true;
    }
    
    vec2 offset = vec2(0.05, 0.1);
    
    vec2 id = vec2(floor(amountCell * uv_Norm.x), floor(amountCell * ratio * uv_Norm.y));
    float n = Hash21(id);
    if(OutsideSquare(vec2(cellXSize + offset.x, cellYSize + offset.y), vec2(n * 0.1 + 0.15+ 0.15* cos(u_time), fract(n * 12.0) * 0.1 + 0.4), offset ) )outsideCell = true;
    
    return outsideCell;
}

void main( void )
{

	float newTime = u_time * 2.0;
    
	vec2 newTexCoord;
	newTexCoord.s = gl_TexCoord[0].s + (cos(newTime + (gl_TexCoord[0].s*0.01)) * 10.0);
	newTexCoord.t = gl_TexCoord[0].t + (sin(newTime + (gl_TexCoord[0].t*0.01)) * 10.0);

	vec2 texCoordRed 	= newTexCoord;
	vec2 texCoordGreen 	= newTexCoord;
	vec2 texCoordBlue 	= newTexCoord;

	texCoordRed 	+= vec2( cos((newTime * 2.76)), sin((newTime * 2.12)) ) * 10.0;// * sin(u_time);
	texCoordGreen 	+= vec2( cos((newTime * 2.23)), sin((newTime * 2.40)) ) * 10.0;// * sin(u_time);
	texCoordBlue 	+= vec2( cos((newTime * 2.98)), sin((newTime * 2.82)) ) * 10.0;// * sin(u_time);	

	float colorR = texture2DRect( u_tex_unit0, gl_TexCoord[0].st ).r;
	float colorG = texture2DRect( u_tex_unit0, gl_TexCoord[0].st).g;
	float colorB = texture2DRect( u_tex_unit0, gl_TexCoord[0].st).b;
    
    vec4 colors = texture2DRect( u_tex_unit0, gl_TexCoord[0].st );
    
    
    if(CellCheck()){
        // pattern Folding OUTSIDE
//        colorR = 1.0;
    }else{
        for( int i = 0; i < iterationsGlitch; i++){

            float iSpeed = floor(i/2.0) + mod(i, u_time);
            float iSine = i*sin((u_time + iSpeed) );
            if( floor(i/2.0) == 0) iSine = i * cos((u_time + iSpeed));
            vec4 newC = glitchColors( gl_TexCoord[0].st, u_tex_unit0, iSine);
            colors.rgba = mix(newC, colors, colors - newC);

        }
    }
    gl_FragColor = colors;
//	gl_FragColor = vec4( colorR, colorG, colorB, 1.0 );
}

