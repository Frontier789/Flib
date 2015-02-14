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
#ifndef FRONTIER_VECTOR4_HPP_INCLUDED
#define FRONTIER_VECTOR4_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Will_result_in.hpp>
#include <FRONTIER/System/macros/ONLYFORDOXY.hpp>
#define FRONTIER_VECTOR4
namespace fm
{
	template<class> class vector4;
	template<class> class vector3;
	template<class> class vector2;


	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Templated class for using
	///			   4D <a href="http://en.wikipedia.org/wiki/Euclidean_vector">euclidean vectors</a>
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
			components = 4u ///< Public value indicating the amount of component_type's in the class
		};
		
		static vector4<T> White;     ///< 0xFFFFFFFF
		static vector4<T> Silver;    ///< 0xBFBFBFFF
		static vector4<T> Gray;      ///< 0x7B7B7BFF
		static vector4<T> Grey;      ///< 0x7B7B7BFF
		static vector4<T> Black;     ///< 0x000000FF
		static vector4<T> Red;       ///< 0xFF0000FF
		static vector4<T> Brown;     ///< 0x7B4000FF
		static vector4<T> Yellow;    ///< 0xFFFF00FF
		static vector4<T> Lime;      ///< 0x00FF00FF
		static vector4<T> Green;     ///< 0x218C21FF
		static vector4<T> Teal;      ///< 0x007B7BFF
		static vector4<T> Blue;      ///< 0x0000FFFF
		static vector4<T> Navy;      ///< 0x00007BFF
		static vector4<T> Purple;    ///< 0x8F00D6FF
		static vector4<T> Maroon;    ///< 0x7B0000FF
		static vector4<T> Pink;      ///< 0xFF00FFFF
		static vector4<T> Olive;     ///< 0x7B7B00FF
		static vector4<T> Orange;    ///< 0xFFA300FF
		static vector4<T> Aqua;      ///< 0x00FFFFFF
		static vector4<T> Bone;      ///< 0xF2F2E6FF
		static vector4<T> Tan;       ///< 0xD1B28CFF
		static vector4<T> Charcoal;  ///< 0x444444FF
		static vector4<T> Royal;     ///< 0x074C9EFF
		static vector4<T> Azure;     ///< 0x007FFFFF
		static vector4<T> Aquamarine;///< 0x7FFFD2FF
		static vector4<T> Forest;    ///< 0x218921FF
		static vector4<T> Gold;      ///< 0xFFD600FF
		static vector4<T> Coral;     ///< 0xFF7F4FFF
		static vector4<T> Lavender;  ///< 0xB57FDBFF
		static vector4<T> Indigo;    ///< 0x490082FF
		
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

		}; ///< Anonymous union holding the value of fourth dimension


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
		vector4(const T &XYZ,const T &W=T(1.0));

		/////////////////////////////////////////////////////////////
		/// @brief Construct 4D vector from four coordinates
		///
		/// @param X x coordinate
		/// @param Y y coordinate
		/// @param Z z coordinate
		/// @param W w coordinate
		///
		/////////////////////////////////////////////////////////////
		vector4(const T &X,const T &Y,const T &Z,const T &W=T(1));

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
		vector4(const T (&XYZ)[3],const T &W=T(1));

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
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference unsign();

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
		vector4<T> byComp(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Multiplies the vector's components
		///
		/// The multiplication is performed on all
		/// components invidually
		///
		/// @param X Multiply x by @a X
		/// @param Y Multiply y by @a Y
		/// @param Z Multiply z by @a Z
		/// @param W Multiply w by @a W
		///
		/// @return The product vector
		///
		/////////////////////////////////////////////////////////////
		vector4<T> byComp(const T &X,const T &Y,const T &Z,const T &W=T(1.0)) const;

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
		vector4<T> byCompInv(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the inverse of a multiplication
		///
		/// The multiplication is performed on all
		/// components invidually and then the inversion
		///
		/// @param X Multiply x by @a X
		/// @param Y Multiply y by @a Y
		/// @param Z Multiply z by @a Z
		/// @param W Multiply w by @a W
		///
		/// @return The result vector
		///
		/////////////////////////////////////////////////////////////
		vector4<T> byCompInv(const T &X,const T &Y,const T &Z,const T &W=T(1.0)) const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert 4D vector to template class using constructor
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
		/// @brief Convert 4D vector to template class with x,y,z,w fields
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have x, y, z and w fields
		///
		/// @return Converted instance of T2
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		T2 convertxyzw() const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert 4D vector to template class with r,g,b,a fields
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have r, g, b and a fields
		///
		/// @return Converted instance of T2
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		T2 convertrgba() const;

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
		/// @brief Overload of binary operator ()
		///
		/// @param vec The vector's new value
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(const_reference vec);

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
		/// @brief Convert hex value to 4D color
		///
		/// @param hex Hexadecimal value of the color
		///
		/// @return converted color
		///
		/////////////////////////////////////////////////////////////
		static vector4<T> fromHex(unsigned int hex);

		/////////////////////////////////////////////////////////////
		/// @brief Convert template class with x, y, z and w fields to 4D vector
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have x, y, z and w fields
		///
		/// @param vec The vector to convert
		///
		/// @return The converted vector
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		static vector4<T> load(const T2 &vec);

		/////////////////////////////////////////////////////////////
		/// @brief Convert template class with x,y,z,w fields to 4D vector
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have x, y, z and w fields
		///
		/// @param vec The vector to convert
		///
		/// @return The converted vector
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		static vector4<T> loadxyzw(const T2 &vec);

		/////////////////////////////////////////////////////////////
		/// @brief Convert template class with r,g,b,a fields to 4D vector
		///
		/// This functions fails to compile ifdef
		/// T2 doesn't have r, g, b and a fields
		///
		/// @param vec The vector to convert
		///
		/// @return The converted vector
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		static vector4<T> loadrgba(const T2 &vec);

		/////////////////////////////////////////////////////////////
		/// @brief Convert the vector to bool
		///
		/// @return True iff the vector's length is not 0 (at least one of x,y,z,w is not 0)
		///
		/////////////////////////////////////////////////////////////
		operator bool() const;
	};
	
#ifndef FRONTIER_ONLY_FOR_DOXYGEN

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
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,+)> operator+(const vector4<T> &left,const vector4<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise substraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,-)> operator-(const vector4<T> &left,const vector4<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector4<T> &left,const vector4<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise substraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector4<T> &left,const vector4<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise addition of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector4<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (vector)
	/// @param right Right operand (scalar)
	///
	/// @return Memberwise substraction of @a left and @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector4<T> &left,const T2 &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::vector4
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (scalar)
	/// @param right Right operand (vector)
	///
	/// @return Memberwise multiplication of @a right by @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const T &left,const vector4<T2> &right);

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
	/// @brief Overload of unary operator -
	///
	/// @param vec Vector to negate
	///
	/// @return Memberwise negation of @a vec
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	vector4<T> operator-(const vector4<T> &vec);

#endif

	typedef vector4<float> 	      vec4;
	typedef vector4<float> 	      vec4f;
	typedef vector4<double>       vec4d;
	typedef vector4<int>   	      vec4i;
	typedef vector4<unsigned int> vec4u;
}

#endif // FRONTIER_VECTOR4_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Vector4.inl>
#endif

////////////////////////////////////////////////////////////
/// @class fm::vector4
/// @ingroup System
///
/// fm::vector4 is a templated class for manipulating 4D
/// vectors, it can also be used to represent color, position etc.
///
/// The template parameter T is the type of the coordinates. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
///
/// The most common specializations are typedefed:
/// @li fm::vector4<float> is fm::vec3
/// @li fm::vector4<float> is fm::vec3f
/// @li fm::vector4<int> is fm::vec3i
/// @li fm::vector4<unsigned int> is fm::vec3u
///
/// The fm::vector4 class posesses a simple yet feature rich interface, its x, y, z and w members
/// can be accessed directly and they have aliases such as r,g,b,a,x,y,z,w and it
/// contains mathematical function such as length, norm etc.
///
/// Usage example:
/// @code
/// fm::vec4 v(5,4,2,1);
///	std::cout<<v.x<<" "; //
/// std::cout<<v.r<<" "; // these print the same value (5)
///	std::cout<<std::endl;
///
/// fm::vec4 v1(1,2,3,4),v2(4,5.5555,6.0606,1),v3;
/// v3 = v1 + v2*5.41f;
/// std::cout<<v3<<std::endl;
///
///
/// class OtherVectorType
/// {
///	public:
///		float r,g,b,a;
/// };
///
/// OtherVectorType otherVec;
/// otherVec.r = 1;
/// otherVec.g = .1;
/// otherVec.b = .4;
/// otherVec.a = 1;
/// fm::vec4 v=fm::vec4::loadrgba(otherVec);
/// std::cout<<v<<std::endl;
///
/// OtherVectorType otherVec2;
/// fm::vec3 v(-42,42.5,0.1);
/// otherVec2=v.convertrgba<OtherVectorType>();
/// std::cout<<otherVec2.r<<" "<<otherVec2.g<<" "<<otherVec2.b<<" "<<otherVec2.a<<std::endl;
///
///
/// // Functions
/// fm::vec4 v(-5,5.55,-5,0);
/// v.unsign();
/// std::cout<<v<<std::endl;
///
///
/// @endcode
///
/// @see fm::vector2
/// @see fm::vector3
///
////////////////////////////////////////////////////////////
