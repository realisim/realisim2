#version 410 core

uniform bool uApplyLighting;
uniform dvec3 uLightPosition = dvec3(1.0, 1.0, 1.0);
uniform bool uUseSampler = false;
uniform sampler2D uDiffuseSampler;
uniform float uAlphaFactor; //mutiplies the alpha channel of color.

const dvec3 lightColor = dvec3(0.8, 0.8, 0.8);
const float ambientFactor = 0.05f;

// Interpolated values from the vertex shaders
in vec3 oVertex;
in vec3 oNormal;
in vec2 oTexCoords;
in vec4 oColor;

// Ouput data
out vec4 frag_color;

void main()
{
    vec4 finalColor = oColor;
    if(uUseSampler == true)
    {
        finalColor = texture(uDiffuseSampler, oTexCoords);
    }

    if(uApplyLighting == true)
    {
        vec3 normal = normalize(oNormal); 
        vec3 lightDir = normalize(vec3(uLightPosition));

        vec3 ambientC = ambientFactor * vec3(lightColor);

        float diff = max(dot(normal, lightDir), 0.0f);
        vec3 diffuseC = diff * vec3(lightColor);

        finalColor = vec4((ambientC + diffuseC) * finalColor.rgb, finalColor.a);
    }
    
    finalColor.a *= uAlphaFactor;

    // apply gamma correction
    //float gamma = 2.2;
    //finalColor.rgb = pow(finalColor.rgb, vec3(1.0/gamma));

    frag_color = finalColor;
}


