#version 430

uniform float roll;
uniform image2D destTex;
layout(rgba32f) uniform image2D addTex;

layout (local_size_x = 16, local_size_y = 16) in;

void main()
{
	ivec2 iPos = ivec2(gl_GlobalInvocationID.xy);

	vec2  localV = vec2(ivec2(gl_LocalInvocationID.xy) - ivec2(8));

	float localCoef = length(localV/8.0);

	float globalCoef = sin(length(gl_WorkGroupID.xy)*0.5 + roll)*1.3;
	
	vec4 added = imageLoad(addTex, iPos);
	
	imageStore(destTex, iPos, vec4(1.0-globalCoef*localCoef, 0.0, 0.0, 1.0) + added);
}