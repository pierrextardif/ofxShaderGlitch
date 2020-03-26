#version 330

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

in vec4 position;
in vec2 texcoord;
in vec4 normal;

out vec3 vecProj;
out vec3 normalProj;

out vec2 vUv;

void main()
{
    vUv = texcoord;
    
    vecProj = vec3(modelViewMatrix * position);
    normalProj = normalize(normalMatrix * normal).xyz;

    
    gl_Position = modelViewProjectionMatrix * position;
    
}
