#ifndef FRONTIER_TIME_HPP_INCLUDED
#define FRONTIER_TIME_HPP_INCLUDED
#define FRONTIER_TIME
namespace fm
{
	/////////////////////////////////////////////////////////////
	///
	/// @brief Class used to represent the amount of some time
	///
	/////////////////////////////////////////////////////////////
	class Time
	{
		long m_amount; ///< A long value holding the time in Microseconds
	public:
		typedef long value_type;
		typedef Time &reference;
		typedef const Time &const_reference;
		enum {
			dimensions = 1u ///< Public value indicating the amount of value_type's in the class
		};
		enum MeasureType {
			Microseconds=1,   ///< Indicates that the passed value means microseconds
			Milliseconds=1000,///< Indicates that the passed value means milliseconds
			Seconds=1000000   ///< Indicates that the passed value means seconds
		};

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes the time with zero
		///
		/////////////////////////////////////////////////////////////
		Time();

		/////////////////////////////////////////////////////////////
		/// @brief Create fm::Time from plain value
		///
		/// @param value The value
		/// @param type  The @a type decides how @a value is interpreted
		///
		/////////////////////////////////////////////////////////////
		Time(const long double &value,MeasureType type);

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// Simply copies the value of @a copy 
		///
		/////////////////////////////////////////////////////////////
		Time(const Time &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Get time value as microseconds
		///
		/// @return Represented value as microseconds
		///
		/////////////////////////////////////////////////////////////
		long asMicroseconds() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get time value as milliseconds
		///
		/// @return Represented value as milliseconds
		///
		/////////////////////////////////////////////////////////////
		int asMilliseconds() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get time value as seconds
		///
		/// @return Represented value as seconds
		///
		/////////////////////////////////////////////////////////////
		float asSeconds() const;
		

		/////////////////////////////////////////////////////////////
		/// @brief Get time value as microseconds
		///
		/// @return Represented value as microseconds
		///
		/////////////////////////////////////////////////////////////
		long asMicsecs() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get time value as milliseconds
		///
		/// @return Represented value as milliseconds
		///
		/////////////////////////////////////////////////////////////
		int asMilsecs() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get time value as seconds
		///
		/// @return Represented value as seconds
		///
		/////////////////////////////////////////////////////////////
		float asSecs() const;

		/////////////////////////////////////////////////////////////
		/// @brief Static const value indicating a time value of 0
		///
		/////////////////////////////////////////////////////////////
		static const Time Zero;
	};

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	Time operator+(const Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	Time operator-(const Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of unary operator -
	///
	/// @param time Operand (Time)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	Time operator-(const Time &time);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	Time operator*(const Time &left,const float &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (scalar)
	/// @param right Right operand (Time)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	Time operator*(const float &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	Time operator/(const Time &left,const float &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	Time &operator+=(Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	Time &operator-=(Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	Time &operator*=(Time &left,const float &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	Time &operator/=(Time &left,const float &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return True if @a left and @a right are equal
	///
	/////////////////////////////////////////////////////////////
	bool operator==(const Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return True if @a left and @a right are not equal
	///
	/////////////////////////////////////////////////////////////
	bool operator!=(const Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return True if @a left is smaller than @a right
	///
	/////////////////////////////////////////////////////////////
	bool operator< (const Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return True if @a left is smaller or equal than @a right
	///
	/////////////////////////////////////////////////////////////
	bool operator<=(const Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return True if @a left is bigger than @a right
	///
	/////////////////////////////////////////////////////////////
	bool operator> (const Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (Time)
	/// @param right Right operand (Time)
	///
	/// @return True if @a left is bigger or equal than @a right
	///
	/////////////////////////////////////////////////////////////
	bool operator>=(const Time &left,const Time &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Create fm::Time from long value
	///
	/// @param Msecs The long value indicating time in microseconds
	///
	/// @return The create fm::Time object
	///
	/////////////////////////////////////////////////////////////
	Time microseconds(long Msecs);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Create fm::Time from int value
	///
	/// @param msecs The int value indicating time in milliseconds
	///
	/// @return The create fm::Time object
	///
	/////////////////////////////////////////////////////////////
	Time milliseconds(int msecs);

	/////////////////////////////////////////////////////////////
	/// @relates fm::Time
	/// @brief Create fm::Time from float value
	///
	/// @param secs The float value indicating time in seconds
	///
	/// @return The create fm::Time object
	///
	/////////////////////////////////////////////////////////////
	Time seconds(float secs);
}
#endif //FRONTIER_TIME_HPP_INCLUDED

////////////////////////////////////////////////////////////
/// @class fm::Time
/// @ingroup System
///
/// Class used to store length of time period
///
/// Usage example:
/// @code
///
/// fm::Time t(5,fm::Time::Seconds);
/// std::cout<<(t<fm::microseconds(5000))<<std::endl;
/// std::cout<<(t+fm::Time(4000,fm::Time::Milliseconds)).asSeconds()<<"s"<<std::endl;
/// 
/// @endcode
/// @see fm::Clock
////////////////////////////////////////////////////////////
