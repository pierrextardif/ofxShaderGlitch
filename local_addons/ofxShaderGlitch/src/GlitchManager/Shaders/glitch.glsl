
// uniforms
float amount = 6.;
float angle = u_time * .5;
//vec2 seed = vec2(.01 * cos(u_time * 2.1), .01 * sin(u_time * 1.7));
//vec2 seed = vec2(.05 * sin(u_time), .05 * cos(u_time));
vec2 seed = vec2(0.005);

float seed_x = u_resImg.x * cos(u_time) * .4;
float seed_y = u_resImg.y * sin(u_time * 1.6) * .4;

    
//100 * sin(u_time);
float distortion_x = 1.0;
float distortion_y = 1.0;


vec4 glitchColors(sampler2DRect tex, vec2 vUv, float indexSeed, float prop){
    
    vec4 originalCol = getTexel(tex, vUv);
    
    vec2 uvForNorm = vec2(vUv.x*(1. + seed.x), vUv.y*(1. + seed.y));
    vec4 normal = getTexel(tex, uvForNorm);
  
//
    if(vUv.y>distortion_x && vUv.y<distortion_x*(1. + seed.x)) {
        if(seed_x>0.){
            vUv.y = 1. - (vUv.y + distortion_y);
        }else {
            vUv.y = distortion_y;
        }
    }

    
    if(vUv.x<distortion_y && vUv.x>distortion_y*(1 + seed.y)) {
        if(seed_y>0.){
            vUv.x=distortion_x;
        }else {
            vUv.x = 1. - (vUv.x + distortion_x);
        }
    }
    
    
    vUv.x+=prop * normal.x*seed_x*(((1 + seed.x) + 1.) / 5.);
    vUv.y+=prop * normal.y*seed_y*(((1 + seed.y) + 1.) / 5.);
    
    
    vec2 offset =  amount * vec2( cos(angle * M_PI), sin(angle * M_PI));
    vec4 cr = getTexel(tex, vUv + offset);
    vec4 cga = getTexel(tex, vUv);
    vec4 cb = getTexel(tex, vUv - offset);

    return vec4(cr.r, cga.g, cb.b, cga.a);
}


vec4 colorSampleShift(sampler2DRect tex, vec2 uv, vec2 imgSize, float intensityShift, vec4 originalColor){
    
    uv = mod(uv + intensityShift * imgSize, imgSize);
    return mix(getTexel(tex, uv), originalColor, 1. - intensityShift * .7);
}


vec4 colorStretch(sampler2DRect tex, vec2 uv, vec2 imgSize, float prop, vec2 intensityShift, vec4 originalColor){
    
//    uv.y = uv.y * 3.2 * intensityShift;
    uv = mod(uv + prop * imgSize, imgSize);
    
    vec4 red = getTexel(tex, uv);
    return mix(red, originalColor, 1. - intensityShift.y * intensityShift.x);
}
