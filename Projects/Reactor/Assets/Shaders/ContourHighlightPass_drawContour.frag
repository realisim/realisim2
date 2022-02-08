
#version 410 core

// Interpolated values from the vertex shaders
in vec2 oTexCoords;
uniform sampler2D uInput;
uniform int uKernel = 3;
uniform vec3 uContourColor = vec3(1.0, 1.0, 1.0);

// Ouput data
out vec4 frag_color;

void main()
{
    vec2 increment = 1.0/textureSize(uInput, 0);
    int kernel = max(uKernel, 1);
    vec4 colorSample;
    int count = 0;
    vec2 displacement;
    for(int j = -kernel; j <= kernel; ++j )    
        for(int i = -kernel; i <= kernel; ++i )
        {
            displacement = vec2(i * increment.x, j * increment.y);
            colorSample = texture(uInput, oTexCoords + displacement);
            if(colorSample.r >= 1.0)
                count += 1;
        }
    
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);;
    if(count > 0 && count < (kernel+1)*(kernel+1) )
        finalColor = vec4(uContourColor, 0.0);
    frag_color = finalColor; 
}
