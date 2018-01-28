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
#ifndef FRONTIER_RECT_HPP_INCLUDED
#define FRONTIER_RECT_HPP_INCLUDED
#include <FRONTIER/System/CommonTypes.hpp>

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/System/Vector2.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#define FRONTIER_RECT
namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Templated class for holding the size and position of a rectangle
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
    template<class T>
    class rect
    {
    public:
		typedef T component_type;
        typedef rect<T> &reference;
        typedef const rect<T> &const_reference;
		enum {
			components = 4u ///< Public value indicating the amount of component_types in the class
		};
		vector2<T> pos;  ///< The position of the rectangle
		vector2<T> size; ///< The size of the rectangle

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes the rectangle with ((0,0),(0,0))
		///
		/////////////////////////////////////////////////////////////
		rect();

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes the rectangle with (@a pos,@a size)
		///
		/// @param pos The position of the rectangle
		/// @param size The size of the rectangle
		///
		/////////////////////////////////////////////////////////////
		explicit rect(const vector2<T> &pos,const vector2<T> &size = vector2<T>());

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// @param copy The rectangle to be copied
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		rect(const rect<T2> &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Test if a point is in the rectangle
		/// 
		/// Points on the edge considered as contained
		/// 
		/// @param p The point to test against
		///
		/// @return True if @a p is in the rectangle, false if not
		///
		/////////////////////////////////////////////////////////////
		bool contains(const vector2<T> &p) const;

		/////////////////////////////////////////////////////////////
		/// @brief Test if two rectangles are the same
		///
		/// @param other The rectangle to test against
		///
		/// @return True if the two rectangles are equal
		///
		/////////////////////////////////////////////////////////////
		bool operator==(const rect<T> &other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Test if two rectangles are not the same
		///
		/// @param other The rectangle to test against
		///
		/// @return True if the two rectangles are not equal
		///
		/////////////////////////////////////////////////////////////
		bool operator!=(const rect<T> &other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Expand the ractangle to contain p
		///
		/// expansion is done by either decreasing pos (and expanding size the same amount)
		/// or by increasing size
		///
		/// @param p The point to include
		///
		/////////////////////////////////////////////////////////////
		reference expand(const vector2<T> &p);

		/////////////////////////////////////////////////////////////
		/// @brief Get the middle point of the rectangle
		///
		/// Equals to pos + size/2
		///
		/// @return The middle point
		///
		/////////////////////////////////////////////////////////////
		vector2<T> mid() const;
    };

    typedef rect<float> 		 rect2f;
    typedef rect<int>   		 rect2i;
    typedef rect<unsigned int>   rect2u;
    typedef rect<fm::Size>  	 rect2s;
}
#endif // FRONTIER_RECT_HPP

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Rect.inl>
#endif