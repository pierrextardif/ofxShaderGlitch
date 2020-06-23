
// ==== area detection ==== //
float area(vec2 uv, float amnt, float u_time, float threshold){
    float col = 0.;
    
    float k = 0.;
    
    
    float speed = u_time * 0.05;
    if(u_continuous != 1.)speed = floor(u_time);
    
    for(k=1.; k <= amnt + 1.; k += .5){
        float levelZoom = k + amnt * sin(floor(k* 12.22));
        vec2 currentUV = uv * levelZoom;
        currentUV += (uv + currentUV) * ( k * k);
        currentUV.x += 60. + 10. * ( speed );
        
        vec2 id = floor(currentUV);
        if((Hash21(id) > threshold * (1. - k / amnt) ) && col >= .2 * ( k - amnt / 4. ) ) {
            col += .2;
        }
    }
    
    col = mod(col, 1.);
    
    return col;
}

vec3 areaDegrade(vec2 uv, float amnt, float u_time, float threshold){
    vec3 col = vec3(0.);
    
    float k = 0.;
    
    
    float speed = u_time * 0.05;
    
    if(u_continuous != 1.)speed = floor(u_time);
    float passes = 0.;
    
    for(k=1.; k <= amnt + 1.; k += .5){
        float levelZoom = k + amnt * sin(floor(k* 12.22));
        vec2 currentUV = uv * levelZoom;
        currentUV += (uv + currentUV) * ( k * k);
        currentUV.x += 60. * sin(speed);
        
        vec2 id = floor(currentUV);
        vec2 f = fract(currentUV);
        float h = Hash21(id);
        if(h > threshold ){
            
//            col.x += h;
            if((Hash21(id) > threshold * (1. - k / amnt) ) && col.x >= .2 * ( k - amnt / 4. ) ) {
                col.x += .2;
            }
            
            float ff = fract(1332.342 * h);
            if(ff < .25){
                col.y += f.x;
                col.z += (1. - f.y);
            }else if(ff >= .25 && ff < .5){
                col.y += f.x;
                col.z += f.y;
            }else if(ff >= .5 && ff < .75){
                col.y += 1. - f.x;
                col.z += f.y;
            }else if(ff >= .75){
                col.y += 1. - f.x;
                col.z += f.y;
            }
            passes += 1.;
        }
    }
    
    col.x = mod(col.x, 1.);
    if(passes == 0.)passes = 1.;
    col.yz /= passes;
    
    
    return col;
}
// ==== area detection ==== //

// ==== stripes ==== //

vec3 stripes(float coord, float speed, float threshold, bool flicker){
    
    vec3 cols = vec3(0.);
    
    
    int k;
    int amnt = u_MaskLayers;
    for(k = 2; k <= 2 + amnt; k += 1){
        float index = k * 6.;
        float speedInterval = floor(coord * index + amnt);
        float dir = Hash21(vec2(speedInterval));
        if(dir > .5)speedInterval = - dir * 3.;
        if(dir < .5)speedInterval = dir * 3.;
        float interval = floor(coord * index + speed * speedInterval  );
        
        
        vec3 rndRgb = vec3(Hash21(vec2(interval)));
        if(flicker){
            rndRgb.g = Hash21(vec2(rndRgb.r + speed * speedInterval));
            rndRgb.b = Hash21(vec2(rndRgb.g + speed * speedInterval));
        }else{
            rndRgb.g = Hash21(vec2(rndRgb.r));
            rndRgb.b = Hash21(vec2(rndRgb.r,rndRgb.g));
        }
        
        if(rndRgb.r < threshold)cols += vec3(rndRgb.r * 4. * (.7 + .3 * abs(sin(speed)) ), rndRgb.gb) / float(amnt);
    }
    
    return cols;
}

// ==== stripes ==== //

// ==== lateral shift 1 ==== //

vec4 lateralShift(sampler2DRect tex, vec2 uv, vec2 originalCoords, float speed, float threshold, bool vertical){
    
    vec3 cols = vec3(0.);
    
    float coord = originalCoords.y;
    if(!vertical)coord = originalCoords.x;
    
    int k;
    int amnt = 0;
    for(k = 2; k <= 2 + amnt; k += 1){
        float index = k * 6.;
        float speedInterval = floor(coord * index + amnt);
        float dir = Hash21(vec2(speedInterval));
        if(dir > .5)speedInterval = - dir * 3.;
        if(dir < .5)speedInterval = dir * 3.;
        float interval = floor(coord * index + speed * speedInterval  );
        
        
        float offset = Hash21(vec2(interval));
        float sineRun = Hash21(vec2(offset * uv.y));
        if(!vertical)sineRun = Hash21(vec2(offset * uv.x));
        
        if(offset < threshold)
        {
            
            if(vertical)uv.x += (offset + sineRun * .005 * sin(uv.y * .1 + 2. * speed)) * u_resImg.x;
            if(!vertical)uv.y += (offset + sineRun * .005 * sin(uv.x * .1 + speed)) * u_resImg.y;
        }
    }
    
//    uv.x = mod(uv.x, u_resImg.x);
    
    return getTexel(tex, uv);
}

// ==== lateral shift 1 ==== //

// ==== lateral shift 2 ==== //

vec2 lateralSlider(vec2 uv, bool h){
    
    float id = floor(u_amntLinesColumns.y * uv.y);
    if(h)id = floor(u_amntLinesColumns.x * uv.x);
    
    float pseudoNoise = fract(sin(id + u_time / 100.0)*40.0) * 0.04;
    
    
        
    return (h ? vec2(uv.x, mod(uv.y + pseudoNoise, 1.0)) : vec2(mod(uv.x + pseudoNoise, 1.0), uv.y) );
}

// ==== lateral shift 2 ==== //


