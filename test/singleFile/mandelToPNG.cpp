#include <Frontier.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

class MandelPlotter
{
	vec2s  m_canvasSize;
	double m_scale;
	vec2d  m_plotMidp;
	Size   m_iterations;
public:
	
	MandelPlotter(vec2s imgSize = vec2s(100,100),double scale = 1.0,vec2d plotmidp = vec2(),Size its = 10) : m_canvasSize(imgSize),
																											 m_scale(scale),
																											 m_plotMidp(plotmidp),
																											 m_iterations(its)
	{
		
	}
	
	void setCanvasSize(vec2s size)
	{
		m_canvasSize = size;
	}
	
	void setPlotArea(float scale,vec2d plotMidp)
	{
		m_scale = scale;
		m_plotMidp = plotMidp;
	}
	
	void setIterations(Size its)
	{
		m_iterations = its;
	}
	
	float getRelativeIterationsOnPlane(vec2d pOnPlane)
	{
		auto compPow2 = [](vec2d a) -> vec2d {
			return vec2d(a.x*a.x - a.y*a.y,2*a.x*a.y);
		};
		
		vec2d z;
		vec2d c = pOnPlane;
		C(m_iterations)
		{
			if (z.LENGTH() >= 2*2)
			{
				float smoothval = i + 1 - log(log(z.length()))/log(2.f);
				return smoothval / m_iterations;
			}

			z = compPow2(z) + c; 
		}

		return 1;
	}

	Color getColorOnPlane(vec2d pOnPlane)
	{
		float ratio = getRelativeIterationsOnPlane(pOnPlane);
		if (ratio > 1) ratio = 1;
		if (ratio < 0) ratio = 0;

		return Color::Black * (1.0 - ratio) + Color::White * (ratio);
	}

	Image render()
	{
		Image img(m_canvasSize);
		
		Cv(m_canvasSize)
		{
			vec2d pOnCompPlane = p - vec2d(m_canvasSize)/2;
			
			pOnCompPlane *= m_scale;
			pOnCompPlane += m_plotMidp;
			
			img.at(p) = getColorOnPlane(pOnCompPlane);
		}
		
		return img;
	}
};

int main()
{
	MandelPlotter plotter(vec2(1280,768),0.003,vec2(-0.5,0),30);
	
	Image img = plotter.render();
	
	Result r = img.saveToFile("mandel.png");
	
	if (!r)
	{
		cout << r << endl;
	}
}
