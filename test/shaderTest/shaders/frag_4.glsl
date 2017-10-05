#version 130

#define FRONTIER_SECONDS
#define FRONTIER_PLY_POS
#define FRONTIER_VIEW_DIR

uniform vec2 u_winSize;

uniform float FRONTIER_SECONDS u_secs;
uniform vec3 FRONTIER_PLY_POS u_camPos;
uniform vec3 FRONTIER_VIEW_DIR u_camDir;

in vec2 va_pos;

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

vec3 sun_dir   = normalize(vec3(4.0,-5.0,5.0));

int max_steps   = int(min(u_secs*5.0,64.0));
float step_size = 0.3;
float min_dist  = 0.01; 

Ray getViewRay()
{
	vec2 v = va_pos * 2.0 - u_winSize;
	
	return Ray(cam_pos,normalize(cam_dir * focal_dist + v.x * cam_right + v.y * cam_up));
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float max_height = 1.0 / 34.0 + 
				   1.0 / 24.0 + 
				   1.0 / 21.0 + 
				   1.0 / 24.0 + 
				   1.0 / 27.0 +
				   1.0 / 17.0  - 1.0;

float height_func(vec2 p)
{
	return sin((length(p) - u_secs) * 2.0) / 34.0 + 
		   sin((length(p - vec2(2.0)) - u_secs) * 4.0 + 1.0) / 24.0 +
		   sin((length(p - vec2(3.0,5.0)) - u_secs) * 5.0 + 1.5) / 21.0 +
		   sin((length(p - vec2(-2.0,-1.0)) - u_secs) * 4.5 + 1.0) / 24.0 +
		   sin((length(p - vec2(-3.0,2.0)) - u_secs) * 4.0 + 1.5) / 27.0 +
		   sin((length(p - vec2(10.0,30.2)) - u_secs*2.0) * 4.0 + 1.0) / 17.0 - 1.0; 
	
	return -6.0 + sin(p.x) + cos(p.y) + p.y*p.y/100.0;
}

vec3 get_normal(vec2 p)
{
	float d = 0.001;
	
	vec3 A = vec3(p.x+d,height_func(p+vec2(d,0.0)),p.y);
	vec3 B = vec3(p.x-d,height_func(p-vec2(d,0.0)),p.y);
	vec3 C = vec3(p.x,height_func(p+vec2(0.0,d)),p.y+d);
	vec3 D = vec3(p.x,height_func(p-vec2(0.0,d)),p.y-d);
	
	return cross(normalize(A-B),normalize(C-D));
}

void raymarch(inout Ray ray,inout int it)
{
	float t = (max_height - ray.pos.y) / ray.dir.y;
	
	if (t < 0.0)
	{
		it = max_steps;
		return;
	}
	
	ray.pos += ray.dir * t;
	
	for (int i=0;i<=max_steps;i++)
	{
		it = i;
		
		float h = height_func(ray.pos.xz);
		
		if (h > ray.pos.y)
		{
			i = max_steps;
			
			float lim_up   = 0.0;
			float lim_down = -step_size;
			
			for (int j=0;j<6;j++)
			{
				float mid = (lim_up + lim_down) / 2.0;
				
				vec3 pt = ray.pos + ray.dir * mid;
				
				if (height_func(pt.xz) > pt.y)
				{
					lim_up = mid;
				}
				else
				{
					lim_down = mid;
				}
			}
			
			ray.pos += ray.dir * lim_up;
		}
		else
		{
			ray.pos += ray.dir * step_size;
		}
	}
}

vec4 get_sky_color(vec3 dir)
{
	float dp = max(dot(dir,-sun_dir),0.0);
	
	vec3 rcolor = vec3(rand(dir.xy)) / 10.0;
	
	vec3 sun_and_sky = mix(vec3(1.0,0.9,0.5),vec3(0.0,0.5,1.0),max(min(pow(1.0-dp,2.0)*1000.0,1.0),0.0));
	
	vec3 green = vec3(max(.7 - dir.y,0.0) * max(dot(-sun_dir.xz,dir.xz),-1.0) * 1.3,0.0,0.0);
	
	return vec4(green + sun_and_sky + rcolor,1.0);
}

float get_fog(vec3 p)
{
	float d = length(p - cam_pos);
	
	return min(max(d / 100.0,0.0),1.0);
}

vec4 get_color(Ray ray,int it)
{
	float r = float(it)/float(max_steps);
	
	vec3  norm  = get_normal(ray.pos.xz);
	float shade = dot(norm,sun_dir)*.7 + .3;
	
	vec3 refl   = reflect(ray.dir,norm);
	
	float dp = max(min(dot(ray.dir,norm),1.0),0.0);
	
	vec3 scene_color = vec3(shade) * mix(get_sky_color(refl).xyz,vec3(0.0,0.2,0.4),dp);
	
	float fog = get_fog(ray.pos);
	
	return vec4(mix(scene_color,vec3(.5),fog),1.0);
}

out vec4 out_color;

void main()
{
	Ray ray = getViewRay();
	int it  = 0;
	
	raymarch(ray,it);
	
	if (it == max_steps || ray.pos.y > max_height)
		out_color = get_sky_color(ray.dir);
	else
		out_color = get_color(ray,it);
}