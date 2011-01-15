[vertex]

attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_TexCoord0;

uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ProjectionMatrix;

varying vec2 vp_TexCoord;

void main()
{
	gl_FrontColor = vec4(vp_Color, 1.0);
	vp_TexCoord = vp_TexCoord0.st;
	gl_Position = vp_ProjectionMatrix * vp_ModelMatrix * vp_ViewMatrix * vec4(vp_Vertex, 1.0);
}

[fragment]

uniform sampler2D vp_Texture0;
varying vec2 vp_TexCoord;

uniform float vp_TexScroll;

void main(void)
{
	vec2 TexCoord = vp_TexCoord;
	TexCoord[0] += vp_TexScroll;
	//vp_TexCoord[1] += vp_TexScroll;
	
	vec4 clouds = texture2D(vp_Texture0, TexCoord);
	gl_FragColor = mix(gl_Color, vec4(1.0,1.0,1.0,1.0), clouds.a);
}