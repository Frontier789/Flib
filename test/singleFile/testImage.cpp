#include <Frontier.hpp>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	float S = 1011;
	
	Image img(vec2(S,S));
	
	float circleR = S/2-S/10-5;
	float circleS = S/10;
	
	Cv(img.size())
	{
		vec2 p(x,y);
		vec2 d = img.size()/2-p;
		float midDist = d.length();
		float circleDist = max<float>(0, circleS - abs(circleR-midDist) ) * (255 / circleS);
		
		circleDist = sin(circleDist/255 * 3.14159265358979 / 2)*255;
		
		Angle<float> a = pol2(d).angle;
		
		vec3 c(cos(a),cos(a*2 + deg(90)),cos(a*3 + deg(130)));
		
		img.at(p) = Color(circleDist*(c+vec3(1,1,1))/2,255);
	}
	
	
	fm::Result r = img.saveToFile("savedToFile.png");
	
	if (!r)
	{
		cout << r.toString() << endl;
	}
	
	
	img.saveToFile("savedToFile.tga");
	
	
	fm::Size bytes = 0;
	fm::Uint8 *ptr;
	img.saveToMemory(ptr,bytes,"tga");
	
	ofstream out("savedToMemory.tga",ios::binary);
	
	out.write((char*)ptr,bytes);
	
	delete[] ptr;
	
	
	
}
