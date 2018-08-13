#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <Frontier.hpp>

class Triangle
{
public:
	vec3 a,b,c;
	int state;
	int depth;
	
	Triangle();
	Triangle(vec3 a,vec3 b,vec3 c,int state = 0,int depth = 0);
	void populate(Triangle *&tris);
	int getNewPopCount();
};

#endif