#version 130

#define White vec4(1,1,1,0)
#define Black vec4(0,0,0,1)

in vec2 va_pos;
out vec4 out_color;

uniform int u_it;

struct Complex
{
	float x;
	float y;
};

vec4 hsv(float h,float s,float v)
{
	vec3 c = vec3(h,s,v);
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return vec4(c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y),1.0);
}

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

	if (i == u_it) return Black;

	return hsv(float(i)/60.0,.87,min(float(i)/10,1));
}

void main()
{
	Complex c = Complex(va_pos.x,va_pos.y);
	out_color = mandel(c);
}