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
#ifndef FRONTIER_VECTOR3_HPP_INCLUDED
#define FRONTIER_VECTOR3_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Will_result_in.hpp>
#define FRONTIER_VECTOR3
namespace fm
{
	template<class> class vector4;
	template<class> class vector3;
	template<class> class vector2;


	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Templated class for using
	///			   3D <a href="http://en.wikipedia.org/wiki/Euclidean_vector">euclidean vectors</a>
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
			components = 3u ///< Public value indicating the amount of component_type's in the class
		};
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

		}; ///< Anonymous union holding the value of first dimension

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

		}; ///< Anonymous union holding the value of second dimension

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

		}; ///< Anonymous union holding the value of third dimension


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
		/// @param Z The z coordinate
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		vector3(const vector2<T2> &other,const T2 &Z = T());

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
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference unsign();

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
		/// target * (v <B>·</B> target) / |v|^2
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
		/// target - target * (v <B>·</B> target) / |v|^2
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
		/// v * (v <B>·</B> target) / |v|^2 - v
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
		vector3<T> byComp(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Multiplies the vector's components
		///
		/// The multiplication is performed on all
		/// components invidually
		///
		/// @param X Multiply x by @a X
		/// @param Y Multiply y by @a Y
		/// @param Z Multiply z by @a Z
		///
		/// @return The product vector
		///
		/////////////////////////////////////////////////////////////
		vector3<T> byComp(const T &X,const T &Y,const T &Z) const;

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
		vector3<T> byCompInv(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the inverse of a multiplication
		///
		/// The multiplication is performed on all
		/// components invidually and then the inversion
		///
		/// @param X Multiply x by @a X
		/// @param Y Multiply y by @a Y
		/// @param Z Multiply z by @a Z
		///
		/// @return The result vector
		///
		/////////////////////////////////////////////////////////////
		vector3<T> byCompInv(const T &X,const T &Y,const T &Z) const;

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
		/// @brief Convert 3D vector to template class using constructor
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have a constructor which takes three Ts
		///
		/// @return Converted instance of T2
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		T2 convert() const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert 3D vector to template class with x,y,z fields
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have x, y and z fields
		///
		/// @return Converted instance of T2
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		T2 convertxyz() const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert 3D vector to template class with r,g,b fields
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have r, g and b fields
		///
		/// @return Converted instance of T2
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		T2 convertrgb() const;


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
		/// @brief Convert template class with x y and z fields to 3D vector
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have x, y and z fields
		///
		/// @param other The vector to convert
		///
		/// @return The converted vector
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		static vector3<T> load(const T2 &other);

		/////////////////////////////////////////////////////////////
		/// @brief Convert template class with x,y,z fields to 3D vector
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have x, y and z fields
		///
		/// @param other The vector to convert
		///
		/// @return The converted vector
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		static vector3<T> loadxyz(const T2 &other);

		/////////////////////////////////////////////////////////////
		/// @brief Convert template class with r g b fields to 3D vector
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have r, g and b fields
		///
		/// @param other The vector to convert
		///
		/// @return The converted vector
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		static vector3<T> loadrgb(const T2 &other);
	};

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
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,+)> operator+(const vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise substraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,-)> operator-(const vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise substraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector3<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise substraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector3<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (scalar)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise multiplication of @a right by @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const T &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left and @a right are the same
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator==(const vector3<T> &left,const vector3<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector3
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return True if @a left and @a right are not the same
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator!=(const vector3<T> &left,const vector3<T> &right);

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
	vector3<T> operator-(const vector3<T> &vec);


	typedef vector3<float> 		  vec3;
	typedef vector3<float> 		  vec3f;
	typedef vector3<int>   		  vec3i;
	typedef vector3<unsigned int> vec3u;

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

////////////////////////////////////////////////////////////
/// @class fm::vector3
/// @ingroup System
///
/// fm::vector3 is a templated class for manipulating 3D
/// vectors, it can also be used to represent color, position etc.
///
/// The template parameter T is the type of the coordinates. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
///
/// The most common specializations are typedefed:
/// @li fm::vector3<float> is fm::vec3
/// @li fm::vector3<float> is fm::vec3f
/// @li fm::vector3<int> is fm::vec3i
/// @li fm::vector3<unsigned int> is fm::vec3u
///
/// The fm::vector3 class posesses a simple yet feature rich interface, its x, y and z members
/// can be accessed directly and they have aliases such as r,g,b,x,y,z and it
/// contains mathematical function like dot product, cross product, length, etc.
///
/// Usage example:
/// @code
/// fm::vec3 v(5,4,2);
///	std::cout<<v.x<<" "; //
/// std::cout<<v.r<<" "; // these print the same value (5)
///	std::cout<<std::endl;
///
/// fm::vec3 v1(1,2,3),v2(4,5.5555,6.0606),v3;
/// v3 = v1 + v2*5.41f;
/// std::cout<<v3<<std::endl;
///
///
/// class OtherVectorType
/// {
///	public:
///		float x,y,z;
/// };
///
/// OtherVectorType otherVec;
/// otherVec.x = 42.2;
/// otherVec.y = -13;
/// otherVec.z = 13.05;
/// fm::vec3 v=fm::vec3::loadxyz(otherVec);
/// std::cout<<v<<std::endl;
///
/// OtherVectorType otherVec2;
/// fm::vec3 v(-42,42.5,0.1);
/// otherVec2=v.convertxyz<OtherVectorType>();
/// std::cout<<otherVec2.x<<" "<<otherVec2.y<<" "<<otherVec2.z<<std::endl;
///
///
/// // Functions
/// fm::vec3 v(-5,5.55,-5);
/// v.unsign();
/// std::cout<<v<<std::endl;
///
/// std::cout<<"example:"<<std::endl;
/// std::cout<<"\tlength    : "<<fm::vec2(5,4,-2.1).length()<<std::endl;
/// std::cout<<"\tLENGTH    : "<<fm::vec2(5,4,-2.1).LENGTH()<<std::endl;
/// std::cout<<"\tnorm      : "<<fm::vec2(5,4,-2.1).norm()<<std::endl;
/// std::cout<<"\tsgn       : "<<fm::vec2(5,4,-2.1).sgn()<<std::endl;
/// std::cout<<"\tcross     : "<<fm::vec2(5,4,-2.1).cross(fm::vec3(1,2,3))<<std::endl;
/// std::cout<<"\tprojTo    : "<<fm::vec2(5,4,-2.1).projTo(fm::vec2(1,0))<<std::endl;
/// std::cout<<"\treflectOn : "<<fm::vec2(5,4,-2.1).reflectOn(fm::vec2(1,0))<<std::endl;
/// std::cout<<"\tinv	    : "<<fm::vec2(5,4,-2.1).inv()<<std::endl;
/// std::cout<<"\tbyComp    : "<<fm::vec2(5,4,-2.1).byComp(fm::vec2(7,4))<<std::endl;
/// std::cout<<"\tbyCompInv : "<<fm::vec2(5,4,-2.1).byCompInv(fm::vec2(2,4))<<std::endl;
/// std::cout<<"\tdot 		: "<<fm::vec2(5,4,-2.1).dot(fm::vec2(-1,1))<<std::endl;
///
///
/// @endcode
///
/// @see fm::vector2
/// @see fm::vector4
///
////////////////////////////////////////////////////////////
