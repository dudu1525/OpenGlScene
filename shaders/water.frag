    #version 410 core
    out vec4 FragColor;

    in vec4 clipSpace;

    uniform sampler2D reflectionTexture;
    uniform sampler2D refractionTexture;
    uniform sampler2D dudvMap; 
    uniform float moveFactor;//for dudv map
    in vec2 textureCoords;//for dudv map
   in vec3 toCameraVector; //for fresnel
   const float waveStrength = 0.02;//for dudv map


    void main()
    {   
       vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;

    vec2 distortion = (texture(dudvMap, textureCoords + moveFactor).rg * 2.0 - 1.0) * waveStrength;
    
    vec2 reflectTexCoords = vec2(ndc.x, 1.0 - ndc.y) + distortion;
    vec2 refractTexCoords = ndc + distortion;


    reflectTexCoords = clamp(reflectTexCoords, 0.001, 0.999);
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    vec4 reflectColour = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColour = texture(refractionTexture, refractTexCoords);

    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot (viewVector,vec3(0.0, 1.0, 0.0) );
    refractiveFactor = pow (refractiveFactor, 0.5);

    FragColor = mix(reflectColour, refractColour, refractiveFactor);
    FragColor = mix(FragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
    } 