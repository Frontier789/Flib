#version 130

#define FRONTIER_CAM_POS

uniform vec3 FRONTIER_CAM_POS u_camP;

uniform sampler2D u_atex;
uniform sampler2D u_stex;
uniform sampler2D u_ntex;

in vec2 va_texpos;
in vec3 va_nrm;
in vec3 va_pos;
in vec3 va_tan;
in vec3 va_bitan;

out vec4 out_clr;

vec3 sun_dir = normalize(vec3(3,9,6));

void main()
{
	vec3 m = (texture(u_ntex,va_texpos).rgb - vec3(.5))*2;
	vec3 n = normalize(va_nrm)*m.b + normalize(va_bitan)*m.g + normalize(va_tan)*m.r;
	vec3 v = normalize(va_pos - u_camP);
	vec3 r = reflect(v,n);
	
	float dp = max(dot(sun_dir,n),0);
	float sp = max(dot(sun_dir,r),0) * (dp > 0 ? 1 : 0);
	
	vec3 ambient  = texture(u_atex,va_texpos).rgb;
	vec3 specular = texture(u_stex,va_texpos).rgb;
	
	out_clr = vec4(specular * pow(sp*1.1,128) + ambient * (dp + .1),1);
	//out_clr = vec4(normalize(abs(va_bitan)),1);
}
