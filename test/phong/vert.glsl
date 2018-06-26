#version 130

#define FRONTIER_MODEL
#define FRONTIER_VIEW
#define FRONTIER_PROJ
#define FRONTIER_POS
#define FRONTIER_NRM
#define FRONTIER_TAN
#define FRONTIER_BITAN
#define FRONTIER_NRMMAT
#define FRONTIER_TEXPOS
#define FRONTIER_TEXMAT

uniform mat4 FRONTIER_NRMMAT u_normMat;
uniform mat4 FRONTIER_TEXMAT u_texUVMat;
uniform mat4 FRONTIER_MODEL  u_modelMat;
uniform mat4 FRONTIER_VIEW   u_viewMat;
uniform mat4 FRONTIER_PROJ   u_projMat;

in vec3 FRONTIER_POS    in_pos;
in vec3 FRONTIER_NRM    in_nrm;
in vec3 FRONTIER_TAN    in_tan;
in vec3 FRONTIER_BITAN  in_bitan;
in vec2 FRONTIER_TEXPOS in_texpos;

out vec2 va_texpos;
out vec3 va_nrm;
out vec3 va_tan;
out vec3 va_bitan;
out vec3 va_pos;

void main()
{
	va_pos = vec3(u_modelMat * vec4(in_pos,1));
	va_nrm = vec3(u_normMat * vec4(in_nrm,1));
	va_tan = vec3(u_normMat * vec4(in_tan,1));
	va_bitan = vec3(u_normMat * vec4(in_bitan,1));
	va_texpos = (u_texUVMat * vec4(in_texpos,0.0,1.0)).xy;
	
	gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos,1.0);
}
