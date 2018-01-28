#version 130

#define White vec4(1,1,1,0)
#define Black vec4(0,0,0,1)

in vec2 va_pos;
out vec4 out_color;

uniform int u_it;
uniform sampler2D u_palette;

struct Complex
{
	float x;
	float y;
};

/////////////////////////////////////////////////////////////
Complex comp_add(Complex c1,Complex c2)
{
	return Complex(c1.x + c2.x, c1.y + c2.y);
}

/////////////////////////////////////////////////////////////
Complex comp_mul(Complex c1,Complex c2)
{
	return Complex(c1.x * c2.x - c1.y * c2.y, c1.x * c2.y + c1.y * c2.x);
}

/////////////////////////////////////////////////////////////
Complex step(Complex xn,Complex c)
{
	return comp_add(comp_mul(xn,xn),c);  // xn*xn + c
}

/////////////////////////////////////////////////////////////
float distFromOrigo(Complex c)
{
	return sqrt(c.x*c.x + c.y*c.y);
}

/////////////////////////////////////////////////////////////
int mandel_steps(Complex c)
{
	Complex x = c; // x1

	for (int i=0;i<u_it;++i)
	{
		if (distFromOrigo(x) > 2) return i;

		x = step(x,c);
	}

	return u_it;
}

/////////////////////////////////////////////////////////////
vec4 mandel(Complex c)
{
	int i = mandel_steps(c);

	float rt = float(i) / float(u_it);

	return texture2D(u_palette,vec2(rt,0));
}

void main()
{
	Complex c = Complex(va_pos.x,va_pos.y);
	out_color = mandel(c);
}