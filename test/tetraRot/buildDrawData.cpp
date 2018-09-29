#include <Frontier.hpp>
#include "Triangle.hpp"
using std::vector;

void buildDrawData(int depth,DrawData &dd,DrawData &ddWf,DrawData &ddGw)
{
	Mesh m;
	vector<Triangle> triangles;
	
	float invSqrt2 = 1 / sqrt(2);
	vec3 basePts[] = {vec3(1,0,-invSqrt2),vec3(-1,0,-invSqrt2),vec3(0,1,invSqrt2),vec3(0,-1,invSqrt2)};
	
	triangles.push_back(Triangle(basePts[0],basePts[1],basePts[2]));
	triangles.push_back(Triangle(basePts[1],basePts[3],basePts[2]));
	triangles.push_back(Triangle(basePts[2],basePts[3],basePts[0]));
	triangles.push_back(Triangle(basePts[3],basePts[1],basePts[0]));
	
	C(depth)
	{
		int popCount = 0;
		for (auto &tri : triangles) popCount += tri.getNewPopCount();
		
		vector<Triangle> buf(popCount);
		
		Triangle *ptr = &buf[0];
		for (auto &tri : triangles) tri.populate(ptr);
		
		buf.swap(triangles);
	}
	
	m.pts.resize(triangles.size()*3);
	m.clr.resize(triangles.size()*3);
	
	vector<vec4> glowColors(triangles.size()*3);
	
	auto clrFromPos = [](vec3 p) -> vec4 {
		return vec4(p.sgn()/2 + vec3(.5,.5,.5),1);
	};
	
	auto glowFromTri = [&](const Triangle &t,int c) -> vec4 {
		
		if (t.depth < 2) return vec4::Black;
		
		if (c == 0) return clrFromPos(t.a);
		if (c == 1) return clrFromPos(t.b);
		if (c == 2) return clrFromPos(t.c);
		
		return vec4::White;
	};
	
	C(triangles.size())
	{
		m.pts[i*3 + 0] = triangles[i].a; m.clr[i*3 + 0] = clrFromPos(triangles[i].a), glowColors[i*3 + 0] = glowFromTri(triangles[i],0);
		m.pts[i*3 + 1] = triangles[i].b; m.clr[i*3 + 1] = clrFromPos(triangles[i].b), glowColors[i*3 + 1] = glowFromTri(triangles[i],1);
		m.pts[i*3 + 2] = triangles[i].c; m.clr[i*3 + 2] = clrFromPos(triangles[i].c), glowColors[i*3 + 2] = glowFromTri(triangles[i],2);
	}
	
	dd = m;
	
	m.clr.swap(glowColors);
	
	ddGw = m;

	C(m.clr.size()) m.clr[i] = vec4::Black;
	
	ddWf = m;
	
}
