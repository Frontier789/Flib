#version 430

layout(rgba8) uniform readonly  image2D u_inTex;
layout(rgba8) uniform writeonly image2D u_outTex;

uniform int u_kernSize;

layout (local_size_x = 1, local_size_y = 1) in;

float gauss(float dx,float dy,float sigma2)
{
	float d = dx*dx + dy*dy;
	return exp(-d / 2.0 / sigma2) / sqrt(2*3.1415*sigma2);
}

void main()
{
	vec4 sum = vec4(0,0,0,0);
	ivec2 index = ivec2(gl_GlobalInvocationID.xy);
	float len = 0;
	
	for (int dx=-u_kernSize;dx<=u_kernSize;++dx)
	for (int dy=-u_kernSize;dy<=u_kernSize;++dy)
	{
		sum += imageLoad(u_inTex, index + ivec2(dx,dy)) * gauss(dx,dy,10);
		len += gauss(dx,dy,10);
	}
	
	vec4 clr = sum / len + imageLoad(u_inTex, index);
	
	imageStore(u_outTex, index, clr);
}