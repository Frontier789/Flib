#version 110

#define FRONTIER_SECONDS

uniform vec2 u_winSize;

uniform float FRONTIER_SECONDS u_secs;

varying vec2 va_pos;



struct Ray
{
	vec3 pos;
	vec3 dir;
};


vec3 cam_pos   = vec3(0,0,0);
vec3 cam_dir   = vec3(0,0,1.0);
vec3 cam_up    = vec3(0,1,0);
vec3 cam_right = vec3(1,0,0);

vec3 sun_dir   = normalize(vec3(4.0,5.0,0.0));

int max_steps  = int(min(u_secs*5.0,128.0));
float min_dist = 0.01; 

Ray getViewRay()
{
	vec2 v = va_pos / u_winSize * 2.0 - vec2(1.0);
	
	return Ray(cam_pos,normalize(cam_dir*0.5 + v.x * cam_right / u_winSize.y * u_winSize.x + v.y * cam_up));
}

float dist_func(vec3 p)
{
	//return length(vec3(7.0,0.0,25.0) - p) - 9.0;
	return min(length(vec3(7.0,sin(u_secs*2.0)*3.0,12.0) - p) - 3.0, min(length(vec3(-7.0,0.0,12.0) - p) - 3.0,p.y+6.0));
}

vec4 get_color_at(vec3 p,int it,float shadow)
{
	float r = float(it)/float(max_steps);
	
	if (r < .3)
	{
		return vec4(mix(vec3(1.0,1.0,1.0),vec3(66.0,132.0,255.0)/255.0,r/.3),1.0);
	}
	
	return vec4(mix(vec3(66.0,132.0,255.0)/255.0,vec3(255.0,64.0,32.0)/255.0,(r-.3)/.7),1.0);
}

void raymarch(inout Ray ray,inout int it)
{
	for (int i=0;i<=max_steps;i++)
	{
		float dist = dist_func(ray.pos);
		
		it = i;
		
		if (dist < min_dist)
			i = max_steps;
		
		ray.pos += ray.dir * dist;
	}
}

void main()
{
	Ray ray = getViewRay();
	int it  = 0;
	
	raymarch(ray,it);
	
	if (it == max_steps)
		gl_FragColor = vec4(0.0,0.0,0.0,1.0);
	else
	{
		Ray shadow_ray = Ray(ray.pos + sun_dir * (min_dist + 0.1),sun_dir);
		
		gl_FragColor = get_color_at(ray.pos,it,0.0);
	}
}