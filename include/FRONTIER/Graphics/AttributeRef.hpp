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
#ifndef FRONTIER_ATTRIBUTE_REF_HPP_INCLUDED
#define FRONTIER_ATTRIBUTE_REF_HPP_INCLUDED

#include <FRONTIER/Graphics/AssocPoint.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_ATTRIBUTE_REF

namespace fg
{
	class Buffer;
	class DrawData;
	class Attribute;
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to reference a not necesserely existing attribute in a drawdata
	/// 
	/// If the attribute does not exist it is created upon assignment
	///
	/// @ingroup Graphics
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API AttributeRef
	{
		DrawData &m_drawData; ///< The bound drawData 
		AssocPoint m_assoc;   ///< The referenced assoc point
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Construct the attribute from its data
		/// 
		/// @param drawData The bound drawdata
		/// @param assoc The referenced assoc point
		/// 
		/////////////////////////////////////////////////////////////
		AttributeRef(DrawData &drawData,AssocPoint assoc = Assoc::Unused);

		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment operator
		/// 
		/// @param attr The attribute to be assigned to the referenced attribute
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		AttributeRef &operator=(const Attribute &attr) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		/// 
		/// @param attr The attribute to be moved to the referenced attribute
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		AttributeRef &operator=(Attribute &&attr);

		/////////////////////////////////////////////////////////////
		/// @brief Swap the referenced attribute with another
		/// 
		/// @param attr The attribute to be swapped with the referenced attribute
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		AttributeRef &swap(Attribute &attr);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the referenced attribute
		/// 
		/// @param data The data to assign
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size N>
		inline AttributeRef &operator=(const T (&data)[N]);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the referenced attribute
		/// 
		/// @param data The data to assign
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size N>
		inline AttributeRef &set(const T (&data)[N]);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the referenced attribute
		/// 
		/// @param pointer Pointer to the data to assign
		/// @param N The number of components to assign
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		inline AttributeRef &set(const T *pointer,fm::Size N);

		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the referenced attribute
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
		AttributeRef &set(fm::Size components,
						  fm::Size stride,
						  fm::Size count,
						  fm::Size componentType,
						  const void *pointer,
						  fm::Size bytesToCopy);
	
		/////////////////////////////////////////////////////////////
		/// @brief Access the pointed attribute
		/// 
		/// @return Reference to the attribute
		/// 
		/////////////////////////////////////////////////////////////
		Attribute &getData();
		
		/////////////////////////////////////////////////////////////
		/// @brief Assign data to the referenced attribute
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
		AttributeRef &set(fm::Size components,
						  fm::Size stride,
						  fm::Size count,
						  fm::Size componentType,
						  fg::Buffer *buffer = nullptr,
						  bool ownBuffer = true);
	};
}

#endif // FRONTIER_ATTRIBUTE_REF_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/AttributeRef.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
