// uniforms


uniform vec2             u_AmntCells;
uniform vec2             u_OffsetCells;

#define M_PI 3.1415926535897932

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

float Hash21(vec2 p){
    p = fract(p * vec2( 123.34, 456.21));
    p += dot(p, p + 5.32);
    return fract(p.x*p.y);
}


float CellsCheck(vec2 uv_Norm){
    
    float cellXSize = fract( u_AmntCells.x * uv_Norm.x);
    float cellYSize = fract( u_AmntCells.y * uv_Norm.y);
    
    vec2 id = vec2(floor(u_AmntCells.x * uv_Norm.x), floor(u_AmntCells.y * uv_Norm.y));
    
    return hexagonalCell(vec2(cellXSize, cellYSize), vec2(0.1, 0.5), u_OffsetCells);
}
