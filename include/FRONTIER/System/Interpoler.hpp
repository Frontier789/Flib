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
#ifndef FRONTIER_INTERPOLER_HPP_INCLUDED
#define FRONTIER_INTERPOLER_HPP_INCLUDED
#define FRONTIER_FUNCTIONS
#include <vector>

namespace fm
{
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Templated virtual base class of the fm::Interpoler class
	///
	/////////////////////////////////////////////////////////////
	template<class VAL,class T=double>
	class Interpoler_Base
	{
	public:
		/////////////////////////////////////////////////////////////
		///
		/// 	@brief A class that holds a VAL (val) and a T (t) indicating that t is mapped to val
		///
		/////////////////////////////////////////////////////////////
		class val_map
		{
		public:
			T t; ///< The mapping point
			VAL val; ///< The mapped value

			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			///
			/// Initializes the mapping point and the mapped value with their types' 
			/// default constructor
			/// 
			/////////////////////////////////////////////////////////////
			val_map();

			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			///
			/// Initializes the mapping point with @a t and the mapped value with 
			/// @a val
			/// 
			/// @param t The mapping point
			/// @param val The mapped value
			/// 
			/////////////////////////////////////////////////////////////
			val_map(const T &t,const VAL &val);
		};
	protected:
		std::vector<val_map> m_data; ///< m_data holds the values (and their mapping points) to be maped
	private:

		/////////////////////////////////////////////////////////////
		/// @brief Inform the class that the data has been changed
		///
		/// This function will be implemented if fm::Interpoler
		/// And it will be only called iternally
		/// 
		/////////////////////////////////////////////////////////////
		virtual void updated()=0;

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the value of the interpolation at @a t
		///
		/// This function will be implemented if fm::Interpoler
		///
		/// @param t The point where the value to be calculated 
		/// 
		/// @return The calculated value
		/// 
		/////////////////////////////////////////////////////////////
		virtual VAL calcVal(const T &t) const=0;
	public:

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Interpoler_Base();

		/////////////////////////////////////////////////////////////
		/// @brief Initialize from data
		/// 
		/// @param data The mapping data
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler_Base(const std::vector<val_map> &data);

		/////////////////////////////////////////////////////////////
		/// @brief Append mapping data
		/// 
		/// @param t The mapping point
		/// @param val The mapped value
		/// 
		/////////////////////////////////////////////////////////////
		void push_back(const T &t,const VAL &val);

		/////////////////////////////////////////////////////////////
		/// @brief Append mapping data
		/// 
		/// @param tval The mapping point and the mapped value
		/// 
		/////////////////////////////////////////////////////////////
		void push_back(const val_map &tval);

		/////////////////////////////////////////////////////////////
		/// @brief Change the number of mapping data pairs
		/// 
		/// Please note that if @a size is begger than the original size
		/// then the new values will be val_map() which may make the interpolation invalid
		/// therefore it is adviseable to use use this function with setPt,setVal,setT
		/// 
		/// @param size The new size
		/// 
		/////////////////////////////////////////////////////////////
		void resize(unsigned int size);

		/////////////////////////////////////////////////////////////
		/// @brief Get the number of mapping pairs
		/// 
		/// @return The number mapping pairs 
		/// 
		/////////////////////////////////////////////////////////////
		unsigned int size() const;

		/////////////////////////////////////////////////////////////
		/// @brief Change a mapping pair
		/// 
		/// @param index The index of the mapping pair 
		/// @param tval The new mapping point and mapped values 
		/// 
		/////////////////////////////////////////////////////////////
		void setPt(unsigned int index,const val_map &tval);

		/////////////////////////////////////////////////////////////
		/// @brief Change a mapping pair
		/// 
		/// @param index The index of the mapping pair 
		/// @param t The new mapping point
		/// @param val The new mapped value
		/// 
		/////////////////////////////////////////////////////////////
		void setPt(unsigned int index,const T &t,const VAL &val);

		/////////////////////////////////////////////////////////////
		/// @brief Change a mapped value
		/// 
		/// @param index The index of the mapped value
		/// @param val The new mapped value
		/// 
		/////////////////////////////////////////////////////////////
		void setVal(unsigned int index,const VAL &val);

		/////////////////////////////////////////////////////////////
		/// @brief Change a mapping point
		/// 
		/// @param index The index of the mapping pair 
		/// @param t The new mapping point
		/// 
		/////////////////////////////////////////////////////////////
		void setT(unsigned int index,const T &t);

		/////////////////////////////////////////////////////////////
		/// @brief Get a mapping point
		/// 
		/// @param index The index of the mapping point
		/// 
		/// @return The mapping point at @a index
		/// 
		/////////////////////////////////////////////////////////////
		const T &getT(unsigned int index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get a mapped value
		/// 
		/// @param index The index of the mapped value
		/// 
		/// @return The mapped value at @a index
		/// 
		/////////////////////////////////////////////////////////////
		const VAL &getVal(unsigned int index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get a mapping pair
		/// 
		/// @param index The index of the mapping pair
		/// 
		/// @return The mapping pair at @a index
		/// 
		/////////////////////////////////////////////////////////////
		const val_map &getPt(unsigned int index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get write-only access to the mapping data 
		/// 
		/// @return const reference to the mapping data
		/// 
		/////////////////////////////////////////////////////////////
		const std::vector<val_map> &getData() const;

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the value at a given mapping point
		/// 
		/// @return The value
		/// 
		/////////////////////////////////////////////////////////////
		VAL operator()(const T &t) const;
	};

	/////////////////////////////////////////////////////////////
	/// @brief Indicates the type of the interpolation
	/// 
	/////////////////////////////////////////////////////////////
	enum InterpolerType { 
		Polynomial,  ///< <a href="http://en.wikipedia.org/wiki/Polynomial_interpolation">Polynomial interpolations</a>
		Nearest,     ///< <a href="http://en.wikipedia.org/wiki/Nearest-neighbor_interpolation">Nearest interpolations</a>
		Linear       ///< <a href="http://en.wikipedia.org/wiki/Linear_interpolation">Linear interpolations</a>
	};

	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Templated class for using
	///			   <a href="http://en.wikipedia.org/wiki/Interpolation">interpolations</a>
	/// 
	/// @ingroup System
	/// 
	/// @see Interpoler<Polynomial,VAL,T>
	/// @see Interpoler<Nearest,VAL,T>
	/// @see Interpoler<Linear,VAL,T>
	/// 
	/////////////////////////////////////////////////////////////
	template<InterpolerType Type,class VAL,class T=double>
	class Interpoler;

	// Polynomial
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Partial specialization of Interpoler<Type,VAL,T> class for
	///			   <a href="http://en.wikipedia.org/wiki/Polynomial_interpolation">Polynomial interpolations</a>
	/// 
	/// @ingroup System
	/// 
	/// You can use this class to define a <a href="http://en.wikipedia.org/wiki/Polynomial_interpolation">polynomial interpolation</a>
	/// that maps T type to VAL type.
	/// 
	/// Please note that the followings are required in order not to generate a compilation error:
	/// 	@li A constructor for T that can be called with a double (T(1.0))
	/// 	@li A multiplication (*) operator that accepts a /const VAL &/ and a /const   T &/
	/// 	@li A multiplication (*) operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li A addition       (+) operator that accepts a /const VAL &/ and a /const VAL &/
	/// 	@li A substraction   (-) operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li A division       (/) operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li VAL and T must be copyable
	/// 	@li VAL() must be null value for addition (VAL()+VAL()==VAL())
	/// 
	/// vector2, vector3, vector4 and all floating-point types satisfy the requirements of VAL<br/>
	/// Also every floating-point types satisfy the requirements of T
	/// 
	/// @see Interpoler<Nearest,VAL,T>
	/// @see Interpoler<Linear,VAL,T>
	/// 
	/////////////////////////////////////////////////////////////
	template<class VAL,class T>
	class Interpoler<Polynomial,VAL,T> : public Interpoler_Base<VAL,T>
	{
	private:
		mutable std::vector<VAL> m_polinom; ///< Holds the calculated polynomial
		mutable bool m_isPolinomValid;      ///< Indicates the polynomial's validity

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the polynomial
		/// 
		/// This function is called iternally
		/// 
		/////////////////////////////////////////////////////////////
		void createPolinom() const;
	public:
		typedef typename Interpoler_Base<VAL,T>::val_map val_map;
		typedef Interpoler<Polynomial,VAL,T> &reference;
		typedef const Interpoler<Polynomial,VAL,T> &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler();

		/////////////////////////////////////////////////////////////
		/// @brief Copy-convert constructor
		/// 
		/// The same data can be used in any type of interpolation therefore
		/// what this this constructor does is copy m_data
		/// 
		/// @param copy The Interpolater to be copied
		/// 
		/////////////////////////////////////////////////////////////
		template<InterpolerType Type2>
		Interpoler(const Interpoler<Type2,VAL,T> &copy);

		/////////////////////////////////////////////////////////////
		/// @brief The implementation of fm::Interpoler_Base<VAL,T>::updated
		/// 
		/////////////////////////////////////////////////////////////
		void updated();

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the value of the interpolation at @a t
		///
		/// This is the implementation of fm::Interpoler_Base<VAL,T>::calcVal
		///
		/// @param t The point where the value to be calculated 
		/// 
		/// @return The calculated value
		/// 
		/////////////////////////////////////////////////////////////
		VAL calcVal(const T &t) const;

		/////////////////////////////////////////////////////////////
		/// @brief Read-only access to the polynomial
		/// 
		/// @return const reference to the polynomial
		/// 
		/////////////////////////////////////////////////////////////
		const std::vector<VAL> &getPolinom() const;

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the sum of two polynomial interpolations
		/// 
		/// This operator is used when declaring a bipolynomial interpolation
		/// (Interpoler<Polynomial,Interpoler<Polynomial,double,double>,double>)
		/// 
		/// @return The sum
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler<Polynomial,VAL,T> operator+(const_reference other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Multiply a Polynomial interpolation with its mapping type 
		/// 
		/// This operator is used when declaring a bipolynomial interpolation
		/// (Interpoler<Polynomial,Interpoler<Polynomial,double,double>,double>)
		/// 
		/// @return The result of the multiplication
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler<Polynomial,VAL,T> operator*(const T &ratio) const;
	};


	// Linear
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Partial specialization of Interpoler<Type,VAL,T> class for
	///			   <a href="http://en.wikipedia.org/wiki/Linear_interpolation">Linear interpolations</a>
	///
	/// @ingroup System
	/// 
	/// You can use this class to define a <a href="http://en.wikipedia.org/wiki/Linear_interpolation">linear interpolation</a>
	/// that maps T type to VAL type.
	/// 
	/// Please note that the followings are required in order not to generate a compilation error
	/// 	@li A constructor for T that can be called with a double (T(1.0))
	/// 	@li A multiplication (*)  operator that accepts a /const VAL &/ and a /const   T &/
	/// 	@li A addition       (+)  operator that accepts a /const VAL &/ and a /const VAL &/
	/// 	@li A substraction   (-)  operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li A comparation    (<=) operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li A comparation    (>=) operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li A comparation    (>)  operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li A comparation    (<)  operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li A division       (/)  operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li VAL and T must be copyable
	/// 
	/// vector2, vector3, vector4 and all floating-point types satisfy the requirements of VAL<br/>
	/// Also every floating-point types satisfy the requirements of T
	/// 
	/// @see Interpoler<Polynomial,VAL,T>
	/// @see Interpoler<Nearest,VAL,T>
	/// 
	/////////////////////////////////////////////////////////////
	template<class VAL,class T>
	class Interpoler<Linear,VAL,T> : public Interpoler_Base<VAL,T>
	{
	public:
		typedef typename Interpoler_Base<VAL,T>::val_map val_map;
		typedef Interpoler<Linear,VAL,T> &reference;
		typedef const Interpoler<Linear,VAL,T> &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler();

		/////////////////////////////////////////////////////////////
		/// @brief Copy-convert constructor
		/// 
		/// The same data can be used in any type of interpolation therefore
		/// what this this constructor does is copy m_data
		/// 
		/// @param copy The Interpolater to be copied
		/// 
		/////////////////////////////////////////////////////////////
		template<InterpolerType Type2>
		Interpoler(const Interpoler<Type2,VAL,T> &copy);

		/////////////////////////////////////////////////////////////
		/// @brief The implementation of fm::Interpoler_Base<VAL,T>::updated
		/// 
		/////////////////////////////////////////////////////////////
		void updated();

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the value of the interpolation at @a t
		///
		/// This is the implementation of fm::Interpoler_Base<VAL,T>::calcVal
		///
		/// @param t The point where the value to be calculated 
		/// 
		/// @return The calculated value
		/// 
		/////////////////////////////////////////////////////////////
		VAL calcVal(const T &t) const;

		/////////////////////////////////////////////////////////////
		/// @brief rw access to the mapping data
		///
		/// @param index The offset in the data
		/// 
		/// @return reference to the mapping pair
		/// 
		/////////////////////////////////////////////////////////////
		val_map &operator[](unsigned int index);

		/////////////////////////////////////////////////////////////
		/// @brief read-only access to the mapping data
		///
		/// @param index The offset in the data
		/// 
		/// @return const reference to the mapping pair
		/// 
		/////////////////////////////////////////////////////////////
		const val_map &operator[](unsigned int index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the sum of two polynomial interpolations
		/// 
		/// This operator is used when declaring an interpolation like
		/// Interpoler<Polynomial,Interpoler<Linear,double,double>,double>
		/// 
		/// @return The sum
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler<Linear,VAL,T> operator+(const_reference other) const;

		/////////////////////////////////////////////////////////////
		/// @brief Multiply a Polynomial interpolation with its mapping type 
		/// 
		/// This operator is used when declaring an interpolation as
		/// Interpoler<Polynomial,Interpoler<Linear,double,double>,double>
		/// 
		/// @return The result of the multiplication
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler<Linear,VAL,T> operator*(const T &ratio) const;
	};

	// Nearest
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Partial specialization of Interpoler<Type,VAL,T> class for
	///			   <a href="http://en.wikipedia.org/wiki/Nearest-neighbor_interpolation">Nearest interpolations</a>
	///
	/// @ingroup System
	/// 
	/// You can use this class to define a <a href="http://en.wikipedia.org/wiki/Nearest-neighbor_interpolation">nearest interpolation</a>
	/// that maps T type to VAL type.
	/// 
	/// Please note that the followings are required in order not to generate a compilation error:
	/// 	@li A multiplication (*) operator that accepts a /const VAL &/ and a /const   T &/
	/// 	@li A addition       (+) operator that accepts a /const VAL &/ and a /const VAL &/
	/// 	@li A substraction   (-) operator that accepts a /const   T &/ and a /const   T &/
	/// 	@li An overload of std::abs which takes a T and its return type must be comperable with <
	/// 	@li VAL and T must be copyable
	/// 
	/// vector2, vector3 and all floating-point types satisfy the requirements of VAL<br/>
	/// Also every floating-point types satisfy the requirements of T
	/// 
	/// @see Interpoler<Polynomial,VAL,T>
	/// @see Interpoler<Linear,VAL,T>
	/// 
	/////////////////////////////////////////////////////////////
	template<class VAL,class T>
	class Interpoler<Nearest,VAL,T> : public Interpoler_Base<VAL,T>
	{
	public:
		typedef typename Interpoler_Base<VAL,T>::val_map val_map;
		typedef Interpoler<Nearest,VAL,T> &reference;
		typedef const Interpoler<Nearest,VAL,T> &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler();

		/////////////////////////////////////////////////////////////
		/// @brief Copy-convert constructor
		/// 
		/// The same data can be used in any type of interpolation therefore
		/// what this this constructor does is copy m_data
		/// 
		/// @param copy The Interpolater to be copied
		/// 
		/////////////////////////////////////////////////////////////
		template<InterpolerType Type2>
		Interpoler(const Interpoler<Type2,VAL,T> &copy);

		/////////////////////////////////////////////////////////////
		/// @brief The implementation of fm::Interpoler_Base<VAL,T>::updated
		/// 
		/////////////////////////////////////////////////////////////
		void updated();

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the value of the interpolation at @a t
		///
		/// This is the implementation of fm::Interpoler_Base<VAL,T>::calcVal
		///
		/// @param t The point where the value to be calculated 
		/// 
		/// @return The calculated value
		/// 
		/////////////////////////////////////////////////////////////
		VAL calcVal(const T &t) const;

		/////////////////////////////////////////////////////////////
		/// @brief rw access to the mapping data
		///
		/// @param index The offset in the data
		/// 
		/// @return reference to the mapping pair
		/// 
		/////////////////////////////////////////////////////////////
		val_map &operator[](unsigned int index);

		/////////////////////////////////////////////////////////////
		/// @brief read-only access to the mapping data
		///
		/// @param index The offset in the data
		/// 
		/// @return const reference to the mapping pair
		/// 
		/////////////////////////////////////////////////////////////
		const val_map &operator[](unsigned int index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the sum of two polynomial interpolations
		/// 
		/// This operator is used when declaring an interpolation such as
		/// Interpoler<Polynomial,Interpoler<Nearest,double,double>,double>
		/// 
		/// @return The sum
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler<Nearest,VAL,T> operator+(const_reference other) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Multiply a Polynomial interpolation with its mapping type 
		/// 
		/// This operator is used when declaring an interpolation such as
		/// Interpoler<Polynomial,Interpoler<Nearest,double,double>,double>
		/// 
		/// @return The result of the multiplication
		/// 
		/////////////////////////////////////////////////////////////
		Interpoler<Nearest,VAL,T> operator*(const T &ratio) const;
	};
}

#endif

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Interpoler.inl>
#endif
