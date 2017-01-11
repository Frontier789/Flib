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
#ifndef FRONTIER_GL_FUNCTIONS_H_INCLUDED
#define FRONTIER_GL_FUNCTIONS_H_INCLUDED
#include <FRONTIER/System/util/API.h>
	
	#ifdef __GNUC__
		#ifndef __stdcall
			#define __stdcall __attribute__((stdcall))
		#endif
	#endif
	
	#ifndef APIENTRY
		#if defined(__MINGW32__) || (defined(_MSC_VER) && (_MSC_VER >= 800)) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
			#define APIENTRY __stdcall
		#else
			#define APIENTRY
		#endif
	#endif //APIENTRY
	
	#ifndef API_ENTRY
		#if defined(_WIN32)
			#define API_ENTRY APIENTRY
		#else
			#define API_ENTRY
		#endif
	#endif //API_ENTRY
	
	#ifdef __cplusplus
	extern "C"
	{
	#endif //__cplusplus

	FRONTIER_API extern void (API_ENTRY *_ptr_to_glAccum)(GLenum op,GLfloat value);
	#define glAccum _ptr_to_glAccum
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glActiveShaderProgram)(GLuint pipeline,GLuint program);
	#define glActiveShaderProgram _ptr_to_glActiveShaderProgram
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glActiveTexture)(GLenum texture);
	#define glActiveTexture _ptr_to_glActiveTexture
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glAlphaFunc)(GLenum func,GLfloat ref);
	#define glAlphaFunc _ptr_to_glAlphaFunc
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glAreTexturesResident)(GLsizei n,const GLuint* textures,GLboolean* residences);
	#define glAreTexturesResident _ptr_to_glAreTexturesResident
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glArrayElement)(GLint i);
	#define glArrayElement _ptr_to_glArrayElement
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glAttachObjectARB)(GLhandleARB containerObj,GLhandleARB obj);
	#define glAttachObjectARB _ptr_to_glAttachObjectARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glAttachShader)(GLuint program,GLuint shader);
	#define glAttachShader _ptr_to_glAttachShader
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBegin)(GLenum mode);
	#define glBegin _ptr_to_glBegin
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBeginConditionalRender)(GLuint id,GLenum mode);
	#define glBeginConditionalRender _ptr_to_glBeginConditionalRender
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBeginQuery)(GLenum target,GLuint id);
	#define glBeginQuery _ptr_to_glBeginQuery
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBeginQueryIndexed)(GLenum target,GLuint index,GLuint id);
	#define glBeginQueryIndexed _ptr_to_glBeginQueryIndexed
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBeginTransformFeedback)(GLenum primitiveMode);
	#define glBeginTransformFeedback _ptr_to_glBeginTransformFeedback
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindAttribLocation)(GLuint program,GLuint index,const GLchar* name);
	#define glBindAttribLocation _ptr_to_glBindAttribLocation
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindAttribLocationARB)(GLhandleARB programObj,GLuint index,const GLcharARB* name);
	#define glBindAttribLocationARB _ptr_to_glBindAttribLocationARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindBuffer)(GLenum target,GLuint buffer);
	#define glBindBuffer _ptr_to_glBindBuffer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindBufferARB)(GLenum target,GLuint buffer);
	#define glBindBufferARB _ptr_to_glBindBufferARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindBufferBase)(GLenum target,GLuint index,GLuint buffer);
	#define glBindBufferBase _ptr_to_glBindBufferBase
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindBufferRange)(GLenum target,GLuint index,GLuint buffer,GLintptr offset,GLsizeiptr size);
	#define glBindBufferRange _ptr_to_glBindBufferRange
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindFragDataLocation)(GLuint program,GLuint color,const GLchar* name);
	#define glBindFragDataLocation _ptr_to_glBindFragDataLocation
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindFragDataLocationIndexed)(GLuint program,GLuint colorNumber,GLuint index,const GLchar* name);
	#define glBindFragDataLocationIndexed _ptr_to_glBindFragDataLocationIndexed
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindFramebuffer)(GLenum target,GLuint framebuffer);
	#define glBindFramebuffer _ptr_to_glBindFramebuffer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindFramebufferEXT)(GLenum target,GLuint framebuffer);
	#define glBindFramebufferEXT _ptr_to_glBindFramebufferEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindImageTexture)(GLuint unit,GLuint texture,GLint level,GLboolean layered,GLint layer,GLenum access,GLenum format);
	#define glBindImageTexture _ptr_to_glBindImageTexture
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindProgramARB)(GLenum target,GLuint program);
	#define glBindProgramARB _ptr_to_glBindProgramARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindProgramPipeline)(GLuint pipeline);
	#define glBindProgramPipeline _ptr_to_glBindProgramPipeline
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindRenderbuffer)(GLenum target,GLuint renderbuffer);
	#define glBindRenderbuffer _ptr_to_glBindRenderbuffer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindRenderbufferEXT)(GLenum target,GLuint renderbuffer);
	#define glBindRenderbufferEXT _ptr_to_glBindRenderbufferEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindSampler)(GLuint unit,GLuint sampler);
	#define glBindSampler _ptr_to_glBindSampler
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindTexture)(GLenum target,GLuint texture);
	#define glBindTexture _ptr_to_glBindTexture
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindTransformFeedback)(GLenum target,GLuint id);
	#define glBindTransformFeedback _ptr_to_glBindTransformFeedback
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBindVertexArray)(GLuint ren_array);
	#define glBindVertexArray _ptr_to_glBindVertexArray
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBitmap)(GLsizei width,GLsizei height,GLfloat xorig,GLfloat yorig,GLfloat xmove,GLfloat ymove,const GLubyte* bitmap);
	#define glBitmap _ptr_to_glBitmap
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlendColor)(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha);
	#define glBlendColor _ptr_to_glBlendColor
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlendEquation)(GLenum mode);
	#define glBlendEquation _ptr_to_glBlendEquation
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlendEquationSeparate)(GLenum modeRGB,GLenum modeAlpha);
	#define glBlendEquationSeparate _ptr_to_glBlendEquationSeparate
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlendEquationSeparatei)(GLuint buf,GLenum modeRGB,GLenum modeAlpha);
	#define glBlendEquationSeparatei _ptr_to_glBlendEquationSeparatei
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlendEquationi)(GLuint buf,GLenum mode);
	#define glBlendEquationi _ptr_to_glBlendEquationi
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlendFunc)(GLenum sfactor,GLenum dfactor);
	#define glBlendFunc _ptr_to_glBlendFunc
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlendFuncSeparate)(GLenum sfactorRGB,GLenum dfactorRGB,GLenum sfactorAlpha,GLenum dfactorAlpha);
	#define glBlendFuncSeparate _ptr_to_glBlendFuncSeparate
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlendFuncSeparatei)(GLuint buf,GLenum srcRGB,GLenum dstRGB,GLenum srcAlpha,GLenum dstAlpha);
	#define glBlendFuncSeparatei _ptr_to_glBlendFuncSeparatei
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlendFunci)(GLuint buf,GLenum src,GLenum dst);
	#define glBlendFunci _ptr_to_glBlendFunci
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBlitFramebuffer)(GLint srcX0,GLint srcY0,GLint srcX1,GLint srcY1,GLint dstX0,GLint dstY0,GLint dstX1,GLint dstY1,GLbitfield mask,GLenum filter);
	#define glBlitFramebuffer _ptr_to_glBlitFramebuffer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBufferData)(GLenum target,GLsizeiptr size,const GLvoid* data,GLenum usage);
	#define glBufferData _ptr_to_glBufferData
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBufferDataARB)(GLenum target,GLsizeiptrARB size,const GLvoid* data,GLenum usage);
	#define glBufferDataARB _ptr_to_glBufferDataARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBufferSubData)(GLenum target,GLintptr offset,GLsizeiptr size,const GLvoid* data);
	#define glBufferSubData _ptr_to_glBufferSubData
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glBufferSubDataARB)(GLenum target,GLintptrARB offset,GLsizeiptrARB size,const GLvoid* data);
	#define glBufferSubDataARB _ptr_to_glBufferSubDataARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCallList)(GLuint list);
	#define glCallList _ptr_to_glCallList
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCallLists)(GLsizei n,GLenum type,const GLvoid* lists);
	#define glCallLists _ptr_to_glCallLists
	FRONTIER_API extern GLenum (API_ENTRY *_ptr_to_glCheckFramebufferStatus)(GLenum target);
	#define glCheckFramebufferStatus _ptr_to_glCheckFramebufferStatus
	FRONTIER_API extern GLenum (API_ENTRY *_ptr_to_glCheckFramebufferStatusEXT)(GLenum target);
	#define glCheckFramebufferStatusEXT _ptr_to_glCheckFramebufferStatusEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClampColor)(GLenum target,GLenum clamp);
	#define glClampColor _ptr_to_glClampColor
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClear)(GLbitfield mask);
	#define glClear _ptr_to_glClear
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearAccum)(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha);
	#define glClearAccum _ptr_to_glClearAccum
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearBufferfi)(GLenum buffer,GLint drawbuffer,GLfloat depth,GLint stencil);
	#define glClearBufferfi _ptr_to_glClearBufferfi
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearBufferfv)(GLenum buffer,GLint drawbuffer,const GLfloat* value);
	#define glClearBufferfv _ptr_to_glClearBufferfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearBufferiv)(GLenum buffer,GLint drawbuffer,const GLint* value);
	#define glClearBufferiv _ptr_to_glClearBufferiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearBufferuiv)(GLenum buffer,GLint drawbuffer,const GLuint* value);
	#define glClearBufferuiv _ptr_to_glClearBufferuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearColor)(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha);
	#define glClearColor _ptr_to_glClearColor
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearDepth)(GLdouble depth);
	#define glClearDepth _ptr_to_glClearDepth
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearDepthf)(GLfloat d);
	#define glClearDepthf _ptr_to_glClearDepthf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearIndex)(GLfloat c);
	#define glClearIndex _ptr_to_glClearIndex
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClearStencil)(GLint s);
	#define glClearStencil _ptr_to_glClearStencil
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClientActiveTexture)(GLenum texture);
	#define glClientActiveTexture _ptr_to_glClientActiveTexture
	FRONTIER_API extern GLenum (API_ENTRY *_ptr_to_glClientWaitSync)(GLsync sync,GLbitfield flags,GLuint64 timeout);
	#define glClientWaitSync _ptr_to_glClientWaitSync
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glClipPlane)(GLenum plane,const GLdouble* equation);
	#define glClipPlane _ptr_to_glClipPlane
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3b)(GLbyte red,GLbyte green,GLbyte blue);
	#define glColor3b _ptr_to_glColor3b
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3bv)(const GLbyte* v);
	#define glColor3bv _ptr_to_glColor3bv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3d)(GLdouble red,GLdouble green,GLdouble blue);
	#define glColor3d _ptr_to_glColor3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3dv)(const GLdouble* v);
	#define glColor3dv _ptr_to_glColor3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3f)(GLfloat red,GLfloat green,GLfloat blue);
	#define glColor3f _ptr_to_glColor3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3fv)(const GLfloat* v);
	#define glColor3fv _ptr_to_glColor3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3i)(GLint red,GLint green,GLint blue);
	#define glColor3i _ptr_to_glColor3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3iv)(const GLint* v);
	#define glColor3iv _ptr_to_glColor3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3s)(GLshort red,GLshort green,GLshort blue);
	#define glColor3s _ptr_to_glColor3s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3sv)(const GLshort* v);
	#define glColor3sv _ptr_to_glColor3sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3ub)(GLubyte red,GLubyte green,GLubyte blue);
	#define glColor3ub _ptr_to_glColor3ub
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3ubv)(const GLubyte* v);
	#define glColor3ubv _ptr_to_glColor3ubv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3ui)(GLuint red,GLuint green,GLuint blue);
	#define glColor3ui _ptr_to_glColor3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3uiv)(const GLuint* v);
	#define glColor3uiv _ptr_to_glColor3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3us)(GLushort red,GLushort green,GLushort blue);
	#define glColor3us _ptr_to_glColor3us
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor3usv)(const GLushort* v);
	#define glColor3usv _ptr_to_glColor3usv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4b)(GLbyte red,GLbyte green,GLbyte blue,GLbyte alpha);
	#define glColor4b _ptr_to_glColor4b
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4bv)(const GLbyte* v);
	#define glColor4bv _ptr_to_glColor4bv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4d)(GLdouble red,GLdouble green,GLdouble blue,GLdouble alpha);
	#define glColor4d _ptr_to_glColor4d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4dv)(const GLdouble* v);
	#define glColor4dv _ptr_to_glColor4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4f)(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha);
	#define glColor4f _ptr_to_glColor4f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4fv)(const GLfloat* v);
	#define glColor4fv _ptr_to_glColor4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4i)(GLint red,GLint green,GLint blue,GLint alpha);
	#define glColor4i _ptr_to_glColor4i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4iv)(const GLint* v);
	#define glColor4iv _ptr_to_glColor4iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4s)(GLshort red,GLshort green,GLshort blue,GLshort alpha);
	#define glColor4s _ptr_to_glColor4s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4sv)(const GLshort* v);
	#define glColor4sv _ptr_to_glColor4sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4ub)(GLubyte red,GLubyte green,GLubyte blue,GLubyte alpha);
	#define glColor4ub _ptr_to_glColor4ub
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4ubv)(const GLubyte* v);
	#define glColor4ubv _ptr_to_glColor4ubv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4ui)(GLuint red,GLuint green,GLuint blue,GLuint alpha);
	#define glColor4ui _ptr_to_glColor4ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4uiv)(const GLuint* v);
	#define glColor4uiv _ptr_to_glColor4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4us)(GLushort red,GLushort green,GLushort blue,GLushort alpha);
	#define glColor4us _ptr_to_glColor4us
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColor4usv)(const GLushort* v);
	#define glColor4usv _ptr_to_glColor4usv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColorMask)(GLboolean red,GLboolean green,GLboolean blue,GLboolean alpha);
	#define glColorMask _ptr_to_glColorMask
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColorMaski)(GLuint index,GLboolean r,GLboolean g,GLboolean b,GLboolean a);
	#define glColorMaski _ptr_to_glColorMaski
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColorMaterial)(GLenum face,GLenum mode);
	#define glColorMaterial _ptr_to_glColorMaterial
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColorP3ui)(GLenum type,GLuint color);
	#define glColorP3ui _ptr_to_glColorP3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColorP3uiv)(GLenum type,const GLuint* color);
	#define glColorP3uiv _ptr_to_glColorP3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColorP4ui)(GLenum type,GLuint color);
	#define glColorP4ui _ptr_to_glColorP4ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColorP4uiv)(GLenum type,const GLuint* color);
	#define glColorP4uiv _ptr_to_glColorP4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glColorPointer)(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer);
	#define glColorPointer _ptr_to_glColorPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCompileShader)(GLuint shader);
	#define glCompileShader _ptr_to_glCompileShader
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCompileShaderARB)(GLhandleARB shaderObj);
	#define glCompileShaderARB _ptr_to_glCompileShaderARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCompressedTexImage1D)(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLint border,GLsizei imageSize,const GLvoid* data);
	#define glCompressedTexImage1D _ptr_to_glCompressedTexImage1D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCompressedTexImage2D)(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLint border,GLsizei imageSize,const GLvoid* data);
	#define glCompressedTexImage2D _ptr_to_glCompressedTexImage2D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCompressedTexImage3D)(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLsizei imageSize,const GLvoid* data);
	#define glCompressedTexImage3D _ptr_to_glCompressedTexImage3D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCompressedTexSubImage1D)(GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLsizei imageSize,const GLvoid* data);
	#define glCompressedTexSubImage1D _ptr_to_glCompressedTexSubImage1D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCompressedTexSubImage2D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLsizei imageSize,const GLvoid* data);
	#define glCompressedTexSubImage2D _ptr_to_glCompressedTexSubImage2D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCompressedTexSubImage3D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLsizei imageSize,const GLvoid* data);
	#define glCompressedTexSubImage3D _ptr_to_glCompressedTexSubImage3D
	FRONTIER_API extern void * (API_ENTRY *_ptr_to_glCopyBufferSubData)(GLenum readTarget,GLenum writeTarget,GLintptr readOffset,GLintptr writeOffset,GLsizeiptr size);
	#define glCopyBufferSubData _ptr_to_glCopyBufferSubData
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCopyPixels)(GLint x,GLint y,GLsizei width,GLsizei height,GLenum type);
	#define glCopyPixels _ptr_to_glCopyPixels
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCopyTexImage1D)(GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLint border);
	#define glCopyTexImage1D _ptr_to_glCopyTexImage1D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCopyTexImage2D)(GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLsizei height,GLint border);
	#define glCopyTexImage2D _ptr_to_glCopyTexImage2D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCopyTexSubImage1D)(GLenum target,GLint level,GLint xoffset,GLint x,GLint y,GLsizei width);
	#define glCopyTexSubImage1D _ptr_to_glCopyTexSubImage1D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCopyTexSubImage2D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint x,GLint y,GLsizei width,GLsizei height);
	#define glCopyTexSubImage2D _ptr_to_glCopyTexSubImage2D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCopyTexSubImage3D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLint x,GLint y,GLsizei width,GLsizei height);
	#define glCopyTexSubImage3D _ptr_to_glCopyTexSubImage3D
	FRONTIER_API extern GLuint (API_ENTRY *_ptr_to_glCreateProgram)();
	#define glCreateProgram _ptr_to_glCreateProgram
	FRONTIER_API extern GLhandleARB (API_ENTRY *_ptr_to_glCreateProgramObjectARB)();
	#define glCreateProgramObjectARB _ptr_to_glCreateProgramObjectARB
	FRONTIER_API extern GLuint (API_ENTRY *_ptr_to_glCreateShader)(GLenum type);
	#define glCreateShader _ptr_to_glCreateShader
	FRONTIER_API extern GLhandleARB (API_ENTRY *_ptr_to_glCreateShaderObjectARB)(GLenum shaderType);
	#define glCreateShaderObjectARB _ptr_to_glCreateShaderObjectARB
	FRONTIER_API extern GLuint (API_ENTRY *_ptr_to_glCreateShaderProgramv)(GLenum type,GLsizei count,const GLchar*const* strings);
	#define glCreateShaderProgramv _ptr_to_glCreateShaderProgramv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glCullFace)(GLenum mode);
	#define glCullFace _ptr_to_glCullFace
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteBuffers)(GLsizei n,const GLuint* buffers);
	#define glDeleteBuffers _ptr_to_glDeleteBuffers
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteBuffersARB)(GLsizei n,const GLuint* buffers);
	#define glDeleteBuffersARB _ptr_to_glDeleteBuffersARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteFramebuffers)(GLsizei n,const GLuint* framebuffers);
	#define glDeleteFramebuffers _ptr_to_glDeleteFramebuffers
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteFramebuffersEXT)(GLsizei n,const GLuint* framebuffers);
	#define glDeleteFramebuffersEXT _ptr_to_glDeleteFramebuffersEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteLists)(GLuint list,GLsizei range);
	#define glDeleteLists _ptr_to_glDeleteLists
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteObjectARB)(GLhandleARB obj);
	#define glDeleteObjectARB _ptr_to_glDeleteObjectARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteProgram)(GLuint program);
	#define glDeleteProgram _ptr_to_glDeleteProgram
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteProgramPipelines)(GLsizei n,const GLuint* pipelines);
	#define glDeleteProgramPipelines _ptr_to_glDeleteProgramPipelines
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteProgramsARB)(GLsizei n,const GLuint* programs);
	#define glDeleteProgramsARB _ptr_to_glDeleteProgramsARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteQueries)(GLsizei n,const GLuint* ids);
	#define glDeleteQueries _ptr_to_glDeleteQueries
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteRenderbuffers)(GLsizei n,const GLuint* renderbuffers);
	#define glDeleteRenderbuffers _ptr_to_glDeleteRenderbuffers
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteRenderbuffersEXT)(GLsizei n,const GLuint* renderbuffers);
	#define glDeleteRenderbuffersEXT _ptr_to_glDeleteRenderbuffersEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteSamplers)(GLsizei count,const GLuint* samplers);
	#define glDeleteSamplers _ptr_to_glDeleteSamplers
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteShader)(GLuint shader);
	#define glDeleteShader _ptr_to_glDeleteShader
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteSync)(GLsync sync);
	#define glDeleteSync _ptr_to_glDeleteSync
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteTextures)(GLsizei n,const GLuint* textures);
	#define glDeleteTextures _ptr_to_glDeleteTextures
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteTransformFeedbacks)(GLsizei n,const GLuint* ids);
	#define glDeleteTransformFeedbacks _ptr_to_glDeleteTransformFeedbacks
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDeleteVertexArrays)(GLsizei n,const GLuint* arrays);
	#define glDeleteVertexArrays _ptr_to_glDeleteVertexArrays
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDepthFunc)(GLenum func);
	#define glDepthFunc _ptr_to_glDepthFunc
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDepthMask)(GLboolean flag);
	#define glDepthMask _ptr_to_glDepthMask
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDepthRange)(GLdouble ren_near,GLdouble ren_far);
	#define glDepthRange _ptr_to_glDepthRange
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDepthRangeArrayv)(GLuint first,GLsizei count,const GLdouble* v);
	#define glDepthRangeArrayv _ptr_to_glDepthRangeArrayv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDepthRangeIndexed)(GLuint index,GLdouble n,GLdouble f);
	#define glDepthRangeIndexed _ptr_to_glDepthRangeIndexed
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDepthRangef)(GLfloat n,GLfloat f);
	#define glDepthRangef _ptr_to_glDepthRangef
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDetachObjectARB)(GLhandleARB containerObj,GLhandleARB attachedObj);
	#define glDetachObjectARB _ptr_to_glDetachObjectARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDetachShader)(GLuint program,GLuint shader);
	#define glDetachShader _ptr_to_glDetachShader
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDisable)(GLenum cap);
	#define glDisable _ptr_to_glDisable
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDisableClientState)(GLenum ren_array);
	#define glDisableClientState _ptr_to_glDisableClientState
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDisableVertexAttribArray)(GLuint index);
	#define glDisableVertexAttribArray _ptr_to_glDisableVertexAttribArray
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDisableVertexAttribArrayARB)(GLuint index);
	#define glDisableVertexAttribArrayARB _ptr_to_glDisableVertexAttribArrayARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDisablei)(GLenum target,GLuint index);
	#define glDisablei _ptr_to_glDisablei
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawArrays)(GLenum mode,GLint first,GLsizei count);
	#define glDrawArrays _ptr_to_glDrawArrays
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawArraysIndirect)(GLenum mode,const GLvoid* indirect);
	#define glDrawArraysIndirect _ptr_to_glDrawArraysIndirect
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawArraysInstanced)(GLenum mode,GLint first,GLsizei count,GLsizei instancecount);
	#define glDrawArraysInstanced _ptr_to_glDrawArraysInstanced
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawArraysInstancedBaseInstance)(GLenum mode,GLint first,GLsizei count,GLsizei instancecount,GLuint baseinstance);
	#define glDrawArraysInstancedBaseInstance _ptr_to_glDrawArraysInstancedBaseInstance
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawBuffer)(GLenum mode);
	#define glDrawBuffer _ptr_to_glDrawBuffer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawBuffers)(GLsizei n,const GLenum* bufs);
	#define glDrawBuffers _ptr_to_glDrawBuffers
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawElements)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices);
	#define glDrawElements _ptr_to_glDrawElements
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawElementsBaseVertex)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLint basevertex);
	#define glDrawElementsBaseVertex _ptr_to_glDrawElementsBaseVertex
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawElementsIndirect)(GLenum mode,GLenum type,const GLvoid* indirect);
	#define glDrawElementsIndirect _ptr_to_glDrawElementsIndirect
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawElementsInstanced)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount);
	#define glDrawElementsInstanced _ptr_to_glDrawElementsInstanced
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawElementsInstancedBaseInstance)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLuint baseinstance);
	#define glDrawElementsInstancedBaseInstance _ptr_to_glDrawElementsInstancedBaseInstance
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawElementsInstancedBaseVertex)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLint basevertex);
	#define glDrawElementsInstancedBaseVertex _ptr_to_glDrawElementsInstancedBaseVertex
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawElementsInstancedBaseVertexBaseInstance)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLint basevertex,GLuint baseinstance);
	#define glDrawElementsInstancedBaseVertexBaseInstance _ptr_to_glDrawElementsInstancedBaseVertexBaseInstance
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawPixels)(GLsizei width,GLsizei height,GLenum format,GLenum type,const GLvoid* pixels);
	#define glDrawPixels _ptr_to_glDrawPixels
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawRangeElements)(GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const GLvoid* indices);
	#define glDrawRangeElements _ptr_to_glDrawRangeElements
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawRangeElementsBaseVertex)(GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const GLvoid* indices,GLint basevertex);
	#define glDrawRangeElementsBaseVertex _ptr_to_glDrawRangeElementsBaseVertex
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawTransformFeedback)(GLenum mode,GLuint id);
	#define glDrawTransformFeedback _ptr_to_glDrawTransformFeedback
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawTransformFeedbackInstanced)(GLenum mode,GLuint id,GLsizei instancecount);
	#define glDrawTransformFeedbackInstanced _ptr_to_glDrawTransformFeedbackInstanced
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawTransformFeedbackStream)(GLenum mode,GLuint id,GLuint stream);
	#define glDrawTransformFeedbackStream _ptr_to_glDrawTransformFeedbackStream
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glDrawTransformFeedbackStreamInstanced)(GLenum mode,GLuint id,GLuint stream,GLsizei instancecount);
	#define glDrawTransformFeedbackStreamInstanced _ptr_to_glDrawTransformFeedbackStreamInstanced
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEdgeFlag)(GLboolean flag);
	#define glEdgeFlag _ptr_to_glEdgeFlag
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEdgeFlagPointer)(GLsizei stride,const GLvoid* pointer);
	#define glEdgeFlagPointer _ptr_to_glEdgeFlagPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEdgeFlagv)(const GLboolean* flag);
	#define glEdgeFlagv _ptr_to_glEdgeFlagv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEnable)(GLenum cap);
	#define glEnable _ptr_to_glEnable
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEnableClientState)(GLenum ren_array);
	#define glEnableClientState _ptr_to_glEnableClientState
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEnableVertexAttribArray)(GLuint index);
	#define glEnableVertexAttribArray _ptr_to_glEnableVertexAttribArray
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEnableVertexAttribArrayARB)(GLuint index);
	#define glEnableVertexAttribArrayARB _ptr_to_glEnableVertexAttribArrayARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEnablei)(GLenum target,GLuint index);
	#define glEnablei _ptr_to_glEnablei
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEnd)();
	#define glEnd _ptr_to_glEnd
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEndConditionalRender)();
	#define glEndConditionalRender _ptr_to_glEndConditionalRender
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEndList)();
	#define glEndList _ptr_to_glEndList
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEndQuery)(GLenum target);
	#define glEndQuery _ptr_to_glEndQuery
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEndQueryIndexed)(GLenum target,GLuint index);
	#define glEndQueryIndexed _ptr_to_glEndQueryIndexed
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEndTransformFeedback)();
	#define glEndTransformFeedback _ptr_to_glEndTransformFeedback
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalCoord1d)(GLdouble u);
	#define glEvalCoord1d _ptr_to_glEvalCoord1d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalCoord1dv)(const GLdouble* u);
	#define glEvalCoord1dv _ptr_to_glEvalCoord1dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalCoord1f)(GLfloat u);
	#define glEvalCoord1f _ptr_to_glEvalCoord1f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalCoord1fv)(const GLfloat* u);
	#define glEvalCoord1fv _ptr_to_glEvalCoord1fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalCoord2d)(GLdouble u,GLdouble v);
	#define glEvalCoord2d _ptr_to_glEvalCoord2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalCoord2dv)(const GLdouble* u);
	#define glEvalCoord2dv _ptr_to_glEvalCoord2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalCoord2f)(GLfloat u,GLfloat v);
	#define glEvalCoord2f _ptr_to_glEvalCoord2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalCoord2fv)(const GLfloat* u);
	#define glEvalCoord2fv _ptr_to_glEvalCoord2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalMesh1)(GLenum mode,GLint i1,GLint i2);
	#define glEvalMesh1 _ptr_to_glEvalMesh1
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalMesh2)(GLenum mode,GLint i1,GLint i2,GLint j1,GLint j2);
	#define glEvalMesh2 _ptr_to_glEvalMesh2
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalPoint1)(GLint i);
	#define glEvalPoint1 _ptr_to_glEvalPoint1
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glEvalPoint2)(GLint i,GLint j);
	#define glEvalPoint2 _ptr_to_glEvalPoint2
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFeedbackBuffer)(GLsizei size,GLenum type,GLfloat* buffer);
	#define glFeedbackBuffer _ptr_to_glFeedbackBuffer
	FRONTIER_API extern GLsync (API_ENTRY *_ptr_to_glFenceSync)(GLenum condition,GLbitfield flags);
	#define glFenceSync _ptr_to_glFenceSync
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFinish)();
	#define glFinish _ptr_to_glFinish
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFlush)();
	#define glFlush _ptr_to_glFlush
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFlushMappedBufferRange)(GLenum target,GLintptr offset,GLsizeiptr length);
	#define glFlushMappedBufferRange _ptr_to_glFlushMappedBufferRange
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFogCoordPointer)(GLenum type,GLsizei stride,const GLvoid* pointer);
	#define glFogCoordPointer _ptr_to_glFogCoordPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFogCoordd)(GLdouble coord);
	#define glFogCoordd _ptr_to_glFogCoordd
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFogCoorddv)(const GLdouble* coord);
	#define glFogCoorddv _ptr_to_glFogCoorddv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFogCoordf)(GLfloat coord);
	#define glFogCoordf _ptr_to_glFogCoordf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFogCoordfv)(const GLfloat* coord);
	#define glFogCoordfv _ptr_to_glFogCoordfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFogf)(GLenum pname,GLfloat param);
	#define glFogf _ptr_to_glFogf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFogfv)(GLenum pname,const GLfloat* params);
	#define glFogfv _ptr_to_glFogfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFogi)(GLenum pname,GLint param);
	#define glFogi _ptr_to_glFogi
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFogiv)(GLenum pname,const GLint* params);
	#define glFogiv _ptr_to_glFogiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferRenderbuffer)(GLenum target,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer);
	#define glFramebufferRenderbuffer _ptr_to_glFramebufferRenderbuffer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferRenderbufferEXT)(GLenum target,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer);
	#define glFramebufferRenderbufferEXT _ptr_to_glFramebufferRenderbufferEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferTexture)(GLenum target,GLenum attachment,GLuint texture,GLint level);
	#define glFramebufferTexture _ptr_to_glFramebufferTexture
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferTexture1D)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level);
	#define glFramebufferTexture1D _ptr_to_glFramebufferTexture1D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferTexture1DEXT)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level);
	#define glFramebufferTexture1DEXT _ptr_to_glFramebufferTexture1DEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferTexture2D)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level);
	#define glFramebufferTexture2D _ptr_to_glFramebufferTexture2D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferTexture2DEXT)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level);
	#define glFramebufferTexture2DEXT _ptr_to_glFramebufferTexture2DEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferTexture3D)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset);
	#define glFramebufferTexture3D _ptr_to_glFramebufferTexture3D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferTexture3DEXT)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset);
	#define glFramebufferTexture3DEXT _ptr_to_glFramebufferTexture3DEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFramebufferTextureLayer)(GLenum target,GLenum attachment,GLuint texture,GLint level,GLint layer);
	#define glFramebufferTextureLayer _ptr_to_glFramebufferTextureLayer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFrontFace)(GLenum mode);
	#define glFrontFace _ptr_to_glFrontFace
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glFrustum)(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar);
	#define glFrustum _ptr_to_glFrustum
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenBuffers)(GLsizei n,GLuint* buffers);
	#define glGenBuffers _ptr_to_glGenBuffers
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenBuffersARB)(GLsizei n,GLuint* buffers);
	#define glGenBuffersARB _ptr_to_glGenBuffersARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenFramebuffers)(GLsizei n,GLuint* framebuffers);
	#define glGenFramebuffers _ptr_to_glGenFramebuffers
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenFramebuffersEXT)(GLsizei n,GLuint* framebuffers);
	#define glGenFramebuffersEXT _ptr_to_glGenFramebuffersEXT
	FRONTIER_API extern GLuint (API_ENTRY *_ptr_to_glGenLists)(GLsizei range);
	#define glGenLists _ptr_to_glGenLists
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenProgramPipelines)(GLsizei n,GLuint* pipelines);
	#define glGenProgramPipelines _ptr_to_glGenProgramPipelines
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenProgramsARB)(GLsizei n,GLuint* programs);
	#define glGenProgramsARB _ptr_to_glGenProgramsARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenQueries)(GLsizei n,GLuint* ids);
	#define glGenQueries _ptr_to_glGenQueries
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenRenderbuffers)(GLsizei n,GLuint* renderbuffers);
	#define glGenRenderbuffers _ptr_to_glGenRenderbuffers
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenRenderbuffersEXT)(GLsizei n,GLuint* renderbuffers);
	#define glGenRenderbuffersEXT _ptr_to_glGenRenderbuffersEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenSamplers)(GLsizei count,GLuint* samplers);
	#define glGenSamplers _ptr_to_glGenSamplers
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenTextures)(GLsizei n,GLuint* textures);
	#define glGenTextures _ptr_to_glGenTextures
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenTransformFeedbacks)(GLsizei n,GLuint* ids);
	#define glGenTransformFeedbacks _ptr_to_glGenTransformFeedbacks
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenVertexArrays)(GLsizei n,GLuint* arrays);
	#define glGenVertexArrays _ptr_to_glGenVertexArrays
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenerateMipmap)(GLenum target);
	#define glGenerateMipmap _ptr_to_glGenerateMipmap
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGenerateMipmapEXT)(GLenum target);
	#define glGenerateMipmapEXT _ptr_to_glGenerateMipmapEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveAtomicCounterBufferiv)(GLuint program,GLuint bufferIndex,GLenum pname,GLint* params);
	#define glGetActiveAtomicCounterBufferiv _ptr_to_glGetActiveAtomicCounterBufferiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveAttrib)(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLint* size,GLenum* type,GLchar* name);
	#define glGetActiveAttrib _ptr_to_glGetActiveAttrib
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveAttribARB)(GLhandleARB programObj,GLuint index,GLsizei maxLength,GLsizei* length,GLint* size,GLenum* type,GLcharARB* name);
	#define glGetActiveAttribARB _ptr_to_glGetActiveAttribARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveSubroutineName)(GLuint program,GLenum shadertype,GLuint index,GLsizei bufsize,GLsizei* length,GLchar* name);
	#define glGetActiveSubroutineName _ptr_to_glGetActiveSubroutineName
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveSubroutineUniformName)(GLuint program,GLenum shadertype,GLuint index,GLsizei bufsize,GLsizei* length,GLchar* name);
	#define glGetActiveSubroutineUniformName _ptr_to_glGetActiveSubroutineUniformName
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveSubroutineUniformiv)(GLuint program,GLenum shadertype,GLuint index,GLenum pname,GLint* values);
	#define glGetActiveSubroutineUniformiv _ptr_to_glGetActiveSubroutineUniformiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveUniform)(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLint* size,GLenum* type,GLchar* name);
	#define glGetActiveUniform _ptr_to_glGetActiveUniform
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveUniformARB)(GLhandleARB programObj,GLuint index,GLsizei maxLength,GLsizei* length,GLint* size,GLenum* type,GLcharARB* name);
	#define glGetActiveUniformARB _ptr_to_glGetActiveUniformARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveUniformBlockName)(GLuint program,GLuint uniformBlockIndex,GLsizei bufSize,GLsizei* length,GLchar* uniformBlockName);
	#define glGetActiveUniformBlockName _ptr_to_glGetActiveUniformBlockName
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveUniformBlockiv)(GLuint program,GLuint uniformBlockIndex,GLenum pname,GLint* params);
	#define glGetActiveUniformBlockiv _ptr_to_glGetActiveUniformBlockiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveUniformName)(GLuint program,GLuint uniformIndex,GLsizei bufSize,GLsizei* length,GLchar* uniformName);
	#define glGetActiveUniformName _ptr_to_glGetActiveUniformName
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetActiveUniformsiv)(GLuint program,GLsizei uniformCount,const GLuint* uniformIndices,GLenum pname,GLint* params);
	#define glGetActiveUniformsiv _ptr_to_glGetActiveUniformsiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetAttachedObjectsARB)(GLhandleARB containerObj,GLsizei maxCount,GLsizei* count,GLhandleARB* obj);
	#define glGetAttachedObjectsARB _ptr_to_glGetAttachedObjectsARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetAttachedShaders)(GLuint program,GLsizei maxCount,GLsizei* count,GLuint* shaders);
	#define glGetAttachedShaders _ptr_to_glGetAttachedShaders
	FRONTIER_API extern GLint (API_ENTRY *_ptr_to_glGetAttribLocation)(GLuint program,const GLchar* name);
	#define glGetAttribLocation _ptr_to_glGetAttribLocation
	FRONTIER_API extern GLint (API_ENTRY *_ptr_to_glGetAttribLocationARB)(GLhandleARB programObj,const GLcharARB* name);
	#define glGetAttribLocationARB _ptr_to_glGetAttribLocationARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetBooleani_v)(GLenum target,GLuint index,GLboolean* data);
	#define glGetBooleani_v _ptr_to_glGetBooleani_v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetBooleanv)(GLenum pname,GLboolean* params);
	#define glGetBooleanv _ptr_to_glGetBooleanv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetBufferParameteri64v)(GLenum target,GLenum pname,GLint64* params);
	#define glGetBufferParameteri64v _ptr_to_glGetBufferParameteri64v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetBufferParameteriv)(GLenum target,GLenum pname,GLint* params);
	#define glGetBufferParameteriv _ptr_to_glGetBufferParameteriv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetBufferParameterivARB)(GLenum target,GLenum pname,GLint* params);
	#define glGetBufferParameterivARB _ptr_to_glGetBufferParameterivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetBufferPointerv)(GLenum target,GLenum pname,GLvoid** params);
	#define glGetBufferPointerv _ptr_to_glGetBufferPointerv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetBufferPointervARB)(GLenum target,GLenum pname,GLvoid** params);
	#define glGetBufferPointervARB _ptr_to_glGetBufferPointervARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetBufferSubData)(GLenum target,GLintptr offset,GLsizeiptr size,GLvoid* data);
	#define glGetBufferSubData _ptr_to_glGetBufferSubData
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetBufferSubDataARB)(GLenum target,GLintptrARB offset,GLsizeiptrARB size,GLvoid* data);
	#define glGetBufferSubDataARB _ptr_to_glGetBufferSubDataARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetClipPlane)(GLenum plane,GLdouble* equation);
	#define glGetClipPlane _ptr_to_glGetClipPlane
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetCompressedTexImage)(GLenum target,GLint level,GLvoid* img);
	#define glGetCompressedTexImage _ptr_to_glGetCompressedTexImage
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetDoublei_v)(GLenum target,GLuint index,GLdouble* data);
	#define glGetDoublei_v _ptr_to_glGetDoublei_v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetDoublev)(GLenum pname,GLdouble* params);
	#define glGetDoublev _ptr_to_glGetDoublev
	FRONTIER_API extern GLenum (API_ENTRY *_ptr_to_glGetError)();
	#define glGetError _ptr_to_glGetError
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetFloati_v)(GLenum target,GLuint index,GLfloat* data);
	#define glGetFloati_v _ptr_to_glGetFloati_v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetFloatv)(GLenum pname,GLfloat* params);
	#define glGetFloatv _ptr_to_glGetFloatv
	FRONTIER_API extern GLint (API_ENTRY *_ptr_to_glGetFragDataIndex)(GLuint program,const GLchar* name);
	#define glGetFragDataIndex _ptr_to_glGetFragDataIndex
	FRONTIER_API extern GLint (API_ENTRY *_ptr_to_glGetFragDataLocation)(GLuint program,const GLchar* name);
	#define glGetFragDataLocation _ptr_to_glGetFragDataLocation
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetFramebufferAttachmentParameteriv)(GLenum target,GLenum attachment,GLenum pname,GLint* params);
	#define glGetFramebufferAttachmentParameteriv _ptr_to_glGetFramebufferAttachmentParameteriv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetFramebufferAttachmentParameterivEXT)(GLenum target,GLenum attachment,GLenum pname,GLint* params);
	#define glGetFramebufferAttachmentParameterivEXT _ptr_to_glGetFramebufferAttachmentParameterivEXT
	FRONTIER_API extern GLhandleARB (API_ENTRY *_ptr_to_glGetHandleARB)(GLenum pname);
	#define glGetHandleARB _ptr_to_glGetHandleARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetInfoLogARB)(GLhandleARB obj,GLsizei maxLength,GLsizei* length,GLcharARB* infoLog);
	#define glGetInfoLogARB _ptr_to_glGetInfoLogARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetInteger64i_v)(GLenum target,GLuint index,GLint64* data);
	#define glGetInteger64i_v _ptr_to_glGetInteger64i_v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetInteger64v)(GLenum pname,GLint64* params);
	#define glGetInteger64v _ptr_to_glGetInteger64v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetIntegeri_v)(GLenum target,GLuint index,GLint* data);
	#define glGetIntegeri_v _ptr_to_glGetIntegeri_v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetIntegerv)(GLenum pname,GLint* params);
	#define glGetIntegerv _ptr_to_glGetIntegerv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetInternalformati64v)(GLenum target,GLenum internalformat,GLenum pname,GLsizei bufSize,GLint64* params);
	#define glGetInternalformati64v _ptr_to_glGetInternalformati64v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetLightfv)(GLenum light,GLenum pname,GLfloat* params);
	#define glGetLightfv _ptr_to_glGetLightfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetLightiv)(GLenum light,GLenum pname,GLint* params);
	#define glGetLightiv _ptr_to_glGetLightiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetMapdv)(GLenum target,GLenum query,GLdouble* v);
	#define glGetMapdv _ptr_to_glGetMapdv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetMapfv)(GLenum target,GLenum query,GLfloat* v);
	#define glGetMapfv _ptr_to_glGetMapfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetMapiv)(GLenum target,GLenum query,GLint* v);
	#define glGetMapiv _ptr_to_glGetMapiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetMaterialfv)(GLenum face,GLenum pname,GLfloat* params);
	#define glGetMaterialfv _ptr_to_glGetMaterialfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetMaterialiv)(GLenum face,GLenum pname,GLint* params);
	#define glGetMaterialiv _ptr_to_glGetMaterialiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetMultisamplefv)(GLenum pname,GLuint index,GLfloat* val);
	#define glGetMultisamplefv _ptr_to_glGetMultisamplefv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetObjectParameterfvARB)(GLhandleARB obj,GLenum pname,GLfloat* params);
	#define glGetObjectParameterfvARB _ptr_to_glGetObjectParameterfvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetObjectParameterivARB)(GLhandleARB obj,GLenum pname,GLint* params);
	#define glGetObjectParameterivARB _ptr_to_glGetObjectParameterivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetPixelMapfv)(GLenum map,GLfloat* values);
	#define glGetPixelMapfv _ptr_to_glGetPixelMapfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetPixelMapuiv)(GLenum map,GLuint* values);
	#define glGetPixelMapuiv _ptr_to_glGetPixelMapuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetPixelMapusv)(GLenum map,GLushort* values);
	#define glGetPixelMapusv _ptr_to_glGetPixelMapusv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetPointerv)(GLenum pname,GLvoid** params);
	#define glGetPointerv _ptr_to_glGetPointerv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetPolygonStipple)(GLubyte* mask);
	#define glGetPolygonStipple _ptr_to_glGetPolygonStipple
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramBinary)(GLuint program,GLsizei bufSize,GLsizei* length,GLenum* binaryFormat,GLvoid* binary);
	#define glGetProgramBinary _ptr_to_glGetProgramBinary
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramEnvParameterdvARB)(GLenum target,GLuint index,GLdouble* params);
	#define glGetProgramEnvParameterdvARB _ptr_to_glGetProgramEnvParameterdvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramEnvParameterfvARB)(GLenum target,GLuint index,GLfloat* params);
	#define glGetProgramEnvParameterfvARB _ptr_to_glGetProgramEnvParameterfvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramInfoLog)(GLuint program,GLsizei bufSize,GLsizei* length,GLchar* infoLog);
	#define glGetProgramInfoLog _ptr_to_glGetProgramInfoLog
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramLocalParameterdvARB)(GLenum target,GLuint index,GLdouble* params);
	#define glGetProgramLocalParameterdvARB _ptr_to_glGetProgramLocalParameterdvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramLocalParameterfvARB)(GLenum target,GLuint index,GLfloat* params);
	#define glGetProgramLocalParameterfvARB _ptr_to_glGetProgramLocalParameterfvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramPipelineInfoLog)(GLuint pipeline,GLsizei bufSize,GLsizei* length,GLchar* infoLog);
	#define glGetProgramPipelineInfoLog _ptr_to_glGetProgramPipelineInfoLog
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramPipelineiv)(GLuint pipeline,GLenum pname,GLint* params);
	#define glGetProgramPipelineiv _ptr_to_glGetProgramPipelineiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramStageiv)(GLuint program,GLenum shadertype,GLenum pname,GLint* values);
	#define glGetProgramStageiv _ptr_to_glGetProgramStageiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramStringARB)(GLenum target,GLenum pname,GLvoid* string);
	#define glGetProgramStringARB _ptr_to_glGetProgramStringARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramiv)(GLuint program,GLenum pname,GLint* params);
	#define glGetProgramiv _ptr_to_glGetProgramiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetProgramivARB)(GLenum target,GLenum pname,GLint* params);
	#define glGetProgramivARB _ptr_to_glGetProgramivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetQueryIndexediv)(GLenum target,GLuint index,GLenum pname,GLint* params);
	#define glGetQueryIndexediv _ptr_to_glGetQueryIndexediv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetQueryObjecti64v)(GLuint id,GLenum pname,GLint64* params);
	#define glGetQueryObjecti64v _ptr_to_glGetQueryObjecti64v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetQueryObjectiv)(GLuint id,GLenum pname,GLint* params);
	#define glGetQueryObjectiv _ptr_to_glGetQueryObjectiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetQueryObjectui64v)(GLuint id,GLenum pname,GLuint64* params);
	#define glGetQueryObjectui64v _ptr_to_glGetQueryObjectui64v
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetQueryObjectuiv)(GLuint id,GLenum pname,GLuint* params);
	#define glGetQueryObjectuiv _ptr_to_glGetQueryObjectuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetQueryiv)(GLenum target,GLenum pname,GLint* params);
	#define glGetQueryiv _ptr_to_glGetQueryiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetRenderbufferParameteriv)(GLenum target,GLenum pname,GLint* params);
	#define glGetRenderbufferParameteriv _ptr_to_glGetRenderbufferParameteriv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetRenderbufferParameterivEXT)(GLenum target,GLenum pname,GLint* params);
	#define glGetRenderbufferParameterivEXT _ptr_to_glGetRenderbufferParameterivEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetSamplerParameterIiv)(GLuint sampler,GLenum pname,GLint* params);
	#define glGetSamplerParameterIiv _ptr_to_glGetSamplerParameterIiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetSamplerParameterIuiv)(GLuint sampler,GLenum pname,GLuint* params);
	#define glGetSamplerParameterIuiv _ptr_to_glGetSamplerParameterIuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetSamplerParameterfv)(GLuint sampler,GLenum pname,GLfloat* params);
	#define glGetSamplerParameterfv _ptr_to_glGetSamplerParameterfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetSamplerParameteriv)(GLuint sampler,GLenum pname,GLint* params);
	#define glGetSamplerParameteriv _ptr_to_glGetSamplerParameteriv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetShaderInfoLog)(GLuint shader,GLsizei bufSize,GLsizei* length,GLchar* infoLog);
	#define glGetShaderInfoLog _ptr_to_glGetShaderInfoLog
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetShaderPrecisionFormat)(GLenum shadertype,GLenum precisiontype,GLint* range,GLint* precision);
	#define glGetShaderPrecisionFormat _ptr_to_glGetShaderPrecisionFormat
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetShaderSource)(GLuint shader,GLsizei bufSize,GLsizei* length,GLchar* source);
	#define glGetShaderSource _ptr_to_glGetShaderSource
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetShaderSourceARB)(GLhandleARB obj,GLsizei maxLength,GLsizei* length,GLcharARB* source);
	#define glGetShaderSourceARB _ptr_to_glGetShaderSourceARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetShaderiv)(GLuint shader,GLenum pname,GLint* params);
	#define glGetShaderiv _ptr_to_glGetShaderiv
	FRONTIER_API extern const GLubyte* (API_ENTRY *_ptr_to_glGetString)(GLenum name);
	#define glGetString _ptr_to_glGetString
	FRONTIER_API extern const GLubyte* (API_ENTRY *_ptr_to_glGetStringi)(GLenum name,GLuint index);
	#define glGetStringi _ptr_to_glGetStringi
	FRONTIER_API extern GLuint (API_ENTRY *_ptr_to_glGetSubroutineIndex)(GLuint program,GLenum shadertype,const GLchar* name);
	#define glGetSubroutineIndex _ptr_to_glGetSubroutineIndex
	FRONTIER_API extern GLint (API_ENTRY *_ptr_to_glGetSubroutineUniformLocation)(GLuint program,GLenum shadertype,const GLchar* name);
	#define glGetSubroutineUniformLocation _ptr_to_glGetSubroutineUniformLocation
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetSynciv)(GLsync sync,GLenum pname,GLsizei bufSize,GLsizei* length,GLint* values);
	#define glGetSynciv _ptr_to_glGetSynciv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexEnvfv)(GLenum target,GLenum pname,GLfloat* params);
	#define glGetTexEnvfv _ptr_to_glGetTexEnvfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexEnviv)(GLenum target,GLenum pname,GLint* params);
	#define glGetTexEnviv _ptr_to_glGetTexEnviv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexGendv)(GLenum coord,GLenum pname,GLdouble* params);
	#define glGetTexGendv _ptr_to_glGetTexGendv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexGenfv)(GLenum coord,GLenum pname,GLfloat* params);
	#define glGetTexGenfv _ptr_to_glGetTexGenfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexGeniv)(GLenum coord,GLenum pname,GLint* params);
	#define glGetTexGeniv _ptr_to_glGetTexGeniv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexImage)(GLenum target,GLint level,GLenum format,GLenum type,GLvoid* pixels);
	#define glGetTexImage _ptr_to_glGetTexImage
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexLevelParameterfv)(GLenum target,GLint level,GLenum pname,GLfloat* params);
	#define glGetTexLevelParameterfv _ptr_to_glGetTexLevelParameterfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexLevelParameteriv)(GLenum target,GLint level,GLenum pname,GLint* params);
	#define glGetTexLevelParameteriv _ptr_to_glGetTexLevelParameteriv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexParameterIiv)(GLenum target,GLenum pname,GLint* params);
	#define glGetTexParameterIiv _ptr_to_glGetTexParameterIiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexParameterIuiv)(GLenum target,GLenum pname,GLuint* params);
	#define glGetTexParameterIuiv _ptr_to_glGetTexParameterIuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexParameterfv)(GLenum target,GLenum pname,GLfloat* params);
	#define glGetTexParameterfv _ptr_to_glGetTexParameterfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTexParameteriv)(GLenum target,GLenum pname,GLint* params);
	#define glGetTexParameteriv _ptr_to_glGetTexParameteriv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetTransformFeedbackVarying)(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLsizei* size,GLenum* type,GLchar* name);
	#define glGetTransformFeedbackVarying _ptr_to_glGetTransformFeedbackVarying
	FRONTIER_API extern GLuint (API_ENTRY *_ptr_to_glGetUniformBlockIndex)(GLuint program,const GLchar* uniformBlockName);
	#define glGetUniformBlockIndex _ptr_to_glGetUniformBlockIndex
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetUniformIndices)(GLuint program,GLsizei uniformCount,const GLchar*const* uniformNames,GLuint* uniformIndices);
	#define glGetUniformIndices _ptr_to_glGetUniformIndices
	FRONTIER_API extern GLint (API_ENTRY *_ptr_to_glGetUniformLocation)(GLuint program,const GLchar* name);
	#define glGetUniformLocation _ptr_to_glGetUniformLocation
	FRONTIER_API extern GLint (API_ENTRY *_ptr_to_glGetUniformLocationARB)(GLhandleARB programObj,const GLcharARB* name);
	#define glGetUniformLocationARB _ptr_to_glGetUniformLocationARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetUniformSubroutineuiv)(GLenum shadertype,GLint location,GLuint* params);
	#define glGetUniformSubroutineuiv _ptr_to_glGetUniformSubroutineuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetUniformdv)(GLuint program,GLint location,GLdouble* params);
	#define glGetUniformdv _ptr_to_glGetUniformdv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetUniformfv)(GLuint program,GLint location,GLfloat* params);
	#define glGetUniformfv _ptr_to_glGetUniformfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetUniformfvARB)(GLhandleARB programObj,GLint location,GLfloat* params);
	#define glGetUniformfvARB _ptr_to_glGetUniformfvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetUniformiv)(GLuint program,GLint location,GLint* params);
	#define glGetUniformiv _ptr_to_glGetUniformiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetUniformivARB)(GLhandleARB programObj,GLint location,GLint* params);
	#define glGetUniformivARB _ptr_to_glGetUniformivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetUniformuiv)(GLuint program,GLint location,GLuint* params);
	#define glGetUniformuiv _ptr_to_glGetUniformuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribIiv)(GLuint index,GLenum pname,GLint* params);
	#define glGetVertexAttribIiv _ptr_to_glGetVertexAttribIiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribIuiv)(GLuint index,GLenum pname,GLuint* params);
	#define glGetVertexAttribIuiv _ptr_to_glGetVertexAttribIuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribLdv)(GLuint index,GLenum pname,GLdouble* params);
	#define glGetVertexAttribLdv _ptr_to_glGetVertexAttribLdv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribPointerv)(GLuint index,GLenum pname,GLvoid** pointer);
	#define glGetVertexAttribPointerv _ptr_to_glGetVertexAttribPointerv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribPointervARB)(GLuint index,GLenum pname,GLvoid** pointer);
	#define glGetVertexAttribPointervARB _ptr_to_glGetVertexAttribPointervARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribdv)(GLuint index,GLenum pname,GLdouble* params);
	#define glGetVertexAttribdv _ptr_to_glGetVertexAttribdv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribdvARB)(GLuint index,GLenum pname,GLdouble* params);
	#define glGetVertexAttribdvARB _ptr_to_glGetVertexAttribdvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribfv)(GLuint index,GLenum pname,GLfloat* params);
	#define glGetVertexAttribfv _ptr_to_glGetVertexAttribfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribfvARB)(GLuint index,GLenum pname,GLfloat* params);
	#define glGetVertexAttribfvARB _ptr_to_glGetVertexAttribfvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribiv)(GLuint index,GLenum pname,GLint* params);
	#define glGetVertexAttribiv _ptr_to_glGetVertexAttribiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glGetVertexAttribivARB)(GLuint index,GLenum pname,GLint* params);
	#define glGetVertexAttribivARB _ptr_to_glGetVertexAttribivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glHint)(GLenum target,GLenum mode);
	#define glHint _ptr_to_glHint
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexMask)(GLuint mask);
	#define glIndexMask _ptr_to_glIndexMask
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexPointer)(GLenum type,GLsizei stride,const GLvoid* pointer);
	#define glIndexPointer _ptr_to_glIndexPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexd)(GLdouble c);
	#define glIndexd _ptr_to_glIndexd
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexdv)(const GLdouble* c);
	#define glIndexdv _ptr_to_glIndexdv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexf)(GLfloat c);
	#define glIndexf _ptr_to_glIndexf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexfv)(const GLfloat* c);
	#define glIndexfv _ptr_to_glIndexfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexi)(GLint c);
	#define glIndexi _ptr_to_glIndexi
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexiv)(const GLint* c);
	#define glIndexiv _ptr_to_glIndexiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexs)(GLshort c);
	#define glIndexs _ptr_to_glIndexs
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexsv)(const GLshort* c);
	#define glIndexsv _ptr_to_glIndexsv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexub)(GLubyte c);
	#define glIndexub _ptr_to_glIndexub
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glIndexubv)(const GLubyte* c);
	#define glIndexubv _ptr_to_glIndexubv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glInitNames)();
	#define glInitNames _ptr_to_glInitNames
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glInterleavedArrays)(GLenum format,GLsizei stride,const GLvoid* pointer);
	#define glInterleavedArrays _ptr_to_glInterleavedArrays
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsBuffer)(GLuint buffer);
	#define glIsBuffer _ptr_to_glIsBuffer
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsBufferARB)(GLuint buffer);
	#define glIsBufferARB _ptr_to_glIsBufferARB
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsEnabled)(GLenum cap);
	#define glIsEnabled _ptr_to_glIsEnabled
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsEnabledi)(GLenum target,GLuint index);
	#define glIsEnabledi _ptr_to_glIsEnabledi
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsFramebuffer)(GLuint framebuffer);
	#define glIsFramebuffer _ptr_to_glIsFramebuffer
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsFramebufferEXT)(GLuint framebuffer);
	#define glIsFramebufferEXT _ptr_to_glIsFramebufferEXT
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsList)(GLuint list);
	#define glIsList _ptr_to_glIsList
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsProgram)(GLuint program);
	#define glIsProgram _ptr_to_glIsProgram
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsProgramARB)(GLuint program);
	#define glIsProgramARB _ptr_to_glIsProgramARB
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsProgramPipeline)(GLuint pipeline);
	#define glIsProgramPipeline _ptr_to_glIsProgramPipeline
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsQuery)(GLuint id);
	#define glIsQuery _ptr_to_glIsQuery
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsRenderbuffer)(GLuint renderbuffer);
	#define glIsRenderbuffer _ptr_to_glIsRenderbuffer
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsRenderbufferEXT)(GLuint renderbuffer);
	#define glIsRenderbufferEXT _ptr_to_glIsRenderbufferEXT
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsSampler)(GLuint sampler);
	#define glIsSampler _ptr_to_glIsSampler
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsShader)(GLuint shader);
	#define glIsShader _ptr_to_glIsShader
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsSync)(GLsync sync);
	#define glIsSync _ptr_to_glIsSync
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsTexture)(GLuint texture);
	#define glIsTexture _ptr_to_glIsTexture
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsTransformFeedback)(GLuint id);
	#define glIsTransformFeedback _ptr_to_glIsTransformFeedback
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glIsVertexArray)(GLuint ren_array);
	#define glIsVertexArray _ptr_to_glIsVertexArray
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLightModelf)(GLenum pname,GLfloat param);
	#define glLightModelf _ptr_to_glLightModelf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLightModelfv)(GLenum pname,const GLfloat* params);
	#define glLightModelfv _ptr_to_glLightModelfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLightModeli)(GLenum pname,GLint param);
	#define glLightModeli _ptr_to_glLightModeli
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLightModeliv)(GLenum pname,const GLint* params);
	#define glLightModeliv _ptr_to_glLightModeliv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLightf)(GLenum light,GLenum pname,GLfloat param);
	#define glLightf _ptr_to_glLightf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLightfv)(GLenum light,GLenum pname,const GLfloat* params);
	#define glLightfv _ptr_to_glLightfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLighti)(GLenum light,GLenum pname,GLint param);
	#define glLighti _ptr_to_glLighti
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLightiv)(GLenum light,GLenum pname,const GLint* params);
	#define glLightiv _ptr_to_glLightiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLineStipple)(GLint factor,GLushort pattern);
	#define glLineStipple _ptr_to_glLineStipple
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLineWidth)(GLfloat width);
	#define glLineWidth _ptr_to_glLineWidth
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLinkProgram)(GLuint program);
	#define glLinkProgram _ptr_to_glLinkProgram
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLinkProgramARB)(GLhandleARB programObj);
	#define glLinkProgramARB _ptr_to_glLinkProgramARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glListBase)(GLuint base);
	#define glListBase _ptr_to_glListBase
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLoadIdentity)();
	#define glLoadIdentity _ptr_to_glLoadIdentity
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLoadMatrixd)(const GLdouble* m);
	#define glLoadMatrixd _ptr_to_glLoadMatrixd
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLoadMatrixf)(const GLfloat* m);
	#define glLoadMatrixf _ptr_to_glLoadMatrixf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLoadName)(GLuint name);
	#define glLoadName _ptr_to_glLoadName
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLoadTransposeMatrixd)(const GLdouble* m);
	#define glLoadTransposeMatrixd _ptr_to_glLoadTransposeMatrixd
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLoadTransposeMatrixf)(const GLfloat* m);
	#define glLoadTransposeMatrixf _ptr_to_glLoadTransposeMatrixf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glLogicOp)(GLenum opcode);
	#define glLogicOp _ptr_to_glLogicOp
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMap1d)(GLenum target,GLdouble u1,GLdouble u2,GLint stride,GLint order,const GLdouble* points);
	#define glMap1d _ptr_to_glMap1d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMap1f)(GLenum target,GLfloat u1,GLfloat u2,GLint stride,GLint order,const GLfloat* points);
	#define glMap1f _ptr_to_glMap1f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMap2d)(GLenum target,GLdouble u1,GLdouble u2,GLint ustride,GLint uorder,GLdouble v1,GLdouble v2,GLint vstride,GLint vorder,const GLdouble* points);
	#define glMap2d _ptr_to_glMap2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMap2f)(GLenum target,GLfloat u1,GLfloat u2,GLint ustride,GLint uorder,GLfloat v1,GLfloat v2,GLint vstride,GLint vorder,const GLfloat* points);
	#define glMap2f _ptr_to_glMap2f
	FRONTIER_API extern void * (API_ENTRY *_ptr_to_glMapBuffer)(GLenum target,GLenum access);
	#define glMapBuffer _ptr_to_glMapBuffer
	FRONTIER_API extern void * (API_ENTRY *_ptr_to_glMapBufferARB)(GLenum target,GLenum access);
	#define glMapBufferARB _ptr_to_glMapBufferARB
	FRONTIER_API extern void * (API_ENTRY *_ptr_to_glMapBufferRange)(GLenum target,GLintptr offset,GLsizeiptr length,GLbitfield access);
	#define glMapBufferRange _ptr_to_glMapBufferRange
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMapGrid1d)(GLint un,GLdouble u1,GLdouble u2);
	#define glMapGrid1d _ptr_to_glMapGrid1d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMapGrid1f)(GLint un,GLfloat u1,GLfloat u2);
	#define glMapGrid1f _ptr_to_glMapGrid1f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMapGrid2d)(GLint un,GLdouble u1,GLdouble u2,GLint vn,GLdouble v1,GLdouble v2);
	#define glMapGrid2d _ptr_to_glMapGrid2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMapGrid2f)(GLint un,GLfloat u1,GLfloat u2,GLint vn,GLfloat v1,GLfloat v2);
	#define glMapGrid2f _ptr_to_glMapGrid2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMaterialf)(GLenum face,GLenum pname,GLfloat param);
	#define glMaterialf _ptr_to_glMaterialf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMaterialfv)(GLenum face,GLenum pname,const GLfloat* params);
	#define glMaterialfv _ptr_to_glMaterialfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMateriali)(GLenum face,GLenum pname,GLint param);
	#define glMateriali _ptr_to_glMateriali
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMaterialiv)(GLenum face,GLenum pname,const GLint* params);
	#define glMaterialiv _ptr_to_glMaterialiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMatrixMode)(GLenum mode);
	#define glMatrixMode _ptr_to_glMatrixMode
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMemoryBarrier)(GLbitfield barriers);
	#define glMemoryBarrier _ptr_to_glMemoryBarrier
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMinSampleShading)(GLfloat value);
	#define glMinSampleShading _ptr_to_glMinSampleShading
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultMatrixd)(const GLdouble* m);
	#define glMultMatrixd _ptr_to_glMultMatrixd
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultMatrixf)(const GLfloat* m);
	#define glMultMatrixf _ptr_to_glMultMatrixf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultTransposeMatrixd)(const GLdouble* m);
	#define glMultTransposeMatrixd _ptr_to_glMultTransposeMatrixd
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultTransposeMatrixf)(const GLfloat* m);
	#define glMultTransposeMatrixf _ptr_to_glMultTransposeMatrixf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiDrawArrays)(GLenum mode,const GLint* first,const GLsizei* count,GLsizei drawcount);
	#define glMultiDrawArrays _ptr_to_glMultiDrawArrays
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiDrawElements)(GLenum mode,const GLsizei* count,GLenum type,const GLvoid*const* indices,GLsizei drawcount);
	#define glMultiDrawElements _ptr_to_glMultiDrawElements
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiDrawElementsBaseVertex)(GLenum mode,const GLsizei* count,GLenum type,const GLvoid*const* indices,GLsizei drawcount,const GLint* basevertex);
	#define glMultiDrawElementsBaseVertex _ptr_to_glMultiDrawElementsBaseVertex
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord1d)(GLenum target,GLdouble s);
	#define glMultiTexCoord1d _ptr_to_glMultiTexCoord1d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord1dv)(GLenum target,const GLdouble* v);
	#define glMultiTexCoord1dv _ptr_to_glMultiTexCoord1dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord1f)(GLenum target,GLfloat s);
	#define glMultiTexCoord1f _ptr_to_glMultiTexCoord1f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord1fv)(GLenum target,const GLfloat* v);
	#define glMultiTexCoord1fv _ptr_to_glMultiTexCoord1fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord1i)(GLenum target,GLint s);
	#define glMultiTexCoord1i _ptr_to_glMultiTexCoord1i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord1iv)(GLenum target,const GLint* v);
	#define glMultiTexCoord1iv _ptr_to_glMultiTexCoord1iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord1s)(GLenum target,GLshort s);
	#define glMultiTexCoord1s _ptr_to_glMultiTexCoord1s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord1sv)(GLenum target,const GLshort* v);
	#define glMultiTexCoord1sv _ptr_to_glMultiTexCoord1sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord2d)(GLenum target,GLdouble s,GLdouble t);
	#define glMultiTexCoord2d _ptr_to_glMultiTexCoord2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord2dv)(GLenum target,const GLdouble* v);
	#define glMultiTexCoord2dv _ptr_to_glMultiTexCoord2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord2f)(GLenum target,GLfloat s,GLfloat t);
	#define glMultiTexCoord2f _ptr_to_glMultiTexCoord2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord2fv)(GLenum target,const GLfloat* v);
	#define glMultiTexCoord2fv _ptr_to_glMultiTexCoord2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord2i)(GLenum target,GLint s,GLint t);
	#define glMultiTexCoord2i _ptr_to_glMultiTexCoord2i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord2iv)(GLenum target,const GLint* v);
	#define glMultiTexCoord2iv _ptr_to_glMultiTexCoord2iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord2s)(GLenum target,GLshort s,GLshort t);
	#define glMultiTexCoord2s _ptr_to_glMultiTexCoord2s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord2sv)(GLenum target,const GLshort* v);
	#define glMultiTexCoord2sv _ptr_to_glMultiTexCoord2sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord3d)(GLenum target,GLdouble s,GLdouble t,GLdouble r);
	#define glMultiTexCoord3d _ptr_to_glMultiTexCoord3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord3dv)(GLenum target,const GLdouble* v);
	#define glMultiTexCoord3dv _ptr_to_glMultiTexCoord3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord3f)(GLenum target,GLfloat s,GLfloat t,GLfloat r);
	#define glMultiTexCoord3f _ptr_to_glMultiTexCoord3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord3fv)(GLenum target,const GLfloat* v);
	#define glMultiTexCoord3fv _ptr_to_glMultiTexCoord3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord3i)(GLenum target,GLint s,GLint t,GLint r);
	#define glMultiTexCoord3i _ptr_to_glMultiTexCoord3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord3iv)(GLenum target,const GLint* v);
	#define glMultiTexCoord3iv _ptr_to_glMultiTexCoord3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord3s)(GLenum target,GLshort s,GLshort t,GLshort r);
	#define glMultiTexCoord3s _ptr_to_glMultiTexCoord3s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord3sv)(GLenum target,const GLshort* v);
	#define glMultiTexCoord3sv _ptr_to_glMultiTexCoord3sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord4d)(GLenum target,GLdouble s,GLdouble t,GLdouble r,GLdouble q);
	#define glMultiTexCoord4d _ptr_to_glMultiTexCoord4d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord4dv)(GLenum target,const GLdouble* v);
	#define glMultiTexCoord4dv _ptr_to_glMultiTexCoord4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord4f)(GLenum target,GLfloat s,GLfloat t,GLfloat r,GLfloat q);
	#define glMultiTexCoord4f _ptr_to_glMultiTexCoord4f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord4fv)(GLenum target,const GLfloat* v);
	#define glMultiTexCoord4fv _ptr_to_glMultiTexCoord4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord4i)(GLenum target,GLint s,GLint t,GLint r,GLint q);
	#define glMultiTexCoord4i _ptr_to_glMultiTexCoord4i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord4iv)(GLenum target,const GLint* v);
	#define glMultiTexCoord4iv _ptr_to_glMultiTexCoord4iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord4s)(GLenum target,GLshort s,GLshort t,GLshort r,GLshort q);
	#define glMultiTexCoord4s _ptr_to_glMultiTexCoord4s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoord4sv)(GLenum target,const GLshort* v);
	#define glMultiTexCoord4sv _ptr_to_glMultiTexCoord4sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoordP1ui)(GLenum texture,GLenum type,GLuint coords);
	#define glMultiTexCoordP1ui _ptr_to_glMultiTexCoordP1ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoordP1uiv)(GLenum texture,GLenum type,const GLuint* coords);
	#define glMultiTexCoordP1uiv _ptr_to_glMultiTexCoordP1uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoordP2ui)(GLenum texture,GLenum type,GLuint coords);
	#define glMultiTexCoordP2ui _ptr_to_glMultiTexCoordP2ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoordP2uiv)(GLenum texture,GLenum type,const GLuint* coords);
	#define glMultiTexCoordP2uiv _ptr_to_glMultiTexCoordP2uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoordP3ui)(GLenum texture,GLenum type,GLuint coords);
	#define glMultiTexCoordP3ui _ptr_to_glMultiTexCoordP3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoordP3uiv)(GLenum texture,GLenum type,const GLuint* coords);
	#define glMultiTexCoordP3uiv _ptr_to_glMultiTexCoordP3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoordP4ui)(GLenum texture,GLenum type,GLuint coords);
	#define glMultiTexCoordP4ui _ptr_to_glMultiTexCoordP4ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glMultiTexCoordP4uiv)(GLenum texture,GLenum type,const GLuint* coords);
	#define glMultiTexCoordP4uiv _ptr_to_glMultiTexCoordP4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNewList)(GLuint list,GLenum mode);
	#define glNewList _ptr_to_glNewList
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3b)(GLbyte nx,GLbyte ny,GLbyte nz);
	#define glNormal3b _ptr_to_glNormal3b
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3bv)(const GLbyte* v);
	#define glNormal3bv _ptr_to_glNormal3bv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3d)(GLdouble nx,GLdouble ny,GLdouble nz);
	#define glNormal3d _ptr_to_glNormal3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3dv)(const GLdouble* v);
	#define glNormal3dv _ptr_to_glNormal3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3f)(GLfloat nx,GLfloat ny,GLfloat nz);
	#define glNormal3f _ptr_to_glNormal3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3fv)(const GLfloat* v);
	#define glNormal3fv _ptr_to_glNormal3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3i)(GLint nx,GLint ny,GLint nz);
	#define glNormal3i _ptr_to_glNormal3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3iv)(const GLint* v);
	#define glNormal3iv _ptr_to_glNormal3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3s)(GLshort nx,GLshort ny,GLshort nz);
	#define glNormal3s _ptr_to_glNormal3s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormal3sv)(const GLshort* v);
	#define glNormal3sv _ptr_to_glNormal3sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormalP3ui)(GLenum type,GLuint coords);
	#define glNormalP3ui _ptr_to_glNormalP3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormalP3uiv)(GLenum type,const GLuint* coords);
	#define glNormalP3uiv _ptr_to_glNormalP3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glNormalPointer)(GLenum type,GLsizei stride,const GLvoid* pointer);
	#define glNormalPointer _ptr_to_glNormalPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glOrtho)(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar);
	#define glOrtho _ptr_to_glOrtho
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPassThrough)(GLfloat token);
	#define glPassThrough _ptr_to_glPassThrough
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPatchParameterfv)(GLenum pname,const GLfloat* values);
	#define glPatchParameterfv _ptr_to_glPatchParameterfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPatchParameteri)(GLenum pname,GLint value);
	#define glPatchParameteri _ptr_to_glPatchParameteri
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPauseTransformFeedback)();
	#define glPauseTransformFeedback _ptr_to_glPauseTransformFeedback
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPixelMapfv)(GLenum map,GLsizei mapsize,const GLfloat* values);
	#define glPixelMapfv _ptr_to_glPixelMapfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPixelMapuiv)(GLenum map,GLsizei mapsize,const GLuint* values);
	#define glPixelMapuiv _ptr_to_glPixelMapuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPixelMapusv)(GLenum map,GLsizei mapsize,const GLushort* values);
	#define glPixelMapusv _ptr_to_glPixelMapusv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPixelStoref)(GLenum pname,GLfloat param);
	#define glPixelStoref _ptr_to_glPixelStoref
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPixelStorei)(GLenum pname,GLint param);
	#define glPixelStorei _ptr_to_glPixelStorei
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPixelTransferf)(GLenum pname,GLfloat param);
	#define glPixelTransferf _ptr_to_glPixelTransferf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPixelTransferi)(GLenum pname,GLint param);
	#define glPixelTransferi _ptr_to_glPixelTransferi
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPixelZoom)(GLfloat xfactor,GLfloat yfactor);
	#define glPixelZoom _ptr_to_glPixelZoom
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPointParameterf)(GLenum pname,GLfloat param);
	#define glPointParameterf _ptr_to_glPointParameterf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPointParameterfv)(GLenum pname,const GLfloat* params);
	#define glPointParameterfv _ptr_to_glPointParameterfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPointParameteri)(GLenum pname,GLint param);
	#define glPointParameteri _ptr_to_glPointParameteri
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPointParameteriv)(GLenum pname,const GLint* params);
	#define glPointParameteriv _ptr_to_glPointParameteriv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPointSize)(GLfloat size);
	#define glPointSize _ptr_to_glPointSize
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPolygonMode)(GLenum face,GLenum mode);
	#define glPolygonMode _ptr_to_glPolygonMode
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPolygonOffset)(GLfloat factor,GLfloat units);
	#define glPolygonOffset _ptr_to_glPolygonOffset
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPolygonStipple)(const GLubyte* mask);
	#define glPolygonStipple _ptr_to_glPolygonStipple
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPopAttrib)();
	#define glPopAttrib _ptr_to_glPopAttrib
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPopClientAttrib)();
	#define glPopClientAttrib _ptr_to_glPopClientAttrib
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPopMatrix)();
	#define glPopMatrix _ptr_to_glPopMatrix
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPopName)();
	#define glPopName _ptr_to_glPopName
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPrimitiveRestartIndex)(GLuint index);
	#define glPrimitiveRestartIndex _ptr_to_glPrimitiveRestartIndex
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPrioritizeTextures)(GLsizei n,const GLuint* textures,const GLfloat* priorities);
	#define glPrioritizeTextures _ptr_to_glPrioritizeTextures
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramBinary)(GLuint program,GLenum binaryFormat,const GLvoid* binary,GLsizei length);
	#define glProgramBinary _ptr_to_glProgramBinary
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramEnvParameter4dARB)(GLenum target,GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w);
	#define glProgramEnvParameter4dARB _ptr_to_glProgramEnvParameter4dARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramEnvParameter4dvARB)(GLenum target,GLuint index,const GLdouble* params);
	#define glProgramEnvParameter4dvARB _ptr_to_glProgramEnvParameter4dvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramEnvParameter4fARB)(GLenum target,GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w);
	#define glProgramEnvParameter4fARB _ptr_to_glProgramEnvParameter4fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramEnvParameter4fvARB)(GLenum target,GLuint index,const GLfloat* params);
	#define glProgramEnvParameter4fvARB _ptr_to_glProgramEnvParameter4fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramLocalParameter4dARB)(GLenum target,GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w);
	#define glProgramLocalParameter4dARB _ptr_to_glProgramLocalParameter4dARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramLocalParameter4dvARB)(GLenum target,GLuint index,const GLdouble* params);
	#define glProgramLocalParameter4dvARB _ptr_to_glProgramLocalParameter4dvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramLocalParameter4fARB)(GLenum target,GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w);
	#define glProgramLocalParameter4fARB _ptr_to_glProgramLocalParameter4fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramLocalParameter4fvARB)(GLenum target,GLuint index,const GLfloat* params);
	#define glProgramLocalParameter4fvARB _ptr_to_glProgramLocalParameter4fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramParameteri)(GLuint program,GLenum pname,GLint value);
	#define glProgramParameteri _ptr_to_glProgramParameteri
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramStringARB)(GLenum target,GLenum format,GLsizei len,const GLvoid* string);
	#define glProgramStringARB _ptr_to_glProgramStringARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform1d)(GLuint program,GLint location,GLdouble v0);
	#define glProgramUniform1d _ptr_to_glProgramUniform1d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform1dv)(GLuint program,GLint location,GLsizei count,const GLdouble* value);
	#define glProgramUniform1dv _ptr_to_glProgramUniform1dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform1f)(GLuint program,GLint location,GLfloat v0);
	#define glProgramUniform1f _ptr_to_glProgramUniform1f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform1fv)(GLuint program,GLint location,GLsizei count,const GLfloat* value);
	#define glProgramUniform1fv _ptr_to_glProgramUniform1fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform1i)(GLuint program,GLint location,GLint v0);
	#define glProgramUniform1i _ptr_to_glProgramUniform1i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform1iv)(GLuint program,GLint location,GLsizei count,const GLint* value);
	#define glProgramUniform1iv _ptr_to_glProgramUniform1iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform1ui)(GLuint program,GLint location,GLuint v0);
	#define glProgramUniform1ui _ptr_to_glProgramUniform1ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform1uiv)(GLuint program,GLint location,GLsizei count,const GLuint* value);
	#define glProgramUniform1uiv _ptr_to_glProgramUniform1uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform2d)(GLuint program,GLint location,GLdouble v0,GLdouble v1);
	#define glProgramUniform2d _ptr_to_glProgramUniform2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform2dv)(GLuint program,GLint location,GLsizei count,const GLdouble* value);
	#define glProgramUniform2dv _ptr_to_glProgramUniform2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform2f)(GLuint program,GLint location,GLfloat v0,GLfloat v1);
	#define glProgramUniform2f _ptr_to_glProgramUniform2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform2fv)(GLuint program,GLint location,GLsizei count,const GLfloat* value);
	#define glProgramUniform2fv _ptr_to_glProgramUniform2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform2i)(GLuint program,GLint location,GLint v0,GLint v1);
	#define glProgramUniform2i _ptr_to_glProgramUniform2i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform2iv)(GLuint program,GLint location,GLsizei count,const GLint* value);
	#define glProgramUniform2iv _ptr_to_glProgramUniform2iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform2ui)(GLuint program,GLint location,GLuint v0,GLuint v1);
	#define glProgramUniform2ui _ptr_to_glProgramUniform2ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform2uiv)(GLuint program,GLint location,GLsizei count,const GLuint* value);
	#define glProgramUniform2uiv _ptr_to_glProgramUniform2uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform3d)(GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2);
	#define glProgramUniform3d _ptr_to_glProgramUniform3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform3dv)(GLuint program,GLint location,GLsizei count,const GLdouble* value);
	#define glProgramUniform3dv _ptr_to_glProgramUniform3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform3f)(GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2);
	#define glProgramUniform3f _ptr_to_glProgramUniform3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform3fv)(GLuint program,GLint location,GLsizei count,const GLfloat* value);
	#define glProgramUniform3fv _ptr_to_glProgramUniform3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform3i)(GLuint program,GLint location,GLint v0,GLint v1,GLint v2);
	#define glProgramUniform3i _ptr_to_glProgramUniform3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform3iv)(GLuint program,GLint location,GLsizei count,const GLint* value);
	#define glProgramUniform3iv _ptr_to_glProgramUniform3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform3ui)(GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2);
	#define glProgramUniform3ui _ptr_to_glProgramUniform3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform3uiv)(GLuint program,GLint location,GLsizei count,const GLuint* value);
	#define glProgramUniform3uiv _ptr_to_glProgramUniform3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform4d)(GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2,GLdouble v3);
	#define glProgramUniform4d _ptr_to_glProgramUniform4d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform4dv)(GLuint program,GLint location,GLsizei count,const GLdouble* value);
	#define glProgramUniform4dv _ptr_to_glProgramUniform4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform4f)(GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3);
	#define glProgramUniform4f _ptr_to_glProgramUniform4f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform4fv)(GLuint program,GLint location,GLsizei count,const GLfloat* value);
	#define glProgramUniform4fv _ptr_to_glProgramUniform4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform4i)(GLuint program,GLint location,GLint v0,GLint v1,GLint v2,GLint v3);
	#define glProgramUniform4i _ptr_to_glProgramUniform4i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform4iv)(GLuint program,GLint location,GLsizei count,const GLint* value);
	#define glProgramUniform4iv _ptr_to_glProgramUniform4iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform4ui)(GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3);
	#define glProgramUniform4ui _ptr_to_glProgramUniform4ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniform4uiv)(GLuint program,GLint location,GLsizei count,const GLuint* value);
	#define glProgramUniform4uiv _ptr_to_glProgramUniform4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix2dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glProgramUniformMatrix2dv _ptr_to_glProgramUniformMatrix2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix2fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glProgramUniformMatrix2fv _ptr_to_glProgramUniformMatrix2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix2x3dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glProgramUniformMatrix2x3dv _ptr_to_glProgramUniformMatrix2x3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix2x3fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glProgramUniformMatrix2x3fv _ptr_to_glProgramUniformMatrix2x3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix2x4dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glProgramUniformMatrix2x4dv _ptr_to_glProgramUniformMatrix2x4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix2x4fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glProgramUniformMatrix2x4fv _ptr_to_glProgramUniformMatrix2x4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix3dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glProgramUniformMatrix3dv _ptr_to_glProgramUniformMatrix3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix3fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glProgramUniformMatrix3fv _ptr_to_glProgramUniformMatrix3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix3x2dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glProgramUniformMatrix3x2dv _ptr_to_glProgramUniformMatrix3x2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix3x2fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glProgramUniformMatrix3x2fv _ptr_to_glProgramUniformMatrix3x2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix3x4dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glProgramUniformMatrix3x4dv _ptr_to_glProgramUniformMatrix3x4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix3x4fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glProgramUniformMatrix3x4fv _ptr_to_glProgramUniformMatrix3x4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix4dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glProgramUniformMatrix4dv _ptr_to_glProgramUniformMatrix4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix4fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glProgramUniformMatrix4fv _ptr_to_glProgramUniformMatrix4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix4x2dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glProgramUniformMatrix4x2dv _ptr_to_glProgramUniformMatrix4x2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix4x2fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glProgramUniformMatrix4x2fv _ptr_to_glProgramUniformMatrix4x2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix4x3dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glProgramUniformMatrix4x3dv _ptr_to_glProgramUniformMatrix4x3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProgramUniformMatrix4x3fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glProgramUniformMatrix4x3fv _ptr_to_glProgramUniformMatrix4x3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glProvokingVertex)(GLenum mode);
	#define glProvokingVertex _ptr_to_glProvokingVertex
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPushAttrib)(GLbitfield mask);
	#define glPushAttrib _ptr_to_glPushAttrib
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPushClientAttrib)(GLbitfield mask);
	#define glPushClientAttrib _ptr_to_glPushClientAttrib
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPushMatrix)();
	#define glPushMatrix _ptr_to_glPushMatrix
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glPushName)(GLuint name);
	#define glPushName _ptr_to_glPushName
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glQueryCounter)(GLuint id,GLenum target);
	#define glQueryCounter _ptr_to_glQueryCounter
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos2d)(GLdouble x,GLdouble y);
	#define glRasterPos2d _ptr_to_glRasterPos2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos2dv)(const GLdouble* v);
	#define glRasterPos2dv _ptr_to_glRasterPos2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos2f)(GLfloat x,GLfloat y);
	#define glRasterPos2f _ptr_to_glRasterPos2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos2fv)(const GLfloat* v);
	#define glRasterPos2fv _ptr_to_glRasterPos2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos2i)(GLint x,GLint y);
	#define glRasterPos2i _ptr_to_glRasterPos2i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos2iv)(const GLint* v);
	#define glRasterPos2iv _ptr_to_glRasterPos2iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos2s)(GLshort x,GLshort y);
	#define glRasterPos2s _ptr_to_glRasterPos2s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos2sv)(const GLshort* v);
	#define glRasterPos2sv _ptr_to_glRasterPos2sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos3d)(GLdouble x,GLdouble y,GLdouble z);
	#define glRasterPos3d _ptr_to_glRasterPos3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos3dv)(const GLdouble* v);
	#define glRasterPos3dv _ptr_to_glRasterPos3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos3f)(GLfloat x,GLfloat y,GLfloat z);
	#define glRasterPos3f _ptr_to_glRasterPos3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos3fv)(const GLfloat* v);
	#define glRasterPos3fv _ptr_to_glRasterPos3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos3i)(GLint x,GLint y,GLint z);
	#define glRasterPos3i _ptr_to_glRasterPos3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos3iv)(const GLint* v);
	#define glRasterPos3iv _ptr_to_glRasterPos3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos3s)(GLshort x,GLshort y,GLshort z);
	#define glRasterPos3s _ptr_to_glRasterPos3s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos3sv)(const GLshort* v);
	#define glRasterPos3sv _ptr_to_glRasterPos3sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos4d)(GLdouble x,GLdouble y,GLdouble z,GLdouble w);
	#define glRasterPos4d _ptr_to_glRasterPos4d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos4dv)(const GLdouble* v);
	#define glRasterPos4dv _ptr_to_glRasterPos4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos4f)(GLfloat x,GLfloat y,GLfloat z,GLfloat w);
	#define glRasterPos4f _ptr_to_glRasterPos4f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos4fv)(const GLfloat* v);
	#define glRasterPos4fv _ptr_to_glRasterPos4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos4i)(GLint x,GLint y,GLint z,GLint w);
	#define glRasterPos4i _ptr_to_glRasterPos4i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos4iv)(const GLint* v);
	#define glRasterPos4iv _ptr_to_glRasterPos4iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos4s)(GLshort x,GLshort y,GLshort z,GLshort w);
	#define glRasterPos4s _ptr_to_glRasterPos4s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRasterPos4sv)(const GLshort* v);
	#define glRasterPos4sv _ptr_to_glRasterPos4sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glReadBuffer)(GLenum mode);
	#define glReadBuffer _ptr_to_glReadBuffer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glReadPixels)(GLint x,GLint y,GLsizei width,GLsizei height,GLenum format,GLenum type,GLvoid* pixels);
	#define glReadPixels _ptr_to_glReadPixels
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRectd)(GLdouble x1,GLdouble y1,GLdouble x2,GLdouble y2);
	#define glRectd _ptr_to_glRectd
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRectdv)(const GLdouble* v1,const GLdouble* v2);
	#define glRectdv _ptr_to_glRectdv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRectf)(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2);
	#define glRectf _ptr_to_glRectf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRectfv)(const GLfloat* v1,const GLfloat* v2);
	#define glRectfv _ptr_to_glRectfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRecti)(GLint x1,GLint y1,GLint x2,GLint y2);
	#define glRecti _ptr_to_glRecti
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRectiv)(const GLint* v1,const GLint* v2);
	#define glRectiv _ptr_to_glRectiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRects)(GLshort x1,GLshort y1,GLshort x2,GLshort y2);
	#define glRects _ptr_to_glRects
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRectsv)(const GLshort* v1,const GLshort* v2);
	#define glRectsv _ptr_to_glRectsv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glReleaseShaderCompiler)();
	#define glReleaseShaderCompiler _ptr_to_glReleaseShaderCompiler
	FRONTIER_API extern GLint (API_ENTRY *_ptr_to_glRenderMode)(GLenum mode);
	#define glRenderMode _ptr_to_glRenderMode
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRenderbufferStorage)(GLenum target,GLenum internalformat,GLsizei width,GLsizei height);
	#define glRenderbufferStorage _ptr_to_glRenderbufferStorage
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRenderbufferStorageEXT)(GLenum target,GLenum internalformat,GLsizei width,GLsizei height);
	#define glRenderbufferStorageEXT _ptr_to_glRenderbufferStorageEXT
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRenderbufferStorageMultisample)(GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height);
	#define glRenderbufferStorageMultisample _ptr_to_glRenderbufferStorageMultisample
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glResumeTransformFeedback)();
	#define glResumeTransformFeedback _ptr_to_glResumeTransformFeedback
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRotated)(GLdouble angle,GLdouble x,GLdouble y,GLdouble z);
	#define glRotated _ptr_to_glRotated
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glRotatef)(GLfloat angle,GLfloat x,GLfloat y,GLfloat z);
	#define glRotatef _ptr_to_glRotatef
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSampleCoverage)(GLfloat value,GLboolean invert);
	#define glSampleCoverage _ptr_to_glSampleCoverage
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSampleMaski)(GLuint index,GLbitfield mask);
	#define glSampleMaski _ptr_to_glSampleMaski
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSamplerParameterIiv)(GLuint sampler,GLenum pname,const GLint* param);
	#define glSamplerParameterIiv _ptr_to_glSamplerParameterIiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSamplerParameterIuiv)(GLuint sampler,GLenum pname,const GLuint* param);
	#define glSamplerParameterIuiv _ptr_to_glSamplerParameterIuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSamplerParameterf)(GLuint sampler,GLenum pname,GLfloat param);
	#define glSamplerParameterf _ptr_to_glSamplerParameterf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSamplerParameterfv)(GLuint sampler,GLenum pname,const GLfloat* param);
	#define glSamplerParameterfv _ptr_to_glSamplerParameterfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSamplerParameteri)(GLuint sampler,GLenum pname,GLint param);
	#define glSamplerParameteri _ptr_to_glSamplerParameteri
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSamplerParameteriv)(GLuint sampler,GLenum pname,const GLint* param);
	#define glSamplerParameteriv _ptr_to_glSamplerParameteriv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glScaled)(GLdouble x,GLdouble y,GLdouble z);
	#define glScaled _ptr_to_glScaled
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glScalef)(GLfloat x,GLfloat y,GLfloat z);
	#define glScalef _ptr_to_glScalef
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glScissor)(GLint x,GLint y,GLsizei width,GLsizei height);
	#define glScissor _ptr_to_glScissor
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glScissorArrayv)(GLuint first,GLsizei count,const GLint* v);
	#define glScissorArrayv _ptr_to_glScissorArrayv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glScissorIndexed)(GLuint index,GLint left,GLint bottom,GLsizei width,GLsizei height);
	#define glScissorIndexed _ptr_to_glScissorIndexed
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glScissorIndexedv)(GLuint index,const GLint* v);
	#define glScissorIndexedv _ptr_to_glScissorIndexedv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3b)(GLbyte red,GLbyte green,GLbyte blue);
	#define glSecondaryColor3b _ptr_to_glSecondaryColor3b
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3bv)(const GLbyte* v);
	#define glSecondaryColor3bv _ptr_to_glSecondaryColor3bv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3d)(GLdouble red,GLdouble green,GLdouble blue);
	#define glSecondaryColor3d _ptr_to_glSecondaryColor3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3dv)(const GLdouble* v);
	#define glSecondaryColor3dv _ptr_to_glSecondaryColor3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3f)(GLfloat red,GLfloat green,GLfloat blue);
	#define glSecondaryColor3f _ptr_to_glSecondaryColor3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3fv)(const GLfloat* v);
	#define glSecondaryColor3fv _ptr_to_glSecondaryColor3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3i)(GLint red,GLint green,GLint blue);
	#define glSecondaryColor3i _ptr_to_glSecondaryColor3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3iv)(const GLint* v);
	#define glSecondaryColor3iv _ptr_to_glSecondaryColor3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3s)(GLshort red,GLshort green,GLshort blue);
	#define glSecondaryColor3s _ptr_to_glSecondaryColor3s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3sv)(const GLshort* v);
	#define glSecondaryColor3sv _ptr_to_glSecondaryColor3sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3ub)(GLubyte red,GLubyte green,GLubyte blue);
	#define glSecondaryColor3ub _ptr_to_glSecondaryColor3ub
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3ubv)(const GLubyte* v);
	#define glSecondaryColor3ubv _ptr_to_glSecondaryColor3ubv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3ui)(GLuint red,GLuint green,GLuint blue);
	#define glSecondaryColor3ui _ptr_to_glSecondaryColor3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3uiv)(const GLuint* v);
	#define glSecondaryColor3uiv _ptr_to_glSecondaryColor3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3us)(GLushort red,GLushort green,GLushort blue);
	#define glSecondaryColor3us _ptr_to_glSecondaryColor3us
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColor3usv)(const GLushort* v);
	#define glSecondaryColor3usv _ptr_to_glSecondaryColor3usv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColorP3ui)(GLenum type,GLuint color);
	#define glSecondaryColorP3ui _ptr_to_glSecondaryColorP3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColorP3uiv)(GLenum type,const GLuint* color);
	#define glSecondaryColorP3uiv _ptr_to_glSecondaryColorP3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSecondaryColorPointer)(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer);
	#define glSecondaryColorPointer _ptr_to_glSecondaryColorPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glSelectBuffer)(GLsizei size,GLuint* buffer);
	#define glSelectBuffer _ptr_to_glSelectBuffer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glShadeModel)(GLenum mode);
	#define glShadeModel _ptr_to_glShadeModel
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glShaderBinary)(GLsizei count,const GLuint* shaders,GLenum binaryformat,const GLvoid* binary,GLsizei length);
	#define glShaderBinary _ptr_to_glShaderBinary
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glShaderSource)(GLuint shader,GLsizei count,const GLchar*const* string,const GLint* length);
	#define glShaderSource _ptr_to_glShaderSource
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glShaderSourceARB)(GLhandleARB shaderObj,GLsizei count,const GLcharARB** string,const GLint* length);
	#define glShaderSourceARB _ptr_to_glShaderSourceARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glStencilFunc)(GLenum func,GLint ref,GLuint mask);
	#define glStencilFunc _ptr_to_glStencilFunc
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glStencilFuncSeparate)(GLenum face,GLenum func,GLint ref,GLuint mask);
	#define glStencilFuncSeparate _ptr_to_glStencilFuncSeparate
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glStencilMask)(GLuint mask);
	#define glStencilMask _ptr_to_glStencilMask
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glStencilMaskSeparate)(GLenum face,GLuint mask);
	#define glStencilMaskSeparate _ptr_to_glStencilMaskSeparate
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glStencilOp)(GLenum fail,GLenum zfail,GLenum zpass);
	#define glStencilOp _ptr_to_glStencilOp
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glStencilOpSeparate)(GLenum face,GLenum sfail,GLenum dpfail,GLenum dppass);
	#define glStencilOpSeparate _ptr_to_glStencilOpSeparate
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexBuffer)(GLenum target,GLenum internalformat,GLuint buffer);
	#define glTexBuffer _ptr_to_glTexBuffer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord1d)(GLdouble s);
	#define glTexCoord1d _ptr_to_glTexCoord1d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord1dv)(const GLdouble* v);
	#define glTexCoord1dv _ptr_to_glTexCoord1dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord1f)(GLfloat s);
	#define glTexCoord1f _ptr_to_glTexCoord1f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord1fv)(const GLfloat* v);
	#define glTexCoord1fv _ptr_to_glTexCoord1fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord1i)(GLint s);
	#define glTexCoord1i _ptr_to_glTexCoord1i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord1iv)(const GLint* v);
	#define glTexCoord1iv _ptr_to_glTexCoord1iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord1s)(GLshort s);
	#define glTexCoord1s _ptr_to_glTexCoord1s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord1sv)(const GLshort* v);
	#define glTexCoord1sv _ptr_to_glTexCoord1sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord2d)(GLdouble s,GLdouble t);
	#define glTexCoord2d _ptr_to_glTexCoord2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord2dv)(const GLdouble* v);
	#define glTexCoord2dv _ptr_to_glTexCoord2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord2f)(GLfloat s,GLfloat t);
	#define glTexCoord2f _ptr_to_glTexCoord2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord2fv)(const GLfloat* v);
	#define glTexCoord2fv _ptr_to_glTexCoord2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord2i)(GLint s,GLint t);
	#define glTexCoord2i _ptr_to_glTexCoord2i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord2iv)(const GLint* v);
	#define glTexCoord2iv _ptr_to_glTexCoord2iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord2s)(GLshort s,GLshort t);
	#define glTexCoord2s _ptr_to_glTexCoord2s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord2sv)(const GLshort* v);
	#define glTexCoord2sv _ptr_to_glTexCoord2sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord3d)(GLdouble s,GLdouble t,GLdouble r);
	#define glTexCoord3d _ptr_to_glTexCoord3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord3dv)(const GLdouble* v);
	#define glTexCoord3dv _ptr_to_glTexCoord3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord3f)(GLfloat s,GLfloat t,GLfloat r);
	#define glTexCoord3f _ptr_to_glTexCoord3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord3fv)(const GLfloat* v);
	#define glTexCoord3fv _ptr_to_glTexCoord3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord3i)(GLint s,GLint t,GLint r);
	#define glTexCoord3i _ptr_to_glTexCoord3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord3iv)(const GLint* v);
	#define glTexCoord3iv _ptr_to_glTexCoord3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord3s)(GLshort s,GLshort t,GLshort r);
	#define glTexCoord3s _ptr_to_glTexCoord3s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord3sv)(const GLshort* v);
	#define glTexCoord3sv _ptr_to_glTexCoord3sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord4d)(GLdouble s,GLdouble t,GLdouble r,GLdouble q);
	#define glTexCoord4d _ptr_to_glTexCoord4d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord4dv)(const GLdouble* v);
	#define glTexCoord4dv _ptr_to_glTexCoord4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord4f)(GLfloat s,GLfloat t,GLfloat r,GLfloat q);
	#define glTexCoord4f _ptr_to_glTexCoord4f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord4fv)(const GLfloat* v);
	#define glTexCoord4fv _ptr_to_glTexCoord4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord4i)(GLint s,GLint t,GLint r,GLint q);
	#define glTexCoord4i _ptr_to_glTexCoord4i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord4iv)(const GLint* v);
	#define glTexCoord4iv _ptr_to_glTexCoord4iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord4s)(GLshort s,GLshort t,GLshort r,GLshort q);
	#define glTexCoord4s _ptr_to_glTexCoord4s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoord4sv)(const GLshort* v);
	#define glTexCoord4sv _ptr_to_glTexCoord4sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoordP1ui)(GLenum type,GLuint coords);
	#define glTexCoordP1ui _ptr_to_glTexCoordP1ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoordP1uiv)(GLenum type,const GLuint* coords);
	#define glTexCoordP1uiv _ptr_to_glTexCoordP1uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoordP2ui)(GLenum type,GLuint coords);
	#define glTexCoordP2ui _ptr_to_glTexCoordP2ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoordP2uiv)(GLenum type,const GLuint* coords);
	#define glTexCoordP2uiv _ptr_to_glTexCoordP2uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoordP3ui)(GLenum type,GLuint coords);
	#define glTexCoordP3ui _ptr_to_glTexCoordP3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoordP3uiv)(GLenum type,const GLuint* coords);
	#define glTexCoordP3uiv _ptr_to_glTexCoordP3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoordP4ui)(GLenum type,GLuint coords);
	#define glTexCoordP4ui _ptr_to_glTexCoordP4ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoordP4uiv)(GLenum type,const GLuint* coords);
	#define glTexCoordP4uiv _ptr_to_glTexCoordP4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexCoordPointer)(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer);
	#define glTexCoordPointer _ptr_to_glTexCoordPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexEnvf)(GLenum target,GLenum pname,GLfloat param);
	#define glTexEnvf _ptr_to_glTexEnvf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexEnvfv)(GLenum target,GLenum pname,const GLfloat* params);
	#define glTexEnvfv _ptr_to_glTexEnvfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexEnvi)(GLenum target,GLenum pname,GLint param);
	#define glTexEnvi _ptr_to_glTexEnvi
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexEnviv)(GLenum target,GLenum pname,const GLint* params);
	#define glTexEnviv _ptr_to_glTexEnviv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexGend)(GLenum coord,GLenum pname,GLdouble param);
	#define glTexGend _ptr_to_glTexGend
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexGendv)(GLenum coord,GLenum pname,const GLdouble* params);
	#define glTexGendv _ptr_to_glTexGendv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexGenf)(GLenum coord,GLenum pname,GLfloat param);
	#define glTexGenf _ptr_to_glTexGenf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexGenfv)(GLenum coord,GLenum pname,const GLfloat* params);
	#define glTexGenfv _ptr_to_glTexGenfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexGeni)(GLenum coord,GLenum pname,GLint param);
	#define glTexGeni _ptr_to_glTexGeni
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexGeniv)(GLenum coord,GLenum pname,const GLint* params);
	#define glTexGeniv _ptr_to_glTexGeniv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexImage1D)(GLenum target,GLint level,GLint internalformat,GLsizei width,GLint border,GLenum format,GLenum type,const GLvoid* pixels);
	#define glTexImage1D _ptr_to_glTexImage1D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexImage2D)(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,GLenum format,GLenum type,const GLvoid* pixels);
	#define glTexImage2D _ptr_to_glTexImage2D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexImage2DMultisample)(GLenum target,GLsizei samples,GLint internalformat,GLsizei width,GLsizei height,GLboolean fixedsamplelocations);
	#define glTexImage2DMultisample _ptr_to_glTexImage2DMultisample
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexImage3D)(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLenum format,GLenum type,const GLvoid* pixels);
	#define glTexImage3D _ptr_to_glTexImage3D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexImage3DMultisample)(GLenum target,GLsizei samples,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLboolean fixedsamplelocations);
	#define glTexImage3DMultisample _ptr_to_glTexImage3DMultisample
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexParameterIiv)(GLenum target,GLenum pname,const GLint* params);
	#define glTexParameterIiv _ptr_to_glTexParameterIiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexParameterIuiv)(GLenum target,GLenum pname,const GLuint* params);
	#define glTexParameterIuiv _ptr_to_glTexParameterIuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexParameterf)(GLenum target,GLenum pname,GLfloat param);
	#define glTexParameterf _ptr_to_glTexParameterf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexParameterfv)(GLenum target,GLenum pname,const GLfloat* params);
	#define glTexParameterfv _ptr_to_glTexParameterfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexParameteri)(GLenum target,GLenum pname,GLint param);
	#define glTexParameteri _ptr_to_glTexParameteri
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexParameteriv)(GLenum target,GLenum pname,const GLint* params);
	#define glTexParameteriv _ptr_to_glTexParameteriv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexStorage1D)(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width);
	#define glTexStorage1D _ptr_to_glTexStorage1D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexStorage2D)(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height);
	#define glTexStorage2D _ptr_to_glTexStorage2D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexStorage3D)(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth);
	#define glTexStorage3D _ptr_to_glTexStorage3D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexSubImage1D)(GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLenum type,const GLvoid* pixels);
	#define glTexSubImage1D _ptr_to_glTexSubImage1D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexSubImage2D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLenum type,const GLvoid* pixels);
	#define glTexSubImage2D _ptr_to_glTexSubImage2D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTexSubImage3D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,const GLvoid* pixels);
	#define glTexSubImage3D _ptr_to_glTexSubImage3D
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTransformFeedbackVaryings)(GLuint program,GLsizei count,const GLchar*const* varyings,GLenum bufferMode);
	#define glTransformFeedbackVaryings _ptr_to_glTransformFeedbackVaryings
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTranslated)(GLdouble x,GLdouble y,GLdouble z);
	#define glTranslated _ptr_to_glTranslated
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glTranslatef)(GLfloat x,GLfloat y,GLfloat z);
	#define glTranslatef _ptr_to_glTranslatef
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1d)(GLint location,GLdouble x);
	#define glUniform1d _ptr_to_glUniform1d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1dv)(GLint location,GLsizei count,const GLdouble* value);
	#define glUniform1dv _ptr_to_glUniform1dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1f)(GLint location,GLfloat v0);
	#define glUniform1f _ptr_to_glUniform1f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1fARB)(GLint location,GLfloat v0);
	#define glUniform1fARB _ptr_to_glUniform1fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1fv)(GLint location,GLsizei count,const GLfloat* value);
	#define glUniform1fv _ptr_to_glUniform1fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1fvARB)(GLint location,GLsizei count,const GLfloat* value);
	#define glUniform1fvARB _ptr_to_glUniform1fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1i)(GLint location,GLint v0);
	#define glUniform1i _ptr_to_glUniform1i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1iARB)(GLint location,GLint v0);
	#define glUniform1iARB _ptr_to_glUniform1iARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1iv)(GLint location,GLsizei count,const GLint* value);
	#define glUniform1iv _ptr_to_glUniform1iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1ivARB)(GLint location,GLsizei count,const GLint* value);
	#define glUniform1ivARB _ptr_to_glUniform1ivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1ui)(GLint location,GLuint v0);
	#define glUniform1ui _ptr_to_glUniform1ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform1uiv)(GLint location,GLsizei count,const GLuint* value);
	#define glUniform1uiv _ptr_to_glUniform1uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2d)(GLint location,GLdouble x,GLdouble y);
	#define glUniform2d _ptr_to_glUniform2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2dv)(GLint location,GLsizei count,const GLdouble* value);
	#define glUniform2dv _ptr_to_glUniform2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2f)(GLint location,GLfloat v0,GLfloat v1);
	#define glUniform2f _ptr_to_glUniform2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2fARB)(GLint location,GLfloat v0,GLfloat v1);
	#define glUniform2fARB _ptr_to_glUniform2fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2fv)(GLint location,GLsizei count,const GLfloat* value);
	#define glUniform2fv _ptr_to_glUniform2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2fvARB)(GLint location,GLsizei count,const GLfloat* value);
	#define glUniform2fvARB _ptr_to_glUniform2fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2i)(GLint location,GLint v0,GLint v1);
	#define glUniform2i _ptr_to_glUniform2i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2iARB)(GLint location,GLint v0,GLint v1);
	#define glUniform2iARB _ptr_to_glUniform2iARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2iv)(GLint location,GLsizei count,const GLint* value);
	#define glUniform2iv _ptr_to_glUniform2iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2ivARB)(GLint location,GLsizei count,const GLint* value);
	#define glUniform2ivARB _ptr_to_glUniform2ivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2ui)(GLint location,GLuint v0,GLuint v1);
	#define glUniform2ui _ptr_to_glUniform2ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform2uiv)(GLint location,GLsizei count,const GLuint* value);
	#define glUniform2uiv _ptr_to_glUniform2uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3d)(GLint location,GLdouble x,GLdouble y,GLdouble z);
	#define glUniform3d _ptr_to_glUniform3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3dv)(GLint location,GLsizei count,const GLdouble* value);
	#define glUniform3dv _ptr_to_glUniform3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3f)(GLint location,GLfloat v0,GLfloat v1,GLfloat v2);
	#define glUniform3f _ptr_to_glUniform3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3fARB)(GLint location,GLfloat v0,GLfloat v1,GLfloat v2);
	#define glUniform3fARB _ptr_to_glUniform3fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3fv)(GLint location,GLsizei count,const GLfloat* value);
	#define glUniform3fv _ptr_to_glUniform3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3fvARB)(GLint location,GLsizei count,const GLfloat* value);
	#define glUniform3fvARB _ptr_to_glUniform3fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3i)(GLint location,GLint v0,GLint v1,GLint v2);
	#define glUniform3i _ptr_to_glUniform3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3iARB)(GLint location,GLint v0,GLint v1,GLint v2);
	#define glUniform3iARB _ptr_to_glUniform3iARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3iv)(GLint location,GLsizei count,const GLint* value);
	#define glUniform3iv _ptr_to_glUniform3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3ivARB)(GLint location,GLsizei count,const GLint* value);
	#define glUniform3ivARB _ptr_to_glUniform3ivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3ui)(GLint location,GLuint v0,GLuint v1,GLuint v2);
	#define glUniform3ui _ptr_to_glUniform3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform3uiv)(GLint location,GLsizei count,const GLuint* value);
	#define glUniform3uiv _ptr_to_glUniform3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4d)(GLint location,GLdouble x,GLdouble y,GLdouble z,GLdouble w);
	#define glUniform4d _ptr_to_glUniform4d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4dv)(GLint location,GLsizei count,const GLdouble* value);
	#define glUniform4dv _ptr_to_glUniform4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4f)(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3);
	#define glUniform4f _ptr_to_glUniform4f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4fARB)(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3);
	#define glUniform4fARB _ptr_to_glUniform4fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4fv)(GLint location,GLsizei count,const GLfloat* value);
	#define glUniform4fv _ptr_to_glUniform4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4fvARB)(GLint location,GLsizei count,const GLfloat* value);
	#define glUniform4fvARB _ptr_to_glUniform4fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4i)(GLint location,GLint v0,GLint v1,GLint v2,GLint v3);
	#define glUniform4i _ptr_to_glUniform4i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4iARB)(GLint location,GLint v0,GLint v1,GLint v2,GLint v3);
	#define glUniform4iARB _ptr_to_glUniform4iARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4iv)(GLint location,GLsizei count,const GLint* value);
	#define glUniform4iv _ptr_to_glUniform4iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4ivARB)(GLint location,GLsizei count,const GLint* value);
	#define glUniform4ivARB _ptr_to_glUniform4ivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4ui)(GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3);
	#define glUniform4ui _ptr_to_glUniform4ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniform4uiv)(GLint location,GLsizei count,const GLuint* value);
	#define glUniform4uiv _ptr_to_glUniform4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformBlockBinding)(GLuint program,GLuint uniformBlockIndex,GLuint uniformBlockBinding);
	#define glUniformBlockBinding _ptr_to_glUniformBlockBinding
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix2dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glUniformMatrix2dv _ptr_to_glUniformMatrix2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix2fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix2fv _ptr_to_glUniformMatrix2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix2fvARB)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix2fvARB _ptr_to_glUniformMatrix2fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix2x3dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glUniformMatrix2x3dv _ptr_to_glUniformMatrix2x3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix2x3fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix2x3fv _ptr_to_glUniformMatrix2x3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix2x4dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glUniformMatrix2x4dv _ptr_to_glUniformMatrix2x4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix2x4fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix2x4fv _ptr_to_glUniformMatrix2x4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix3dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glUniformMatrix3dv _ptr_to_glUniformMatrix3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix3fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix3fv _ptr_to_glUniformMatrix3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix3fvARB)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix3fvARB _ptr_to_glUniformMatrix3fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix3x2dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glUniformMatrix3x2dv _ptr_to_glUniformMatrix3x2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix3x2fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix3x2fv _ptr_to_glUniformMatrix3x2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix3x4dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glUniformMatrix3x4dv _ptr_to_glUniformMatrix3x4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix3x4fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix3x4fv _ptr_to_glUniformMatrix3x4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix4dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glUniformMatrix4dv _ptr_to_glUniformMatrix4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix4fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix4fv _ptr_to_glUniformMatrix4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix4fvARB)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix4fvARB _ptr_to_glUniformMatrix4fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix4x2dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glUniformMatrix4x2dv _ptr_to_glUniformMatrix4x2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix4x2fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix4x2fv _ptr_to_glUniformMatrix4x2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix4x3dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value);
	#define glUniformMatrix4x3dv _ptr_to_glUniformMatrix4x3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformMatrix4x3fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
	#define glUniformMatrix4x3fv _ptr_to_glUniformMatrix4x3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUniformSubroutinesuiv)(GLenum shadertype,GLsizei count,const GLuint* indices);
	#define glUniformSubroutinesuiv _ptr_to_glUniformSubroutinesuiv
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glUnmapBuffer)(GLenum target);
	#define glUnmapBuffer _ptr_to_glUnmapBuffer
	FRONTIER_API extern GLboolean (API_ENTRY *_ptr_to_glUnmapBufferARB)(GLenum target);
	#define glUnmapBufferARB _ptr_to_glUnmapBufferARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUseProgram)(GLuint program);
	#define glUseProgram _ptr_to_glUseProgram
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUseProgramObjectARB)(GLhandleARB programObj);
	#define glUseProgramObjectARB _ptr_to_glUseProgramObjectARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glUseProgramStages)(GLuint pipeline,GLbitfield stages,GLuint program);
	#define glUseProgramStages _ptr_to_glUseProgramStages
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glValidateProgram)(GLuint program);
	#define glValidateProgram _ptr_to_glValidateProgram
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glValidateProgramARB)(GLhandleARB programObj);
	#define glValidateProgramARB _ptr_to_glValidateProgramARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glValidateProgramPipeline)(GLuint pipeline);
	#define glValidateProgramPipeline _ptr_to_glValidateProgramPipeline
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex2d)(GLdouble x,GLdouble y);
	#define glVertex2d _ptr_to_glVertex2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex2dv)(const GLdouble* v);
	#define glVertex2dv _ptr_to_glVertex2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex2f)(GLfloat x,GLfloat y);
	#define glVertex2f _ptr_to_glVertex2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex2fv)(const GLfloat* v);
	#define glVertex2fv _ptr_to_glVertex2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex2i)(GLint x,GLint y);
	#define glVertex2i _ptr_to_glVertex2i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex2iv)(const GLint* v);
	#define glVertex2iv _ptr_to_glVertex2iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex2s)(GLshort x,GLshort y);
	#define glVertex2s _ptr_to_glVertex2s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex2sv)(const GLshort* v);
	#define glVertex2sv _ptr_to_glVertex2sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex3d)(GLdouble x,GLdouble y,GLdouble z);
	#define glVertex3d _ptr_to_glVertex3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex3dv)(const GLdouble* v);
	#define glVertex3dv _ptr_to_glVertex3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex3f)(GLfloat x,GLfloat y,GLfloat z);
	#define glVertex3f _ptr_to_glVertex3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex3fv)(const GLfloat* v);
	#define glVertex3fv _ptr_to_glVertex3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex3i)(GLint x,GLint y,GLint z);
	#define glVertex3i _ptr_to_glVertex3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex3iv)(const GLint* v);
	#define glVertex3iv _ptr_to_glVertex3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex3s)(GLshort x,GLshort y,GLshort z);
	#define glVertex3s _ptr_to_glVertex3s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex3sv)(const GLshort* v);
	#define glVertex3sv _ptr_to_glVertex3sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex4d)(GLdouble x,GLdouble y,GLdouble z,GLdouble w);
	#define glVertex4d _ptr_to_glVertex4d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex4dv)(const GLdouble* v);
	#define glVertex4dv _ptr_to_glVertex4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex4f)(GLfloat x,GLfloat y,GLfloat z,GLfloat w);
	#define glVertex4f _ptr_to_glVertex4f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex4fv)(const GLfloat* v);
	#define glVertex4fv _ptr_to_glVertex4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex4i)(GLint x,GLint y,GLint z,GLint w);
	#define glVertex4i _ptr_to_glVertex4i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex4iv)(const GLint* v);
	#define glVertex4iv _ptr_to_glVertex4iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex4s)(GLshort x,GLshort y,GLshort z,GLshort w);
	#define glVertex4s _ptr_to_glVertex4s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertex4sv)(const GLshort* v);
	#define glVertex4sv _ptr_to_glVertex4sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1d)(GLuint index,GLdouble x);
	#define glVertexAttrib1d _ptr_to_glVertexAttrib1d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1dARB)(GLuint index,GLdouble v0);
	#define glVertexAttrib1dARB _ptr_to_glVertexAttrib1dARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1dv)(GLuint index,const GLdouble* v);
	#define glVertexAttrib1dv _ptr_to_glVertexAttrib1dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1dvARB)(GLuint index,const GLdouble* v);
	#define glVertexAttrib1dvARB _ptr_to_glVertexAttrib1dvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1f)(GLuint index,GLfloat x);
	#define glVertexAttrib1f _ptr_to_glVertexAttrib1f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1fARB)(GLuint index,GLfloat v0);
	#define glVertexAttrib1fARB _ptr_to_glVertexAttrib1fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1fv)(GLuint index,const GLfloat* v);
	#define glVertexAttrib1fv _ptr_to_glVertexAttrib1fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1fvARB)(GLuint index,const GLfloat* v);
	#define glVertexAttrib1fvARB _ptr_to_glVertexAttrib1fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1s)(GLuint index,GLshort x);
	#define glVertexAttrib1s _ptr_to_glVertexAttrib1s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1sARB)(GLuint index,GLshort v0);
	#define glVertexAttrib1sARB _ptr_to_glVertexAttrib1sARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1sv)(GLuint index,const GLshort* v);
	#define glVertexAttrib1sv _ptr_to_glVertexAttrib1sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib1svARB)(GLuint index,const GLshort* v);
	#define glVertexAttrib1svARB _ptr_to_glVertexAttrib1svARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2d)(GLuint index,GLdouble x,GLdouble y);
	#define glVertexAttrib2d _ptr_to_glVertexAttrib2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2dARB)(GLuint index,GLdouble v0,GLdouble v1);
	#define glVertexAttrib2dARB _ptr_to_glVertexAttrib2dARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2dv)(GLuint index,const GLdouble* v);
	#define glVertexAttrib2dv _ptr_to_glVertexAttrib2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2dvARB)(GLuint index,const GLdouble* v);
	#define glVertexAttrib2dvARB _ptr_to_glVertexAttrib2dvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2f)(GLuint index,GLfloat x,GLfloat y);
	#define glVertexAttrib2f _ptr_to_glVertexAttrib2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2fARB)(GLuint index,GLfloat v0,GLfloat v1);
	#define glVertexAttrib2fARB _ptr_to_glVertexAttrib2fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2fv)(GLuint index,const GLfloat* v);
	#define glVertexAttrib2fv _ptr_to_glVertexAttrib2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2fvARB)(GLuint index,const GLfloat* v);
	#define glVertexAttrib2fvARB _ptr_to_glVertexAttrib2fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2s)(GLuint index,GLshort x,GLshort y);
	#define glVertexAttrib2s _ptr_to_glVertexAttrib2s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2sARB)(GLuint index,GLshort v0,GLshort v1);
	#define glVertexAttrib2sARB _ptr_to_glVertexAttrib2sARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2sv)(GLuint index,const GLshort* v);
	#define glVertexAttrib2sv _ptr_to_glVertexAttrib2sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib2svARB)(GLuint index,const GLshort* v);
	#define glVertexAttrib2svARB _ptr_to_glVertexAttrib2svARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3d)(GLuint index,GLdouble x,GLdouble y,GLdouble z);
	#define glVertexAttrib3d _ptr_to_glVertexAttrib3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3dARB)(GLuint index,GLdouble v0,GLdouble v1,GLdouble v2);
	#define glVertexAttrib3dARB _ptr_to_glVertexAttrib3dARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3dv)(GLuint index,const GLdouble* v);
	#define glVertexAttrib3dv _ptr_to_glVertexAttrib3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3dvARB)(GLuint index,const GLdouble* v);
	#define glVertexAttrib3dvARB _ptr_to_glVertexAttrib3dvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3f)(GLuint index,GLfloat x,GLfloat y,GLfloat z);
	#define glVertexAttrib3f _ptr_to_glVertexAttrib3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3fARB)(GLuint index,GLfloat v0,GLfloat v1,GLfloat v2);
	#define glVertexAttrib3fARB _ptr_to_glVertexAttrib3fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3fv)(GLuint index,const GLfloat* v);
	#define glVertexAttrib3fv _ptr_to_glVertexAttrib3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3fvARB)(GLuint index,const GLfloat* v);
	#define glVertexAttrib3fvARB _ptr_to_glVertexAttrib3fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3s)(GLuint index,GLshort x,GLshort y,GLshort z);
	#define glVertexAttrib3s _ptr_to_glVertexAttrib3s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3sARB)(GLuint index,GLshort v0,GLshort v1,GLshort v2);
	#define glVertexAttrib3sARB _ptr_to_glVertexAttrib3sARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3sv)(GLuint index,const GLshort* v);
	#define glVertexAttrib3sv _ptr_to_glVertexAttrib3sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib3svARB)(GLuint index,const GLshort* v);
	#define glVertexAttrib3svARB _ptr_to_glVertexAttrib3svARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4Nbv)(GLuint index,const GLbyte* v);
	#define glVertexAttrib4Nbv _ptr_to_glVertexAttrib4Nbv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4NbvARB)(GLuint index,const GLbyte* v);
	#define glVertexAttrib4NbvARB _ptr_to_glVertexAttrib4NbvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4Niv)(GLuint index,const GLint* v);
	#define glVertexAttrib4Niv _ptr_to_glVertexAttrib4Niv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4NivARB)(GLuint index,const GLint* v);
	#define glVertexAttrib4NivARB _ptr_to_glVertexAttrib4NivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4Nsv)(GLuint index,const GLshort* v);
	#define glVertexAttrib4Nsv _ptr_to_glVertexAttrib4Nsv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4NsvARB)(GLuint index,const GLshort* v);
	#define glVertexAttrib4NsvARB _ptr_to_glVertexAttrib4NsvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4Nub)(GLuint index,GLubyte x,GLubyte y,GLubyte z,GLubyte w);
	#define glVertexAttrib4Nub _ptr_to_glVertexAttrib4Nub
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4NubARB)(GLuint index,GLubyte x,GLubyte y,GLubyte z,GLubyte w);
	#define glVertexAttrib4NubARB _ptr_to_glVertexAttrib4NubARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4Nubv)(GLuint index,const GLubyte* v);
	#define glVertexAttrib4Nubv _ptr_to_glVertexAttrib4Nubv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4NubvARB)(GLuint index,const GLubyte* v);
	#define glVertexAttrib4NubvARB _ptr_to_glVertexAttrib4NubvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4Nuiv)(GLuint index,const GLuint* v);
	#define glVertexAttrib4Nuiv _ptr_to_glVertexAttrib4Nuiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4NuivARB)(GLuint index,const GLuint* v);
	#define glVertexAttrib4NuivARB _ptr_to_glVertexAttrib4NuivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4Nusv)(GLuint index,const GLushort* v);
	#define glVertexAttrib4Nusv _ptr_to_glVertexAttrib4Nusv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4NusvARB)(GLuint index,const GLushort* v);
	#define glVertexAttrib4NusvARB _ptr_to_glVertexAttrib4NusvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4bv)(GLuint index,const GLbyte* v);
	#define glVertexAttrib4bv _ptr_to_glVertexAttrib4bv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4bvARB)(GLuint index,const GLbyte* v);
	#define glVertexAttrib4bvARB _ptr_to_glVertexAttrib4bvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4d)(GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w);
	#define glVertexAttrib4d _ptr_to_glVertexAttrib4d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4dARB)(GLuint index,GLdouble v0,GLdouble v1,GLdouble v2,GLdouble v3);
	#define glVertexAttrib4dARB _ptr_to_glVertexAttrib4dARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4dv)(GLuint index,const GLdouble* v);
	#define glVertexAttrib4dv _ptr_to_glVertexAttrib4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4dvARB)(GLuint index,const GLdouble* v);
	#define glVertexAttrib4dvARB _ptr_to_glVertexAttrib4dvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4f)(GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w);
	#define glVertexAttrib4f _ptr_to_glVertexAttrib4f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4fARB)(GLuint index,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3);
	#define glVertexAttrib4fARB _ptr_to_glVertexAttrib4fARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4fv)(GLuint index,const GLfloat* v);
	#define glVertexAttrib4fv _ptr_to_glVertexAttrib4fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4fvARB)(GLuint index,const GLfloat* v);
	#define glVertexAttrib4fvARB _ptr_to_glVertexAttrib4fvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4iv)(GLuint index,const GLint* v);
	#define glVertexAttrib4iv _ptr_to_glVertexAttrib4iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4ivARB)(GLuint index,const GLint* v);
	#define glVertexAttrib4ivARB _ptr_to_glVertexAttrib4ivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4s)(GLuint index,GLshort x,GLshort y,GLshort z,GLshort w);
	#define glVertexAttrib4s _ptr_to_glVertexAttrib4s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4sARB)(GLuint index,GLshort v0,GLshort v1,GLshort v2,GLshort v3);
	#define glVertexAttrib4sARB _ptr_to_glVertexAttrib4sARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4sv)(GLuint index,const GLshort* v);
	#define glVertexAttrib4sv _ptr_to_glVertexAttrib4sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4svARB)(GLuint index,const GLshort* v);
	#define glVertexAttrib4svARB _ptr_to_glVertexAttrib4svARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4ubv)(GLuint index,const GLubyte* v);
	#define glVertexAttrib4ubv _ptr_to_glVertexAttrib4ubv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4ubvARB)(GLuint index,const GLubyte* v);
	#define glVertexAttrib4ubvARB _ptr_to_glVertexAttrib4ubvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4uiv)(GLuint index,const GLuint* v);
	#define glVertexAttrib4uiv _ptr_to_glVertexAttrib4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4uivARB)(GLuint index,const GLuint* v);
	#define glVertexAttrib4uivARB _ptr_to_glVertexAttrib4uivARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4usv)(GLuint index,const GLushort* v);
	#define glVertexAttrib4usv _ptr_to_glVertexAttrib4usv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttrib4usvARB)(GLuint index,const GLushort* v);
	#define glVertexAttrib4usvARB _ptr_to_glVertexAttrib4usvARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribDivisor)(GLuint index,GLuint divisor);
	#define glVertexAttribDivisor _ptr_to_glVertexAttribDivisor
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI1i)(GLuint index,GLint x);
	#define glVertexAttribI1i _ptr_to_glVertexAttribI1i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI1iv)(GLuint index,const GLint* v);
	#define glVertexAttribI1iv _ptr_to_glVertexAttribI1iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI1ui)(GLuint index,GLuint x);
	#define glVertexAttribI1ui _ptr_to_glVertexAttribI1ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI1uiv)(GLuint index,const GLuint* v);
	#define glVertexAttribI1uiv _ptr_to_glVertexAttribI1uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI2i)(GLuint index,GLint x,GLint y);
	#define glVertexAttribI2i _ptr_to_glVertexAttribI2i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI2iv)(GLuint index,const GLint* v);
	#define glVertexAttribI2iv _ptr_to_glVertexAttribI2iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI2ui)(GLuint index,GLuint x,GLuint y);
	#define glVertexAttribI2ui _ptr_to_glVertexAttribI2ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI2uiv)(GLuint index,const GLuint* v);
	#define glVertexAttribI2uiv _ptr_to_glVertexAttribI2uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI3i)(GLuint index,GLint x,GLint y,GLint z);
	#define glVertexAttribI3i _ptr_to_glVertexAttribI3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI3iv)(GLuint index,const GLint* v);
	#define glVertexAttribI3iv _ptr_to_glVertexAttribI3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI3ui)(GLuint index,GLuint x,GLuint y,GLuint z);
	#define glVertexAttribI3ui _ptr_to_glVertexAttribI3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI3uiv)(GLuint index,const GLuint* v);
	#define glVertexAttribI3uiv _ptr_to_glVertexAttribI3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI4bv)(GLuint index,const GLbyte* v);
	#define glVertexAttribI4bv _ptr_to_glVertexAttribI4bv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI4i)(GLuint index,GLint x,GLint y,GLint z,GLint w);
	#define glVertexAttribI4i _ptr_to_glVertexAttribI4i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI4iv)(GLuint index,const GLint* v);
	#define glVertexAttribI4iv _ptr_to_glVertexAttribI4iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI4sv)(GLuint index,const GLshort* v);
	#define glVertexAttribI4sv _ptr_to_glVertexAttribI4sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI4ubv)(GLuint index,const GLubyte* v);
	#define glVertexAttribI4ubv _ptr_to_glVertexAttribI4ubv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI4ui)(GLuint index,GLuint x,GLuint y,GLuint z,GLuint w);
	#define glVertexAttribI4ui _ptr_to_glVertexAttribI4ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI4uiv)(GLuint index,const GLuint* v);
	#define glVertexAttribI4uiv _ptr_to_glVertexAttribI4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribI4usv)(GLuint index,const GLushort* v);
	#define glVertexAttribI4usv _ptr_to_glVertexAttribI4usv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribIPointer)(GLuint index,GLint size,GLenum type,GLsizei stride,const GLvoid* pointer);
	#define glVertexAttribIPointer _ptr_to_glVertexAttribIPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribL1d)(GLuint index,GLdouble x);
	#define glVertexAttribL1d _ptr_to_glVertexAttribL1d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribL1dv)(GLuint index,const GLdouble* v);
	#define glVertexAttribL1dv _ptr_to_glVertexAttribL1dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribL2d)(GLuint index,GLdouble x,GLdouble y);
	#define glVertexAttribL2d _ptr_to_glVertexAttribL2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribL2dv)(GLuint index,const GLdouble* v);
	#define glVertexAttribL2dv _ptr_to_glVertexAttribL2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribL3d)(GLuint index,GLdouble x,GLdouble y,GLdouble z);
	#define glVertexAttribL3d _ptr_to_glVertexAttribL3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribL3dv)(GLuint index,const GLdouble* v);
	#define glVertexAttribL3dv _ptr_to_glVertexAttribL3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribL4d)(GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w);
	#define glVertexAttribL4d _ptr_to_glVertexAttribL4d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribL4dv)(GLuint index,const GLdouble* v);
	#define glVertexAttribL4dv _ptr_to_glVertexAttribL4dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribLPointer)(GLuint index,GLint size,GLenum type,GLsizei stride,const GLvoid* pointer);
	#define glVertexAttribLPointer _ptr_to_glVertexAttribLPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribP1ui)(GLuint index,GLenum type,GLboolean normalized,GLuint value);
	#define glVertexAttribP1ui _ptr_to_glVertexAttribP1ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribP1uiv)(GLuint index,GLenum type,GLboolean normalized,const GLuint* value);
	#define glVertexAttribP1uiv _ptr_to_glVertexAttribP1uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribP2ui)(GLuint index,GLenum type,GLboolean normalized,GLuint value);
	#define glVertexAttribP2ui _ptr_to_glVertexAttribP2ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribP2uiv)(GLuint index,GLenum type,GLboolean normalized,const GLuint* value);
	#define glVertexAttribP2uiv _ptr_to_glVertexAttribP2uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribP3ui)(GLuint index,GLenum type,GLboolean normalized,GLuint value);
	#define glVertexAttribP3ui _ptr_to_glVertexAttribP3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribP3uiv)(GLuint index,GLenum type,GLboolean normalized,const GLuint* value);
	#define glVertexAttribP3uiv _ptr_to_glVertexAttribP3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribP4ui)(GLuint index,GLenum type,GLboolean normalized,GLuint value);
	#define glVertexAttribP4ui _ptr_to_glVertexAttribP4ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribP4uiv)(GLuint index,GLenum type,GLboolean normalized,const GLuint* value);
	#define glVertexAttribP4uiv _ptr_to_glVertexAttribP4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribPointer)(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const GLvoid* pointer);
	#define glVertexAttribPointer _ptr_to_glVertexAttribPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexAttribPointerARB)(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const GLvoid* pointer);
	#define glVertexAttribPointerARB _ptr_to_glVertexAttribPointerARB
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexP2ui)(GLenum type,GLuint value);
	#define glVertexP2ui _ptr_to_glVertexP2ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexP2uiv)(GLenum type,const GLuint* value);
	#define glVertexP2uiv _ptr_to_glVertexP2uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexP3ui)(GLenum type,GLuint value);
	#define glVertexP3ui _ptr_to_glVertexP3ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexP3uiv)(GLenum type,const GLuint* value);
	#define glVertexP3uiv _ptr_to_glVertexP3uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexP4ui)(GLenum type,GLuint value);
	#define glVertexP4ui _ptr_to_glVertexP4ui
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexP4uiv)(GLenum type,const GLuint* value);
	#define glVertexP4uiv _ptr_to_glVertexP4uiv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glVertexPointer)(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer);
	#define glVertexPointer _ptr_to_glVertexPointer
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glViewport)(GLint x,GLint y,GLsizei width,GLsizei height);
	#define glViewport _ptr_to_glViewport
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glViewportArrayv)(GLuint first,GLsizei count,const GLfloat* v);
	#define glViewportArrayv _ptr_to_glViewportArrayv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glViewportIndexedf)(GLuint index,GLfloat x,GLfloat y,GLfloat w,GLfloat h);
	#define glViewportIndexedf _ptr_to_glViewportIndexedf
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glViewportIndexedfv)(GLuint index,const GLfloat* v);
	#define glViewportIndexedfv _ptr_to_glViewportIndexedfv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWaitSync)(GLsync sync,GLbitfield flags,GLuint64 timeout);
	#define glWaitSync _ptr_to_glWaitSync
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos2d)(GLdouble x,GLdouble y);
	#define glWindowPos2d _ptr_to_glWindowPos2d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos2dv)(const GLdouble* v);
	#define glWindowPos2dv _ptr_to_glWindowPos2dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos2f)(GLfloat x,GLfloat y);
	#define glWindowPos2f _ptr_to_glWindowPos2f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos2fv)(const GLfloat* v);
	#define glWindowPos2fv _ptr_to_glWindowPos2fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos2i)(GLint x,GLint y);
	#define glWindowPos2i _ptr_to_glWindowPos2i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos2iv)(const GLint* v);
	#define glWindowPos2iv _ptr_to_glWindowPos2iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos2s)(GLshort x,GLshort y);
	#define glWindowPos2s _ptr_to_glWindowPos2s
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos2sv)(const GLshort* v);
	#define glWindowPos2sv _ptr_to_glWindowPos2sv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos3d)(GLdouble x,GLdouble y,GLdouble z);
	#define glWindowPos3d _ptr_to_glWindowPos3d
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos3dv)(const GLdouble* v);
	#define glWindowPos3dv _ptr_to_glWindowPos3dv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos3f)(GLfloat x,GLfloat y,GLfloat z);
	#define glWindowPos3f _ptr_to_glWindowPos3f
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos3fv)(const GLfloat* v);
	#define glWindowPos3fv _ptr_to_glWindowPos3fv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos3i)(GLint x,GLint y,GLint z);
	#define glWindowPos3i _ptr_to_glWindowPos3i
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos3iv)(const GLint* v);
	#define glWindowPos3iv _ptr_to_glWindowPos3iv
	FRONTIER_API extern void (API_ENTRY *_ptr_to_glWindowPos3s)(GLshort x,GLshort y,GLshort z);
	#define glWindowPos3s _ptr_to_glWindowPos3s
	
	#ifdef __cplusplus
	}
	#endif //__cplusplus
	
#endif //FRONTIER_GL_FUNCTIONS_H_INCLUDED
