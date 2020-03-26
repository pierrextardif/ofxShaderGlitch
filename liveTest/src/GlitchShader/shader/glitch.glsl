// uniforms

float seed_x = 100;
float seed_y = 100;
float distortion_x = 1.0;
float distortion_y = 1.0;


vec4 glitchColors(vec2 vUv, sampler2DRect tex, float indexSeed){
    
    float xs = floor(vUv.x / 0.5);
    float ys = floor(vUv.y / 0.5);
    vec4 normal = texture2DRect(tex, vUv*(1 + seed)*(1 + seed));
    if(vUv.y<distortion_x && vUv.y>distortion_x*(1 + seed)) {
        if(seed_x>0.){
            vUv.y = 1. - (vUv.y + distortion_y);
        }else {
            vUv.y = distortion_y;
        }
    }
    if(vUv.x<distortion_y && vUv.x>distortion_y*(1 + seed)) {
        if(seed_y>0.){
            vUv.x=distortion_x;
        }else {
            vUv.x = 1. - (vUv.x + distortion_x);
        }
    }
    vUv.x+=normal.x*seed_x*(((1 + seed) + indexSeed)/5.);
    vUv.y+=normal.y*seed_y*(((1 + seed) + indexSeed)/5.);
    
// base sur rgb shift
    vec2 offset = amount*vec2( cos(angle), sin(angle));
    vec4 cr = texture2DRect(tex, vUv + offset);
    vec4 cga = texture2DRect(tex, vUv);
    vec4 cb = texture2DRect(tex, vUv - offset);
    return vec4(cr.r, cga.g, cb.b, cga.a);
}
