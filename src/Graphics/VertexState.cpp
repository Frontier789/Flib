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
#include <FRONTIER/Graphics/VertexState.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
	void VertexState::init()
	{
		if (!getGlId() || !glIsVertexArray(getGlId()))
			glCheck(glGenVertexArrays(1,&getGlId()));
	}

	////////////////////////////////////////////////////////////
	VertexState::VertexState()
	{

	}

	////////////////////////////////////////////////////////////
	VertexState::~VertexState()
	{
		if (getGlId() && glIsVertexArray(getGlId()))
			glCheck(glDeleteVertexArrays(1,&getGlId()));
	}

	////////////////////////////////////////////////////////////
	void VertexState::bind()
	{
		if (!getGlId())
			init();

		glCheck(glBindVertexArray(getGlId()));
	}

	////////////////////////////////////////////////////////////
	void VertexState::bind(VertexState *vao)
	{
		if (vao)
			vao->bind();
		else
			glCheck(glBindVertexArray(0));
	}

	////////////////////////////////////////////////////////////
	bool VertexState::isAvailable()
	{
		return ::priv::so_loader.getProcAddr("glGenVertexArrays") != fm::nullPtr;
	}
}
