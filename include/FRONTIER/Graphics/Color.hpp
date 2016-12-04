////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_COLOR_HPP_INCLUDED
#define FRONTIER_COLOR_HPP_INCLUDED
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/util/API.h>
#define FRONTIER_COLOR

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to hold a color's value in <a href="http://en.wikipedia.org/wiki/RGBA_color_space">RGBA color space</a>
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Color
	{
	public:
		typedef fm::Uint8 component_type;
		typedef Color &reference;
		typedef const Color &const_reference;
		enum {
			components = 4u ///< Public value indicating the amount of component_types in the class
		};

		static Color White;     ///< The color that's hex code matches 0xFFFFFFFF
		static Color Silver;    ///< The color that's hex code matches 0xBFBFBFFF
		static Color Gray;      ///< The color that's hex code matches 0x7B7B7BFF
		static Color Grey;      ///< The color that's hex code matches 0x7B7B7BFF
		static Color Black;     ///< The color that's hex code matches 0x000000FF
		static Color Red;       ///< The color that's hex code matches 0xFF0000FF
		static Color Brown;     ///< The color that's hex code matches 0x7B4000FF
		static Color Yellow;    ///< The color that's hex code matches 0xFFFF00FF
		static Color Lime;      ///< The color that's hex code matches 0x00FF00FF
		static Color Green;     ///< The color that's hex code matches 0x218C21FF
		static Color Teal;      ///< The color that's hex code matches 0x007B7BFF
		static Color Blue;      ///< The color that's hex code matches 0x0000FFFF
		static Color Navy;      ///< The color that's hex code matches 0x00007BFF
		static Color Purple;    ///< The color that's hex code matches 0x8F00D6FF
		static Color Maroon;    ///< The color that's hex code matches 0x7B0000FF
		static Color Pink;      ///< The color that's hex code matches 0xFF00FFFF
		static Color Olive;     ///< The color that's hex code matches 0x7B7B00FF
		static Color Orange;    ///< The color that's hex code matches 0xFFA300FF
		static Color Aqua;      ///< The color that's hex code matches 0x00FFFFFF
		static Color Bone;      ///< The color that's hex code matches 0xF2F2E6FF
		static Color Tan;       ///< The color that's hex code matches 0xD1B28CFF
		static Color Charcoal;  ///< The color that's hex code matches 0x444444FF
		static Color Royal;     ///< The color that's hex code matches 0x074C9EFF
		static Color Azure;     ///< The color that's hex code matches 0x007FFFFF
		static Color Aquamarine;///< The color that's hex code matches 0x7FFFD2FF
		static Color Forest;    ///< The color that's hex code matches 0x218921FF
		static Color Gold;      ///< The color that's hex code matches 0xFFD600FF
		static Color Coral;     ///< The color that's hex code matches 0xFF7F4FFF
		static Color Lavender;  ///< The color that's hex code matches 0xB57FDBFF
		static Color Indigo;    ///< The color that's hex code matches 0x490082FF

		fm::Uint8 r; ///< Red component
		fm::Uint8 g; ///< Green component
		fm::Uint8 b; ///< Blue component
		fm::Uint8 a; ///< Alpha (transparency) component

		/////////////////////////////////////////////////////////////
		/// @brief Construct color with same Red, Green, Blue value (opaque)
		///
		/// @param rgb The value of red, green and blue
		/// @param a Transparency
		///
		/////////////////////////////////////////////////////////////
		explicit Color(fm::Uint8 rgb = 255,fm::Uint8 a = 255);

		/////////////////////////////////////////////////////////////
		/// @brief Construct color from Red, Green and Blue values
		///
		/// @param r Red component
		/// @param g Green component
		/// @param b Blue component
		/// @param a Transparency
		///
		/////////////////////////////////////////////////////////////
		Color(fm::Uint8 r,fm::Uint8 g,fm::Uint8 b,fm::Uint8 a = 255);

		/////////////////////////////////////////////////////////////
		/// @brief Convert 3D vector to color (opaque)
		///
		/// This constructor simply copies the data from @a color
		/// therefore Color(vec3(0.99,0.98,0.98)) will result in black color (0,0,0,255)
		///
		/// @param color The color to be copied
		///
		/////////////////////////////////////////////////////////////
		Color(const fm::vector3<float> &color,fm::Uint8 a = 255);

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
		/// @brief Get the rgb part of the color
		///
		/// @return The rgb value (in range 0-255)
		///
		/////////////////////////////////////////////////////////////
		const fm::vector3<fm::Uint8> &rgb() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the rgba part of the color
		///
		/// @return The rgba value (in range 0-255)
		///
		/////////////////////////////////////////////////////////////
		const fm::vector4<fm::Uint8> &rgba() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the rgb part of the color
		///
		/// @return The rgb value (in range 0-255)
		///
		/////////////////////////////////////////////////////////////
		fm::vector3<fm::Uint8> &rgb();

		/////////////////////////////////////////////////////////////
		/// @brief Get the rgba part of the color
		///
		/// @return The rgba value (in range 0-255)
		///
		/////////////////////////////////////////////////////////////
		fm::vector4<fm::Uint8> &rgba();

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
		reference operator()(fm::Uint8 r,fm::Uint8 g,fm::Uint8 b);

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
		reference operator()(fm::Uint8 r,fm::Uint8 g,fm::Uint8 b,fm::Uint8 a);

		/////////////////////////////////////////////////////////////
		/// @brief Create a color from a hex value
		///
		/// @param hex The value of the color (r,g,b,a order)
		///
		/// @return The constructed color
		///
		/////////////////////////////////////////////////////////////
		static Color fromHex(fm::Uint32 hex);
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
