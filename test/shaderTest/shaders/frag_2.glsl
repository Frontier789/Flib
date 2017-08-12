#version 110

#define FRONTIER_SECONDS

uniform vec2 u_winSize;

uniform float u_k_userdef;

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

float sphere_dist(vec3 p)
{
	return length(p) - 1.0;
}

float Union(float a,float b)
{
	return min(a,b);
}

float dist_func(vec3 p)
{
	return Union(Union(sphere_dist(p + vec3(0.0,0.0,-10.0))-2.0,
					   sphere_dist(p + vec3(5.0,0.0,-10.0))-2.0),p.y+6.0);
}

float norm_helper_derivate(vec3 p,vec3 d,float h)
{
	return (dist_func(p+d) - dist_func(p-d)) / (2.0 * h);
}

vec3 get_normal(vec3 p)
{
	const float h = 0.00005;
	vec3 grad = vec3(norm_helper_derivate(p,vec3(h,0.0,0.0),h),
					 norm_helper_derivate(p,vec3(0.0,h,0.0),h),
					 norm_helper_derivate(p,vec3(0.0,0.0,h),h));
	
	return normalize(grad);
}

vec4 get_color_at(vec3 p,int it,float shadow)
{
	return vec4(vec3(dot(sun_dir,get_normal(p)) * shadow),1.0);
	
	return vec4(get_normal(p),1.0); // normal based
	
	return vec4(vec3(float(it)/float(max_steps)),1.0); // step count base
}

float get_shadow(in Ray ray)
{
	float res = 1.0;
	float all_dist = 0.0;
	
	for (int i=0;i<=max_steps;i++)
	{
		float dist = dist_func(ray.pos + ray.dir * all_dist);
		
		if (dist < min_dist)
			return 0.0;
		
		all_dist += dist;
		
		res = min(res,(u_k_userdef*15.0 + 2.0) * dist / all_dist);
	}
	
	return res;
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

vec4 calc_color()
{
	Ray ray = getViewRay();
	int it = 0;
	
	raymarch(ray,it);
	
	if (it == max_steps)
		return vec4(0.0,0.0,0.0,1.0);
	else
	{
		Ray shadow_ray = Ray(ray.pos + sun_dir * (min_dist + 0.1),sun_dir);
		
		float shadow = get_shadow(shadow_ray);
		
		return get_color_at(ray.pos,it,shadow);
	}
}

void main()
{
	gl_FragColor = calc_color();
}