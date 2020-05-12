
float Hash21(vec2 p){
    p = fract(p * vec2( 123.34, 456.21));
    p += dot(p, p + 5.32);
    return fract(p.x*p.y);
}

float Hash22(vec2 p) {
    p = fract(p*vec2(123.34, 456.21));
    p += dot(p, p+45.32);
    return clamp(fract(p.x * p.y), 0.0, 1.0);
}

float Hash23(float p) {
    p = fract(p * 456.21);
    p += exp(p * 6.23);
    return clamp(fract(p), 0.0, 1.0);
}

vec4 Hash4Vec(vec2 oP){
    oP += dot(oP, oP* 332.1);
    vec4 p = vec4(oP, oP.x * 5.683, oP.y * 27.49);
    p.xy *= 74.77;
    return vec4(fract(p.y * p.x), fract(dot(p.x, p.w)), fract(p.z), fract(p.y * p.y));
}

mat2 Rot(float a){
    float s=sin(a), c=cos(a);
    return mat2(c, -s, s, c);
}
