#version 150

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;

uniform int   u_doLight;
uniform float u_time;

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in vec2 geom_pos[];
in vec4 geom_color[];
in vec3 geom_normal[];

out vec4 frag_color;
out vec2 frag_subp;

void main()
{
	mat4 mvp  = u_projMat * u_viewMat * u_modelMat;
	vec3 norm = normalize(geom_normal[0]);
	
	float dotp = 1.0;
	
	if (u_doLight == 1)
	{
		float angle = (sin(u_time/3.0)+1.0)/2.0 * (3.14159265358979-0.6)+0.3;
		
		dotp = max(dot(norm,normalize(vec3(cos(angle),sin(angle),-sin(angle)))),0.1);		
	}
	
	frag_subp = vec2( 0.57735026919, 0.0); gl_Position = mvp * vec4( (geom_pos[0] + frag_subp),0.0,1.0); frag_color = geom_color[0]*dotp; EmitVertex();
	frag_subp = vec2( 0.28867513459, 0.5); gl_Position = mvp * vec4( (geom_pos[0] + frag_subp),0.0,1.0); frag_color = geom_color[0]*dotp; EmitVertex();
	frag_subp = vec2( 0.28867513459,-0.5); gl_Position = mvp * vec4( (geom_pos[0] + frag_subp),0.0,1.0); frag_color = geom_color[0]*dotp; EmitVertex();
	frag_subp = vec2(-0.28867513459, 0.5); gl_Position = mvp * vec4( (geom_pos[0] + frag_subp),0.0,1.0); frag_color = geom_color[0]*dotp; EmitVertex();
	frag_subp = vec2(-0.28867513459,-0.5); gl_Position = mvp * vec4( (geom_pos[0] + frag_subp),0.0,1.0); frag_color = geom_color[0]*dotp; EmitVertex();
	frag_subp = vec2(-0.57735026919, 0.0); gl_Position = mvp * vec4( (geom_pos[0] + frag_subp),0.0,1.0); frag_color = geom_color[0]*dotp; EmitVertex();
	
	EndPrimitive();
}

