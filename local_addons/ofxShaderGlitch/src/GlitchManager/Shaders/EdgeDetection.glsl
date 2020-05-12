// coming from https://towardsdatascience.com/canny-edge-detection-step-by-step-in-python-computer-vision-b49c3a2d8123

uniform vec2 u_ThicknessEdge;

#define M_PI 3.1415926535897932

// X directional search matrix.
mat3 GX = mat3( -1.0, 0.0, 1.0,
                -2.0, 0.0, 2.0,
                -1.0, 0.0, 1.0 );
// Y directional search matrix.
mat3 GY =  mat3( 1.0,  2.0,  1.0,
                 0.0,  0.0,  0.0,
                -1.0, -2.0, -1.0 );

vec4 Blur(sampler2DRect imageTexture, vec2 uv, vec2 imgRes, vec2 kernelSize){
    vec4 sum = vec4(0.0);
    
    //the direction of our blur
    //(1.0, 0.0) -> x-axis blur
    //(0.0, 1.0) -> y-axis blur
    float hstep = 1.0;
    float vstep = 1.0;
    
    //apply blurring, using a 9-tap filter with predefined gaussian weights
    
    sum += texture2DRect(imageTexture, vec2(uv.x - 4.0 * kernelSize.x * hstep, uv.y - 4.0 * kernelSize.y * vstep)) * 0.0162162162;
    sum += texture2DRect(imageTexture, vec2(uv.x - 3.0 * kernelSize.x * hstep, uv.y - 3.0 * kernelSize.y * vstep)) * 0.0540540541;
    sum += texture2DRect(imageTexture, vec2(uv.x - 2.0 * kernelSize.x * hstep, uv.y - 2.0 * kernelSize.y * vstep)) * 0.1216216216;
    sum += texture2DRect(imageTexture, vec2(uv.x - 1.0 * kernelSize.x * hstep, uv.y - 1.0 * kernelSize.y * vstep)) * 0.1945945946;
    
    sum += texture2DRect(imageTexture, vec2(uv.x, uv.y)) * 0.2270270270;
    
    sum += texture2DRect(imageTexture, vec2(uv.x + 1.0 * kernelSize.x * hstep, uv.y + 1.0 * kernelSize.y * vstep)) * 0.1945945946;
    sum += texture2DRect(imageTexture, vec2(uv.x + 2.0 * kernelSize.x * hstep, uv.y + 2.0 * kernelSize.y * vstep)) * 0.1216216216;
    sum += texture2DRect(imageTexture, vec2(uv.x + 3.0 * kernelSize.x * hstep, uv.y + 3.0 * kernelSize.y * vstep)) * 0.0540540541;
    sum += texture2DRect(imageTexture, vec2(uv.x + 4.0 * kernelSize.x * hstep, uv.y + 4.0 * kernelSize.y * vstep)) * 0.0162162162;

    //discard alpha for our simple demo, multiply by vertex color and return
    return sum;
}

vec4 sobelEdge(sampler2DRect imageTexture, vec2 uv, vec2 imgRes)
{
    
    vec2 kernelSize = vec2(3);
    vec4  fSumX = vec4( 0.0,0.0,0.0,0.0 );
    vec4  fSumY = vec4( 0.0,0.0,0.0,0.0 );
    vec4 fTotalSum = vec4( 0.0,0.0,0.0,0.0 );
    float fXIndex = uv.x;
    float fYIndex = uv.y;

    /* image boundaries Top, Bottom, Left, Right pixels*/
    if( ( fYIndex > 1.0 || fYIndex < imgRes.y - 1.0 ||
            fXIndex > 1.0 || fXIndex < imgRes.x - 1.0 ))
    {
        // X Directional Gradient calculation.
        for(float I=-1.0; I<=1.0; I = I + 1.0)
        {
            for(float J=-1.0; J<=1.0; J = J + 1.0)
            {
                float fTempX = ( fXIndex + I + 1.0 ) ;
                float fTempY = ( fYIndex + J + 1.0 ) ;
                
//                vec4 fTempSumX = texture2DRect( imageTexture, vec2( fTempX, fTempY ));
                vec4 fTempSumX = Blur(imageTexture,vec2(fTempX, fTempY), imgRes, kernelSize);
                fSumX = fSumX + ( fTempSumX * vec4( GX[int(I+1.0)][int(J+1.0)],
                                                    GX[int(I+1.0)][int(J+1.0)],
                                                    GX[int(I+1.0)][int(J+1.0)],
                                                    GX[int(I+1.0)][int(J+1.0)]));
            }
        }

        { // Y Directional Gradient calculation.
            for(float I=-1.0; I<=1.0; I = I + 1.0)
            {
                for(float J=-1.0; J<=1.0; J = J + 1.0)
                {
                    float fTempX = ( fXIndex + I + 1.0 );
                    float fTempY = ( fYIndex + J + 1.0 );

//                    vec4 fTempSumY = texture2DRect( imageTexture, vec2( fTempX, fTempY ));
                    vec4 fTempSumY = Blur(imageTexture,vec2(fTempX, fTempY), imgRes, kernelSize);
                    fSumY = fSumY + ( fTempSumY * vec4( GY[int(I+1.0)][int(J+1.0)],
                                                        GY[int(I+1.0)][int(J+1.0)],
                                                        GY[int(I+1.0)][int(J+1.0)],
                                                        GY[int(I+1.0)][int(J+1.0)]));
                }
            }
            // Combine X Directional and Y Directional Gradient.
            vec4 fTem = fSumX * fSumX + fSumY * fSumY;
            fTotalSum = sqrt( fTem );
        }
    }
    return fTotalSum;
}

void make_kernel(inout vec4 n[9], sampler2DRect tex, vec2 coord, vec2 imgRes)
{
    float w = 1.0 / imgRes.x;
    float h = 1.0 / imgRes.y;
    
    
    vec2 kernelSize = vec2(2);

    n[0] = Blur(tex, coord + vec2( -w, -h), imgRes, kernelSize);
    n[1] = Blur(tex, coord + vec2(0.0, -h), imgRes, kernelSize);
    n[2] = Blur(tex, coord + vec2(  w, -h), imgRes, kernelSize);
    n[3] = Blur(tex, coord + vec2( -w, 0.0), imgRes, kernelSize);
    n[4] = Blur(tex, coord, imgRes, kernelSize);
    n[5] = Blur(tex, coord + vec2(  w, 0.0), imgRes, kernelSize);
    n[6] = Blur(tex, coord + vec2( -w, h), imgRes, kernelSize);
    n[7] = Blur(tex, coord + vec2(0.0, h), imgRes, kernelSize);
    n[8] = Blur(tex, coord + vec2(  w, h), imgRes, kernelSize);


}

vec4 sobel(sampler2DRect imageTexture, vec2 uv, vec2 imgRes){
    vec4 n[9];
    make_kernel( n, imageTexture, uv, imgRes);

    vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
      vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
    vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

    return vec4( 1.0 - sobel.rgb, 1.0 );
}

vec4 edgeDetection(sampler2DRect imageTexture, vec2 uv, vec2 imgRes){
    vec4 fTotalSum = vec4(0);
    
//    vec4 averageBlurredFrag = Blur(imageTexture, uv, imgRes, vec2(5));
    
    
    vec4 sobel = sobelEdge(imageTexture, uv, imgRes);
//    vec4 sobel = sobel(imageTexture, uv, imgRes);
    
    float highThreshold = 0.15;
    float lowThreshold = 0.01;
    
    float strong = 1.0;
    float weak = 0.1;
    
    
    if(sobel.r > highThreshold || sobel.g > highThreshold || sobel.b > highThreshold)sobel = vec4(strong);
//    if(sobel.r < lowThreshold && sobel.g < lowThreshold   && sobel.b < lowThreshold){
//        sobel = vec4(weak);
//        bool isNeighBoorStrong = false;
//        for( int i = -1; i < 1; i++){
//            for( int j = -1; j < 1; j++){
//                vec4 sobelNeighboor = sobelEdge(imageTexture, vec2(uv.x+i, uv.y +j), imgRes);
//                if(     sobelNeighboor.r > highThreshold
//                   &&   sobelNeighboor.g > highThreshold
//                   &&   sobelNeighboor.b > highThreshold){
//                    isNeighBoorStrong = true;
//                }
//            }
//        }
//        if(isNeighBoorStrong)sobel = vec4(strong);
//        
//    }
//    
    
    return sobel;
}
