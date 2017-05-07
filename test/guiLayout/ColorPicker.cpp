#include <Frontier.hpp>
#include "ColorPicker.hpp"

#include <iostream>
using namespace std;

// handle pos, write color

ColorPicker::ColorPicker(GuiContext &cont) : GuiElement(cont)
{
	m_drawData.positions = {vec2(50,0),vec2(0,100),vec2(100,100)};
	m_drawData.colors    = {vec4(1,0,0),vec4(0,1,0),vec4(0,0,1)};
	setSize(vec2(100,100));
	
	Mesh circle = Mesh::getCircle(6.0,42,[](float &d) -> float {
		float offset = cos(deg(d*1440 + 180));
		
		return 1 + 0.05 * (offset + 0.95);
	});
	circle.clr.resize(42);
	C(42)
		circle.clr[i] = vec4::White;
	circle.faces[0].primitive = fg::LineLoop;
	
	m_circle = DrawData(circle);
}
	
Color ColorPicker::getColorAt(vec2 p)
{
	vec2 p1 = vec2( 50,  0) + getPosition();
	vec2 p2 = vec2(  0,100) + getPosition();
	vec2 p3 = vec2(100,100) + getPosition();
	
	mat2 T(p1.x-p3.x, p2.x-p3.x,
		   p1.y-p3.y, p2.y-p3.y);
	
	vec2 lxy = T.inverse() * (p - p3);
	vec3 l = vec3(lxy,1 - lxy.x - lxy.y);
	
	return Color(l*256);
}

void ColorPicker::onDraw(fg::ShaderManager &shader)
{
	shader.getModelStack().push().mul(MATRIX::translation(getPosition()));
	shader.useTexture(nullptr);
	shader.draw(m_drawData);
	shader.getModelStack().pop();
	 
	shader.getModelStack().push().mul(MATRIX::translation(m_lastp));
	shader.useTexture(nullptr);
	shader.draw(m_circle);
	shader.getModelStack().pop();
}

void ColorPicker::onPress(fw::Mouse::Button button,fm::vec2 p)
{
	callCallback(getColorAt(p));
	m_lastp = p;
}

bool ColorPicker::contains(fm::vec2 p) const
{
	p -= getPosition();
	return p.x*2 + p.y >= 100 && p.x*2 - p.y <= 100 && p.y <= 100;
}

void ColorPicker::onMouseMoved(fm::vec2 p,fm::vec2 prevP)
{
	if (isPressed(Mouse::Left))
	{
		if (!mouseInside())
		{
			vec2 A = vec2(150,200)/3 + getPosition();
			vec2 B = p;
			
			C(15)
			{
				vec2 mid = (A + B)/2;
				if (contains(mid))
				{
					A = mid;
				}
				else
				{
					B = mid;
				}
			}
			
			p = A;
		}
		
		callCallback(getColorAt(p));
		m_lastp = p;
	}
}

bool ColorPicker::onEvent(fw::Event &ev)
{
	if (mouseInside())
	if (ev.type == fw::Event::MouseWheelMoved)
	{
		cout << "colorpicker scroll" << endl;
	}
	
	return false;
}

