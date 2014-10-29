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
#ifndef FRONTIER_VECTOR2_HPP_INCLUDED
#define FRONTIER_VECTOR2_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Will_result_in.hpp>
#define FRONTIER_VECTOR2

namespace fm
{
	template<class> class vector4;
	template<class> class vector3;
	template<class> class vector2;


	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Templated class for using
	///			   2D <a href="http://en.wikipedia.org/wiki/Euclidean_vector">euclidean vectors</a>
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
			components = 2u ///< Public value indicating the amount of component_type's in the class
		};
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
			/// @brief real part
			///
			/// use first dimension as real part
			///
			T r;

		}; ///< Anonymous union holding the value of first dimension

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
			/// @brief imaginary part
			///
			/// use second dimension as imaginary part
			///
			T c;

		}; ///< Anonymous union holding the value of second dimension


		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes 2D vector with (0,0)
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
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference unsign();

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
		/// v * (v <B>·</B> target) / |v|^2 - v;
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
		/// x=1/x  y=1/y
		///
		/// @return The inverted vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> inv() const;

		/////////////////////////////////////////////////////////////
		/// @brief Multiplies the vector by @a target
		///
		/// The multiplication is performed on all
		/// components invidually
		///
		/// @param target The vector to multiply by
		///
		/// @return The product vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> byComp(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Multiplies the vector's components
		///
		/// The multiplication is performed on all
		/// components invidually
		///
		/// @param X Multiply x by @a X
		/// @param Y Multiply y by @a Y
		///
		/// @return The product vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> byComp(const T &X,const T &Y) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the inverse of a multiplication
		///
		/// The multiplication is performed on all
		/// components invidually and then the inversion
		///
		/// @param target The vector to multiply by
		///
		/// @return The result vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> byCompInv(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the inverse of a multiplication
		///
		/// The multiplication is performed on all
		/// components invidually and then the inversion
		///
		/// @param X Multiply x by @a X
		/// @param Y Multiply y by @a Y
		///
		/// @return The result vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> byCompInv(const T &X,const T &Y) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get a perpendicular vector
		///
		/// @return The perpendicular vector
		///
		/////////////////////////////////////////////////////////////
		vector2<T> perp() const;

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
		/// @brief Convert 2D vector to template class using constructor
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have a constructor which takes two Ts
		///
		/// @return Converted instance of T2
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		T2 convert() const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert 2D vector to template class with x y fields
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have x and y fields
		///
		/// @return Converted instance of T2
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		T2 convertxy() const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert 2D vector to template class with w h fields
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have w and h fields
		///
		/// @return Converted instance of T2
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		T2 convertwh() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get rw access to the minimum of {x,y}
		///
		/// @return min(x,y)
		///
		/////////////////////////////////////////////////////////////
		T &minxy();

		/////////////////////////////////////////////////////////////
		/// @brief Get value of the minimum of {x,y}
		///
		/// @return min(x,y)
		///
		/////////////////////////////////////////////////////////////
		const T &minxy() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get rw access to the minimum of {w,h}
		///
		/// @return min(w,h)
		///
		/////////////////////////////////////////////////////////////
		T &minwh();

		/////////////////////////////////////////////////////////////
		/// @brief Get value of the minimum of {w,h}
		///
		/// @return min(w,h)
		///
		/////////////////////////////////////////////////////////////
		const T &minwh() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get rw access to the maximum of {x,y}
		///
		/// @return max(x,y)
		///
		/////////////////////////////////////////////////////////////
		T &maxxy();

		/////////////////////////////////////////////////////////////
		/// @brief Get value of the maximum of {x,y}
		///
		/// @return max(x,y)
		///
		/////////////////////////////////////////////////////////////
		const T &maxxy() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get rw access to the maximum of {w,h}
		///
		/// @return max(w,h)
		///
		/////////////////////////////////////////////////////////////
		T &maxwh();

		/////////////////////////////////////////////////////////////
		/// @brief Get value of the maximum of {w,h}
		///
		/// @return max(w,h)
		///
		/////////////////////////////////////////////////////////////
		const T &maxwh() const;


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
		/// @brief Overload of binary operator ()
		///
		/// @param other The vector's new value
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(const_reference other);

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
		/// @brief Overload of binary operator *=
		///
		/// @param scalar The multiplicant
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator*=(const T &scalar);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator /=
		///
		/// @param scalar The divisor
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator/=(const T &scalar);

		/////////////////////////////////////////////////////////////
		/// @brief Convert template class with x y fields to 2D vector
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have x and y fields
		///
		/// @param other The vector to convert
		///
		/// @return The converted vector
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		static vector2<T> load(const T2 &other);

		/////////////////////////////////////////////////////////////
		/// @brief Convert template class with x y fields to 2D vector
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have x and y fields
		///
		/// @param other The vector to convert
		///
		/// @return The converted vector
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		static vector2<T> loadxy(const T2 &other);

		/////////////////////////////////////////////////////////////
		/// @brief Convert template class with w h fields to 2D vector
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have w and h fields
		///
		/// @param other The vector to convert
		///
		/// @return The converted vector
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		static vector2<T> loadwh(const T2 &other);
	};

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator+=(vector2<T> &left,const vector2<T2> &right);

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
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<FRONTIER_FIND_RETURN_TYPE(T,T2,+)> operator+(const vector2<T> &left,const vector2<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise substraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<FRONTIER_FIND_RETURN_TYPE(T,T2,-)> operator-(const vector2<T> &left,const vector2<T2> &right);

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
	vector2<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector2<T> &left,const vector2<T2> &right);

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
	vector2<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector2<T> &left,const vector2<T2> &right);

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
	vector2<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector2<T> &left,const T2 &right);

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
	vector2<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector2<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector2
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (scalar)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise multiplication with @a other
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const T &left,const vector2<T2> &right);

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
	vector2<T> operator-(const vector2<T> &vec);


	typedef vector2<float> 		  vec2;
	typedef vector2<float> 		  vec2f;
	typedef vector2<int>   		  vec2i;
	typedef vector2<unsigned int> vec2u;

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

////////////////////////////////////////////////////////////
/// @class fm::vector2
/// @ingroup System
///
/// fm::vector2 is a templated class for manipulating 2D
/// vectors, it can also be used to represent size, position etc.
///
/// The template parameter T is the type of the coordinates. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
///
/// The most common specializations are typedefed:
/// @li fm::vector2<float> is fm::vec2
/// @li fm::vector2<float> is fm::vec2f
/// @li fm::vector2<int> is fm::vec2i
/// @li fm::vector2<unsigned int> is fm::vec2u
/// @li fm::vector2<fm::Size> is fm::vec2s
///
/// The fm::vector2 class posesses a simple yet feature rich interface, its x and y members
/// can be accessed directly and they have aliases such as w,h,r,c and it
/// contains mathematical function like dot product, cross product, length, etc.
///
/// Usage example:
/// @code
/// fm::vec2 v(5,4);
///	std::cout<<v.x<<" "; //
/// std::cout<<v.w<<" "; // these print the same value (5)
/// std::cout<<v.r<<" "; //
///	std::cout<<std::endl;
///
/// fm::vec2 v1(1,2),v2(4,5.5555),v3;
/// v3 = v1 + v2*5.41f;
/// std::cout<<v3<<std::endl;
///
///
/// class OtherVectorType
/// {
///	public:
///		float x,y;
/// };
///
/// OtherVectorType otherVec;
/// otherVec.x = 42.2;
/// otherVec.y = -13;
/// fm::vec2 v=fm::vec2::loadxy(otherVec);
/// std::cout<<v<<std::endl;
///
/// OtherVectorType otherVec2;
/// fm::vec2 v(-42,42.5);
/// otherVec2=v.convert<OtherVectorType>();
/// std::cout<<otherVec2.x<<" "<<otherVec2.y<<std::endl;
///
///
/// // Functions
/// fm::vec2 v(-5,5.55);
/// v.unsign();
/// std::cout<<v<<std::endl;
///
/// std::cout<<"example:"<<std::endl;
/// std::cout<<"\tlength    : "<<fm::vec2(5,4).length()<<std::endl;
/// std::cout<<"\tLENGTH    : "<<fm::vec2(5,4).LENGTH()<<std::endl;
/// std::cout<<"\tnorm      : "<<fm::vec2(5,4).norm()<<std::endl;
/// std::cout<<"\tsgn       : "<<fm::vec2(5,4).sgn()<<std::endl;
/// std::cout<<"\tprojTo    : "<<fm::vec2(5,4).projTo(fm::vec2(1,0))<<std::endl;
/// std::cout<<"\treflectOn : "<<fm::vec2(5,4).reflectOn(fm::vec2(1,0))<<std::endl;
/// std::cout<<"\tinv	    : "<<fm::vec2(5,4).inv()<<std::endl;
/// std::cout<<"\tbyComp    : "<<fm::vec2(5,4).byComp(fm::vec2(7,4))<<std::endl;
/// std::cout<<"\tbyCompInv : "<<fm::vec2(5,4).byCompInv(fm::vec2(2,4))<<std::endl;
/// std::cout<<"\tperp 		: "<<fm::vec2(5,4).perp()<<std::endl;
/// std::cout<<"\tdot 		: "<<fm::vec2(5,4).dot(fm::vec2(-1,1))<<std::endl;
///
///
/// @endcode
///
/// @see fm::vector3
/// @see fm::vector4
///
////////////////////////////////////////////////////////////
