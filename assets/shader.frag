#version 150

in vec4	Color;
in vec3	Normal;

const vec4 fogColor = vec4(.98, .98,.98, 1.0);
const float FogDensity = 0.0002;

out vec4 			oColor;

void main( void )
{
    //float depth = gl_FragCoord.z / gl_FragCoord.w;
    
    float dist = 0;
    float fogFactor = 0;
    
    //compute distance used in fog equations
    dist = (gl_FragCoord.z / gl_FragCoord.w);
    dist = max(dist-500, 0);
    dist *= 1.2;
    
    // 20 - fog starts; 80 - fog ends
    //fogFactor = (1500.0 - dist)/(1500.0 - 400.0);
    //fogFactor = clamp( fogFactor, 0.0, 1.0 );
    
    fogFactor = 1.0 /exp( (dist * FogDensity)* (dist * FogDensity));
    fogFactor = clamp( fogFactor, 0.0, 1.0 );
    
    
    vec3 normal = normalize( -Normal );
    float diffuse = max( dot( normal, vec3( 0, 0, -1 ) ), 0 );
    if (Color.y > .5)
        diffuse = 1;
    oColor = mix(fogColor, Color*diffuse, fogFactor);
}