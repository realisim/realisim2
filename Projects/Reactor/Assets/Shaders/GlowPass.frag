#version 410 core

uniform bool uUseSampler = false;
uniform sampler2D uGlowSampler;
uniform vec2 uTextureScaling = vec2(1.0, 1.0);

// Interpolated values from the vertex shaders
in vec3 oVertex;
in vec2 oTexCoords;

// Ouput data
out vec4 frag_color;

void main()
{
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
    if(uUseSampler == true)
    {
        finalColor = texture(uGlowSampler, oTexCoords * uTextureScaling);
    }

    frag_color = finalColor;
}


