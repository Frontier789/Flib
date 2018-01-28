#version 130

#define FRONTIER_SECONDS

uniform vec2 u_winSize;

uniform float FRONTIER_SECONDS u_secs;

in vec2 va_pos;

float pi = 3.14159265358979;
float zoom_time = 8.0;
float real_time = u_secs /* 0.0 + 8.0*/;
float time_ratio = sin(real_time / zoom_time * pi / 2.0 - pi / 2.0)*.5+.5;
float discrete_time = float(int(real_time / 4.0 / zoom_time));


vec2 comp_sqr(vec2 c)
{
	return vec2(c.x*c.x - c.y*c.y, 2.0*c.x*c.y);
}

float comp_len2(vec2 c)
{
	return c.x*c.x + c.y*c.y;
}

float mandelAt(vec2 p,bool continous)
{
	vec2 seed = p;
	vec2 z = p;

	for(int n=0; n<int(time_ratio * 180.0); n++)
	{
		z = comp_sqr(z) + seed;
		if(comp_len2(z) > 4.0) 
		{
			return continous ? (float(n) + 1.0 - log(log(length(z)))/log(2.0)) : float(n);
		}	
	}
	
	return 0.0;
}

vec4 mandelColor(float coef)
{/*
	return vec4(vec3(coef/100.0),1.0);
*/
	return vec4((-cos(0.038*coef)+1.0)/2.0, 
				(-cos(0.070*coef)+1.0)/2.0, 
				(-cos(0.161*coef)+1.0)/2.0, 
				1.0);
}

const vec2 pts[6] = vec2[](
	vec2(-0.7,0.35),
	vec2(-0.101115,0.956298),
	vec2(-0.74803,0.1),
	vec2(-0.000201 ,-0.813793),
	vec2(-0.745428,0.113009),
	vec2(-0.23514,0.827215)
);

out vec4 out_color;

void main()
{
	int id = int(discrete_time) % 6;
	vec2 offset = pts[id];
	
	float zoom = pow(2.0,time_ratio * 18.0);
	
	vec2 pt = (va_pos - u_winSize/2.0) / 200.0 / zoom + offset;
	
	out_color = mandelColor(mandelAt(pt,true));
}