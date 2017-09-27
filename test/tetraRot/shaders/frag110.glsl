#version 110

uniform sampler2D u_tex;

varying vec2 va_pos[7];

// 0.00598	0.060626	0.241843	0.383103	0.241843	0.060626	0.00598

void main()
{
	gl_FragColor = 
	texture2D(u_tex,va_pos[0]) * 0.006 + 
	texture2D(u_tex,va_pos[1]) * 0.061 + 
	texture2D(u_tex,va_pos[2]) * 0.242 + 
	texture2D(u_tex,va_pos[3]) * 0.383 + 
	texture2D(u_tex,va_pos[4]) * 0.242 + 
	texture2D(u_tex,va_pos[5]) * 0.061 + 
	texture2D(u_tex,va_pos[6]) * 0.006;
}