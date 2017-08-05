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
#ifndef FRONTIER_FLOATATTRIBUPDATER_HPP_INCLUDED
#define FRONTIER_FLOATATTRIBUPDATER_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/util/API.h>
#include <vector>

namespace fg
{
	class Attribute;
}

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Class responsible for updating a float attribute
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API FloatAttributeUpdater
	{
	public:
		std::vector<float> m_data; ///< The data
		fg::Attribute &m_attrib;   ///< Reference to the attribute needed to be updated
		fm::Size m_firstUpdated;   ///< The index of the first item updated
		fm::Size m_lastUpdated;    ///< The index of the last item updated
		fm::Size m_floatPerVec;    ///< Number of floats in a vec
		fm::Size m_vecsPerItem;    ///< Number of vecs per item
		fm::Size m_capacity;       ///< The number of allocated items
		fm::Size m_uploads;        ///< The number of uploads done
		bool m_lastTimeMap;        ///< Stores whether last time map was used
		float *m_mappedPtr;        ///< The pointer to the mapped data
		
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of floats per item
		/// 
		/// @return The number of floats per item
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getFloatsPerItem() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the number of bytes per item
		/// 
		/// @return The number of bytes per item
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getBytesPerItem() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param attrib The attribute to keep updated
		/// @param floatPerVec The number of floats in a vec
		/// @param vecsPerItem The number of vecs per item
		/// 
		/////////////////////////////////////////////////////////////
		FloatAttributeUpdater(fg::Attribute &attrib,fm::Size floatPerVec,fm::Size vecsPerItem = 1);
		
		/////////////////////////////////////////////////////////////
		/// @brief Register an update
		/// 
		/// @param index The index of the updated item
		/// @param value The new value of the updated item
		/// 
		/////////////////////////////////////////////////////////////
		void update(fm::Size index,const float *value);
		
		/////////////////////////////////////////////////////////////
		/// @brief Register an update from an index
		/// 
		/// @param index The index of the updated item
		/// @param fromIndex The index to copy from
		/// 
		/////////////////////////////////////////////////////////////
		void updateFrom(fm::Size index,fm::Size fromIndex);
		
		/////////////////////////////////////////////////////////////
		/// @brief Fetch value at index
		/// 
		/// @param index The index of th item to fetch
		/// @param The value returned
		/// 
		/////////////////////////////////////////////////////////////
		void fetch(fm::Size index,float *ret) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the allocated capacity
		/// 
		/// @param capacity The new capacity
		/// 
		/////////////////////////////////////////////////////////////
		void setCapacity(fm::Size capacity);
		
		/////////////////////////////////////////////////////////////
		/// @brief Push a new value by increasing the allocated data size
		/// 
		/// @param value The value new item
		/// 
		/////////////////////////////////////////////////////////////
		void push(const float *value);
		
		/////////////////////////////////////////////////////////////
		/// @brief Pop the last item
		/// 
		/////////////////////////////////////////////////////////////
		void pop();
		
		/////////////////////////////////////////////////////////////
		/// @brief Unmap all mapped data
		/// 
		/////////////////////////////////////////////////////////////
		void unMap();
		
		/////////////////////////////////////////////////////////////
		/// @brief Prepare the attribute for being used
		/// 
		/////////////////////////////////////////////////////////////
		void prepareDraw();
	};
}

#endif // FRONTIER_FLOATATTRIBUPDATER_HPP_INCLUDED

