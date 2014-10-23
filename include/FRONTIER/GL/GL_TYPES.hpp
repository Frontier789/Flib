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
#ifndef FRONTIER_GL_TYPES_HPP_INCLUDED
#define FRONTIER_GL_TYPES_HPP_INCLUDED
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>


typedef void        GLvoid;

typedef char        GLchar;
typedef GLchar      GLcharARB;
typedef fm::Int8    GLbyte;
typedef fm::Uint8   GLboolean;
typedef fm::Uint8   GLubyte;

typedef fm::Int16   GLshort;
typedef fm::Uint16  GLushort;

typedef fm::Int32   GLsizei;
typedef fm::Int32   GLint;
typedef fm::Uint32  GLuint;
typedef fm::Uint32  GLenum;
typedef fm::Uint32  GLbitfield;
                    
typedef fm::Float32 GLfloat;
typedef fm::Float32 GLclampf;
typedef fm::Float64 GLdouble;
typedef fm::Float64 GLclampd;

#ifdef __APPLE__
	typedef void        *GLhandleARB;
#else
	typedef fm::Uint32   GLhandleARB;
#endif

typedef fm::Uint16  GLhalf;
typedef GLhalf      GLhalfARB;
typedef GLint       GLfixed;
typedef fm::Ptrdiff GLintptr;
typedef fm::Size    GLsizeiptr;
typedef fm::Int64   GLint64;
typedef fm::Uint64  GLuint64;
typedef GLintptr    GLintptrARB;
typedef GLsizeiptr  GLsizeiptrARB;
typedef GLint64     GLint64EXT;
typedef GLuint64    GLuint64EXT;
typedef struct __GLsync *GLsync;

#endif // FRONTIER_GL_TYPES_HPP_INCLUDED

