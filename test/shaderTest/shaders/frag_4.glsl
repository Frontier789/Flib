#version 110

#define FRONTIER_SECONDS

uniform vec2 u_winSize;

uniform float FRONTIER_SECONDS u_secs;

varying vec2 va_pos;

uniform float u_fov_userdef;
float real_fov = 130.0 - u_fov_userdef * 70.0;

float focal_dist = u_winSize.x / tan(real_fov / 2.0 / 180.0 * 3.14159265358979);


struct Ray
{
	vec3 pos;
	vec3 dir;
};


vec3 cam_pos   = vec3(0,0,0);
vec3 cam_dir   = vec3(0,0,1);
vec3 cam_up    = vec3(0,1,0);
vec3 cam_right = cross(cam_up,cam_dir);

vec3 sun_dir   = normalize(vec3(-4.0,-5.0,5.0));

int max_steps   = int(min(u_secs*5.0,64.0));
float step_size = 1.0;
float min_dist  = 0.01; 

Ray getViewRay()
{
	vec2 v = va_pos * 2.0 - u_winSize;
	
	return Ray(cam_pos,normalize(cam_dir * focal_dist + v.x * cam_right + v.y * cam_up));
}

float height_func(vec2 p)
{
	return -6.0 + sin(p.x) + cos(p.y) + p.y*p.y/100.0;
}

vec3 get_normal(vec2 p)
{
	float d = 0.0001;
	
	vec3 A = vec3(p.x+d,height_func(p+vec2(d,0.0)),p.y);
	vec3 B = vec3(p.x-d,height_func(p-vec2(d,0.0)),p.y);
	vec3 C = vec3(p.x,height_func(p+vec2(0.0,d)),p.y+d);
	vec3 D = vec3(p.x,height_func(p-vec2(0.0,d)),p.y-d);
	
	return cross(normalize(A-B),normalize(C-D));
}

void raymarch(inout Ray ray,inout int it)
{
	for (int i=0;i<=max_steps;i++)
	{
		it = i;
		
		float h = height_func(ray.pos.xz);
		
		if (h > ray.pos.y)
			i = max_steps;
		
		ray.pos += ray.dir * step_size;
	}
}

vec4 get_color(Ray ray,int it)
{
	float r = float(it)/float(max_steps);
	
	if (r < .3)
	{
		return vec4(mix(vec3(1.0,1.0,1.0),vec3(66.0,132.0,255.0)/255.0,r/.3),1.0);
	}
	
	return vec4(mix(vec3(66.0,132.0,255.0)/255.0,vec3(255.0,64.0,32.0)/255.0,(r-.3)/.7),1.0);
}

void main()
{
	Ray ray = getViewRay();
	int it  = 0;
	
	raymarch(ray,it);
	
	if (it == max_steps)
		gl_FragColor = vec4(vec3(0.0),1.0);
	else
		gl_FragColor = get_color(ray,it);
}