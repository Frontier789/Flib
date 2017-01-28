#include "HexPlotter.hpp"

HexPlotter::HexPlotter()
{
	
}

fm::Size HexPlotter::getVertexCountPerHex() const
{
	return 3*6;
}

void HexPlotter::addHex(vec2 midp,vec4 color,vec2 *&pts,vec4 *&clr,vec2 *&tpt)
{
	C(6)
	{
		vec2 OA = pol2(sqrt(3.0)/3.0,deg( 0 + 60*i));
		vec2 OB = pol2(sqrt(3.0)/3.0,deg(60 + 60*i));
		
		*(pts+0) = midp;      *(clr+0) = color; *(tpt+0) = vec2();
		*(pts+1) = midp + OA; *(clr+1) = color; *(tpt+1) = OA;
		*(pts+2) = midp + OB; *(clr+2) = color; *(tpt+2) = OB;
		
		pts += 3;
		clr += 3;
	}
}

void HexPlotter::onDraw(ShaderManager &shader)
{
	shader.draw(drawData);
}

void HexPlotter::onUpdate(const fm::Time &dt)
{
	(void)dt;
}
