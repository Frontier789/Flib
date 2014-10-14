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
#ifndef FRONTIER_ANGLE_HPP_INCLUDED
#define FRONTIER_ANGLE_HPP_INCLUDED
#include <FRONTIER/Config.hpp>
#define FRONTIER_ANGLE
namespace fm
{
	/////////////////////////////////////////////////////////////
	///
	/// @brief Class used to hold and convert the value of an angle
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Angle
	{
		float m_amount; ///< A float holding the angle always in radians
	public:
		typedef float value_type;
		typedef Angle &reference;
		typedef const Angle &const_reference;
		enum {
			components = 1u ///< Public value indicating the amount of value_type's in the class
		};
		/////////////////////////////////////////////////////////////
		/// @brief Enumerated type used to decide if a float value
		///		   is in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///		   or in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/////////////////////////////////////////////////////////////
		enum MeasureType{Deg=0,     ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
						 DEG=0,     ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
						 degs=0,    ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
						 Degs=0,    ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
						 DEGS=0,    ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
						 Degrees=0, ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
						 DEGREES=0, ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>

						 Rad=1,     ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
						 RAD=1,     ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
						 rads=1,    ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
						 Rads=1,    ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
						 RADS=1,    ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
						 Radians=1, ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
						 RADIANS=1  ///< Indicates that the value is in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
						};

		/////////////////////////////////////////////////////////////
		/// @brief Public static value indicating that the constructor
		/// 	   treats the passed float as if it
		///		   is in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///		   or as if it is in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/////////////////////////////////////////////////////////////
		static MeasureType globalMeasureType;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes angle with zero
		///
		/////////////////////////////////////////////////////////////
		Angle();

		/////////////////////////////////////////////////////////////
		/// @brief Construct angle from value
		///
		/// Please note that if fm::Angle::globalMeasureType is Deg,DEG,degs,Degs,Degrees or DEGREES then
		/// @a angle is treated as if it was in <a href="http://en.wikipedia.org/wiki/Radian">radians</a> and if fm::Angle::globalMeasureType
		/// is Rad,RAD,rads,Rads,RADS,Radians or RADIANS then @a angle is treated as if it was in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>.
		///
		/// @param angle Value to initialize with
		///
		/////////////////////////////////////////////////////////////
		Angle(float angle);

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// @param copy Angle to copy
		///
		/////////////////////////////////////////////////////////////
		Angle(const Angle &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Construct angle from value
		///
		/// If @a type is Deg,DEG,degs,Degs,Degrees or DEGREES then @a angle is treated as if it was in
		/// <a href="http://en.wikipedia.org/wiki/Radian">radians</a> and if @a type
		/// is Rad,RAD,rads,Rads,RADS,Radians or RADIANS then @a angle is treated as if it was in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>.
		///
		/////////////////////////////////////////////////////////////
		Angle(float angle,MeasureType type);



		/////////////////////////////////////////////////////////////
		/// @brief Get the angle as <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///
		/// @return The angle in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///
		/////////////////////////////////////////////////////////////
		float asDergrees() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the angle as <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/// @return The angle in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/////////////////////////////////////////////////////////////
		float asRadians() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the angle as <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///
		/// @return The angle in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///
		/////////////////////////////////////////////////////////////
		float asDegs() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the angle as <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/// @return The angle in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/////////////////////////////////////////////////////////////
		float asRads() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the angle as  <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///
		/// @return The angle in  <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
		///
		/////////////////////////////////////////////////////////////
		float asDeg() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the angle as <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/// @return The angle in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
		///
		/////////////////////////////////////////////////////////////
		float asRad() const;
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
	Angle FRONTIER_API operator+(const Angle &left,const Angle &right);

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
	Angle FRONTIER_API operator-(const Angle &left,const Angle &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return Result of the multiplication
	///
	/////////////////////////////////////////////////////////////
	Angle FRONTIER_API operator*(const Angle &left,const float &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (float)
	/// @param right Right operand (Angle)
	///
	/// @return Result of the multiplication
	///
	/////////////////////////////////////////////////////////////
    Angle FRONTIER_API operator*(const float &left,const Angle &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return Result of the division
	///
	/////////////////////////////////////////////////////////////
	Angle FRONTIER_API operator/(const Angle &left,const float &right);

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
	Angle FRONTIER_API &operator+=(Angle &left,const Angle &right);

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
	Angle FRONTIER_API &operator-=(Angle &left,const Angle &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	Angle FRONTIER_API &operator*=(Angle &left,const float &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	Angle FRONTIER_API &operator/=(Angle &left,const float &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (Angle)
	/// @param right Right operand (float)
	///
	/// @return True if @a left equals @a right
	///
	/////////////////////////////////////////////////////////////
	bool FRONTIER_API operator==(const Angle &left,const Angle &right);

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
	bool FRONTIER_API operator!=(const Angle &left,const Angle &right);

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
	bool FRONTIER_API operator< (const Angle &left,const Angle &right);

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
	bool FRONTIER_API operator<=(const Angle &left,const Angle &right);

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
	bool FRONTIER_API operator> (const Angle &left,const Angle &right);

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
	bool FRONTIER_API operator>=(const Angle &left,const Angle &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Create a fm::Angle from @a amount degrees
	///
	/// @param amount The angle in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
	///
	/// @return The created instance of fm::Angle
	///
	/////////////////////////////////////////////////////////////
	Angle FRONTIER_API degrees(float amount);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Create a fm::Angle from @a amount radians
	///
	/// @param amount The angle in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
	///
	/// @return The created instance of fm::Angle
	///
	/////////////////////////////////////////////////////////////
	Angle FRONTIER_API radians(float amount);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Create a fm::Angle from @a amount degrees
	///
	/// @param amount The angle in <a href="http://en.wikipedia.org/wiki/Degree_%28angle%29">degrees</a>
	///
	/// @return The created instance of fm::Angle
	///
	/////////////////////////////////////////////////////////////
	Angle FRONTIER_API deg(float amount);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Create a fm::Angle from @a amount radians
	///
	/// @param amount The angle in <a href="http://en.wikipedia.org/wiki/Radian">radians</a>
	///
	/// @return The created instance of fm::Angle
	///
	/////////////////////////////////////////////////////////////
	Angle FRONTIER_API rad(float amount);
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
    float FRONTIER_API sin(const fm::Angle &angle);
    
	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of the std::cos with fm::Angle
	///
	/// @param angle The angle to calculate the <a href="http://en.wikipedia.org/wiki/Cosine#Sine.2C_cosine_and_tangent">cosine</a> of
	///
	/// @return <a href="http://en.wikipedia.org/wiki/Cosine#Sine.2C_cosine_and_tangent">cos</a>(angle)
	///
	/////////////////////////////////////////////////////////////
    float FRONTIER_API cos(const fm::Angle &angle);
    
	/////////////////////////////////////////////////////////////
	/// @relates fm::Angle
	/// @brief Overload of the std::tan with fm::Angle
	///
	/// @param angle The angle to calculate the <a href="http://en.wikipedia.org/wiki/Cosine#Sine.2C_cosine_and_tangent">tangent</a> of
	///
	/// @return <a href="http://en.wikipedia.org/wiki/Cosine#Sine.2C_cosine_and_tangent">tan</a>(angle)
	///
	/////////////////////////////////////////////////////////////
    float FRONTIER_API tan(const fm::Angle &angle);
    
}

#endif // FRONTIER_ANGLE_HPP_INCLUDED

////////////////////////////////////////////////////////////
/// @class fm::Angle
/// @ingroup System
///
/// Usage example:
/// @code
///
/// fm::Angle a(180);
/// std::cout<<std::cos(a)<<std::endl;
/// std::cout<<std::cos(fm::deg(180))<<std::endl;
/// std::cout<<std::cos(fm::rad(fm::PI))<<std::endl;
///
/// @endcode
////////////////////////////////////////////////////////////
