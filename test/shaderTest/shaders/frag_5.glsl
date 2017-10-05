#version 140

#define FRONTIER_SECONDS
#define FRONTIER_VIEWMAT

uniform vec2 u_winSize;

uniform float FRONTIER_SECONDS u_secs;
uniform mat4 FRONTIER_VIEWMAT u_viewMat;

in vec2 va_pos;

uniform float u_fov_userdef;
float real_fov = 130.0 - u_fov_userdef * 70.0;

float focal_dist = u_winSize.x / tan(real_fov / 2.0 / 180.0 * 3.14159265358979);


struct Ray
{
	vec3 pos;
	vec3 dir;
};

vec3 cam_pos   = vec3(inverse(u_viewMat) * vec4(0.0,0.0,0.0,1.0));
vec3 cam_dir   = vec3(inverse(u_viewMat) * vec4(0.0,0.0,-1.0,0.0));
vec3 cam_right = normalize(cross(vec3(0,1,0),cam_dir));
vec3 cam_up    = cross(cam_dir,cam_right);

vec3 sun_dir   = normalize(vec3(4.0,5.0,5.0));

int max_steps  = int(min(u_secs*5.0,42.0));
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

float torus_dist(vec3 p,float R,float r)
{
	vec2 q = vec2(length(p.xz)-R,p.y);
	return length(q)-r;
}

vec3 rotateX(vec3 p,float a)
{
	return vec3(p.x,p.y * cos(a) - p.z * sin(a),
					p.y * sin(a) + p.z * cos(a));
}

vec3 rotateY(vec3 p,float a)
{
	return vec3(p.x * cos(a) - p.z * sin(a),p.y,
				p.x * sin(a) + p.z * cos(a));
}

float Union(float a,float b)
{
	return min(a,b);
}

float Substract(float a,float b)
{
	return max(a,-b);
}

vec2 SubstractMat(float a,float b,float mat_a,float mat_b)
{
	if (a > -b) return vec2(a,mat_a);
	
	return vec2(-b,mat_b);
}

float Modulate(float a,float m)
{
	return mod(a,m) - m/2.0;
}

float Noodle(float a,float l)
{
	return (abs(a - l) - abs(a + l)) / 2.0 + a;
}

float cube_dist(vec3 p,vec3 b)
{
	vec3 d = abs(p) - b;
	return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float repc_dist(vec3 p,float s)
{
	vec3 d = abs(p) - vec3(s);
	return min(max(d.x,d.y),min(max(d.x,d.z),max(d.z,d.y)));
}

float dist_func(vec3 p)
{
	float d = cube_dist(p + vec3(0.0,0.0,10.0),vec3(3.0));
	
	float r = 1.0;
	
	for (int i=0;i<5;i++)
	{
		d = Substract(d,repc_dist(vec3(Modulate(p.x+3.0,r*6.0),
									   Modulate(p.y+3.0,r*6.0),
									   Modulate(p.z+13.0,r*6.0)),r));
		r /= 3.01 + r*0.1;
	}
	
	return d;
}

vec2 dist_func_mat(vec3 p)
{
	vec2 dmat = vec2(cube_dist(vec3(p.x,p.y,p.z+10.0),vec3(3.0)),0.0);
	
	float r = 1.0;
	
	for (int i=0;i<5;i++)
	{
		dmat = SubstractMat(dmat.x,repc_dist(vec3(Modulate(p.x+3.0,r*6.0),
									   Modulate(p.y+3.0,r*6.0),
									   Modulate(p.z+13.0,r*6.0)),r),dmat.y,float(i+1));
		r /= 3.01 + r*0.1;
	}
	
	return dmat;
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

uniform float u_pw_userdef;

float get_ao(vec3 p,vec3 n)
{
	float s = 0.0;
	float dtop = dist_func(p);
	
	for (int x=1;x<7;x++)
	{
		float f = pow(8.0,float(x)) / pow(8.0,6.0);
		
		float expected = dtop + f;
	
		float real = dist_func(p + n*f);
		
		s += (expected - real) / 3.0;
	}
	
	
	
	return 1.0 - min(s,1.0);
}

vec4 get_color_at(vec3 p,int it,float shadow)
{
	vec3 n = get_normal(p);
	
	float dp = max(dot(sun_dir,n),0.2);
	
	float camD = length(p - cam_pos);
	
	float fog = max(min(camD/100.0,1.0),0.0);
	
	shadow = shadow * .8 + .2;
	
	return vec4(mix(vec3(get_ao(p,n)*dp*shadow),vec3(.0,.0,.1),fog),1.0);
	/*
	return vec4(get_normal(p),1.0); // normal based
	
	return vec4(vec3(float(it)/float(max_steps)),1.0); // step count base
	*/
}

vec4 get_color_at_mat(vec3 p,int it,float shadow,float mat)
{
	vec3 n = get_normal(p);
	
	float dp = max(dot(sun_dir,n),0.2);
	
	float camD = length(p - cam_pos);
	
	float fog = max(min(camD/8.0,1.0),0.0);
	fog = fog*fog;
	
	shadow = shadow * .8 + .2;
	
	return vec4(mix(vec3(get_ao(p,n)*dp*shadow),vec3(.0,.0,.0),fog) * vec3(sin(mat+0.5)*.5+.5,sin(mat*3.2+.3)*.5+.5,sin(mat*2.0+.9)*.5+.5),1.0);
	/*
	return vec4(get_normal(p),1.0); // normal based
	
	return vec4(vec3(float(it)/float(max_steps)),1.0); // step count base
	*/
}

float get_shadow(in Ray ray)
{
	float res = 1.0;
	float all_dist = 0.0;
	
	for (int i=0;i<=max_steps/2;i++)
	{
		float dist = dist_func(ray.pos + ray.dir * all_dist);
		
		if (dist < min_dist)
			return 0.0;
		
		all_dist += dist;
		
		res = min(res,13.0 * dist / all_dist);
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


void raymarch_mat(inout Ray ray,inout int it,inout float mat)
{
	for (int i=0;i<=max_steps;i++)
	{
		vec2 distm = dist_func_mat(ray.pos);
		
		it = i;
		mat = distm.y;
		
		if (distm.x < min_dist)
			i = max_steps;
		
		ray.pos += ray.dir * distm.x;
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

vec4 calc_color_mat()
{
	Ray ray = getViewRay();
	int it = 0;
	
	float mat = 0.0;
	
	raymarch_mat(ray,it,mat);
	
	if (it == max_steps)
		return vec4(0.0,0.0,0.0,1.0);
	else
	{
		Ray shadow_ray = Ray(ray.pos + sun_dir * (min_dist + 0.1),sun_dir);
		
		float shadow = get_shadow(shadow_ray);
		
		return get_color_at_mat(ray.pos,it,shadow,mat);
	}
}

out vec4 out_color;

void main()
{
	out_color = calc_color_mat();
}