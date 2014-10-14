////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_COLOR_HPP_INCLUDED
#define FRONTIER_COLOR_HPP_INCLUDED
#include <FRONTIER/Config.hpp>
#define FRONTIER_COLOR
namespace fm
{
	template<class>
	class vector3;
	template<class>
	class vector4;
}
namespace fg
{
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Class used to hold a color's value in <a href="http://en.wikipedia.org/wiki/RGBA_color_space">RGBA color space</a>
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Color
	{
	public:
		typedef unsigned char value_type;
		typedef Color &reference;
		typedef const Color &const_reference;
		enum {
			components = 4u ///< Public value indicating the amount of value_type's in the class
		};

		static Color White;     ///< 0xFFFFFFFF
		static Color Silver;    ///< 0xBFBFBFFF
		static Color Gray;      ///< 0x7B7B7BFF
		static Color Grey;      ///< 0x7B7B7BFF
		static Color Black;     ///< 0x000000FF
		static Color Red;       ///< 0xFF0000FF
		static Color Brown;     ///< 0x7B4000FF
		static Color Yellow;    ///< 0xFFFF00FF
		static Color Lime;      ///< 0x00FF00FF
		static Color Green;     ///< 0x218C21FF
		static Color Teal;      ///< 0x007B7BFF
		static Color Blue;      ///< 0x0000FFFF
		static Color Navy;      ///< 0x00007BFF
		static Color Purple;    ///< 0x8F00D6FF
		static Color Maroon;    ///< 0x7B0000FF
		static Color Pink;      ///< 0xFF00FFFF
		static Color Olive;     ///< 0x7B7B00FF
		static Color Orange;    ///< 0xFFA300FF
		static Color Aqua;      ///< 0x00FFFFFF
		static Color Bone;      ///< 0xF2F2E6FF
		static Color Tan;       ///< 0xD1B28CFF
		static Color Charcoal;  ///< 0x444444FF
		static Color Royal;     ///< 0x074C9EFF
		static Color Azure;     ///< 0x007FFFFF
		static Color Aquamarine;///< 0x7FFFD2FF
		static Color Forest;    ///< 0x218921FF
		static Color Gold;      ///< 0xFFD600FF
		static Color Coral;     ///< 0xFF7F4FFF
		static Color Lavender;  ///< 0xB57FDBFF
		static Color Indigo;    ///< 0x490082FF

		unsigned char r; ///< Red component
		unsigned char g; ///< Green component
		unsigned char b; ///< Blue component
		unsigned char a; ///< Alpha (transparency) component

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes the color to opaque white (255,255,255,255)
		///
		/////////////////////////////////////////////////////////////
		Color();

		/////////////////////////////////////////////////////////////
		/// @brief Construct color with same Red, Green, Blue value (opaque)
		///
		/// @param rgb The value of red, green and blue
		///
		/////////////////////////////////////////////////////////////
		explicit Color(unsigned char rgb);

		/////////////////////////////////////////////////////////////
		/// @brief Construct color with same Red, Green and Blue value
		///
		/// @param rgb The value of red, green and blue
		/// @param a Transparency
		///
		/////////////////////////////////////////////////////////////
		Color(unsigned char rgb,unsigned char a);

		/////////////////////////////////////////////////////////////
		/// @brief Construct color from Red, Green and Blue values
		///
		/// @param r Red component
		/// @param g Green component
		/// @param b Blue component
		///
		/////////////////////////////////////////////////////////////
		Color(unsigned char r,unsigned char g,unsigned char b);

		/////////////////////////////////////////////////////////////
		/// @brief Construct color from Red, Green, Blue and alpha values
		///
		/// @param r Red component
		/// @param g Green component
		/// @param b Blue component
		/// @param a Transparency
		///
		/////////////////////////////////////////////////////////////
		Color(unsigned char r,unsigned char g,unsigned char b,unsigned char a);

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// @param copy The color to be copied
		///
		/////////////////////////////////////////////////////////////
		Color(const Color &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Convert 3D vector to color (opaque)
		///
		/// This constructor simply copies the data from @a color
		/// therefore Color(vec3(0.99,0.98,0.98)) will result in black color (0,0,0,255)
		///
		/// @param color The color to be copied
		///
		/////////////////////////////////////////////////////////////
		Color(const fm::vector3<float> &color);

		/////////////////////////////////////////////////////////////
		/// @brief Convert 3D vector and alpha value to color
		///
		/// This constructor simply copies the data from @a color
		/// therefore Color(vec3(0.99,0.98,0.98)) will result in black color (0,0,0,@a a)
		///
		/// @param color The color to be copied
		/// @param a Transparency
		///
		/////////////////////////////////////////////////////////////
		Color(const fm::vector3<float> &color,float a);

		/////////////////////////////////////////////////////////////
		/// @brief Convert 4D vector and alpha value to color
		///
		/// This constructor simply copies the data from @a color
		/// therefore Color(vec4(0.99,0.98,0.98,1)) will result in transparent black color (0,0,0,1)
		///
		/// @param color The color to be copied
		///
		/////////////////////////////////////////////////////////////
		Color(const fm::vector4<float> &color);

		/////////////////////////////////////////////////////////////
		/// @brief Change the Red, Green and Blue copmonents of the color
		///
		/// @param r New Red value
		/// @param g New Green value
		/// @param b New Blue value
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(unsigned char r,unsigned char g,unsigned char b);

		/////////////////////////////////////////////////////////////
		/// @brief Change the Red, Green, Blue and Alpha copmonents of the color
		///
		/// @param r New Red value
		/// @param g New Green value
		/// @param b New Blue value
		/// @param a New Alpha value
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(unsigned char r,unsigned char g,unsigned char b,unsigned char a);

		/////////////////////////////////////////////////////////////
		/// @brief Create a color from a hex value
		///
		/// @param hex The value of the color (r,g,b,a order)
		///
		/// @return The constructed color
		///
		/////////////////////////////////////////////////////////////
		static Color fromHex(unsigned int hex);

		/////////////////////////////////////////////////////////////
		/// @brief Convert Color to 4D vector
		///
		/// This function DOES divide the r,g,b,a values of the color by 255.f
		///
		/////////////////////////////////////////////////////////////
		operator fm::vector4<float>() const;
	};

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (Color)
	///
	/// @return reference to itself
	///
	/////////////////////////////////////////////////////////////
	Color FRONTIER_API &operator+=(Color &left,const Color &right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (Color)
	///
	/// @return reference to itself
	///
	/////////////////////////////////////////////////////////////
	Color FRONTIER_API &operator-=(Color &left,const Color &right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (scalar)
	///
	/// @return reference to itself
	///
	/////////////////////////////////////////////////////////////
	Color FRONTIER_API &operator*=(Color &left,float right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (scalar)
	///
	/// @return reference to itself
	///
	/////////////////////////////////////////////////////////////
	Color FRONTIER_API &operator/=(Color &left,float right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (Color)
	///
	/// @return Result of the addition
	///
	/////////////////////////////////////////////////////////////
	Color FRONTIER_API operator+(const Color &left,const Color &right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (Color)
	///
	/// @return Result of the substraction
	///
	/////////////////////////////////////////////////////////////
	Color FRONTIER_API operator-(const Color &left,const Color &right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the multiplication
	///
	/////////////////////////////////////////////////////////////
	Color FRONTIER_API operator*(const Color &left,float right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the division
	///
	/////////////////////////////////////////////////////////////
	Color FRONTIER_API operator/(const Color &left,float right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (Color)
	///
	/// @return True if the two colors are the same
	///
	/////////////////////////////////////////////////////////////
	bool FRONTIER_API operator==(const Color &left,const Color &right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (Color)
	/// @param right Right operand (Color)
	///
	/// @return True if the two colors are not the same
	///
	/////////////////////////////////////////////////////////////
	bool FRONTIER_API operator!=(const Color &left,const Color &right);
}
#endif //FRONTIER_COLOR_HPP_INCLUDED

#ifndef FRONTIER_DONT_INLUDE_INL
	#include <FRONTIER/Graphics/Color.inl>
#endif

////////////////////////////////////////////////////////////
/// @class fg::Color
/// @ingroup Graphics
///
/// fg::Color is used to hold the value of a color in RGBA color space
/// you can read about it <a href="http://en.wikipedia.org/wiki/RGBA_color_space">in this wikipeda article</a>
/// 
/// fg::Color is laid out in memory as |red bit||green bit||blue bit||alpha bit|
/// 
/// @see fg::Image
///
////////////////////////////////////////////////////////////