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
	T travel;
	vector3<T> pos,bari,norm;
};

template<class T>
struct Triangle
{
	vector3<T> A,B,C;
	vector3<T> n;
	
	matrix<2,3,T> gauss;
	
	void calcAux() {
		n = (B-A).cross(B-C).sgn();
		
		matrix<3,2,T> m(A-B,C-B);
		
		gauss = (m.t()*m).i()*m.t();
	}
};

template<class T>
Intersection<T> intersect(Triangle<T> triangle,Ray<T> ray)
{
	T num = (triangle.B - ray.p).dot(triangle.n);
	T denom = ray.v.dot(triangle.n);

	T r = num/denom;
	vector3<T> p = ray.p + ray.v * r;

	vec2 sol = triangle.gauss * matrix<3,1>(p - triangle.B);
	
	bool in = sol.x >= 0 && sol.x <= 1 && sol.y >= 0 && sol.y <= 1 && sol.x+sol.y <= 1;
	
	return Intersection<T>{in,r,p,vec3(sol,1-sol.x-sol.y),triangle.n};
}

int main()
{
	vec3 p,v(0,0,-1),u(0,-1,0),r(1,0,0);
	float focal = 300;
	
	Clock clk;
	vector<Triangle<float>> tris;
	Mesh m = Mesh::getDiamond(7,80);
	
	mat4 tr = MATRIX::translation(vec3(0,-40,-200));
	
	for (auto &face : m.faces) {
		m.forEachTriangle(face,[&](fm::Uint32 ind0,fm::Uint32 ind1,fm::Uint32 ind2){
			
			vec3 A = tr * vec4(m.pts[ind0],1);
			vec3 B = tr * vec4(m.pts[ind1],1);
			vec3 C = tr * vec4(m.pts[ind2],1);
			
			tris.push_back(Triangle<float>{A,B,C,vec3(),matrix<2,3>()});
		});
	}
	
	for (auto &a : tris) 
		a.calcAux();
	
	cout << "preparation took: " << clk.ms() << "ms" << endl;
	cout << "Tris count: " << tris.size() << endl;
	
	Image img(vec2(640,480));
	clk.restart();
	
	vec2i s = img.getSize();
	for (int i=0;i<s.w;++i)
	for (int j=0;j<s.h;++j) {
		vec3 pix = v*focal + u*(j - s.h/2.f) + r*(i - s.w/2.f);
		vec4 col(0,0,0,0);
		
		Ray<float> ray(p,pix);
		Intersection<float> finters{false,0,vec3(),vec3(),vec3()};
		
		for (auto &tri : tris) {
			auto inters = intersect(tri,ray);
			if (inters.exist && (!finters.exist || inters.travel < finters.travel))
				finters = inters;
		}
		
		if (finters.exist) {
			col += vec4(finters.norm.dot(vec3(-1,1,-1).sgn()),1);
		} else {
			col += vec4::Black;
		}
		
		col = col.clamp(0,1);
		
		img.set(vec2i(i,j),col*255);
	}
	cout << "calc took: " << clk.ms() << "ms" << endl;
	
	img.saveToFile("result.png");
}
