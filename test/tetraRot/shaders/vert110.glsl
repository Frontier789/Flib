#version 110

#define FRONTIER_POS

uniform vec2 u_texSize;
uniform bool u_vertical;

attribute vec2 FRONTIER_POS in_pos;

varying vec2 va_pos[7];

void main()
{
	gl_Position = vec4(in_pos,0.0,1.0);
	
	vec2 rpos = in_pos * .5 + vec2(.5);
	
	if (u_vertical)
	{
		va_pos[0] = rpos + vec2( 3.,0.0) / u_texSize;
		va_pos[1] = rpos + vec2( 2.,0.0) / u_texSize;
		va_pos[2] = rpos + vec2( 1.,0.0) / u_texSize;
		va_pos[3] = rpos + vec2( 0.,0.0) / u_texSize;
		va_pos[4] = rpos + vec2(-1.,0.0) / u_texSize;
		va_pos[5] = rpos + vec2(-2.,0.0) / u_texSize;
		va_pos[6] = rpos + vec2(-3.,0.0) / u_texSize;
	}
	else
	{
		va_pos[0] = rpos + vec2(0.0, 3.) / u_texSize;
		va_pos[1] = rpos + vec2(0.0, 2.) / u_texSize;
		va_pos[2] = rpos + vec2(0.0, 1.) / u_texSize;
		va_pos[3] = rpos + vec2(0.0, 0.) / u_texSize;
		va_pos[4] = rpos + vec2(0.0,-1.) / u_texSize;
		va_pos[5] = rpos + vec2(0.0,-2.) / u_texSize;
		va_pos[6] = rpos + vec2(0.0,-3.) / u_texSize;
	}
}

