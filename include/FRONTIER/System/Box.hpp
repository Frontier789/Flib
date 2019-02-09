////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_BOX_HPP_INCLUDED
#define FRONTIER_BOX_HPP_INCLUDED
#include <FRONTIER/System/CommonTypes.hpp>

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/System/Vector3.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#define FRONTIER_BOX
namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Templated class for holding the size and position of a box
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
    template<class T>
    class box
    {
    public:
		typedef T component_type;
        typedef box<T> &reference;
        typedef const box<T> &const_reference;
		enum {
			components = 6u ///< Public value indicating the amount of component_types in the class
		};
		vector3<T> pos;  ///< The position of the box
		vector3<T> size; ///< The size of the box

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes the box with ((0,0,0),(0,0,0))
		///
		/////////////////////////////////////////////////////////////
		box();

		/////////////////////////////////////////////////////////////
		/// @brief Construct a box from pos and size
		///
		/// Initializes the box with (@a pos,@a size)
		///
		/// @param pos The position of the box
		/// @param size The size of the box
		///
		/////////////////////////////////////////////////////////////
		explicit box(const vector3<T> &pos,const vector3<T> &size = vector3<T>());

		/////////////////////////////////////////////////////////////
		/// @brief Construct a box from pos and size
		///
		/// @param x The x position of the box
		/// @param y The y position of the box
		/// @param z The z position of the box
		/// @param w The width of the box
		/// @param h The height of the box
		/// @param d The depth of the box
		///
		/////////////////////////////////////////////////////////////
		box(const T &x,const T &y,const T &z,const T &w,const T &h,const T &d);

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// @param copy The box to be copied
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		box(const box<T2> &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Test if a point is in the box
		/// 
		/// Points on the edge considered as contained
		/// 
		/// @param p The point to test against
		///
		/// @return True if @a p is in the box, false if not
		///
		/////////////////////////////////////////////////////////////
		bool contains(const vector3<T> &p) const;

		/////////////////////////////////////////////////////////////
		/// @brief Test if two rectangles are the same
		///
		/// @param other The box to test against
		///
		/// @return True if the two rectangles are equal
		///
		/////////////////////////////////////////////////////////////
		bool operator==(const box<T> &other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Test if two rectangles are not the same
		///
		/// @param other The box to test against
		///
		/// @return True if the two rectangles are not equal
		///
		/////////////////////////////////////////////////////////////
		bool operator!=(const box<T> &other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Expand the ractangle to contain p
		///
		/// expansion is done by either decreasing pos (and expanding size the same amount)
		/// or by increasing size
		///
		/// @param p The point to include
		///
		/////////////////////////////////////////////////////////////
		reference expand(const vector3<T> &p);

		/////////////////////////////////////////////////////////////
		/// @brief Get the middle point of the box
		///
		/// Equals to pos + size/2
		///
		/// @return The middle point
		///
		/////////////////////////////////////////////////////////////
		vector3<T> mid() const;

		/////////////////////////////////////////////////////////////
		/// @brief Check if two rectangles intersect
		///
		/// @return True iff the two intersect
		///
		/////////////////////////////////////////////////////////////
		bool intersects(const box<T> &r) const;
    };

    typedef box<float> 		  box3f;
    typedef box<int>   		  box3i;
    typedef box<unsigned int> box3u;
    typedef box<fm::Size>  	  box3s;
}
#endif // FRONTIER_BOX_HPP

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Box.inl>
#endif