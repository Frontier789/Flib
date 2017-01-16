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
#ifndef FRONTIER_ANGLE_HPP_INCLUDED
#define FRONTIER_ANGLE_HPP_INCLUDED
#include <FRONTIER/System/util/RequireCpp11.hpp>
#define FRONTIER_ANGLE
#include <cmath>

#define F_PI 3.141592653589793238

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to hold and convert the value of an angle
	///
	/////////////////////////////////////////////////////////////
	template<class T = float>
	class Angle
	{
		T m_amount; ///< Holds the angle in radians
	
	public:
		typedef T component_type;
		typedef Angle &reference;
		typedef const Angle &const_reference;
		enum {
			components = 1u ///< Public value indicating the amount of component_types in the class
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes angle with zero
		///
		/////////////////////////////////////////////////////////////
		Angle();

		/////////////////////////////////////////////////////////////
		/// @brief Construct angle from value in radians
		/// 
		/// @param angle Value to initialize with (in radians)
		///
		/////////////////////////////////////////////////////////////
		Angle(const T &angle);

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// @param copy Angle to copy
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		Angle(const Angle<T2> &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Get the angle as  <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///
		/// @return The angle in  <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///
		/////////////////////////////////////////////////////////////
		T asDeg() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the angle as <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/// @return The angle in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/////////////////////////////////////////////////////////////
		T asRad() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the angle as <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/// @return The angle in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		operator Angle<T2>() const;
	};

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (Angle)
	///
	/// @return Result of the addition
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator+(const Angle<T> &left,const Angle<T2> &right) -> Angle<decltype(left.asRad()+right.asRad())>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (Angle)
	///
	/// @return Result of the substraction
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator-(const Angle<T> &left,const Angle<T2> &right) -> Angle<decltype(left.asRad()-right.asRad())>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of unary operator -
	///
	/// @param angle The operand (Angle)
	///
	/// @return Result of the negation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator-(const Angle<T> &angle) -> Angle<decltype(-angle.asRad())>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the multiplication
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const Angle<T> &left,const T2 &right) -> Angle<decltype(left.asRad()*right)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (scalar)
	/// @param right Right operand (Angle)
	///
	/// @return Result of the multiplication
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const T2 &left,const Angle<T> &right) -> Angle<decltype(left*right.asRad())>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the division
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator/(const Angle<T> &left,const T &right) -> Angle<decltype(left.asRad()/right)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (Angle)
	///
	/// @return Result of the division
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const Angle<T> &left,const Angle<T2> &right) -> Angle<decltype(left.asRad()/right.asRad())>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (Angle)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T> &operator+=(Angle<T> &left,const Angle<T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (Angle)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T> &operator-=(Angle<T> &left,const Angle<T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T> &operator*=(Angle<T> &left,const T &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T> &operator/=(Angle<T> &left,const T &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (Angle)
	///
	/// @return True if @a left equals @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator==(const Angle<T> &left,const Angle<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return True if @a left does not equal @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator!=(const Angle<T> &left,const Angle<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator <
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return True if @a left is smaller @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator< (const Angle<T> &left,const Angle<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator <=
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return True if @a left is smaller or equal @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator<=(const Angle<T> &left,const Angle<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator >
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return True if @a left is bigger @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator> (const Angle<T> &left,const Angle<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator >=
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return True if @a left is bigger or equal @a right
	///
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator>=(const Angle<T> &left,const Angle<T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Create a fm::Angle from @a amount degrees
	///
	/// @param amount The angle in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
	///
	/// @return The created instance of fm::Angle
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto deg(const T &amount) -> Angle<decltype(amount / 180.f * F_PI)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Create a fm::Angle from @a amount radians
	///
	/// @param amount The angle in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
	///
	/// @return The created instance of fm::Angle
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto rad(const T &amount) -> Angle<decltype(amount * F_PI)>;
	
	
	typedef Angle<float> Anglef;
}

namespace std
{
	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of the std::sin with fm::Angle
	///
	/// @param angle The angle to calculate the <a href="http://en.wikipedia.org/wiki/Sine">sine</a> of
	///
	/// @return <a href="http://en.wikipedia.org/wiki/Sine">sin</a>(angle)
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	auto sin(const fm::Angle<T> &angle) -> decltype(sin(angle.asRad()));

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of the std::cos with fm::Angle
	///
	/// @param angle The angle to calculate the <a href="http://en.wikipedia.org/wiki/Cosine#Sine.2C_cosine_and_tangent">cosine</a> of
	///
	/// @return <a href="http://en.wikipedia.org/wiki/Cosine#Sine.2C_cosine_and_tangent">cos</a>(angle)
	///
	/////////////////////////////////////////////////////////////
	template<class T>
    auto cos(const fm::Angle<T> &angle) -> decltype(cos(angle.asRad()));

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of the std::tan with fm::Angle
	///
	/// @param angle The angle to calculate the <a href="http://en.wikipedia.org/wiki/Cosine#Sine.2C_cosine_and_tangent">tangent</a> of
	///
	/// @return <a href="http://en.wikipedia.org/wiki/Cosine#Sine.2C_cosine_and_tangent">tan</a>(angle)
	///
	/////////////////////////////////////////////////////////////
	template<class T>
    auto tan(const fm::Angle<T> &angle) -> decltype(tan(angle.asRad()));
}

#endif // FRONTIER_ANGLE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Angle.inl>
#endif
