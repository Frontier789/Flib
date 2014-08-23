#include <FRONTIER/Graphics/Color.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
	Color::Color() : r(255),
					 g(255),
					 b(255),
					 a(255)
	{

	}

	////////////////////////////////////////////////////////////
	Color::Color(unsigned char rgb) : r(rgb),
									  g(rgb),
									  b(rgb),
									  a(255)
	{

	}

	////////////////////////////////////////////////////////////
	Color::Color(unsigned char rgb,unsigned char a) : r(rgb),
									  				  g(rgb),
									  				  b(rgb),
									  				  a(a)
	{

	}

	////////////////////////////////////////////////////////////
	Color::Color(unsigned char r,unsigned char g,unsigned char b) : r(r),
																	g(g),
																	b(b),
																	a(255)
	{

	}

	////////////////////////////////////////////////////////////
	Color::Color(unsigned char r,unsigned char g,unsigned char b,unsigned char a) : r(r),
																					g(g),
																					b(b),
																					a(a)
	{

	}

	////////////////////////////////////////////////////////////
	Color::Color(const Color &copy) : r(copy.r),
									  g(copy.g),
									  b(copy.b),
									  a(copy.a)
	{

	}

	////////////////////////////////////////////////////////////
	Color::Color(const fm::vector3<float> &color) : r(color.r),
													g(color.g),
													b(color.b),
													a(255)
	{

	}

	////////////////////////////////////////////////////////////
	Color::Color(const fm::vector3<float> &color,float a) : r(color.r),
															g(color.g),
															b(color.b),
															a(a)
	{

	}

	////////////////////////////////////////////////////////////
	Color::Color(const fm::vector4<float> &color) : r(color.r),
													g(color.g),
													b(color.b),
													a(color.a)
	{

	}


    /// operators /////////////////////////////////////////////////////////
	Color &Color::operator()(unsigned char r,unsigned char g,unsigned char b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		return *this;
	}


    ////////////////////////////////////////////////////////////
	Color &Color::operator()(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		return *this;
	}

    ////////////////////////////////////////////////////////////
	Color Color::fromHex(unsigned int hex)
	{
        return Color(((hex>>24)%256),
					 ((hex>>16)%256),
                     ((hex>>8 )%256),
                     ((hex>>0 )%256));
	}

	/////////////////////////////////////////////////////////////
	Color::operator fm::vector4<float>() const
	{
		return fm::vector4<float>(float(r)/255.f,
								  float(g)/255.f,
								  float(b)/255.f,
								  float(a)/255.f);
	}


    /// color names /////////////////////////////////////////////////////////
	Color Color::White    	= Color(255,255,255,255);
	Color Color::Silver   	= Color(192,192,192,255);
	Color Color::Gray	  	= Color(128,128,128,255);
	Color Color::Grey	  	= Color(128,128,128,255);
	Color Color::Black    	= Color(0  ,0  ,0  ,255);
	Color Color::Red	  	= Color(255,0  ,0  ,255);
	Color Color::Brown    	= Color(128,64 ,0  ,255);
	Color Color::Yellow   	= Color(255,255,0  ,255);
	Color Color::Lime	  	= Color(0  ,255,0  ,255);
	Color Color::Green    	= Color(33 ,140,33 ,255);
	Color Color::Teal	  	= Color(0  ,128,128,255);
	Color Color::Blue	  	= Color(0  ,0  ,255,255);
	Color Color::Navy	  	= Color(0  ,0  ,128,255);
	Color Color::Purple   	= Color(142,0  ,214,255);
	Color Color::Maroon   	= Color(128,0  ,0  ,255);
	Color Color::Pink	  	= Color(255,0  ,255,255);
	Color Color::Olive	  	= Color(128,128,0  ,255);
	Color Color::Orange	  	= Color(255,165,0  ,255);
	Color Color::Aqua	  	= Color(0  ,255,255,255);
	Color Color::Bone	  	= Color(245,245,232,255);
	Color Color::Tan	  	= Color(210,180,140,255);
	Color Color::Charcoal 	= Color(70 ,70 ,70 ,255);
	Color Color::Royal	  	= Color(8  ,76 ,158,255);
	Color Color::Azure	  	= Color(0  ,127,255,255);
	Color Color::Aquamarine = Color(127,255,212,255);
	Color Color::Forest		= Color(34 ,139,34 ,255);
	Color Color::Gold		= Color(255,215,0  ,255);
	Color Color::Coral		= Color(255,127,80 ,255);
	Color Color::Lavender	= Color(181,126,220,255);
	Color Color::Indigo		= Color(75 ,0  ,130,255);


    ////////////////////////////////////////////////////////////
	Color &operator+=(Color &left,const Color &right)
	{
		left.r += right.r;
		left.g += right.g;
		left.b += right.b;
		left.a += right.a;
		return left;
	}


    ////////////////////////////////////////////////////////////
	Color &operator-=(Color &left,const Color &right)
	{
		left.r -= right.r;
		left.g -= right.g;
		left.b -= right.b;
		left.a -= right.a;
		return left;
	}


    ////////////////////////////////////////////////////////////
	Color &operator*=(Color &left,float right)
	{
		left.r *= right;
		left.g *= right;
		left.b *= right;
		left.a *= right;
		return left;
	}


    ////////////////////////////////////////////////////////////
	Color &operator/=(Color &left,float right)
	{
		left.r /= right;
		left.g /= right;
		left.b /= right;
		left.a /= right;
		return left;
	}


    ////////////////////////////////////////////////////////////
	Color operator+(const Color &left,const Color &right)
	{
		return Color(left.r+right.r,
					 left.g+right.g,
					 left.b+right.b,
					 left.a+right.a);
	}


    ////////////////////////////////////////////////////////////
	Color operator-(const Color &left,const Color &right)
	{
		return Color(left.r-right.r,
					 left.g-right.g,
					 left.b-right.b,
					 left.a-right.a);
	}


    ////////////////////////////////////////////////////////////
	Color operator*(const Color &left,float right)
	{
		return Color(float(left.r) * right,
					 float(left.g) * right,
					 float(left.b) * right,
					 float(left.a) * right);
	}


    ////////////////////////////////////////////////////////////
	Color operator/(const Color &left,float right)
	{
		return Color(float(left.r) / right,
					 float(left.g) / right,
					 float(left.b) / right,
					 float(left.a) / right);
	}


    ////////////////////////////////////////////////////////////
	bool operator==(const Color &left,const Color &right)
	{
		return (left.r==right.r) && (left.g==right.g) && (left.b==right.b) && (left.a==right.a);
	}


    ////////////////////////////////////////////////////////////
	bool operator!=(const Color &left,const Color &right)
	{
		return (left.r!=right.r) || (left.g!=right.g) || (left.b!=right.b) || (left.a!=right.a);
	}


}
