#include "Triangle.hpp"

Triangle::Triangle() : state(0) {}
Triangle::Triangle(vec3 a,vec3 b,vec3 c,int state,int depth) : a(a), b(b), c(c), state(state), depth(depth) {}

void Triangle::populate(Triangle *&tris)
{
	if (state == 0)
	{
		vec3 midA = (b+c)/2;
		vec3 midB = (a+c)/2;
		vec3 midC = (b+a)/2;
		
		vec3 n = (a-b).cross(a-c).sgn();
		vec3 mid = (a+b+c)/3 + sqrt(5.f/6.f) / 2 * (a-b).length() * n;
		
		*(tris++) = Triangle(a,midC,midB,1,depth+1);
		*(tris++) = Triangle(b,midA,midC,1,depth+1);
		*(tris++) = Triangle(c,midB,midA,1,depth+1);
		
		*(tris++) = Triangle(midA,midB,mid,0,depth+1);
		*(tris++) = Triangle(midB,midC,mid,0,depth+1);
		*(tris++) = Triangle(midC,midA,mid,0,depth+1);
	}
	if (state == 1)
	{
		*(tris++) = *this;
	}
}

int Triangle::getNewPopCount()
{
	if (state == 0)
		return 6;
	
	return 1;
}
