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
	bool exist;
	vector3<T> pos,bari;
};

template<class T>
Intersection<T> intersect(vector3<T> A,vector3<T> B,vector3<T> C,Ray<T> ray)
{
	vector3<T> n = (B-A).cross(B-C).sgn();

	T num = (B - ray.p).dot(n);
	T denom = ray.v.dot(n);

	vector3<T> p = ray.p + ray.v * num/denom;

	matrix<3,2,T> m(A-B,C-B);

	vec2 sol = (m.t()*m).i()*m.t() * matrix<3,1>(p - B);

	return Intersection<T>{sol.x >= 0 && sol.x <= 1 && sol.y >= 0 && sol.y <= 1 && sol.x+sol.y <= 1,p,vec3(sol,1-sol.x-sol.y)};
}

int main()
{
	vec3 p,v(0,0,-1),u(0,1,0),r(1,0,0);
	float focal = 300;
	
	Image img(vec2(640,480));
	Clock clk;
	
	vec2i s = img.getSize();
	for (int i=0;i<s.w;++i)
	for (int j=0;j<s.h;++j) {
		vec3 pix = v*focal + u*(j - s.h/2.f) + r*(i - s.w/2.f);
		vec4 col(0,0,0,0);
		
		Ray<float> ray(p,pix);

		auto inters = intersect(vec3(-50,-50,-200),vec3(100,50,-200),vec3(-100,100,-200),ray);
		if (inters.exist) {
			col += vec4(inters.bari,1);
		} else {
			col += vec4::Black;
		}
		
		img.set(vec2i(i,j),col*255);
	}
	cout << "calc took: " << clk.ms() << "ms" << endl;
	
	img.saveToFile("triangle.png");
}
