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
#ifndef FRONTIER_VECTOR2_HPP_INCLUDED
#define FRONTIER_VECTOR2_HPP_INCLUDED
#include <FRONTIER/System/util/RequireCpp11.hpp>
#define FRONTIER_VECTOR2

namespace fm
{
	template<class> class vector4;
	template<class> class vector3;
	template<class> class vector2;

	/////////////////////////////////////////////////////////////
	/// @brief Templated class for using 2D <a href="http://en.wikipedia.org/wiki/Euclidean_vector">euclidean vectors</a>
	///
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	class vector2
	{
	public:
		typedef T component_type;
		typedef vector2<T> &reference;
		typedef const vector2<T> &const_reference;
		enum {
			components = 2u ///< Public value indicating the amount of component_types in the class
		};

		/////////////////////////////////////////////////////////////
		/// @brief Anonymous union holding the value of first dimension
		///
		/// x, w and u refer to the same memory region with the same type
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
			/// @brief width
			///
			/// use first dimension as width
			///
			T w;
			///
			/// @brief first texture coordinate
			///
			/// use first dimension as texture coordinate
			///
			T u;

		};

		/////////////////////////////////////////////////////////////
		/// @brief Anonymous union holding the value of second dimension
		///
		/// y, h and v refer to the same memory region with the same type
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
			/// @brief height
			///
			/// use second dimension as height
			///
			T h;
			///
			/// @brief second texture coordinate
			///
			/// use second dimension as texture coordinate
			///
			T v;

		};


		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes 2D vector with (T(),T())
		///
		/////////////////////////////////////////////////////////////
		vector2();

		/////////////////////////////////////////////////////////////
		/// @brief Construct 2D vector from two identical coordinates
		///
		/// @param XY x and y coordinate
		///
		/////////////////////////////////////////////////////////////
		vector2(const T &XY);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 2D vector from two coordinates
		///
		/// @param X x coordinate
		/// @param Y y coordinate
		///
		/////////////////////////////////////////////////////////////
		vector2(const T &X,const T &Y);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 2D vector from an array
		///
		/// @param XY Array holding the x and y coordinate
		///
		/////////////////////////////////////////////////////////////
		vector2(const T (&XY)[2]);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 2D vector from other 2D vector
		///
		/// @param other 2D vector to construct from
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector2(const vector2<T2> &other);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 2D vector from 3D vector with different type
		///
		/// This constructor simply ignores the z coordinate
		/// therefore it is the same as vector2(other.x,other.y)
		///
		/// @param other 3D vector to construct from
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector2(const vector3<T2> &other);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 2D vector from 4D vector with different type
		///
		/// This constructor simply ignores the z and w coordinates
		/// therefore it is the same as vector2(other.x,other.y)
		///
		/// @param other 4D vector to construct from
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector2(const vector4<T2> &other);

		/////////////////////////////////////////////////////////////
		/// @brief Strips both coordinates from their signs
		///
		/// x=|x|, y=|y|
		///
		/// @return The unsigned vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> unsign() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the length of the 2D vector
		///
		/// |v|
		///
		/// @return Length of the vector
		///
		/////////////////////////////////////////////////////////////
		T length() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the squared length of the 2D vector
		///
		/// |v|^2
		///
		/// @return Squared length of the vector
		///
		/////////////////////////////////////////////////////////////
		T LENGTH() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the normed version of the 2D vector
		///
		/// v/|v|
		///
		/// @return The vector, normalized
		///
		/////////////////////////////////////////////////////////////
		vector2<T> norm() const;

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
		/// @brief Get the normed version of the 2D vector
		///
		/// v/|v|
		///
		/// @return The vector, normed
		///
		/////////////////////////////////////////////////////////////
		vector2<T> sgn() const;

		/////////////////////////////////////////////////////////////
		/// @brief Project the vector to @a target
		///
		/// target * (v <B>·</B> target) / |v|^2;
		///
		/// @param target The vector to project to
		///
		/// @return The projected vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> projTo(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Reflect the vector on @a target
		///
		/// target * (v <B>·</B> target) / |v|^2 - v;
		///
		/// @param target The vector to reflect on
		///
		/// @return The reflected vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> reflectOn(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Inverts the vector
		///
		/// x = T(1)/x  y = T(1)/y
		///
		/// @return The inverted vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> inv() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get a perpendicular vector
		///
		/// @return The perpendicular vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> perp() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the area of the vector
		///
		/// @return w*h
		///
		/////////////////////////////////////////////////////////////
		T area() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the <a href="http://en.wikipedia.org/wiki/Dot_product">dot product</a> of two vectors
		///
		/// @param other The vector to take the dot product with
		///
		/// @return The dot product
		///
		/////////////////////////////////////////////////////////////
		T dot(const_reference other) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get rw access to the minimum of the components
		///
		/// @return min(x,y)
		///
		/////////////////////////////////////////////////////////////
		T &min();

		/////////////////////////////////////////////////////////////
		/// @brief Get value of the minimum of the components
		///
		/// @return min(x,y)
		///
		/////////////////////////////////////////////////////////////
		const T &min() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get rw access to the maximum of the components
		///
		/// @return max(x,y)
		///
		/////////////////////////////////////////////////////////////
		T &max();

		/////////////////////////////////////////////////////////////
		/// @brief Get value of the maximum of the components
		///
		/// @return max(x,y)
		///
		/////////////////////////////////////////////////////////////
		const T &max() const;

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator ()
		///
		/// @param X x's new value
		/// @param Y y's new value
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(const T &X,const T &Y);

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
	};
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator+(const vector2<T> &left,const vector2<T2> &right) -> vector2<decltype(left.x+right.x)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise subtraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator-(const vector2<T> &left,const vector2<T2> &right) -> vector2<decltype(left.x-right.x)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector2<T> &left,const vector2<T2> &right) -> vector2<decltype(left.x*right.x)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise division of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector2<T> &left,const vector2<T2> &right) -> vector2<decltype(left.x/right.x)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator %
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise modulo of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector2<T> &left,const vector2<T2> &right) -> vector2<decltype(left.x%right.x)>;
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x b                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector2<T> &left,const T2 &right) -> vector2<decltype(left.x*right)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const T2 &left,const vector2<T> &right) -> vector2<decltype(left*right.x)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise division of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector2<T> &left,const T2 &right) -> vector2<decltype(left.x/right)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator %
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise modulo of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector2<T> &left,const T2 &right) -> vector2<decltype(left.x%right)>;
	
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x= B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator-=(vector2<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator*=(vector2<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator/=(vector2<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator %=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator%=(vector2<T> &left,const vector2<T2> &right);
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x= b                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator*=(vector2<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator/=(vector2<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator %=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator%=(vector2<T> &left,const T2 &right);
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A eq B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left equals @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator==(const vector2<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left not equals @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator!=(const vector2<T> &left,const vector2<T2> &right);
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
						 x A                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of unary operator -
	///
	/// @param vec Vector to negate
	///
	/// @return Memberwise negation of @a vec
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator-(const vector2<T> &vec) -> vector2<decltype(-vec.x)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of unary operator +
	///
	/// @param vec Vector to negate
	///
	/// @return Memberwise negation of @a vec
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator+(const vector2<T> &vec) -> vector2<decltype(+vec.x)>;
	
	typedef vector2<float> 	       vec2;
	typedef vector2<int>   	       vec2i;
	typedef vector2<float> 	       vec2f;
	typedef vector2<double>        vec2d;
	typedef vector2<unsigned int>  vec2u;
	typedef vector2<unsigned char> vec2b;
}

namespace std
{
	template<class T>
	T abs(const fm::vector2<T> &vec);

	template<class T>
	fm::vector2<T> sgn(const fm::vector2<T> &vec);
}

#endif // FRONTIER_VECTOR2_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Vector2.inl>
#endif
