    #version 410 core
    out vec4 FragColor;

    in vec4 clipSpace;

    uniform sampler2D reflectionTexture;
    uniform sampler2D refractionTexture;
    uniform sampler2D dudvMap; 
    uniform sampler2D normalMap; 

    uniform float moveFactor;//for dudv map
    in vec2 textureCoords;//for dudv map
   in vec3 toCameraVector; //for fresnel
   const float waveStrength = 0.02;//for dudv map

   uniform vec3 lightColour;
   in vec3 fromLightVector;

   const float shineDamper = 20.0;
   const float  reflectivity = 0.6;


   uniform sampler2D depthMap; //for soft edges
   float near = 10.0; 
    float far = 100000.0; 
 
    void main()
    {   
       vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;

    


    vec2 distortion = (texture(dudvMap, textureCoords + moveFactor).rg * 2.0 - 1.0) * waveStrength;
    
    vec2 reflectTexCoords = vec2(ndc.x, 1.0 - ndc.y) + distortion;
    vec2 refractTexCoords = ndc + distortion;

       //FOR SOFT EDGES:
       float depth = texture(depthMap, refractTexCoords).r;//sample depth by refracr depth map
       float floorDistance =2.0* near* far/(far+near- (2.0*depth - 1.0)*(far-near));  //distance from camera to floor (initially between 0 and 1, but not linear)
       depth = gl_FragCoord.z;
       float waterDistance = 2.0* near* far/(far+near- (2.0*depth - 1.0)*(far-near));

       float waterDepth = floorDistance - waterDistance;




    reflectTexCoords = clamp(reflectTexCoords, 0.001, 0.999);
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    vec4 reflectColour = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColour = texture(refractionTexture, refractTexCoords);

    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot (viewVector,vec3(0.0, 1.0, 0.0) );
    refractiveFactor = pow (refractiveFactor, 0.5);

   vec4 normalMapColour =texture(normalMap, distortion);
   vec3 normal = vec3(normalMapColour.r*2.0-1.0,normalMapColour.b, normalMapColour.g*2.0-1.0 );
   normal = normalize(normal);


   vec3 reflectedLight = reflect (normalize(fromLightVector), normal);
   float specular = max (dot (reflectedLight, viewVector), 0.0);
   specular = pow (specular, shineDamper);
   vec3 specularHighlights = lightColour *specular*reflectivity;

    FragColor = mix(reflectColour, refractColour, refractiveFactor);
    FragColor = mix(FragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2)+ vec4(specularHighlights, 0.0);
  //  FragColor.a =clamp( waterDepth/3.0, 0.0, 1.0);
    } 