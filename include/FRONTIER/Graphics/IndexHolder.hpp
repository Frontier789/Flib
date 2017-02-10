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
#ifndef FRONTIER_INDEXHOLDER_HPP_INCLUDED
#define FRONTIER_INDEXHOLDER_HPP_INCLUDED

#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_INDEXHOLDER

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Simple wrapper class used to hold indices
	///
	/// Automatically uses 16 bits if the indices fit in [0,2^16-1] range
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API IndexHolder
	{
		void *m_ptr;      ///< The pointer to the allocated memory
		bool m_use16bits; ///< Stores whether 16 or 32 bit integers to be used
		fm::Size m_size;  ///< The number of indices in the array
		
	public:
		typedef IndexHolder &reference;
		typedef const IndexHolder &const_reference;
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the array with given size
		///
		/// @param N The number of elements to alocate
		/// @param use16bits True iff 16 bit integers is to be used
		/// 
		/////////////////////////////////////////////////////////////
		explicit IndexHolder(fm::Size N,bool use16bits = true);

		/////////////////////////////////////////////////////////////
		/// @brief Create the array from a 32 bit array
		///
		/// @param arrayToCopy The array to be copied
		/// @param indexCount The number of elements to be copied
		/// 
		/////////////////////////////////////////////////////////////
		IndexHolder(const fm::Uint32 *arrayToCopy,fm::Size indexCount);

		/////////////////////////////////////////////////////////////
		/// @brief Create the array from another array
		///
		/// @param copy The array to be copied
		/// 
		/////////////////////////////////////////////////////////////
		IndexHolder(const IndexHolder &copy) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param move The array to be moved
		/// 
		/////////////////////////////////////////////////////////////
		IndexHolder(IndexHolder &&move);

		/////////////////////////////////////////////////////////////
		/// @brief Default detructor
		/// 
		/////////////////////////////////////////////////////////////
		~IndexHolder();

		/////////////////////////////////////////////////////////////
		/// @brief Change an element of the array
		///
		/// @param index The index of the element
		/// @param val The new value (gets converted to Uint16 if needed)
		/// 
		/////////////////////////////////////////////////////////////
		void set(fm::Size index,fm::Uint32 val);

		/////////////////////////////////////////////////////////////
		/// @brief Get an element of the array
		///
		/// @param index The index of the element
		///
		/// @return The value of the element
		/// 
		/////////////////////////////////////////////////////////////
		fm::Uint32 get(fm::Size index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the number of elements in the array
		///
		/// @return The number of elements
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the array uses 16 bit integers
		///
		/// @return True iff the array uses 16 bit integers
		/// 
		/////////////////////////////////////////////////////////////
		bool doUse16bits() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the pointer of the array
		/// 
		/// May be null
		///
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		const void *getPtr() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the pointer of the array
		/// 
		/// May be null
		///
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		void *getPtr();
		
		/////////////////////////////////////////////////////////////
		/// @brief Swap the content of the two objects
		///
		/// swaps in constant time
		///
		/// @param arr The object to swap with
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference swap(IndexHolder &arr);
	};
}

#endif // FRONTIER_INDEXHOLDER_HPP_INCLUDED
