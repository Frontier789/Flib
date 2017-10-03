#version 130

#define FRONTIER_MODEL
#define FRONTIER_VIEW
#define FRONTIER_PROJ
#define FRONTIER_POS
#define FRONTIER_TEXPOS

uniform mat4 FRONTIER_MODEL  u_modelMat;
uniform mat4 FRONTIER_VIEW   u_viewMat;
uniform mat4 FRONTIER_PROJ   u_projMat;

in vec2 FRONTIER_POS in_pos;
in vec2 FRONTIER_TEXPOS in_uvs;

out vec2 va_texp;

void main()
{
	gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos,0.0,1.0);
	
	va_texp = in_uvs;
}