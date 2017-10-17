#version 140

#define FRONTIER_POS

uniform vec2 u_winSize;

in vec2 FRONTIER_POS in_pos;

out vec2 va_pos;

void main()
{
	gl_Position = vec4(in_pos*2.0 - vec2(1.0,1.0),0.0,1.0);
	
	va_pos = in_pos * u_winSize;
}

