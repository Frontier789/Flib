////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)		   ///
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
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_ATTRIBUTE
#include <type_traits>

namespace fg
{
	class Buffer;
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to store a vertex property of a drawing call such as position, color or normal
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Attribute
	{
	public:
		fm::Size componentType; ///< The OpenGL id of the type of components
		fm::Size components;    ///< The number of components
		fg::Buffer *buf; ///< The buffer that holds the data 
		fm::Size stride; ///< The offset between two vertex data
		fm::Size count;  ///< The number of vertices
		bool ownBuffer;  ///< Stores whether the buffer was created by this class

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param components The number of components
		/// @param stride The offset between two vertex data
		/// @param count The number of vertices
		/// @param componentType The OpenGL id of the type of components
		/// @param buffer The buffer that holds the data 
		/// @param ownBuffer Indicates whether the buffer is to be created by this class
		/// 
		/////////////////////////////////////////////////////////////
		Attribute(fm::Size components = 0,
				  fm::Size stride = 0,
				  fm::Size count = 0,
				  fm::Size componentType = 0,
				  fg::Buffer *buffer = nullptr,
				  bool ownBuffer = true);
		
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
		inline typename std::enable_if<fg::Is_GLDataType<T>::value,Attribute>::type &operator=(const T (&data)[N]);

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
		inline typename std::enable_if<!fg::Is_GLDataType<T>::value,Attribute>::type &operator=(const T (&data)[N]);

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
		inline typename std::enable_if<fg::Is_GLDataType<T>::value,Attribute>::type &set(const T (&data)[N]);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the attribute
		/// 
		/// If T is a GL data type componentType is assumed to be T,
		/// components to be 1
		/// If T is not a GL data type componentType is fetched from T::component_type
		/// and components from T::components
		/// 
		/// @param pointer Pointer to the data to assign
		/// @param N The number of Ts to requested to be copied
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		inline typename std::enable_if<fg::Is_GLDataType<T>::value,Attribute>::type &set(const T *pointer,fm::Size N);

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
		inline typename std::enable_if<!fg::Is_GLDataType<T>::value,Attribute>::type &set(const T (&data)[N]);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the attribute
		/// 
		/// If T is a GL data type componentType is assumed to be T,
		/// components to be 1
		/// If T is not a GL data type componentType is fetched from T::component_type
		/// and components from T::components
		/// 
		/// @param pointer Pointer to the data to assign
		/// @param N The number of Ts to requested to be copied
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		inline typename std::enable_if<!fg::Is_GLDataType<T>::value,Attribute>::type &set(const T *pointer,fm::Size N);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the attribute
		/// 
		/// @param components The number of components
		/// @param stride The offset between two vertex data
		/// @param count The number of vertices
		/// @param componentType The OpenGL id of the type of components
		/// @param pointer Pointer to the data to be copied
		/// @param bytesToCopy Number of bytes to be copied
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Attribute &set(fm::Size components,
					   fm::Size stride,
					   fm::Size count,
					   fm::Size componentType,
					   const void *pointer,
					   fm::Size bytesToCopy);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the attribute
		/// 
		/// @param components The number of components
		/// @param stride The offset between two vertex data
		/// @param count The number of vertices
		/// @param componentType The OpenGL id of the type of components
		/// @param buffer The buffer that holds the data 
		/// @param ownBuffer Indicates whether the buffer is to be created by this class
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Attribute &set(fm::Size components,
					   fm::Size stride,
					   fm::Size count,
					   fm::Size componentType,
					   fg::Buffer *buffer = nullptr,
					   bool ownBuffer = true);
	};
}

#endif // FRONTIER_ATTRIBUTE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Attribute.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
