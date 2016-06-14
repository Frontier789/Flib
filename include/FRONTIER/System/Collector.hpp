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
#ifndef FRONTIER_COLLECTOR_HPP_INCLUDED
#define FRONTIER_COLLECTOR_HPP_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>
#define FRONTIER_COLLECTOR
#include <deque>

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief A simple class used to collect elements in a visually appealing way
	/// 
	/////////////////////////////////////////////////////////////
	template<class T,class Container = std::deque<T> >
	class Collector
	{
		mutable Container m_container; ///< The underlying container
	public:
		typedef Collector<T,Container> &reference;
		typedef const Collector<T,Container> &const_reference;
			
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		Collector();

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/////////////////////////////////////////////////////////////
		template<class Container2>
		Collector(const Collector<T,Container2> &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Access a given element in the collection
		/// 
		/// Uses Container::operator[]
		/// 
		/// @return The element
		/// 
		/////////////////////////////////////////////////////////////
		T &operator[](const fm::Size &i);

		/////////////////////////////////////////////////////////////
		/// @brief Access a given element in the collection
		/// 
		/// Uses Container::operator[]
		/// 
		/// @return The element
		/// 
		/////////////////////////////////////////////////////////////
		const T &operator[](const fm::Size &i) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the contained data to only one element
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T2>
		const Collector<T,Container> &operator=(const T2 &element) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of elements stored
		/// 
		/// Uses Container::size()
		/// 
		/// @return The number of elements stored
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size size() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the begining of the underlying container
		/// 
		/// Uses Container::begin()
		/// 
		/// @return The begining
		/// 
		/////////////////////////////////////////////////////////////
		typename Container::iterator begin();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the begining of the underlying container
		/// 
		/// Uses Container::begin()
		/// 
		/// @return The begining
		/// 
		/////////////////////////////////////////////////////////////
		typename Container::const_iterator begin() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the ending of the underlying container
		/// 
		/// Uses Container::end()
		/// 
		/// @return The ending
		/// 
		/////////////////////////////////////////////////////////////
		typename Container::iterator end();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the ending of the underlying container
		/// 
		/// Uses Container::end()
		/// 
		/// @return The ending
		/// 
		/////////////////////////////////////////////////////////////
		typename Container::const_iterator end() const;

		/////////////////////////////////////////////////////////////
		/// @brief Append the back of the container
		/// 
		/// Uses Container::push_back()
		/// 
		/// @param data The data to be appended to the container
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		const Collector<T,Container> &push_back(const T &data) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Append the front of the container
		///
		/// Uses Container::push_front()
		/// 
		/// @param data The data to be appended to the container
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		const Collector<T,Container> &push_front(const T &data) const;
	};

	/////////////////////////////////////////////////////////////
	/// @brief Append the back of the container
	/// 
	/// Uses Container::push_back()
	/// 
	/// @param data The data to be appended to the container
	/// @param collector The collector to be appended
	/// 
	/// @return Reference to itself
	/// 
	/////////////////////////////////////////////////////////////
	template<class T,class Container,class T2>
	const Collector<T,Container> &operator,(const Collector<T,Container> &collector,const T2 &data);

	/////////////////////////////////////////////////////////////
	/// @brief Append the front of the container
	///
	/// Uses Container::push_front()
	/// 
	/// @param data The data to be appended to the container
	/// @param collector The collector to be appended
	/// 
	/// @return Reference to itself
	/// 
	/////////////////////////////////////////////////////////////
	template<class T,class Container,class T2>
	const Collector<T,Container> &operator,(const T2 &data,const Collector<T,Container> &collector);

	/////////////////////////////////////////////////////////////
	/// @brief Append the back of the container
	/// 
	/// Uses Container::push_back()
	/// 
	/// @param data The data to be appended to the container
	/// @param collector The collector to be appended
	/// 
	/// @return Reference to itself
	/// 
	/////////////////////////////////////////////////////////////
	template<class T,class Container,class T2>
	const Collector<T,Container> &operator<<(const Collector<T,Container> &collector,const T2 &data);

	/////////////////////////////////////////////////////////////
	/// @brief Append the front of the container
	///
	/// Uses Container::push_front()
	/// 
	/// @param data The data to be appended to the container
	/// @param collector The collector to be appended
	/// 
	/// @return Reference to itself
	/// 
	/////////////////////////////////////////////////////////////
	template<class T,class Container,class T2>
	const Collector<T,Container> &operator>>(const T2 &data,const Collector<T,Container> &collector);
}


#endif // FRONTIER_COLLECTOR_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Collector.inl>
#endif

////////////////////////////////////////////////////////////
/// @class fm::Collector
/// @ingroup System
///
/// Usage example:
/// @code
/// class MyClass
/// {
/// 	int t[16];
/// public:
/// 	MyClass(const &fm::Collector<int> &c)
/// 	{
/// 		fm::Size n = c.size();
/// 		C(n<16 ? n : 16)
/// 			t[i] = c[i];
/// 	}
/// };
/// 
/// // note the double (())
/// MyClass mc((fm::Collector<int>()=
/// 		    1,2,3,4,
/// 		    5,4,5,4,
/// 		    2,5,8,2,
/// 		    1,2,1,2));
/// // mc.t is now {1,2,3,4,5,4,5,4,2,5,8,2,1,2,1,2}
/// @endcode
/// 
////////////////////////////////////////////////////////////