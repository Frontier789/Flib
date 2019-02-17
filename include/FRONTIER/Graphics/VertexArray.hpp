////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_VERTEXARRAY_HPP_INCLUDED
#define FRONTIER_VERTEXARRAY_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>
#include <FRONTIER/Graphics/AssocPoint.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Ref.hpp>
#include <vector>

#define FRONTIER_VERTEXARRAY


namespace fg
{
	class DrawData;
	
	/////////////////////////////////////////////////////////////
	/// @brief Stores vertex array data on gpu
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API VertexArray : public fg::GlObject, public fm::NonCopyable
	{
		std::vector<bool> m_attrsEnabled; ///< Stores whther the generic vertex atttributes are enabled
		fm::Result setAttributeBound(fm::Size attrId,const Attribute &attr); ///< Internal helper function
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		VertexArray() = default;
		
		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param mv The state to move
		///
		/////////////////////////////////////////////////////////////
		VertexArray(VertexArray &&mv);
		
		/////////////////////////////////////////////////////////////
		/// @brief Swap two vertex array objects
		///
		/// @param state The state to swap with
		///
		/////////////////////////////////////////////////////////////
		VertexArray &swap(VertexArray &state);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~VertexArray();

		/////////////////////////////////////////////////////////////
		/// @brief Destroy the shader object
		///
		/// Releases all OGL data
		///
		/////////////////////////////////////////////////////////////
        fm::Result destroy();

		/////////////////////////////////////////////////////////////
		/// @brief Create the vertex array object
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		fm::Result create();

		/////////////////////////////////////////////////////////////
		/// @brief Reset the vertex array object
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		fm::Result clearData();

		/////////////////////////////////////////////////////////////
		/// @brief Bind the vertex array object for usage
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		fm::Result bind() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set an attribute of the vao
		///
		/// @param attrId The id of the Attribute
		/// @param buf The buffer to set
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		fm::Result setAttribute(fm::Size attrId,const Attribute &attr);

		/////////////////////////////////////////////////////////////
		/// @brief Set an interleaved attribute of the vao
		/// 
		/// @param attrId The id of the Attribute
		/// @param buf The buffer to set
		/// @param stride The offset between the start of two consecutive items in bytes
		/// @param offset The offset to the beginning of the data in bytes
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		template<class T>
		fm::Result setAttribute(fm::Size attrId,fm::Ref<fg::Buffer> buf,fm::Size stride = 0,fm::Size offset = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Bind a vertex array object for usage
		///
		/// @param vao The vertex array object to be bound
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		static fm::Result bind(fm::Ref<const VertexArray> vao);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the attributes of a vertex array object from a drawdata
		///
		/// @param drawData The draw data to load the attributes from
		/// @param assocToAttrId A function that converts association points to attribute ids
		/// 
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		fm::Result setAttributes(const DrawData &drawData,fm::Delegate<fm::Size,fg::AssocPoint> assocToAttrId);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set one attribute of the vertex array to a given constant 
		///
		/// @param attrId The id of the attribute to set
		/// @param t The data to set
		/// 
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		template<class T,class = typename std::enable_if<Is_GLDataType<T>::value>::type>
		fm::Result setAttribute(fm::Size attrId,const T &t);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set one attribute of the vertex array to a given value 
		///
		/// @param attrId The id of the attribute to set
		/// @param compCount The number of components in the data's type
		/// @param compType Encodes the data's cmponents' type
		/// @param ptr Pointer to the data to set
		/// 
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		fm::Result setAttribute(fm::Size attrId,fm::Size compCount,fm::Size compType,const void *ptr);

		/////////////////////////////////////////////////////////////
		/// @brief Check if vertex array objects are available
		///
		/// @return True iff vertex array objects are available
		///
		/////////////////////////////////////////////////////////////
		static bool isAvailable();
	};

	typedef VertexArray VertexState;
}

#endif // FRONTIER_VERTEXARRAY_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/VertexArray.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
