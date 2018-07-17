////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)		   ///
///																	   ///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			   ///
/// condition that this disclaimer is not modified/removed.			   ///
/// You may not misclaim the origin of this software.				   ///
///																	   ///
/// If you use this software in your program/project a				   ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							   ///
///																	   ///
/// You should have received a copy of GNU GPL with this software	   ///
///																	   ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_ATTRIBUTE_HPP_INCLUDED
#define FRONTIER_ATTRIBUTE_HPP_INCLUDED

#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_ATTRIBUTE
#include <initializer_list>
#include <type_traits>

namespace fg
{
	class Buffer;
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to store a vertex property of a drawing call such as position, color or normal
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Attribute
	{
	public:
		fm::Size instancesPerUpdate; ///< Number of instances between updates
		fm::Size compCount; ///< The number of components 0 means fetch from buffer props
		fm::Size compType;  ///< The OpenGL id of the type of components 0 means fetch from buffer props
		fg::Buffer *buf; ///< The buffer that holds the data 
		fm::Size stride; ///< The offset between two vertex data
		fm::Size offset; ///< The offset to the first item
		fm::Size count;  ///< The number of vertices 0 means fetch from buffer props
		bool ownBuffer;  ///< Stores whether the buffer was created by this class
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		Attribute();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct an empty attribute
		/// 
		/////////////////////////////////////////////////////////////
		Attribute(std::nullptr_t);

		/////////////////////////////////////////////////////////////
		/// @brief Construct the Attribute from a Buffer
		/// 
		/// @param buf The buffer to use
		/// @param instancesPerUpdate Number of instances between updates
		/// 
		/////////////////////////////////////////////////////////////
		Attribute(fg::Buffer &buf,fm::Size instancesPerUpdate = 0);
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~Attribute();

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param attr The attribute to be copied
		/// 
		/////////////////////////////////////////////////////////////
		Attribute(const Attribute &attr) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param attr The attribute to be moved
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Attribute(Attribute &&attr);

		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment operator
		/// 
		/// @param attr The attribute to be copied
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Attribute &operator=(const Attribute &attr) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		/// 
		/// @param attr The attribute to be moved
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Attribute &operator=(Attribute &&attr);

		/////////////////////////////////////////////////////////////
		/// @brief Swap two attributes
		/// 
		/// @param attr The attribute to swap with
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Attribute &swap(Attribute &attr);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the attribute through a buffer
		/// 
		/// @param data The data to assign
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Attribute &operator=(fg::Buffer &buf);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the attribute
		/// 
		/// If T is a GL data type componentType is assumed to be T,
		/// components to be 1
		/// If T is not a GL data type componentType is fetched from T::component_type
		/// and components from T::components
		/// 
		/// @param data The data to assign
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size N>
		Attribute &operator=(const T (&data)[N]);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the attribute
		/// 
		/// If T is a GL data type componentType is assumed to be T,
		/// components to be 1
		/// If T is not a GL data type componentType is fetched from T::component_type
		/// and components from T::components
		/// 
		/// @param data The data to assign
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		Attribute &operator=(std::initializer_list<T> data);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the attribute
		/// 
		/// If T is a GL data type componentType is assumed to be T,
		/// components to be 1
		/// If T is not a GL data type componentType is fetched from T::component_type
		/// and components from T::components
		/// 
		/// @param pointer Pointer to the data to assign
		/// @param N The number of Ts to be requested to be copied
		/// @param instancesPerUpdate Number of instances between updates
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		Attribute &set(const T *pointer,fm::Size N,fm::Size instancesPerUpdate = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Assign buffer to the attribute
		/// 
		/// If T is a GL data type componentType is assumed to be T,
		/// components to be 1
		/// If T is not a GL data type componentType is fetched from T::component_type
		/// and components from T::components
		/// 
		/// @param buf The buffer to use
		/// @param stride The offset between the start of two consecutive items in bytes
		/// @param offset Offset to the first item in buffer in bytes
		/// @param instancesPerUpdate Number of instances between updates
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		Attribute &set(fg::Buffer &buf,fm::Size stride = 0,fm::Size offset = 0,fm::Size instancesPerUpdate = 0);
	};
}

#endif // FRONTIER_ATTRIBUTE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Attribute.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
