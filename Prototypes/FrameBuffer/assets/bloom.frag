#version 410 core

// Interpolated values from the vertex shaders
in vec2 oTexCoords;
uniform sampler2D uSamplerInput;
uniform float uBloomFactor;

// Ouput data
out vec4 frag_color;

void main()
{
    // apply blur
    vec2 increment = 1.0/textureSize(uSamplerInput, 0);
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    int kernel = max(int(20.0 * uBloomFactor), 1);
    int count = 0;
    vec2 displacement;
    for(int j = -kernel; j <= kernel; ++j )
        for(int i = -kernel; i <= kernel; ++i )
        {
            displacement = vec2(i * increment.x, j * increment.y);
            color += texture(uSamplerInput, oTexCoords + displacement);
            count++;
        }
    vec4 finalColor = color/count;
    frag_color = vec4(finalColor.r * 1.3, 0.0, finalColor.b * 1.2, 0.0);
}


