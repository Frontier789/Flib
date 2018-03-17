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
#ifndef FRONTIER_VERTEXSTATE_HPP_INCLUDED
#define FRONTIER_VERTEXSTATE_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Ref.hpp>

#define FRONTIER_VERTEXSTATE


namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Stores vertex state data on gpu
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API VertexState : public fg::GlObject, public fm::NonCopyable
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		VertexState() = default;
		
		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param mv The state to move
		///
		/////////////////////////////////////////////////////////////
		VertexState(VertexState &&mv);
		
		/////////////////////////////////////////////////////////////
		/// @brief Swap two vertex state objects
		///
		/// @param state The state to swap with
		///
		/////////////////////////////////////////////////////////////
		VertexState &swap(VertexState &state);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~VertexState();

		/////////////////////////////////////////////////////////////
		/// @brief Create the vertex state object
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		fm::Result create();

		/////////////////////////////////////////////////////////////
		/// @brief Reset the vertex state object
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		fm::Result clearData();

		/////////////////////////////////////////////////////////////
		/// @brief Bind the vertex state object for usage
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
		/// @brief Bind a vertex state object for usage
		///
		/// @param vao The vertex state object to be bound
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		static fm::Result bind(fm::Ref<const VertexState> vao);

		/////////////////////////////////////////////////////////////
		/// @brief Check if vertex state objects are available
		///
		/// @return True iff vertex state objects are available
		///
		/////////////////////////////////////////////////////////////
		static bool isAvailable();
	};

	typedef VertexState VertexArray;
}

#endif // FRONTIER_VERTEXSTATE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/VertexState.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
