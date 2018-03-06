#version 430

layout(rgba8) uniform image2D u_inTex;
layout(rgba8) uniform image2D u_outTex;

layout (local_size_x = 1, local_size_y = 1) in;

void main()
{
	ivec2 index = ivec2(gl_GlobalInvocationID.xy);
	
	vec4 clr = imageLoad(u_inTex, index);
	
	imageStore(u_outTex, index, vec4(vec3(clr.r * 0.3 + clr.g * 0.65 + clr.b * 0.05),1));
}