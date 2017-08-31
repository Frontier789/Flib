#version 110

#define FRONTIER_SECONDS

uniform vec2 u_winSize;

uniform float FRONTIER_SECONDS u_secs;

varying vec2 va_pos;

float pi = 3.14159265358979;
float time_ratio = sin(u_secs / 12.0 * pi / 2.0 - pi / 2.0)*.5+.5;


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

void main()
{
	vec2 offset = vec2(-0.7,0.35);
	
	float zoom = pow(2.0,time_ratio * 18.0);
	
	vec2 pt = (va_pos - u_winSize/2.0) / 200.0 / zoom + offset;
	
	gl_FragColor = mandelColor(mandelAt(pt,true));
}