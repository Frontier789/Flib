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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_RECT_HPP_INCLUDED
#define FRONTIER_RECT_HPP_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/Vector2.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_RECT
namespace fm
{
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Templated class for holding the size and position of a rectangle
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
			components = 4u ///< Public value indicating the amount of component_type's in the class
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
		/// Initializes the rectangle with (@a pos,(0,0))
		///
		/// @param pos The position of the rectangle
		///
		/////////////////////////////////////////////////////////////
		rect(const vector2<T> &pos);

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes the rectangle with (@a pos,@a size)
		///
		/// @param pos The position of the rectangle
		/// @param size The size of the rectangle
		///
		/////////////////////////////////////////////////////////////
		rect(const vector2<T> &pos,const vector2<T> &size);

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes the rectangle with ((@a x,@a y),@a size)
		///
		/// @param x The x coordinate of the rectangle's position
		/// @param y The y coordinate of the rectangle's position
		/// @param size The size of the rectangle
		///
		/////////////////////////////////////////////////////////////
		rect(const T &x,const T &y,const vector2<T> &size);

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes the rectangle with ((@a x,@a y),(@a w,@a h))
		///
		/// @param x The x coordinate of the rectangle's position
		/// @param y The y coordinate of the rectangle's position
		/// @param w The width of the rectangle
		/// @param h The height of the rectangle
		///
		/////////////////////////////////////////////////////////////
		rect(const T &x,const T &y,const T &w,const T &h);

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes the rectangle with (@a pos,(@a w,@a h))
		///
		/// @param pos The position of the rectangle
		/// @param w The width of the rectangle
		/// @param h The height of the rectangle
		///
		/////////////////////////////////////////////////////////////
		rect(const vector2<T> &pos,const T &w,const T &h);

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
		/// @param p The point to test against
		///
		/// @return True if @a p is in the rectangle, false if not
		///
		/////////////////////////////////////////////////////////////
		bool contains(const vector2<T> &p) const;

		/////////////////////////////////////////////////////////////
		/// @brief Test if two rectangles intersect
		///
		/// @param other The rectangle to test against
		///
		/// @return True if @a other intersects with the rectangle, false otherwise
		///
		/////////////////////////////////////////////////////////////
		bool intersects(const rect<T> &other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Test if two rectangles intersect
		///
		/// @param other The rectangle to test against
		/// @param intersection If they intersect then @a intersection is set to the intersection rectangle
		///
		/// @return True if @a other intersects with the rectangle, false otherwise
		///
		/////////////////////////////////////////////////////////////
		bool intersects(const rect<T> &other,rect<T> &intersection) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the middle point of the rectangle
		///
		/// Equals to pos+size/2
		///
		/// @return The middle point
		///
		/////////////////////////////////////////////////////////////
		vector2<T> midpt() const;

		/////////////////////////////////////////////////////////////
		/// @brief Expand the rectangle to contain @a p
		///
		/// @param p This point will be in the rectangle after the call
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference extend(const vector2<T> &p);

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the area of the rectangle
		///
		/// The area is always non-negative
		///
		/// @return The area
		///
		/////////////////////////////////////////////////////////////
		T area() const;

		/////////////////////////////////////////////////////////////
		/// @brief Find out if the first rectangle is bigger than the second
		///
		/// @param left The supposedly bigger rectangle
		/// @param right The supposedly smaller rectangle
		///
		/// @return True iff @a left is bigger than @a right
		///
		/////////////////////////////////////////////////////////////
		static bool isBigger(const rect<T> &left,const rect<T> &right);

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

////////////////////////////////////////////////////////////
/// @class fm::rect
/// @ingroup System
///
/// fm::rect is a templated class for manipulating axis aligned rectangles
///
/// The template parameter T is the type of the coordinates. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
///
/// The most common specializations are typedefed:
/// @li fm::rect<float> is fm::rect2f
/// @li fm::rect<int> is fm::rect2i
/// @li fm::rect<unsigned int> is fm::rect2u
///
/// Usage example:
/// @code
///
/// fm::rect2f r(5,5,10,10);
/// fm::vec2 p(6,7);
/// std::cout<<(r.contains(p) ? "p is in r" : "p is not in r")<<std::endl;
/// r.extend(fm::vec2(11.11,12.112));
/// r.size = fm::vec2(5,5);
/// std::cout<<"r\'s new area is:"<<r.area()<<std::endl;
///
/// @endcode
///
/// @see fm::vector2
/// @see fm::vector4
///
////////////////////////////////////////////////////////////
