
#version 410 core

// Interpolated values from the vertex shaders
in vec2 oTexCoords;
uniform sampler2D uInput;
uniform bool uHorizontal;
uniform int uKernel = 5;
float gaussianValues[6] = float[]( 0.227027, 
    0.1946, 0.1216, 0.054054, 0.016216, 0.005);

// Ouput data
out vec4 frag_color;

void main()
{
    vec2 increment = 1.0/textureSize(uInput, 0);
    vec4 color = texture(uInput, oTexCoords) * gaussianValues[0];
    int kernel = max(uKernel, 1);

    vec2 displacement0;
    vec2 displacement1;
    for(int i = 1; i <= kernel; ++i )
    {
        if(uHorizontal)
            displacement0 = vec2(i * increment.x, 0);
        else
            displacement0 = vec2(0, i * increment.y);
        displacement1 = -displacement0;
        color += texture(uInput, oTexCoords + displacement0) * gaussianValues[i];
        color += texture(uInput, oTexCoords + displacement1) * gaussianValues[i];
    }

    vec4 finalColor = color;
    //vec4 finalColor = texture(uInput, oTexCoords);

    frag_color = finalColor; 
}




//#version 410 core
//
//// Interpolated values from the vertex shaders
//in vec2 oTexCoords;
//uniform sampler2D uInput;
//uniform bool uHorizontal;
//uniform int uKernel = 5;
//
//// Ouput data
//out vec4 frag_color;
//
//void main()
//{
//    vec2 increment = 1.0/textureSize(uInput, 0);
//    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
//    int kernel = max(uKernel, 1);
//    int count = 0;
//    vec2 displacement;
//    for(int i = -kernel; i <= kernel; ++i )
//    {
//        if(uHorizontal)
//            displacement = vec2(i * increment.x, 0);
//        else
//            displacement = vec2(0, i * increment.y);
//        color += texture(uInput, oTexCoords + displacement);
//        count++;
//    }
//
//    vec4 finalColor = color/count;
//
//    frag_color = finalColor; 
//}


