#version 130

#define FRONTIER_POS

uniform vec2 u_winSize;
uniform mat4 u_mvt;

in vec2 FRONTIER_POS in_pos;

out vec2 va_pos;

void main()
{
	gl_Position = vec4((in_pos*2 - vec2(1,1)) * vec2(1,-1),0,1);
	
	va_pos = (u_mvt * vec4(in_pos * u_winSize,0,1)).xy;
}

