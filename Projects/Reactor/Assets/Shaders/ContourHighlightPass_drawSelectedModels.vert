#version 410

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec3 vertexNormal_modelspace;
//layout(location = 2) in vec2 vertexTexCoord;
//layout(location = 3) in vec4 vertexColor;

// Values that stay constant for the whole mesh.
//
uniform dmat4 uProjectionMatrix;
uniform dmat4 uViewMatrix;
uniform dmat4 uModelMatrix;

// Output data ; will be interpolated for each fragment.

void main(){

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  vec4( uProjectionMatrix * uViewMatrix * uModelMatrix * dvec4(vertexPosition_modelspace, 1) ); 
}