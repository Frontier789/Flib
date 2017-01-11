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
#ifndef FRONTIER_VERTEXSTATE_HPP_INCLUDED
#define FRONTIER_VERTEXSTATE_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/System/util/dont_include_inl_end>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_VERTEXSTATE


namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Stores vertex state data on gpu
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API VertexState : public fg::GlObject, public fm::NonCopyable
	{
		void init(); ///< internal initializer function
	public:

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		VertexState();

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~VertexState();

		/////////////////////////////////////////////////////////////
		/// @brief Bind the vertex state object for usage
		///
		/////////////////////////////////////////////////////////////
		void bind();

		/////////////////////////////////////////////////////////////
		/// @brief Bind a vertex state object for usage
		///
		/// @param vao The vertex state object to be bound
		///
		/////////////////////////////////////////////////////////////
		static void bind(VertexState *vao);

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
