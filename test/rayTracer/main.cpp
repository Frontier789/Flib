#include <Frontier.hpp>
#include <iostream>
#include <set>

using namespace std;

template<class T>
struct Ray
{
	union {
		vector3<T> p,pos,position;
	};
	union {
		vector3<T> v,vel,velocity;
	};
	
	Ray(vector3<T> p = vector3<T>(),vector3<T> v = vector3<T>(1)) : p(p), v(v) {}
};

template<class T>
struct Intersection
{
	union {
		T t,time;
	};
	bool exist;

	Intersection(T t=T(),bool e=false) : t(t), exist(e) {}
};

template<class T>
Intersection<T> intersect(vector3<T> A,vector3<T> B,vector3<T> C,Ray<T> ray)
{
	vector3<T> n = (B-A).cross(B-C).sgn();

	T num = (B - ray.p).dot(n);
	T denom = ray.v.dot(n);

	T r = num/denom;

	vector3<T> p = ray.p + ray.v * r;

	matrix<3,2,T> m{(A-B).x,(C-B).x,(A-B).y,(C-B).y,(A-B).z,(C-B).z};
	auto mtm = m.t() * m;
	auto mtmi = mtm.inverse();
	auto mtmimt = mtmi * m.t();

	auto msol = mtmimt * matrix<3,1>{(p - B).x,(p-B).y,(p-B).z};
	auto sol = vec2(msol[0][0],msol[1][0]);


	return Intersection<T>(r, sol.x >= 0 && sol.x <= 1 && sol.y >= 0 && sol.y <= 1 && sol.x+sol.y <= 1);
}

int main()
{
	Image img(vec2(640,480));

	vec3 p,v(0,0,-1),u(0,1,0),r(1,0,0);
	float focal = 300;
	
	vec2i s = img.getSize();
	for (int i=0;i<s.w;++i)
	for (int j=0;j<s.h;++j) {
		vec3 pix = v*focal + u*(j - s.h/2.f) + r*(i - s.w/2.f);
		vec4 col(0,0,0,0);
		
		Ray<float> ray(p,pix);

		auto inters = intersect(vec3(-50,-50,-200),vec3(100,50,-200),vec3(-100,100,-200),ray);
		if (inters.exist) {
			col += vec4::White;
		} else {
			col += vec4::Black;
		}
		
		img.set(vec2i(i,j),col*255);
	}

	img.saveToFile("triangle.png");
}
