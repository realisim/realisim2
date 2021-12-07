#version 410 core

// Interpolated values from the vertex shaders
in vec2 oTexCoords;
uniform sampler2D uInput;

// Ouput data
out vec4 frag_color;

void main()
{
    frag_color = texture(uInput, oTexCoords); 
}


