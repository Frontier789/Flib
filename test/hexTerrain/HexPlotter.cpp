#include "HexPlotter.hpp"

HexPlotter::HexPlotter(bool useGeomShader) : useGeomShader(useGeomShader)
{
	
}

fm::Size HexPlotter::getVertexCountPerHex() const
{
	return useGeomShader ? 1 : 3*6;
}

void HexPlotter::addHex(vec2 midp,vec4 color,vec3 normal,vec2 *&pts,vec4 *&clr,vec2 *&tpt,vec3 *&nrm)
{
	if (useGeomShader)
	{
		*(pts++) = midp; *(clr++) = color; *(nrm++) = normal;
	}
	else
	{
		C(6)
		{
			vec2 OA = pol2(sqrt(3.0)/3.0,deg( 0 + 60*i));
			vec2 OB = pol2(sqrt(3.0)/3.0,deg(60 + 60*i));
			
			*(pts++) = midp;      *(clr++) = color; *(tpt++) = vec2(); *(nrm++) = normal;
			*(pts++) = midp + OA; *(clr++) = color; *(tpt++) = OA;     *(nrm++) = normal;
			*(pts++) = midp + OB; *(clr++) = color; *(tpt++) = OB;     *(nrm++) = normal;
		}
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
