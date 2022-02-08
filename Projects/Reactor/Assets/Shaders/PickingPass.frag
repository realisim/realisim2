#version 410 core

// Interpolated values from the vertex shaders
uniform vec4 uIdAsColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

// Ouput data
out vec4 frag_color;

void main()
{
    frag_color = uIdAsColor; 
}


