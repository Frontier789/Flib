////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/System/macros/OS.h>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>

namespace fg
{
	fm::Log OpenGL_log = std::cout;
	void printNULL(const char *funcName)
	{
		OpenGL_log << "Error: "<<funcName<<" is NULL"<<std::endl;
	}
}
	
#ifdef FRONTIER_OS_MACOS
	#include <mach-o/dyld.h>
	
	priv::SO_LOADER::SO_LOADER()
	{
		p1 = NSAddImage("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", NSADDIMAGE_OPTION_RETURN_ON_ERROR);
	}
	
	priv::SO_LOADER::~SO_LOADER()
	{
		
	}
	
	void (*priv::SO_LOADER::getProcAddr(const std::string &name))()
	{
		NSSymbol symbol;
		char *symbolName;
		
		// prepend a '_' for the Unix C symbol mangling convention
		std::string name2 = '_'+name;
		symbol = image ? NSLookupSymbolInImage((const mach_header *)image, 
											   name2.c_str(), 
											   NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR) : fm::nullPtr;
		
		return symbol ? NSAddressOfSymbol(symbol) : fm::nullPtr;
	}
	
	#define FRONTIER_HAS_SO_LOADER 1
#endif // macOS

#if defined(FRONTIER_OS_SOLARIS) || defined(FRONTIER_OS_IRIX)
	#include <dlfcn.h>
	#include <stdio.h>
	
	priv::SO_LOADER::SO_LOADER()
	{
		p1 = dlopen(fm::nullPtr, RTLD_LAZY | RTLD_LOCAL);
		
		if (p1)
			p2 = dlsym(p1, "glXGetProcAddress");
	}
	
	priv::SO_LOADER::~SO_LOADER()
	{
		
	}
	
	void (*priv::SO_LOADER::getProcAddr(const std::string &name))()
	{
		if (p2)
			return ((void*(API_ENTRY*)(const GLubyte*))p2)(name.c_str());
		
		return dlsym(p1,name.c_str());
	}
	
	#define FRONTIER_HAS_SO_LOADER 1
#endif //Irix and Solaris

#ifdef FRONTIER_OS_ANDROID
	#include <dlfcn.h>
	
	priv::SO_LOADER::SO_LOADER()
	{
		p1 = dlopen("libGLESv2.so", RTLD_NOW|RTLD_LOCAL);
		
		if (!p1) p1 = dlopen("libGLES_CM.so", RTLD_NOW|RTLD_LOCAL);
		
		if (!p1) p1 = dlopen("libGLESv1_CM.so", RTLD_NOW|RTLD_LOCAL);
		
		if (!p1) fg::OpenGL_log << "Error: No GLES shared object found" << std::endl;
	}
	
	priv::SO_LOADER::~SO_LOADER()
	{
		if (p1) dlclose(p1);
	}
	
	void (*priv::SO_LOADER::getProcAddr(const std::string &name))()
	{
		void *symbol = dlsym(p1, name.c_str());
		if (!symbol) 
		{
			// append an underscore for platforms that need that.
			std::string name2 = '_'+name;
			
			symbol = dlsym(p1, name2.c_str());
			
			if (!symbol)
				fg::OpenGL_log << "Error: Couldn't find symbol " << name 
						   << " (" << (const char *)dlerror() << ")" <<std::endl;
		}
		return (void(*)())symbol;
	}
	
	#define FRONTIER_HAS_SO_LOADER 1
#endif //Android

#ifdef FRONTIER_OS_WINDOWS
	#include <windows.h>

	#ifdef _MSC_VER
	#pragma warning(disable: 4055)
	#pragma warning(disable: 4054)
	#endif

	
	priv::SO_LOADER::SO_LOADER()
	{
		
	}
	
	priv::SO_LOADER::~SO_LOADER()
	{
		
	}
	
	bool TestPointer(const PROC pTest)
	{
		if(!pTest)
			return false;
		
		ptrdiff_t iTest = (ptrdiff_t)pTest;
		
		if(iTest == 1 || 
		   iTest == 2 || 
		   iTest == 3 || 
		   iTest == -1) 
		   return false;
		
		return true;
	}
	
	void (*priv::SO_LOADER::getProcAddr(const std::string &name))()
	{
		HMODULE glMod = NULL;
		PROC pFunc = wglGetProcAddress((LPCSTR)name.c_str());
		if(TestPointer(pFunc))
			return (void(*)())pFunc;
		
		glMod = GetModuleHandleA("OpenGL32.dll");
		return (void(*)())GetProcAddress(glMod, (LPCSTR)name.c_str());
	}
	
	#define FRONTIER_HAS_SO_LOADER 1
#endif

#if defined(FRONTIER_OS_LINUX) && !defined(FRONTIER_OS_ANDROID)
	#include <GL/glx.h>
	
	priv::SO_LOADER::SO_LOADER()
	{
		
	}
	
	priv::SO_LOADER::~SO_LOADER()
	{
		
	}
	
	void (*priv::SO_LOADER::getProcAddr(const std::string &name))()
	{
		return glXGetProcAddressARB((const GLubyte*)name.c_str());
	}
	
	#define FRONTIER_HAS_SO_LOADER 1
#endif // A linux that is not android

#ifndef FRONTIER_HAS_SO_LOADER 
	#warning No retrieveProcAddress implementation!
	
	priv::SO_LOADER::SO_LOADER()
	{
		
	}
	
	priv::SO_LOADER::~SO_LOADER()
	{
		
	}
	
	void (*priv::SO_LOADER::getProcAddr(const std::string &name))()
	{
		return fm::nullPtr;
	}
#endif // Unsupported

#define retrieveProcAddress(name) priv::so_loader.getProcAddr(name)

namespace priv
{
	SO_LOADER so_loader;
}

#define GL_FUNC_IMPL(retType, funcName, argList, typeList, callList) \
static retType API_ENTRY _choose_##funcName argList                                  \
{                                                                                    \
	void (*gotPtr)() = (void(*)())retrieveProcAddress(#funcName);                    \
	if (!gotPtr)                                                                     \
		fg::printNULL(#funcName);                                                    \
	else                                                                             \
	{                                                                                \
		_ptr_to_##funcName = (retType(API_ENTRY*) typeList)gotPtr;                   \
		return _ptr_to_##funcName callList;                                          \
	}                                                                                \
	typedef retType TRetType;                                                        \
	return TRetType();                                                               \
}                                                                                    \
FRONTIER_API retType (API_ENTRY *_ptr_to_##funcName) argList = _choose_##funcName

GL_FUNC_IMPL(void,glAccum,(GLenum op,GLfloat value),(GLenum,GLfloat),(op,value));
GL_FUNC_IMPL(void,glActiveShaderProgram,(GLuint pipeline,GLuint program),(GLuint,GLuint),(pipeline,program));
GL_FUNC_IMPL(void,glActiveTexture,(GLenum texture),(GLenum),(texture));
GL_FUNC_IMPL(void,glAlphaFunc,(GLenum func,GLfloat ref),(GLenum,GLfloat),(func,ref));
GL_FUNC_IMPL(GLboolean,glAreTexturesResident,(GLsizei n,const GLuint* textures,GLboolean* residences),(GLsizei,const GLuint*,GLboolean*),(n,textures,residences));
GL_FUNC_IMPL(void,glArrayElement,(GLint i),(GLint),(i));
GL_FUNC_IMPL(void,glAttachObjectARB,(GLhandleARB containerObj,GLhandleARB obj),(GLhandleARB,GLhandleARB),(containerObj,obj));
GL_FUNC_IMPL(void,glAttachShader,(GLuint program,GLuint shader),(GLuint,GLuint),(program,shader));
GL_FUNC_IMPL(void,glBegin,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glBeginConditionalRender,(GLuint id,GLenum mode),(GLuint,GLenum),(id,mode));
GL_FUNC_IMPL(void,glBeginQuery,(GLenum target,GLuint id),(GLenum,GLuint),(target,id));
GL_FUNC_IMPL(void,glBeginQueryIndexed,(GLenum target,GLuint index,GLuint id),(GLenum,GLuint,GLuint),(target,index,id));
GL_FUNC_IMPL(void,glBeginTransformFeedback,(GLenum primitiveMode),(GLenum),(primitiveMode));
GL_FUNC_IMPL(void,glBindAttribLocation,(GLuint program,GLuint index,const GLchar* name),(GLuint,GLuint,const GLchar*),(program,index,name));
GL_FUNC_IMPL(void,glBindAttribLocationARB,(GLhandleARB programObj,GLuint index,const GLcharARB* name),(GLhandleARB,GLuint,const GLcharARB*),(programObj,index,name));
GL_FUNC_IMPL(void,glBindBuffer,(GLenum target,GLuint buffer),(GLenum,GLuint),(target,buffer));
GL_FUNC_IMPL(void,glBindBufferARB,(GLenum target,GLuint buffer),(GLenum,GLuint),(target,buffer));
GL_FUNC_IMPL(void,glBindBufferBase,(GLenum target,GLuint index,GLuint buffer),(GLenum,GLuint,GLuint),(target,index,buffer));
GL_FUNC_IMPL(void,glBindBufferRange,(GLenum target,GLuint index,GLuint buffer,GLintptr offset,GLsizeiptr size),(GLenum,GLuint,GLuint,GLintptr,GLsizeiptr),(target,index,buffer,offset,size));
GL_FUNC_IMPL(void,glBindFragDataLocation,(GLuint program,GLuint color,const GLchar* name),(GLuint,GLuint,const GLchar*),(program,color,name));
GL_FUNC_IMPL(void,glBindFragDataLocationIndexed,(GLuint program,GLuint colorNumber,GLuint index,const GLchar* name),(GLuint,GLuint,GLuint,const GLchar*),(program,colorNumber,index,name));
GL_FUNC_IMPL(void,glBindFramebuffer,(GLenum target,GLuint framebuffer),(GLenum,GLuint),(target,framebuffer));
GL_FUNC_IMPL(void,glBindFramebufferEXT,(GLenum target,GLuint framebuffer),(GLenum,GLuint),(target,framebuffer));
GL_FUNC_IMPL(void,glBindImageTexture,(GLuint unit,GLuint texture,GLint level,GLboolean layered,GLint layer,GLenum access,GLenum format),(GLuint,GLuint,GLint,GLboolean,GLint,GLenum,GLenum),(unit,texture,level,layered,layer,access,format));
GL_FUNC_IMPL(void,glBindProgramARB,(GLenum target,GLuint program),(GLenum,GLuint),(target,program));
GL_FUNC_IMPL(void,glBindProgramPipeline,(GLuint pipeline),(GLuint),(pipeline));
GL_FUNC_IMPL(void,glBindRenderbuffer,(GLenum target,GLuint renderbuffer),(GLenum,GLuint),(target,renderbuffer));
GL_FUNC_IMPL(void,glBindRenderbufferEXT,(GLenum target,GLuint renderbuffer),(GLenum,GLuint),(target,renderbuffer));
GL_FUNC_IMPL(void,glBindSampler,(GLuint unit,GLuint sampler),(GLuint,GLuint),(unit,sampler));
GL_FUNC_IMPL(void,glBindTexture,(GLenum target,GLuint texture),(GLenum,GLuint),(target,texture));
GL_FUNC_IMPL(void,glBindTransformFeedback,(GLenum target,GLuint id),(GLenum,GLuint),(target,id));
GL_FUNC_IMPL(void,glBindVertexArray,(GLuint ren_array),(GLuint),(ren_array));
GL_FUNC_IMPL(void,glBitmap,(GLsizei width,GLsizei height,GLfloat xorig,GLfloat yorig,GLfloat xmove,GLfloat ymove,const GLubyte* bitmap),(GLsizei,GLsizei,GLfloat,GLfloat,GLfloat,GLfloat,const GLubyte*),(width,height,xorig,yorig,xmove,ymove,bitmap));
GL_FUNC_IMPL(void,glBlendColor,(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha),(GLfloat,GLfloat,GLfloat,GLfloat),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glBlendEquation,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glBlendEquationSeparate,(GLenum modeRGB,GLenum modeAlpha),(GLenum,GLenum),(modeRGB,modeAlpha));
GL_FUNC_IMPL(void,glBlendEquationSeparatei,(GLuint buf,GLenum modeRGB,GLenum modeAlpha),(GLuint,GLenum,GLenum),(buf,modeRGB,modeAlpha));
GL_FUNC_IMPL(void,glBlendEquationi,(GLuint buf,GLenum mode),(GLuint,GLenum),(buf,mode));
GL_FUNC_IMPL(void,glBlendFunc,(GLenum sfactor,GLenum dfactor),(GLenum,GLenum),(sfactor,dfactor));
GL_FUNC_IMPL(void,glBlendFuncSeparate,(GLenum sfactorRGB,GLenum dfactorRGB,GLenum sfactorAlpha,GLenum dfactorAlpha),(GLenum,GLenum,GLenum,GLenum),(sfactorRGB,dfactorRGB,sfactorAlpha,dfactorAlpha));
GL_FUNC_IMPL(void,glBlendFuncSeparatei,(GLuint buf,GLenum srcRGB,GLenum dstRGB,GLenum srcAlpha,GLenum dstAlpha),(GLuint,GLenum,GLenum,GLenum,GLenum),(buf,srcRGB,dstRGB,srcAlpha,dstAlpha));
GL_FUNC_IMPL(void,glBlendFunci,(GLuint buf,GLenum src,GLenum dst),(GLuint,GLenum,GLenum),(buf,src,dst));
GL_FUNC_IMPL(void,glBlitFramebuffer,(GLint srcX0,GLint srcY0,GLint srcX1,GLint srcY1,GLint dstX0,GLint dstY0,GLint dstX1,GLint dstY1,GLbitfield mask,GLenum filter),(GLint,GLint,GLint,GLint,GLint,GLint,GLint,GLint,GLbitfield,GLenum),(srcX0,srcY0,srcX1,srcY1,dstX0,dstY0,dstX1,dstY1,mask,filter));
GL_FUNC_IMPL(void,glBufferData,(GLenum target,GLsizeiptr size,const GLvoid* data,GLenum usage),(GLenum,GLsizeiptr,const GLvoid*,GLenum),(target,size,data,usage));
GL_FUNC_IMPL(void,glBufferDataARB,(GLenum target,GLsizeiptrARB size,const GLvoid* data,GLenum usage),(GLenum,GLsizeiptrARB,const GLvoid*,GLenum),(target,size,data,usage));
GL_FUNC_IMPL(void,glBufferSubData,(GLenum target,GLintptr offset,GLsizeiptr size,const GLvoid* data),(GLenum,GLintptr,GLsizeiptr,const GLvoid*),(target,offset,size,data));
GL_FUNC_IMPL(void,glBufferSubDataARB,(GLenum target,GLintptrARB offset,GLsizeiptrARB size,const GLvoid* data),(GLenum,GLintptrARB,GLsizeiptrARB,const GLvoid*),(target,offset,size,data));
GL_FUNC_IMPL(void,glCallList,(GLuint list),(GLuint),(list));
GL_FUNC_IMPL(void,glCallLists,(GLsizei n,GLenum type,const GLvoid* lists),(GLsizei,GLenum,const GLvoid*),(n,type,lists));
GL_FUNC_IMPL(GLenum,glCheckFramebufferStatus,(GLenum target),(GLenum),(target));
GL_FUNC_IMPL(GLenum,glCheckFramebufferStatusEXT,(GLenum target),(GLenum),(target));
GL_FUNC_IMPL(void,glClampColor,(GLenum target,GLenum clamp),(GLenum,GLenum),(target,clamp));
GL_FUNC_IMPL(void,glClear,(GLbitfield mask),(GLbitfield),(mask));
GL_FUNC_IMPL(void,glClearAccum,(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha),(GLfloat,GLfloat,GLfloat,GLfloat),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glClearBufferfi,(GLenum buffer,GLint drawbuffer,GLfloat depth,GLint stencil),(GLenum,GLint,GLfloat,GLint),(buffer,drawbuffer,depth,stencil));
GL_FUNC_IMPL(void,glClearBufferfv,(GLenum buffer,GLint drawbuffer,const GLfloat* value),(GLenum,GLint,const GLfloat*),(buffer,drawbuffer,value));
GL_FUNC_IMPL(void,glClearBufferiv,(GLenum buffer,GLint drawbuffer,const GLint* value),(GLenum,GLint,const GLint*),(buffer,drawbuffer,value));
GL_FUNC_IMPL(void,glClearBufferuiv,(GLenum buffer,GLint drawbuffer,const GLuint* value),(GLenum,GLint,const GLuint*),(buffer,drawbuffer,value));
GL_FUNC_IMPL(void,glClearColor,(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha),(GLfloat,GLfloat,GLfloat,GLfloat),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glClearDepth,(GLdouble depth),(GLdouble),(depth));
GL_FUNC_IMPL(void,glClearDepthf,(GLfloat d),(GLfloat),(d));
GL_FUNC_IMPL(void,glClearIndex,(GLfloat c),(GLfloat),(c));
GL_FUNC_IMPL(void,glClearStencil,(GLint s),(GLint),(s));
GL_FUNC_IMPL(void,glClientActiveTexture,(GLenum texture),(GLenum),(texture));
GL_FUNC_IMPL(GLenum,glClientWaitSync,(GLsync sync,GLbitfield flags,GLuint64 timeout),(GLsync,GLbitfield,GLuint64),(sync,flags,timeout));
GL_FUNC_IMPL(void,glClipPlane,(GLenum plane,const GLdouble* equation),(GLenum,const GLdouble*),(plane,equation));
GL_FUNC_IMPL(void,glColor3b,(GLbyte red,GLbyte green,GLbyte blue),(GLbyte,GLbyte,GLbyte),(red,green,blue));
GL_FUNC_IMPL(void,glColor3bv,(const GLbyte* v),(const GLbyte*),(v));
GL_FUNC_IMPL(void,glColor3d,(GLdouble red,GLdouble green,GLdouble blue),(GLdouble,GLdouble,GLdouble),(red,green,blue));
GL_FUNC_IMPL(void,glColor3dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glColor3f,(GLfloat red,GLfloat green,GLfloat blue),(GLfloat,GLfloat,GLfloat),(red,green,blue));
GL_FUNC_IMPL(void,glColor3fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glColor3i,(GLint red,GLint green,GLint blue),(GLint,GLint,GLint),(red,green,blue));
GL_FUNC_IMPL(void,glColor3iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glColor3s,(GLshort red,GLshort green,GLshort blue),(GLshort,GLshort,GLshort),(red,green,blue));
GL_FUNC_IMPL(void,glColor3sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glColor3ub,(GLubyte red,GLubyte green,GLubyte blue),(GLubyte,GLubyte,GLubyte),(red,green,blue));
GL_FUNC_IMPL(void,glColor3ubv,(const GLubyte* v),(const GLubyte*),(v));
GL_FUNC_IMPL(void,glColor3ui,(GLuint red,GLuint green,GLuint blue),(GLuint,GLuint,GLuint),(red,green,blue));
GL_FUNC_IMPL(void,glColor3uiv,(const GLuint* v),(const GLuint*),(v));
GL_FUNC_IMPL(void,glColor3us,(GLushort red,GLushort green,GLushort blue),(GLushort,GLushort,GLushort),(red,green,blue));
GL_FUNC_IMPL(void,glColor3usv,(const GLushort* v),(const GLushort*),(v));
GL_FUNC_IMPL(void,glColor4b,(GLbyte red,GLbyte green,GLbyte blue,GLbyte alpha),(GLbyte,GLbyte,GLbyte,GLbyte),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glColor4bv,(const GLbyte* v),(const GLbyte*),(v));
GL_FUNC_IMPL(void,glColor4d,(GLdouble red,GLdouble green,GLdouble blue,GLdouble alpha),(GLdouble,GLdouble,GLdouble,GLdouble),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glColor4dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glColor4f,(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha),(GLfloat,GLfloat,GLfloat,GLfloat),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glColor4fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glColor4i,(GLint red,GLint green,GLint blue,GLint alpha),(GLint,GLint,GLint,GLint),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glColor4iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glColor4s,(GLshort red,GLshort green,GLshort blue,GLshort alpha),(GLshort,GLshort,GLshort,GLshort),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glColor4sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glColor4ub,(GLubyte red,GLubyte green,GLubyte blue,GLubyte alpha),(GLubyte,GLubyte,GLubyte,GLubyte),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glColor4ubv,(const GLubyte* v),(const GLubyte*),(v));
GL_FUNC_IMPL(void,glColor4ui,(GLuint red,GLuint green,GLuint blue,GLuint alpha),(GLuint,GLuint,GLuint,GLuint),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glColor4uiv,(const GLuint* v),(const GLuint*),(v));
GL_FUNC_IMPL(void,glColor4us,(GLushort red,GLushort green,GLushort blue,GLushort alpha),(GLushort,GLushort,GLushort,GLushort),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glColor4usv,(const GLushort* v),(const GLushort*),(v));
GL_FUNC_IMPL(void,glColorMask,(GLboolean red,GLboolean green,GLboolean blue,GLboolean alpha),(GLboolean,GLboolean,GLboolean,GLboolean),(red,green,blue,alpha));
GL_FUNC_IMPL(void,glColorMaski,(GLuint index,GLboolean r,GLboolean g,GLboolean b,GLboolean a),(GLuint,GLboolean,GLboolean,GLboolean,GLboolean),(index,r,g,b,a));
GL_FUNC_IMPL(void,glColorMaterial,(GLenum face,GLenum mode),(GLenum,GLenum),(face,mode));
GL_FUNC_IMPL(void,glColorP3ui,(GLenum type,GLuint color),(GLenum,GLuint),(type,color));
GL_FUNC_IMPL(void,glColorP3uiv,(GLenum type,const GLuint* color),(GLenum,const GLuint*),(type,color));
GL_FUNC_IMPL(void,glColorP4ui,(GLenum type,GLuint color),(GLenum,GLuint),(type,color));
GL_FUNC_IMPL(void,glColorP4uiv,(GLenum type,const GLuint* color),(GLenum,const GLuint*),(type,color));
GL_FUNC_IMPL(void,glColorPointer,(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer),(GLint,GLenum,GLsizei,const GLvoid*),(size,type,stride,pointer));
GL_FUNC_IMPL(void,glCompileShader,(GLuint shader),(GLuint),(shader));
GL_FUNC_IMPL(void,glCompileShaderARB,(GLhandleARB shaderObj),(GLhandleARB),(shaderObj));
GL_FUNC_IMPL(void,glCompressedTexImage1D,(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLint border,GLsizei imageSize,const GLvoid* data),(GLenum,GLint,GLenum,GLsizei,GLint,GLsizei,const GLvoid*),(target,level,internalformat,width,border,imageSize,data));
GL_FUNC_IMPL(void,glCompressedTexImage2D,(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLint border,GLsizei imageSize,const GLvoid* data),(GLenum,GLint,GLenum,GLsizei,GLsizei,GLint,GLsizei,const GLvoid*),(target,level,internalformat,width,height,border,imageSize,data));
GL_FUNC_IMPL(void,glCompressedTexImage3D,(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLsizei imageSize,const GLvoid* data),(GLenum,GLint,GLenum,GLsizei,GLsizei,GLsizei,GLint,GLsizei,const GLvoid*),(target,level,internalformat,width,height,depth,border,imageSize,data));
GL_FUNC_IMPL(void,glCompressedTexSubImage1D,(GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLsizei imageSize,const GLvoid* data),(GLenum,GLint,GLint,GLsizei,GLenum,GLsizei,const GLvoid*),(target,level,xoffset,width,format,imageSize,data));
GL_FUNC_IMPL(void,glCompressedTexSubImage2D,(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLsizei imageSize,const GLvoid* data),(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLsizei,const GLvoid*),(target,level,xoffset,yoffset,width,height,format,imageSize,data));
GL_FUNC_IMPL(void,glCompressedTexSubImage3D,(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLsizei imageSize,const GLvoid* data),(GLenum,GLint,GLint,GLint,GLint,GLsizei,GLsizei,GLsizei,GLenum,GLsizei,const GLvoid*),(target,level,xoffset,yoffset,zoffset,width,height,depth,format,imageSize,data));
GL_FUNC_IMPL(void *,glCopyBufferSubData,(GLenum readTarget,GLenum writeTarget,GLintptr readOffset,GLintptr writeOffset,GLsizeiptr size),(GLenum,GLenum,GLintptr,GLintptr,GLsizeiptr),(readTarget,writeTarget,readOffset,writeOffset,size));
GL_FUNC_IMPL(void,glCopyPixels,(GLint x,GLint y,GLsizei width,GLsizei height,GLenum type),(GLint,GLint,GLsizei,GLsizei,GLenum),(x,y,width,height,type));
GL_FUNC_IMPL(void,glCopyTexImage1D,(GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLint border),(GLenum,GLint,GLenum,GLint,GLint,GLsizei,GLint),(target,level,internalformat,x,y,width,border));
GL_FUNC_IMPL(void,glCopyTexImage2D,(GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLsizei height,GLint border),(GLenum,GLint,GLenum,GLint,GLint,GLsizei,GLsizei,GLint),(target,level,internalformat,x,y,width,height,border));
GL_FUNC_IMPL(void,glCopyTexSubImage1D,(GLenum target,GLint level,GLint xoffset,GLint x,GLint y,GLsizei width),(GLenum,GLint,GLint,GLint,GLint,GLsizei),(target,level,xoffset,x,y,width));
GL_FUNC_IMPL(void,glCopyTexSubImage2D,(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint x,GLint y,GLsizei width,GLsizei height),(GLenum,GLint,GLint,GLint,GLint,GLint,GLsizei,GLsizei),(target,level,xoffset,yoffset,x,y,width,height));
GL_FUNC_IMPL(void,glCopyTexSubImage3D,(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLint x,GLint y,GLsizei width,GLsizei height),(GLenum,GLint,GLint,GLint,GLint,GLint,GLint,GLsizei,GLsizei),(target,level,xoffset,yoffset,zoffset,x,y,width,height));
GL_FUNC_IMPL(GLuint,glCreateProgram,(),(),());
GL_FUNC_IMPL(GLhandleARB,glCreateProgramObjectARB,(),(),());
GL_FUNC_IMPL(GLuint,glCreateShader,(GLenum type),(GLenum),(type));
GL_FUNC_IMPL(GLhandleARB,glCreateShaderObjectARB,(GLenum shaderType),(GLenum),(shaderType));
GL_FUNC_IMPL(GLuint,glCreateShaderProgramv,(GLenum type,GLsizei count,const GLchar*const* strings),(GLenum,GLsizei,const GLchar*const*),(type,count,strings));
GL_FUNC_IMPL(void,glCullFace,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glDeleteBuffers,(GLsizei n,const GLuint* buffers),(GLsizei,const GLuint*),(n,buffers));
GL_FUNC_IMPL(void,glDeleteBuffersARB,(GLsizei n,const GLuint* buffers),(GLsizei,const GLuint*),(n,buffers));
GL_FUNC_IMPL(void,glDeleteFramebuffers,(GLsizei n,const GLuint* framebuffers),(GLsizei,const GLuint*),(n,framebuffers));
GL_FUNC_IMPL(void,glDeleteFramebuffersEXT,(GLsizei n,const GLuint* framebuffers),(GLsizei,const GLuint*),(n,framebuffers));
GL_FUNC_IMPL(void,glDeleteLists,(GLuint list,GLsizei range),(GLuint,GLsizei),(list,range));
GL_FUNC_IMPL(void,glDeleteObjectARB,(GLhandleARB obj),(GLhandleARB),(obj));
GL_FUNC_IMPL(void,glDeleteProgram,(GLuint program),(GLuint),(program));
GL_FUNC_IMPL(void,glDeleteProgramPipelines,(GLsizei n,const GLuint* pipelines),(GLsizei,const GLuint*),(n,pipelines));
GL_FUNC_IMPL(void,glDeleteProgramsARB,(GLsizei n,const GLuint* programs),(GLsizei,const GLuint*),(n,programs));
GL_FUNC_IMPL(void,glDeleteQueries,(GLsizei n,const GLuint* ids),(GLsizei,const GLuint*),(n,ids));
GL_FUNC_IMPL(void,glDeleteRenderbuffers,(GLsizei n,const GLuint* renderbuffers),(GLsizei,const GLuint*),(n,renderbuffers));
GL_FUNC_IMPL(void,glDeleteRenderbuffersEXT,(GLsizei n,const GLuint* renderbuffers),(GLsizei,const GLuint*),(n,renderbuffers));
GL_FUNC_IMPL(void,glDeleteSamplers,(GLsizei count,const GLuint* samplers),(GLsizei,const GLuint*),(count,samplers));
GL_FUNC_IMPL(void,glDeleteShader,(GLuint shader),(GLuint),(shader));
GL_FUNC_IMPL(void,glDeleteSync,(GLsync sync),(GLsync),(sync));
GL_FUNC_IMPL(void,glDeleteTextures,(GLsizei n,const GLuint* textures),(GLsizei,const GLuint*),(n,textures));
GL_FUNC_IMPL(void,glDeleteTransformFeedbacks,(GLsizei n,const GLuint* ids),(GLsizei,const GLuint*),(n,ids));
GL_FUNC_IMPL(void,glDeleteVertexArrays,(GLsizei n,const GLuint* arrays),(GLsizei,const GLuint*),(n,arrays));
GL_FUNC_IMPL(void,glDepthFunc,(GLenum func),(GLenum),(func));
GL_FUNC_IMPL(void,glDepthMask,(GLboolean flag),(GLboolean),(flag));
GL_FUNC_IMPL(void,glDepthRange,(GLdouble ren_near,GLdouble ren_far),(GLdouble,GLdouble),(ren_near,ren_far));
GL_FUNC_IMPL(void,glDepthRangeArrayv,(GLuint first,GLsizei count,const GLdouble* v),(GLuint,GLsizei,const GLdouble*),(first,count,v));
GL_FUNC_IMPL(void,glDepthRangeIndexed,(GLuint index,GLdouble n,GLdouble f),(GLuint,GLdouble,GLdouble),(index,n,f));
GL_FUNC_IMPL(void,glDepthRangef,(GLfloat n,GLfloat f),(GLfloat,GLfloat),(n,f));
GL_FUNC_IMPL(void,glDetachObjectARB,(GLhandleARB containerObj,GLhandleARB attachedObj),(GLhandleARB,GLhandleARB),(containerObj,attachedObj));
GL_FUNC_IMPL(void,glDetachShader,(GLuint program,GLuint shader),(GLuint,GLuint),(program,shader));
GL_FUNC_IMPL(void,glDisable,(GLenum cap),(GLenum),(cap));
GL_FUNC_IMPL(void,glDisableClientState,(GLenum ren_array),(GLenum),(ren_array));
GL_FUNC_IMPL(void,glDisableVertexAttribArray,(GLuint index),(GLuint),(index));
GL_FUNC_IMPL(void,glDisableVertexAttribArrayARB,(GLuint index),(GLuint),(index));
GL_FUNC_IMPL(void,glDisablei,(GLenum target,GLuint index),(GLenum,GLuint),(target,index));
GL_FUNC_IMPL(void,glDrawArrays,(GLenum mode,GLint first,GLsizei count),(GLenum,GLint,GLsizei),(mode,first,count));
GL_FUNC_IMPL(void,glDrawArraysIndirect,(GLenum mode,const GLvoid* indirect),(GLenum,const GLvoid*),(mode,indirect));
GL_FUNC_IMPL(void,glDrawArraysInstanced,(GLenum mode,GLint first,GLsizei count,GLsizei instancecount),(GLenum,GLint,GLsizei,GLsizei),(mode,first,count,instancecount));
GL_FUNC_IMPL(void,glDrawArraysInstancedBaseInstance,(GLenum mode,GLint first,GLsizei count,GLsizei instancecount,GLuint baseinstance),(GLenum,GLint,GLsizei,GLsizei,GLuint),(mode,first,count,instancecount,baseinstance));
GL_FUNC_IMPL(void,glDrawBuffer,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glDrawBuffers,(GLsizei n,const GLenum* bufs),(GLsizei,const GLenum*),(n,bufs));
GL_FUNC_IMPL(void,glDrawElements,(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices),(GLenum,GLsizei,GLenum,const GLvoid*),(mode,count,type,indices));
GL_FUNC_IMPL(void,glDrawElementsBaseVertex,(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLint basevertex),(GLenum,GLsizei,GLenum,const GLvoid*,GLint),(mode,count,type,indices,basevertex));
GL_FUNC_IMPL(void,glDrawElementsIndirect,(GLenum mode,GLenum type,const GLvoid* indirect),(GLenum,GLenum,const GLvoid*),(mode,type,indirect));
GL_FUNC_IMPL(void,glDrawElementsInstanced,(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount),(GLenum,GLsizei,GLenum,const GLvoid*,GLsizei),(mode,count,type,indices,instancecount));
GL_FUNC_IMPL(void,glDrawElementsInstancedBaseInstance,(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLuint baseinstance),(GLenum,GLsizei,GLenum,const GLvoid*,GLsizei,GLuint),(mode,count,type,indices,instancecount,baseinstance));
GL_FUNC_IMPL(void,glDrawElementsInstancedBaseVertex,(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLint basevertex),(GLenum,GLsizei,GLenum,const GLvoid*,GLsizei,GLint),(mode,count,type,indices,instancecount,basevertex));
GL_FUNC_IMPL(void,glDrawElementsInstancedBaseVertexBaseInstance,(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLint basevertex,GLuint baseinstance),(GLenum,GLsizei,GLenum,const GLvoid*,GLsizei,GLint,GLuint),(mode,count,type,indices,instancecount,basevertex,baseinstance));
GL_FUNC_IMPL(void,glDrawPixels,(GLsizei width,GLsizei height,GLenum format,GLenum type,const GLvoid* pixels),(GLsizei,GLsizei,GLenum,GLenum,const GLvoid*),(width,height,format,type,pixels));
GL_FUNC_IMPL(void,glDrawRangeElements,(GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const GLvoid* indices),(GLenum,GLuint,GLuint,GLsizei,GLenum,const GLvoid*),(mode,start,end,count,type,indices));
GL_FUNC_IMPL(void,glDrawRangeElementsBaseVertex,(GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const GLvoid* indices,GLint basevertex),(GLenum,GLuint,GLuint,GLsizei,GLenum,const GLvoid*,GLint),(mode,start,end,count,type,indices,basevertex));
GL_FUNC_IMPL(void,glDrawTransformFeedback,(GLenum mode,GLuint id),(GLenum,GLuint),(mode,id));
GL_FUNC_IMPL(void,glDrawTransformFeedbackInstanced,(GLenum mode,GLuint id,GLsizei instancecount),(GLenum,GLuint,GLsizei),(mode,id,instancecount));
GL_FUNC_IMPL(void,glDrawTransformFeedbackStream,(GLenum mode,GLuint id,GLuint stream),(GLenum,GLuint,GLuint),(mode,id,stream));
GL_FUNC_IMPL(void,glDrawTransformFeedbackStreamInstanced,(GLenum mode,GLuint id,GLuint stream,GLsizei instancecount),(GLenum,GLuint,GLuint,GLsizei),(mode,id,stream,instancecount));
GL_FUNC_IMPL(void,glEdgeFlag,(GLboolean flag),(GLboolean),(flag));
GL_FUNC_IMPL(void,glEdgeFlagPointer,(GLsizei stride,const GLvoid* pointer),(GLsizei,const GLvoid*),(stride,pointer));
GL_FUNC_IMPL(void,glEdgeFlagv,(const GLboolean* flag),(const GLboolean*),(flag));
GL_FUNC_IMPL(void,glEnable,(GLenum cap),(GLenum),(cap));
GL_FUNC_IMPL(void,glEnableClientState,(GLenum ren_array),(GLenum),(ren_array));
GL_FUNC_IMPL(void,glEnableVertexAttribArray,(GLuint index),(GLuint),(index));
GL_FUNC_IMPL(void,glEnableVertexAttribArrayARB,(GLuint index),(GLuint),(index));
GL_FUNC_IMPL(void,glEnablei,(GLenum target,GLuint index),(GLenum,GLuint),(target,index));
GL_FUNC_IMPL(void,glEnd,(),(),());
GL_FUNC_IMPL(void,glEndConditionalRender,(),(),());
GL_FUNC_IMPL(void,glEndList,(),(),());
GL_FUNC_IMPL(void,glEndQuery,(GLenum target),(GLenum),(target));
GL_FUNC_IMPL(void,glEndQueryIndexed,(GLenum target,GLuint index),(GLenum,GLuint),(target,index));
GL_FUNC_IMPL(void,glEndTransformFeedback,(),(),());
GL_FUNC_IMPL(void,glEvalCoord1d,(GLdouble u),(GLdouble),(u));
GL_FUNC_IMPL(void,glEvalCoord1dv,(const GLdouble* u),(const GLdouble*),(u));
GL_FUNC_IMPL(void,glEvalCoord1f,(GLfloat u),(GLfloat),(u));
GL_FUNC_IMPL(void,glEvalCoord1fv,(const GLfloat* u),(const GLfloat*),(u));
GL_FUNC_IMPL(void,glEvalCoord2d,(GLdouble u,GLdouble v),(GLdouble,GLdouble),(u,v));
GL_FUNC_IMPL(void,glEvalCoord2dv,(const GLdouble* u),(const GLdouble*),(u));
GL_FUNC_IMPL(void,glEvalCoord2f,(GLfloat u,GLfloat v),(GLfloat,GLfloat),(u,v));
GL_FUNC_IMPL(void,glEvalCoord2fv,(const GLfloat* u),(const GLfloat*),(u));
GL_FUNC_IMPL(void,glEvalMesh1,(GLenum mode,GLint i1,GLint i2),(GLenum,GLint,GLint),(mode,i1,i2));
GL_FUNC_IMPL(void,glEvalMesh2,(GLenum mode,GLint i1,GLint i2,GLint j1,GLint j2),(GLenum,GLint,GLint,GLint,GLint),(mode,i1,i2,j1,j2));
GL_FUNC_IMPL(void,glEvalPoint1,(GLint i),(GLint),(i));
GL_FUNC_IMPL(void,glEvalPoint2,(GLint i,GLint j),(GLint,GLint),(i,j));
GL_FUNC_IMPL(void,glFeedbackBuffer,(GLsizei size,GLenum type,GLfloat* buffer),(GLsizei,GLenum,GLfloat*),(size,type,buffer));
GL_FUNC_IMPL(GLsync,glFenceSync,(GLenum condition,GLbitfield flags),(GLenum,GLbitfield),(condition,flags));
GL_FUNC_IMPL(void,glFinish,(),(),());
GL_FUNC_IMPL(void,glFlush,(),(),());
GL_FUNC_IMPL(void,glFlushMappedBufferRange,(GLenum target,GLintptr offset,GLsizeiptr length),(GLenum,GLintptr,GLsizeiptr),(target,offset,length));
GL_FUNC_IMPL(void,glFogCoordPointer,(GLenum type,GLsizei stride,const GLvoid* pointer),(GLenum,GLsizei,const GLvoid*),(type,stride,pointer));
GL_FUNC_IMPL(void,glFogCoordd,(GLdouble coord),(GLdouble),(coord));
GL_FUNC_IMPL(void,glFogCoorddv,(const GLdouble* coord),(const GLdouble*),(coord));
GL_FUNC_IMPL(void,glFogCoordf,(GLfloat coord),(GLfloat),(coord));
GL_FUNC_IMPL(void,glFogCoordfv,(const GLfloat* coord),(const GLfloat*),(coord));
GL_FUNC_IMPL(void,glFogf,(GLenum pname,GLfloat param),(GLenum,GLfloat),(pname,param));
GL_FUNC_IMPL(void,glFogfv,(GLenum pname,const GLfloat* params),(GLenum,const GLfloat*),(pname,params));
GL_FUNC_IMPL(void,glFogi,(GLenum pname,GLint param),(GLenum,GLint),(pname,param));
GL_FUNC_IMPL(void,glFogiv,(GLenum pname,const GLint* params),(GLenum,const GLint*),(pname,params));
GL_FUNC_IMPL(void,glFramebufferRenderbuffer,(GLenum target,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer),(GLenum,GLenum,GLenum,GLuint),(target,attachment,renderbuffertarget,renderbuffer));
GL_FUNC_IMPL(void,glFramebufferRenderbufferEXT,(GLenum target,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer),(GLenum,GLenum,GLenum,GLuint),(target,attachment,renderbuffertarget,renderbuffer));
GL_FUNC_IMPL(void,glFramebufferTexture,(GLenum target,GLenum attachment,GLuint texture,GLint level),(GLenum,GLenum,GLuint,GLint),(target,attachment,texture,level));
GL_FUNC_IMPL(void,glFramebufferTexture1D,(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level),(GLenum,GLenum,GLenum,GLuint,GLint),(target,attachment,textarget,texture,level));
GL_FUNC_IMPL(void,glFramebufferTexture1DEXT,(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level),(GLenum,GLenum,GLenum,GLuint,GLint),(target,attachment,textarget,texture,level));
GL_FUNC_IMPL(void,glFramebufferTexture2D,(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level),(GLenum,GLenum,GLenum,GLuint,GLint),(target,attachment,textarget,texture,level));
GL_FUNC_IMPL(void,glFramebufferTexture2DEXT,(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level),(GLenum,GLenum,GLenum,GLuint,GLint),(target,attachment,textarget,texture,level));
GL_FUNC_IMPL(void,glFramebufferTexture3D,(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset),(GLenum,GLenum,GLenum,GLuint,GLint,GLint),(target,attachment,textarget,texture,level,zoffset));
GL_FUNC_IMPL(void,glFramebufferTexture3DEXT,(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset),(GLenum,GLenum,GLenum,GLuint,GLint,GLint),(target,attachment,textarget,texture,level,zoffset));
GL_FUNC_IMPL(void,glFramebufferTextureLayer,(GLenum target,GLenum attachment,GLuint texture,GLint level,GLint layer),(GLenum,GLenum,GLuint,GLint,GLint),(target,attachment,texture,level,layer));
GL_FUNC_IMPL(void,glFrontFace,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glFrustum,(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar),(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble),(left,right,bottom,top,zNear,zFar));
GL_FUNC_IMPL(void,glGenBuffers,(GLsizei n,GLuint* buffers),(GLsizei,GLuint*),(n,buffers));
GL_FUNC_IMPL(void,glGenBuffersARB,(GLsizei n,GLuint* buffers),(GLsizei,GLuint*),(n,buffers));
GL_FUNC_IMPL(void,glGenFramebuffers,(GLsizei n,GLuint* framebuffers),(GLsizei,GLuint*),(n,framebuffers));
GL_FUNC_IMPL(void,glGenFramebuffersEXT,(GLsizei n,GLuint* framebuffers),(GLsizei,GLuint*),(n,framebuffers));
GL_FUNC_IMPL(GLuint,glGenLists,(GLsizei range),(GLsizei),(range));
GL_FUNC_IMPL(void,glGenProgramPipelines,(GLsizei n,GLuint* pipelines),(GLsizei,GLuint*),(n,pipelines));
GL_FUNC_IMPL(void,glGenProgramsARB,(GLsizei n,GLuint* programs),(GLsizei,GLuint*),(n,programs));
GL_FUNC_IMPL(void,glGenQueries,(GLsizei n,GLuint* ids),(GLsizei,GLuint*),(n,ids));
GL_FUNC_IMPL(void,glGenRenderbuffers,(GLsizei n,GLuint* renderbuffers),(GLsizei,GLuint*),(n,renderbuffers));
GL_FUNC_IMPL(void,glGenRenderbuffersEXT,(GLsizei n,GLuint* renderbuffers),(GLsizei,GLuint*),(n,renderbuffers));
GL_FUNC_IMPL(void,glGenSamplers,(GLsizei count,GLuint* samplers),(GLsizei,GLuint*),(count,samplers));
GL_FUNC_IMPL(void,glGenTextures,(GLsizei n,GLuint* textures),(GLsizei,GLuint*),(n,textures));
GL_FUNC_IMPL(void,glGenTransformFeedbacks,(GLsizei n,GLuint* ids),(GLsizei,GLuint*),(n,ids));
GL_FUNC_IMPL(void,glGenVertexArrays,(GLsizei n,GLuint* arrays),(GLsizei,GLuint*),(n,arrays));
GL_FUNC_IMPL(void,glGenerateMipmap,(GLenum target),(GLenum),(target));
GL_FUNC_IMPL(void,glGenerateMipmapEXT,(GLenum target),(GLenum),(target));
GL_FUNC_IMPL(void,glGetActiveAtomicCounterBufferiv,(GLuint program,GLuint bufferIndex,GLenum pname,GLint* params),(GLuint,GLuint,GLenum,GLint*),(program,bufferIndex,pname,params));
GL_FUNC_IMPL(void,glGetActiveAttrib,(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLint* size,GLenum* type,GLchar* name),(GLuint,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,GLchar*),(program,index,bufSize,length,size,type,name));
GL_FUNC_IMPL(void,glGetActiveAttribARB,(GLhandleARB programObj,GLuint index,GLsizei maxLength,GLsizei* length,GLint* size,GLenum* type,GLcharARB* name),(GLhandleARB,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,GLcharARB*),(programObj,index,maxLength,length,size,type,name));
GL_FUNC_IMPL(void,glGetActiveSubroutineName,(GLuint program,GLenum shadertype,GLuint index,GLsizei bufsize,GLsizei* length,GLchar* name),(GLuint,GLenum,GLuint,GLsizei,GLsizei*,GLchar*),(program,shadertype,index,bufsize,length,name));
GL_FUNC_IMPL(void,glGetActiveSubroutineUniformName,(GLuint program,GLenum shadertype,GLuint index,GLsizei bufsize,GLsizei* length,GLchar* name),(GLuint,GLenum,GLuint,GLsizei,GLsizei*,GLchar*),(program,shadertype,index,bufsize,length,name));
GL_FUNC_IMPL(void,glGetActiveSubroutineUniformiv,(GLuint program,GLenum shadertype,GLuint index,GLenum pname,GLint* values),(GLuint,GLenum,GLuint,GLenum,GLint*),(program,shadertype,index,pname,values));
GL_FUNC_IMPL(void,glGetActiveUniform,(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLint* size,GLenum* type,GLchar* name),(GLuint,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,GLchar*),(program,index,bufSize,length,size,type,name));
GL_FUNC_IMPL(void,glGetActiveUniformARB,(GLhandleARB programObj,GLuint index,GLsizei maxLength,GLsizei* length,GLint* size,GLenum* type,GLcharARB* name),(GLhandleARB,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,GLcharARB*),(programObj,index,maxLength,length,size,type,name));
GL_FUNC_IMPL(void,glGetActiveUniformBlockName,(GLuint program,GLuint uniformBlockIndex,GLsizei bufSize,GLsizei* length,GLchar* uniformBlockName),(GLuint,GLuint,GLsizei,GLsizei*,GLchar*),(program,uniformBlockIndex,bufSize,length,uniformBlockName));
GL_FUNC_IMPL(void,glGetActiveUniformBlockiv,(GLuint program,GLuint uniformBlockIndex,GLenum pname,GLint* params),(GLuint,GLuint,GLenum,GLint*),(program,uniformBlockIndex,pname,params));
GL_FUNC_IMPL(void,glGetActiveUniformName,(GLuint program,GLuint uniformIndex,GLsizei bufSize,GLsizei* length,GLchar* uniformName),(GLuint,GLuint,GLsizei,GLsizei*,GLchar*),(program,uniformIndex,bufSize,length,uniformName));
GL_FUNC_IMPL(void,glGetActiveUniformsiv,(GLuint program,GLsizei uniformCount,const GLuint* uniformIndices,GLenum pname,GLint* params),(GLuint,GLsizei,const GLuint*,GLenum,GLint*),(program,uniformCount,uniformIndices,pname,params));
GL_FUNC_IMPL(void,glGetAttachedObjectsARB,(GLhandleARB containerObj,GLsizei maxCount,GLsizei* count,GLhandleARB* obj),(GLhandleARB,GLsizei,GLsizei*,GLhandleARB*),(containerObj,maxCount,count,obj));
GL_FUNC_IMPL(void,glGetAttachedShaders,(GLuint program,GLsizei maxCount,GLsizei* count,GLuint* shaders),(GLuint,GLsizei,GLsizei*,GLuint*),(program,maxCount,count,shaders));
GL_FUNC_IMPL(GLint,glGetAttribLocation,(GLuint program,const GLchar* name),(GLuint,const GLchar*),(program,name));
GL_FUNC_IMPL(GLint,glGetAttribLocationARB,(GLhandleARB programObj,const GLcharARB* name),(GLhandleARB,const GLcharARB*),(programObj,name));
GL_FUNC_IMPL(void,glGetBooleani_v,(GLenum target,GLuint index,GLboolean* data),(GLenum,GLuint,GLboolean*),(target,index,data));
GL_FUNC_IMPL(void,glGetBooleanv,(GLenum pname,GLboolean* params),(GLenum,GLboolean*),(pname,params));
GL_FUNC_IMPL(void,glGetBufferParameteri64v,(GLenum target,GLenum pname,GLint64* params),(GLenum,GLenum,GLint64*),(target,pname,params));
GL_FUNC_IMPL(void,glGetBufferParameteriv,(GLenum target,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetBufferParameterivARB,(GLenum target,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetBufferPointerv,(GLenum target,GLenum pname,GLvoid** params),(GLenum,GLenum,GLvoid**),(target,pname,params));
GL_FUNC_IMPL(void,glGetBufferPointervARB,(GLenum target,GLenum pname,GLvoid** params),(GLenum,GLenum,GLvoid**),(target,pname,params));
GL_FUNC_IMPL(void,glGetBufferSubData,(GLenum target,GLintptr offset,GLsizeiptr size,GLvoid* data),(GLenum,GLintptr,GLsizeiptr,GLvoid*),(target,offset,size,data));
GL_FUNC_IMPL(void,glGetBufferSubDataARB,(GLenum target,GLintptrARB offset,GLsizeiptrARB size,GLvoid* data),(GLenum,GLintptrARB,GLsizeiptrARB,GLvoid*),(target,offset,size,data));
GL_FUNC_IMPL(void,glGetClipPlane,(GLenum plane,GLdouble* equation),(GLenum,GLdouble*),(plane,equation));
GL_FUNC_IMPL(void,glGetCompressedTexImage,(GLenum target,GLint level,GLvoid* img),(GLenum,GLint,GLvoid*),(target,level,img));
GL_FUNC_IMPL(void,glGetDoublei_v,(GLenum target,GLuint index,GLdouble* data),(GLenum,GLuint,GLdouble*),(target,index,data));
GL_FUNC_IMPL(void,glGetDoublev,(GLenum pname,GLdouble* params),(GLenum,GLdouble*),(pname,params));
GL_FUNC_IMPL(GLenum,glGetError,(),(),());
GL_FUNC_IMPL(void,glGetFloati_v,(GLenum target,GLuint index,GLfloat* data),(GLenum,GLuint,GLfloat*),(target,index,data));
GL_FUNC_IMPL(void,glGetFloatv,(GLenum pname,GLfloat* params),(GLenum,GLfloat*),(pname,params));
GL_FUNC_IMPL(GLint,glGetFragDataIndex,(GLuint program,const GLchar* name),(GLuint,const GLchar*),(program,name));
GL_FUNC_IMPL(GLint,glGetFragDataLocation,(GLuint program,const GLchar* name),(GLuint,const GLchar*),(program,name));
GL_FUNC_IMPL(void,glGetFramebufferAttachmentParameteriv,(GLenum target,GLenum attachment,GLenum pname,GLint* params),(GLenum,GLenum,GLenum,GLint*),(target,attachment,pname,params));
GL_FUNC_IMPL(void,glGetFramebufferAttachmentParameterivEXT,(GLenum target,GLenum attachment,GLenum pname,GLint* params),(GLenum,GLenum,GLenum,GLint*),(target,attachment,pname,params));
GL_FUNC_IMPL(GLhandleARB,glGetHandleARB,(GLenum pname),(GLenum),(pname));
GL_FUNC_IMPL(void,glGetInfoLogARB,(GLhandleARB obj,GLsizei maxLength,GLsizei* length,GLcharARB* infoLog),(GLhandleARB,GLsizei,GLsizei*,GLcharARB*),(obj,maxLength,length,infoLog));
GL_FUNC_IMPL(void,glGetInteger64i_v,(GLenum target,GLuint index,GLint64* data),(GLenum,GLuint,GLint64*),(target,index,data));
GL_FUNC_IMPL(void,glGetInteger64v,(GLenum pname,GLint64* params),(GLenum,GLint64*),(pname,params));
GL_FUNC_IMPL(void,glGetIntegeri_v,(GLenum target,GLuint index,GLint* data),(GLenum,GLuint,GLint*),(target,index,data));
GL_FUNC_IMPL(void,glGetIntegerv,(GLenum pname,GLint* params),(GLenum,GLint*),(pname,params));
GL_FUNC_IMPL(void,glGetInternalformati64v,(GLenum target,GLenum internalformat,GLenum pname,GLsizei bufSize,GLint64* params),(GLenum,GLenum,GLenum,GLsizei,GLint64*),(target,internalformat,pname,bufSize,params));
GL_FUNC_IMPL(void,glGetLightfv,(GLenum light,GLenum pname,GLfloat* params),(GLenum,GLenum,GLfloat*),(light,pname,params));
GL_FUNC_IMPL(void,glGetLightiv,(GLenum light,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(light,pname,params));
GL_FUNC_IMPL(void,glGetMapdv,(GLenum target,GLenum query,GLdouble* v),(GLenum,GLenum,GLdouble*),(target,query,v));
GL_FUNC_IMPL(void,glGetMapfv,(GLenum target,GLenum query,GLfloat* v),(GLenum,GLenum,GLfloat*),(target,query,v));
GL_FUNC_IMPL(void,glGetMapiv,(GLenum target,GLenum query,GLint* v),(GLenum,GLenum,GLint*),(target,query,v));
GL_FUNC_IMPL(void,glGetMaterialfv,(GLenum face,GLenum pname,GLfloat* params),(GLenum,GLenum,GLfloat*),(face,pname,params));
GL_FUNC_IMPL(void,glGetMaterialiv,(GLenum face,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(face,pname,params));
GL_FUNC_IMPL(void,glGetMultisamplefv,(GLenum pname,GLuint index,GLfloat* val),(GLenum,GLuint,GLfloat*),(pname,index,val));
GL_FUNC_IMPL(void,glGetObjectParameterfvARB,(GLhandleARB obj,GLenum pname,GLfloat* params),(GLhandleARB,GLenum,GLfloat*),(obj,pname,params));
GL_FUNC_IMPL(void,glGetObjectParameterivARB,(GLhandleARB obj,GLenum pname,GLint* params),(GLhandleARB,GLenum,GLint*),(obj,pname,params));
GL_FUNC_IMPL(void,glGetPixelMapfv,(GLenum map,GLfloat* values),(GLenum,GLfloat*),(map,values));
GL_FUNC_IMPL(void,glGetPixelMapuiv,(GLenum map,GLuint* values),(GLenum,GLuint*),(map,values));
GL_FUNC_IMPL(void,glGetPixelMapusv,(GLenum map,GLushort* values),(GLenum,GLushort*),(map,values));
GL_FUNC_IMPL(void,glGetPointerv,(GLenum pname,GLvoid** params),(GLenum,GLvoid**),(pname,params));
GL_FUNC_IMPL(void,glGetPolygonStipple,(GLubyte* mask),(GLubyte*),(mask));
GL_FUNC_IMPL(void,glGetProgramBinary,(GLuint program,GLsizei bufSize,GLsizei* length,GLenum* binaryFormat,GLvoid* binary),(GLuint,GLsizei,GLsizei*,GLenum*,GLvoid*),(program,bufSize,length,binaryFormat,binary));
GL_FUNC_IMPL(void,glGetProgramEnvParameterdvARB,(GLenum target,GLuint index,GLdouble* params),(GLenum,GLuint,GLdouble*),(target,index,params));
GL_FUNC_IMPL(void,glGetProgramEnvParameterfvARB,(GLenum target,GLuint index,GLfloat* params),(GLenum,GLuint,GLfloat*),(target,index,params));
GL_FUNC_IMPL(void,glGetProgramInfoLog,(GLuint program,GLsizei bufSize,GLsizei* length,GLchar* infoLog),(GLuint,GLsizei,GLsizei*,GLchar*),(program,bufSize,length,infoLog));
GL_FUNC_IMPL(void,glGetProgramLocalParameterdvARB,(GLenum target,GLuint index,GLdouble* params),(GLenum,GLuint,GLdouble*),(target,index,params));
GL_FUNC_IMPL(void,glGetProgramLocalParameterfvARB,(GLenum target,GLuint index,GLfloat* params),(GLenum,GLuint,GLfloat*),(target,index,params));
GL_FUNC_IMPL(void,glGetProgramPipelineInfoLog,(GLuint pipeline,GLsizei bufSize,GLsizei* length,GLchar* infoLog),(GLuint,GLsizei,GLsizei*,GLchar*),(pipeline,bufSize,length,infoLog));
GL_FUNC_IMPL(void,glGetProgramPipelineiv,(GLuint pipeline,GLenum pname,GLint* params),(GLuint,GLenum,GLint*),(pipeline,pname,params));
GL_FUNC_IMPL(void,glGetProgramStageiv,(GLuint program,GLenum shadertype,GLenum pname,GLint* values),(GLuint,GLenum,GLenum,GLint*),(program,shadertype,pname,values));
GL_FUNC_IMPL(void,glGetProgramStringARB,(GLenum target,GLenum pname,GLvoid* string),(GLenum,GLenum,GLvoid*),(target,pname,string));
GL_FUNC_IMPL(void,glGetProgramiv,(GLuint program,GLenum pname,GLint* params),(GLuint,GLenum,GLint*),(program,pname,params));
GL_FUNC_IMPL(void,glGetProgramivARB,(GLenum target,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetQueryIndexediv,(GLenum target,GLuint index,GLenum pname,GLint* params),(GLenum,GLuint,GLenum,GLint*),(target,index,pname,params));
GL_FUNC_IMPL(void,glGetQueryObjecti64v,(GLuint id,GLenum pname,GLint64* params),(GLuint,GLenum,GLint64*),(id,pname,params));
GL_FUNC_IMPL(void,glGetQueryObjectiv,(GLuint id,GLenum pname,GLint* params),(GLuint,GLenum,GLint*),(id,pname,params));
GL_FUNC_IMPL(void,glGetQueryObjectui64v,(GLuint id,GLenum pname,GLuint64* params),(GLuint,GLenum,GLuint64*),(id,pname,params));
GL_FUNC_IMPL(void,glGetQueryObjectuiv,(GLuint id,GLenum pname,GLuint* params),(GLuint,GLenum,GLuint*),(id,pname,params));
GL_FUNC_IMPL(void,glGetQueryiv,(GLenum target,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetRenderbufferParameteriv,(GLenum target,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetRenderbufferParameterivEXT,(GLenum target,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetSamplerParameterIiv,(GLuint sampler,GLenum pname,GLint* params),(GLuint,GLenum,GLint*),(sampler,pname,params));
GL_FUNC_IMPL(void,glGetSamplerParameterIuiv,(GLuint sampler,GLenum pname,GLuint* params),(GLuint,GLenum,GLuint*),(sampler,pname,params));
GL_FUNC_IMPL(void,glGetSamplerParameterfv,(GLuint sampler,GLenum pname,GLfloat* params),(GLuint,GLenum,GLfloat*),(sampler,pname,params));
GL_FUNC_IMPL(void,glGetSamplerParameteriv,(GLuint sampler,GLenum pname,GLint* params),(GLuint,GLenum,GLint*),(sampler,pname,params));
GL_FUNC_IMPL(void,glGetShaderInfoLog,(GLuint shader,GLsizei bufSize,GLsizei* length,GLchar* infoLog),(GLuint,GLsizei,GLsizei*,GLchar*),(shader,bufSize,length,infoLog));
GL_FUNC_IMPL(void,glGetShaderPrecisionFormat,(GLenum shadertype,GLenum precisiontype,GLint* range,GLint* precision),(GLenum,GLenum,GLint*,GLint*),(shadertype,precisiontype,range,precision));
GL_FUNC_IMPL(void,glGetShaderSource,(GLuint shader,GLsizei bufSize,GLsizei* length,GLchar* source),(GLuint,GLsizei,GLsizei*,GLchar*),(shader,bufSize,length,source));
GL_FUNC_IMPL(void,glGetShaderSourceARB,(GLhandleARB obj,GLsizei maxLength,GLsizei* length,GLcharARB* source),(GLhandleARB,GLsizei,GLsizei*,GLcharARB*),(obj,maxLength,length,source));
GL_FUNC_IMPL(void,glGetShaderiv,(GLuint shader,GLenum pname,GLint* params),(GLuint,GLenum,GLint*),(shader,pname,params));
GL_FUNC_IMPL(const GLubyte*,glGetString,(GLenum name),(GLenum),(name));
GL_FUNC_IMPL(const GLubyte*,glGetStringi,(GLenum name,GLuint index),(GLenum,GLuint),(name,index));
GL_FUNC_IMPL(GLuint,glGetSubroutineIndex,(GLuint program,GLenum shadertype,const GLchar* name),(GLuint,GLenum,const GLchar*),(program,shadertype,name));
GL_FUNC_IMPL(GLint,glGetSubroutineUniformLocation,(GLuint program,GLenum shadertype,const GLchar* name),(GLuint,GLenum,const GLchar*),(program,shadertype,name));
GL_FUNC_IMPL(void,glGetSynciv,(GLsync sync,GLenum pname,GLsizei bufSize,GLsizei* length,GLint* values),(GLsync,GLenum,GLsizei,GLsizei*,GLint*),(sync,pname,bufSize,length,values));
GL_FUNC_IMPL(void,glGetTexEnvfv,(GLenum target,GLenum pname,GLfloat* params),(GLenum,GLenum,GLfloat*),(target,pname,params));
GL_FUNC_IMPL(void,glGetTexEnviv,(GLenum target,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetTexGendv,(GLenum coord,GLenum pname,GLdouble* params),(GLenum,GLenum,GLdouble*),(coord,pname,params));
GL_FUNC_IMPL(void,glGetTexGenfv,(GLenum coord,GLenum pname,GLfloat* params),(GLenum,GLenum,GLfloat*),(coord,pname,params));
GL_FUNC_IMPL(void,glGetTexGeniv,(GLenum coord,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(coord,pname,params));
GL_FUNC_IMPL(void,glGetTexImage,(GLenum target,GLint level,GLenum format,GLenum type,GLvoid* pixels),(GLenum,GLint,GLenum,GLenum,GLvoid*),(target,level,format,type,pixels));
GL_FUNC_IMPL(void,glGetTexLevelParameterfv,(GLenum target,GLint level,GLenum pname,GLfloat* params),(GLenum,GLint,GLenum,GLfloat*),(target,level,pname,params));
GL_FUNC_IMPL(void,glGetTexLevelParameteriv,(GLenum target,GLint level,GLenum pname,GLint* params),(GLenum,GLint,GLenum,GLint*),(target,level,pname,params));
GL_FUNC_IMPL(void,glGetTexParameterIiv,(GLenum target,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetTexParameterIuiv,(GLenum target,GLenum pname,GLuint* params),(GLenum,GLenum,GLuint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetTexParameterfv,(GLenum target,GLenum pname,GLfloat* params),(GLenum,GLenum,GLfloat*),(target,pname,params));
GL_FUNC_IMPL(void,glGetTexParameteriv,(GLenum target,GLenum pname,GLint* params),(GLenum,GLenum,GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glGetTransformFeedbackVarying,(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLsizei* size,GLenum* type,GLchar* name),(GLuint,GLuint,GLsizei,GLsizei*,GLsizei*,GLenum*,GLchar*),(program,index,bufSize,length,size,type,name));
GL_FUNC_IMPL(GLuint,glGetUniformBlockIndex,(GLuint program,const GLchar* uniformBlockName),(GLuint,const GLchar*),(program,uniformBlockName));
GL_FUNC_IMPL(void,glGetUniformIndices,(GLuint program,GLsizei uniformCount,const GLchar*const* uniformNames,GLuint* uniformIndices),(GLuint,GLsizei,const GLchar*const*,GLuint*),(program,uniformCount,uniformNames,uniformIndices));
GL_FUNC_IMPL(GLint,glGetUniformLocation,(GLuint program,const GLchar* name),(GLuint,const GLchar*),(program,name));
GL_FUNC_IMPL(GLint,glGetUniformLocationARB,(GLhandleARB programObj,const GLcharARB* name),(GLhandleARB,const GLcharARB*),(programObj,name));
GL_FUNC_IMPL(void,glGetUniformSubroutineuiv,(GLenum shadertype,GLint location,GLuint* params),(GLenum,GLint,GLuint*),(shadertype,location,params));
GL_FUNC_IMPL(void,glGetUniformdv,(GLuint program,GLint location,GLdouble* params),(GLuint,GLint,GLdouble*),(program,location,params));
GL_FUNC_IMPL(void,glGetUniformfv,(GLuint program,GLint location,GLfloat* params),(GLuint,GLint,GLfloat*),(program,location,params));
GL_FUNC_IMPL(void,glGetUniformfvARB,(GLhandleARB programObj,GLint location,GLfloat* params),(GLhandleARB,GLint,GLfloat*),(programObj,location,params));
GL_FUNC_IMPL(void,glGetUniformiv,(GLuint program,GLint location,GLint* params),(GLuint,GLint,GLint*),(program,location,params));
GL_FUNC_IMPL(void,glGetUniformivARB,(GLhandleARB programObj,GLint location,GLint* params),(GLhandleARB,GLint,GLint*),(programObj,location,params));
GL_FUNC_IMPL(void,glGetUniformuiv,(GLuint program,GLint location,GLuint* params),(GLuint,GLint,GLuint*),(program,location,params));
GL_FUNC_IMPL(void,glGetVertexAttribIiv,(GLuint index,GLenum pname,GLint* params),(GLuint,GLenum,GLint*),(index,pname,params));
GL_FUNC_IMPL(void,glGetVertexAttribIuiv,(GLuint index,GLenum pname,GLuint* params),(GLuint,GLenum,GLuint*),(index,pname,params));
GL_FUNC_IMPL(void,glGetVertexAttribLdv,(GLuint index,GLenum pname,GLdouble* params),(GLuint,GLenum,GLdouble*),(index,pname,params));
GL_FUNC_IMPL(void,glGetVertexAttribPointerv,(GLuint index,GLenum pname,GLvoid** pointer),(GLuint,GLenum,GLvoid**),(index,pname,pointer));
GL_FUNC_IMPL(void,glGetVertexAttribPointervARB,(GLuint index,GLenum pname,GLvoid** pointer),(GLuint,GLenum,GLvoid**),(index,pname,pointer));
GL_FUNC_IMPL(void,glGetVertexAttribdv,(GLuint index,GLenum pname,GLdouble* params),(GLuint,GLenum,GLdouble*),(index,pname,params));
GL_FUNC_IMPL(void,glGetVertexAttribdvARB,(GLuint index,GLenum pname,GLdouble* params),(GLuint,GLenum,GLdouble*),(index,pname,params));
GL_FUNC_IMPL(void,glGetVertexAttribfv,(GLuint index,GLenum pname,GLfloat* params),(GLuint,GLenum,GLfloat*),(index,pname,params));
GL_FUNC_IMPL(void,glGetVertexAttribfvARB,(GLuint index,GLenum pname,GLfloat* params),(GLuint,GLenum,GLfloat*),(index,pname,params));
GL_FUNC_IMPL(void,glGetVertexAttribiv,(GLuint index,GLenum pname,GLint* params),(GLuint,GLenum,GLint*),(index,pname,params));
GL_FUNC_IMPL(void,glGetVertexAttribivARB,(GLuint index,GLenum pname,GLint* params),(GLuint,GLenum,GLint*),(index,pname,params));
GL_FUNC_IMPL(void,glHint,(GLenum target,GLenum mode),(GLenum,GLenum),(target,mode));
GL_FUNC_IMPL(void,glIndexMask,(GLuint mask),(GLuint),(mask));
GL_FUNC_IMPL(void,glIndexPointer,(GLenum type,GLsizei stride,const GLvoid* pointer),(GLenum,GLsizei,const GLvoid*),(type,stride,pointer));
GL_FUNC_IMPL(void,glIndexd,(GLdouble c),(GLdouble),(c));
GL_FUNC_IMPL(void,glIndexdv,(const GLdouble* c),(const GLdouble*),(c));
GL_FUNC_IMPL(void,glIndexf,(GLfloat c),(GLfloat),(c));
GL_FUNC_IMPL(void,glIndexfv,(const GLfloat* c),(const GLfloat*),(c));
GL_FUNC_IMPL(void,glIndexi,(GLint c),(GLint),(c));
GL_FUNC_IMPL(void,glIndexiv,(const GLint* c),(const GLint*),(c));
GL_FUNC_IMPL(void,glIndexs,(GLshort c),(GLshort),(c));
GL_FUNC_IMPL(void,glIndexsv,(const GLshort* c),(const GLshort*),(c));
GL_FUNC_IMPL(void,glIndexub,(GLubyte c),(GLubyte),(c));
GL_FUNC_IMPL(void,glIndexubv,(const GLubyte* c),(const GLubyte*),(c));
GL_FUNC_IMPL(void,glInitNames,(),(),());
GL_FUNC_IMPL(void,glInterleavedArrays,(GLenum format,GLsizei stride,const GLvoid* pointer),(GLenum,GLsizei,const GLvoid*),(format,stride,pointer));
GL_FUNC_IMPL(GLboolean,glIsBuffer,(GLuint buffer),(GLuint),(buffer));
GL_FUNC_IMPL(GLboolean,glIsBufferARB,(GLuint buffer),(GLuint),(buffer));
GL_FUNC_IMPL(GLboolean,glIsEnabled,(GLenum cap),(GLenum),(cap));
GL_FUNC_IMPL(GLboolean,glIsEnabledi,(GLenum target,GLuint index),(GLenum,GLuint),(target,index));
GL_FUNC_IMPL(GLboolean,glIsFramebuffer,(GLuint framebuffer),(GLuint),(framebuffer));
GL_FUNC_IMPL(GLboolean,glIsFramebufferEXT,(GLuint framebuffer),(GLuint),(framebuffer));
GL_FUNC_IMPL(GLboolean,glIsList,(GLuint list),(GLuint),(list));
GL_FUNC_IMPL(GLboolean,glIsProgram,(GLuint program),(GLuint),(program));
GL_FUNC_IMPL(GLboolean,glIsProgramARB,(GLuint program),(GLuint),(program));
GL_FUNC_IMPL(GLboolean,glIsProgramPipeline,(GLuint pipeline),(GLuint),(pipeline));
GL_FUNC_IMPL(GLboolean,glIsQuery,(GLuint id),(GLuint),(id));
GL_FUNC_IMPL(GLboolean,glIsRenderbuffer,(GLuint renderbuffer),(GLuint),(renderbuffer));
GL_FUNC_IMPL(GLboolean,glIsRenderbufferEXT,(GLuint renderbuffer),(GLuint),(renderbuffer));
GL_FUNC_IMPL(GLboolean,glIsSampler,(GLuint sampler),(GLuint),(sampler));
GL_FUNC_IMPL(GLboolean,glIsShader,(GLuint shader),(GLuint),(shader));
GL_FUNC_IMPL(GLboolean,glIsSync,(GLsync sync),(GLsync),(sync));
GL_FUNC_IMPL(GLboolean,glIsTexture,(GLuint texture),(GLuint),(texture));
GL_FUNC_IMPL(GLboolean,glIsTransformFeedback,(GLuint id),(GLuint),(id));
GL_FUNC_IMPL(GLboolean,glIsVertexArray,(GLuint ren_array),(GLuint),(ren_array));
GL_FUNC_IMPL(void,glLightModelf,(GLenum pname,GLfloat param),(GLenum,GLfloat),(pname,param));
GL_FUNC_IMPL(void,glLightModelfv,(GLenum pname,const GLfloat* params),(GLenum,const GLfloat*),(pname,params));
GL_FUNC_IMPL(void,glLightModeli,(GLenum pname,GLint param),(GLenum,GLint),(pname,param));
GL_FUNC_IMPL(void,glLightModeliv,(GLenum pname,const GLint* params),(GLenum,const GLint*),(pname,params));
GL_FUNC_IMPL(void,glLightf,(GLenum light,GLenum pname,GLfloat param),(GLenum,GLenum,GLfloat),(light,pname,param));
GL_FUNC_IMPL(void,glLightfv,(GLenum light,GLenum pname,const GLfloat* params),(GLenum,GLenum,const GLfloat*),(light,pname,params));
GL_FUNC_IMPL(void,glLighti,(GLenum light,GLenum pname,GLint param),(GLenum,GLenum,GLint),(light,pname,param));
GL_FUNC_IMPL(void,glLightiv,(GLenum light,GLenum pname,const GLint* params),(GLenum,GLenum,const GLint*),(light,pname,params));
GL_FUNC_IMPL(void,glLineStipple,(GLint factor,GLushort pattern),(GLint,GLushort),(factor,pattern));
GL_FUNC_IMPL(void,glLineWidth,(GLfloat width),(GLfloat),(width));
GL_FUNC_IMPL(void,glLinkProgram,(GLuint program),(GLuint),(program));
GL_FUNC_IMPL(void,glLinkProgramARB,(GLhandleARB programObj),(GLhandleARB),(programObj));
GL_FUNC_IMPL(void,glListBase,(GLuint base),(GLuint),(base));
GL_FUNC_IMPL(void,glLoadIdentity,(),(),());
GL_FUNC_IMPL(void,glLoadMatrixd,(const GLdouble* m),(const GLdouble*),(m));
GL_FUNC_IMPL(void,glLoadMatrixf,(const GLfloat* m),(const GLfloat*),(m));
GL_FUNC_IMPL(void,glLoadName,(GLuint name),(GLuint),(name));
GL_FUNC_IMPL(void,glLoadTransposeMatrixd,(const GLdouble* m),(const GLdouble*),(m));
GL_FUNC_IMPL(void,glLoadTransposeMatrixf,(const GLfloat* m),(const GLfloat*),(m));
GL_FUNC_IMPL(void,glLogicOp,(GLenum opcode),(GLenum),(opcode));
GL_FUNC_IMPL(void,glMap1d,(GLenum target,GLdouble u1,GLdouble u2,GLint stride,GLint order,const GLdouble* points),(GLenum,GLdouble,GLdouble,GLint,GLint,const GLdouble*),(target,u1,u2,stride,order,points));
GL_FUNC_IMPL(void,glMap1f,(GLenum target,GLfloat u1,GLfloat u2,GLint stride,GLint order,const GLfloat* points),(GLenum,GLfloat,GLfloat,GLint,GLint,const GLfloat*),(target,u1,u2,stride,order,points));
GL_FUNC_IMPL(void,glMap2d,(GLenum target,GLdouble u1,GLdouble u2,GLint ustride,GLint uorder,GLdouble v1,GLdouble v2,GLint vstride,GLint vorder,const GLdouble* points),(GLenum,GLdouble,GLdouble,GLint,GLint,GLdouble,GLdouble,GLint,GLint,const GLdouble*),(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points));
GL_FUNC_IMPL(void,glMap2f,(GLenum target,GLfloat u1,GLfloat u2,GLint ustride,GLint uorder,GLfloat v1,GLfloat v2,GLint vstride,GLint vorder,const GLfloat* points),(GLenum,GLfloat,GLfloat,GLint,GLint,GLfloat,GLfloat,GLint,GLint,const GLfloat*),(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points));
GL_FUNC_IMPL(void *,glMapBuffer,(GLenum target,GLenum access),(GLenum,GLenum),(target,access));
GL_FUNC_IMPL(void *,glMapBufferARB,(GLenum target,GLenum access),(GLenum,GLenum),(target,access));
GL_FUNC_IMPL(void *,glMapBufferRange,(GLenum target,GLintptr offset,GLsizeiptr length,GLbitfield access),(GLenum,GLintptr,GLsizeiptr,GLbitfield),(target,offset,length,access));
GL_FUNC_IMPL(void,glMapGrid1d,(GLint un,GLdouble u1,GLdouble u2),(GLint,GLdouble,GLdouble),(un,u1,u2));
GL_FUNC_IMPL(void,glMapGrid1f,(GLint un,GLfloat u1,GLfloat u2),(GLint,GLfloat,GLfloat),(un,u1,u2));
GL_FUNC_IMPL(void,glMapGrid2d,(GLint un,GLdouble u1,GLdouble u2,GLint vn,GLdouble v1,GLdouble v2),(GLint,GLdouble,GLdouble,GLint,GLdouble,GLdouble),(un,u1,u2,vn,v1,v2));
GL_FUNC_IMPL(void,glMapGrid2f,(GLint un,GLfloat u1,GLfloat u2,GLint vn,GLfloat v1,GLfloat v2),(GLint,GLfloat,GLfloat,GLint,GLfloat,GLfloat),(un,u1,u2,vn,v1,v2));
GL_FUNC_IMPL(void,glMaterialf,(GLenum face,GLenum pname,GLfloat param),(GLenum,GLenum,GLfloat),(face,pname,param));
GL_FUNC_IMPL(void,glMaterialfv,(GLenum face,GLenum pname,const GLfloat* params),(GLenum,GLenum,const GLfloat*),(face,pname,params));
GL_FUNC_IMPL(void,glMateriali,(GLenum face,GLenum pname,GLint param),(GLenum,GLenum,GLint),(face,pname,param));
GL_FUNC_IMPL(void,glMaterialiv,(GLenum face,GLenum pname,const GLint* params),(GLenum,GLenum,const GLint*),(face,pname,params));
GL_FUNC_IMPL(void,glMatrixMode,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glMemoryBarrier,(GLbitfield barriers),(GLbitfield),(barriers));
GL_FUNC_IMPL(void,glMinSampleShading,(GLfloat value),(GLfloat),(value));
GL_FUNC_IMPL(void,glMultMatrixd,(const GLdouble* m),(const GLdouble*),(m));
GL_FUNC_IMPL(void,glMultMatrixf,(const GLfloat* m),(const GLfloat*),(m));
GL_FUNC_IMPL(void,glMultTransposeMatrixd,(const GLdouble* m),(const GLdouble*),(m));
GL_FUNC_IMPL(void,glMultTransposeMatrixf,(const GLfloat* m),(const GLfloat*),(m));
GL_FUNC_IMPL(void,glMultiDrawArrays,(GLenum mode,const GLint* first,const GLsizei* count,GLsizei drawcount),(GLenum,const GLint*,const GLsizei*,GLsizei),(mode,first,count,drawcount));
GL_FUNC_IMPL(void,glMultiDrawElements,(GLenum mode,const GLsizei* count,GLenum type,const GLvoid*const* indices,GLsizei drawcount),(GLenum,const GLsizei*,GLenum,const GLvoid*const*,GLsizei),(mode,count,type,indices,drawcount));
GL_FUNC_IMPL(void,glMultiDrawElementsBaseVertex,(GLenum mode,const GLsizei* count,GLenum type,const GLvoid*const* indices,GLsizei drawcount,const GLint* basevertex),(GLenum,const GLsizei*,GLenum,const GLvoid*const*,GLsizei,const GLint*),(mode,count,type,indices,drawcount,basevertex));
GL_FUNC_IMPL(void,glMultiTexCoord1d,(GLenum target,GLdouble s),(GLenum,GLdouble),(target,s));
GL_FUNC_IMPL(void,glMultiTexCoord1dv,(GLenum target,const GLdouble* v),(GLenum,const GLdouble*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord1f,(GLenum target,GLfloat s),(GLenum,GLfloat),(target,s));
GL_FUNC_IMPL(void,glMultiTexCoord1fv,(GLenum target,const GLfloat* v),(GLenum,const GLfloat*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord1i,(GLenum target,GLint s),(GLenum,GLint),(target,s));
GL_FUNC_IMPL(void,glMultiTexCoord1iv,(GLenum target,const GLint* v),(GLenum,const GLint*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord1s,(GLenum target,GLshort s),(GLenum,GLshort),(target,s));
GL_FUNC_IMPL(void,glMultiTexCoord1sv,(GLenum target,const GLshort* v),(GLenum,const GLshort*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord2d,(GLenum target,GLdouble s,GLdouble t),(GLenum,GLdouble,GLdouble),(target,s,t));
GL_FUNC_IMPL(void,glMultiTexCoord2dv,(GLenum target,const GLdouble* v),(GLenum,const GLdouble*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord2f,(GLenum target,GLfloat s,GLfloat t),(GLenum,GLfloat,GLfloat),(target,s,t));
GL_FUNC_IMPL(void,glMultiTexCoord2fv,(GLenum target,const GLfloat* v),(GLenum,const GLfloat*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord2i,(GLenum target,GLint s,GLint t),(GLenum,GLint,GLint),(target,s,t));
GL_FUNC_IMPL(void,glMultiTexCoord2iv,(GLenum target,const GLint* v),(GLenum,const GLint*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord2s,(GLenum target,GLshort s,GLshort t),(GLenum,GLshort,GLshort),(target,s,t));
GL_FUNC_IMPL(void,glMultiTexCoord2sv,(GLenum target,const GLshort* v),(GLenum,const GLshort*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord3d,(GLenum target,GLdouble s,GLdouble t,GLdouble r),(GLenum,GLdouble,GLdouble,GLdouble),(target,s,t,r));
GL_FUNC_IMPL(void,glMultiTexCoord3dv,(GLenum target,const GLdouble* v),(GLenum,const GLdouble*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord3f,(GLenum target,GLfloat s,GLfloat t,GLfloat r),(GLenum,GLfloat,GLfloat,GLfloat),(target,s,t,r));
GL_FUNC_IMPL(void,glMultiTexCoord3fv,(GLenum target,const GLfloat* v),(GLenum,const GLfloat*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord3i,(GLenum target,GLint s,GLint t,GLint r),(GLenum,GLint,GLint,GLint),(target,s,t,r));
GL_FUNC_IMPL(void,glMultiTexCoord3iv,(GLenum target,const GLint* v),(GLenum,const GLint*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord3s,(GLenum target,GLshort s,GLshort t,GLshort r),(GLenum,GLshort,GLshort,GLshort),(target,s,t,r));
GL_FUNC_IMPL(void,glMultiTexCoord3sv,(GLenum target,const GLshort* v),(GLenum,const GLshort*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord4d,(GLenum target,GLdouble s,GLdouble t,GLdouble r,GLdouble q),(GLenum,GLdouble,GLdouble,GLdouble,GLdouble),(target,s,t,r,q));
GL_FUNC_IMPL(void,glMultiTexCoord4dv,(GLenum target,const GLdouble* v),(GLenum,const GLdouble*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord4f,(GLenum target,GLfloat s,GLfloat t,GLfloat r,GLfloat q),(GLenum,GLfloat,GLfloat,GLfloat,GLfloat),(target,s,t,r,q));
GL_FUNC_IMPL(void,glMultiTexCoord4fv,(GLenum target,const GLfloat* v),(GLenum,const GLfloat*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord4i,(GLenum target,GLint s,GLint t,GLint r,GLint q),(GLenum,GLint,GLint,GLint,GLint),(target,s,t,r,q));
GL_FUNC_IMPL(void,glMultiTexCoord4iv,(GLenum target,const GLint* v),(GLenum,const GLint*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoord4s,(GLenum target,GLshort s,GLshort t,GLshort r,GLshort q),(GLenum,GLshort,GLshort,GLshort,GLshort),(target,s,t,r,q));
GL_FUNC_IMPL(void,glMultiTexCoord4sv,(GLenum target,const GLshort* v),(GLenum,const GLshort*),(target,v));
GL_FUNC_IMPL(void,glMultiTexCoordP1ui,(GLenum texture,GLenum type,GLuint coords),(GLenum,GLenum,GLuint),(texture,type,coords));
GL_FUNC_IMPL(void,glMultiTexCoordP1uiv,(GLenum texture,GLenum type,const GLuint* coords),(GLenum,GLenum,const GLuint*),(texture,type,coords));
GL_FUNC_IMPL(void,glMultiTexCoordP2ui,(GLenum texture,GLenum type,GLuint coords),(GLenum,GLenum,GLuint),(texture,type,coords));
GL_FUNC_IMPL(void,glMultiTexCoordP2uiv,(GLenum texture,GLenum type,const GLuint* coords),(GLenum,GLenum,const GLuint*),(texture,type,coords));
GL_FUNC_IMPL(void,glMultiTexCoordP3ui,(GLenum texture,GLenum type,GLuint coords),(GLenum,GLenum,GLuint),(texture,type,coords));
GL_FUNC_IMPL(void,glMultiTexCoordP3uiv,(GLenum texture,GLenum type,const GLuint* coords),(GLenum,GLenum,const GLuint*),(texture,type,coords));
GL_FUNC_IMPL(void,glMultiTexCoordP4ui,(GLenum texture,GLenum type,GLuint coords),(GLenum,GLenum,GLuint),(texture,type,coords));
GL_FUNC_IMPL(void,glMultiTexCoordP4uiv,(GLenum texture,GLenum type,const GLuint* coords),(GLenum,GLenum,const GLuint*),(texture,type,coords));
GL_FUNC_IMPL(void,glNewList,(GLuint list,GLenum mode),(GLuint,GLenum),(list,mode));
GL_FUNC_IMPL(void,glNormal3b,(GLbyte nx,GLbyte ny,GLbyte nz),(GLbyte,GLbyte,GLbyte),(nx,ny,nz));
GL_FUNC_IMPL(void,glNormal3bv,(const GLbyte* v),(const GLbyte*),(v));
GL_FUNC_IMPL(void,glNormal3d,(GLdouble nx,GLdouble ny,GLdouble nz),(GLdouble,GLdouble,GLdouble),(nx,ny,nz));
GL_FUNC_IMPL(void,glNormal3dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glNormal3f,(GLfloat nx,GLfloat ny,GLfloat nz),(GLfloat,GLfloat,GLfloat),(nx,ny,nz));
GL_FUNC_IMPL(void,glNormal3fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glNormal3i,(GLint nx,GLint ny,GLint nz),(GLint,GLint,GLint),(nx,ny,nz));
GL_FUNC_IMPL(void,glNormal3iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glNormal3s,(GLshort nx,GLshort ny,GLshort nz),(GLshort,GLshort,GLshort),(nx,ny,nz));
GL_FUNC_IMPL(void,glNormal3sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glNormalP3ui,(GLenum type,GLuint coords),(GLenum,GLuint),(type,coords));
GL_FUNC_IMPL(void,glNormalP3uiv,(GLenum type,const GLuint* coords),(GLenum,const GLuint*),(type,coords));
GL_FUNC_IMPL(void,glNormalPointer,(GLenum type,GLsizei stride,const GLvoid* pointer),(GLenum,GLsizei,const GLvoid*),(type,stride,pointer));
GL_FUNC_IMPL(void,glOrtho,(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar),(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble),(left,right,bottom,top,zNear,zFar));
GL_FUNC_IMPL(void,glPassThrough,(GLfloat token),(GLfloat),(token));
GL_FUNC_IMPL(void,glPatchParameterfv,(GLenum pname,const GLfloat* values),(GLenum,const GLfloat*),(pname,values));
GL_FUNC_IMPL(void,glPatchParameteri,(GLenum pname,GLint value),(GLenum,GLint),(pname,value));
GL_FUNC_IMPL(void,glPauseTransformFeedback,(),(),());
GL_FUNC_IMPL(void,glPixelMapfv,(GLenum map,GLsizei mapsize,const GLfloat* values),(GLenum,GLsizei,const GLfloat*),(map,mapsize,values));
GL_FUNC_IMPL(void,glPixelMapuiv,(GLenum map,GLsizei mapsize,const GLuint* values),(GLenum,GLsizei,const GLuint*),(map,mapsize,values));
GL_FUNC_IMPL(void,glPixelMapusv,(GLenum map,GLsizei mapsize,const GLushort* values),(GLenum,GLsizei,const GLushort*),(map,mapsize,values));
GL_FUNC_IMPL(void,glPixelStoref,(GLenum pname,GLfloat param),(GLenum,GLfloat),(pname,param));
GL_FUNC_IMPL(void,glPixelStorei,(GLenum pname,GLint param),(GLenum,GLint),(pname,param));
GL_FUNC_IMPL(void,glPixelTransferf,(GLenum pname,GLfloat param),(GLenum,GLfloat),(pname,param));
GL_FUNC_IMPL(void,glPixelTransferi,(GLenum pname,GLint param),(GLenum,GLint),(pname,param));
GL_FUNC_IMPL(void,glPixelZoom,(GLfloat xfactor,GLfloat yfactor),(GLfloat,GLfloat),(xfactor,yfactor));
GL_FUNC_IMPL(void,glPointParameterf,(GLenum pname,GLfloat param),(GLenum,GLfloat),(pname,param));
GL_FUNC_IMPL(void,glPointParameterfv,(GLenum pname,const GLfloat* params),(GLenum,const GLfloat*),(pname,params));
GL_FUNC_IMPL(void,glPointParameteri,(GLenum pname,GLint param),(GLenum,GLint),(pname,param));
GL_FUNC_IMPL(void,glPointParameteriv,(GLenum pname,const GLint* params),(GLenum,const GLint*),(pname,params));
GL_FUNC_IMPL(void,glPointSize,(GLfloat size),(GLfloat),(size));
GL_FUNC_IMPL(void,glPolygonMode,(GLenum face,GLenum mode),(GLenum,GLenum),(face,mode));
GL_FUNC_IMPL(void,glPolygonOffset,(GLfloat factor,GLfloat units),(GLfloat,GLfloat),(factor,units));
GL_FUNC_IMPL(void,glPolygonStipple,(const GLubyte* mask),(const GLubyte*),(mask));
GL_FUNC_IMPL(void,glPopAttrib,(),(),());
GL_FUNC_IMPL(void,glPopClientAttrib,(),(),());
GL_FUNC_IMPL(void,glPopMatrix,(),(),());
GL_FUNC_IMPL(void,glPopName,(),(),());
GL_FUNC_IMPL(void,glPrimitiveRestartIndex,(GLuint index),(GLuint),(index));
GL_FUNC_IMPL(void,glPrioritizeTextures,(GLsizei n,const GLuint* textures,const GLfloat* priorities),(GLsizei,const GLuint*,const GLfloat*),(n,textures,priorities));
GL_FUNC_IMPL(void,glProgramBinary,(GLuint program,GLenum binaryFormat,const GLvoid* binary,GLsizei length),(GLuint,GLenum,const GLvoid*,GLsizei),(program,binaryFormat,binary,length));
GL_FUNC_IMPL(void,glProgramEnvParameter4dARB,(GLenum target,GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w),(GLenum,GLuint,GLdouble,GLdouble,GLdouble,GLdouble),(target,index,x,y,z,w));
GL_FUNC_IMPL(void,glProgramEnvParameter4dvARB,(GLenum target,GLuint index,const GLdouble* params),(GLenum,GLuint,const GLdouble*),(target,index,params));
GL_FUNC_IMPL(void,glProgramEnvParameter4fARB,(GLenum target,GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w),(GLenum,GLuint,GLfloat,GLfloat,GLfloat,GLfloat),(target,index,x,y,z,w));
GL_FUNC_IMPL(void,glProgramEnvParameter4fvARB,(GLenum target,GLuint index,const GLfloat* params),(GLenum,GLuint,const GLfloat*),(target,index,params));
GL_FUNC_IMPL(void,glProgramLocalParameter4dARB,(GLenum target,GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w),(GLenum,GLuint,GLdouble,GLdouble,GLdouble,GLdouble),(target,index,x,y,z,w));
GL_FUNC_IMPL(void,glProgramLocalParameter4dvARB,(GLenum target,GLuint index,const GLdouble* params),(GLenum,GLuint,const GLdouble*),(target,index,params));
GL_FUNC_IMPL(void,glProgramLocalParameter4fARB,(GLenum target,GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w),(GLenum,GLuint,GLfloat,GLfloat,GLfloat,GLfloat),(target,index,x,y,z,w));
GL_FUNC_IMPL(void,glProgramLocalParameter4fvARB,(GLenum target,GLuint index,const GLfloat* params),(GLenum,GLuint,const GLfloat*),(target,index,params));
GL_FUNC_IMPL(void,glProgramParameteri,(GLuint program,GLenum pname,GLint value),(GLuint,GLenum,GLint),(program,pname,value));
GL_FUNC_IMPL(void,glProgramStringARB,(GLenum target,GLenum format,GLsizei len,const GLvoid* string),(GLenum,GLenum,GLsizei,const GLvoid*),(target,format,len,string));
GL_FUNC_IMPL(void,glProgramUniform1d,(GLuint program,GLint location,GLdouble v0),(GLuint,GLint,GLdouble),(program,location,v0));
GL_FUNC_IMPL(void,glProgramUniform1dv,(GLuint program,GLint location,GLsizei count,const GLdouble* value),(GLuint,GLint,GLsizei,const GLdouble*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform1f,(GLuint program,GLint location,GLfloat v0),(GLuint,GLint,GLfloat),(program,location,v0));
GL_FUNC_IMPL(void,glProgramUniform1fv,(GLuint program,GLint location,GLsizei count,const GLfloat* value),(GLuint,GLint,GLsizei,const GLfloat*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform1i,(GLuint program,GLint location,GLint v0),(GLuint,GLint,GLint),(program,location,v0));
GL_FUNC_IMPL(void,glProgramUniform1iv,(GLuint program,GLint location,GLsizei count,const GLint* value),(GLuint,GLint,GLsizei,const GLint*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform1ui,(GLuint program,GLint location,GLuint v0),(GLuint,GLint,GLuint),(program,location,v0));
GL_FUNC_IMPL(void,glProgramUniform1uiv,(GLuint program,GLint location,GLsizei count,const GLuint* value),(GLuint,GLint,GLsizei,const GLuint*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform2d,(GLuint program,GLint location,GLdouble v0,GLdouble v1),(GLuint,GLint,GLdouble,GLdouble),(program,location,v0,v1));
GL_FUNC_IMPL(void,glProgramUniform2dv,(GLuint program,GLint location,GLsizei count,const GLdouble* value),(GLuint,GLint,GLsizei,const GLdouble*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform2f,(GLuint program,GLint location,GLfloat v0,GLfloat v1),(GLuint,GLint,GLfloat,GLfloat),(program,location,v0,v1));
GL_FUNC_IMPL(void,glProgramUniform2fv,(GLuint program,GLint location,GLsizei count,const GLfloat* value),(GLuint,GLint,GLsizei,const GLfloat*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform2i,(GLuint program,GLint location,GLint v0,GLint v1),(GLuint,GLint,GLint,GLint),(program,location,v0,v1));
GL_FUNC_IMPL(void,glProgramUniform2iv,(GLuint program,GLint location,GLsizei count,const GLint* value),(GLuint,GLint,GLsizei,const GLint*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform2ui,(GLuint program,GLint location,GLuint v0,GLuint v1),(GLuint,GLint,GLuint,GLuint),(program,location,v0,v1));
GL_FUNC_IMPL(void,glProgramUniform2uiv,(GLuint program,GLint location,GLsizei count,const GLuint* value),(GLuint,GLint,GLsizei,const GLuint*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform3d,(GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2),(GLuint,GLint,GLdouble,GLdouble,GLdouble),(program,location,v0,v1,v2));
GL_FUNC_IMPL(void,glProgramUniform3dv,(GLuint program,GLint location,GLsizei count,const GLdouble* value),(GLuint,GLint,GLsizei,const GLdouble*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform3f,(GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2),(GLuint,GLint,GLfloat,GLfloat,GLfloat),(program,location,v0,v1,v2));
GL_FUNC_IMPL(void,glProgramUniform3fv,(GLuint program,GLint location,GLsizei count,const GLfloat* value),(GLuint,GLint,GLsizei,const GLfloat*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform3i,(GLuint program,GLint location,GLint v0,GLint v1,GLint v2),(GLuint,GLint,GLint,GLint,GLint),(program,location,v0,v1,v2));
GL_FUNC_IMPL(void,glProgramUniform3iv,(GLuint program,GLint location,GLsizei count,const GLint* value),(GLuint,GLint,GLsizei,const GLint*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform3ui,(GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2),(GLuint,GLint,GLuint,GLuint,GLuint),(program,location,v0,v1,v2));
GL_FUNC_IMPL(void,glProgramUniform3uiv,(GLuint program,GLint location,GLsizei count,const GLuint* value),(GLuint,GLint,GLsizei,const GLuint*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform4d,(GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2,GLdouble v3),(GLuint,GLint,GLdouble,GLdouble,GLdouble,GLdouble),(program,location,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glProgramUniform4dv,(GLuint program,GLint location,GLsizei count,const GLdouble* value),(GLuint,GLint,GLsizei,const GLdouble*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform4f,(GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3),(GLuint,GLint,GLfloat,GLfloat,GLfloat,GLfloat),(program,location,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glProgramUniform4fv,(GLuint program,GLint location,GLsizei count,const GLfloat* value),(GLuint,GLint,GLsizei,const GLfloat*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform4i,(GLuint program,GLint location,GLint v0,GLint v1,GLint v2,GLint v3),(GLuint,GLint,GLint,GLint,GLint,GLint),(program,location,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glProgramUniform4iv,(GLuint program,GLint location,GLsizei count,const GLint* value),(GLuint,GLint,GLsizei,const GLint*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniform4ui,(GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3),(GLuint,GLint,GLuint,GLuint,GLuint,GLuint),(program,location,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glProgramUniform4uiv,(GLuint program,GLint location,GLsizei count,const GLuint* value),(GLuint,GLint,GLsizei,const GLuint*),(program,location,count,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix2dv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLuint,GLint,GLsizei,GLboolean,const GLdouble*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix2fv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLuint,GLint,GLsizei,GLboolean,const GLfloat*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix2x3dv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLuint,GLint,GLsizei,GLboolean,const GLdouble*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix2x3fv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLuint,GLint,GLsizei,GLboolean,const GLfloat*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix2x4dv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLuint,GLint,GLsizei,GLboolean,const GLdouble*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix2x4fv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLuint,GLint,GLsizei,GLboolean,const GLfloat*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix3dv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLuint,GLint,GLsizei,GLboolean,const GLdouble*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix3fv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLuint,GLint,GLsizei,GLboolean,const GLfloat*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix3x2dv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLuint,GLint,GLsizei,GLboolean,const GLdouble*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix3x2fv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLuint,GLint,GLsizei,GLboolean,const GLfloat*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix3x4dv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLuint,GLint,GLsizei,GLboolean,const GLdouble*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix3x4fv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLuint,GLint,GLsizei,GLboolean,const GLfloat*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix4dv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLuint,GLint,GLsizei,GLboolean,const GLdouble*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix4fv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLuint,GLint,GLsizei,GLboolean,const GLfloat*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix4x2dv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLuint,GLint,GLsizei,GLboolean,const GLdouble*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix4x2fv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLuint,GLint,GLsizei,GLboolean,const GLfloat*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix4x3dv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLuint,GLint,GLsizei,GLboolean,const GLdouble*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProgramUniformMatrix4x3fv,(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLuint,GLint,GLsizei,GLboolean,const GLfloat*),(program,location,count,transpose,value));
GL_FUNC_IMPL(void,glProvokingVertex,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glPushAttrib,(GLbitfield mask),(GLbitfield),(mask));
GL_FUNC_IMPL(void,glPushClientAttrib,(GLbitfield mask),(GLbitfield),(mask));
GL_FUNC_IMPL(void,glPushMatrix,(),(),());
GL_FUNC_IMPL(void,glPushName,(GLuint name),(GLuint),(name));
GL_FUNC_IMPL(void,glQueryCounter,(GLuint id,GLenum target),(GLuint,GLenum),(id,target));
GL_FUNC_IMPL(void,glRasterPos2d,(GLdouble x,GLdouble y),(GLdouble,GLdouble),(x,y));
GL_FUNC_IMPL(void,glRasterPos2dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glRasterPos2f,(GLfloat x,GLfloat y),(GLfloat,GLfloat),(x,y));
GL_FUNC_IMPL(void,glRasterPos2fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glRasterPos2i,(GLint x,GLint y),(GLint,GLint),(x,y));
GL_FUNC_IMPL(void,glRasterPos2iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glRasterPos2s,(GLshort x,GLshort y),(GLshort,GLshort),(x,y));
GL_FUNC_IMPL(void,glRasterPos2sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glRasterPos3d,(GLdouble x,GLdouble y,GLdouble z),(GLdouble,GLdouble,GLdouble),(x,y,z));
GL_FUNC_IMPL(void,glRasterPos3dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glRasterPos3f,(GLfloat x,GLfloat y,GLfloat z),(GLfloat,GLfloat,GLfloat),(x,y,z));
GL_FUNC_IMPL(void,glRasterPos3fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glRasterPos3i,(GLint x,GLint y,GLint z),(GLint,GLint,GLint),(x,y,z));
GL_FUNC_IMPL(void,glRasterPos3iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glRasterPos3s,(GLshort x,GLshort y,GLshort z),(GLshort,GLshort,GLshort),(x,y,z));
GL_FUNC_IMPL(void,glRasterPos3sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glRasterPos4d,(GLdouble x,GLdouble y,GLdouble z,GLdouble w),(GLdouble,GLdouble,GLdouble,GLdouble),(x,y,z,w));
GL_FUNC_IMPL(void,glRasterPos4dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glRasterPos4f,(GLfloat x,GLfloat y,GLfloat z,GLfloat w),(GLfloat,GLfloat,GLfloat,GLfloat),(x,y,z,w));
GL_FUNC_IMPL(void,glRasterPos4fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glRasterPos4i,(GLint x,GLint y,GLint z,GLint w),(GLint,GLint,GLint,GLint),(x,y,z,w));
GL_FUNC_IMPL(void,glRasterPos4iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glRasterPos4s,(GLshort x,GLshort y,GLshort z,GLshort w),(GLshort,GLshort,GLshort,GLshort),(x,y,z,w));
GL_FUNC_IMPL(void,glRasterPos4sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glReadBuffer,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glReadPixels,(GLint x,GLint y,GLsizei width,GLsizei height,GLenum format,GLenum type,GLvoid* pixels),(GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,GLvoid*),(x,y,width,height,format,type,pixels));
GL_FUNC_IMPL(void,glRectd,(GLdouble x1,GLdouble y1,GLdouble x2,GLdouble y2),(GLdouble,GLdouble,GLdouble,GLdouble),(x1,y1,x2,y2));
GL_FUNC_IMPL(void,glRectdv,(const GLdouble* v1,const GLdouble* v2),(const GLdouble*,const GLdouble*),(v1,v2));
GL_FUNC_IMPL(void,glRectf,(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2),(GLfloat,GLfloat,GLfloat,GLfloat),(x1,y1,x2,y2));
GL_FUNC_IMPL(void,glRectfv,(const GLfloat* v1,const GLfloat* v2),(const GLfloat*,const GLfloat*),(v1,v2));
GL_FUNC_IMPL(void,glRecti,(GLint x1,GLint y1,GLint x2,GLint y2),(GLint,GLint,GLint,GLint),(x1,y1,x2,y2));
GL_FUNC_IMPL(void,glRectiv,(const GLint* v1,const GLint* v2),(const GLint*,const GLint*),(v1,v2));
GL_FUNC_IMPL(void,glRects,(GLshort x1,GLshort y1,GLshort x2,GLshort y2),(GLshort,GLshort,GLshort,GLshort),(x1,y1,x2,y2));
GL_FUNC_IMPL(void,glRectsv,(const GLshort* v1,const GLshort* v2),(const GLshort*,const GLshort*),(v1,v2));
GL_FUNC_IMPL(void,glReleaseShaderCompiler,(),(),());
GL_FUNC_IMPL(GLint,glRenderMode,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glRenderbufferStorage,(GLenum target,GLenum internalformat,GLsizei width,GLsizei height),(GLenum,GLenum,GLsizei,GLsizei),(target,internalformat,width,height));
GL_FUNC_IMPL(void,glRenderbufferStorageEXT,(GLenum target,GLenum internalformat,GLsizei width,GLsizei height),(GLenum,GLenum,GLsizei,GLsizei),(target,internalformat,width,height));
GL_FUNC_IMPL(void,glRenderbufferStorageMultisample,(GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height),(GLenum,GLsizei,GLenum,GLsizei,GLsizei),(target,samples,internalformat,width,height));
GL_FUNC_IMPL(void,glResumeTransformFeedback,(),(),());
GL_FUNC_IMPL(void,glRotated,(GLdouble angle,GLdouble x,GLdouble y,GLdouble z),(GLdouble,GLdouble,GLdouble,GLdouble),(angle,x,y,z));
GL_FUNC_IMPL(void,glRotatef,(GLfloat angle,GLfloat x,GLfloat y,GLfloat z),(GLfloat,GLfloat,GLfloat,GLfloat),(angle,x,y,z));
GL_FUNC_IMPL(void,glSampleCoverage,(GLfloat value,GLboolean invert),(GLfloat,GLboolean),(value,invert));
GL_FUNC_IMPL(void,glSampleMaski,(GLuint index,GLbitfield mask),(GLuint,GLbitfield),(index,mask));
GL_FUNC_IMPL(void,glSamplerParameterIiv,(GLuint sampler,GLenum pname,const GLint* param),(GLuint,GLenum,const GLint*),(sampler,pname,param));
GL_FUNC_IMPL(void,glSamplerParameterIuiv,(GLuint sampler,GLenum pname,const GLuint* param),(GLuint,GLenum,const GLuint*),(sampler,pname,param));
GL_FUNC_IMPL(void,glSamplerParameterf,(GLuint sampler,GLenum pname,GLfloat param),(GLuint,GLenum,GLfloat),(sampler,pname,param));
GL_FUNC_IMPL(void,glSamplerParameterfv,(GLuint sampler,GLenum pname,const GLfloat* param),(GLuint,GLenum,const GLfloat*),(sampler,pname,param));
GL_FUNC_IMPL(void,glSamplerParameteri,(GLuint sampler,GLenum pname,GLint param),(GLuint,GLenum,GLint),(sampler,pname,param));
GL_FUNC_IMPL(void,glSamplerParameteriv,(GLuint sampler,GLenum pname,const GLint* param),(GLuint,GLenum,const GLint*),(sampler,pname,param));
GL_FUNC_IMPL(void,glScaled,(GLdouble x,GLdouble y,GLdouble z),(GLdouble,GLdouble,GLdouble),(x,y,z));
GL_FUNC_IMPL(void,glScalef,(GLfloat x,GLfloat y,GLfloat z),(GLfloat,GLfloat,GLfloat),(x,y,z));
GL_FUNC_IMPL(void,glScissor,(GLint x,GLint y,GLsizei width,GLsizei height),(GLint,GLint,GLsizei,GLsizei),(x,y,width,height));
GL_FUNC_IMPL(void,glScissorArrayv,(GLuint first,GLsizei count,const GLint* v),(GLuint,GLsizei,const GLint*),(first,count,v));
GL_FUNC_IMPL(void,glScissorIndexed,(GLuint index,GLint left,GLint bottom,GLsizei width,GLsizei height),(GLuint,GLint,GLint,GLsizei,GLsizei),(index,left,bottom,width,height));
GL_FUNC_IMPL(void,glScissorIndexedv,(GLuint index,const GLint* v),(GLuint,const GLint*),(index,v));
GL_FUNC_IMPL(void,glSecondaryColor3b,(GLbyte red,GLbyte green,GLbyte blue),(GLbyte,GLbyte,GLbyte),(red,green,blue));
GL_FUNC_IMPL(void,glSecondaryColor3bv,(const GLbyte* v),(const GLbyte*),(v));
GL_FUNC_IMPL(void,glSecondaryColor3d,(GLdouble red,GLdouble green,GLdouble blue),(GLdouble,GLdouble,GLdouble),(red,green,blue));
GL_FUNC_IMPL(void,glSecondaryColor3dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glSecondaryColor3f,(GLfloat red,GLfloat green,GLfloat blue),(GLfloat,GLfloat,GLfloat),(red,green,blue));
GL_FUNC_IMPL(void,glSecondaryColor3fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glSecondaryColor3i,(GLint red,GLint green,GLint blue),(GLint,GLint,GLint),(red,green,blue));
GL_FUNC_IMPL(void,glSecondaryColor3iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glSecondaryColor3s,(GLshort red,GLshort green,GLshort blue),(GLshort,GLshort,GLshort),(red,green,blue));
GL_FUNC_IMPL(void,glSecondaryColor3sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glSecondaryColor3ub,(GLubyte red,GLubyte green,GLubyte blue),(GLubyte,GLubyte,GLubyte),(red,green,blue));
GL_FUNC_IMPL(void,glSecondaryColor3ubv,(const GLubyte* v),(const GLubyte*),(v));
GL_FUNC_IMPL(void,glSecondaryColor3ui,(GLuint red,GLuint green,GLuint blue),(GLuint,GLuint,GLuint),(red,green,blue));
GL_FUNC_IMPL(void,glSecondaryColor3uiv,(const GLuint* v),(const GLuint*),(v));
GL_FUNC_IMPL(void,glSecondaryColor3us,(GLushort red,GLushort green,GLushort blue),(GLushort,GLushort,GLushort),(red,green,blue));
GL_FUNC_IMPL(void,glSecondaryColor3usv,(const GLushort* v),(const GLushort*),(v));
GL_FUNC_IMPL(void,glSecondaryColorP3ui,(GLenum type,GLuint color),(GLenum,GLuint),(type,color));
GL_FUNC_IMPL(void,glSecondaryColorP3uiv,(GLenum type,const GLuint* color),(GLenum,const GLuint*),(type,color));
GL_FUNC_IMPL(void,glSecondaryColorPointer,(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer),(GLint,GLenum,GLsizei,const GLvoid*),(size,type,stride,pointer));
GL_FUNC_IMPL(void,glSelectBuffer,(GLsizei size,GLuint* buffer),(GLsizei,GLuint*),(size,buffer));
GL_FUNC_IMPL(void,glShadeModel,(GLenum mode),(GLenum),(mode));
GL_FUNC_IMPL(void,glShaderBinary,(GLsizei count,const GLuint* shaders,GLenum binaryformat,const GLvoid* binary,GLsizei length),(GLsizei,const GLuint*,GLenum,const GLvoid*,GLsizei),(count,shaders,binaryformat,binary,length));
GL_FUNC_IMPL(void,glShaderSource,(GLuint shader,GLsizei count,const GLchar*const* string,const GLint* length),(GLuint,GLsizei,const GLchar*const*,const GLint*),(shader,count,string,length));
GL_FUNC_IMPL(void,glShaderSourceARB,(GLhandleARB shaderObj,GLsizei count,const GLcharARB** string,const GLint* length),(GLhandleARB,GLsizei,const GLcharARB**,const GLint*),(shaderObj,count,string,length));
GL_FUNC_IMPL(void,glStencilFunc,(GLenum func,GLint ref,GLuint mask),(GLenum,GLint,GLuint),(func,ref,mask));
GL_FUNC_IMPL(void,glStencilFuncSeparate,(GLenum face,GLenum func,GLint ref,GLuint mask),(GLenum,GLenum,GLint,GLuint),(face,func,ref,mask));
GL_FUNC_IMPL(void,glStencilMask,(GLuint mask),(GLuint),(mask));
GL_FUNC_IMPL(void,glStencilMaskSeparate,(GLenum face,GLuint mask),(GLenum,GLuint),(face,mask));
GL_FUNC_IMPL(void,glStencilOp,(GLenum fail,GLenum zfail,GLenum zpass),(GLenum,GLenum,GLenum),(fail,zfail,zpass));
GL_FUNC_IMPL(void,glStencilOpSeparate,(GLenum face,GLenum sfail,GLenum dpfail,GLenum dppass),(GLenum,GLenum,GLenum,GLenum),(face,sfail,dpfail,dppass));
GL_FUNC_IMPL(void,glTexBuffer,(GLenum target,GLenum internalformat,GLuint buffer),(GLenum,GLenum,GLuint),(target,internalformat,buffer));
GL_FUNC_IMPL(void,glTexCoord1d,(GLdouble s),(GLdouble),(s));
GL_FUNC_IMPL(void,glTexCoord1dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glTexCoord1f,(GLfloat s),(GLfloat),(s));
GL_FUNC_IMPL(void,glTexCoord1fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glTexCoord1i,(GLint s),(GLint),(s));
GL_FUNC_IMPL(void,glTexCoord1iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glTexCoord1s,(GLshort s),(GLshort),(s));
GL_FUNC_IMPL(void,glTexCoord1sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glTexCoord2d,(GLdouble s,GLdouble t),(GLdouble,GLdouble),(s,t));
GL_FUNC_IMPL(void,glTexCoord2dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glTexCoord2f,(GLfloat s,GLfloat t),(GLfloat,GLfloat),(s,t));
GL_FUNC_IMPL(void,glTexCoord2fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glTexCoord2i,(GLint s,GLint t),(GLint,GLint),(s,t));
GL_FUNC_IMPL(void,glTexCoord2iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glTexCoord2s,(GLshort s,GLshort t),(GLshort,GLshort),(s,t));
GL_FUNC_IMPL(void,glTexCoord2sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glTexCoord3d,(GLdouble s,GLdouble t,GLdouble r),(GLdouble,GLdouble,GLdouble),(s,t,r));
GL_FUNC_IMPL(void,glTexCoord3dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glTexCoord3f,(GLfloat s,GLfloat t,GLfloat r),(GLfloat,GLfloat,GLfloat),(s,t,r));
GL_FUNC_IMPL(void,glTexCoord3fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glTexCoord3i,(GLint s,GLint t,GLint r),(GLint,GLint,GLint),(s,t,r));
GL_FUNC_IMPL(void,glTexCoord3iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glTexCoord3s,(GLshort s,GLshort t,GLshort r),(GLshort,GLshort,GLshort),(s,t,r));
GL_FUNC_IMPL(void,glTexCoord3sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glTexCoord4d,(GLdouble s,GLdouble t,GLdouble r,GLdouble q),(GLdouble,GLdouble,GLdouble,GLdouble),(s,t,r,q));
GL_FUNC_IMPL(void,glTexCoord4dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glTexCoord4f,(GLfloat s,GLfloat t,GLfloat r,GLfloat q),(GLfloat,GLfloat,GLfloat,GLfloat),(s,t,r,q));
GL_FUNC_IMPL(void,glTexCoord4fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glTexCoord4i,(GLint s,GLint t,GLint r,GLint q),(GLint,GLint,GLint,GLint),(s,t,r,q));
GL_FUNC_IMPL(void,glTexCoord4iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glTexCoord4s,(GLshort s,GLshort t,GLshort r,GLshort q),(GLshort,GLshort,GLshort,GLshort),(s,t,r,q));
GL_FUNC_IMPL(void,glTexCoord4sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glTexCoordP1ui,(GLenum type,GLuint coords),(GLenum,GLuint),(type,coords));
GL_FUNC_IMPL(void,glTexCoordP1uiv,(GLenum type,const GLuint* coords),(GLenum,const GLuint*),(type,coords));
GL_FUNC_IMPL(void,glTexCoordP2ui,(GLenum type,GLuint coords),(GLenum,GLuint),(type,coords));
GL_FUNC_IMPL(void,glTexCoordP2uiv,(GLenum type,const GLuint* coords),(GLenum,const GLuint*),(type,coords));
GL_FUNC_IMPL(void,glTexCoordP3ui,(GLenum type,GLuint coords),(GLenum,GLuint),(type,coords));
GL_FUNC_IMPL(void,glTexCoordP3uiv,(GLenum type,const GLuint* coords),(GLenum,const GLuint*),(type,coords));
GL_FUNC_IMPL(void,glTexCoordP4ui,(GLenum type,GLuint coords),(GLenum,GLuint),(type,coords));
GL_FUNC_IMPL(void,glTexCoordP4uiv,(GLenum type,const GLuint* coords),(GLenum,const GLuint*),(type,coords));
GL_FUNC_IMPL(void,glTexCoordPointer,(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer),(GLint,GLenum,GLsizei,const GLvoid*),(size,type,stride,pointer));
GL_FUNC_IMPL(void,glTexEnvf,(GLenum target,GLenum pname,GLfloat param),(GLenum,GLenum,GLfloat),(target,pname,param));
GL_FUNC_IMPL(void,glTexEnvfv,(GLenum target,GLenum pname,const GLfloat* params),(GLenum,GLenum,const GLfloat*),(target,pname,params));
GL_FUNC_IMPL(void,glTexEnvi,(GLenum target,GLenum pname,GLint param),(GLenum,GLenum,GLint),(target,pname,param));
GL_FUNC_IMPL(void,glTexEnviv,(GLenum target,GLenum pname,const GLint* params),(GLenum,GLenum,const GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glTexGend,(GLenum coord,GLenum pname,GLdouble param),(GLenum,GLenum,GLdouble),(coord,pname,param));
GL_FUNC_IMPL(void,glTexGendv,(GLenum coord,GLenum pname,const GLdouble* params),(GLenum,GLenum,const GLdouble*),(coord,pname,params));
GL_FUNC_IMPL(void,glTexGenf,(GLenum coord,GLenum pname,GLfloat param),(GLenum,GLenum,GLfloat),(coord,pname,param));
GL_FUNC_IMPL(void,glTexGenfv,(GLenum coord,GLenum pname,const GLfloat* params),(GLenum,GLenum,const GLfloat*),(coord,pname,params));
GL_FUNC_IMPL(void,glTexGeni,(GLenum coord,GLenum pname,GLint param),(GLenum,GLenum,GLint),(coord,pname,param));
GL_FUNC_IMPL(void,glTexGeniv,(GLenum coord,GLenum pname,const GLint* params),(GLenum,GLenum,const GLint*),(coord,pname,params));
GL_FUNC_IMPL(void,glTexImage1D,(GLenum target,GLint level,GLint internalformat,GLsizei width,GLint border,GLenum format,GLenum type,const GLvoid* pixels),(GLenum,GLint,GLint,GLsizei,GLint,GLenum,GLenum,const GLvoid*),(target,level,internalformat,width,border,format,type,pixels));
GL_FUNC_IMPL(void,glTexImage2D,(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,GLenum format,GLenum type,const GLvoid* pixels),(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*),(target,level,internalformat,width,height,border,format,type,pixels));
GL_FUNC_IMPL(void,glTexImage2DMultisample,(GLenum target,GLsizei samples,GLint internalformat,GLsizei width,GLsizei height,GLboolean fixedsamplelocations),(GLenum,GLsizei,GLint,GLsizei,GLsizei,GLboolean),(target,samples,internalformat,width,height,fixedsamplelocations));
GL_FUNC_IMPL(void,glTexImage3D,(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLenum format,GLenum type,const GLvoid* pixels),(GLenum,GLint,GLint,GLsizei,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*),(target,level,internalformat,width,height,depth,border,format,type,pixels));
GL_FUNC_IMPL(void,glTexImage3DMultisample,(GLenum target,GLsizei samples,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLboolean fixedsamplelocations),(GLenum,GLsizei,GLint,GLsizei,GLsizei,GLsizei,GLboolean),(target,samples,internalformat,width,height,depth,fixedsamplelocations));
GL_FUNC_IMPL(void,glTexParameterIiv,(GLenum target,GLenum pname,const GLint* params),(GLenum,GLenum,const GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glTexParameterIuiv,(GLenum target,GLenum pname,const GLuint* params),(GLenum,GLenum,const GLuint*),(target,pname,params));
GL_FUNC_IMPL(void,glTexParameterf,(GLenum target,GLenum pname,GLfloat param),(GLenum,GLenum,GLfloat),(target,pname,param));
GL_FUNC_IMPL(void,glTexParameterfv,(GLenum target,GLenum pname,const GLfloat* params),(GLenum,GLenum,const GLfloat*),(target,pname,params));
GL_FUNC_IMPL(void,glTexParameteri,(GLenum target,GLenum pname,GLint param),(GLenum,GLenum,GLint),(target,pname,param));
GL_FUNC_IMPL(void,glTexParameteriv,(GLenum target,GLenum pname,const GLint* params),(GLenum,GLenum,const GLint*),(target,pname,params));
GL_FUNC_IMPL(void,glTexStorage1D,(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width),(GLenum,GLsizei,GLenum,GLsizei),(target,levels,internalformat,width));
GL_FUNC_IMPL(void,glTexStorage2D,(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height),(GLenum,GLsizei,GLenum,GLsizei,GLsizei),(target,levels,internalformat,width,height));
GL_FUNC_IMPL(void,glTexStorage3D,(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth),(GLenum,GLsizei,GLenum,GLsizei,GLsizei,GLsizei),(target,levels,internalformat,width,height,depth));
GL_FUNC_IMPL(void,glTexSubImage1D,(GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLenum type,const GLvoid* pixels),(GLenum,GLint,GLint,GLsizei,GLenum,GLenum,const GLvoid*),(target,level,xoffset,width,format,type,pixels));
GL_FUNC_IMPL(void,glTexSubImage2D,(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLenum type,const GLvoid* pixels),(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*),(target,level,xoffset,yoffset,width,height,format,type,pixels));
GL_FUNC_IMPL(void,glTexSubImage3D,(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,const GLvoid* pixels),(GLenum,GLint,GLint,GLint,GLint,GLsizei,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*),(target,level,xoffset,yoffset,zoffset,width,height,depth,format,type,pixels));
GL_FUNC_IMPL(void,glTransformFeedbackVaryings,(GLuint program,GLsizei count,const GLchar*const* varyings,GLenum bufferMode),(GLuint,GLsizei,const GLchar*const*,GLenum),(program,count,varyings,bufferMode));
GL_FUNC_IMPL(void,glTranslated,(GLdouble x,GLdouble y,GLdouble z),(GLdouble,GLdouble,GLdouble),(x,y,z));
GL_FUNC_IMPL(void,glTranslatef,(GLfloat x,GLfloat y,GLfloat z),(GLfloat,GLfloat,GLfloat),(x,y,z));
GL_FUNC_IMPL(void,glUniform1d,(GLint location,GLdouble x),(GLint,GLdouble),(location,x));
GL_FUNC_IMPL(void,glUniform1dv,(GLint location,GLsizei count,const GLdouble* value),(GLint,GLsizei,const GLdouble*),(location,count,value));
GL_FUNC_IMPL(void,glUniform1f,(GLint location,GLfloat v0),(GLint,GLfloat),(location,v0));
GL_FUNC_IMPL(void,glUniform1fARB,(GLint location,GLfloat v0),(GLint,GLfloat),(location,v0));
GL_FUNC_IMPL(void,glUniform1fv,(GLint location,GLsizei count,const GLfloat* value),(GLint,GLsizei,const GLfloat*),(location,count,value));
GL_FUNC_IMPL(void,glUniform1fvARB,(GLint location,GLsizei count,const GLfloat* value),(GLint,GLsizei,const GLfloat*),(location,count,value));
GL_FUNC_IMPL(void,glUniform1i,(GLint location,GLint v0),(GLint,GLint),(location,v0));
GL_FUNC_IMPL(void,glUniform1iARB,(GLint location,GLint v0),(GLint,GLint),(location,v0));
GL_FUNC_IMPL(void,glUniform1iv,(GLint location,GLsizei count,const GLint* value),(GLint,GLsizei,const GLint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform1ivARB,(GLint location,GLsizei count,const GLint* value),(GLint,GLsizei,const GLint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform1ui,(GLint location,GLuint v0),(GLint,GLuint),(location,v0));
GL_FUNC_IMPL(void,glUniform1uiv,(GLint location,GLsizei count,const GLuint* value),(GLint,GLsizei,const GLuint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform2d,(GLint location,GLdouble x,GLdouble y),(GLint,GLdouble,GLdouble),(location,x,y));
GL_FUNC_IMPL(void,glUniform2dv,(GLint location,GLsizei count,const GLdouble* value),(GLint,GLsizei,const GLdouble*),(location,count,value));
GL_FUNC_IMPL(void,glUniform2f,(GLint location,GLfloat v0,GLfloat v1),(GLint,GLfloat,GLfloat),(location,v0,v1));
GL_FUNC_IMPL(void,glUniform2fARB,(GLint location,GLfloat v0,GLfloat v1),(GLint,GLfloat,GLfloat),(location,v0,v1));
GL_FUNC_IMPL(void,glUniform2fv,(GLint location,GLsizei count,const GLfloat* value),(GLint,GLsizei,const GLfloat*),(location,count,value));
GL_FUNC_IMPL(void,glUniform2fvARB,(GLint location,GLsizei count,const GLfloat* value),(GLint,GLsizei,const GLfloat*),(location,count,value));
GL_FUNC_IMPL(void,glUniform2i,(GLint location,GLint v0,GLint v1),(GLint,GLint,GLint),(location,v0,v1));
GL_FUNC_IMPL(void,glUniform2iARB,(GLint location,GLint v0,GLint v1),(GLint,GLint,GLint),(location,v0,v1));
GL_FUNC_IMPL(void,glUniform2iv,(GLint location,GLsizei count,const GLint* value),(GLint,GLsizei,const GLint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform2ivARB,(GLint location,GLsizei count,const GLint* value),(GLint,GLsizei,const GLint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform2ui,(GLint location,GLuint v0,GLuint v1),(GLint,GLuint,GLuint),(location,v0,v1));
GL_FUNC_IMPL(void,glUniform2uiv,(GLint location,GLsizei count,const GLuint* value),(GLint,GLsizei,const GLuint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform3d,(GLint location,GLdouble x,GLdouble y,GLdouble z),(GLint,GLdouble,GLdouble,GLdouble),(location,x,y,z));
GL_FUNC_IMPL(void,glUniform3dv,(GLint location,GLsizei count,const GLdouble* value),(GLint,GLsizei,const GLdouble*),(location,count,value));
GL_FUNC_IMPL(void,glUniform3f,(GLint location,GLfloat v0,GLfloat v1,GLfloat v2),(GLint,GLfloat,GLfloat,GLfloat),(location,v0,v1,v2));
GL_FUNC_IMPL(void,glUniform3fARB,(GLint location,GLfloat v0,GLfloat v1,GLfloat v2),(GLint,GLfloat,GLfloat,GLfloat),(location,v0,v1,v2));
GL_FUNC_IMPL(void,glUniform3fv,(GLint location,GLsizei count,const GLfloat* value),(GLint,GLsizei,const GLfloat*),(location,count,value));
GL_FUNC_IMPL(void,glUniform3fvARB,(GLint location,GLsizei count,const GLfloat* value),(GLint,GLsizei,const GLfloat*),(location,count,value));
GL_FUNC_IMPL(void,glUniform3i,(GLint location,GLint v0,GLint v1,GLint v2),(GLint,GLint,GLint,GLint),(location,v0,v1,v2));
GL_FUNC_IMPL(void,glUniform3iARB,(GLint location,GLint v0,GLint v1,GLint v2),(GLint,GLint,GLint,GLint),(location,v0,v1,v2));
GL_FUNC_IMPL(void,glUniform3iv,(GLint location,GLsizei count,const GLint* value),(GLint,GLsizei,const GLint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform3ivARB,(GLint location,GLsizei count,const GLint* value),(GLint,GLsizei,const GLint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform3ui,(GLint location,GLuint v0,GLuint v1,GLuint v2),(GLint,GLuint,GLuint,GLuint),(location,v0,v1,v2));
GL_FUNC_IMPL(void,glUniform3uiv,(GLint location,GLsizei count,const GLuint* value),(GLint,GLsizei,const GLuint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform4d,(GLint location,GLdouble x,GLdouble y,GLdouble z,GLdouble w),(GLint,GLdouble,GLdouble,GLdouble,GLdouble),(location,x,y,z,w));
GL_FUNC_IMPL(void,glUniform4dv,(GLint location,GLsizei count,const GLdouble* value),(GLint,GLsizei,const GLdouble*),(location,count,value));
GL_FUNC_IMPL(void,glUniform4f,(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3),(GLint,GLfloat,GLfloat,GLfloat,GLfloat),(location,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glUniform4fARB,(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3),(GLint,GLfloat,GLfloat,GLfloat,GLfloat),(location,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glUniform4fv,(GLint location,GLsizei count,const GLfloat* value),(GLint,GLsizei,const GLfloat*),(location,count,value));
GL_FUNC_IMPL(void,glUniform4fvARB,(GLint location,GLsizei count,const GLfloat* value),(GLint,GLsizei,const GLfloat*),(location,count,value));
GL_FUNC_IMPL(void,glUniform4i,(GLint location,GLint v0,GLint v1,GLint v2,GLint v3),(GLint,GLint,GLint,GLint,GLint),(location,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glUniform4iARB,(GLint location,GLint v0,GLint v1,GLint v2,GLint v3),(GLint,GLint,GLint,GLint,GLint),(location,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glUniform4iv,(GLint location,GLsizei count,const GLint* value),(GLint,GLsizei,const GLint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform4ivARB,(GLint location,GLsizei count,const GLint* value),(GLint,GLsizei,const GLint*),(location,count,value));
GL_FUNC_IMPL(void,glUniform4ui,(GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3),(GLint,GLuint,GLuint,GLuint,GLuint),(location,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glUniform4uiv,(GLint location,GLsizei count,const GLuint* value),(GLint,GLsizei,const GLuint*),(location,count,value));
GL_FUNC_IMPL(void,glUniformBlockBinding,(GLuint program,GLuint uniformBlockIndex,GLuint uniformBlockBinding),(GLuint,GLuint,GLuint),(program,uniformBlockIndex,uniformBlockBinding));
GL_FUNC_IMPL(void,glUniformMatrix2dv,(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLint,GLsizei,GLboolean,const GLdouble*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix2fv,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix2fvARB,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix2x3dv,(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLint,GLsizei,GLboolean,const GLdouble*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix2x3fv,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix2x4dv,(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLint,GLsizei,GLboolean,const GLdouble*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix2x4fv,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix3dv,(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLint,GLsizei,GLboolean,const GLdouble*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix3fv,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix3fvARB,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix3x2dv,(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLint,GLsizei,GLboolean,const GLdouble*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix3x2fv,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix3x4dv,(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLint,GLsizei,GLboolean,const GLdouble*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix3x4fv,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix4dv,(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLint,GLsizei,GLboolean,const GLdouble*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix4fv,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix4fvARB,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix4x2dv,(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLint,GLsizei,GLboolean,const GLdouble*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix4x2fv,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix4x3dv,(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value),(GLint,GLsizei,GLboolean,const GLdouble*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformMatrix4x3fv,(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value),(GLint,GLsizei,GLboolean,const GLfloat*),(location,count,transpose,value));
GL_FUNC_IMPL(void,glUniformSubroutinesuiv,(GLenum shadertype,GLsizei count,const GLuint* indices),(GLenum,GLsizei,const GLuint*),(shadertype,count,indices));
GL_FUNC_IMPL(GLboolean,glUnmapBuffer,(GLenum target),(GLenum),(target));
GL_FUNC_IMPL(GLboolean,glUnmapBufferARB,(GLenum target),(GLenum),(target));
GL_FUNC_IMPL(void,glUseProgram,(GLuint program),(GLuint),(program));
GL_FUNC_IMPL(void,glUseProgramObjectARB,(GLhandleARB programObj),(GLhandleARB),(programObj));
GL_FUNC_IMPL(void,glUseProgramStages,(GLuint pipeline,GLbitfield stages,GLuint program),(GLuint,GLbitfield,GLuint),(pipeline,stages,program));
GL_FUNC_IMPL(void,glValidateProgram,(GLuint program),(GLuint),(program));
GL_FUNC_IMPL(void,glValidateProgramARB,(GLhandleARB programObj),(GLhandleARB),(programObj));
GL_FUNC_IMPL(void,glValidateProgramPipeline,(GLuint pipeline),(GLuint),(pipeline));
GL_FUNC_IMPL(void,glVertex2d,(GLdouble x,GLdouble y),(GLdouble,GLdouble),(x,y));
GL_FUNC_IMPL(void,glVertex2dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glVertex2f,(GLfloat x,GLfloat y),(GLfloat,GLfloat),(x,y));
GL_FUNC_IMPL(void,glVertex2fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glVertex2i,(GLint x,GLint y),(GLint,GLint),(x,y));
GL_FUNC_IMPL(void,glVertex2iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glVertex2s,(GLshort x,GLshort y),(GLshort,GLshort),(x,y));
GL_FUNC_IMPL(void,glVertex2sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glVertex3d,(GLdouble x,GLdouble y,GLdouble z),(GLdouble,GLdouble,GLdouble),(x,y,z));
GL_FUNC_IMPL(void,glVertex3dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glVertex3f,(GLfloat x,GLfloat y,GLfloat z),(GLfloat,GLfloat,GLfloat),(x,y,z));
GL_FUNC_IMPL(void,glVertex3fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glVertex3i,(GLint x,GLint y,GLint z),(GLint,GLint,GLint),(x,y,z));
GL_FUNC_IMPL(void,glVertex3iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glVertex3s,(GLshort x,GLshort y,GLshort z),(GLshort,GLshort,GLshort),(x,y,z));
GL_FUNC_IMPL(void,glVertex3sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glVertex4d,(GLdouble x,GLdouble y,GLdouble z,GLdouble w),(GLdouble,GLdouble,GLdouble,GLdouble),(x,y,z,w));
GL_FUNC_IMPL(void,glVertex4dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glVertex4f,(GLfloat x,GLfloat y,GLfloat z,GLfloat w),(GLfloat,GLfloat,GLfloat,GLfloat),(x,y,z,w));
GL_FUNC_IMPL(void,glVertex4fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glVertex4i,(GLint x,GLint y,GLint z,GLint w),(GLint,GLint,GLint,GLint),(x,y,z,w));
GL_FUNC_IMPL(void,glVertex4iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glVertex4s,(GLshort x,GLshort y,GLshort z,GLshort w),(GLshort,GLshort,GLshort,GLshort),(x,y,z,w));
GL_FUNC_IMPL(void,glVertex4sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glVertexAttrib1d,(GLuint index,GLdouble x),(GLuint,GLdouble),(index,x));
GL_FUNC_IMPL(void,glVertexAttrib1dARB,(GLuint index,GLdouble v0),(GLuint,GLdouble),(index,v0));
GL_FUNC_IMPL(void,glVertexAttrib1dv,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib1dvARB,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib1f,(GLuint index,GLfloat x),(GLuint,GLfloat),(index,x));
GL_FUNC_IMPL(void,glVertexAttrib1fARB,(GLuint index,GLfloat v0),(GLuint,GLfloat),(index,v0));
GL_FUNC_IMPL(void,glVertexAttrib1fv,(GLuint index,const GLfloat* v),(GLuint,const GLfloat*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib1fvARB,(GLuint index,const GLfloat* v),(GLuint,const GLfloat*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib1s,(GLuint index,GLshort x),(GLuint,GLshort),(index,x));
GL_FUNC_IMPL(void,glVertexAttrib1sARB,(GLuint index,GLshort v0),(GLuint,GLshort),(index,v0));
GL_FUNC_IMPL(void,glVertexAttrib1sv,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib1svARB,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib2d,(GLuint index,GLdouble x,GLdouble y),(GLuint,GLdouble,GLdouble),(index,x,y));
GL_FUNC_IMPL(void,glVertexAttrib2dARB,(GLuint index,GLdouble v0,GLdouble v1),(GLuint,GLdouble,GLdouble),(index,v0,v1));
GL_FUNC_IMPL(void,glVertexAttrib2dv,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib2dvARB,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib2f,(GLuint index,GLfloat x,GLfloat y),(GLuint,GLfloat,GLfloat),(index,x,y));
GL_FUNC_IMPL(void,glVertexAttrib2fARB,(GLuint index,GLfloat v0,GLfloat v1),(GLuint,GLfloat,GLfloat),(index,v0,v1));
GL_FUNC_IMPL(void,glVertexAttrib2fv,(GLuint index,const GLfloat* v),(GLuint,const GLfloat*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib2fvARB,(GLuint index,const GLfloat* v),(GLuint,const GLfloat*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib2s,(GLuint index,GLshort x,GLshort y),(GLuint,GLshort,GLshort),(index,x,y));
GL_FUNC_IMPL(void,glVertexAttrib2sARB,(GLuint index,GLshort v0,GLshort v1),(GLuint,GLshort,GLshort),(index,v0,v1));
GL_FUNC_IMPL(void,glVertexAttrib2sv,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib2svARB,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib3d,(GLuint index,GLdouble x,GLdouble y,GLdouble z),(GLuint,GLdouble,GLdouble,GLdouble),(index,x,y,z));
GL_FUNC_IMPL(void,glVertexAttrib3dARB,(GLuint index,GLdouble v0,GLdouble v1,GLdouble v2),(GLuint,GLdouble,GLdouble,GLdouble),(index,v0,v1,v2));
GL_FUNC_IMPL(void,glVertexAttrib3dv,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib3dvARB,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib3f,(GLuint index,GLfloat x,GLfloat y,GLfloat z),(GLuint,GLfloat,GLfloat,GLfloat),(index,x,y,z));
GL_FUNC_IMPL(void,glVertexAttrib3fARB,(GLuint index,GLfloat v0,GLfloat v1,GLfloat v2),(GLuint,GLfloat,GLfloat,GLfloat),(index,v0,v1,v2));
GL_FUNC_IMPL(void,glVertexAttrib3fv,(GLuint index,const GLfloat* v),(GLuint,const GLfloat*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib3fvARB,(GLuint index,const GLfloat* v),(GLuint,const GLfloat*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib3s,(GLuint index,GLshort x,GLshort y,GLshort z),(GLuint,GLshort,GLshort,GLshort),(index,x,y,z));
GL_FUNC_IMPL(void,glVertexAttrib3sARB,(GLuint index,GLshort v0,GLshort v1,GLshort v2),(GLuint,GLshort,GLshort,GLshort),(index,v0,v1,v2));
GL_FUNC_IMPL(void,glVertexAttrib3sv,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib3svARB,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4Nbv,(GLuint index,const GLbyte* v),(GLuint,const GLbyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4NbvARB,(GLuint index,const GLbyte* v),(GLuint,const GLbyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4Niv,(GLuint index,const GLint* v),(GLuint,const GLint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4NivARB,(GLuint index,const GLint* v),(GLuint,const GLint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4Nsv,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4NsvARB,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4Nub,(GLuint index,GLubyte x,GLubyte y,GLubyte z,GLubyte w),(GLuint,GLubyte,GLubyte,GLubyte,GLubyte),(index,x,y,z,w));
GL_FUNC_IMPL(void,glVertexAttrib4NubARB,(GLuint index,GLubyte x,GLubyte y,GLubyte z,GLubyte w),(GLuint,GLubyte,GLubyte,GLubyte,GLubyte),(index,x,y,z,w));
GL_FUNC_IMPL(void,glVertexAttrib4Nubv,(GLuint index,const GLubyte* v),(GLuint,const GLubyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4NubvARB,(GLuint index,const GLubyte* v),(GLuint,const GLubyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4Nuiv,(GLuint index,const GLuint* v),(GLuint,const GLuint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4NuivARB,(GLuint index,const GLuint* v),(GLuint,const GLuint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4Nusv,(GLuint index,const GLushort* v),(GLuint,const GLushort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4NusvARB,(GLuint index,const GLushort* v),(GLuint,const GLushort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4bv,(GLuint index,const GLbyte* v),(GLuint,const GLbyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4bvARB,(GLuint index,const GLbyte* v),(GLuint,const GLbyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4d,(GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w),(GLuint,GLdouble,GLdouble,GLdouble,GLdouble),(index,x,y,z,w));
GL_FUNC_IMPL(void,glVertexAttrib4dARB,(GLuint index,GLdouble v0,GLdouble v1,GLdouble v2,GLdouble v3),(GLuint,GLdouble,GLdouble,GLdouble,GLdouble),(index,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glVertexAttrib4dv,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4dvARB,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4f,(GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w),(GLuint,GLfloat,GLfloat,GLfloat,GLfloat),(index,x,y,z,w));
GL_FUNC_IMPL(void,glVertexAttrib4fARB,(GLuint index,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3),(GLuint,GLfloat,GLfloat,GLfloat,GLfloat),(index,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glVertexAttrib4fv,(GLuint index,const GLfloat* v),(GLuint,const GLfloat*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4fvARB,(GLuint index,const GLfloat* v),(GLuint,const GLfloat*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4iv,(GLuint index,const GLint* v),(GLuint,const GLint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4ivARB,(GLuint index,const GLint* v),(GLuint,const GLint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4s,(GLuint index,GLshort x,GLshort y,GLshort z,GLshort w),(GLuint,GLshort,GLshort,GLshort,GLshort),(index,x,y,z,w));
GL_FUNC_IMPL(void,glVertexAttrib4sARB,(GLuint index,GLshort v0,GLshort v1,GLshort v2,GLshort v3),(GLuint,GLshort,GLshort,GLshort,GLshort),(index,v0,v1,v2,v3));
GL_FUNC_IMPL(void,glVertexAttrib4sv,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4svARB,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4ubv,(GLuint index,const GLubyte* v),(GLuint,const GLubyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4ubvARB,(GLuint index,const GLubyte* v),(GLuint,const GLubyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4uiv,(GLuint index,const GLuint* v),(GLuint,const GLuint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4uivARB,(GLuint index,const GLuint* v),(GLuint,const GLuint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4usv,(GLuint index,const GLushort* v),(GLuint,const GLushort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttrib4usvARB,(GLuint index,const GLushort* v),(GLuint,const GLushort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribDivisor,(GLuint index,GLuint divisor),(GLuint,GLuint),(index,divisor));
GL_FUNC_IMPL(void,glVertexAttribI1i,(GLuint index,GLint x),(GLuint,GLint),(index,x));
GL_FUNC_IMPL(void,glVertexAttribI1iv,(GLuint index,const GLint* v),(GLuint,const GLint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI1ui,(GLuint index,GLuint x),(GLuint,GLuint),(index,x));
GL_FUNC_IMPL(void,glVertexAttribI1uiv,(GLuint index,const GLuint* v),(GLuint,const GLuint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI2i,(GLuint index,GLint x,GLint y),(GLuint,GLint,GLint),(index,x,y));
GL_FUNC_IMPL(void,glVertexAttribI2iv,(GLuint index,const GLint* v),(GLuint,const GLint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI2ui,(GLuint index,GLuint x,GLuint y),(GLuint,GLuint,GLuint),(index,x,y));
GL_FUNC_IMPL(void,glVertexAttribI2uiv,(GLuint index,const GLuint* v),(GLuint,const GLuint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI3i,(GLuint index,GLint x,GLint y,GLint z),(GLuint,GLint,GLint,GLint),(index,x,y,z));
GL_FUNC_IMPL(void,glVertexAttribI3iv,(GLuint index,const GLint* v),(GLuint,const GLint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI3ui,(GLuint index,GLuint x,GLuint y,GLuint z),(GLuint,GLuint,GLuint,GLuint),(index,x,y,z));
GL_FUNC_IMPL(void,glVertexAttribI3uiv,(GLuint index,const GLuint* v),(GLuint,const GLuint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI4bv,(GLuint index,const GLbyte* v),(GLuint,const GLbyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI4i,(GLuint index,GLint x,GLint y,GLint z,GLint w),(GLuint,GLint,GLint,GLint,GLint),(index,x,y,z,w));
GL_FUNC_IMPL(void,glVertexAttribI4iv,(GLuint index,const GLint* v),(GLuint,const GLint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI4sv,(GLuint index,const GLshort* v),(GLuint,const GLshort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI4ubv,(GLuint index,const GLubyte* v),(GLuint,const GLubyte*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI4ui,(GLuint index,GLuint x,GLuint y,GLuint z,GLuint w),(GLuint,GLuint,GLuint,GLuint,GLuint),(index,x,y,z,w));
GL_FUNC_IMPL(void,glVertexAttribI4uiv,(GLuint index,const GLuint* v),(GLuint,const GLuint*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribI4usv,(GLuint index,const GLushort* v),(GLuint,const GLushort*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribIPointer,(GLuint index,GLint size,GLenum type,GLsizei stride,const GLvoid* pointer),(GLuint,GLint,GLenum,GLsizei,const GLvoid*),(index,size,type,stride,pointer));
GL_FUNC_IMPL(void,glVertexAttribL1d,(GLuint index,GLdouble x),(GLuint,GLdouble),(index,x));
GL_FUNC_IMPL(void,glVertexAttribL1dv,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribL2d,(GLuint index,GLdouble x,GLdouble y),(GLuint,GLdouble,GLdouble),(index,x,y));
GL_FUNC_IMPL(void,glVertexAttribL2dv,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribL3d,(GLuint index,GLdouble x,GLdouble y,GLdouble z),(GLuint,GLdouble,GLdouble,GLdouble),(index,x,y,z));
GL_FUNC_IMPL(void,glVertexAttribL3dv,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribL4d,(GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w),(GLuint,GLdouble,GLdouble,GLdouble,GLdouble),(index,x,y,z,w));
GL_FUNC_IMPL(void,glVertexAttribL4dv,(GLuint index,const GLdouble* v),(GLuint,const GLdouble*),(index,v));
GL_FUNC_IMPL(void,glVertexAttribLPointer,(GLuint index,GLint size,GLenum type,GLsizei stride,const GLvoid* pointer),(GLuint,GLint,GLenum,GLsizei,const GLvoid*),(index,size,type,stride,pointer));
GL_FUNC_IMPL(void,glVertexAttribP1ui,(GLuint index,GLenum type,GLboolean normalized,GLuint value),(GLuint,GLenum,GLboolean,GLuint),(index,type,normalized,value));
GL_FUNC_IMPL(void,glVertexAttribP1uiv,(GLuint index,GLenum type,GLboolean normalized,const GLuint* value),(GLuint,GLenum,GLboolean,const GLuint*),(index,type,normalized,value));
GL_FUNC_IMPL(void,glVertexAttribP2ui,(GLuint index,GLenum type,GLboolean normalized,GLuint value),(GLuint,GLenum,GLboolean,GLuint),(index,type,normalized,value));
GL_FUNC_IMPL(void,glVertexAttribP2uiv,(GLuint index,GLenum type,GLboolean normalized,const GLuint* value),(GLuint,GLenum,GLboolean,const GLuint*),(index,type,normalized,value));
GL_FUNC_IMPL(void,glVertexAttribP3ui,(GLuint index,GLenum type,GLboolean normalized,GLuint value),(GLuint,GLenum,GLboolean,GLuint),(index,type,normalized,value));
GL_FUNC_IMPL(void,glVertexAttribP3uiv,(GLuint index,GLenum type,GLboolean normalized,const GLuint* value),(GLuint,GLenum,GLboolean,const GLuint*),(index,type,normalized,value));
GL_FUNC_IMPL(void,glVertexAttribP4ui,(GLuint index,GLenum type,GLboolean normalized,GLuint value),(GLuint,GLenum,GLboolean,GLuint),(index,type,normalized,value));
GL_FUNC_IMPL(void,glVertexAttribP4uiv,(GLuint index,GLenum type,GLboolean normalized,const GLuint* value),(GLuint,GLenum,GLboolean,const GLuint*),(index,type,normalized,value));
GL_FUNC_IMPL(void,glVertexAttribPointer,(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const GLvoid* pointer),(GLuint,GLint,GLenum,GLboolean,GLsizei,const GLvoid*),(index,size,type,normalized,stride,pointer));
GL_FUNC_IMPL(void,glVertexAttribPointerARB,(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const GLvoid* pointer),(GLuint,GLint,GLenum,GLboolean,GLsizei,const GLvoid*),(index,size,type,normalized,stride,pointer));
GL_FUNC_IMPL(void,glVertexP2ui,(GLenum type,GLuint value),(GLenum,GLuint),(type,value));
GL_FUNC_IMPL(void,glVertexP2uiv,(GLenum type,const GLuint* value),(GLenum,const GLuint*),(type,value));
GL_FUNC_IMPL(void,glVertexP3ui,(GLenum type,GLuint value),(GLenum,GLuint),(type,value));
GL_FUNC_IMPL(void,glVertexP3uiv,(GLenum type,const GLuint* value),(GLenum,const GLuint*),(type,value));
GL_FUNC_IMPL(void,glVertexP4ui,(GLenum type,GLuint value),(GLenum,GLuint),(type,value));
GL_FUNC_IMPL(void,glVertexP4uiv,(GLenum type,const GLuint* value),(GLenum,const GLuint*),(type,value));
GL_FUNC_IMPL(void,glVertexPointer,(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer),(GLint,GLenum,GLsizei,const GLvoid*),(size,type,stride,pointer));
GL_FUNC_IMPL(void,glViewport,(GLint x,GLint y,GLsizei width,GLsizei height),(GLint,GLint,GLsizei,GLsizei),(x,y,width,height));
GL_FUNC_IMPL(void,glViewportArrayv,(GLuint first,GLsizei count,const GLfloat* v),(GLuint,GLsizei,const GLfloat*),(first,count,v));
GL_FUNC_IMPL(void,glViewportIndexedf,(GLuint index,GLfloat x,GLfloat y,GLfloat w,GLfloat h),(GLuint,GLfloat,GLfloat,GLfloat,GLfloat),(index,x,y,w,h));
GL_FUNC_IMPL(void,glViewportIndexedfv,(GLuint index,const GLfloat* v),(GLuint,const GLfloat*),(index,v));
GL_FUNC_IMPL(void,glWaitSync,(GLsync sync,GLbitfield flags,GLuint64 timeout),(GLsync,GLbitfield,GLuint64),(sync,flags,timeout));
GL_FUNC_IMPL(void,glWindowPos2d,(GLdouble x,GLdouble y),(GLdouble,GLdouble),(x,y));
GL_FUNC_IMPL(void,glWindowPos2dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glWindowPos2f,(GLfloat x,GLfloat y),(GLfloat,GLfloat),(x,y));
GL_FUNC_IMPL(void,glWindowPos2fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glWindowPos2i,(GLint x,GLint y),(GLint,GLint),(x,y));
GL_FUNC_IMPL(void,glWindowPos2iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glWindowPos2s,(GLshort x,GLshort y),(GLshort,GLshort),(x,y));
GL_FUNC_IMPL(void,glWindowPos2sv,(const GLshort* v),(const GLshort*),(v));
GL_FUNC_IMPL(void,glWindowPos3d,(GLdouble x,GLdouble y,GLdouble z),(GLdouble,GLdouble,GLdouble),(x,y,z));
GL_FUNC_IMPL(void,glWindowPos3dv,(const GLdouble* v),(const GLdouble*),(v));
GL_FUNC_IMPL(void,glWindowPos3f,(GLfloat x,GLfloat y,GLfloat z),(GLfloat,GLfloat,GLfloat),(x,y,z));
GL_FUNC_IMPL(void,glWindowPos3fv,(const GLfloat* v),(const GLfloat*),(v));
GL_FUNC_IMPL(void,glWindowPos3i,(GLint x,GLint y,GLint z),(GLint,GLint,GLint),(x,y,z));
GL_FUNC_IMPL(void,glWindowPos3iv,(const GLint* v),(const GLint*),(v));
GL_FUNC_IMPL(void,glWindowPos3s,(GLshort x,GLshort y,GLshort z),(GLshort,GLshort,GLshort),(x,y,z));
