#version 110

#define FRONTIER_MODEL
#define FRONTIER_VIEW
#define FRONTIER_PROJ
#define FRONTIER_SECONDS
#define FRONTIER_POS
#define FRONTIER_CLR
#define FRONTIER_NRM
#define FRONTIER_TEXPOS

uniform mat4 FRONTIER_MODEL u_modelMat;
uniform mat4 FRONTIER_VIEW  u_viewMat;
uniform mat4 FRONTIER_PROJ  u_projMat;

uniform float FRONTIER_SECONDS u_time;
uniform int   u_doLight;

attribute vec2 FRONTIER_POS    in_pos;
attribute vec4 FRONTIER_CLR    in_color;
attribute vec3 FRONTIER_NRM    in_normal;
attribute vec2 FRONTIER_TEXPOS in_texpos;

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

