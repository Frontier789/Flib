#ifndef FRONTIER_POLAR_HPP_INCLUDED
#define FRONTIER_POLAR_HPP_INCLUDED
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Angle.hpp>
#define FRONTIER_POLAR
namespace fm
{

	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Templated class for using
	///			   2D <a href="http://en.wikipedia.org/wiki/Polar_coordinate_system">polar vectors</a>
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	class polar2
	{
	public:
	    typedef polar2 &reference;
	    typedef const polar2 &const_reference;
		union {
			T r;      ///< Access the length with the naming convension r (radius)
			T l;      ///< Access the length with the naming convension l (length)
			T length; ///< Access the length with the naming convension length
		}; ///< Anonymous union holding the length

		Angle angle; ///< The angle of the polar vector

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes 2D polar vector with (0,0)
		///
		/////////////////////////////////////////////////////////////
		polar2();

		/////////////////////////////////////////////////////////////
		/// @brief Construct a polar vector from length
		///
		/// Initializes 2D polar vector with (length,0)
		///
		/// @param length Length
		///
		/////////////////////////////////////////////////////////////
		explicit polar2(const T &length);

		/////////////////////////////////////////////////////////////
		/// @brief Construct a polar vector from length and angle
		///
		/// Initializes 2D polar vector with (length,angle)
		///
		/// @param length Length
		/// @param angle  Angle
		///
		/////////////////////////////////////////////////////////////
		polar2(const T &length,const Angle &angle);

		/////////////////////////////////////////////////////////////
		/// @brief Copy a polar vector
		///
		/// @param copy The vector to be copied
		///
		/////////////////////////////////////////////////////////////
		polar2(const_reference copy);

		/////////////////////////////////////////////////////////////
		/// @brief Convert 2D <a href="http://en.wikipedia.org/wiki/Euclidean_vector">euclidean vectors</a> to 2D polar vector
		///
		/// @param vec The vector to be converted
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		polar2(const vector2<T2> &vec);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator ()
		///
		/// @param Length The new length
		/// @param Alpha The new angle
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(const T &Length,const Angle &Alpha);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator ()
		///
		/// @param vec Overwrite the polar vector with @a vec (converted to polar vector first)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		template<class T2>
		reference operator()(const vector2<T2> &vec);

		/////////////////////////////////////////////////////////////
		/// @brief Project the polar vector to @a target
		///
		/// @param target The vector to project to
		///
		/// @return The projected vector
		///
		/////////////////////////////////////////////////////////////
		polar2<T> projTo(const_reference target) const;

		/////////////////////////////////////////////////////////////
		/// @brief Reflect the polar vector on @a pol
		///
		/// @param pol The vector to reflect on
		///
		/// @return The reflected vector
		///
		/////////////////////////////////////////////////////////////
		polar2<T> reflectOn(const_reference pol) const;

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
		operator vector2<T>() const;
	};


	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (polar vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> &operator+=(polar2<T> &left,const polar2<T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (polar vector)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> &operator-=(polar2<T> &left,const polar2<T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> &operator*=(polar2<T> &left,const T &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator /=
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (scalar)
	///
	/// @return Reference to @a left
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> &operator/=(polar2<T> &left,const T &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of unary operator -
	///
	/// @param pol The polar vector to negate
	///
	/// @return @a pol negated
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> operator-(const polar2<T> &pol);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (polar vector)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> operator+(const polar2<T> &left,const polar2<T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (polar vector)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> operator-(const polar2<T> &left,const polar2<T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> operator*(const polar2<T> &left,const T &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (scalar)
	/// @param right Right operand (polar vector)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> operator*(const T &left,const polar2<T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator /
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> operator/(const polar2<T> &left,const T &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (polar vector)
	///
	/// @return True if @a left and @a right are equal
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	bool operator==(const polar2<T> &left,const polar2<T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::polar2
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (polar vector)
	/// @param right Right operand (polar vector)
	///
	/// @return True if @a left and @a right are not equal
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	bool operator!=(const polar2<T> &left,const polar2<T> &right);

	typedef polar2<float> pol2;
	typedef polar2<float> pol2f;
	typedef polar2<int>   pol2i;
}
#endif // FRONTIER_POLAR_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Polar2.inl>
#endif

////////////////////////////////////////////////////////////
/// @class fm::polar2
/// @ingroup System
///
/// fm::polar2 is a templated class for manipulating 2D polar
/// vectors.
///
/// The template parameter T is the type of the length. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
///
/// The most common specializations are typedefed:
/// @li fm::polar2<float> is fm::pol2
/// @li fm::polar2<float> is fm::pol2f
/// @li fm::polar2<int> is fm::poli
///
/// Usage example:
/// @code
///
/// const int N=10;
/// fm::vec2 circlePts[N];
/// for (int i=0;i<N;i++) // filling an array with the points of a circle
/// 	circlePts[i] = fm::pol2(1,fm::deg(i/float(N-1)*360.f));
///
/// @endcode
///
/// @see fm::vector3
/// @see fm::vector4
///
////////////////////////////////////////////////////////////