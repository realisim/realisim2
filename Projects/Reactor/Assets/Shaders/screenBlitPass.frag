#version 410 core

// Interpolated values from the vertex shaders
in vec2 oTexCoords;
uniform sampler2D uInput;

// Ouput data
out vec4 frag_color;

void main()
{
    vec4 finalColor = texture(uInput, oTexCoords); 

    // apply gamma correction
    //float gamma = 2.2;
    //finalColor.rgb = pow(finalColor.rgb, vec3(1.0/gamma));

    frag_color = finalColor; 
}


