#version 150

uniform mat4	ciModelViewProjection;
uniform mat3	ciNormalMatrix;

in vec4		ciPosition;
in vec2		ciTexCoord0;
in vec3		ciNormal;
in vec4		ciColor;
in vec4		vInstancePosition; // per-instance position variable
out highp vec2	TexCoord;
out lowp vec4	Color;
out highp vec3	Normal;

void main( void )
{
    vec4 p = ciPosition;
    p.x *= vInstancePosition.w*.65;
    p.y *= vInstancePosition.w*.65;
    p.z *= vInstancePosition.w*.65;
	gl_Position	= ciModelViewProjection * (  (p) + vec4( vInstancePosition.xyz, 0 ) );
	/*if (Color.w > .7)
        Color = ciColor;
    else*/
    Color 		= ciColor;// * vInstancePosition.w/4.0)*0.5;
	TexCoord	= ciTexCoord0;
	Normal		= ciNormalMatrix * ciNormal;
}
