#version 410 core

// Interpolated values from the vertex shaders
in vec2 oTexCoords;
uniform sampler2D uInput;
uniform bool uHorizontal;
uniform int uKernel = 5;

// Ouput data
out vec4 frag_color;

void main()
{
    vec2 increment = 1.0/textureSize(uInput, 0);
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    int kernel = max(uKernel, 1);
    int count = 0;
    vec2 displacement;
    for(int i = -kernel; i <= kernel; ++i )
    {
        if(uHorizontal)
            displacement = vec2(i * increment.x, 0);
        else
            displacement = vec2(0, i * increment.y);
        color += texture(uInput, oTexCoords + displacement);
        count++;
    }

    vec4 finalColor = color/count;

    frag_color = finalColor; 
}


