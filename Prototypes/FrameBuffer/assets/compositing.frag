#version 410 core

// Interpolated values from the vertex shaders
in vec2 oTexCoords;
uniform sampler2D uOriginal;
uniform sampler2D uBloomed;

// Ouput data
out vec4 frag_color;

void main()
{
    vec4 ori = texture(uOriginal, oTexCoords);
    vec4 bloomed = texture(uBloomed, oTexCoords);

    frag_color = ori;
    if(bloomed.x > 0)
    {
        frag_color = ori + bloomed;
    }
    
}


