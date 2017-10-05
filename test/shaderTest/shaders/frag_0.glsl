#version 130

#define FRONTIER_SECONDS

uniform vec2 u_winSize;

uniform float FRONTIER_SECONDS u_secs;

in vec2 va_pos;

out vec4 out_color;

void main()
{
	vec2  delta = va_pos - u_winSize / 2.0;
	float dOrigin = length(delta);
	vec2  direction = delta / dOrigin;
	float angle = acos(direction.x) * sign(direction.y);
	
	out_color = vec4(vec3(sin(dOrigin / 10.0 + cos(angle * 3.0 + dOrigin / (sin(u_secs) * 200.0)) * 10.0 - u_secs*10.0)*0.5 + 0.5),1.0);
}