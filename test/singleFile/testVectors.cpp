#include <Frontier.hpp>
#include <iostream>

using std::cout;
using std::endl;

template<class T>
T fv3(vector2<T> v)
{
	return v.x + v.y;
}

int main()
{
	float v2[] = {8,9};
	float v3[] = {8,0,-1};
	float v4[] = {8,0,-1,7};
	
	cout << "Here are some vectors created with different constructors" << endl;
	cout << vec2() << "\t\t" << vec2(1) << "\t\t" << vec2(4,5)  << "\t\t" << vec2(v2) << endl;
	cout << vec3() << "\t\t" << vec3(4) << "\t\t" << vec3(42,2) << "\t\t" << vec3(2,8,9) << "\t\t" << vec3(v3) << endl;
	cout << vec4() << "\t\t" << vec4(8) << "\t\t" << vec4(4,0) << "\t\t" << vec4(2,8,9) << "\t\t" << vec4(2,8,9,2) << "\t\t" << vec4(v4) << endl;
	
	cout << endl;
	cout << "Here are some vector functions" << endl;
	cout << "  length" << endl;
	cout << "  " << vec2(3,4).length() << "    " << vec3(1,2,3).length() << "    " << vec4(5,6,7,8).length() << endl;
	cout << "  scalar product" << endl;
	cout << "  " << vec2(3,4).dot(vec2(7,8)) << "    " << vec3(1,2,3).dot(vec3(0,0,1)) << endl;
	cout << "  vectorial product" << endl;
	cout << "  " << vec2(3,4).perp() << "    " << vec3(1,2,3).cross(vec3(0,0,1)) << endl;
	cout << "  calling a vec function with different vec type" << endl;
	cout << "  " << fv3<float>(vec3f(1,2,3)) << "    " << fv3<int>(vec3f(1.1,2.9,5.3)) << endl;
	
	cout << "  operations between different vec types" << endl;
	cout << "  " << vec2(1,2) + vec3(4,5,6) << endl;
	cout << "  " << vec3(1,2,4) / vec2(2) << endl;
	cout << "  " << vec3(1,2,4) % vec2(0.6) << endl;
	cout << "  " << vec3(1,2,4) - vec4(0.6) << endl;
	cout << "  " << vec2(-5.2,3) % vec3(1.6) << endl;
	cout << "  " << vec4(-5.2,3,2,3) % vec3(1.6) << endl;
	
	vec3 a;
	vec2 b;
	
	a += b;
	b += a;
}
