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
#ifndef FRONTIER_VECTOR4_HPP_INCLUDED
#define FRONTIER_VECTOR4_HPP_INCLUDED
#include <FRONTIER/System/util/RequireCpp11.hpp>
#include <FRONTIER/System/util/RETTYPE.hpp>
#define FRONTIER_VECTOR4

namespace fm
{
	template<class> class vector4;
	template<class> class vector3;
	template<class> class vector2;


	/////////////////////////////////////////////////////////////
	/// @brief Templated class for managing 4D <a href="http://en.wikipedia.org/wiki/Euclidean_vector">euclidean vectors</a>
	///
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	class vector4
	{
	public:
		typedef T component_type;
		typedef vector4<T> &reference;
		typedef const vector4<T> &const_reference;
		enum {
			components = 4u ///< Public value indicating the amount of component_types in the class
		};

		static vector4<T> White;     ///< The color that's hex code matches 0xFFFFFFFF
		static vector4<T> Silver;    ///< The color that's hex code matches 0xBFBFBFFF
		static vector4<T> Gray;      ///< The color that's hex code matches 0x7B7B7BFF
		static vector4<T> Grey;      ///< The color that's hex code matches 0x7B7B7BFF
		static vector4<T> Black;     ///< The color that's hex code matches 0x000000FF
		static vector4<T> Red;       ///< The color that's hex code matches 0xFF0000FF
		static vector4<T> Brown;     ///< The color that's hex code matches 0x7B4000FF
		static vector4<T> Yellow;    ///< The color that's hex code matches 0xFFFF00FF
		static vector4<T> Lime;      ///< The color that's hex code matches 0x00FF00FF
		static vector4<T> Green;     ///< The color that's hex code matches 0x218C21FF
		static vector4<T> Teal;      ///< The color that's hex code matches 0x007B7BFF
		static vector4<T> Blue;      ///< The color that's hex code matches 0x0000FFFF
		static vector4<T> Navy;      ///< The color that's hex code matches 0x00007BFF
		static vector4<T> Purple;    ///< The color that's hex code matches 0x8F00D6FF
		static vector4<T> Maroon;    ///< The color that's hex code matches 0x7B0000FF
		static vector4<T> Pink;      ///< The color that's hex code matches 0xFF00FFFF
		static vector4<T> Olive;     ///< The color that's hex code matches 0x7B7B00FF
		static vector4<T> Orange;    ///< The color that's hex code matches 0xFFA300FF
		static vector4<T> Aqua;      ///< The color that's hex code matches 0x00FFFFFF
		static vector4<T> Bone;      ///< The color that's hex code matches 0xF2F2E6FF
		static vector4<T> Tan;       ///< The color that's hex code matches 0xD1B28CFF
		static vector4<T> Charcoal;  ///< The color that's hex code matches 0x444444FF
		static vector4<T> Royal;     ///< The color that's hex code matches 0x074C9EFF
		static vector4<T> Azure;     ///< The color that's hex code matches 0x007FFFFF
		static vector4<T> Aquamarine;///< The color that's hex code matches 0x7FFFD2FF
		static vector4<T> Forest;    ///< The color that's hex code matches 0x218921FF
		static vector4<T> Gold;      ///< The color that's hex code matches 0xFFD600FF
		static vector4<T> Coral;     ///< The color that's hex code matches 0xFF7F4FFF
		static vector4<T> Lavender;  ///< The color that's hex code matches 0xB57FDBFF
		static vector4<T> Indigo;    ///< The color that's hex code matches 0x490082FF

		/////////////////////////////////////////////////////////////
		/// @brief Anonymous union holding the value of first dimension
		///
		/// x and r refer to the same memory region with the same type
		/// thus they are interchangeable
		///
		/////////////////////////////////////////////////////////////
		union {

			///
			/// @brief x coordinate
			///
			/// use first dimension as x coordinate
			///
			T x;
			///
			/// @brief red component
			///
			/// use first dimension as red component of a color
			///
			T r;

		};

		/////////////////////////////////////////////////////////////
		/// @brief Anonymous union holding the value of second dimension
		///
		/// y and g refer to the same memory region with the same type
		/// thus they are interchangeable
		///
		/////////////////////////////////////////////////////////////
		union {

			///
			/// @brief y coordinate
			///
			/// use second dimension as y coordinate
			///
			T y;
			///
			/// @brief green component
			///
			/// use second dimension as green component of a color
			///
			T g;

		};

		/////////////////////////////////////////////////////////////
		/// @brief Anonymous union holding the value of third dimension
		///
		/// z and b refer to the same memory region with the same type
		/// thus they are interchangeable
		///
		/////////////////////////////////////////////////////////////
		union {

			///
			/// @brief z coordinate
			///
			/// use third dimension as z coordinate
			///
			T z;
			///
			/// @brief blue component
			///
			/// use third dimension as blue component of a color
			///
			T b;

		};

		/////////////////////////////////////////////////////////////
		/// @brief Anonymous union holding the value of third dimension
		///
		/// w and a refer to the same memory region with the same type
		/// thus they are interchangeable
		///
		/////////////////////////////////////////////////////////////
		union {

			///
			/// @brief w coordinate
			///
			/// use fourth dimension as w coordinate
			///
			T w;
			///
			/// @brief alpha component
			///
			/// use fourth dimension as alpha component of a color
			///
			T a;

		};


		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes 4D vector with (0,0,0,1)
		///
		/////////////////////////////////////////////////////////////
		vector4();

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from two coordinates
		///
		/// @param XYZ x,y,z coordinates
		/// @param W w coordinate
		///
		/////////////////////////////////////////////////////////////
		vector4(const T &XYZ,const T &W = T(1));

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from four coordinates
		///
		/// @param X x coordinate
		/// @param Y y coordinate
		/// @param Z z coordinate
		/// @param W w coordinate
		///
		/////////////////////////////////////////////////////////////
		vector4(const T &X,const T &Y,const T &Z,const T &W = T(1));

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from an array
		///
		/// @param XYZW Array holding the x, y, z and w coordinates
		///
		/////////////////////////////////////////////////////////////
		vector4(const T (&XYZW)[4]);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from an array and a w
		///
		/// @param XYZ Array holding the x, y and z coordinates
		/// @param W w coordinate
		///
		/////////////////////////////////////////////////////////////
		vector4(const T (&XYZ)[3],const T &W = T(1));

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from vec 4D vector
		///
		/// @param vec 4D vector to construct from
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector4(const vector4<T2> &vec);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from 2D vector with different type
		///
		/// This constructor sets the x,y coordinates to @a vec 's x and y
		/// z to 0 and w to 1
		///
		/// @param vec 2D vector to construct from
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector4(const vector2<T2> &vec);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from 2D vector with different type
		///
		/// This constructor sets the x,y coordinates to @a vec 's x and y
		/// z to @a Z and w to 1
		///
		/// @param vec 2D vector to construct from
		/// @param Z z's new value
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class T3>
		vector4(const vector2<T2> &vec,const T3 &Z);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from 2D vector with different type
		///
		/// This constructor sets the x,y coordinates to @a vec 's x and y
		/// z to @a Z and w to @a W
		///
		/// @param vec 2D vector to construct from
		/// @param Z z's new value
		/// @param W w's new value
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class T3,class T4>
		vector4(const vector2<T2> &vec,const T3 &Z,const T4 &W);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from 3D vector with different type
		///
		/// This constructor uses @a vec to fill the x, y and z fileds
		/// and sets the w field to 1
		///
		/// @param vec 3D vector to construct from
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector4(const vector3<T2> &vec);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from 3D vector with different type
		///
		/// This constructor uses @a vec to fill the x, y and z fileds
		/// and sets the w field to @a W
		///
		/// @param vec 3D vector to construct from
		/// @param W w's new value
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class T3>
		vector4(const vector3<T2> &vec,const T3 &W);

		/////////////////////////////////////////////////////////////
		/// @brief Strips both coordinates from their signs
		///
		/// x=|x|, y=|y|, z=|z|, w=|w|
		///
		/// @return The unsigned vector
		///
		/////////////////////////////////////////////////////////////
		vector4<T> unsign();

		/////////////////////////////////////////////////////////////
		/// @brief Get the length of the 4D vector
		///
		/// |v|
		///
		/// @return Length of the vector
		///
		/////////////////////////////////////////////////////////////
		T length() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the squared length of the 4D vector
		///
		/// |v|^2
		///
		/// @return Squared length of the vector
		///
		/////////////////////////////////////////////////////////////
		T LENGTH() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the normed version of the 4D vector
		///
		/// v/|v|
		///
		/// @return The vector, normalized
		///
		/////////////////////////////////////////////////////////////
		vector4<T> norm() const;

		/////////////////////////////////////////////////////////////
		/// @brief Normalize the vector
		///
		/// v /= |v|
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference normalize();

		/////////////////////////////////////////////////////////////
		/// @brief Get the normed version of the 4D vector
		///
		/// v/|v|
		///
		/// @return The vector, normed
		///
		/////////////////////////////////////////////////////////////
		vector4<T> sgn() const;

		/////////////////////////////////////////////////////////////
		/// @brief Inverts the vector
		///
		/// x=1/x  y=1/y  z=1/z  w=1/w
		///
		/// @return The inverted vector
		///
		/////////////////////////////////////////////////////////////
		vector4<T> inv() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert 4D color to hex value
		///
		/// @return hex Hexadecimal value of the color
		///
		/////////////////////////////////////////////////////////////
		unsigned int toHex() const;


		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator ()
		///
		/// @param X x's new value
		/// @param Y y's new value
		/// @param Z z's new value
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(const T &X,const T &Y,const T &Z);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator ()
		///
		/// @param X x's new value
		/// @param Y y's new value
		/// @param Z z's new value
		/// @param W w's new value
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(const T &X,const T &Y,const T &Z,const T &W);

        /////////////////////////////////////////////////////////////
        /// @brief Overload of binary operator []
        ///
        /// @param index Offset from the beginning of the vector
        ///
        /// @return Reference to the vector's index-th dimension
        ///
        /////////////////////////////////////////////////////////////
        T &operator[](unsigned int index);

        /////////////////////////////////////////////////////////////
        /// @brief Overload of binary operator []
        ///
        /// @param index Offset from the beginning of the vector
        ///
        /// @return Reference to the vector's index-th dimension
        ///
        /////////////////////////////////////////////////////////////
        const T &operator[](unsigned int index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert hex value to 4D color
		///
		/// @param hex Hexadecimal value of the color
		///
		/// @return converted color
		///
		/////////////////////////////////////////////////////////////
		static vector4<T> fromHex(unsigned int hex);
	};
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator+(const vector4<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,+)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator+(const vector4<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,+)>;


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise subtraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator-(const vector4<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,-)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise subtraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator-(const vector4<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,-)>;


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector4<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,*)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector4<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,*)>;


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise division of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector4<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,/)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise division of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector4<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,/)>;


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator %
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise modulo of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector4<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,+)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator %
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise modulo of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector4<T> &left,const vector3<T2> &right) -> vector4<RETTYPE(T,T2,+)>;
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x b                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector4<T> &left,const T2 &right) -> vector4<decltype(left.x*right)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const T2 &left,const vector4<T> &right) -> vector4<decltype(left*right.x)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise division of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector4<T> &left,const T2 &right) -> vector4<decltype(left.x/right)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator %
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise modulo of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector4<T> &left,const T2 &right) -> vector4<decltype(left.x+right)>;
	
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x= B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator+=(vector4<T> &left,const vector4<T2> &right);
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator+=(vector4<T> &left,const vector3<T2> &right);
	

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator-=(vector4<T> &left,const vector4<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator-=(vector4<T> &left,const vector3<T2> &right);
	

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator*=(vector4<T> &left,const vector4<T2> &right);
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator*=(vector4<T> &left,const vector3<T2> &right);
	

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator/=(vector4<T> &left,const vector4<T2> &right);
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator/=(vector4<T> &left,const vector3<T2> &right);


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator %=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator%=(vector4<T> &left,const vector4<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator %=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator%=(vector4<T> &left,const vector3<T2> &right);
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x= b                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator*=(vector4<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator/=(vector4<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator %=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<T> &operator%=(vector4<T> &left,const T2 &right);
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A eq B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left and @a right are the same
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator==(const vector4<T> &left,const vector4<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left and @a right are the same
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator==(const vector4<T> &left,const vector3<T2> &right);
	

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left and @a right are not the same
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator!=(const vector4<T> &left,const vector4<T> &right);
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left and @a right are not the same
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator!=(const vector4<T> &left,const vector3<T> &right);
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
						  x A                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of unary operator -
	///
	/// @param vec Vector to negate
	///
	/// @return Memberwise negation of @a vec
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator-(const vector4<T> &vec) -> vector4<decltype(-vec.x)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of unary operator +
	///
	/// @param vec Vector to negate
	///
	/// @return Memberwise negation of @a vec
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator+(const vector4<T> &vec) -> vector4<decltype(+vec.x)>;

	typedef vector4<float> 	       vec4;
	typedef vector4<int>   	       vec4i;
	typedef vector4<float> 	       vec4f;
	typedef vector4<double>        vec4d;
	typedef vector4<unsigned int>  vec4u;
	typedef vector4<unsigned char> vec4b;

}

#endif // FRONTIER_VECTOR4_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Vector4.inl>
#endif
