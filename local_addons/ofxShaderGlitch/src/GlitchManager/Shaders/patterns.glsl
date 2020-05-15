
float area(vec2 uv, float amnt, float u_time, float threshold){
    float col = 0.;
    
    float k = 0.;
    
    
    float speed = u_time * 0.05;
    if(u_continuousMosh != 1.)speed = floor(u_time);
    
    for(k=1.; k <= amnt + 1.; k += .5){
        float levelZoom = k + amnt * sin(floor(k* 12.22));
        vec2 currentUV = uv * levelZoom;
        currentUV += (uv + currentUV) * ( k * k);
        currentUV.x += 60. * sin( speed );
        
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
    if(u_continuousMosh != 1.)speed = floor(u_time);
    float passes = 0.;
    
    for(k=1.; k <= amnt + 1.; k += .5){
        float levelZoom = k + amnt * sin(floor(k* 12.22));
        vec2 currentUV = uv * levelZoom;
        currentUV += (uv + currentUV) * ( k * k);
        currentUV.x += 60. * sin( speed );
        
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
