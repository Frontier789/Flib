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
#ifndef FRONTIER_POLAR3_HPP_INCLUDED
#define FRONTIER_POLAR3_HPP_INCLUDED
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Angle.hpp>
#define FRONTIER_POLAR3

/*
pol3(1, 0, 0) == vec3(1,0,0)
pol3(1,90, 0) == vec3(0,1,0)
pol3(1, 0,90) == vec3(0,0,1)
*/

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Templated class for using 3D <a href="https://en.wikipedia.org/wiki/Spherical_coordinate_system">spherical vectors</a>
	///
	/////////////////////////////////////////////////////////////
	template<class T,class A = float>
	class polar3
	{
	public:
	    typedef polar3 &reference;
	    typedef const polar3 &const_reference;

		union {
			T l;      ///< Access the length with the naming convension r (radius)
			T r;      ///< Access the length with the naming convension l (length)
			T length; ///< Access the length with the naming convension length
		};

		Angle<A> theta; /// Theta (angle on x,y plane)
		Angle<A> phi;   /// Phi   (angle on pol2(1,theta),z plane)

		/////////////////////////////////////////////////////////////
		/// @brief Construct a polar vector from length
		///
		/// Initializes 3D polar vector with (length,theta,phi)
		///
		/// @param length Length
		/// @param theta  Theta (angle on x,y plane)
		/// @param phi    Phi   (angle on pol2(1,theta),z plane)
		///
		/////////////////////////////////////////////////////////////
		explicit polar3(const T &length = T(),const Angle<A> &theta = fm::deg(0),const Angle<A> &phi = fm::deg(0));

		/////////////////////////////////////////////////////////////
		/// @brief Copy a polar vector
		///
		/// @param copy The vector to be copied
		///
		/////////////////////////////////////////////////////////////
		polar3(const_reference copy);

		/////////////////////////////////////////////////////////////
		/// @brief Convert 3D <a href="http://en.wikipedia.org/wiki/Euclidean_vector">euclidean vectors</a> to 3D polar vector
		///
		/// @param vec The vector to be converted
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		polar3(const vector3<T2> &vec);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator +=
		///
		/// @param other Right operand (polar vector)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class A2>
		reference operator+=(const polar3<T2,A2> &other);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator -=
		///
		/// @param other Right operand (polar vector)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class A2>
		reference operator-=(const polar3<T2,A2> &other);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator *=
		///
		/// @param scalar Right operand (scalar)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator*=(const T &scalar);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator /=
		///
		/// @param scalar Right operand (scalar)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator/=(const T &scalar);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator +
		///
		/// @param other Right operand (polar vector)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class A2>
		auto operator+(const polar3<T2,A2> &other) const -> polar3<decltype(length + other.length),decltype((theta + other.theta).asRad())>;

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator -
		///
		/// @param other Right operand (polar vector)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class A2>
		auto operator-(const polar3<T2,A2> &other) const -> polar3<decltype(length - other.length),decltype((theta - other.theta).asRad())>;

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator *
		///
		/// @param scalar Right operand (scalar)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		auto operator*(const T2 &scalar) const -> polar3<decltype(length * scalar),A>;

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator /
		///
		/// @param scalar Right operand (scalar)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		auto operator/(const T2 &scalar) const -> polar3<decltype(length / scalar),A>;

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator ==
		///
		/// @param other Right operand (polar vector)
		///
		/// @return True if the vector and @a other are equal
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class A2>
		bool operator==(const polar3<T2,A2> &other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator ==
		///
		/// @param other Right operand (polar vector)
		///
		/// @return True if the vector and @a other are not equal
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class A2>
		bool operator!=(const polar3<T2,A2> &other) const;

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
		polar3<T,A> cross(const_reference other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Project the polar vector to @a target
		///
		/// @param target The vector to project to
		///
		/// @return The projected vector
		///
		/////////////////////////////////////////////////////////////
		polar3<T,A> projTo(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Project the vector to a plane
		///
		/// @param normal the normal vector of the plane
		///
		/// @return The projected vector
		///
		/////////////////////////////////////////////////////////////
		polar3<T,A> projToPlane(const_reference normal) const;

		/////////////////////////////////////////////////////////////
		/// @brief Reflect the polar vector on @a pol
		///
		/// @param pol The vector to reflect on
		///
		/// @return The reflected vector
		///
		/////////////////////////////////////////////////////////////
		polar3<T,A> reflectOn(const_reference pol) const;

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the dot product of two polar vectors
		///
		/// @param pol The right operand of the dot product
		///
		/// @return The dot product
		///
		/////////////////////////////////////////////////////////////
		T dot(const_reference pol) const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert the polar vector to euclidean vector
		///
		/////////////////////////////////////////////////////////////
		operator vector3<T>() const;
	};

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar3
	/// @brief Overload of unary operator -
	///
	/// @param pol The polar vector to negate
	///
	/// @return @a pol negated
	///
	/////////////////////////////////////////////////////////////
	template<class T,class A>
	auto operator-(const polar3<T,A> &pol) -> polar3<decltype(-pol.length),A>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar3
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (scalar)
	/// @param right Right operand (polar vector)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T,class A,class T2>
	auto operator*(const T2 &left,const polar3<T,A> &right) -> polar3<decltype(left*right.length),A>;

	typedef polar3<float>  pol3;
	typedef polar3<float>  pol3f;
	typedef polar3<double> pol3d;
	typedef polar3<int>    pol3i;
}
#endif // FRONTIER_POLAR3_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Polar3.inl>
#endif
