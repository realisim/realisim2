#version 410

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec2 vertexTexCoord;

// Values that stay constant for the whole mesh.
//
uniform dmat4 uProjectionMatrix;

// Output data ; will be interpolated for each fragment.
out vec2 oTexCoords;

void main(){

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  vec4( uProjectionMatrix * dvec4(vertexPosition_modelspace, 1) ); 
    oTexCoords = vertexTexCoord;
}