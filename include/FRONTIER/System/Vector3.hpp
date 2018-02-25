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
#ifndef FRONTIER_VECTOR3_HPP_INCLUDED
#define FRONTIER_VECTOR3_HPP_INCLUDED
#include <FRONTIER/System/util/RequireCpp11.hpp>
#include <FRONTIER/System/util/RETTYPE.hpp>
#define FRONTIER_VECTOR3

namespace fm
{
	template<class> class vector4;
	template<class> class vector3;
	template<class> class vector2;


	/////////////////////////////////////////////////////////////
	/// @brief Templated class for using 3D <a href="http://en.wikipedia.org/wiki/Euclidean_vector">euclidean vectors</a>
	///
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	class vector3
	{
	public:
		typedef T component_type;
		typedef vector3<T> &reference;
		typedef const vector3<T> &const_reference;
		enum {
			components = 3u ///< Public value indicating the amount of component_types in the class
		};

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
		/// @brief Default constructor
		///
		/// Initializes 3D vector with (0,0,0)
		///
		/////////////////////////////////////////////////////////////
		vector3();

		/////////////////////////////////////////////////////////////
		/// @brief Construct 3D vector from three matching coordinates
		///
		/// @param XYZ x,y and z coordinates
		///
		/////////////////////////////////////////////////////////////
		vector3(const T &XYZ);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 3D vector from three coordinates
		///
		/// @param X x coordinate
		/// @param Y y coordinate
		/// @param Z z coordinate
		///
		/////////////////////////////////////////////////////////////
		vector3(const T &X,const T &Y,const T &Z=T());
		/////////////////////////////////////////////////////////////
		/// @brief Construct 3D vector from an array
		///
		/// @param XYZ Array holding the x, y and z coordinates
		///
		/////////////////////////////////////////////////////////////
		vector3(const T (&XYZ)[3]);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 3D vector from other 3D vector
		///
		/// @param other 3D vector to construct from
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector3(const vector3<T2> &other);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 3D vector from 2D vector with different type
		///
		/// This constructor sets the z coordinate to Z and
		/// uses @a other to fill x and y
		///
		/// @param other 2D vector to construct from
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector3(const vector2<T2> &other);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 3D vector from 2D vector with different type
		///
		/// This constructor sets the z coordinate to Z and
		/// uses @a other to fill x and y
		///
		/// @param other 2D vector to construct from
		/// @param Z The z coordinate
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class T3>
		vector3(const vector2<T2> &other,const T3 &Z);

		/////////////////////////////////////////////////////////////
		/// @brief Construct 3D vector from 4D vector with different type
		///
		/// This constructor simply ignores the w coordinate
		/// therefore it is the same as vector3(other.x,other.y,other.z)
		///
		/// @param other 4D vector to construct from
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector3(const vector4<T2> &other);

		/////////////////////////////////////////////////////////////
		/// @brief Strips both coordinates from their signs
		///
		/// x=|x|, y=|y|, z=|z|
		///
		/// @return The unsigned vector
		///
		/////////////////////////////////////////////////////////////
		vector3<T> unsign();

		/////////////////////////////////////////////////////////////
		/// @brief Get the length of the 3D vector
		///
		/// |v|
		///
		/// @return Length of the vector
		///
		/////////////////////////////////////////////////////////////
		T length() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the squared length of the 3D vector
		///
		/// |v|^2
		///
		/// @return Squared length of the vector
		///
		/////////////////////////////////////////////////////////////
		T LENGTH() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the area of the 3D vector
		///
		/// v.x*v.y*v.z
		///
		/// @return Area of the vector
		///
		/////////////////////////////////////////////////////////////
		T volume() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the normed version of the 3D vector
		///
		/// v/|v|
		///
		/// @return The vector, normalized
		///
		/////////////////////////////////////////////////////////////
		vector3<T> norm() const;

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
		/// @brief Get the normed version of the 3D vector
		///
		/// v/|v|
		///
		/// @return The vector, normed
		///
		/////////////////////////////////////////////////////////////
		vector3<T> sgn() const;

		/////////////////////////////////////////////////////////////
		/// @brief Compute cross product
		///
		/// the result is perpendicular to the original vector and
		/// to @a other
		///
		/// @param other The right opreand (vector)
		///
		/// @return The result vector
		///
		/////////////////////////////////////////////////////////////
		vector3<T> cross(const_reference other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Project the vector to @a target
		///
		/// target * (v <B>�</B> target) / |v|^2
		///
		/// @param target The vector to project to
		///
		/// @return The projected vector
		///
		/////////////////////////////////////////////////////////////
		vector3<T> projTo(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Project the vector to a plane
		///
		/// v - normal * (v <B>�</B> normal) / |v|^2
		///
		/// @param normal the normal vector of the plane
		///
		/// @return The projected vector
		///
		/////////////////////////////////////////////////////////////
		vector3<T> projToPlane(const_reference normal) const;

		/////////////////////////////////////////////////////////////
		/// @brief Reflect the vector on @a target
		///
		/// v * (v <B>�</B> target) / |v|^2 - v
		///
		/// @param target The vector to reflect on
		///
		/// @return The reflected vector
		///
		/////////////////////////////////////////////////////////////
		vector3<T> reflectOn(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Inverts the vector
		///
		/// x=1/x  y=1/y  z=1/z
		///
		/// @return The inverted vector
		///
		/////////////////////////////////////////////////////////////
		vector3<T> inv() const;
		
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
	/// @relates fm::vector3
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator+(const vector3<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,+)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator+(const vector3<T> &left,const vector2<T2> &right) -> vector3<RETTYPE(T,T2,+)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator+(const vector3<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,+)>;


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise subtraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator-(const vector3<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,-)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise subtraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator-(const vector3<T> &left,const vector2<T2> &right) -> vector3<RETTYPE(T,T2,-)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise subtraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator-(const vector3<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,-)>;


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector3<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,*)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector3<T> &left,const vector2<T2> &right) -> vector3<RETTYPE(T,T2,*)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector3<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,*)>;


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise division of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector3<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,/)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise division of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector3<T> &left,const vector2<T2> &right) -> vector3<RETTYPE(T,T2,/)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise division of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector3<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,/)>;
	

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator %
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise modulo of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector3<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,+)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator %
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise modulo of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector3<T> &left,const vector2<T2> &right) -> vector3<RETTYPE(T,T2,+)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator %
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise modulo of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector3<T> &left,const vector4<T2> &right) -> vector4<RETTYPE(T,T2,+)>;
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x b                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector3<T> &left,const T2 &right) -> vector3<decltype(left.x*right)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise multiplication of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const T2 &left,const vector3<T> &right) -> vector3<decltype(left*right.x)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise division of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector3<T> &left,const T2 &right) -> vector3<decltype(left.x/right)>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator %
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise modulo of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector3<T> &left,const T2 &right) -> vector3<decltype(left.x+right)>;
	
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x= B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator+=(vector3<T> &left,const vector3<T2> &right);
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator+=(vector3<T> &left,const vector2<T2> &right);
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator+=(vector3<T> &left,const vector4<T2> &right);
	

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator-=(vector3<T> &left,const vector3<T2> &right);
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator-=(vector3<T> &left,const vector2<T2> &right);
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator-=(vector3<T> &left,const vector4<T2> &right);
	

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator*=(vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator*=(vector3<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator*=(vector3<T> &left,const vector4<T2> &right);


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator/=(vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator/=(vector3<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator/=(vector3<T> &left,const vector4<T2> &right);


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator %=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator%=(vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator %=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator%=(vector3<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator %=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator%=(vector3<T> &left,const vector4<T2> &right);
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x= b                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator*=(vector3<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator/=(vector3<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator %=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<T> &operator%=(vector3<T> &left,const T2 &right);
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A eq B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left equals @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator==(const vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left equals @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator==(const vector3<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left equals @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator==(const vector3<T> &left,const vector4<T2> &right);


	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left not equals @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator!=(const vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left not equals @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator!=(const vector3<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left not equals @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator!=(const vector3<T> &left,const vector4<T2> &right);
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
						  x A                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of unary operator -
	///
	/// @param vec Vector to negate
	///
	/// @return Memberwise negation of @a vec
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator-(const vector3<T> &vec) -> vector3<decltype(-vec.x)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of unary operator +
	///
	/// @param vec Vector to negate
	///
	/// @return Memberwise negation of @a vec
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator+(const vector3<T> &vec) -> vector3<decltype(+vec.x)>;

	typedef vector3<float> 	       vec3;
	typedef vector3<int>   	       vec3i;
	typedef vector3<float> 	       vec3f;
	typedef vector3<double>        vec3d;
	typedef vector3<unsigned int>  vec3u;
	typedef vector3<unsigned char> vec3b;
}

namespace std
{
	template<class T>
	T abs(const fm::vector3<T> &vec);

	template<class T>
	fm::vector3<T> sgn(const fm::vector3<T> &vec);
}

#endif // FRONTIER_VECTOR3_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Vector3.inl>
#endif
