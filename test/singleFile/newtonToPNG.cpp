#include <Frontier.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

class NewtonPlotter
{
	vec2s  m_canvasSize;
	double m_scale;
	vec2   m_plotMidp;
	Size   m_iterations;
	vector<vec2> m_polynomial;
	vec2   m_argument;
public:
	
	NewtonPlotter(vec2s imgSize = vec2s(100,100),
				  double scale = 1.0,
				  vec2 plotmidp = vec2(),
				  Size its = 10,
				  const vector<vec2> &poly = vector<vec2>(),
				  vec2 argument = vec2(1,0)) : m_canvasSize(imgSize),
											   m_scale(scale),
											   m_plotMidp(plotmidp),
											   m_iterations(its),
											   m_polynomial(poly),
											   m_argument(argument)
	{
		
	}
	
	vec2 comp_mul(vec2 a,vec2 b)
	{
		return vec2(a.x*b.x - a.y*b.y,a.y*b.x + a.x*b.y);
	}
	
	vec2 comp_div(vec2 a,vec2 b)
	{
		return comp_mul(a,b*vec2(1,-1)) / b.LENGTH();
	}
	
	vec2 calcP(vec2 z)
	{
		vec2 ret;
		vec2 r(1,0);
		for (vec2 c : m_polynomial)
		{
			ret += comp_mul(r,c);
			r = comp_mul(r,z);
		}
		
		return ret;
	}
	
	vec2 calcPdx(vec2 z)
	{
		vec2 ret;
		vec2 r(1,0);
		for (fm::Size i=1;i<m_polynomial.size();++i)
		{
			vec2 c = m_polynomial[i] * i;
			ret += comp_mul(r,c);
			r = comp_mul(r,z);
		}
		
		return ret;
	}
	
	vec2 nextZ(vec2 z)
	{
		return z - comp_mul(m_argument , comp_div(calcP(z) , calcPdx(z)));
	}
	
	Color fixColor(vec4 c)
	{
		return Color(min(c.r,255.f),min(c.g,255.f),min(c.b,255.f),min(c.a,255.f));
	}
	
	Color getColorOnPlane(vec2 compP)
	{
		vec2 p = compP;
		int i = 0;
		int ai = m_iterations;
		for(;i<m_iterations;++i)
		{
			vec2 np = nextZ(p);
			
			if ((p-np).LENGTH() < 0.01)
			{
				if (ai > i) ai = i;
			}
			
			p = np;
		}
		
		Color c = vec4(vec3(255,255,255) - vec3(ai,ai,ai)/m_iterations*255,255);
		
		return c;
	}
	
	Image render()
	{
		Image img(m_canvasSize);
		
		Cv(m_canvasSize)
		{
			vec2 pOnCompPlane = p - vec2(m_canvasSize)/2;
			
			pOnCompPlane *= m_scale;
			pOnCompPlane += m_plotMidp;
			
			img.at(p) = getColorOnPlane(pOnCompPlane);
		}
		
		Image blurred;
		blurred.create(img.getSize());
		
		Cv(img.getSize()-vec2(2,2))
		{
			vec4 c;
			c += vec4(img.getTexel(p+vec2(1,1)).rgba())*0.6;
			c += vec4(img.getTexel(p+vec2(0,1)).rgba())*0.10;
			c += vec4(img.getTexel(p+vec2(1,0)).rgba())*0.10;
			c += vec4(img.getTexel(p+vec2(2,1)).rgba())*0.10;
			c += vec4(img.getTexel(p+vec2(1,2)).rgba())*0.10;
			c.a = 255;
			
			blurred.setTexel(p+vec2(1,1),c);
		}
		
		return blurred;
	}
};

int main()
{
	NewtonPlotter plotter(vec2(2048,2048),0.003,vec2(),30,{vec2(-2,0),vec2(),vec2(),vec2(5,0),vec2(),vec2(),vec2(3,0),vec2(),vec2(),vec2(1,0)});
	
	Image img = plotter.render();
	
	Result r = img.saveToFile("newton.png");
	
	if (!r)
	{
		cout << r << endl;
	}
}
