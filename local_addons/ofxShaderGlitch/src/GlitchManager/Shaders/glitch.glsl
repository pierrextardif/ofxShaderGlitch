
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
    
    vec4 originalCol = texture2DRect(tex, vUv);
//    originalCol.r = 1.;
    
//    vec4 normal = texture2DRect(tex, vUv*(1. + seed)*(1 + seed));
    vec2 uvForNorm = vec2(vUv.x*(1. + seed.x), vUv.y*(1. + seed.y));
    vec4 normal = texture2DRect(tex, uvForNorm);
  
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
    vec4 cr = texture2DRect(tex, vUv + offset);
    vec4 cga = texture2DRect(tex, vUv);
    vec4 cb = texture2DRect(tex, vUv - offset);
    
    if(cr.r < .02 && originalCol.r > .02)cr.r = originalCol.r;
    if(cga.g < .02 && originalCol.g > .02)cga.g = originalCol.g;
    if(cb.b < .02 && originalCol.b > .02)cb.b = originalCol.g;
//    return cr;
    return vec4(cr.r, cga.g, cb.b, cga.a);
//    return mix(normal, originalCol, .5);
}
