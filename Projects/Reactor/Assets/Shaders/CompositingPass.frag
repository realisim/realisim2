#version 410 core

// Interpolated values from the vertex shaders
in vec2 oTexCoords;
uniform sampler2D uSamplerBase;
uniform sampler2D uSamplerToAdd0;
uniform sampler2D uSamplerToAdd1;

// Ouput data
out vec4 frag_color;

void main()
{
    vec4 colorA = texture(uSamplerBase, oTexCoords);
    vec4 colorB = texture(uSamplerToAdd0, oTexCoords);
    vec4 colorC = texture(uSamplerToAdd1, oTexCoords);

    colorA += colorB + colorC;
    frag_color = colorA; 
}


