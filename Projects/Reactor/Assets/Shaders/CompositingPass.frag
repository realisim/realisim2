#version 410 core

// Interpolated values from the vertex shaders
in vec2 oTexCoords;
uniform sampler2D uSamplerBase;
uniform sampler2D uSamplerToAdd;

// Ouput data
out vec4 frag_color;

void main()
{
    vec4 colorA = texture(uSamplerBase, oTexCoords);
    vec4 colorB = texture(uSamplerToAdd, oTexCoords);

    colorA += colorB;
    frag_color = colorA; 
}


