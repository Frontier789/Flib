#version 110

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;

uniform int   u_doLight;
uniform float u_time;

attribute vec2 in_pos;
attribute vec4 in_color;
attribute vec3 in_normal;
attribute vec2 in_texpos;

varying vec4 va_color;
varying vec2 va_texpos;

void main()
{
	gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos,0.0,1.0);
	
	float dotp = 1.0;
	
	vec3 norm = normalize(in_normal);
	
	if (u_doLight == 1)
	{
		float angle = (sin(u_time/3.0)+1.0)/2.0 * (3.14159265358979-0.6)+0.3;
		
		dotp = max(dot(norm,normalize(vec3(cos(angle),sin(angle),-sin(angle)))),0.1) ;		
	}
	
	va_color  = in_color * dotp;
	va_texpos = in_texpos;
}

