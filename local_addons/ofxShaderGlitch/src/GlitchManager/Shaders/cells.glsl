// uniforms


uniform vec2             u_OffsetCells;


#define M_PI 3.1415926535897932

// ==== Lines & clumns ==== //
float LinesCheck(vec2 uv_Norm){
    
    float insideLine = 0.0;
    float hPos = fract(u_amntLinesColumns.x * uv_Norm.x);
    
    float verticalID = floor(u_amntLinesColumns.x * uv_Norm.x);
    if(verticalID == floor(fract(u_time * u_speedLinesColumns.x) * u_amntLinesColumns.x))insideLine = 1.0;
    
    return insideLine;
}
float ColumnsCheck(vec2 uv_Norm){
    
    float insideColumn = 0.0;
    float hPos = fract(u_amntLinesColumns.y * uv_Norm.y);
    
    float horizontalID = floor(u_amntLinesColumns.y * uv_Norm.y);
    if(horizontalID == floor(fract(u_time * u_speedLinesColumns.y) * u_amntLinesColumns.y))insideColumn = 1.0;
    return insideColumn;
}

float ColOrLineCheck(vec2 uv_Norm){
    return min( (LinesCheck(uv_Norm) + ColumnsCheck(uv_Norm) ), 1.0f);
}

float ColAndLineCheck(vec2 uv_Norm){
    return (LinesCheck(uv_Norm) * ColumnsCheck(uv_Norm));

}

// ==== Cells ==== //

float hexagonalCell(vec2 cellXY, vec2 valSlopes, vec2 offset){

    float InsideCell = abs(sin(fract(cellXY.y) * M_PI) + sin(fract(cellXY.x) * M_PI));
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
    if( (cellXSize <= slope)&&(cellYSize <= A * cellXSize + B)){
       InsideCell = 0.0f;
    }
    A = (1 - heightSlope) / slope;
    B = heightSlope;
    // BOTTOM side
    if(     (cellXSize <= slope)&&
           (cellYSize >= A * cellXSize + B)){
        InsideCell = 0.0f;
        
    }

    
//    slope -= 2 * offset.x;
    heightSlope -= 2 * offset.y;
    // Right Side
    // TOP side
    A = heightSlope / slope;
    B = heightSlope - A;

    if( cellXSize >= slope &&
      (cellYSize <= A * cellXSize + B) ){
       InsideCell = 0.0f;
    }
    // BOTTOM side
    A = (heightSlope - 1) / slope;
    B = heightSlope - A;

    if( cellXSize >= slope &&
      (cellYSize >= A * cellXSize + B) ){
       InsideCell = 0.0f;
    }
    return min(InsideCell, 1.0f);
}

float CellsCheck(vec2 uv_Norm){
    
    float cellXSize = fract( u_amntLinesColumns.x * uv_Norm.x);
    float cellYSize = fract( u_amntLinesColumns.y * uv_Norm.y);
    
    vec2 id = vec2(floor(u_amntLinesColumns.x * uv_Norm.x), floor(u_amntLinesColumns.y * uv_Norm.y));
    
    return hexagonalCell(vec2(cellXSize, cellYSize), vec2(0.1, 0.5), u_OffsetCells);
}

// ==== Cells flip ==== //

vec4 texFlipV(sampler2DRect tex, vec2 uv, vec4 originalColors, vec2 uvNorms, bool horizontal, bool addGradiant){
    vec4 colors = originalColors;
    
    
    vec2 pos = floor(uvNorms * u_amntLinesColumns);
    
    if(u_gradStart.x < uvNorms.x && uvNorms.x < u_gradFinish.x && u_gradStart.y < uvNorms.y && uvNorms.y < u_gradFinish.y){
        
        vec2 newUVs = uv;
        if(mod(pos.y, 2.) == 0.){
            if(horizontal){
                newUVs = vec2(u_resImg - uv);
            }else{
                newUVs = vec2(u_resImg.x - uv.x, uv.y);
            }
        }
        
        colors = getTexel(tex, newUVs);
        if(addGradiant)colors = gradiantColor(uvNorms, u_gradStart, u_gradFinish, colors, u_gradiantColor);
    }
    
    return colors;
    
}

vec4 texFlipH(sampler2DRect tex, vec2 uv, vec4 originalColors, vec2 uvNorms, bool vertical, bool addGradiant){
    vec4 colors = originalColors;
    
    
    vec2 pos = floor(uvNorms * u_amntLinesColumns);
    
    if(u_gradStart.x < uvNorms.x && uvNorms.x < u_gradFinish.x && u_gradStart.y < uvNorms.y && uvNorms.y < u_gradFinish.y){
        
        vec2 newUVs = uv;
        if(mod(pos.x, 2.) == 0.){
            if(vertical){
                newUVs = vec2(u_resImg - uv);
            }else{
                newUVs = vec2(uv.x, u_resImg.y - uv.y);
            }
        }
        
        colors = getTexel(tex, newUVs);
        if(addGradiant)colors = gradiantColor(uvNorms, u_gradStart, u_gradFinish, colors, u_gradiantColor);
    }
    
    return colors;
    
}

float doInnerFlip(vec2 uvNorm){
    vec2 inside = uvNorm;
    
    float prop = 0.;
    if(u_gradStart.x < inside.x && inside.x < u_gradFinish.x && u_gradStart.y < inside.y && inside.y < u_gradFinish.y){
        prop = 1.0;
    }
    
    
    return prop;
}

// ==== Cells flip ==== //


// ==== Generative Pattern 1 ==== //

int cutTextureFDiagonal(vec2 uv, int cutAngle, vec4 angles){
    
    int col = 1 - cutAngle;
    if(uv.x > -uv.y + angles.x * 2. - 1.) col = cutAngle;
    if(uv.x < -uv.y + angles.x * 2. - 1.) col = 1 - cutAngle;
    if(uv.x < -uv.y + angles.y * 2. - 1.) col = cutAngle;
    if(uv.x < -uv.y + angles.z * 2. - 1.) col = 1 - cutAngle;
    if(uv.x < -uv.y + angles.w * 2. - 1.) col = cutAngle;
 
    return col;
}

int cutTextureSDiagonal(vec2 uv, int cutAngle, vec4 angles){
    
    int col = 1 - cutAngle;
    
    if(uv.x > uv.y + angles.x * 2. - 1.) col = cutAngle;
    if(uv.x < uv.y + angles.x * 2. - 1.) col = 1 - cutAngle;
    if(uv.x < uv.y + angles.y * 2. - 1.) col = cutAngle;
    if(uv.x < uv.y + angles.z * 2. - 1.) col = 1 - cutAngle;
    if(uv.x < uv.y + angles.w * 2. - 1.) col = cutAngle;
 
    return col;
}

vec4 Cuts(float zoom){
    return vec4(mod(fract(zoom * 21.31), .2) + .7,
                mod(zoom * 11.1, .2) + .45,
                mod(fract(zoom * 6.563), .15) + .2,
                mod(zoom * 6.666, .2));
}

   
float designDiagonalPattern(vec2 uv, float zoom, vec2 time, vec2 speed){
    
    int col = 0;
    
    uv += speed * time.x;
    uv *= zoom * 2.;
    
    
    vec2 p = fract(uv) - .5;
    vec2 id = floor(uv);
    
    float side = Hash21(id);
    
    vec4 cuts = Cuts(zoom * id.x * id.y);
    
    if(side >= .5)col += cutTextureSDiagonal(p, int(time.y), cuts );
    if(side <  .5)col += cutTextureFDiagonal(p, int(time.y), cuts );
    
    return float(col);
}
// use it by doing :

float ImageCut(vec2 uv, float AmntLayers, vec2 time){
    float subCol = 0.;
    
    float k = 0.;
    
//    float coeff = floor((.5 * sin(time.y) + .5 ) * AmntLayers);
    time.y = int(mod(floor(time.y * 5.), 2.0) );
    
    float offsetK = 0.;
    if( mod(AmntLayers, 2) == 0.)offsetK = 1.;
    
    for( k = offsetK; k < AmntLayers; k += 1.){

        float z = Hash21(vec2(k, k + 1.) );
        
        // round not accepted with this version of OpenGL!
        if(z < .5){
            z = 0.;
        }else{
            z = 1.;
        }
        
        float rMod = mod(z + 2.52, 1.0);
        if(rMod < .5){
            rMod = 0.;
        }else{
            rMod = 1.;
        }
        
        float currentCoef = 1. / (AmntLayers);
//        if(coeff == k)currentCoef += .2;
        
        
        vec2 speed = vec2( ((1. - k / AmntLayers ) - .5) * z,
                           (k / AmntLayers* .5 - .25) * rMod);
        subCol += currentCoef * designDiagonalPattern(uv, k,  time, speed);
    
    }
    
    if(subCol < .5){
        subCol = 0.;
    }else{
        subCol = 1.;
    }
    
    return subCol;
}


// ==== Generative Pattern 2 ==== //

bool addDotsWavy(vec2 uv, float probability){
    
    float preciGrid = 900.0;
    float hRank = sin(  floor(uv.x * preciGrid + 2.0));
    float vRank = cos(floor((uv.y + u_time * .2) * preciGrid));
    float randVal = random(vec2(hRank * sin(u_time * 0.000002), vRank * cos(u_time * 0.000001)));
    
    float pseudoRX = fract((sin(hRank * 2.0) - randVal * u_time * 0.3) );
    float pseudoRY = fract(cos(uv.y * 10.0 + u_time * 0.01) + u_time* 0.2);
    if( (pseudoRX * pseudoRY / randVal) < 0.8){
        return false;
    }else{
        return true;
    }
}

bool addDots(vec2 uv, float probability){
    
    float preciGrid = 900.0;
    float hRank = sin(floor(uv.x * preciGrid ));
    float vRank = cos(floor((uv.y + u_time * 0.02) * preciGrid));
    float randVal = random(vec2(hRank * sin(u_time * 0.000002), vRank * cos(u_time * 0.000001)));
    

    float pseudoRX = fract( abs(cos(uv.x * 10.0 + u_time * 0.3) ) );
    float pseudoRY = fract( (abs(cos(uv.y * 10. - u_time) ) ));
    if( pseudoRX * pseudoRY / randVal < 0.8){
        return false;
    }else{
        return true;
    }
}
float waveDotX(vec2 uv, float speed, vec2 precisionGrid, float speedDots){
    float hRank = sin(floor((uv.x + speedDots)* precisionGrid.x));
    float vRank = cos(floor((uv.y) * precisionGrid.y));
    float randVal = random(vec2(hRank * sin(speed * 0.000001), vRank * cos(speed * 0.000002)));
    
    
    float pseudoRX = fract( randVal + (abs( cos( uv.x - speed) ) ));
    return mod(pseudoRX / randVal, 1.);
}

float waveDotY(vec2 uv, float speed, vec2 precisionGrid, float speedDots){
    float hRank = sin(floor(uv.x * precisionGrid.x ));
    float vRank = cos(floor((uv.y + speedDots) * precisionGrid.y));
    float randVal = random(vec2(hRank * sin(speed * 0.000002), vRank * cos(speed * 0.000001)));
    
    
    float pseudoRY = fract( randVal + (abs( cos( uv.y - speed) ) ));
    return mod(pseudoRY / randVal, 1.);
}

float addWavesDots(vec2 uv, int amnt, float mainSpeed, vec2 precisionGrid){
    
    float pos = 0.;
    for (int i = 1; i < amnt + 1; i++) {
        pos += waveDotY(uv * i, mainSpeed / i, precisionGrid * i, mainSpeed * .05) / (amnt);
        
    }
    return pos;
    
}
