
vec4 gradiantOutline(vec2 uv, vec2 start, vec2 finish, vec4 originalColors, vec4 neonColor ){
    
    // edge contour
    vec2 thicknessContour = vec2(1.0) / u_resImg;
    if( ( (uv.x >= start.x && (uv.x <= start.x + thicknessContour.x)&& uv.y >= start.y && uv.y <= finish.y) ) ||
        ( (uv.x >= finish.x - thicknessContour.x ) && uv.x <= finish.x && uv.y >= start.y && uv.y <= finish.y)||
        ( (uv.x >= start.x && uv.x <= finish.x && uv.y >= start.y && uv.y <= start.y + thicknessContour.y) ) ||
        ( (uv.x >= start.x && uv.x <= finish.x && (uv.y >= finish.y - thicknessContour.y) && uv.y <= finish.y) ) )
    {
        originalColors = neonColor;
    }
    
    return originalColors;
}

vec4 gradiantColor(vec2 uv, vec2 start, vec2 finish, vec4 originalColors, vec4 neonColor ){
    
    if( (uv.x > start.x) && (uv.y > start.y)  && (uv.x < finish.x ) && (uv.y < finish.y )){
        
        float slopeY = 1.0 / (finish.y - start.y);
        float offsetY = - start.y * slopeY;
        
        float newY = slopeY * uv.y + offsetY;
        vec4 grad = neonColor;
        
        
        // noise
        float noiseAdded = Hash22(vec2(uv.x, newY + 0.01 * u_time ) );
        grad.r += newY * noiseAdded;
        grad.g += newY * noiseAdded;
        grad.b += newY * noiseAdded;
        
        
        float alpha = 1.0 - newY;
        
//        light colors average :
        originalColors.rgb = vec3(0.21 * originalColors.r + 0.72 * originalColors.g + 0.07 * originalColors.b);
        originalColors = mix(originalColors,grad, alpha * u_alphaGradiant);        
        
        
    }

    // edge contour
    originalColors = gradiantOutline(uv, start, finish, originalColors, neonColor);
    
    
    return originalColors;
}

