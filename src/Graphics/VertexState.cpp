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
#include <FRONTIER/Graphics/VertexState.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/GL/GLBindKeeper.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	fm::Result VertexState::create()
	{
		if (!getGlId())
			return glCheck(glGenVertexArrays(1,&getGlId()));
		
		return fm::Result();
	}
	
	/////////////////////////////////////////////////////////////
	VertexState::VertexState(VertexState &&mv)
	{
		mv.swap(*this);
	}
	
	/////////////////////////////////////////////////////////////
	VertexState &VertexState::swap(VertexState &state)
	{
		std::swap(getGlId(),state.getGlId());
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result VertexState::clearData()
	{
		fm::Result res;
		
		if (getGlId())
		{
			res += glCheck(glDeleteVertexArrays(1,&getGlId()));
			res += create();
		}
		
		return res;
	}

	////////////////////////////////////////////////////////////
	VertexState::~VertexState()
	{
		if (getGlId())
			glCheck(glDeleteVertexArrays(1,&getGlId()));
	}

	////////////////////////////////////////////////////////////
	fm::Result VertexState::bind() const
	{
		return glCheck(glBindVertexArray(getGlId()));
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result VertexState::setAttribute(fm::Size attrId,const Attribute &attr)
	{
		GLBindKeeper guard(glBindVertexArray,GL_VERTEX_ARRAY_BINDING,getGlId());
		
		fm::Result res = fg::Buffer::bind(attr.buf,fg::ArrayBuffer);
		
		if (attr.buf)
		{
			res += glCheck(glEnableVertexAttribArray(attrId));
			res += glCheck(glVertexAttribPointer(attrId, attr.compCount, attr.compType, GL_FALSE, attr.stride, (void*)attr.offset));
		}
		else 
			res += glCheck(glDisableVertexAttribArray(attrId));
		
		return res;
	}

	////////////////////////////////////////////////////////////
	fm::Result VertexState::bind(fm::Ref<const VertexState> vao)
	{
		if (vao)
			return vao->bind();
		else
			return glCheck(glBindVertexArray(0));
	}

	////////////////////////////////////////////////////////////
	bool VertexState::isAvailable()
	{
		return ::priv::so_loader.getProcAddr("glGenVertexArrays") != nullptr;
	}
}
