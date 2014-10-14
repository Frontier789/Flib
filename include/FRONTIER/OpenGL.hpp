////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_OPENGL_HPP_INCLUDED
#define FRONTIER_OPENGL_HPP_INCLUDED
#include <FRONTIER/System/Log.hpp>
#define FRONTIER_OPENGL

namespace fg
{
	extern fm::Log OpenGL_log;
}

//////////////////////////////////
/// It would cause name conflict so we
/// cannot let neither of gl.h, glew.h, glext.h, gltypes.h and glATI.h
/// headers to be included before this header
//////////////////////////////////

#if defined(__gl_h_) || defined(__GL_H__)
	#error Attempt to include OpenGL.hpp after including gl.h
#endif

#if defined(__glew_h__) || defined(__GLEW_H__)
	#error Attempt to include OpenGL.hpp after including glew.h
#endif

#if defined(__glext_h_) || defined(__GLEXT_H_)
	#error Attempt to include OpenGL.hpp after including glext.h
#endif

#if defined(__gltypes_h_)
	#error Attempt to include OpenGL.hpp after gltypes.h
#endif

#if defined(__gl_ATI_h_)
	#error Attempt to include OpenGL.hpp after including glATI.h
#endif


//////////////////////////////////
/// Prevent name conflict if above headers
/// included after OpenGL.hpp
//////////////////////////////////

#define __glew_h__
#define __GLEW_H__
#define __gl_h_
#define __GL_H__
#define __glext_h_
#define __GLEXT_H_
#define __gltypes_h_
#define __gl_ATI_h_


//////////////////////////////////
/// Advertise defined extensions
//////////////////////////////////

#ifndef GL_ARB_vertex_buffer_object
	#define GL_ARB_vertex_buffer_object 1
#endif

#ifndef GL_ARB_shader_objects
	#define GL_ARB_shader_objects 1
#endif

#ifndef GL_EXT_framebuffer_object
	#define GL_EXT_framebuffer_object 1
#endif

#ifndef GL_ARB_vertex_shader
	#define GL_ARB_vertex_shader 1
#endif

#ifndef GL_ARB_fragment_shader
	#define GL_ARB_fragment_shader 1
#endif

#ifndef GL_ARB_fragment_program
	#define GL_ARB_fragment_program 1
#endif

#ifndef GL_ARB_vertex_program
	#define GL_ARB_vertex_program 1
#endif


#include <FRONTIER/GL/GL_VALUES.h>
#include <FRONTIER/GL/GL_TYPES.hpp>
#include <FRONTIER/GL/GL_FUNCTIONS.h>



#endif
