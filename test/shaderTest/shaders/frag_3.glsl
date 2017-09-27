#version 110

#define FRONTIER_SECONDS
#define FRONTIER_PLY_POS
#define FRONTIER_VIEW_DIR

uniform vec2 u_winSize;

uniform float FRONTIER_SECONDS u_secs;
uniform vec3 FRONTIER_PLY_POS u_camPos;
uniform vec3 FRONTIER_VIEW_DIR u_camDir;

varying vec2 va_pos;

uniform float u_k_userdef;

uniform float u_fov_userdef;
float real_fov = 130.0 - u_fov_userdef * 70.0;

float focal_dist = u_winSize.x / tan(real_fov / 2.0 / 180.0 * 3.14159265358979);


struct Ray
{
	vec3 pos;
	vec3 dir;
};


vec3 cam_pos   = u_camPos;
vec3 cam_dir   = u_camDir;
vec3 cam_right = normalize(cross(vec3(0,1,0),cam_dir));
vec3 cam_up    = cross(cam_dir,cam_right);

vec3 sun_dir   = normalize(vec3(4.0,5.0,-5.0));

int max_steps  = int(min(u_secs*5.0,64.0));
float min_dist = 0.01; 

Ray getViewRay()
{
	vec2 v = va_pos * 2.0 - u_winSize;
	
	return Ray(cam_pos,normalize(cam_dir * focal_dist + v.x * cam_right + v.y * cam_up));
}

float sphere_dist(vec3 p)
{
	return length(p) - 1.0;
}

float Union(float a,float b)
{
	return min(a,b);
}

float Substract(float a,float b)
{
	return max(a,-b);
}

float Modulate(float a,float m)
{
	return mod(a,m) - m/2.0;
}

float rander(float v)
{
	return fract(sin(v*4512.12 + 12.45)*7865.12);
}

float dist_func(vec3 p)
{
	return Union(
		Substract(
			Union(
				length(vec3(Modulate(p.x,10.0),Modulate(p.y - 5.0 - u_secs*5.0,10.0),p.z - 35.0)) - 3.0,
				p.y+6.0
			),
			length(vec3(Modulate(7.0 - p.x + rander(pow(1.5,p.y)),5.0),Modulate(sin(u_secs*2.0)-6.0 - p.y,5.0),Modulate(8.0 - p.z,5.0))) - 3.0	
		),
		length(vec3(Modulate(7.0 - p.x,5.0),sin(u_secs*2.0)-6.0 - p.y,8.0 - p.z)) - 1.0	- sin(u_secs)
	);
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
		
		gl_FragColor = get_color_at(ray.pos,it,1.0);
	}
}