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
#include <FRONTIER/GL/GLBindKeeper.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	GLBindKeeper::GLBindKeeper(void (API_ENTRY *binder)(GLenum,GLuint),GLenum bindPoint,GLenum binding,GLuint tmpId) : 
		m_enum(bindPoint),
		m_binderA(binder),
		m_binderB(nullptr)
	{
		GLint id;
		glGetIntegerv(binding,&id);
		m_prevId = id;
		
		if (m_prevId != tmpId)
			binder(bindPoint,tmpId);
		else
			m_binderA = nullptr;
	}
	/////////////////////////////////////////////////////////////
	GLBindKeeper::GLBindKeeper(void (API_ENTRY *binder)(GLuint),GLenum binding,GLuint tmpId) : 
		m_binderA(nullptr),
		m_binderB(binder)
	{
		GLint id;
		glGetIntegerv(binding,&id);
		m_prevId = id;
		
		if (m_prevId != tmpId)
			binder(tmpId);
		else
			m_binderB = nullptr;
	}
	
	/////////////////////////////////////////////////////////////
	GLBindKeeper::~GLBindKeeper()
	{
		if (!m_binderA && !m_binderB) return;
		
		if (m_binderA)
			m_binderA(m_enum,m_prevId);
		else 
			m_binderB(m_prevId);
	}
}
