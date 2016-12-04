#include <Frontier.hpp>
#include <iostream>
using namespace std;

int main()
{
	Image img(vec2(101,101));
	
	Cxy(101,101)
	{
		vec2 p(x,y);
		vec2 d = vec2(50,50)-p;
		float midDist = d.length();
		float circleDist = max<float>(0, 15 - abs(40-midDist) ) * 15;
		
		img.at(p) = Color(circleDist,255);
	}
	
	img.saveToFile("asd.bmp");
}
