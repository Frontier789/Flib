//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
#include <FRONTIER/OpenGL.hpp>
#include <FRONTIER/Config.hpp>
#include <iostream>

namespace fg
{
	fm::Log OpenGL_log = std::cout;
}

#include <cstdlib>
#include <string>

#ifdef FRONTIER_OS_MACOS
	#include <mach-o/dyld.h>

	static void *AppleGLGetProcAddress (const GLubyte *name)
	{
		static const struct mach_header* image = NULL;
		NSSymbol symbol;
		char* symbolName;
		if (NULL == image)
			image = NSAddImage("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", NSADDIMAGE_OPTION_RETURN_ON_ERROR);
		
		// prepend a '_' for the Unix C symbol mangling convention
		symbolName = malloc(strlen((const char*)name) + 2);
		strcpy(symbolName+1, (const char*)name);
		symbolName[0] = '_';
		symbol = image ? NSLookupSymbolInImage(image, symbolName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR) : NULL;
		free(symbolName);
		return symbol ? NSAddressOfSymbol(symbol) : NULL;
	}
	#define retrieveProcAddress(name) AppleGLGetProcAddress(name)
#endif // macOS

#if defined(FRONTIER_OS_SOLARIS) || defined(FRONTIER_OS_IRIX)
	#include <dlfcn.h>
	#include <stdio.h>
	
	static void *SunGetProcAddress (const GLubyte *name)
	{
	  static void* h = NULL;
	  static void* gpa;

	  if (h == NULL)
	  {
		if ((h = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL)) == NULL) return NULL;
		gpa = dlsym(h, "glXGetProcAddress");
	  }

	  if (gpa != NULL)
		return ((void*(API_ENTRY*)(const GLubyte*))gpa)(name);
	  else
		return dlsym(h, (const char*)name);
	}
	#define retrieveProcAddress(name) SunGetProcAddress(name)
#endif //Irix and Solaris

#ifdef FRONTIER_OS_ANDROID
	#include <dlfcn.h>
	
	class SharedObject
	{
	public:
		void *m_handle;
		SharedObject() : m_handle(NULL) {}
		~SharedObject()
		{
			if (m_handle)
				dlclose(m_handle);
		}
		bool load(const char *name)
		{
			m_handle = dlopen(name, RTLD_NOW|RTLD_LOCAL);
			return m_handle  != NULL;
		}
	};

	void *AndroidGetProcAddress(const char *name)
	{
		static SharedObject so;
		if (!so.load("libGLESv2.so"))
			if (!so.load("libGLES_CM.so"))
				if (!so.load("libGLESv1_CM.so"))
				{
					fg::OpenGL_log << "Error: No GLES shared object found" << std::endl;
					return NULL;
				}
		
		void *symbol = dlsym(so.m_handle, name);
		if (!symbol) 
		{
			// append an underscore for platforms that need that.
			unsigned int length=0;
			for (;name[length]!='\0';length++);
			length--;
			
			char *_name = new char[length+1];
			
			for (unsigned int i=0;name[i]!='\0';i++)
				_name[i+1]=name[i];
			_name[0] = '_';
			
			symbol = dlsym(so.m_handle, _name);
			
			delete[] _name;
			
			if (!symbol)
				fg::OpenGL_log << "Error: Couldn't find symbol " << name 
						   << " (" << (const char *)dlerror() << ")" <<std::endl;
		}
		return (symbol);
	}
	#define retrieveProcAddress(name) AndroidGetProcAddress(name)
#endif //Android

#ifdef FRONTIER_OS_WINDOWS
	#include <windows.h>

	#ifdef _MSC_VER
	#pragma warning(disable: 4055)
	#pragma warning(disable: 4054)
	#endif

	static int TestPointer(const PROC pTest)
	{
		ptrdiff_t iTest;
		if(!pTest) 
			return 0;
		iTest = (ptrdiff_t)pTest;
		
		if(iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1) return 0;
		
		return 1;
	}

	static PROC WinGetProcAddress(const char *name)
	{
		HMODULE glMod = NULL;
		PROC pFunc = wglGetProcAddress((LPCSTR)name);
		if(TestPointer(pFunc))
			return pFunc;
		
		glMod = GetModuleHandleA("OpenGL32.dll");
		return (PROC)GetProcAddress(glMod, (LPCSTR)name);
	}
	#define retrieveProcAddress(name) WinGetProcAddress(name)
#endif

#ifndef retrieveProcAddress //other
	#include <GL/glx.h>
	#define retrieveProcAddress(name) glXGetProcAddressARB((const GLubyte*)name)
#endif



static void API_ENTRY _choose_glAccum(GLenum op,GLfloat value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glAccum");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glAccum is NULL " << std::endl;
	else
	{
		_ptr_to_glAccum = (void(API_ENTRY*)(GLenum,GLfloat))gotPtr;
		return _ptr_to_glAccum(op,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glAccum)(GLenum op,GLfloat value) = _choose_glAccum;
static void API_ENTRY _choose_glActiveShaderProgram(GLuint pipeline,GLuint program)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glActiveShaderProgram");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glActiveShaderProgram is NULL " << std::endl;
	else
	{
		_ptr_to_glActiveShaderProgram = (void(API_ENTRY*)(GLuint,GLuint))gotPtr;
		return _ptr_to_glActiveShaderProgram(pipeline,program);
	}
	return;
}
void (API_ENTRY *_ptr_to_glActiveShaderProgram)(GLuint pipeline,GLuint program) = _choose_glActiveShaderProgram;
static void API_ENTRY _choose_glActiveTexture(GLenum texture)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glActiveTexture");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glActiveTexture is NULL " << std::endl;
	else
	{
		_ptr_to_glActiveTexture = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glActiveTexture(texture);
	}
	return;
}
void (API_ENTRY *_ptr_to_glActiveTexture)(GLenum texture) = _choose_glActiveTexture;
static void API_ENTRY _choose_glAlphaFunc(GLenum func,GLfloat ref)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glAlphaFunc");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glAlphaFunc is NULL " << std::endl;
	else
	{
		_ptr_to_glAlphaFunc = (void(API_ENTRY*)(GLenum,GLfloat))gotPtr;
		return _ptr_to_glAlphaFunc(func,ref);
	}
	return;
}
void (API_ENTRY *_ptr_to_glAlphaFunc)(GLenum func,GLfloat ref) = _choose_glAlphaFunc;
static GLboolean API_ENTRY _choose_glAreTexturesResident(GLsizei n,const GLuint* textures,GLboolean* residences)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glAreTexturesResident");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glAreTexturesResident is NULL " << std::endl;
	else
	{
		_ptr_to_glAreTexturesResident = (GLboolean(API_ENTRY*)(GLsizei,const GLuint*,GLboolean*))gotPtr;
		return _ptr_to_glAreTexturesResident(n,textures,residences);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glAreTexturesResident)(GLsizei n,const GLuint* textures,GLboolean* residences) = _choose_glAreTexturesResident;
static void API_ENTRY _choose_glArrayElement(GLint i)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glArrayElement");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glArrayElement is NULL " << std::endl;
	else
	{
		_ptr_to_glArrayElement = (void(API_ENTRY*)(GLint))gotPtr;
		return _ptr_to_glArrayElement(i);
	}
	return;
}
void (API_ENTRY *_ptr_to_glArrayElement)(GLint i) = _choose_glArrayElement;
static void API_ENTRY _choose_glAttachObjectARB(GLhandleARB containerObj,GLhandleARB obj)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glAttachObjectARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glAttachObjectARB is NULL " << std::endl;
	else
	{
		_ptr_to_glAttachObjectARB = (void(API_ENTRY*)(GLhandleARB,GLhandleARB))gotPtr;
		return _ptr_to_glAttachObjectARB(containerObj,obj);
	}
	return;
}
void (API_ENTRY *_ptr_to_glAttachObjectARB)(GLhandleARB containerObj,GLhandleARB obj) = _choose_glAttachObjectARB;
static void API_ENTRY _choose_glAttachShader(GLuint program,GLuint shader)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glAttachShader");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glAttachShader is NULL " << std::endl;
	else
	{
		_ptr_to_glAttachShader = (void(API_ENTRY*)(GLuint,GLuint))gotPtr;
		return _ptr_to_glAttachShader(program,shader);
	}
	return;
}
void (API_ENTRY *_ptr_to_glAttachShader)(GLuint program,GLuint shader) = _choose_glAttachShader;
static void API_ENTRY _choose_glBegin(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBegin");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBegin is NULL " << std::endl;
	else
	{
		_ptr_to_glBegin = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glBegin(mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBegin)(GLenum mode) = _choose_glBegin;
static void API_ENTRY _choose_glBeginConditionalRender(GLuint id,GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBeginConditionalRender");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBeginConditionalRender is NULL " << std::endl;
	else
	{
		_ptr_to_glBeginConditionalRender = (void(API_ENTRY*)(GLuint,GLenum))gotPtr;
		return _ptr_to_glBeginConditionalRender(id,mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBeginConditionalRender)(GLuint id,GLenum mode) = _choose_glBeginConditionalRender;
static void API_ENTRY _choose_glBeginQuery(GLenum target,GLuint id)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBeginQuery");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBeginQuery is NULL " << std::endl;
	else
	{
		_ptr_to_glBeginQuery = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBeginQuery(target,id);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBeginQuery)(GLenum target,GLuint id) = _choose_glBeginQuery;
static void API_ENTRY _choose_glBeginQueryIndexed(GLenum target,GLuint index,GLuint id)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBeginQueryIndexed");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBeginQueryIndexed is NULL " << std::endl;
	else
	{
		_ptr_to_glBeginQueryIndexed = (void(API_ENTRY*)(GLenum,GLuint,GLuint))gotPtr;
		return _ptr_to_glBeginQueryIndexed(target,index,id);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBeginQueryIndexed)(GLenum target,GLuint index,GLuint id) = _choose_glBeginQueryIndexed;
static void API_ENTRY _choose_glBeginTransformFeedback(GLenum primitiveMode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBeginTransformFeedback");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBeginTransformFeedback is NULL " << std::endl;
	else
	{
		_ptr_to_glBeginTransformFeedback = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glBeginTransformFeedback(primitiveMode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBeginTransformFeedback)(GLenum primitiveMode) = _choose_glBeginTransformFeedback;
static void API_ENTRY _choose_glBindAttribLocation(GLuint program,GLuint index,const GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindAttribLocation");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindAttribLocation is NULL " << std::endl;
	else
	{
		_ptr_to_glBindAttribLocation = (void(API_ENTRY*)(GLuint,GLuint,const GLchar*))gotPtr;
		return _ptr_to_glBindAttribLocation(program,index,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindAttribLocation)(GLuint program,GLuint index,const GLchar* name) = _choose_glBindAttribLocation;
static void API_ENTRY _choose_glBindAttribLocationARB(GLhandleARB programObj,GLuint index,const GLcharARB* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindAttribLocationARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindAttribLocationARB is NULL " << std::endl;
	else
	{
		_ptr_to_glBindAttribLocationARB = (void(API_ENTRY*)(GLhandleARB,GLuint,const GLcharARB*))gotPtr;
		return _ptr_to_glBindAttribLocationARB(programObj,index,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindAttribLocationARB)(GLhandleARB programObj,GLuint index,const GLcharARB* name) = _choose_glBindAttribLocationARB;
static void API_ENTRY _choose_glBindBuffer(GLenum target,GLuint buffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindBuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindBuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glBindBuffer = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBindBuffer(target,buffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindBuffer)(GLenum target,GLuint buffer) = _choose_glBindBuffer;
static void API_ENTRY _choose_glBindBufferARB(GLenum target,GLuint buffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindBufferARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindBufferARB is NULL " << std::endl;
	else
	{
		_ptr_to_glBindBufferARB = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBindBufferARB(target,buffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindBufferARB)(GLenum target,GLuint buffer) = _choose_glBindBufferARB;
static void API_ENTRY _choose_glBindBufferBase(GLenum target,GLuint index,GLuint buffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindBufferBase");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindBufferBase is NULL " << std::endl;
	else
	{
		_ptr_to_glBindBufferBase = (void(API_ENTRY*)(GLenum,GLuint,GLuint))gotPtr;
		return _ptr_to_glBindBufferBase(target,index,buffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindBufferBase)(GLenum target,GLuint index,GLuint buffer) = _choose_glBindBufferBase;
static void API_ENTRY _choose_glBindBufferRange(GLenum target,GLuint index,GLuint buffer,GLintptr offset,GLsizeiptr size)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindBufferRange");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindBufferRange is NULL " << std::endl;
	else
	{
		_ptr_to_glBindBufferRange = (void(API_ENTRY*)(GLenum,GLuint,GLuint,GLintptr,GLsizeiptr))gotPtr;
		return _ptr_to_glBindBufferRange(target,index,buffer,offset,size);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindBufferRange)(GLenum target,GLuint index,GLuint buffer,GLintptr offset,GLsizeiptr size) = _choose_glBindBufferRange;
static void API_ENTRY _choose_glBindFragDataLocation(GLuint program,GLuint color,const GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindFragDataLocation");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindFragDataLocation is NULL " << std::endl;
	else
	{
		_ptr_to_glBindFragDataLocation = (void(API_ENTRY*)(GLuint,GLuint,const GLchar*))gotPtr;
		return _ptr_to_glBindFragDataLocation(program,color,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindFragDataLocation)(GLuint program,GLuint color,const GLchar* name) = _choose_glBindFragDataLocation;
static void API_ENTRY _choose_glBindFragDataLocationIndexed(GLuint program,GLuint colorNumber,GLuint index,const GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindFragDataLocationIndexed");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindFragDataLocationIndexed is NULL " << std::endl;
	else
	{
		_ptr_to_glBindFragDataLocationIndexed = (void(API_ENTRY*)(GLuint,GLuint,GLuint,const GLchar*))gotPtr;
		return _ptr_to_glBindFragDataLocationIndexed(program,colorNumber,index,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindFragDataLocationIndexed)(GLuint program,GLuint colorNumber,GLuint index,const GLchar* name) = _choose_glBindFragDataLocationIndexed;
static void API_ENTRY _choose_glBindFramebuffer(GLenum target,GLuint framebuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindFramebuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindFramebuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glBindFramebuffer = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBindFramebuffer(target,framebuffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindFramebuffer)(GLenum target,GLuint framebuffer) = _choose_glBindFramebuffer;
static void API_ENTRY _choose_glBindFramebufferEXT(GLenum target,GLuint framebuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindFramebufferEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindFramebufferEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glBindFramebufferEXT = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBindFramebufferEXT(target,framebuffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindFramebufferEXT)(GLenum target,GLuint framebuffer) = _choose_glBindFramebufferEXT;
static void API_ENTRY _choose_glBindImageTexture(GLuint unit,GLuint texture,GLint level,GLboolean layered,GLint layer,GLenum access,GLenum format)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindImageTexture");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindImageTexture is NULL " << std::endl;
	else
	{
		_ptr_to_glBindImageTexture = (void(API_ENTRY*)(GLuint,GLuint,GLint,GLboolean,GLint,GLenum,GLenum))gotPtr;
		return _ptr_to_glBindImageTexture(unit,texture,level,layered,layer,access,format);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindImageTexture)(GLuint unit,GLuint texture,GLint level,GLboolean layered,GLint layer,GLenum access,GLenum format) = _choose_glBindImageTexture;
static void API_ENTRY _choose_glBindProgramARB(GLenum target,GLuint program)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindProgramARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindProgramARB is NULL " << std::endl;
	else
	{
		_ptr_to_glBindProgramARB = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBindProgramARB(target,program);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindProgramARB)(GLenum target,GLuint program) = _choose_glBindProgramARB;
static void API_ENTRY _choose_glBindProgramPipeline(GLuint pipeline)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindProgramPipeline");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindProgramPipeline is NULL " << std::endl;
	else
	{
		_ptr_to_glBindProgramPipeline = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glBindProgramPipeline(pipeline);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindProgramPipeline)(GLuint pipeline) = _choose_glBindProgramPipeline;
static void API_ENTRY _choose_glBindRenderbuffer(GLenum target,GLuint renderbuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindRenderbuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindRenderbuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glBindRenderbuffer = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBindRenderbuffer(target,renderbuffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindRenderbuffer)(GLenum target,GLuint renderbuffer) = _choose_glBindRenderbuffer;
static void API_ENTRY _choose_glBindRenderbufferEXT(GLenum target,GLuint renderbuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindRenderbufferEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindRenderbufferEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glBindRenderbufferEXT = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBindRenderbufferEXT(target,renderbuffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindRenderbufferEXT)(GLenum target,GLuint renderbuffer) = _choose_glBindRenderbufferEXT;
static void API_ENTRY _choose_glBindSampler(GLuint unit,GLuint sampler)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindSampler");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindSampler is NULL " << std::endl;
	else
	{
		_ptr_to_glBindSampler = (void(API_ENTRY*)(GLuint,GLuint))gotPtr;
		return _ptr_to_glBindSampler(unit,sampler);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindSampler)(GLuint unit,GLuint sampler) = _choose_glBindSampler;
static void API_ENTRY _choose_glBindTexture(GLenum target,GLuint texture)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindTexture");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindTexture is NULL " << std::endl;
	else
	{
		_ptr_to_glBindTexture = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBindTexture(target,texture);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindTexture)(GLenum target,GLuint texture) = _choose_glBindTexture;
static void API_ENTRY _choose_glBindTransformFeedback(GLenum target,GLuint id)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindTransformFeedback");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindTransformFeedback is NULL " << std::endl;
	else
	{
		_ptr_to_glBindTransformFeedback = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glBindTransformFeedback(target,id);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindTransformFeedback)(GLenum target,GLuint id) = _choose_glBindTransformFeedback;
static void API_ENTRY _choose_glBindVertexArray(GLuint ren_array)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBindVertexArray");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBindVertexArray is NULL " << std::endl;
	else
	{
		_ptr_to_glBindVertexArray = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glBindVertexArray(ren_array);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBindVertexArray)(GLuint ren_array) = _choose_glBindVertexArray;
static void API_ENTRY _choose_glBitmap(GLsizei width,GLsizei height,GLfloat xorig,GLfloat yorig,GLfloat xmove,GLfloat ymove,const GLubyte* bitmap)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBitmap");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBitmap is NULL " << std::endl;
	else
	{
		_ptr_to_glBitmap = (void(API_ENTRY*)(GLsizei,GLsizei,GLfloat,GLfloat,GLfloat,GLfloat,const GLubyte*))gotPtr;
		return _ptr_to_glBitmap(width,height,xorig,yorig,xmove,ymove,bitmap);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBitmap)(GLsizei width,GLsizei height,GLfloat xorig,GLfloat yorig,GLfloat xmove,GLfloat ymove,const GLubyte* bitmap) = _choose_glBitmap;
static void API_ENTRY _choose_glBlendColor(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlendColor");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlendColor is NULL " << std::endl;
	else
	{
		_ptr_to_glBlendColor = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glBlendColor(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlendColor)(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha) = _choose_glBlendColor;
static void API_ENTRY _choose_glBlendEquation(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlendEquation");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlendEquation is NULL " << std::endl;
	else
	{
		_ptr_to_glBlendEquation = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glBlendEquation(mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlendEquation)(GLenum mode) = _choose_glBlendEquation;
static void API_ENTRY _choose_glBlendEquationSeparate(GLenum modeRGB,GLenum modeAlpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlendEquationSeparate");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlendEquationSeparate is NULL " << std::endl;
	else
	{
		_ptr_to_glBlendEquationSeparate = (void(API_ENTRY*)(GLenum,GLenum))gotPtr;
		return _ptr_to_glBlendEquationSeparate(modeRGB,modeAlpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlendEquationSeparate)(GLenum modeRGB,GLenum modeAlpha) = _choose_glBlendEquationSeparate;
static void API_ENTRY _choose_glBlendEquationSeparatei(GLuint buf,GLenum modeRGB,GLenum modeAlpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlendEquationSeparatei");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlendEquationSeparatei is NULL " << std::endl;
	else
	{
		_ptr_to_glBlendEquationSeparatei = (void(API_ENTRY*)(GLuint,GLenum,GLenum))gotPtr;
		return _ptr_to_glBlendEquationSeparatei(buf,modeRGB,modeAlpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlendEquationSeparatei)(GLuint buf,GLenum modeRGB,GLenum modeAlpha) = _choose_glBlendEquationSeparatei;
static void API_ENTRY _choose_glBlendEquationi(GLuint buf,GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlendEquationi");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlendEquationi is NULL " << std::endl;
	else
	{
		_ptr_to_glBlendEquationi = (void(API_ENTRY*)(GLuint,GLenum))gotPtr;
		return _ptr_to_glBlendEquationi(buf,mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlendEquationi)(GLuint buf,GLenum mode) = _choose_glBlendEquationi;
static void API_ENTRY _choose_glBlendFunc(GLenum sfactor,GLenum dfactor)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlendFunc");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlendFunc is NULL " << std::endl;
	else
	{
		_ptr_to_glBlendFunc = (void(API_ENTRY*)(GLenum,GLenum))gotPtr;
		return _ptr_to_glBlendFunc(sfactor,dfactor);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlendFunc)(GLenum sfactor,GLenum dfactor) = _choose_glBlendFunc;
static void API_ENTRY _choose_glBlendFuncSeparate(GLenum sfactorRGB,GLenum dfactorRGB,GLenum sfactorAlpha,GLenum dfactorAlpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlendFuncSeparate");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlendFuncSeparate is NULL " << std::endl;
	else
	{
		_ptr_to_glBlendFuncSeparate = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLenum))gotPtr;
		return _ptr_to_glBlendFuncSeparate(sfactorRGB,dfactorRGB,sfactorAlpha,dfactorAlpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlendFuncSeparate)(GLenum sfactorRGB,GLenum dfactorRGB,GLenum sfactorAlpha,GLenum dfactorAlpha) = _choose_glBlendFuncSeparate;
static void API_ENTRY _choose_glBlendFuncSeparatei(GLuint buf,GLenum srcRGB,GLenum dstRGB,GLenum srcAlpha,GLenum dstAlpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlendFuncSeparatei");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlendFuncSeparatei is NULL " << std::endl;
	else
	{
		_ptr_to_glBlendFuncSeparatei = (void(API_ENTRY*)(GLuint,GLenum,GLenum,GLenum,GLenum))gotPtr;
		return _ptr_to_glBlendFuncSeparatei(buf,srcRGB,dstRGB,srcAlpha,dstAlpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlendFuncSeparatei)(GLuint buf,GLenum srcRGB,GLenum dstRGB,GLenum srcAlpha,GLenum dstAlpha) = _choose_glBlendFuncSeparatei;
static void API_ENTRY _choose_glBlendFunci(GLuint buf,GLenum src,GLenum dst)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlendFunci");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlendFunci is NULL " << std::endl;
	else
	{
		_ptr_to_glBlendFunci = (void(API_ENTRY*)(GLuint,GLenum,GLenum))gotPtr;
		return _ptr_to_glBlendFunci(buf,src,dst);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlendFunci)(GLuint buf,GLenum src,GLenum dst) = _choose_glBlendFunci;
static void API_ENTRY _choose_glBlitFramebuffer(GLint srcX0,GLint srcY0,GLint srcX1,GLint srcY1,GLint dstX0,GLint dstY0,GLint dstX1,GLint dstY1,GLbitfield mask,GLenum filter)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBlitFramebuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBlitFramebuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glBlitFramebuffer = (void(API_ENTRY*)(GLint,GLint,GLint,GLint,GLint,GLint,GLint,GLint,GLbitfield,GLenum))gotPtr;
		return _ptr_to_glBlitFramebuffer(srcX0,srcY0,srcX1,srcY1,dstX0,dstY0,dstX1,dstY1,mask,filter);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBlitFramebuffer)(GLint srcX0,GLint srcY0,GLint srcX1,GLint srcY1,GLint dstX0,GLint dstY0,GLint dstX1,GLint dstY1,GLbitfield mask,GLenum filter) = _choose_glBlitFramebuffer;
static void API_ENTRY _choose_glBufferData(GLenum target,GLsizeiptr size,const GLvoid* data,GLenum usage)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBufferData");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBufferData is NULL " << std::endl;
	else
	{
		_ptr_to_glBufferData = (void(API_ENTRY*)(GLenum,GLsizeiptr,const GLvoid*,GLenum))gotPtr;
		return _ptr_to_glBufferData(target,size,data,usage);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBufferData)(GLenum target,GLsizeiptr size,const GLvoid* data,GLenum usage) = _choose_glBufferData;
static void API_ENTRY _choose_glBufferDataARB(GLenum target,GLsizeiptrARB size,const GLvoid* data,GLenum usage)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBufferDataARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBufferDataARB is NULL " << std::endl;
	else
	{
		_ptr_to_glBufferDataARB = (void(API_ENTRY*)(GLenum,GLsizeiptrARB,const GLvoid*,GLenum))gotPtr;
		return _ptr_to_glBufferDataARB(target,size,data,usage);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBufferDataARB)(GLenum target,GLsizeiptrARB size,const GLvoid* data,GLenum usage) = _choose_glBufferDataARB;
static void API_ENTRY _choose_glBufferSubData(GLenum target,GLintptr offset,GLsizeiptr size,const GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBufferSubData");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBufferSubData is NULL " << std::endl;
	else
	{
		_ptr_to_glBufferSubData = (void(API_ENTRY*)(GLenum,GLintptr,GLsizeiptr,const GLvoid*))gotPtr;
		return _ptr_to_glBufferSubData(target,offset,size,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBufferSubData)(GLenum target,GLintptr offset,GLsizeiptr size,const GLvoid* data) = _choose_glBufferSubData;
static void API_ENTRY _choose_glBufferSubDataARB(GLenum target,GLintptrARB offset,GLsizeiptrARB size,const GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glBufferSubDataARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glBufferSubDataARB is NULL " << std::endl;
	else
	{
		_ptr_to_glBufferSubDataARB = (void(API_ENTRY*)(GLenum,GLintptrARB,GLsizeiptrARB,const GLvoid*))gotPtr;
		return _ptr_to_glBufferSubDataARB(target,offset,size,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glBufferSubDataARB)(GLenum target,GLintptrARB offset,GLsizeiptrARB size,const GLvoid* data) = _choose_glBufferSubDataARB;
static void API_ENTRY _choose_glCallList(GLuint list)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCallList");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCallList is NULL " << std::endl;
	else
	{
		_ptr_to_glCallList = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glCallList(list);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCallList)(GLuint list) = _choose_glCallList;
static void API_ENTRY _choose_glCallLists(GLsizei n,GLenum type,const GLvoid* lists)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCallLists");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCallLists is NULL " << std::endl;
	else
	{
		_ptr_to_glCallLists = (void(API_ENTRY*)(GLsizei,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glCallLists(n,type,lists);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCallLists)(GLsizei n,GLenum type,const GLvoid* lists) = _choose_glCallLists;
static GLenum API_ENTRY _choose_glCheckFramebufferStatus(GLenum target)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCheckFramebufferStatus");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCheckFramebufferStatus is NULL " << std::endl;
	else
	{
		_ptr_to_glCheckFramebufferStatus = (GLenum(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glCheckFramebufferStatus(target);
	}
	typedef GLenum RET_TYPE;
	return RET_TYPE();
}
GLenum (API_ENTRY *_ptr_to_glCheckFramebufferStatus)(GLenum target) = _choose_glCheckFramebufferStatus;
static GLenum API_ENTRY _choose_glCheckFramebufferStatusEXT(GLenum target)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCheckFramebufferStatusEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCheckFramebufferStatusEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glCheckFramebufferStatusEXT = (GLenum(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glCheckFramebufferStatusEXT(target);
	}
	typedef GLenum RET_TYPE;
	return RET_TYPE();
}
GLenum (API_ENTRY *_ptr_to_glCheckFramebufferStatusEXT)(GLenum target) = _choose_glCheckFramebufferStatusEXT;
static void API_ENTRY _choose_glClampColor(GLenum target,GLenum clamp)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClampColor");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClampColor is NULL " << std::endl;
	else
	{
		_ptr_to_glClampColor = (void(API_ENTRY*)(GLenum,GLenum))gotPtr;
		return _ptr_to_glClampColor(target,clamp);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClampColor)(GLenum target,GLenum clamp) = _choose_glClampColor;
static void API_ENTRY _choose_glClear(GLbitfield mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClear");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClear is NULL " << std::endl;
	else
	{
		_ptr_to_glClear = (void(API_ENTRY*)(GLbitfield))gotPtr;
		return _ptr_to_glClear(mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClear)(GLbitfield mask) = _choose_glClear;
static void API_ENTRY _choose_glClearAccum(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearAccum");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearAccum is NULL " << std::endl;
	else
	{
		_ptr_to_glClearAccum = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glClearAccum(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearAccum)(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha) = _choose_glClearAccum;
static void API_ENTRY _choose_glClearBufferfi(GLenum buffer,GLint drawbuffer,GLfloat depth,GLint stencil)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearBufferfi");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearBufferfi is NULL " << std::endl;
	else
	{
		_ptr_to_glClearBufferfi = (void(API_ENTRY*)(GLenum,GLint,GLfloat,GLint))gotPtr;
		return _ptr_to_glClearBufferfi(buffer,drawbuffer,depth,stencil);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearBufferfi)(GLenum buffer,GLint drawbuffer,GLfloat depth,GLint stencil) = _choose_glClearBufferfi;
static void API_ENTRY _choose_glClearBufferfv(GLenum buffer,GLint drawbuffer,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearBufferfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearBufferfv is NULL " << std::endl;
	else
	{
		_ptr_to_glClearBufferfv = (void(API_ENTRY*)(GLenum,GLint,const GLfloat*))gotPtr;
		return _ptr_to_glClearBufferfv(buffer,drawbuffer,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearBufferfv)(GLenum buffer,GLint drawbuffer,const GLfloat* value) = _choose_glClearBufferfv;
static void API_ENTRY _choose_glClearBufferiv(GLenum buffer,GLint drawbuffer,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearBufferiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearBufferiv is NULL " << std::endl;
	else
	{
		_ptr_to_glClearBufferiv = (void(API_ENTRY*)(GLenum,GLint,const GLint*))gotPtr;
		return _ptr_to_glClearBufferiv(buffer,drawbuffer,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearBufferiv)(GLenum buffer,GLint drawbuffer,const GLint* value) = _choose_glClearBufferiv;
static void API_ENTRY _choose_glClearBufferuiv(GLenum buffer,GLint drawbuffer,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearBufferuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearBufferuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glClearBufferuiv = (void(API_ENTRY*)(GLenum,GLint,const GLuint*))gotPtr;
		return _ptr_to_glClearBufferuiv(buffer,drawbuffer,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearBufferuiv)(GLenum buffer,GLint drawbuffer,const GLuint* value) = _choose_glClearBufferuiv;
static void API_ENTRY _choose_glClearColor(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearColor");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearColor is NULL " << std::endl;
	else
	{
		_ptr_to_glClearColor = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glClearColor(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearColor)(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha) = _choose_glClearColor;
static void API_ENTRY _choose_glClearDepth(GLdouble depth)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearDepth");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearDepth is NULL " << std::endl;
	else
	{
		_ptr_to_glClearDepth = (void(API_ENTRY*)(GLdouble))gotPtr;
		return _ptr_to_glClearDepth(depth);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearDepth)(GLdouble depth) = _choose_glClearDepth;
static void API_ENTRY _choose_glClearDepthf(GLfloat d)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearDepthf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearDepthf is NULL " << std::endl;
	else
	{
		_ptr_to_glClearDepthf = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glClearDepthf(d);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearDepthf)(GLfloat d) = _choose_glClearDepthf;
static void API_ENTRY _choose_glClearIndex(GLfloat c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearIndex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearIndex is NULL " << std::endl;
	else
	{
		_ptr_to_glClearIndex = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glClearIndex(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearIndex)(GLfloat c) = _choose_glClearIndex;
static void API_ENTRY _choose_glClearStencil(GLint s)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClearStencil");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClearStencil is NULL " << std::endl;
	else
	{
		_ptr_to_glClearStencil = (void(API_ENTRY*)(GLint))gotPtr;
		return _ptr_to_glClearStencil(s);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClearStencil)(GLint s) = _choose_glClearStencil;
static void API_ENTRY _choose_glClientActiveTexture(GLenum texture)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClientActiveTexture");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClientActiveTexture is NULL " << std::endl;
	else
	{
		_ptr_to_glClientActiveTexture = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glClientActiveTexture(texture);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClientActiveTexture)(GLenum texture) = _choose_glClientActiveTexture;
static GLenum API_ENTRY _choose_glClientWaitSync(GLsync sync,GLbitfield flags,GLuint64 timeout)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClientWaitSync");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClientWaitSync is NULL " << std::endl;
	else
	{
		_ptr_to_glClientWaitSync = (GLenum(API_ENTRY*)(GLsync,GLbitfield,GLuint64))gotPtr;
		return _ptr_to_glClientWaitSync(sync,flags,timeout);
	}
	typedef GLenum RET_TYPE;
	return RET_TYPE();
}
GLenum (API_ENTRY *_ptr_to_glClientWaitSync)(GLsync sync,GLbitfield flags,GLuint64 timeout) = _choose_glClientWaitSync;
static void API_ENTRY _choose_glClipPlane(GLenum plane,const GLdouble* equation)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glClipPlane");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glClipPlane is NULL " << std::endl;
	else
	{
		_ptr_to_glClipPlane = (void(API_ENTRY*)(GLenum,const GLdouble*))gotPtr;
		return _ptr_to_glClipPlane(plane,equation);
	}
	return;
}
void (API_ENTRY *_ptr_to_glClipPlane)(GLenum plane,const GLdouble* equation) = _choose_glClipPlane;
static void API_ENTRY _choose_glColor3b(GLbyte red,GLbyte green,GLbyte blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3b");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3b is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3b = (void(API_ENTRY*)(GLbyte,GLbyte,GLbyte))gotPtr;
		return _ptr_to_glColor3b(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3b)(GLbyte red,GLbyte green,GLbyte blue) = _choose_glColor3b;
static void API_ENTRY _choose_glColor3bv(const GLbyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3bv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3bv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3bv = (void(API_ENTRY*)(const GLbyte*))gotPtr;
		return _ptr_to_glColor3bv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3bv)(const GLbyte* v) = _choose_glColor3bv;
static void API_ENTRY _choose_glColor3d(GLdouble red,GLdouble green,GLdouble blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3d is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glColor3d(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3d)(GLdouble red,GLdouble green,GLdouble blue) = _choose_glColor3d;
static void API_ENTRY _choose_glColor3dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glColor3dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3dv)(const GLdouble* v) = _choose_glColor3dv;
static void API_ENTRY _choose_glColor3f(GLfloat red,GLfloat green,GLfloat blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3f is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glColor3f(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3f)(GLfloat red,GLfloat green,GLfloat blue) = _choose_glColor3f;
static void API_ENTRY _choose_glColor3fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glColor3fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3fv)(const GLfloat* v) = _choose_glColor3fv;
static void API_ENTRY _choose_glColor3i(GLint red,GLint green,GLint blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3i is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3i = (void(API_ENTRY*)(GLint,GLint,GLint))gotPtr;
		return _ptr_to_glColor3i(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3i)(GLint red,GLint green,GLint blue) = _choose_glColor3i;
static void API_ENTRY _choose_glColor3iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glColor3iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3iv)(const GLint* v) = _choose_glColor3iv;
static void API_ENTRY _choose_glColor3s(GLshort red,GLshort green,GLshort blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3s is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3s = (void(API_ENTRY*)(GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glColor3s(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3s)(GLshort red,GLshort green,GLshort blue) = _choose_glColor3s;
static void API_ENTRY _choose_glColor3sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3sv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glColor3sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3sv)(const GLshort* v) = _choose_glColor3sv;
static void API_ENTRY _choose_glColor3ub(GLubyte red,GLubyte green,GLubyte blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3ub");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3ub is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3ub = (void(API_ENTRY*)(GLubyte,GLubyte,GLubyte))gotPtr;
		return _ptr_to_glColor3ub(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3ub)(GLubyte red,GLubyte green,GLubyte blue) = _choose_glColor3ub;
static void API_ENTRY _choose_glColor3ubv(const GLubyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3ubv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3ubv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3ubv = (void(API_ENTRY*)(const GLubyte*))gotPtr;
		return _ptr_to_glColor3ubv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3ubv)(const GLubyte* v) = _choose_glColor3ubv;
static void API_ENTRY _choose_glColor3ui(GLuint red,GLuint green,GLuint blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3ui = (void(API_ENTRY*)(GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glColor3ui(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3ui)(GLuint red,GLuint green,GLuint blue) = _choose_glColor3ui;
static void API_ENTRY _choose_glColor3uiv(const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3uiv = (void(API_ENTRY*)(const GLuint*))gotPtr;
		return _ptr_to_glColor3uiv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3uiv)(const GLuint* v) = _choose_glColor3uiv;
static void API_ENTRY _choose_glColor3us(GLushort red,GLushort green,GLushort blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3us");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3us is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3us = (void(API_ENTRY*)(GLushort,GLushort,GLushort))gotPtr;
		return _ptr_to_glColor3us(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3us)(GLushort red,GLushort green,GLushort blue) = _choose_glColor3us;
static void API_ENTRY _choose_glColor3usv(const GLushort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor3usv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor3usv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor3usv = (void(API_ENTRY*)(const GLushort*))gotPtr;
		return _ptr_to_glColor3usv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor3usv)(const GLushort* v) = _choose_glColor3usv;
static void API_ENTRY _choose_glColor4b(GLbyte red,GLbyte green,GLbyte blue,GLbyte alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4b");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4b is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4b = (void(API_ENTRY*)(GLbyte,GLbyte,GLbyte,GLbyte))gotPtr;
		return _ptr_to_glColor4b(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4b)(GLbyte red,GLbyte green,GLbyte blue,GLbyte alpha) = _choose_glColor4b;
static void API_ENTRY _choose_glColor4bv(const GLbyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4bv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4bv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4bv = (void(API_ENTRY*)(const GLbyte*))gotPtr;
		return _ptr_to_glColor4bv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4bv)(const GLbyte* v) = _choose_glColor4bv;
static void API_ENTRY _choose_glColor4d(GLdouble red,GLdouble green,GLdouble blue,GLdouble alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4d is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glColor4d(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4d)(GLdouble red,GLdouble green,GLdouble blue,GLdouble alpha) = _choose_glColor4d;
static void API_ENTRY _choose_glColor4dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glColor4dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4dv)(const GLdouble* v) = _choose_glColor4dv;
static void API_ENTRY _choose_glColor4f(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4f is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glColor4f(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4f)(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha) = _choose_glColor4f;
static void API_ENTRY _choose_glColor4fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glColor4fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4fv)(const GLfloat* v) = _choose_glColor4fv;
static void API_ENTRY _choose_glColor4i(GLint red,GLint green,GLint blue,GLint alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4i is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4i = (void(API_ENTRY*)(GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glColor4i(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4i)(GLint red,GLint green,GLint blue,GLint alpha) = _choose_glColor4i;
static void API_ENTRY _choose_glColor4iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4iv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glColor4iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4iv)(const GLint* v) = _choose_glColor4iv;
static void API_ENTRY _choose_glColor4s(GLshort red,GLshort green,GLshort blue,GLshort alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4s is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4s = (void(API_ENTRY*)(GLshort,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glColor4s(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4s)(GLshort red,GLshort green,GLshort blue,GLshort alpha) = _choose_glColor4s;
static void API_ENTRY _choose_glColor4sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4sv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glColor4sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4sv)(const GLshort* v) = _choose_glColor4sv;
static void API_ENTRY _choose_glColor4ub(GLubyte red,GLubyte green,GLubyte blue,GLubyte alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4ub");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4ub is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4ub = (void(API_ENTRY*)(GLubyte,GLubyte,GLubyte,GLubyte))gotPtr;
		return _ptr_to_glColor4ub(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4ub)(GLubyte red,GLubyte green,GLubyte blue,GLubyte alpha) = _choose_glColor4ub;
static void API_ENTRY _choose_glColor4ubv(const GLubyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4ubv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4ubv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4ubv = (void(API_ENTRY*)(const GLubyte*))gotPtr;
		return _ptr_to_glColor4ubv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4ubv)(const GLubyte* v) = _choose_glColor4ubv;
static void API_ENTRY _choose_glColor4ui(GLuint red,GLuint green,GLuint blue,GLuint alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4ui is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4ui = (void(API_ENTRY*)(GLuint,GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glColor4ui(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4ui)(GLuint red,GLuint green,GLuint blue,GLuint alpha) = _choose_glColor4ui;
static void API_ENTRY _choose_glColor4uiv(const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4uiv = (void(API_ENTRY*)(const GLuint*))gotPtr;
		return _ptr_to_glColor4uiv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4uiv)(const GLuint* v) = _choose_glColor4uiv;
static void API_ENTRY _choose_glColor4us(GLushort red,GLushort green,GLushort blue,GLushort alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4us");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4us is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4us = (void(API_ENTRY*)(GLushort,GLushort,GLushort,GLushort))gotPtr;
		return _ptr_to_glColor4us(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4us)(GLushort red,GLushort green,GLushort blue,GLushort alpha) = _choose_glColor4us;
static void API_ENTRY _choose_glColor4usv(const GLushort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColor4usv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColor4usv is NULL " << std::endl;
	else
	{
		_ptr_to_glColor4usv = (void(API_ENTRY*)(const GLushort*))gotPtr;
		return _ptr_to_glColor4usv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColor4usv)(const GLushort* v) = _choose_glColor4usv;
static void API_ENTRY _choose_glColorMask(GLboolean red,GLboolean green,GLboolean blue,GLboolean alpha)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColorMask");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColorMask is NULL " << std::endl;
	else
	{
		_ptr_to_glColorMask = (void(API_ENTRY*)(GLboolean,GLboolean,GLboolean,GLboolean))gotPtr;
		return _ptr_to_glColorMask(red,green,blue,alpha);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColorMask)(GLboolean red,GLboolean green,GLboolean blue,GLboolean alpha) = _choose_glColorMask;
static void API_ENTRY _choose_glColorMaski(GLuint index,GLboolean r,GLboolean g,GLboolean b,GLboolean a)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColorMaski");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColorMaski is NULL " << std::endl;
	else
	{
		_ptr_to_glColorMaski = (void(API_ENTRY*)(GLuint,GLboolean,GLboolean,GLboolean,GLboolean))gotPtr;
		return _ptr_to_glColorMaski(index,r,g,b,a);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColorMaski)(GLuint index,GLboolean r,GLboolean g,GLboolean b,GLboolean a) = _choose_glColorMaski;
static void API_ENTRY _choose_glColorMaterial(GLenum face,GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColorMaterial");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColorMaterial is NULL " << std::endl;
	else
	{
		_ptr_to_glColorMaterial = (void(API_ENTRY*)(GLenum,GLenum))gotPtr;
		return _ptr_to_glColorMaterial(face,mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColorMaterial)(GLenum face,GLenum mode) = _choose_glColorMaterial;
static void API_ENTRY _choose_glColorP3ui(GLenum type,GLuint color)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColorP3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColorP3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glColorP3ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glColorP3ui(type,color);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColorP3ui)(GLenum type,GLuint color) = _choose_glColorP3ui;
static void API_ENTRY _choose_glColorP3uiv(GLenum type,const GLuint* color)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColorP3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColorP3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glColorP3uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glColorP3uiv(type,color);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColorP3uiv)(GLenum type,const GLuint* color) = _choose_glColorP3uiv;
static void API_ENTRY _choose_glColorP4ui(GLenum type,GLuint color)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColorP4ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColorP4ui is NULL " << std::endl;
	else
	{
		_ptr_to_glColorP4ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glColorP4ui(type,color);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColorP4ui)(GLenum type,GLuint color) = _choose_glColorP4ui;
static void API_ENTRY _choose_glColorP4uiv(GLenum type,const GLuint* color)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColorP4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColorP4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glColorP4uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glColorP4uiv(type,color);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColorP4uiv)(GLenum type,const GLuint* color) = _choose_glColorP4uiv;
static void API_ENTRY _choose_glColorPointer(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glColorPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glColorPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glColorPointer = (void(API_ENTRY*)(GLint,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glColorPointer(size,type,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glColorPointer)(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer) = _choose_glColorPointer;
static void API_ENTRY _choose_glCompileShader(GLuint shader)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCompileShader");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCompileShader is NULL " << std::endl;
	else
	{
		_ptr_to_glCompileShader = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glCompileShader(shader);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCompileShader)(GLuint shader) = _choose_glCompileShader;
static void API_ENTRY _choose_glCompileShaderARB(GLhandleARB shaderObj)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCompileShaderARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCompileShaderARB is NULL " << std::endl;
	else
	{
		_ptr_to_glCompileShaderARB = (void(API_ENTRY*)(GLhandleARB))gotPtr;
		return _ptr_to_glCompileShaderARB(shaderObj);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCompileShaderARB)(GLhandleARB shaderObj) = _choose_glCompileShaderARB;
static void API_ENTRY _choose_glCompressedTexImage1D(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLint border,GLsizei imageSize,const GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCompressedTexImage1D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCompressedTexImage1D is NULL " << std::endl;
	else
	{
		_ptr_to_glCompressedTexImage1D = (void(API_ENTRY*)(GLenum,GLint,GLenum,GLsizei,GLint,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glCompressedTexImage1D(target,level,internalformat,width,border,imageSize,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCompressedTexImage1D)(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLint border,GLsizei imageSize,const GLvoid* data) = _choose_glCompressedTexImage1D;
static void API_ENTRY _choose_glCompressedTexImage2D(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLint border,GLsizei imageSize,const GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCompressedTexImage2D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCompressedTexImage2D is NULL " << std::endl;
	else
	{
		_ptr_to_glCompressedTexImage2D = (void(API_ENTRY*)(GLenum,GLint,GLenum,GLsizei,GLsizei,GLint,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glCompressedTexImage2D(target,level,internalformat,width,height,border,imageSize,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCompressedTexImage2D)(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLint border,GLsizei imageSize,const GLvoid* data) = _choose_glCompressedTexImage2D;
static void API_ENTRY _choose_glCompressedTexImage3D(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLsizei imageSize,const GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCompressedTexImage3D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCompressedTexImage3D is NULL " << std::endl;
	else
	{
		_ptr_to_glCompressedTexImage3D = (void(API_ENTRY*)(GLenum,GLint,GLenum,GLsizei,GLsizei,GLsizei,GLint,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glCompressedTexImage3D(target,level,internalformat,width,height,depth,border,imageSize,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCompressedTexImage3D)(GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLsizei imageSize,const GLvoid* data) = _choose_glCompressedTexImage3D;
static void API_ENTRY _choose_glCompressedTexSubImage1D(GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLsizei imageSize,const GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCompressedTexSubImage1D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCompressedTexSubImage1D is NULL " << std::endl;
	else
	{
		_ptr_to_glCompressedTexSubImage1D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLsizei,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glCompressedTexSubImage1D(target,level,xoffset,width,format,imageSize,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCompressedTexSubImage1D)(GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLsizei imageSize,const GLvoid* data) = _choose_glCompressedTexSubImage1D;
static void API_ENTRY _choose_glCompressedTexSubImage2D(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLsizei imageSize,const GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCompressedTexSubImage2D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCompressedTexSubImage2D is NULL " << std::endl;
	else
	{
		_ptr_to_glCompressedTexSubImage2D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glCompressedTexSubImage2D(target,level,xoffset,yoffset,width,height,format,imageSize,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCompressedTexSubImage2D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLsizei imageSize,const GLvoid* data) = _choose_glCompressedTexSubImage2D;
static void API_ENTRY _choose_glCompressedTexSubImage3D(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLsizei imageSize,const GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCompressedTexSubImage3D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCompressedTexSubImage3D is NULL " << std::endl;
	else
	{
		_ptr_to_glCompressedTexSubImage3D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint,GLint,GLsizei,GLsizei,GLsizei,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glCompressedTexSubImage3D(target,level,xoffset,yoffset,zoffset,width,height,depth,format,imageSize,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCompressedTexSubImage3D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLsizei imageSize,const GLvoid* data) = _choose_glCompressedTexSubImage3D;
static void API_ENTRY _choose_glCopyBufferSubData(GLenum readTarget,GLenum writeTarget,GLintptr readOffset,GLintptr writeOffset,GLsizeiptr size)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCopyBufferSubData");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCopyBufferSubData is NULL " << std::endl;
	else
	{
		_ptr_to_glCopyBufferSubData = (void(API_ENTRY*)(GLenum,GLenum,GLintptr,GLintptr,GLsizeiptr))gotPtr;
		return _ptr_to_glCopyBufferSubData(readTarget,writeTarget,readOffset,writeOffset,size);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCopyBufferSubData)(GLenum readTarget,GLenum writeTarget,GLintptr readOffset,GLintptr writeOffset,GLsizeiptr size) = _choose_glCopyBufferSubData;
static void API_ENTRY _choose_glCopyPixels(GLint x,GLint y,GLsizei width,GLsizei height,GLenum type)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCopyPixels");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCopyPixels is NULL " << std::endl;
	else
	{
		_ptr_to_glCopyPixels = (void(API_ENTRY*)(GLint,GLint,GLsizei,GLsizei,GLenum))gotPtr;
		return _ptr_to_glCopyPixels(x,y,width,height,type);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCopyPixels)(GLint x,GLint y,GLsizei width,GLsizei height,GLenum type) = _choose_glCopyPixels;
static void API_ENTRY _choose_glCopyTexImage1D(GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLint border)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCopyTexImage1D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCopyTexImage1D is NULL " << std::endl;
	else
	{
		_ptr_to_glCopyTexImage1D = (void(API_ENTRY*)(GLenum,GLint,GLenum,GLint,GLint,GLsizei,GLint))gotPtr;
		return _ptr_to_glCopyTexImage1D(target,level,internalformat,x,y,width,border);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCopyTexImage1D)(GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLint border) = _choose_glCopyTexImage1D;
static void API_ENTRY _choose_glCopyTexImage2D(GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLsizei height,GLint border)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCopyTexImage2D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCopyTexImage2D is NULL " << std::endl;
	else
	{
		_ptr_to_glCopyTexImage2D = (void(API_ENTRY*)(GLenum,GLint,GLenum,GLint,GLint,GLsizei,GLsizei,GLint))gotPtr;
		return _ptr_to_glCopyTexImage2D(target,level,internalformat,x,y,width,height,border);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCopyTexImage2D)(GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLsizei height,GLint border) = _choose_glCopyTexImage2D;
static void API_ENTRY _choose_glCopyTexSubImage1D(GLenum target,GLint level,GLint xoffset,GLint x,GLint y,GLsizei width)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCopyTexSubImage1D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCopyTexSubImage1D is NULL " << std::endl;
	else
	{
		_ptr_to_glCopyTexSubImage1D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint,GLint,GLsizei))gotPtr;
		return _ptr_to_glCopyTexSubImage1D(target,level,xoffset,x,y,width);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCopyTexSubImage1D)(GLenum target,GLint level,GLint xoffset,GLint x,GLint y,GLsizei width) = _choose_glCopyTexSubImage1D;
static void API_ENTRY _choose_glCopyTexSubImage2D(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint x,GLint y,GLsizei width,GLsizei height)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCopyTexSubImage2D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCopyTexSubImage2D is NULL " << std::endl;
	else
	{
		_ptr_to_glCopyTexSubImage2D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint,GLint,GLint,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCopyTexSubImage2D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint x,GLint y,GLsizei width,GLsizei height) = _choose_glCopyTexSubImage2D;
static void API_ENTRY _choose_glCopyTexSubImage3D(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLint x,GLint y,GLsizei width,GLsizei height)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCopyTexSubImage3D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCopyTexSubImage3D is NULL " << std::endl;
	else
	{
		_ptr_to_glCopyTexSubImage3D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint,GLint,GLint,GLint,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glCopyTexSubImage3D(target,level,xoffset,yoffset,zoffset,x,y,width,height);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCopyTexSubImage3D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLint x,GLint y,GLsizei width,GLsizei height) = _choose_glCopyTexSubImage3D;
static GLuint API_ENTRY _choose_glCreateProgram()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCreateProgram");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCreateProgram is NULL " << std::endl;
	else
	{
		_ptr_to_glCreateProgram = (GLuint(API_ENTRY*)())gotPtr;
		return _ptr_to_glCreateProgram();
	}
	typedef GLuint RET_TYPE;
	return RET_TYPE();
}
GLuint (API_ENTRY *_ptr_to_glCreateProgram)() = _choose_glCreateProgram;
static GLhandleARB API_ENTRY _choose_glCreateProgramObjectARB()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCreateProgramObjectARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCreateProgramObjectARB is NULL " << std::endl;
	else
	{
		_ptr_to_glCreateProgramObjectARB = (GLhandleARB(API_ENTRY*)())gotPtr;
		return _ptr_to_glCreateProgramObjectARB();
	}
	typedef GLhandleARB RET_TYPE;
	return RET_TYPE();
}
GLhandleARB (API_ENTRY *_ptr_to_glCreateProgramObjectARB)() = _choose_glCreateProgramObjectARB;
static GLuint API_ENTRY _choose_glCreateShader(GLenum type)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCreateShader");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCreateShader is NULL " << std::endl;
	else
	{
		_ptr_to_glCreateShader = (GLuint(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glCreateShader(type);
	}
	typedef GLuint RET_TYPE;
	return RET_TYPE();
}
GLuint (API_ENTRY *_ptr_to_glCreateShader)(GLenum type) = _choose_glCreateShader;
static GLhandleARB API_ENTRY _choose_glCreateShaderObjectARB(GLenum shaderType)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCreateShaderObjectARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCreateShaderObjectARB is NULL " << std::endl;
	else
	{
		_ptr_to_glCreateShaderObjectARB = (GLhandleARB(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glCreateShaderObjectARB(shaderType);
	}
	typedef GLhandleARB RET_TYPE;
	return RET_TYPE();
}
GLhandleARB (API_ENTRY *_ptr_to_glCreateShaderObjectARB)(GLenum shaderType) = _choose_glCreateShaderObjectARB;
static GLuint API_ENTRY _choose_glCreateShaderProgramv(GLenum type,GLsizei count,const GLchar*const* strings)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCreateShaderProgramv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCreateShaderProgramv is NULL " << std::endl;
	else
	{
		_ptr_to_glCreateShaderProgramv = (GLuint(API_ENTRY*)(GLenum,GLsizei,const GLchar*const*))gotPtr;
		return _ptr_to_glCreateShaderProgramv(type,count,strings);
	}
	typedef GLuint RET_TYPE;
	return RET_TYPE();
}
GLuint (API_ENTRY *_ptr_to_glCreateShaderProgramv)(GLenum type,GLsizei count,const GLchar*const* strings) = _choose_glCreateShaderProgramv;
static void API_ENTRY _choose_glCullFace(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glCullFace");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glCullFace is NULL " << std::endl;
	else
	{
		_ptr_to_glCullFace = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glCullFace(mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glCullFace)(GLenum mode) = _choose_glCullFace;
static void API_ENTRY _choose_glDeleteBuffers(GLsizei n,const GLuint* buffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteBuffers");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteBuffers is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteBuffers = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteBuffers(n,buffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteBuffers)(GLsizei n,const GLuint* buffers) = _choose_glDeleteBuffers;
static void API_ENTRY _choose_glDeleteBuffersARB(GLsizei n,const GLuint* buffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteBuffersARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteBuffersARB is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteBuffersARB = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteBuffersARB(n,buffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteBuffersARB)(GLsizei n,const GLuint* buffers) = _choose_glDeleteBuffersARB;
static void API_ENTRY _choose_glDeleteFramebuffers(GLsizei n,const GLuint* framebuffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteFramebuffers");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteFramebuffers is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteFramebuffers = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteFramebuffers(n,framebuffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteFramebuffers)(GLsizei n,const GLuint* framebuffers) = _choose_glDeleteFramebuffers;
static void API_ENTRY _choose_glDeleteFramebuffersEXT(GLsizei n,const GLuint* framebuffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteFramebuffersEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteFramebuffersEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteFramebuffersEXT = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteFramebuffersEXT(n,framebuffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteFramebuffersEXT)(GLsizei n,const GLuint* framebuffers) = _choose_glDeleteFramebuffersEXT;
static void API_ENTRY _choose_glDeleteLists(GLuint list,GLsizei range)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteLists");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteLists is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteLists = (void(API_ENTRY*)(GLuint,GLsizei))gotPtr;
		return _ptr_to_glDeleteLists(list,range);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteLists)(GLuint list,GLsizei range) = _choose_glDeleteLists;
static void API_ENTRY _choose_glDeleteObjectARB(GLhandleARB obj)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteObjectARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteObjectARB is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteObjectARB = (void(API_ENTRY*)(GLhandleARB))gotPtr;
		return _ptr_to_glDeleteObjectARB(obj);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteObjectARB)(GLhandleARB obj) = _choose_glDeleteObjectARB;
static void API_ENTRY _choose_glDeleteProgram(GLuint program)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteProgram");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteProgram is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteProgram = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glDeleteProgram(program);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteProgram)(GLuint program) = _choose_glDeleteProgram;
static void API_ENTRY _choose_glDeleteProgramPipelines(GLsizei n,const GLuint* pipelines)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteProgramPipelines");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteProgramPipelines is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteProgramPipelines = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteProgramPipelines(n,pipelines);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteProgramPipelines)(GLsizei n,const GLuint* pipelines) = _choose_glDeleteProgramPipelines;
static void API_ENTRY _choose_glDeleteProgramsARB(GLsizei n,const GLuint* programs)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteProgramsARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteProgramsARB is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteProgramsARB = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteProgramsARB(n,programs);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteProgramsARB)(GLsizei n,const GLuint* programs) = _choose_glDeleteProgramsARB;
static void API_ENTRY _choose_glDeleteQueries(GLsizei n,const GLuint* ids)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteQueries");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteQueries is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteQueries = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteQueries(n,ids);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteQueries)(GLsizei n,const GLuint* ids) = _choose_glDeleteQueries;
static void API_ENTRY _choose_glDeleteRenderbuffers(GLsizei n,const GLuint* renderbuffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteRenderbuffers");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteRenderbuffers is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteRenderbuffers = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteRenderbuffers(n,renderbuffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteRenderbuffers)(GLsizei n,const GLuint* renderbuffers) = _choose_glDeleteRenderbuffers;
static void API_ENTRY _choose_glDeleteRenderbuffersEXT(GLsizei n,const GLuint* renderbuffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteRenderbuffersEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteRenderbuffersEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteRenderbuffersEXT = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteRenderbuffersEXT(n,renderbuffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteRenderbuffersEXT)(GLsizei n,const GLuint* renderbuffers) = _choose_glDeleteRenderbuffersEXT;
static void API_ENTRY _choose_glDeleteSamplers(GLsizei count,const GLuint* samplers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteSamplers");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteSamplers is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteSamplers = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteSamplers(count,samplers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteSamplers)(GLsizei count,const GLuint* samplers) = _choose_glDeleteSamplers;
static void API_ENTRY _choose_glDeleteShader(GLuint shader)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteShader");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteShader is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteShader = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glDeleteShader(shader);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteShader)(GLuint shader) = _choose_glDeleteShader;
static void API_ENTRY _choose_glDeleteSync(GLsync sync)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteSync");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteSync is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteSync = (void(API_ENTRY*)(GLsync))gotPtr;
		return _ptr_to_glDeleteSync(sync);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteSync)(GLsync sync) = _choose_glDeleteSync;
static void API_ENTRY _choose_glDeleteTextures(GLsizei n,const GLuint* textures)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteTextures");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteTextures is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteTextures = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteTextures(n,textures);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteTextures)(GLsizei n,const GLuint* textures) = _choose_glDeleteTextures;
static void API_ENTRY _choose_glDeleteTransformFeedbacks(GLsizei n,const GLuint* ids)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteTransformFeedbacks");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteTransformFeedbacks is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteTransformFeedbacks = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteTransformFeedbacks(n,ids);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteTransformFeedbacks)(GLsizei n,const GLuint* ids) = _choose_glDeleteTransformFeedbacks;
static void API_ENTRY _choose_glDeleteVertexArrays(GLsizei n,const GLuint* arrays)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDeleteVertexArrays");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDeleteVertexArrays is NULL " << std::endl;
	else
	{
		_ptr_to_glDeleteVertexArrays = (void(API_ENTRY*)(GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glDeleteVertexArrays(n,arrays);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDeleteVertexArrays)(GLsizei n,const GLuint* arrays) = _choose_glDeleteVertexArrays;
static void API_ENTRY _choose_glDepthFunc(GLenum func)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDepthFunc");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDepthFunc is NULL " << std::endl;
	else
	{
		_ptr_to_glDepthFunc = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glDepthFunc(func);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDepthFunc)(GLenum func) = _choose_glDepthFunc;
static void API_ENTRY _choose_glDepthMask(GLboolean flag)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDepthMask");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDepthMask is NULL " << std::endl;
	else
	{
		_ptr_to_glDepthMask = (void(API_ENTRY*)(GLboolean))gotPtr;
		return _ptr_to_glDepthMask(flag);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDepthMask)(GLboolean flag) = _choose_glDepthMask;
static void API_ENTRY _choose_glDepthRange(GLdouble ren_near,GLdouble ren_far)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDepthRange");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDepthRange is NULL " << std::endl;
	else
	{
		_ptr_to_glDepthRange = (void(API_ENTRY*)(GLdouble,GLdouble))gotPtr;
		return _ptr_to_glDepthRange(ren_near,ren_far);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDepthRange)(GLdouble ren_near,GLdouble ren_far) = _choose_glDepthRange;
static void API_ENTRY _choose_glDepthRangeArrayv(GLuint first,GLsizei count,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDepthRangeArrayv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDepthRangeArrayv is NULL " << std::endl;
	else
	{
		_ptr_to_glDepthRangeArrayv = (void(API_ENTRY*)(GLuint,GLsizei,const GLdouble*))gotPtr;
		return _ptr_to_glDepthRangeArrayv(first,count,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDepthRangeArrayv)(GLuint first,GLsizei count,const GLdouble* v) = _choose_glDepthRangeArrayv;
static void API_ENTRY _choose_glDepthRangeIndexed(GLuint index,GLdouble n,GLdouble f)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDepthRangeIndexed");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDepthRangeIndexed is NULL " << std::endl;
	else
	{
		_ptr_to_glDepthRangeIndexed = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glDepthRangeIndexed(index,n,f);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDepthRangeIndexed)(GLuint index,GLdouble n,GLdouble f) = _choose_glDepthRangeIndexed;
static void API_ENTRY _choose_glDepthRangef(GLfloat n,GLfloat f)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDepthRangef");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDepthRangef is NULL " << std::endl;
	else
	{
		_ptr_to_glDepthRangef = (void(API_ENTRY*)(GLfloat,GLfloat))gotPtr;
		return _ptr_to_glDepthRangef(n,f);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDepthRangef)(GLfloat n,GLfloat f) = _choose_glDepthRangef;
static void API_ENTRY _choose_glDetachObjectARB(GLhandleARB containerObj,GLhandleARB attachedObj)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDetachObjectARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDetachObjectARB is NULL " << std::endl;
	else
	{
		_ptr_to_glDetachObjectARB = (void(API_ENTRY*)(GLhandleARB,GLhandleARB))gotPtr;
		return _ptr_to_glDetachObjectARB(containerObj,attachedObj);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDetachObjectARB)(GLhandleARB containerObj,GLhandleARB attachedObj) = _choose_glDetachObjectARB;
static void API_ENTRY _choose_glDetachShader(GLuint program,GLuint shader)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDetachShader");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDetachShader is NULL " << std::endl;
	else
	{
		_ptr_to_glDetachShader = (void(API_ENTRY*)(GLuint,GLuint))gotPtr;
		return _ptr_to_glDetachShader(program,shader);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDetachShader)(GLuint program,GLuint shader) = _choose_glDetachShader;
static void API_ENTRY _choose_glDisable(GLenum cap)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDisable");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDisable is NULL " << std::endl;
	else
	{
		_ptr_to_glDisable = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glDisable(cap);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDisable)(GLenum cap) = _choose_glDisable;
static void API_ENTRY _choose_glDisableClientState(GLenum ren_array)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDisableClientState");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDisableClientState is NULL " << std::endl;
	else
	{
		_ptr_to_glDisableClientState = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glDisableClientState(ren_array);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDisableClientState)(GLenum ren_array) = _choose_glDisableClientState;
static void API_ENTRY _choose_glDisableVertexAttribArray(GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDisableVertexAttribArray");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDisableVertexAttribArray is NULL " << std::endl;
	else
	{
		_ptr_to_glDisableVertexAttribArray = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glDisableVertexAttribArray(index);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDisableVertexAttribArray)(GLuint index) = _choose_glDisableVertexAttribArray;
static void API_ENTRY _choose_glDisableVertexAttribArrayARB(GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDisableVertexAttribArrayARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDisableVertexAttribArrayARB is NULL " << std::endl;
	else
	{
		_ptr_to_glDisableVertexAttribArrayARB = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glDisableVertexAttribArrayARB(index);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDisableVertexAttribArrayARB)(GLuint index) = _choose_glDisableVertexAttribArrayARB;
static void API_ENTRY _choose_glDisablei(GLenum target,GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDisablei");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDisablei is NULL " << std::endl;
	else
	{
		_ptr_to_glDisablei = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glDisablei(target,index);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDisablei)(GLenum target,GLuint index) = _choose_glDisablei;
static void API_ENTRY _choose_glDrawArrays(GLenum mode,GLint first,GLsizei count)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawArrays");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawArrays is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawArrays = (void(API_ENTRY*)(GLenum,GLint,GLsizei))gotPtr;
		return _ptr_to_glDrawArrays(mode,first,count);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawArrays)(GLenum mode,GLint first,GLsizei count) = _choose_glDrawArrays;
static void API_ENTRY _choose_glDrawArraysIndirect(GLenum mode,const GLvoid* indirect)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawArraysIndirect");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawArraysIndirect is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawArraysIndirect = (void(API_ENTRY*)(GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glDrawArraysIndirect(mode,indirect);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawArraysIndirect)(GLenum mode,const GLvoid* indirect) = _choose_glDrawArraysIndirect;
static void API_ENTRY _choose_glDrawArraysInstanced(GLenum mode,GLint first,GLsizei count,GLsizei instancecount)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawArraysInstanced");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawArraysInstanced is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawArraysInstanced = (void(API_ENTRY*)(GLenum,GLint,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glDrawArraysInstanced(mode,first,count,instancecount);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawArraysInstanced)(GLenum mode,GLint first,GLsizei count,GLsizei instancecount) = _choose_glDrawArraysInstanced;
static void API_ENTRY _choose_glDrawArraysInstancedBaseInstance(GLenum mode,GLint first,GLsizei count,GLsizei instancecount,GLuint baseinstance)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawArraysInstancedBaseInstance");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawArraysInstancedBaseInstance is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawArraysInstancedBaseInstance = (void(API_ENTRY*)(GLenum,GLint,GLsizei,GLsizei,GLuint))gotPtr;
		return _ptr_to_glDrawArraysInstancedBaseInstance(mode,first,count,instancecount,baseinstance);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawArraysInstancedBaseInstance)(GLenum mode,GLint first,GLsizei count,GLsizei instancecount,GLuint baseinstance) = _choose_glDrawArraysInstancedBaseInstance;
static void API_ENTRY _choose_glDrawBuffer(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawBuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawBuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawBuffer = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glDrawBuffer(mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawBuffer)(GLenum mode) = _choose_glDrawBuffer;
static void API_ENTRY _choose_glDrawBuffers(GLsizei n,const GLenum* bufs)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawBuffers");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawBuffers is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawBuffers = (void(API_ENTRY*)(GLsizei,const GLenum*))gotPtr;
		return _ptr_to_glDrawBuffers(n,bufs);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawBuffers)(GLsizei n,const GLenum* bufs) = _choose_glDrawBuffers;
static void API_ENTRY _choose_glDrawElements(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawElements");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawElements is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawElements = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glDrawElements(mode,count,type,indices);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawElements)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices) = _choose_glDrawElements;
static void API_ENTRY _choose_glDrawElementsBaseVertex(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLint basevertex)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawElementsBaseVertex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawElementsBaseVertex is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawElementsBaseVertex = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,const GLvoid*,GLint))gotPtr;
		return _ptr_to_glDrawElementsBaseVertex(mode,count,type,indices,basevertex);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawElementsBaseVertex)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLint basevertex) = _choose_glDrawElementsBaseVertex;
static void API_ENTRY _choose_glDrawElementsIndirect(GLenum mode,GLenum type,const GLvoid* indirect)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawElementsIndirect");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawElementsIndirect is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawElementsIndirect = (void(API_ENTRY*)(GLenum,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glDrawElementsIndirect(mode,type,indirect);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawElementsIndirect)(GLenum mode,GLenum type,const GLvoid* indirect) = _choose_glDrawElementsIndirect;
static void API_ENTRY _choose_glDrawElementsInstanced(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawElementsInstanced");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawElementsInstanced is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawElementsInstanced = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,const GLvoid*,GLsizei))gotPtr;
		return _ptr_to_glDrawElementsInstanced(mode,count,type,indices,instancecount);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawElementsInstanced)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount) = _choose_glDrawElementsInstanced;
static void API_ENTRY _choose_glDrawElementsInstancedBaseInstance(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLuint baseinstance)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawElementsInstancedBaseInstance");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawElementsInstancedBaseInstance is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawElementsInstancedBaseInstance = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,const GLvoid*,GLsizei,GLuint))gotPtr;
		return _ptr_to_glDrawElementsInstancedBaseInstance(mode,count,type,indices,instancecount,baseinstance);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawElementsInstancedBaseInstance)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLuint baseinstance) = _choose_glDrawElementsInstancedBaseInstance;
static void API_ENTRY _choose_glDrawElementsInstancedBaseVertex(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLint basevertex)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawElementsInstancedBaseVertex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawElementsInstancedBaseVertex is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawElementsInstancedBaseVertex = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,const GLvoid*,GLsizei,GLint))gotPtr;
		return _ptr_to_glDrawElementsInstancedBaseVertex(mode,count,type,indices,instancecount,basevertex);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawElementsInstancedBaseVertex)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLint basevertex) = _choose_glDrawElementsInstancedBaseVertex;
static void API_ENTRY _choose_glDrawElementsInstancedBaseVertexBaseInstance(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLint basevertex,GLuint baseinstance)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawElementsInstancedBaseVertexBaseInstance");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawElementsInstancedBaseVertexBaseInstance is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawElementsInstancedBaseVertexBaseInstance = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,const GLvoid*,GLsizei,GLint,GLuint))gotPtr;
		return _ptr_to_glDrawElementsInstancedBaseVertexBaseInstance(mode,count,type,indices,instancecount,basevertex,baseinstance);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawElementsInstancedBaseVertexBaseInstance)(GLenum mode,GLsizei count,GLenum type,const GLvoid* indices,GLsizei instancecount,GLint basevertex,GLuint baseinstance) = _choose_glDrawElementsInstancedBaseVertexBaseInstance;
static void API_ENTRY _choose_glDrawPixels(GLsizei width,GLsizei height,GLenum format,GLenum type,const GLvoid* pixels)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawPixels");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawPixels is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawPixels = (void(API_ENTRY*)(GLsizei,GLsizei,GLenum,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glDrawPixels(width,height,format,type,pixels);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawPixels)(GLsizei width,GLsizei height,GLenum format,GLenum type,const GLvoid* pixels) = _choose_glDrawPixels;
static void API_ENTRY _choose_glDrawRangeElements(GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const GLvoid* indices)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawRangeElements");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawRangeElements is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawRangeElements = (void(API_ENTRY*)(GLenum,GLuint,GLuint,GLsizei,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glDrawRangeElements(mode,start,end,count,type,indices);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawRangeElements)(GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const GLvoid* indices) = _choose_glDrawRangeElements;
static void API_ENTRY _choose_glDrawRangeElementsBaseVertex(GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const GLvoid* indices,GLint basevertex)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawRangeElementsBaseVertex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawRangeElementsBaseVertex is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawRangeElementsBaseVertex = (void(API_ENTRY*)(GLenum,GLuint,GLuint,GLsizei,GLenum,const GLvoid*,GLint))gotPtr;
		return _ptr_to_glDrawRangeElementsBaseVertex(mode,start,end,count,type,indices,basevertex);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawRangeElementsBaseVertex)(GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const GLvoid* indices,GLint basevertex) = _choose_glDrawRangeElementsBaseVertex;
static void API_ENTRY _choose_glDrawTransformFeedback(GLenum mode,GLuint id)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawTransformFeedback");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawTransformFeedback is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawTransformFeedback = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glDrawTransformFeedback(mode,id);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawTransformFeedback)(GLenum mode,GLuint id) = _choose_glDrawTransformFeedback;
static void API_ENTRY _choose_glDrawTransformFeedbackInstanced(GLenum mode,GLuint id,GLsizei instancecount)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawTransformFeedbackInstanced");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawTransformFeedbackInstanced is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawTransformFeedbackInstanced = (void(API_ENTRY*)(GLenum,GLuint,GLsizei))gotPtr;
		return _ptr_to_glDrawTransformFeedbackInstanced(mode,id,instancecount);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawTransformFeedbackInstanced)(GLenum mode,GLuint id,GLsizei instancecount) = _choose_glDrawTransformFeedbackInstanced;
static void API_ENTRY _choose_glDrawTransformFeedbackStream(GLenum mode,GLuint id,GLuint stream)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawTransformFeedbackStream");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawTransformFeedbackStream is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawTransformFeedbackStream = (void(API_ENTRY*)(GLenum,GLuint,GLuint))gotPtr;
		return _ptr_to_glDrawTransformFeedbackStream(mode,id,stream);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawTransformFeedbackStream)(GLenum mode,GLuint id,GLuint stream) = _choose_glDrawTransformFeedbackStream;
static void API_ENTRY _choose_glDrawTransformFeedbackStreamInstanced(GLenum mode,GLuint id,GLuint stream,GLsizei instancecount)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glDrawTransformFeedbackStreamInstanced");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glDrawTransformFeedbackStreamInstanced is NULL " << std::endl;
	else
	{
		_ptr_to_glDrawTransformFeedbackStreamInstanced = (void(API_ENTRY*)(GLenum,GLuint,GLuint,GLsizei))gotPtr;
		return _ptr_to_glDrawTransformFeedbackStreamInstanced(mode,id,stream,instancecount);
	}
	return;
}
void (API_ENTRY *_ptr_to_glDrawTransformFeedbackStreamInstanced)(GLenum mode,GLuint id,GLuint stream,GLsizei instancecount) = _choose_glDrawTransformFeedbackStreamInstanced;
static void API_ENTRY _choose_glEdgeFlag(GLboolean flag)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEdgeFlag");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEdgeFlag is NULL " << std::endl;
	else
	{
		_ptr_to_glEdgeFlag = (void(API_ENTRY*)(GLboolean))gotPtr;
		return _ptr_to_glEdgeFlag(flag);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEdgeFlag)(GLboolean flag) = _choose_glEdgeFlag;
static void API_ENTRY _choose_glEdgeFlagPointer(GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEdgeFlagPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEdgeFlagPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glEdgeFlagPointer = (void(API_ENTRY*)(GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glEdgeFlagPointer(stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEdgeFlagPointer)(GLsizei stride,const GLvoid* pointer) = _choose_glEdgeFlagPointer;
static void API_ENTRY _choose_glEdgeFlagv(const GLboolean* flag)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEdgeFlagv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEdgeFlagv is NULL " << std::endl;
	else
	{
		_ptr_to_glEdgeFlagv = (void(API_ENTRY*)(const GLboolean*))gotPtr;
		return _ptr_to_glEdgeFlagv(flag);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEdgeFlagv)(const GLboolean* flag) = _choose_glEdgeFlagv;
static void API_ENTRY _choose_glEnable(GLenum cap)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEnable");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEnable is NULL " << std::endl;
	else
	{
		_ptr_to_glEnable = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glEnable(cap);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEnable)(GLenum cap) = _choose_glEnable;
static void API_ENTRY _choose_glEnableClientState(GLenum ren_array)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEnableClientState");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEnableClientState is NULL " << std::endl;
	else
	{
		_ptr_to_glEnableClientState = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glEnableClientState(ren_array);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEnableClientState)(GLenum ren_array) = _choose_glEnableClientState;
static void API_ENTRY _choose_glEnableVertexAttribArray(GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEnableVertexAttribArray");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEnableVertexAttribArray is NULL " << std::endl;
	else
	{
		_ptr_to_glEnableVertexAttribArray = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glEnableVertexAttribArray(index);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEnableVertexAttribArray)(GLuint index) = _choose_glEnableVertexAttribArray;
static void API_ENTRY _choose_glEnableVertexAttribArrayARB(GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEnableVertexAttribArrayARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEnableVertexAttribArrayARB is NULL " << std::endl;
	else
	{
		_ptr_to_glEnableVertexAttribArrayARB = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glEnableVertexAttribArrayARB(index);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEnableVertexAttribArrayARB)(GLuint index) = _choose_glEnableVertexAttribArrayARB;
static void API_ENTRY _choose_glEnablei(GLenum target,GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEnablei");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEnablei is NULL " << std::endl;
	else
	{
		_ptr_to_glEnablei = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glEnablei(target,index);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEnablei)(GLenum target,GLuint index) = _choose_glEnablei;
static void API_ENTRY _choose_glEnd()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEnd");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEnd is NULL " << std::endl;
	else
	{
		_ptr_to_glEnd = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glEnd();
	}
	return;
}
void (API_ENTRY *_ptr_to_glEnd)() = _choose_glEnd;
static void API_ENTRY _choose_glEndConditionalRender()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEndConditionalRender");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEndConditionalRender is NULL " << std::endl;
	else
	{
		_ptr_to_glEndConditionalRender = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glEndConditionalRender();
	}
	return;
}
void (API_ENTRY *_ptr_to_glEndConditionalRender)() = _choose_glEndConditionalRender;
static void API_ENTRY _choose_glEndList()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEndList");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEndList is NULL " << std::endl;
	else
	{
		_ptr_to_glEndList = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glEndList();
	}
	return;
}
void (API_ENTRY *_ptr_to_glEndList)() = _choose_glEndList;
static void API_ENTRY _choose_glEndQuery(GLenum target)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEndQuery");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEndQuery is NULL " << std::endl;
	else
	{
		_ptr_to_glEndQuery = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glEndQuery(target);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEndQuery)(GLenum target) = _choose_glEndQuery;
static void API_ENTRY _choose_glEndQueryIndexed(GLenum target,GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEndQueryIndexed");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEndQueryIndexed is NULL " << std::endl;
	else
	{
		_ptr_to_glEndQueryIndexed = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glEndQueryIndexed(target,index);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEndQueryIndexed)(GLenum target,GLuint index) = _choose_glEndQueryIndexed;
static void API_ENTRY _choose_glEndTransformFeedback()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEndTransformFeedback");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEndTransformFeedback is NULL " << std::endl;
	else
	{
		_ptr_to_glEndTransformFeedback = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glEndTransformFeedback();
	}
	return;
}
void (API_ENTRY *_ptr_to_glEndTransformFeedback)() = _choose_glEndTransformFeedback;
static void API_ENTRY _choose_glEvalCoord1d(GLdouble u)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalCoord1d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalCoord1d is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalCoord1d = (void(API_ENTRY*)(GLdouble))gotPtr;
		return _ptr_to_glEvalCoord1d(u);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalCoord1d)(GLdouble u) = _choose_glEvalCoord1d;
static void API_ENTRY _choose_glEvalCoord1dv(const GLdouble* u)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalCoord1dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalCoord1dv is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalCoord1dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glEvalCoord1dv(u);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalCoord1dv)(const GLdouble* u) = _choose_glEvalCoord1dv;
static void API_ENTRY _choose_glEvalCoord1f(GLfloat u)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalCoord1f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalCoord1f is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalCoord1f = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glEvalCoord1f(u);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalCoord1f)(GLfloat u) = _choose_glEvalCoord1f;
static void API_ENTRY _choose_glEvalCoord1fv(const GLfloat* u)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalCoord1fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalCoord1fv is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalCoord1fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glEvalCoord1fv(u);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalCoord1fv)(const GLfloat* u) = _choose_glEvalCoord1fv;
static void API_ENTRY _choose_glEvalCoord2d(GLdouble u,GLdouble v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalCoord2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalCoord2d is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalCoord2d = (void(API_ENTRY*)(GLdouble,GLdouble))gotPtr;
		return _ptr_to_glEvalCoord2d(u,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalCoord2d)(GLdouble u,GLdouble v) = _choose_glEvalCoord2d;
static void API_ENTRY _choose_glEvalCoord2dv(const GLdouble* u)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalCoord2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalCoord2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalCoord2dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glEvalCoord2dv(u);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalCoord2dv)(const GLdouble* u) = _choose_glEvalCoord2dv;
static void API_ENTRY _choose_glEvalCoord2f(GLfloat u,GLfloat v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalCoord2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalCoord2f is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalCoord2f = (void(API_ENTRY*)(GLfloat,GLfloat))gotPtr;
		return _ptr_to_glEvalCoord2f(u,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalCoord2f)(GLfloat u,GLfloat v) = _choose_glEvalCoord2f;
static void API_ENTRY _choose_glEvalCoord2fv(const GLfloat* u)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalCoord2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalCoord2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalCoord2fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glEvalCoord2fv(u);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalCoord2fv)(const GLfloat* u) = _choose_glEvalCoord2fv;
static void API_ENTRY _choose_glEvalMesh1(GLenum mode,GLint i1,GLint i2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalMesh1");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalMesh1 is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalMesh1 = (void(API_ENTRY*)(GLenum,GLint,GLint))gotPtr;
		return _ptr_to_glEvalMesh1(mode,i1,i2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalMesh1)(GLenum mode,GLint i1,GLint i2) = _choose_glEvalMesh1;
static void API_ENTRY _choose_glEvalMesh2(GLenum mode,GLint i1,GLint i2,GLint j1,GLint j2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalMesh2");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalMesh2 is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalMesh2 = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glEvalMesh2(mode,i1,i2,j1,j2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalMesh2)(GLenum mode,GLint i1,GLint i2,GLint j1,GLint j2) = _choose_glEvalMesh2;
static void API_ENTRY _choose_glEvalPoint1(GLint i)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalPoint1");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalPoint1 is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalPoint1 = (void(API_ENTRY*)(GLint))gotPtr;
		return _ptr_to_glEvalPoint1(i);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalPoint1)(GLint i) = _choose_glEvalPoint1;
static void API_ENTRY _choose_glEvalPoint2(GLint i,GLint j)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glEvalPoint2");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glEvalPoint2 is NULL " << std::endl;
	else
	{
		_ptr_to_glEvalPoint2 = (void(API_ENTRY*)(GLint,GLint))gotPtr;
		return _ptr_to_glEvalPoint2(i,j);
	}
	return;
}
void (API_ENTRY *_ptr_to_glEvalPoint2)(GLint i,GLint j) = _choose_glEvalPoint2;
static void API_ENTRY _choose_glFeedbackBuffer(GLsizei size,GLenum type,GLfloat* buffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFeedbackBuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFeedbackBuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glFeedbackBuffer = (void(API_ENTRY*)(GLsizei,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glFeedbackBuffer(size,type,buffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFeedbackBuffer)(GLsizei size,GLenum type,GLfloat* buffer) = _choose_glFeedbackBuffer;
static GLsync API_ENTRY _choose_glFenceSync(GLenum condition,GLbitfield flags)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFenceSync");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFenceSync is NULL " << std::endl;
	else
	{
		_ptr_to_glFenceSync = (GLsync(API_ENTRY*)(GLenum,GLbitfield))gotPtr;
		return _ptr_to_glFenceSync(condition,flags);
	}
	typedef GLsync RET_TYPE;
	return RET_TYPE();
}
GLsync (API_ENTRY *_ptr_to_glFenceSync)(GLenum condition,GLbitfield flags) = _choose_glFenceSync;
static void API_ENTRY _choose_glFinish()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFinish");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFinish is NULL " << std::endl;
	else
	{
		_ptr_to_glFinish = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glFinish();
	}
	return;
}
void (API_ENTRY *_ptr_to_glFinish)() = _choose_glFinish;
static void API_ENTRY _choose_glFlush()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFlush");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFlush is NULL " << std::endl;
	else
	{
		_ptr_to_glFlush = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glFlush();
	}
	return;
}
void (API_ENTRY *_ptr_to_glFlush)() = _choose_glFlush;
static void API_ENTRY _choose_glFlushMappedBufferRange(GLenum target,GLintptr offset,GLsizeiptr length)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFlushMappedBufferRange");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFlushMappedBufferRange is NULL " << std::endl;
	else
	{
		_ptr_to_glFlushMappedBufferRange = (void(API_ENTRY*)(GLenum,GLintptr,GLsizeiptr))gotPtr;
		return _ptr_to_glFlushMappedBufferRange(target,offset,length);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFlushMappedBufferRange)(GLenum target,GLintptr offset,GLsizeiptr length) = _choose_glFlushMappedBufferRange;
static void API_ENTRY _choose_glFogCoordPointer(GLenum type,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFogCoordPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFogCoordPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glFogCoordPointer = (void(API_ENTRY*)(GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glFogCoordPointer(type,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFogCoordPointer)(GLenum type,GLsizei stride,const GLvoid* pointer) = _choose_glFogCoordPointer;
static void API_ENTRY _choose_glFogCoordd(GLdouble coord)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFogCoordd");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFogCoordd is NULL " << std::endl;
	else
	{
		_ptr_to_glFogCoordd = (void(API_ENTRY*)(GLdouble))gotPtr;
		return _ptr_to_glFogCoordd(coord);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFogCoordd)(GLdouble coord) = _choose_glFogCoordd;
static void API_ENTRY _choose_glFogCoorddv(const GLdouble* coord)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFogCoorddv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFogCoorddv is NULL " << std::endl;
	else
	{
		_ptr_to_glFogCoorddv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glFogCoorddv(coord);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFogCoorddv)(const GLdouble* coord) = _choose_glFogCoorddv;
static void API_ENTRY _choose_glFogCoordf(GLfloat coord)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFogCoordf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFogCoordf is NULL " << std::endl;
	else
	{
		_ptr_to_glFogCoordf = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glFogCoordf(coord);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFogCoordf)(GLfloat coord) = _choose_glFogCoordf;
static void API_ENTRY _choose_glFogCoordfv(const GLfloat* coord)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFogCoordfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFogCoordfv is NULL " << std::endl;
	else
	{
		_ptr_to_glFogCoordfv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glFogCoordfv(coord);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFogCoordfv)(const GLfloat* coord) = _choose_glFogCoordfv;
static void API_ENTRY _choose_glFogf(GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFogf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFogf is NULL " << std::endl;
	else
	{
		_ptr_to_glFogf = (void(API_ENTRY*)(GLenum,GLfloat))gotPtr;
		return _ptr_to_glFogf(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFogf)(GLenum pname,GLfloat param) = _choose_glFogf;
static void API_ENTRY _choose_glFogfv(GLenum pname,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFogfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFogfv is NULL " << std::endl;
	else
	{
		_ptr_to_glFogfv = (void(API_ENTRY*)(GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glFogfv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFogfv)(GLenum pname,const GLfloat* params) = _choose_glFogfv;
static void API_ENTRY _choose_glFogi(GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFogi");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFogi is NULL " << std::endl;
	else
	{
		_ptr_to_glFogi = (void(API_ENTRY*)(GLenum,GLint))gotPtr;
		return _ptr_to_glFogi(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFogi)(GLenum pname,GLint param) = _choose_glFogi;
static void API_ENTRY _choose_glFogiv(GLenum pname,const GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFogiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFogiv is NULL " << std::endl;
	else
	{
		_ptr_to_glFogiv = (void(API_ENTRY*)(GLenum,const GLint*))gotPtr;
		return _ptr_to_glFogiv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFogiv)(GLenum pname,const GLint* params) = _choose_glFogiv;
static void API_ENTRY _choose_glFramebufferRenderbuffer(GLenum target,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferRenderbuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferRenderbuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferRenderbuffer = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLuint))gotPtr;
		return _ptr_to_glFramebufferRenderbuffer(target,attachment,renderbuffertarget,renderbuffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferRenderbuffer)(GLenum target,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer) = _choose_glFramebufferRenderbuffer;
static void API_ENTRY _choose_glFramebufferRenderbufferEXT(GLenum target,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferRenderbufferEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferRenderbufferEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferRenderbufferEXT = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLuint))gotPtr;
		return _ptr_to_glFramebufferRenderbufferEXT(target,attachment,renderbuffertarget,renderbuffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferRenderbufferEXT)(GLenum target,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer) = _choose_glFramebufferRenderbufferEXT;
static void API_ENTRY _choose_glFramebufferTexture(GLenum target,GLenum attachment,GLuint texture,GLint level)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferTexture");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferTexture is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferTexture = (void(API_ENTRY*)(GLenum,GLenum,GLuint,GLint))gotPtr;
		return _ptr_to_glFramebufferTexture(target,attachment,texture,level);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferTexture)(GLenum target,GLenum attachment,GLuint texture,GLint level) = _choose_glFramebufferTexture;
static void API_ENTRY _choose_glFramebufferTexture1D(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferTexture1D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferTexture1D is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferTexture1D = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLuint,GLint))gotPtr;
		return _ptr_to_glFramebufferTexture1D(target,attachment,textarget,texture,level);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferTexture1D)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level) = _choose_glFramebufferTexture1D;
static void API_ENTRY _choose_glFramebufferTexture1DEXT(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferTexture1DEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferTexture1DEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferTexture1DEXT = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLuint,GLint))gotPtr;
		return _ptr_to_glFramebufferTexture1DEXT(target,attachment,textarget,texture,level);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferTexture1DEXT)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level) = _choose_glFramebufferTexture1DEXT;
static void API_ENTRY _choose_glFramebufferTexture2D(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferTexture2D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferTexture2D is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferTexture2D = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLuint,GLint))gotPtr;
		return _ptr_to_glFramebufferTexture2D(target,attachment,textarget,texture,level);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferTexture2D)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level) = _choose_glFramebufferTexture2D;
static void API_ENTRY _choose_glFramebufferTexture2DEXT(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferTexture2DEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferTexture2DEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferTexture2DEXT = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLuint,GLint))gotPtr;
		return _ptr_to_glFramebufferTexture2DEXT(target,attachment,textarget,texture,level);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferTexture2DEXT)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level) = _choose_glFramebufferTexture2DEXT;
static void API_ENTRY _choose_glFramebufferTexture3D(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferTexture3D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferTexture3D is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferTexture3D = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLuint,GLint,GLint))gotPtr;
		return _ptr_to_glFramebufferTexture3D(target,attachment,textarget,texture,level,zoffset);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferTexture3D)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset) = _choose_glFramebufferTexture3D;
static void API_ENTRY _choose_glFramebufferTexture3DEXT(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferTexture3DEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferTexture3DEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferTexture3DEXT = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLuint,GLint,GLint))gotPtr;
		return _ptr_to_glFramebufferTexture3DEXT(target,attachment,textarget,texture,level,zoffset);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferTexture3DEXT)(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset) = _choose_glFramebufferTexture3DEXT;
static void API_ENTRY _choose_glFramebufferTextureLayer(GLenum target,GLenum attachment,GLuint texture,GLint level,GLint layer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFramebufferTextureLayer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFramebufferTextureLayer is NULL " << std::endl;
	else
	{
		_ptr_to_glFramebufferTextureLayer = (void(API_ENTRY*)(GLenum,GLenum,GLuint,GLint,GLint))gotPtr;
		return _ptr_to_glFramebufferTextureLayer(target,attachment,texture,level,layer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFramebufferTextureLayer)(GLenum target,GLenum attachment,GLuint texture,GLint level,GLint layer) = _choose_glFramebufferTextureLayer;
static void API_ENTRY _choose_glFrontFace(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFrontFace");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFrontFace is NULL " << std::endl;
	else
	{
		_ptr_to_glFrontFace = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glFrontFace(mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFrontFace)(GLenum mode) = _choose_glFrontFace;
static void API_ENTRY _choose_glFrustum(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glFrustum");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glFrustum is NULL " << std::endl;
	else
	{
		_ptr_to_glFrustum = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glFrustum(left,right,bottom,top,zNear,zFar);
	}
	return;
}
void (API_ENTRY *_ptr_to_glFrustum)(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar) = _choose_glFrustum;
static void API_ENTRY _choose_glGenBuffers(GLsizei n,GLuint* buffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenBuffers");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenBuffers is NULL " << std::endl;
	else
	{
		_ptr_to_glGenBuffers = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenBuffers(n,buffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenBuffers)(GLsizei n,GLuint* buffers) = _choose_glGenBuffers;
static void API_ENTRY _choose_glGenBuffersARB(GLsizei n,GLuint* buffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenBuffersARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenBuffersARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGenBuffersARB = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenBuffersARB(n,buffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenBuffersARB)(GLsizei n,GLuint* buffers) = _choose_glGenBuffersARB;
static void API_ENTRY _choose_glGenFramebuffers(GLsizei n,GLuint* framebuffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenFramebuffers");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenFramebuffers is NULL " << std::endl;
	else
	{
		_ptr_to_glGenFramebuffers = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenFramebuffers(n,framebuffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenFramebuffers)(GLsizei n,GLuint* framebuffers) = _choose_glGenFramebuffers;
static void API_ENTRY _choose_glGenFramebuffersEXT(GLsizei n,GLuint* framebuffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenFramebuffersEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenFramebuffersEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glGenFramebuffersEXT = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenFramebuffersEXT(n,framebuffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenFramebuffersEXT)(GLsizei n,GLuint* framebuffers) = _choose_glGenFramebuffersEXT;
static GLuint API_ENTRY _choose_glGenLists(GLsizei range)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenLists");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenLists is NULL " << std::endl;
	else
	{
		_ptr_to_glGenLists = (GLuint(API_ENTRY*)(GLsizei))gotPtr;
		return _ptr_to_glGenLists(range);
	}
	typedef GLuint RET_TYPE;
	return RET_TYPE();
}
GLuint (API_ENTRY *_ptr_to_glGenLists)(GLsizei range) = _choose_glGenLists;
static void API_ENTRY _choose_glGenProgramPipelines(GLsizei n,GLuint* pipelines)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenProgramPipelines");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenProgramPipelines is NULL " << std::endl;
	else
	{
		_ptr_to_glGenProgramPipelines = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenProgramPipelines(n,pipelines);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenProgramPipelines)(GLsizei n,GLuint* pipelines) = _choose_glGenProgramPipelines;
static void API_ENTRY _choose_glGenProgramsARB(GLsizei n,GLuint* programs)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenProgramsARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenProgramsARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGenProgramsARB = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenProgramsARB(n,programs);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenProgramsARB)(GLsizei n,GLuint* programs) = _choose_glGenProgramsARB;
static void API_ENTRY _choose_glGenQueries(GLsizei n,GLuint* ids)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenQueries");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenQueries is NULL " << std::endl;
	else
	{
		_ptr_to_glGenQueries = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenQueries(n,ids);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenQueries)(GLsizei n,GLuint* ids) = _choose_glGenQueries;
static void API_ENTRY _choose_glGenRenderbuffers(GLsizei n,GLuint* renderbuffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenRenderbuffers");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenRenderbuffers is NULL " << std::endl;
	else
	{
		_ptr_to_glGenRenderbuffers = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenRenderbuffers(n,renderbuffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenRenderbuffers)(GLsizei n,GLuint* renderbuffers) = _choose_glGenRenderbuffers;
static void API_ENTRY _choose_glGenRenderbuffersEXT(GLsizei n,GLuint* renderbuffers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenRenderbuffersEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenRenderbuffersEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glGenRenderbuffersEXT = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenRenderbuffersEXT(n,renderbuffers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenRenderbuffersEXT)(GLsizei n,GLuint* renderbuffers) = _choose_glGenRenderbuffersEXT;
static void API_ENTRY _choose_glGenSamplers(GLsizei count,GLuint* samplers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenSamplers");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenSamplers is NULL " << std::endl;
	else
	{
		_ptr_to_glGenSamplers = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenSamplers(count,samplers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenSamplers)(GLsizei count,GLuint* samplers) = _choose_glGenSamplers;
static void API_ENTRY _choose_glGenTextures(GLsizei n,GLuint* textures)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenTextures");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenTextures is NULL " << std::endl;
	else
	{
		_ptr_to_glGenTextures = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenTextures(n,textures);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenTextures)(GLsizei n,GLuint* textures) = _choose_glGenTextures;
static void API_ENTRY _choose_glGenTransformFeedbacks(GLsizei n,GLuint* ids)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenTransformFeedbacks");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenTransformFeedbacks is NULL " << std::endl;
	else
	{
		_ptr_to_glGenTransformFeedbacks = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenTransformFeedbacks(n,ids);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenTransformFeedbacks)(GLsizei n,GLuint* ids) = _choose_glGenTransformFeedbacks;
static void API_ENTRY _choose_glGenVertexArrays(GLsizei n,GLuint* arrays)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenVertexArrays");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenVertexArrays is NULL " << std::endl;
	else
	{
		_ptr_to_glGenVertexArrays = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glGenVertexArrays(n,arrays);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenVertexArrays)(GLsizei n,GLuint* arrays) = _choose_glGenVertexArrays;
static void API_ENTRY _choose_glGenerateMipmap(GLenum target)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenerateMipmap");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenerateMipmap is NULL " << std::endl;
	else
	{
		_ptr_to_glGenerateMipmap = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glGenerateMipmap(target);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenerateMipmap)(GLenum target) = _choose_glGenerateMipmap;
static void API_ENTRY _choose_glGenerateMipmapEXT(GLenum target)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGenerateMipmapEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGenerateMipmapEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glGenerateMipmapEXT = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glGenerateMipmapEXT(target);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGenerateMipmapEXT)(GLenum target) = _choose_glGenerateMipmapEXT;
static void API_ENTRY _choose_glGetActiveAtomicCounterBufferiv(GLuint program,GLuint bufferIndex,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveAtomicCounterBufferiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveAtomicCounterBufferiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveAtomicCounterBufferiv = (void(API_ENTRY*)(GLuint,GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetActiveAtomicCounterBufferiv(program,bufferIndex,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveAtomicCounterBufferiv)(GLuint program,GLuint bufferIndex,GLenum pname,GLint* params) = _choose_glGetActiveAtomicCounterBufferiv;
static void API_ENTRY _choose_glGetActiveAttrib(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLint* size,GLenum* type,GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveAttrib");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveAttrib is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveAttrib = (void(API_ENTRY*)(GLuint,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,GLchar*))gotPtr;
		return _ptr_to_glGetActiveAttrib(program,index,bufSize,length,size,type,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveAttrib)(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLint* size,GLenum* type,GLchar* name) = _choose_glGetActiveAttrib;
static void API_ENTRY _choose_glGetActiveAttribARB(GLhandleARB programObj,GLuint index,GLsizei maxLength,GLsizei* length,GLint* size,GLenum* type,GLcharARB* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveAttribARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveAttribARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveAttribARB = (void(API_ENTRY*)(GLhandleARB,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,GLcharARB*))gotPtr;
		return _ptr_to_glGetActiveAttribARB(programObj,index,maxLength,length,size,type,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveAttribARB)(GLhandleARB programObj,GLuint index,GLsizei maxLength,GLsizei* length,GLint* size,GLenum* type,GLcharARB* name) = _choose_glGetActiveAttribARB;
static void API_ENTRY _choose_glGetActiveSubroutineName(GLuint program,GLenum shadertype,GLuint index,GLsizei bufsize,GLsizei* length,GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveSubroutineName");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveSubroutineName is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveSubroutineName = (void(API_ENTRY*)(GLuint,GLenum,GLuint,GLsizei,GLsizei*,GLchar*))gotPtr;
		return _ptr_to_glGetActiveSubroutineName(program,shadertype,index,bufsize,length,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveSubroutineName)(GLuint program,GLenum shadertype,GLuint index,GLsizei bufsize,GLsizei* length,GLchar* name) = _choose_glGetActiveSubroutineName;
static void API_ENTRY _choose_glGetActiveSubroutineUniformName(GLuint program,GLenum shadertype,GLuint index,GLsizei bufsize,GLsizei* length,GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveSubroutineUniformName");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveSubroutineUniformName is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveSubroutineUniformName = (void(API_ENTRY*)(GLuint,GLenum,GLuint,GLsizei,GLsizei*,GLchar*))gotPtr;
		return _ptr_to_glGetActiveSubroutineUniformName(program,shadertype,index,bufsize,length,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveSubroutineUniformName)(GLuint program,GLenum shadertype,GLuint index,GLsizei bufsize,GLsizei* length,GLchar* name) = _choose_glGetActiveSubroutineUniformName;
static void API_ENTRY _choose_glGetActiveSubroutineUniformiv(GLuint program,GLenum shadertype,GLuint index,GLenum pname,GLint* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveSubroutineUniformiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveSubroutineUniformiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveSubroutineUniformiv = (void(API_ENTRY*)(GLuint,GLenum,GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetActiveSubroutineUniformiv(program,shadertype,index,pname,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveSubroutineUniformiv)(GLuint program,GLenum shadertype,GLuint index,GLenum pname,GLint* values) = _choose_glGetActiveSubroutineUniformiv;
static void API_ENTRY _choose_glGetActiveUniform(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLint* size,GLenum* type,GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveUniform");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveUniform is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveUniform = (void(API_ENTRY*)(GLuint,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,GLchar*))gotPtr;
		return _ptr_to_glGetActiveUniform(program,index,bufSize,length,size,type,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveUniform)(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLint* size,GLenum* type,GLchar* name) = _choose_glGetActiveUniform;
static void API_ENTRY _choose_glGetActiveUniformARB(GLhandleARB programObj,GLuint index,GLsizei maxLength,GLsizei* length,GLint* size,GLenum* type,GLcharARB* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveUniformARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveUniformARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveUniformARB = (void(API_ENTRY*)(GLhandleARB,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,GLcharARB*))gotPtr;
		return _ptr_to_glGetActiveUniformARB(programObj,index,maxLength,length,size,type,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveUniformARB)(GLhandleARB programObj,GLuint index,GLsizei maxLength,GLsizei* length,GLint* size,GLenum* type,GLcharARB* name) = _choose_glGetActiveUniformARB;
static void API_ENTRY _choose_glGetActiveUniformBlockName(GLuint program,GLuint uniformBlockIndex,GLsizei bufSize,GLsizei* length,GLchar* uniformBlockName)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveUniformBlockName");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveUniformBlockName is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveUniformBlockName = (void(API_ENTRY*)(GLuint,GLuint,GLsizei,GLsizei*,GLchar*))gotPtr;
		return _ptr_to_glGetActiveUniformBlockName(program,uniformBlockIndex,bufSize,length,uniformBlockName);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveUniformBlockName)(GLuint program,GLuint uniformBlockIndex,GLsizei bufSize,GLsizei* length,GLchar* uniformBlockName) = _choose_glGetActiveUniformBlockName;
static void API_ENTRY _choose_glGetActiveUniformBlockiv(GLuint program,GLuint uniformBlockIndex,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveUniformBlockiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveUniformBlockiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveUniformBlockiv = (void(API_ENTRY*)(GLuint,GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetActiveUniformBlockiv(program,uniformBlockIndex,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveUniformBlockiv)(GLuint program,GLuint uniformBlockIndex,GLenum pname,GLint* params) = _choose_glGetActiveUniformBlockiv;
static void API_ENTRY _choose_glGetActiveUniformName(GLuint program,GLuint uniformIndex,GLsizei bufSize,GLsizei* length,GLchar* uniformName)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveUniformName");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveUniformName is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveUniformName = (void(API_ENTRY*)(GLuint,GLuint,GLsizei,GLsizei*,GLchar*))gotPtr;
		return _ptr_to_glGetActiveUniformName(program,uniformIndex,bufSize,length,uniformName);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveUniformName)(GLuint program,GLuint uniformIndex,GLsizei bufSize,GLsizei* length,GLchar* uniformName) = _choose_glGetActiveUniformName;
static void API_ENTRY _choose_glGetActiveUniformsiv(GLuint program,GLsizei uniformCount,const GLuint* uniformIndices,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetActiveUniformsiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetActiveUniformsiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetActiveUniformsiv = (void(API_ENTRY*)(GLuint,GLsizei,const GLuint*,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetActiveUniformsiv(program,uniformCount,uniformIndices,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetActiveUniformsiv)(GLuint program,GLsizei uniformCount,const GLuint* uniformIndices,GLenum pname,GLint* params) = _choose_glGetActiveUniformsiv;
static void API_ENTRY _choose_glGetAttachedObjectsARB(GLhandleARB containerObj,GLsizei maxCount,GLsizei* count,GLhandleARB* obj)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetAttachedObjectsARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetAttachedObjectsARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetAttachedObjectsARB = (void(API_ENTRY*)(GLhandleARB,GLsizei,GLsizei*,GLhandleARB*))gotPtr;
		return _ptr_to_glGetAttachedObjectsARB(containerObj,maxCount,count,obj);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetAttachedObjectsARB)(GLhandleARB containerObj,GLsizei maxCount,GLsizei* count,GLhandleARB* obj) = _choose_glGetAttachedObjectsARB;
static void API_ENTRY _choose_glGetAttachedShaders(GLuint program,GLsizei maxCount,GLsizei* count,GLuint* shaders)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetAttachedShaders");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetAttachedShaders is NULL " << std::endl;
	else
	{
		_ptr_to_glGetAttachedShaders = (void(API_ENTRY*)(GLuint,GLsizei,GLsizei*,GLuint*))gotPtr;
		return _ptr_to_glGetAttachedShaders(program,maxCount,count,shaders);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetAttachedShaders)(GLuint program,GLsizei maxCount,GLsizei* count,GLuint* shaders) = _choose_glGetAttachedShaders;
static GLint API_ENTRY _choose_glGetAttribLocation(GLuint program,const GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetAttribLocation");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetAttribLocation is NULL " << std::endl;
	else
	{
		_ptr_to_glGetAttribLocation = (GLint(API_ENTRY*)(GLuint,const GLchar*))gotPtr;
		return _ptr_to_glGetAttribLocation(program,name);
	}
	typedef GLint RET_TYPE;
	return RET_TYPE();
}
GLint (API_ENTRY *_ptr_to_glGetAttribLocation)(GLuint program,const GLchar* name) = _choose_glGetAttribLocation;
static GLint API_ENTRY _choose_glGetAttribLocationARB(GLhandleARB programObj,const GLcharARB* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetAttribLocationARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetAttribLocationARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetAttribLocationARB = (GLint(API_ENTRY*)(GLhandleARB,const GLcharARB*))gotPtr;
		return _ptr_to_glGetAttribLocationARB(programObj,name);
	}
	typedef GLint RET_TYPE;
	return RET_TYPE();
}
GLint (API_ENTRY *_ptr_to_glGetAttribLocationARB)(GLhandleARB programObj,const GLcharARB* name) = _choose_glGetAttribLocationARB;
static void API_ENTRY _choose_glGetBooleani_v(GLenum target,GLuint index,GLboolean* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetBooleani_v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetBooleani_v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetBooleani_v = (void(API_ENTRY*)(GLenum,GLuint,GLboolean*))gotPtr;
		return _ptr_to_glGetBooleani_v(target,index,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetBooleani_v)(GLenum target,GLuint index,GLboolean* data) = _choose_glGetBooleani_v;
static void API_ENTRY _choose_glGetBooleanv(GLenum pname,GLboolean* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetBooleanv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetBooleanv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetBooleanv = (void(API_ENTRY*)(GLenum,GLboolean*))gotPtr;
		return _ptr_to_glGetBooleanv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetBooleanv)(GLenum pname,GLboolean* params) = _choose_glGetBooleanv;
static void API_ENTRY _choose_glGetBufferParameteri64v(GLenum target,GLenum pname,GLint64* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetBufferParameteri64v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetBufferParameteri64v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetBufferParameteri64v = (void(API_ENTRY*)(GLenum,GLenum,GLint64*))gotPtr;
		return _ptr_to_glGetBufferParameteri64v(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetBufferParameteri64v)(GLenum target,GLenum pname,GLint64* params) = _choose_glGetBufferParameteri64v;
static void API_ENTRY _choose_glGetBufferParameteriv(GLenum target,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetBufferParameteriv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetBufferParameteriv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetBufferParameteriv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetBufferParameteriv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetBufferParameteriv)(GLenum target,GLenum pname,GLint* params) = _choose_glGetBufferParameteriv;
static void API_ENTRY _choose_glGetBufferParameterivARB(GLenum target,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetBufferParameterivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetBufferParameterivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetBufferParameterivARB = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetBufferParameterivARB(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetBufferParameterivARB)(GLenum target,GLenum pname,GLint* params) = _choose_glGetBufferParameterivARB;
static void API_ENTRY _choose_glGetBufferPointerv(GLenum target,GLenum pname,GLvoid** params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetBufferPointerv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetBufferPointerv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetBufferPointerv = (void(API_ENTRY*)(GLenum,GLenum,GLvoid**))gotPtr;
		return _ptr_to_glGetBufferPointerv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetBufferPointerv)(GLenum target,GLenum pname,GLvoid** params) = _choose_glGetBufferPointerv;
static void API_ENTRY _choose_glGetBufferPointervARB(GLenum target,GLenum pname,GLvoid** params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetBufferPointervARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetBufferPointervARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetBufferPointervARB = (void(API_ENTRY*)(GLenum,GLenum,GLvoid**))gotPtr;
		return _ptr_to_glGetBufferPointervARB(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetBufferPointervARB)(GLenum target,GLenum pname,GLvoid** params) = _choose_glGetBufferPointervARB;
static void API_ENTRY _choose_glGetBufferSubData(GLenum target,GLintptr offset,GLsizeiptr size,GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetBufferSubData");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetBufferSubData is NULL " << std::endl;
	else
	{
		_ptr_to_glGetBufferSubData = (void(API_ENTRY*)(GLenum,GLintptr,GLsizeiptr,GLvoid*))gotPtr;
		return _ptr_to_glGetBufferSubData(target,offset,size,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetBufferSubData)(GLenum target,GLintptr offset,GLsizeiptr size,GLvoid* data) = _choose_glGetBufferSubData;
static void API_ENTRY _choose_glGetBufferSubDataARB(GLenum target,GLintptrARB offset,GLsizeiptrARB size,GLvoid* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetBufferSubDataARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetBufferSubDataARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetBufferSubDataARB = (void(API_ENTRY*)(GLenum,GLintptrARB,GLsizeiptrARB,GLvoid*))gotPtr;
		return _ptr_to_glGetBufferSubDataARB(target,offset,size,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetBufferSubDataARB)(GLenum target,GLintptrARB offset,GLsizeiptrARB size,GLvoid* data) = _choose_glGetBufferSubDataARB;
static void API_ENTRY _choose_glGetClipPlane(GLenum plane,GLdouble* equation)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetClipPlane");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetClipPlane is NULL " << std::endl;
	else
	{
		_ptr_to_glGetClipPlane = (void(API_ENTRY*)(GLenum,GLdouble*))gotPtr;
		return _ptr_to_glGetClipPlane(plane,equation);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetClipPlane)(GLenum plane,GLdouble* equation) = _choose_glGetClipPlane;
static void API_ENTRY _choose_glGetCompressedTexImage(GLenum target,GLint level,GLvoid* img)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetCompressedTexImage");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetCompressedTexImage is NULL " << std::endl;
	else
	{
		_ptr_to_glGetCompressedTexImage = (void(API_ENTRY*)(GLenum,GLint,GLvoid*))gotPtr;
		return _ptr_to_glGetCompressedTexImage(target,level,img);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetCompressedTexImage)(GLenum target,GLint level,GLvoid* img) = _choose_glGetCompressedTexImage;
static void API_ENTRY _choose_glGetDoublei_v(GLenum target,GLuint index,GLdouble* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetDoublei_v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetDoublei_v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetDoublei_v = (void(API_ENTRY*)(GLenum,GLuint,GLdouble*))gotPtr;
		return _ptr_to_glGetDoublei_v(target,index,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetDoublei_v)(GLenum target,GLuint index,GLdouble* data) = _choose_glGetDoublei_v;
static void API_ENTRY _choose_glGetDoublev(GLenum pname,GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetDoublev");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetDoublev is NULL " << std::endl;
	else
	{
		_ptr_to_glGetDoublev = (void(API_ENTRY*)(GLenum,GLdouble*))gotPtr;
		return _ptr_to_glGetDoublev(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetDoublev)(GLenum pname,GLdouble* params) = _choose_glGetDoublev;
static GLenum API_ENTRY _choose_glGetError()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetError");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetError is NULL " << std::endl;
	else
	{
		_ptr_to_glGetError = (GLenum(API_ENTRY*)())gotPtr;
		return _ptr_to_glGetError();
	}
	typedef GLenum RET_TYPE;
	return RET_TYPE();
}
GLenum (API_ENTRY *_ptr_to_glGetError)() = _choose_glGetError;
static void API_ENTRY _choose_glGetFloati_v(GLenum target,GLuint index,GLfloat* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetFloati_v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetFloati_v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetFloati_v = (void(API_ENTRY*)(GLenum,GLuint,GLfloat*))gotPtr;
		return _ptr_to_glGetFloati_v(target,index,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetFloati_v)(GLenum target,GLuint index,GLfloat* data) = _choose_glGetFloati_v;
static void API_ENTRY _choose_glGetFloatv(GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetFloatv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetFloatv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetFloatv = (void(API_ENTRY*)(GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetFloatv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetFloatv)(GLenum pname,GLfloat* params) = _choose_glGetFloatv;
static GLint API_ENTRY _choose_glGetFragDataIndex(GLuint program,const GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetFragDataIndex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetFragDataIndex is NULL " << std::endl;
	else
	{
		_ptr_to_glGetFragDataIndex = (GLint(API_ENTRY*)(GLuint,const GLchar*))gotPtr;
		return _ptr_to_glGetFragDataIndex(program,name);
	}
	typedef GLint RET_TYPE;
	return RET_TYPE();
}
GLint (API_ENTRY *_ptr_to_glGetFragDataIndex)(GLuint program,const GLchar* name) = _choose_glGetFragDataIndex;
static GLint API_ENTRY _choose_glGetFragDataLocation(GLuint program,const GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetFragDataLocation");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetFragDataLocation is NULL " << std::endl;
	else
	{
		_ptr_to_glGetFragDataLocation = (GLint(API_ENTRY*)(GLuint,const GLchar*))gotPtr;
		return _ptr_to_glGetFragDataLocation(program,name);
	}
	typedef GLint RET_TYPE;
	return RET_TYPE();
}
GLint (API_ENTRY *_ptr_to_glGetFragDataLocation)(GLuint program,const GLchar* name) = _choose_glGetFragDataLocation;
static void API_ENTRY _choose_glGetFramebufferAttachmentParameteriv(GLenum target,GLenum attachment,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetFramebufferAttachmentParameteriv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetFramebufferAttachmentParameteriv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetFramebufferAttachmentParameteriv = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetFramebufferAttachmentParameteriv(target,attachment,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetFramebufferAttachmentParameteriv)(GLenum target,GLenum attachment,GLenum pname,GLint* params) = _choose_glGetFramebufferAttachmentParameteriv;
static void API_ENTRY _choose_glGetFramebufferAttachmentParameterivEXT(GLenum target,GLenum attachment,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetFramebufferAttachmentParameterivEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetFramebufferAttachmentParameterivEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glGetFramebufferAttachmentParameterivEXT = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetFramebufferAttachmentParameterivEXT(target,attachment,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetFramebufferAttachmentParameterivEXT)(GLenum target,GLenum attachment,GLenum pname,GLint* params) = _choose_glGetFramebufferAttachmentParameterivEXT;
static GLhandleARB API_ENTRY _choose_glGetHandleARB(GLenum pname)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetHandleARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetHandleARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetHandleARB = (GLhandleARB(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glGetHandleARB(pname);
	}
	typedef GLhandleARB RET_TYPE;
	return RET_TYPE();
}
GLhandleARB (API_ENTRY *_ptr_to_glGetHandleARB)(GLenum pname) = _choose_glGetHandleARB;
static void API_ENTRY _choose_glGetInfoLogARB(GLhandleARB obj,GLsizei maxLength,GLsizei* length,GLcharARB* infoLog)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetInfoLogARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetInfoLogARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetInfoLogARB = (void(API_ENTRY*)(GLhandleARB,GLsizei,GLsizei*,GLcharARB*))gotPtr;
		return _ptr_to_glGetInfoLogARB(obj,maxLength,length,infoLog);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetInfoLogARB)(GLhandleARB obj,GLsizei maxLength,GLsizei* length,GLcharARB* infoLog) = _choose_glGetInfoLogARB;
static void API_ENTRY _choose_glGetInteger64i_v(GLenum target,GLuint index,GLint64* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetInteger64i_v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetInteger64i_v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetInteger64i_v = (void(API_ENTRY*)(GLenum,GLuint,GLint64*))gotPtr;
		return _ptr_to_glGetInteger64i_v(target,index,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetInteger64i_v)(GLenum target,GLuint index,GLint64* data) = _choose_glGetInteger64i_v;
static void API_ENTRY _choose_glGetInteger64v(GLenum pname,GLint64* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetInteger64v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetInteger64v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetInteger64v = (void(API_ENTRY*)(GLenum,GLint64*))gotPtr;
		return _ptr_to_glGetInteger64v(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetInteger64v)(GLenum pname,GLint64* params) = _choose_glGetInteger64v;
static void API_ENTRY _choose_glGetIntegeri_v(GLenum target,GLuint index,GLint* data)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetIntegeri_v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetIntegeri_v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetIntegeri_v = (void(API_ENTRY*)(GLenum,GLuint,GLint*))gotPtr;
		return _ptr_to_glGetIntegeri_v(target,index,data);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetIntegeri_v)(GLenum target,GLuint index,GLint* data) = _choose_glGetIntegeri_v;
static void API_ENTRY _choose_glGetIntegerv(GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetIntegerv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetIntegerv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetIntegerv = (void(API_ENTRY*)(GLenum,GLint*))gotPtr;
		return _ptr_to_glGetIntegerv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetIntegerv)(GLenum pname,GLint* params) = _choose_glGetIntegerv;
static void API_ENTRY _choose_glGetInternalformati64v(GLenum target,GLenum internalformat,GLenum pname,GLsizei bufSize,GLint64* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetInternalformati64v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetInternalformati64v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetInternalformati64v = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLsizei,GLint64*))gotPtr;
		return _ptr_to_glGetInternalformati64v(target,internalformat,pname,bufSize,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetInternalformati64v)(GLenum target,GLenum internalformat,GLenum pname,GLsizei bufSize,GLint64* params) = _choose_glGetInternalformati64v;
static void API_ENTRY _choose_glGetLightfv(GLenum light,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetLightfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetLightfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetLightfv = (void(API_ENTRY*)(GLenum,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetLightfv(light,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetLightfv)(GLenum light,GLenum pname,GLfloat* params) = _choose_glGetLightfv;
static void API_ENTRY _choose_glGetLightiv(GLenum light,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetLightiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetLightiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetLightiv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetLightiv(light,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetLightiv)(GLenum light,GLenum pname,GLint* params) = _choose_glGetLightiv;
static void API_ENTRY _choose_glGetMapdv(GLenum target,GLenum query,GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetMapdv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetMapdv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetMapdv = (void(API_ENTRY*)(GLenum,GLenum,GLdouble*))gotPtr;
		return _ptr_to_glGetMapdv(target,query,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetMapdv)(GLenum target,GLenum query,GLdouble* v) = _choose_glGetMapdv;
static void API_ENTRY _choose_glGetMapfv(GLenum target,GLenum query,GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetMapfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetMapfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetMapfv = (void(API_ENTRY*)(GLenum,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetMapfv(target,query,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetMapfv)(GLenum target,GLenum query,GLfloat* v) = _choose_glGetMapfv;
static void API_ENTRY _choose_glGetMapiv(GLenum target,GLenum query,GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetMapiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetMapiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetMapiv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetMapiv(target,query,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetMapiv)(GLenum target,GLenum query,GLint* v) = _choose_glGetMapiv;
static void API_ENTRY _choose_glGetMaterialfv(GLenum face,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetMaterialfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetMaterialfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetMaterialfv = (void(API_ENTRY*)(GLenum,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetMaterialfv(face,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetMaterialfv)(GLenum face,GLenum pname,GLfloat* params) = _choose_glGetMaterialfv;
static void API_ENTRY _choose_glGetMaterialiv(GLenum face,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetMaterialiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetMaterialiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetMaterialiv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetMaterialiv(face,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetMaterialiv)(GLenum face,GLenum pname,GLint* params) = _choose_glGetMaterialiv;
static void API_ENTRY _choose_glGetMultisamplefv(GLenum pname,GLuint index,GLfloat* val)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetMultisamplefv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetMultisamplefv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetMultisamplefv = (void(API_ENTRY*)(GLenum,GLuint,GLfloat*))gotPtr;
		return _ptr_to_glGetMultisamplefv(pname,index,val);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetMultisamplefv)(GLenum pname,GLuint index,GLfloat* val) = _choose_glGetMultisamplefv;
static void API_ENTRY _choose_glGetObjectParameterfvARB(GLhandleARB obj,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetObjectParameterfvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetObjectParameterfvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetObjectParameterfvARB = (void(API_ENTRY*)(GLhandleARB,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetObjectParameterfvARB(obj,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetObjectParameterfvARB)(GLhandleARB obj,GLenum pname,GLfloat* params) = _choose_glGetObjectParameterfvARB;
static void API_ENTRY _choose_glGetObjectParameterivARB(GLhandleARB obj,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetObjectParameterivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetObjectParameterivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetObjectParameterivARB = (void(API_ENTRY*)(GLhandleARB,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetObjectParameterivARB(obj,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetObjectParameterivARB)(GLhandleARB obj,GLenum pname,GLint* params) = _choose_glGetObjectParameterivARB;
static void API_ENTRY _choose_glGetPixelMapfv(GLenum map,GLfloat* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetPixelMapfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetPixelMapfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetPixelMapfv = (void(API_ENTRY*)(GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetPixelMapfv(map,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetPixelMapfv)(GLenum map,GLfloat* values) = _choose_glGetPixelMapfv;
static void API_ENTRY _choose_glGetPixelMapuiv(GLenum map,GLuint* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetPixelMapuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetPixelMapuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetPixelMapuiv = (void(API_ENTRY*)(GLenum,GLuint*))gotPtr;
		return _ptr_to_glGetPixelMapuiv(map,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetPixelMapuiv)(GLenum map,GLuint* values) = _choose_glGetPixelMapuiv;
static void API_ENTRY _choose_glGetPixelMapusv(GLenum map,GLushort* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetPixelMapusv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetPixelMapusv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetPixelMapusv = (void(API_ENTRY*)(GLenum,GLushort*))gotPtr;
		return _ptr_to_glGetPixelMapusv(map,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetPixelMapusv)(GLenum map,GLushort* values) = _choose_glGetPixelMapusv;
static void API_ENTRY _choose_glGetPointerv(GLenum pname,GLvoid** params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetPointerv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetPointerv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetPointerv = (void(API_ENTRY*)(GLenum,GLvoid**))gotPtr;
		return _ptr_to_glGetPointerv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetPointerv)(GLenum pname,GLvoid** params) = _choose_glGetPointerv;
static void API_ENTRY _choose_glGetPolygonStipple(GLubyte* mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetPolygonStipple");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetPolygonStipple is NULL " << std::endl;
	else
	{
		_ptr_to_glGetPolygonStipple = (void(API_ENTRY*)(GLubyte*))gotPtr;
		return _ptr_to_glGetPolygonStipple(mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetPolygonStipple)(GLubyte* mask) = _choose_glGetPolygonStipple;
static void API_ENTRY _choose_glGetProgramBinary(GLuint program,GLsizei bufSize,GLsizei* length,GLenum* binaryFormat,GLvoid* binary)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramBinary");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramBinary is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramBinary = (void(API_ENTRY*)(GLuint,GLsizei,GLsizei*,GLenum*,GLvoid*))gotPtr;
		return _ptr_to_glGetProgramBinary(program,bufSize,length,binaryFormat,binary);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramBinary)(GLuint program,GLsizei bufSize,GLsizei* length,GLenum* binaryFormat,GLvoid* binary) = _choose_glGetProgramBinary;
static void API_ENTRY _choose_glGetProgramEnvParameterdvARB(GLenum target,GLuint index,GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramEnvParameterdvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramEnvParameterdvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramEnvParameterdvARB = (void(API_ENTRY*)(GLenum,GLuint,GLdouble*))gotPtr;
		return _ptr_to_glGetProgramEnvParameterdvARB(target,index,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramEnvParameterdvARB)(GLenum target,GLuint index,GLdouble* params) = _choose_glGetProgramEnvParameterdvARB;
static void API_ENTRY _choose_glGetProgramEnvParameterfvARB(GLenum target,GLuint index,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramEnvParameterfvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramEnvParameterfvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramEnvParameterfvARB = (void(API_ENTRY*)(GLenum,GLuint,GLfloat*))gotPtr;
		return _ptr_to_glGetProgramEnvParameterfvARB(target,index,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramEnvParameterfvARB)(GLenum target,GLuint index,GLfloat* params) = _choose_glGetProgramEnvParameterfvARB;
static void API_ENTRY _choose_glGetProgramInfoLog(GLuint program,GLsizei bufSize,GLsizei* length,GLchar* infoLog)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramInfoLog");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramInfoLog is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramInfoLog = (void(API_ENTRY*)(GLuint,GLsizei,GLsizei*,GLchar*))gotPtr;
		return _ptr_to_glGetProgramInfoLog(program,bufSize,length,infoLog);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramInfoLog)(GLuint program,GLsizei bufSize,GLsizei* length,GLchar* infoLog) = _choose_glGetProgramInfoLog;
static void API_ENTRY _choose_glGetProgramLocalParameterdvARB(GLenum target,GLuint index,GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramLocalParameterdvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramLocalParameterdvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramLocalParameterdvARB = (void(API_ENTRY*)(GLenum,GLuint,GLdouble*))gotPtr;
		return _ptr_to_glGetProgramLocalParameterdvARB(target,index,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramLocalParameterdvARB)(GLenum target,GLuint index,GLdouble* params) = _choose_glGetProgramLocalParameterdvARB;
static void API_ENTRY _choose_glGetProgramLocalParameterfvARB(GLenum target,GLuint index,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramLocalParameterfvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramLocalParameterfvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramLocalParameterfvARB = (void(API_ENTRY*)(GLenum,GLuint,GLfloat*))gotPtr;
		return _ptr_to_glGetProgramLocalParameterfvARB(target,index,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramLocalParameterfvARB)(GLenum target,GLuint index,GLfloat* params) = _choose_glGetProgramLocalParameterfvARB;
static void API_ENTRY _choose_glGetProgramPipelineInfoLog(GLuint pipeline,GLsizei bufSize,GLsizei* length,GLchar* infoLog)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramPipelineInfoLog");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramPipelineInfoLog is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramPipelineInfoLog = (void(API_ENTRY*)(GLuint,GLsizei,GLsizei*,GLchar*))gotPtr;
		return _ptr_to_glGetProgramPipelineInfoLog(pipeline,bufSize,length,infoLog);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramPipelineInfoLog)(GLuint pipeline,GLsizei bufSize,GLsizei* length,GLchar* infoLog) = _choose_glGetProgramPipelineInfoLog;
static void API_ENTRY _choose_glGetProgramPipelineiv(GLuint pipeline,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramPipelineiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramPipelineiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramPipelineiv = (void(API_ENTRY*)(GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetProgramPipelineiv(pipeline,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramPipelineiv)(GLuint pipeline,GLenum pname,GLint* params) = _choose_glGetProgramPipelineiv;
static void API_ENTRY _choose_glGetProgramStageiv(GLuint program,GLenum shadertype,GLenum pname,GLint* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramStageiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramStageiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramStageiv = (void(API_ENTRY*)(GLuint,GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetProgramStageiv(program,shadertype,pname,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramStageiv)(GLuint program,GLenum shadertype,GLenum pname,GLint* values) = _choose_glGetProgramStageiv;
static void API_ENTRY _choose_glGetProgramStringARB(GLenum target,GLenum pname,GLvoid* string)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramStringARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramStringARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramStringARB = (void(API_ENTRY*)(GLenum,GLenum,GLvoid*))gotPtr;
		return _ptr_to_glGetProgramStringARB(target,pname,string);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramStringARB)(GLenum target,GLenum pname,GLvoid* string) = _choose_glGetProgramStringARB;
static void API_ENTRY _choose_glGetProgramiv(GLuint program,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramiv = (void(API_ENTRY*)(GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetProgramiv(program,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramiv)(GLuint program,GLenum pname,GLint* params) = _choose_glGetProgramiv;
static void API_ENTRY _choose_glGetProgramivARB(GLenum target,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetProgramivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetProgramivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetProgramivARB = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetProgramivARB(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetProgramivARB)(GLenum target,GLenum pname,GLint* params) = _choose_glGetProgramivARB;
static void API_ENTRY _choose_glGetQueryIndexediv(GLenum target,GLuint index,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetQueryIndexediv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetQueryIndexediv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetQueryIndexediv = (void(API_ENTRY*)(GLenum,GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetQueryIndexediv(target,index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetQueryIndexediv)(GLenum target,GLuint index,GLenum pname,GLint* params) = _choose_glGetQueryIndexediv;
static void API_ENTRY _choose_glGetQueryObjecti64v(GLuint id,GLenum pname,GLint64* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetQueryObjecti64v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetQueryObjecti64v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetQueryObjecti64v = (void(API_ENTRY*)(GLuint,GLenum,GLint64*))gotPtr;
		return _ptr_to_glGetQueryObjecti64v(id,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetQueryObjecti64v)(GLuint id,GLenum pname,GLint64* params) = _choose_glGetQueryObjecti64v;
static void API_ENTRY _choose_glGetQueryObjectiv(GLuint id,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetQueryObjectiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetQueryObjectiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetQueryObjectiv = (void(API_ENTRY*)(GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetQueryObjectiv(id,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetQueryObjectiv)(GLuint id,GLenum pname,GLint* params) = _choose_glGetQueryObjectiv;
static void API_ENTRY _choose_glGetQueryObjectui64v(GLuint id,GLenum pname,GLuint64* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetQueryObjectui64v");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetQueryObjectui64v is NULL " << std::endl;
	else
	{
		_ptr_to_glGetQueryObjectui64v = (void(API_ENTRY*)(GLuint,GLenum,GLuint64*))gotPtr;
		return _ptr_to_glGetQueryObjectui64v(id,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetQueryObjectui64v)(GLuint id,GLenum pname,GLuint64* params) = _choose_glGetQueryObjectui64v;
static void API_ENTRY _choose_glGetQueryObjectuiv(GLuint id,GLenum pname,GLuint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetQueryObjectuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetQueryObjectuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetQueryObjectuiv = (void(API_ENTRY*)(GLuint,GLenum,GLuint*))gotPtr;
		return _ptr_to_glGetQueryObjectuiv(id,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetQueryObjectuiv)(GLuint id,GLenum pname,GLuint* params) = _choose_glGetQueryObjectuiv;
static void API_ENTRY _choose_glGetQueryiv(GLenum target,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetQueryiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetQueryiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetQueryiv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetQueryiv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetQueryiv)(GLenum target,GLenum pname,GLint* params) = _choose_glGetQueryiv;
static void API_ENTRY _choose_glGetRenderbufferParameteriv(GLenum target,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetRenderbufferParameteriv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetRenderbufferParameteriv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetRenderbufferParameteriv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetRenderbufferParameteriv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetRenderbufferParameteriv)(GLenum target,GLenum pname,GLint* params) = _choose_glGetRenderbufferParameteriv;
static void API_ENTRY _choose_glGetRenderbufferParameterivEXT(GLenum target,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetRenderbufferParameterivEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetRenderbufferParameterivEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glGetRenderbufferParameterivEXT = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetRenderbufferParameterivEXT(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetRenderbufferParameterivEXT)(GLenum target,GLenum pname,GLint* params) = _choose_glGetRenderbufferParameterivEXT;
static void API_ENTRY _choose_glGetSamplerParameterIiv(GLuint sampler,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetSamplerParameterIiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetSamplerParameterIiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetSamplerParameterIiv = (void(API_ENTRY*)(GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetSamplerParameterIiv(sampler,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetSamplerParameterIiv)(GLuint sampler,GLenum pname,GLint* params) = _choose_glGetSamplerParameterIiv;
static void API_ENTRY _choose_glGetSamplerParameterIuiv(GLuint sampler,GLenum pname,GLuint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetSamplerParameterIuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetSamplerParameterIuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetSamplerParameterIuiv = (void(API_ENTRY*)(GLuint,GLenum,GLuint*))gotPtr;
		return _ptr_to_glGetSamplerParameterIuiv(sampler,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetSamplerParameterIuiv)(GLuint sampler,GLenum pname,GLuint* params) = _choose_glGetSamplerParameterIuiv;
static void API_ENTRY _choose_glGetSamplerParameterfv(GLuint sampler,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetSamplerParameterfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetSamplerParameterfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetSamplerParameterfv = (void(API_ENTRY*)(GLuint,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetSamplerParameterfv(sampler,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetSamplerParameterfv)(GLuint sampler,GLenum pname,GLfloat* params) = _choose_glGetSamplerParameterfv;
static void API_ENTRY _choose_glGetSamplerParameteriv(GLuint sampler,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetSamplerParameteriv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetSamplerParameteriv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetSamplerParameteriv = (void(API_ENTRY*)(GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetSamplerParameteriv(sampler,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetSamplerParameteriv)(GLuint sampler,GLenum pname,GLint* params) = _choose_glGetSamplerParameteriv;
static void API_ENTRY _choose_glGetShaderInfoLog(GLuint shader,GLsizei bufSize,GLsizei* length,GLchar* infoLog)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetShaderInfoLog");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetShaderInfoLog is NULL " << std::endl;
	else
	{
		_ptr_to_glGetShaderInfoLog = (void(API_ENTRY*)(GLuint,GLsizei,GLsizei*,GLchar*))gotPtr;
		return _ptr_to_glGetShaderInfoLog(shader,bufSize,length,infoLog);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetShaderInfoLog)(GLuint shader,GLsizei bufSize,GLsizei* length,GLchar* infoLog) = _choose_glGetShaderInfoLog;
static void API_ENTRY _choose_glGetShaderPrecisionFormat(GLenum shadertype,GLenum precisiontype,GLint* range,GLint* precision)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetShaderPrecisionFormat");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetShaderPrecisionFormat is NULL " << std::endl;
	else
	{
		_ptr_to_glGetShaderPrecisionFormat = (void(API_ENTRY*)(GLenum,GLenum,GLint*,GLint*))gotPtr;
		return _ptr_to_glGetShaderPrecisionFormat(shadertype,precisiontype,range,precision);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetShaderPrecisionFormat)(GLenum shadertype,GLenum precisiontype,GLint* range,GLint* precision) = _choose_glGetShaderPrecisionFormat;
static void API_ENTRY _choose_glGetShaderSource(GLuint shader,GLsizei bufSize,GLsizei* length,GLchar* source)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetShaderSource");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetShaderSource is NULL " << std::endl;
	else
	{
		_ptr_to_glGetShaderSource = (void(API_ENTRY*)(GLuint,GLsizei,GLsizei*,GLchar*))gotPtr;
		return _ptr_to_glGetShaderSource(shader,bufSize,length,source);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetShaderSource)(GLuint shader,GLsizei bufSize,GLsizei* length,GLchar* source) = _choose_glGetShaderSource;
static void API_ENTRY _choose_glGetShaderSourceARB(GLhandleARB obj,GLsizei maxLength,GLsizei* length,GLcharARB* source)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetShaderSourceARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetShaderSourceARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetShaderSourceARB = (void(API_ENTRY*)(GLhandleARB,GLsizei,GLsizei*,GLcharARB*))gotPtr;
		return _ptr_to_glGetShaderSourceARB(obj,maxLength,length,source);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetShaderSourceARB)(GLhandleARB obj,GLsizei maxLength,GLsizei* length,GLcharARB* source) = _choose_glGetShaderSourceARB;
static void API_ENTRY _choose_glGetShaderiv(GLuint shader,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetShaderiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetShaderiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetShaderiv = (void(API_ENTRY*)(GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetShaderiv(shader,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetShaderiv)(GLuint shader,GLenum pname,GLint* params) = _choose_glGetShaderiv;
static const GLubyte* API_ENTRY _choose_glGetString(GLenum name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetString");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetString is NULL " << std::endl;
	else
	{
		_ptr_to_glGetString = (const GLubyte*(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glGetString(name);
	}
	typedef const GLubyte* RET_TYPE;
	return RET_TYPE();
}
const GLubyte* (API_ENTRY *_ptr_to_glGetString)(GLenum name) = _choose_glGetString;
static const GLubyte* API_ENTRY _choose_glGetStringi(GLenum name,GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetStringi");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetStringi is NULL " << std::endl;
	else
	{
		_ptr_to_glGetStringi = (const GLubyte*(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glGetStringi(name,index);
	}
	typedef const GLubyte* RET_TYPE;
	return RET_TYPE();
}
const GLubyte* (API_ENTRY *_ptr_to_glGetStringi)(GLenum name,GLuint index) = _choose_glGetStringi;
static GLuint API_ENTRY _choose_glGetSubroutineIndex(GLuint program,GLenum shadertype,const GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetSubroutineIndex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetSubroutineIndex is NULL " << std::endl;
	else
	{
		_ptr_to_glGetSubroutineIndex = (GLuint(API_ENTRY*)(GLuint,GLenum,const GLchar*))gotPtr;
		return _ptr_to_glGetSubroutineIndex(program,shadertype,name);
	}
	typedef GLuint RET_TYPE;
	return RET_TYPE();
}
GLuint (API_ENTRY *_ptr_to_glGetSubroutineIndex)(GLuint program,GLenum shadertype,const GLchar* name) = _choose_glGetSubroutineIndex;
static GLint API_ENTRY _choose_glGetSubroutineUniformLocation(GLuint program,GLenum shadertype,const GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetSubroutineUniformLocation");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetSubroutineUniformLocation is NULL " << std::endl;
	else
	{
		_ptr_to_glGetSubroutineUniformLocation = (GLint(API_ENTRY*)(GLuint,GLenum,const GLchar*))gotPtr;
		return _ptr_to_glGetSubroutineUniformLocation(program,shadertype,name);
	}
	typedef GLint RET_TYPE;
	return RET_TYPE();
}
GLint (API_ENTRY *_ptr_to_glGetSubroutineUniformLocation)(GLuint program,GLenum shadertype,const GLchar* name) = _choose_glGetSubroutineUniformLocation;
static void API_ENTRY _choose_glGetSynciv(GLsync sync,GLenum pname,GLsizei bufSize,GLsizei* length,GLint* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetSynciv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetSynciv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetSynciv = (void(API_ENTRY*)(GLsync,GLenum,GLsizei,GLsizei*,GLint*))gotPtr;
		return _ptr_to_glGetSynciv(sync,pname,bufSize,length,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetSynciv)(GLsync sync,GLenum pname,GLsizei bufSize,GLsizei* length,GLint* values) = _choose_glGetSynciv;
static void API_ENTRY _choose_glGetTexEnvfv(GLenum target,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexEnvfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexEnvfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexEnvfv = (void(API_ENTRY*)(GLenum,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetTexEnvfv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexEnvfv)(GLenum target,GLenum pname,GLfloat* params) = _choose_glGetTexEnvfv;
static void API_ENTRY _choose_glGetTexEnviv(GLenum target,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexEnviv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexEnviv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexEnviv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetTexEnviv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexEnviv)(GLenum target,GLenum pname,GLint* params) = _choose_glGetTexEnviv;
static void API_ENTRY _choose_glGetTexGendv(GLenum coord,GLenum pname,GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexGendv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexGendv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexGendv = (void(API_ENTRY*)(GLenum,GLenum,GLdouble*))gotPtr;
		return _ptr_to_glGetTexGendv(coord,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexGendv)(GLenum coord,GLenum pname,GLdouble* params) = _choose_glGetTexGendv;
static void API_ENTRY _choose_glGetTexGenfv(GLenum coord,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexGenfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexGenfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexGenfv = (void(API_ENTRY*)(GLenum,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetTexGenfv(coord,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexGenfv)(GLenum coord,GLenum pname,GLfloat* params) = _choose_glGetTexGenfv;
static void API_ENTRY _choose_glGetTexGeniv(GLenum coord,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexGeniv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexGeniv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexGeniv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetTexGeniv(coord,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexGeniv)(GLenum coord,GLenum pname,GLint* params) = _choose_glGetTexGeniv;
static void API_ENTRY _choose_glGetTexImage(GLenum target,GLint level,GLenum format,GLenum type,GLvoid* pixels)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexImage");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexImage is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexImage = (void(API_ENTRY*)(GLenum,GLint,GLenum,GLenum,GLvoid*))gotPtr;
		return _ptr_to_glGetTexImage(target,level,format,type,pixels);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexImage)(GLenum target,GLint level,GLenum format,GLenum type,GLvoid* pixels) = _choose_glGetTexImage;
static void API_ENTRY _choose_glGetTexLevelParameterfv(GLenum target,GLint level,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexLevelParameterfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexLevelParameterfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexLevelParameterfv = (void(API_ENTRY*)(GLenum,GLint,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetTexLevelParameterfv(target,level,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexLevelParameterfv)(GLenum target,GLint level,GLenum pname,GLfloat* params) = _choose_glGetTexLevelParameterfv;
static void API_ENTRY _choose_glGetTexLevelParameteriv(GLenum target,GLint level,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexLevelParameteriv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexLevelParameteriv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexLevelParameteriv = (void(API_ENTRY*)(GLenum,GLint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetTexLevelParameteriv(target,level,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexLevelParameteriv)(GLenum target,GLint level,GLenum pname,GLint* params) = _choose_glGetTexLevelParameteriv;
static void API_ENTRY _choose_glGetTexParameterIiv(GLenum target,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexParameterIiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexParameterIiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexParameterIiv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetTexParameterIiv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexParameterIiv)(GLenum target,GLenum pname,GLint* params) = _choose_glGetTexParameterIiv;
static void API_ENTRY _choose_glGetTexParameterIuiv(GLenum target,GLenum pname,GLuint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexParameterIuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexParameterIuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexParameterIuiv = (void(API_ENTRY*)(GLenum,GLenum,GLuint*))gotPtr;
		return _ptr_to_glGetTexParameterIuiv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexParameterIuiv)(GLenum target,GLenum pname,GLuint* params) = _choose_glGetTexParameterIuiv;
static void API_ENTRY _choose_glGetTexParameterfv(GLenum target,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexParameterfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexParameterfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexParameterfv = (void(API_ENTRY*)(GLenum,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetTexParameterfv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexParameterfv)(GLenum target,GLenum pname,GLfloat* params) = _choose_glGetTexParameterfv;
static void API_ENTRY _choose_glGetTexParameteriv(GLenum target,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTexParameteriv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTexParameteriv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTexParameteriv = (void(API_ENTRY*)(GLenum,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetTexParameteriv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTexParameteriv)(GLenum target,GLenum pname,GLint* params) = _choose_glGetTexParameteriv;
static void API_ENTRY _choose_glGetTransformFeedbackVarying(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLsizei* size,GLenum* type,GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetTransformFeedbackVarying");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetTransformFeedbackVarying is NULL " << std::endl;
	else
	{
		_ptr_to_glGetTransformFeedbackVarying = (void(API_ENTRY*)(GLuint,GLuint,GLsizei,GLsizei*,GLsizei*,GLenum*,GLchar*))gotPtr;
		return _ptr_to_glGetTransformFeedbackVarying(program,index,bufSize,length,size,type,name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetTransformFeedbackVarying)(GLuint program,GLuint index,GLsizei bufSize,GLsizei* length,GLsizei* size,GLenum* type,GLchar* name) = _choose_glGetTransformFeedbackVarying;
static GLuint API_ENTRY _choose_glGetUniformBlockIndex(GLuint program,const GLchar* uniformBlockName)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformBlockIndex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformBlockIndex is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformBlockIndex = (GLuint(API_ENTRY*)(GLuint,const GLchar*))gotPtr;
		return _ptr_to_glGetUniformBlockIndex(program,uniformBlockName);
	}
	typedef GLuint RET_TYPE;
	return RET_TYPE();
}
GLuint (API_ENTRY *_ptr_to_glGetUniformBlockIndex)(GLuint program,const GLchar* uniformBlockName) = _choose_glGetUniformBlockIndex;
static void API_ENTRY _choose_glGetUniformIndices(GLuint program,GLsizei uniformCount,const GLchar*const* uniformNames,GLuint* uniformIndices)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformIndices");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformIndices is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformIndices = (void(API_ENTRY*)(GLuint,GLsizei,const GLchar*const*,GLuint*))gotPtr;
		return _ptr_to_glGetUniformIndices(program,uniformCount,uniformNames,uniformIndices);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetUniformIndices)(GLuint program,GLsizei uniformCount,const GLchar*const* uniformNames,GLuint* uniformIndices) = _choose_glGetUniformIndices;
static GLint API_ENTRY _choose_glGetUniformLocation(GLuint program,const GLchar* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformLocation");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformLocation is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformLocation = (GLint(API_ENTRY*)(GLuint,const GLchar*))gotPtr;
		return _ptr_to_glGetUniformLocation(program,name);
	}
	typedef GLint RET_TYPE;
	return RET_TYPE();
}
GLint (API_ENTRY *_ptr_to_glGetUniformLocation)(GLuint program,const GLchar* name) = _choose_glGetUniformLocation;
static GLint API_ENTRY _choose_glGetUniformLocationARB(GLhandleARB programObj,const GLcharARB* name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformLocationARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformLocationARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformLocationARB = (GLint(API_ENTRY*)(GLhandleARB,const GLcharARB*))gotPtr;
		return _ptr_to_glGetUniformLocationARB(programObj,name);
	}
	typedef GLint RET_TYPE;
	return RET_TYPE();
}
GLint (API_ENTRY *_ptr_to_glGetUniformLocationARB)(GLhandleARB programObj,const GLcharARB* name) = _choose_glGetUniformLocationARB;
static void API_ENTRY _choose_glGetUniformSubroutineuiv(GLenum shadertype,GLint location,GLuint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformSubroutineuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformSubroutineuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformSubroutineuiv = (void(API_ENTRY*)(GLenum,GLint,GLuint*))gotPtr;
		return _ptr_to_glGetUniformSubroutineuiv(shadertype,location,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetUniformSubroutineuiv)(GLenum shadertype,GLint location,GLuint* params) = _choose_glGetUniformSubroutineuiv;
static void API_ENTRY _choose_glGetUniformdv(GLuint program,GLint location,GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformdv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformdv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformdv = (void(API_ENTRY*)(GLuint,GLint,GLdouble*))gotPtr;
		return _ptr_to_glGetUniformdv(program,location,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetUniformdv)(GLuint program,GLint location,GLdouble* params) = _choose_glGetUniformdv;
static void API_ENTRY _choose_glGetUniformfv(GLuint program,GLint location,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformfv = (void(API_ENTRY*)(GLuint,GLint,GLfloat*))gotPtr;
		return _ptr_to_glGetUniformfv(program,location,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetUniformfv)(GLuint program,GLint location,GLfloat* params) = _choose_glGetUniformfv;
static void API_ENTRY _choose_glGetUniformfvARB(GLhandleARB programObj,GLint location,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformfvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformfvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformfvARB = (void(API_ENTRY*)(GLhandleARB,GLint,GLfloat*))gotPtr;
		return _ptr_to_glGetUniformfvARB(programObj,location,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetUniformfvARB)(GLhandleARB programObj,GLint location,GLfloat* params) = _choose_glGetUniformfvARB;
static void API_ENTRY _choose_glGetUniformiv(GLuint program,GLint location,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformiv = (void(API_ENTRY*)(GLuint,GLint,GLint*))gotPtr;
		return _ptr_to_glGetUniformiv(program,location,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetUniformiv)(GLuint program,GLint location,GLint* params) = _choose_glGetUniformiv;
static void API_ENTRY _choose_glGetUniformivARB(GLhandleARB programObj,GLint location,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformivARB = (void(API_ENTRY*)(GLhandleARB,GLint,GLint*))gotPtr;
		return _ptr_to_glGetUniformivARB(programObj,location,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetUniformivARB)(GLhandleARB programObj,GLint location,GLint* params) = _choose_glGetUniformivARB;
static void API_ENTRY _choose_glGetUniformuiv(GLuint program,GLint location,GLuint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetUniformuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetUniformuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetUniformuiv = (void(API_ENTRY*)(GLuint,GLint,GLuint*))gotPtr;
		return _ptr_to_glGetUniformuiv(program,location,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetUniformuiv)(GLuint program,GLint location,GLuint* params) = _choose_glGetUniformuiv;
static void API_ENTRY _choose_glGetVertexAttribIiv(GLuint index,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribIiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribIiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribIiv = (void(API_ENTRY*)(GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetVertexAttribIiv(index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribIiv)(GLuint index,GLenum pname,GLint* params) = _choose_glGetVertexAttribIiv;
static void API_ENTRY _choose_glGetVertexAttribIuiv(GLuint index,GLenum pname,GLuint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribIuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribIuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribIuiv = (void(API_ENTRY*)(GLuint,GLenum,GLuint*))gotPtr;
		return _ptr_to_glGetVertexAttribIuiv(index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribIuiv)(GLuint index,GLenum pname,GLuint* params) = _choose_glGetVertexAttribIuiv;
static void API_ENTRY _choose_glGetVertexAttribLdv(GLuint index,GLenum pname,GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribLdv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribLdv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribLdv = (void(API_ENTRY*)(GLuint,GLenum,GLdouble*))gotPtr;
		return _ptr_to_glGetVertexAttribLdv(index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribLdv)(GLuint index,GLenum pname,GLdouble* params) = _choose_glGetVertexAttribLdv;
static void API_ENTRY _choose_glGetVertexAttribPointerv(GLuint index,GLenum pname,GLvoid** pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribPointerv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribPointerv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribPointerv = (void(API_ENTRY*)(GLuint,GLenum,GLvoid**))gotPtr;
		return _ptr_to_glGetVertexAttribPointerv(index,pname,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribPointerv)(GLuint index,GLenum pname,GLvoid** pointer) = _choose_glGetVertexAttribPointerv;
static void API_ENTRY _choose_glGetVertexAttribPointervARB(GLuint index,GLenum pname,GLvoid** pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribPointervARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribPointervARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribPointervARB = (void(API_ENTRY*)(GLuint,GLenum,GLvoid**))gotPtr;
		return _ptr_to_glGetVertexAttribPointervARB(index,pname,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribPointervARB)(GLuint index,GLenum pname,GLvoid** pointer) = _choose_glGetVertexAttribPointervARB;
static void API_ENTRY _choose_glGetVertexAttribdv(GLuint index,GLenum pname,GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribdv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribdv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribdv = (void(API_ENTRY*)(GLuint,GLenum,GLdouble*))gotPtr;
		return _ptr_to_glGetVertexAttribdv(index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribdv)(GLuint index,GLenum pname,GLdouble* params) = _choose_glGetVertexAttribdv;
static void API_ENTRY _choose_glGetVertexAttribdvARB(GLuint index,GLenum pname,GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribdvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribdvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribdvARB = (void(API_ENTRY*)(GLuint,GLenum,GLdouble*))gotPtr;
		return _ptr_to_glGetVertexAttribdvARB(index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribdvARB)(GLuint index,GLenum pname,GLdouble* params) = _choose_glGetVertexAttribdvARB;
static void API_ENTRY _choose_glGetVertexAttribfv(GLuint index,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribfv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribfv = (void(API_ENTRY*)(GLuint,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetVertexAttribfv(index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribfv)(GLuint index,GLenum pname,GLfloat* params) = _choose_glGetVertexAttribfv;
static void API_ENTRY _choose_glGetVertexAttribfvARB(GLuint index,GLenum pname,GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribfvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribfvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribfvARB = (void(API_ENTRY*)(GLuint,GLenum,GLfloat*))gotPtr;
		return _ptr_to_glGetVertexAttribfvARB(index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribfvARB)(GLuint index,GLenum pname,GLfloat* params) = _choose_glGetVertexAttribfvARB;
static void API_ENTRY _choose_glGetVertexAttribiv(GLuint index,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribiv is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribiv = (void(API_ENTRY*)(GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetVertexAttribiv(index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribiv)(GLuint index,GLenum pname,GLint* params) = _choose_glGetVertexAttribiv;
static void API_ENTRY _choose_glGetVertexAttribivARB(GLuint index,GLenum pname,GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glGetVertexAttribivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glGetVertexAttribivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glGetVertexAttribivARB = (void(API_ENTRY*)(GLuint,GLenum,GLint*))gotPtr;
		return _ptr_to_glGetVertexAttribivARB(index,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glGetVertexAttribivARB)(GLuint index,GLenum pname,GLint* params) = _choose_glGetVertexAttribivARB;
static void API_ENTRY _choose_glHint(GLenum target,GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glHint");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glHint is NULL " << std::endl;
	else
	{
		_ptr_to_glHint = (void(API_ENTRY*)(GLenum,GLenum))gotPtr;
		return _ptr_to_glHint(target,mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glHint)(GLenum target,GLenum mode) = _choose_glHint;
static void API_ENTRY _choose_glIndexMask(GLuint mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexMask");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexMask is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexMask = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIndexMask(mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexMask)(GLuint mask) = _choose_glIndexMask;
static void API_ENTRY _choose_glIndexPointer(GLenum type,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexPointer = (void(API_ENTRY*)(GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glIndexPointer(type,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexPointer)(GLenum type,GLsizei stride,const GLvoid* pointer) = _choose_glIndexPointer;
static void API_ENTRY _choose_glIndexd(GLdouble c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexd");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexd is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexd = (void(API_ENTRY*)(GLdouble))gotPtr;
		return _ptr_to_glIndexd(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexd)(GLdouble c) = _choose_glIndexd;
static void API_ENTRY _choose_glIndexdv(const GLdouble* c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexdv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexdv is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexdv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glIndexdv(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexdv)(const GLdouble* c) = _choose_glIndexdv;
static void API_ENTRY _choose_glIndexf(GLfloat c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexf is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexf = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glIndexf(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexf)(GLfloat c) = _choose_glIndexf;
static void API_ENTRY _choose_glIndexfv(const GLfloat* c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexfv is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexfv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glIndexfv(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexfv)(const GLfloat* c) = _choose_glIndexfv;
static void API_ENTRY _choose_glIndexi(GLint c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexi");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexi is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexi = (void(API_ENTRY*)(GLint))gotPtr;
		return _ptr_to_glIndexi(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexi)(GLint c) = _choose_glIndexi;
static void API_ENTRY _choose_glIndexiv(const GLint* c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexiv is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexiv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glIndexiv(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexiv)(const GLint* c) = _choose_glIndexiv;
static void API_ENTRY _choose_glIndexs(GLshort c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexs");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexs is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexs = (void(API_ENTRY*)(GLshort))gotPtr;
		return _ptr_to_glIndexs(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexs)(GLshort c) = _choose_glIndexs;
static void API_ENTRY _choose_glIndexsv(const GLshort* c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexsv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexsv is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexsv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glIndexsv(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexsv)(const GLshort* c) = _choose_glIndexsv;
static void API_ENTRY _choose_glIndexub(GLubyte c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexub");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexub is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexub = (void(API_ENTRY*)(GLubyte))gotPtr;
		return _ptr_to_glIndexub(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexub)(GLubyte c) = _choose_glIndexub;
static void API_ENTRY _choose_glIndexubv(const GLubyte* c)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIndexubv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIndexubv is NULL " << std::endl;
	else
	{
		_ptr_to_glIndexubv = (void(API_ENTRY*)(const GLubyte*))gotPtr;
		return _ptr_to_glIndexubv(c);
	}
	return;
}
void (API_ENTRY *_ptr_to_glIndexubv)(const GLubyte* c) = _choose_glIndexubv;
static void API_ENTRY _choose_glInitNames()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glInitNames");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glInitNames is NULL " << std::endl;
	else
	{
		_ptr_to_glInitNames = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glInitNames();
	}
	return;
}
void (API_ENTRY *_ptr_to_glInitNames)() = _choose_glInitNames;
static void API_ENTRY _choose_glInterleavedArrays(GLenum format,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glInterleavedArrays");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glInterleavedArrays is NULL " << std::endl;
	else
	{
		_ptr_to_glInterleavedArrays = (void(API_ENTRY*)(GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glInterleavedArrays(format,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glInterleavedArrays)(GLenum format,GLsizei stride,const GLvoid* pointer) = _choose_glInterleavedArrays;
static GLboolean API_ENTRY _choose_glIsBuffer(GLuint buffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsBuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsBuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glIsBuffer = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsBuffer(buffer);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsBuffer)(GLuint buffer) = _choose_glIsBuffer;
static GLboolean API_ENTRY _choose_glIsBufferARB(GLuint buffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsBufferARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsBufferARB is NULL " << std::endl;
	else
	{
		_ptr_to_glIsBufferARB = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsBufferARB(buffer);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsBufferARB)(GLuint buffer) = _choose_glIsBufferARB;
static GLboolean API_ENTRY _choose_glIsEnabled(GLenum cap)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsEnabled");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsEnabled is NULL " << std::endl;
	else
	{
		_ptr_to_glIsEnabled = (GLboolean(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glIsEnabled(cap);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsEnabled)(GLenum cap) = _choose_glIsEnabled;
static GLboolean API_ENTRY _choose_glIsEnabledi(GLenum target,GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsEnabledi");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsEnabledi is NULL " << std::endl;
	else
	{
		_ptr_to_glIsEnabledi = (GLboolean(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glIsEnabledi(target,index);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsEnabledi)(GLenum target,GLuint index) = _choose_glIsEnabledi;
static GLboolean API_ENTRY _choose_glIsFramebuffer(GLuint framebuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsFramebuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsFramebuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glIsFramebuffer = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsFramebuffer(framebuffer);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsFramebuffer)(GLuint framebuffer) = _choose_glIsFramebuffer;
static GLboolean API_ENTRY _choose_glIsFramebufferEXT(GLuint framebuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsFramebufferEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsFramebufferEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glIsFramebufferEXT = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsFramebufferEXT(framebuffer);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsFramebufferEXT)(GLuint framebuffer) = _choose_glIsFramebufferEXT;
static GLboolean API_ENTRY _choose_glIsList(GLuint list)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsList");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsList is NULL " << std::endl;
	else
	{
		_ptr_to_glIsList = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsList(list);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsList)(GLuint list) = _choose_glIsList;
static GLboolean API_ENTRY _choose_glIsProgram(GLuint program)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsProgram");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsProgram is NULL " << std::endl;
	else
	{
		_ptr_to_glIsProgram = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsProgram(program);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsProgram)(GLuint program) = _choose_glIsProgram;
static GLboolean API_ENTRY _choose_glIsProgramARB(GLuint program)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsProgramARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsProgramARB is NULL " << std::endl;
	else
	{
		_ptr_to_glIsProgramARB = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsProgramARB(program);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsProgramARB)(GLuint program) = _choose_glIsProgramARB;
static GLboolean API_ENTRY _choose_glIsProgramPipeline(GLuint pipeline)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsProgramPipeline");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsProgramPipeline is NULL " << std::endl;
	else
	{
		_ptr_to_glIsProgramPipeline = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsProgramPipeline(pipeline);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsProgramPipeline)(GLuint pipeline) = _choose_glIsProgramPipeline;
static GLboolean API_ENTRY _choose_glIsQuery(GLuint id)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsQuery");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsQuery is NULL " << std::endl;
	else
	{
		_ptr_to_glIsQuery = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsQuery(id);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsQuery)(GLuint id) = _choose_glIsQuery;
static GLboolean API_ENTRY _choose_glIsRenderbuffer(GLuint renderbuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsRenderbuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsRenderbuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glIsRenderbuffer = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsRenderbuffer(renderbuffer);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsRenderbuffer)(GLuint renderbuffer) = _choose_glIsRenderbuffer;
static GLboolean API_ENTRY _choose_glIsRenderbufferEXT(GLuint renderbuffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsRenderbufferEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsRenderbufferEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glIsRenderbufferEXT = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsRenderbufferEXT(renderbuffer);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsRenderbufferEXT)(GLuint renderbuffer) = _choose_glIsRenderbufferEXT;
static GLboolean API_ENTRY _choose_glIsSampler(GLuint sampler)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsSampler");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsSampler is NULL " << std::endl;
	else
	{
		_ptr_to_glIsSampler = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsSampler(sampler);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsSampler)(GLuint sampler) = _choose_glIsSampler;
static GLboolean API_ENTRY _choose_glIsShader(GLuint shader)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsShader");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsShader is NULL " << std::endl;
	else
	{
		_ptr_to_glIsShader = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsShader(shader);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsShader)(GLuint shader) = _choose_glIsShader;
static GLboolean API_ENTRY _choose_glIsSync(GLsync sync)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsSync");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsSync is NULL " << std::endl;
	else
	{
		_ptr_to_glIsSync = (GLboolean(API_ENTRY*)(GLsync))gotPtr;
		return _ptr_to_glIsSync(sync);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsSync)(GLsync sync) = _choose_glIsSync;
static GLboolean API_ENTRY _choose_glIsTexture(GLuint texture)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsTexture");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsTexture is NULL " << std::endl;
	else
	{
		_ptr_to_glIsTexture = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsTexture(texture);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsTexture)(GLuint texture) = _choose_glIsTexture;
static GLboolean API_ENTRY _choose_glIsTransformFeedback(GLuint id)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsTransformFeedback");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsTransformFeedback is NULL " << std::endl;
	else
	{
		_ptr_to_glIsTransformFeedback = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsTransformFeedback(id);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsTransformFeedback)(GLuint id) = _choose_glIsTransformFeedback;
static GLboolean API_ENTRY _choose_glIsVertexArray(GLuint ren_array)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glIsVertexArray");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glIsVertexArray is NULL " << std::endl;
	else
	{
		_ptr_to_glIsVertexArray = (GLboolean(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glIsVertexArray(ren_array);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glIsVertexArray)(GLuint ren_array) = _choose_glIsVertexArray;
static void API_ENTRY _choose_glLightModelf(GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLightModelf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLightModelf is NULL " << std::endl;
	else
	{
		_ptr_to_glLightModelf = (void(API_ENTRY*)(GLenum,GLfloat))gotPtr;
		return _ptr_to_glLightModelf(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLightModelf)(GLenum pname,GLfloat param) = _choose_glLightModelf;
static void API_ENTRY _choose_glLightModelfv(GLenum pname,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLightModelfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLightModelfv is NULL " << std::endl;
	else
	{
		_ptr_to_glLightModelfv = (void(API_ENTRY*)(GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glLightModelfv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLightModelfv)(GLenum pname,const GLfloat* params) = _choose_glLightModelfv;
static void API_ENTRY _choose_glLightModeli(GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLightModeli");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLightModeli is NULL " << std::endl;
	else
	{
		_ptr_to_glLightModeli = (void(API_ENTRY*)(GLenum,GLint))gotPtr;
		return _ptr_to_glLightModeli(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLightModeli)(GLenum pname,GLint param) = _choose_glLightModeli;
static void API_ENTRY _choose_glLightModeliv(GLenum pname,const GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLightModeliv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLightModeliv is NULL " << std::endl;
	else
	{
		_ptr_to_glLightModeliv = (void(API_ENTRY*)(GLenum,const GLint*))gotPtr;
		return _ptr_to_glLightModeliv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLightModeliv)(GLenum pname,const GLint* params) = _choose_glLightModeliv;
static void API_ENTRY _choose_glLightf(GLenum light,GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLightf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLightf is NULL " << std::endl;
	else
	{
		_ptr_to_glLightf = (void(API_ENTRY*)(GLenum,GLenum,GLfloat))gotPtr;
		return _ptr_to_glLightf(light,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLightf)(GLenum light,GLenum pname,GLfloat param) = _choose_glLightf;
static void API_ENTRY _choose_glLightfv(GLenum light,GLenum pname,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLightfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLightfv is NULL " << std::endl;
	else
	{
		_ptr_to_glLightfv = (void(API_ENTRY*)(GLenum,GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glLightfv(light,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLightfv)(GLenum light,GLenum pname,const GLfloat* params) = _choose_glLightfv;
static void API_ENTRY _choose_glLighti(GLenum light,GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLighti");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLighti is NULL " << std::endl;
	else
	{
		_ptr_to_glLighti = (void(API_ENTRY*)(GLenum,GLenum,GLint))gotPtr;
		return _ptr_to_glLighti(light,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLighti)(GLenum light,GLenum pname,GLint param) = _choose_glLighti;
static void API_ENTRY _choose_glLightiv(GLenum light,GLenum pname,const GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLightiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLightiv is NULL " << std::endl;
	else
	{
		_ptr_to_glLightiv = (void(API_ENTRY*)(GLenum,GLenum,const GLint*))gotPtr;
		return _ptr_to_glLightiv(light,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLightiv)(GLenum light,GLenum pname,const GLint* params) = _choose_glLightiv;
static void API_ENTRY _choose_glLineStipple(GLint factor,GLushort pattern)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLineStipple");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLineStipple is NULL " << std::endl;
	else
	{
		_ptr_to_glLineStipple = (void(API_ENTRY*)(GLint,GLushort))gotPtr;
		return _ptr_to_glLineStipple(factor,pattern);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLineStipple)(GLint factor,GLushort pattern) = _choose_glLineStipple;
static void API_ENTRY _choose_glLineWidth(GLfloat width)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLineWidth");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLineWidth is NULL " << std::endl;
	else
	{
		_ptr_to_glLineWidth = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glLineWidth(width);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLineWidth)(GLfloat width) = _choose_glLineWidth;
static void API_ENTRY _choose_glLinkProgram(GLuint program)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLinkProgram");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLinkProgram is NULL " << std::endl;
	else
	{
		_ptr_to_glLinkProgram = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glLinkProgram(program);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLinkProgram)(GLuint program) = _choose_glLinkProgram;
static void API_ENTRY _choose_glLinkProgramARB(GLhandleARB programObj)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLinkProgramARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLinkProgramARB is NULL " << std::endl;
	else
	{
		_ptr_to_glLinkProgramARB = (void(API_ENTRY*)(GLhandleARB))gotPtr;
		return _ptr_to_glLinkProgramARB(programObj);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLinkProgramARB)(GLhandleARB programObj) = _choose_glLinkProgramARB;
static void API_ENTRY _choose_glListBase(GLuint base)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glListBase");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glListBase is NULL " << std::endl;
	else
	{
		_ptr_to_glListBase = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glListBase(base);
	}
	return;
}
void (API_ENTRY *_ptr_to_glListBase)(GLuint base) = _choose_glListBase;
static void API_ENTRY _choose_glLoadIdentity()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLoadIdentity");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLoadIdentity is NULL " << std::endl;
	else
	{
		_ptr_to_glLoadIdentity = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glLoadIdentity();
	}
	return;
}
void (API_ENTRY *_ptr_to_glLoadIdentity)() = _choose_glLoadIdentity;
static void API_ENTRY _choose_glLoadMatrixd(const GLdouble* m)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLoadMatrixd");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLoadMatrixd is NULL " << std::endl;
	else
	{
		_ptr_to_glLoadMatrixd = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glLoadMatrixd(m);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLoadMatrixd)(const GLdouble* m) = _choose_glLoadMatrixd;
static void API_ENTRY _choose_glLoadMatrixf(const GLfloat* m)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLoadMatrixf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLoadMatrixf is NULL " << std::endl;
	else
	{
		_ptr_to_glLoadMatrixf = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glLoadMatrixf(m);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLoadMatrixf)(const GLfloat* m) = _choose_glLoadMatrixf;
static void API_ENTRY _choose_glLoadName(GLuint name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLoadName");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLoadName is NULL " << std::endl;
	else
	{
		_ptr_to_glLoadName = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glLoadName(name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLoadName)(GLuint name) = _choose_glLoadName;
static void API_ENTRY _choose_glLoadTransposeMatrixd(const GLdouble* m)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLoadTransposeMatrixd");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLoadTransposeMatrixd is NULL " << std::endl;
	else
	{
		_ptr_to_glLoadTransposeMatrixd = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glLoadTransposeMatrixd(m);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLoadTransposeMatrixd)(const GLdouble* m) = _choose_glLoadTransposeMatrixd;
static void API_ENTRY _choose_glLoadTransposeMatrixf(const GLfloat* m)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLoadTransposeMatrixf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLoadTransposeMatrixf is NULL " << std::endl;
	else
	{
		_ptr_to_glLoadTransposeMatrixf = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glLoadTransposeMatrixf(m);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLoadTransposeMatrixf)(const GLfloat* m) = _choose_glLoadTransposeMatrixf;
static void API_ENTRY _choose_glLogicOp(GLenum opcode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glLogicOp");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glLogicOp is NULL " << std::endl;
	else
	{
		_ptr_to_glLogicOp = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glLogicOp(opcode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glLogicOp)(GLenum opcode) = _choose_glLogicOp;
static void API_ENTRY _choose_glMap1d(GLenum target,GLdouble u1,GLdouble u2,GLint stride,GLint order,const GLdouble* points)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMap1d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMap1d is NULL " << std::endl;
	else
	{
		_ptr_to_glMap1d = (void(API_ENTRY*)(GLenum,GLdouble,GLdouble,GLint,GLint,const GLdouble*))gotPtr;
		return _ptr_to_glMap1d(target,u1,u2,stride,order,points);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMap1d)(GLenum target,GLdouble u1,GLdouble u2,GLint stride,GLint order,const GLdouble* points) = _choose_glMap1d;
static void API_ENTRY _choose_glMap1f(GLenum target,GLfloat u1,GLfloat u2,GLint stride,GLint order,const GLfloat* points)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMap1f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMap1f is NULL " << std::endl;
	else
	{
		_ptr_to_glMap1f = (void(API_ENTRY*)(GLenum,GLfloat,GLfloat,GLint,GLint,const GLfloat*))gotPtr;
		return _ptr_to_glMap1f(target,u1,u2,stride,order,points);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMap1f)(GLenum target,GLfloat u1,GLfloat u2,GLint stride,GLint order,const GLfloat* points) = _choose_glMap1f;
static void API_ENTRY _choose_glMap2d(GLenum target,GLdouble u1,GLdouble u2,GLint ustride,GLint uorder,GLdouble v1,GLdouble v2,GLint vstride,GLint vorder,const GLdouble* points)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMap2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMap2d is NULL " << std::endl;
	else
	{
		_ptr_to_glMap2d = (void(API_ENTRY*)(GLenum,GLdouble,GLdouble,GLint,GLint,GLdouble,GLdouble,GLint,GLint,const GLdouble*))gotPtr;
		return _ptr_to_glMap2d(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMap2d)(GLenum target,GLdouble u1,GLdouble u2,GLint ustride,GLint uorder,GLdouble v1,GLdouble v2,GLint vstride,GLint vorder,const GLdouble* points) = _choose_glMap2d;
static void API_ENTRY _choose_glMap2f(GLenum target,GLfloat u1,GLfloat u2,GLint ustride,GLint uorder,GLfloat v1,GLfloat v2,GLint vstride,GLint vorder,const GLfloat* points)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMap2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMap2f is NULL " << std::endl;
	else
	{
		_ptr_to_glMap2f = (void(API_ENTRY*)(GLenum,GLfloat,GLfloat,GLint,GLint,GLfloat,GLfloat,GLint,GLint,const GLfloat*))gotPtr;
		return _ptr_to_glMap2f(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMap2f)(GLenum target,GLfloat u1,GLfloat u2,GLint ustride,GLint uorder,GLfloat v1,GLfloat v2,GLint vstride,GLint vorder,const GLfloat* points) = _choose_glMap2f;
static void API_ENTRY _choose_glMapBuffer(GLenum target,GLenum access)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMapBuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMapBuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glMapBuffer = (void(API_ENTRY*)(GLenum,GLenum))gotPtr;
		return _ptr_to_glMapBuffer(target,access);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMapBuffer)(GLenum target,GLenum access) = _choose_glMapBuffer;
static void API_ENTRY _choose_glMapBufferARB(GLenum target,GLenum access)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMapBufferARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMapBufferARB is NULL " << std::endl;
	else
	{
		_ptr_to_glMapBufferARB = (void(API_ENTRY*)(GLenum,GLenum))gotPtr;
		return _ptr_to_glMapBufferARB(target,access);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMapBufferARB)(GLenum target,GLenum access) = _choose_glMapBufferARB;
static void API_ENTRY _choose_glMapBufferRange(GLenum target,GLintptr offset,GLsizeiptr length,GLbitfield access)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMapBufferRange");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMapBufferRange is NULL " << std::endl;
	else
	{
		_ptr_to_glMapBufferRange = (void(API_ENTRY*)(GLenum,GLintptr,GLsizeiptr,GLbitfield))gotPtr;
		return _ptr_to_glMapBufferRange(target,offset,length,access);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMapBufferRange)(GLenum target,GLintptr offset,GLsizeiptr length,GLbitfield access) = _choose_glMapBufferRange;
static void API_ENTRY _choose_glMapGrid1d(GLint un,GLdouble u1,GLdouble u2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMapGrid1d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMapGrid1d is NULL " << std::endl;
	else
	{
		_ptr_to_glMapGrid1d = (void(API_ENTRY*)(GLint,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glMapGrid1d(un,u1,u2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMapGrid1d)(GLint un,GLdouble u1,GLdouble u2) = _choose_glMapGrid1d;
static void API_ENTRY _choose_glMapGrid1f(GLint un,GLfloat u1,GLfloat u2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMapGrid1f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMapGrid1f is NULL " << std::endl;
	else
	{
		_ptr_to_glMapGrid1f = (void(API_ENTRY*)(GLint,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glMapGrid1f(un,u1,u2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMapGrid1f)(GLint un,GLfloat u1,GLfloat u2) = _choose_glMapGrid1f;
static void API_ENTRY _choose_glMapGrid2d(GLint un,GLdouble u1,GLdouble u2,GLint vn,GLdouble v1,GLdouble v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMapGrid2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMapGrid2d is NULL " << std::endl;
	else
	{
		_ptr_to_glMapGrid2d = (void(API_ENTRY*)(GLint,GLdouble,GLdouble,GLint,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glMapGrid2d(un,u1,u2,vn,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMapGrid2d)(GLint un,GLdouble u1,GLdouble u2,GLint vn,GLdouble v1,GLdouble v2) = _choose_glMapGrid2d;
static void API_ENTRY _choose_glMapGrid2f(GLint un,GLfloat u1,GLfloat u2,GLint vn,GLfloat v1,GLfloat v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMapGrid2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMapGrid2f is NULL " << std::endl;
	else
	{
		_ptr_to_glMapGrid2f = (void(API_ENTRY*)(GLint,GLfloat,GLfloat,GLint,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glMapGrid2f(un,u1,u2,vn,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMapGrid2f)(GLint un,GLfloat u1,GLfloat u2,GLint vn,GLfloat v1,GLfloat v2) = _choose_glMapGrid2f;
static void API_ENTRY _choose_glMaterialf(GLenum face,GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMaterialf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMaterialf is NULL " << std::endl;
	else
	{
		_ptr_to_glMaterialf = (void(API_ENTRY*)(GLenum,GLenum,GLfloat))gotPtr;
		return _ptr_to_glMaterialf(face,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMaterialf)(GLenum face,GLenum pname,GLfloat param) = _choose_glMaterialf;
static void API_ENTRY _choose_glMaterialfv(GLenum face,GLenum pname,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMaterialfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMaterialfv is NULL " << std::endl;
	else
	{
		_ptr_to_glMaterialfv = (void(API_ENTRY*)(GLenum,GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glMaterialfv(face,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMaterialfv)(GLenum face,GLenum pname,const GLfloat* params) = _choose_glMaterialfv;
static void API_ENTRY _choose_glMateriali(GLenum face,GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMateriali");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMateriali is NULL " << std::endl;
	else
	{
		_ptr_to_glMateriali = (void(API_ENTRY*)(GLenum,GLenum,GLint))gotPtr;
		return _ptr_to_glMateriali(face,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMateriali)(GLenum face,GLenum pname,GLint param) = _choose_glMateriali;
static void API_ENTRY _choose_glMaterialiv(GLenum face,GLenum pname,const GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMaterialiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMaterialiv is NULL " << std::endl;
	else
	{
		_ptr_to_glMaterialiv = (void(API_ENTRY*)(GLenum,GLenum,const GLint*))gotPtr;
		return _ptr_to_glMaterialiv(face,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMaterialiv)(GLenum face,GLenum pname,const GLint* params) = _choose_glMaterialiv;
static void API_ENTRY _choose_glMatrixMode(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMatrixMode");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMatrixMode is NULL " << std::endl;
	else
	{
		_ptr_to_glMatrixMode = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glMatrixMode(mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMatrixMode)(GLenum mode) = _choose_glMatrixMode;
static void API_ENTRY _choose_glMemoryBarrier(GLbitfield barriers)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMemoryBarrier");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMemoryBarrier is NULL " << std::endl;
	else
	{
		_ptr_to_glMemoryBarrier = (void(API_ENTRY*)(GLbitfield))gotPtr;
		return _ptr_to_glMemoryBarrier(barriers);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMemoryBarrier)(GLbitfield barriers) = _choose_glMemoryBarrier;
static void API_ENTRY _choose_glMinSampleShading(GLfloat value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMinSampleShading");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMinSampleShading is NULL " << std::endl;
	else
	{
		_ptr_to_glMinSampleShading = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glMinSampleShading(value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMinSampleShading)(GLfloat value) = _choose_glMinSampleShading;
static void API_ENTRY _choose_glMultMatrixd(const GLdouble* m)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultMatrixd");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultMatrixd is NULL " << std::endl;
	else
	{
		_ptr_to_glMultMatrixd = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glMultMatrixd(m);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultMatrixd)(const GLdouble* m) = _choose_glMultMatrixd;
static void API_ENTRY _choose_glMultMatrixf(const GLfloat* m)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultMatrixf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultMatrixf is NULL " << std::endl;
	else
	{
		_ptr_to_glMultMatrixf = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glMultMatrixf(m);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultMatrixf)(const GLfloat* m) = _choose_glMultMatrixf;
static void API_ENTRY _choose_glMultTransposeMatrixd(const GLdouble* m)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultTransposeMatrixd");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultTransposeMatrixd is NULL " << std::endl;
	else
	{
		_ptr_to_glMultTransposeMatrixd = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glMultTransposeMatrixd(m);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultTransposeMatrixd)(const GLdouble* m) = _choose_glMultTransposeMatrixd;
static void API_ENTRY _choose_glMultTransposeMatrixf(const GLfloat* m)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultTransposeMatrixf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultTransposeMatrixf is NULL " << std::endl;
	else
	{
		_ptr_to_glMultTransposeMatrixf = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glMultTransposeMatrixf(m);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultTransposeMatrixf)(const GLfloat* m) = _choose_glMultTransposeMatrixf;
static void API_ENTRY _choose_glMultiDrawArrays(GLenum mode,const GLint* first,const GLsizei* count,GLsizei drawcount)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiDrawArrays");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiDrawArrays is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiDrawArrays = (void(API_ENTRY*)(GLenum,const GLint*,const GLsizei*,GLsizei))gotPtr;
		return _ptr_to_glMultiDrawArrays(mode,first,count,drawcount);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiDrawArrays)(GLenum mode,const GLint* first,const GLsizei* count,GLsizei drawcount) = _choose_glMultiDrawArrays;
static void API_ENTRY _choose_glMultiDrawElements(GLenum mode,const GLsizei* count,GLenum type,const GLvoid*const* indices,GLsizei drawcount)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiDrawElements");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiDrawElements is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiDrawElements = (void(API_ENTRY*)(GLenum,const GLsizei*,GLenum,const GLvoid*const*,GLsizei))gotPtr;
		return _ptr_to_glMultiDrawElements(mode,count,type,indices,drawcount);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiDrawElements)(GLenum mode,const GLsizei* count,GLenum type,const GLvoid*const* indices,GLsizei drawcount) = _choose_glMultiDrawElements;
static void API_ENTRY _choose_glMultiDrawElementsBaseVertex(GLenum mode,const GLsizei* count,GLenum type,const GLvoid*const* indices,GLsizei drawcount,const GLint* basevertex)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiDrawElementsBaseVertex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiDrawElementsBaseVertex is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiDrawElementsBaseVertex = (void(API_ENTRY*)(GLenum,const GLsizei*,GLenum,const GLvoid*const*,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glMultiDrawElementsBaseVertex(mode,count,type,indices,drawcount,basevertex);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiDrawElementsBaseVertex)(GLenum mode,const GLsizei* count,GLenum type,const GLvoid*const* indices,GLsizei drawcount,const GLint* basevertex) = _choose_glMultiDrawElementsBaseVertex;
static void API_ENTRY _choose_glMultiTexCoord1d(GLenum target,GLdouble s)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord1d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord1d is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord1d = (void(API_ENTRY*)(GLenum,GLdouble))gotPtr;
		return _ptr_to_glMultiTexCoord1d(target,s);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord1d)(GLenum target,GLdouble s) = _choose_glMultiTexCoord1d;
static void API_ENTRY _choose_glMultiTexCoord1dv(GLenum target,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord1dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord1dv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord1dv = (void(API_ENTRY*)(GLenum,const GLdouble*))gotPtr;
		return _ptr_to_glMultiTexCoord1dv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord1dv)(GLenum target,const GLdouble* v) = _choose_glMultiTexCoord1dv;
static void API_ENTRY _choose_glMultiTexCoord1f(GLenum target,GLfloat s)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord1f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord1f is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord1f = (void(API_ENTRY*)(GLenum,GLfloat))gotPtr;
		return _ptr_to_glMultiTexCoord1f(target,s);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord1f)(GLenum target,GLfloat s) = _choose_glMultiTexCoord1f;
static void API_ENTRY _choose_glMultiTexCoord1fv(GLenum target,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord1fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord1fv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord1fv = (void(API_ENTRY*)(GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glMultiTexCoord1fv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord1fv)(GLenum target,const GLfloat* v) = _choose_glMultiTexCoord1fv;
static void API_ENTRY _choose_glMultiTexCoord1i(GLenum target,GLint s)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord1i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord1i is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord1i = (void(API_ENTRY*)(GLenum,GLint))gotPtr;
		return _ptr_to_glMultiTexCoord1i(target,s);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord1i)(GLenum target,GLint s) = _choose_glMultiTexCoord1i;
static void API_ENTRY _choose_glMultiTexCoord1iv(GLenum target,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord1iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord1iv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord1iv = (void(API_ENTRY*)(GLenum,const GLint*))gotPtr;
		return _ptr_to_glMultiTexCoord1iv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord1iv)(GLenum target,const GLint* v) = _choose_glMultiTexCoord1iv;
static void API_ENTRY _choose_glMultiTexCoord1s(GLenum target,GLshort s)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord1s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord1s is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord1s = (void(API_ENTRY*)(GLenum,GLshort))gotPtr;
		return _ptr_to_glMultiTexCoord1s(target,s);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord1s)(GLenum target,GLshort s) = _choose_glMultiTexCoord1s;
static void API_ENTRY _choose_glMultiTexCoord1sv(GLenum target,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord1sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord1sv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord1sv = (void(API_ENTRY*)(GLenum,const GLshort*))gotPtr;
		return _ptr_to_glMultiTexCoord1sv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord1sv)(GLenum target,const GLshort* v) = _choose_glMultiTexCoord1sv;
static void API_ENTRY _choose_glMultiTexCoord2d(GLenum target,GLdouble s,GLdouble t)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord2d is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord2d = (void(API_ENTRY*)(GLenum,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glMultiTexCoord2d(target,s,t);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord2d)(GLenum target,GLdouble s,GLdouble t) = _choose_glMultiTexCoord2d;
static void API_ENTRY _choose_glMultiTexCoord2dv(GLenum target,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord2dv = (void(API_ENTRY*)(GLenum,const GLdouble*))gotPtr;
		return _ptr_to_glMultiTexCoord2dv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord2dv)(GLenum target,const GLdouble* v) = _choose_glMultiTexCoord2dv;
static void API_ENTRY _choose_glMultiTexCoord2f(GLenum target,GLfloat s,GLfloat t)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord2f is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord2f = (void(API_ENTRY*)(GLenum,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glMultiTexCoord2f(target,s,t);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord2f)(GLenum target,GLfloat s,GLfloat t) = _choose_glMultiTexCoord2f;
static void API_ENTRY _choose_glMultiTexCoord2fv(GLenum target,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord2fv = (void(API_ENTRY*)(GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glMultiTexCoord2fv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord2fv)(GLenum target,const GLfloat* v) = _choose_glMultiTexCoord2fv;
static void API_ENTRY _choose_glMultiTexCoord2i(GLenum target,GLint s,GLint t)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord2i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord2i is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord2i = (void(API_ENTRY*)(GLenum,GLint,GLint))gotPtr;
		return _ptr_to_glMultiTexCoord2i(target,s,t);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord2i)(GLenum target,GLint s,GLint t) = _choose_glMultiTexCoord2i;
static void API_ENTRY _choose_glMultiTexCoord2iv(GLenum target,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord2iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord2iv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord2iv = (void(API_ENTRY*)(GLenum,const GLint*))gotPtr;
		return _ptr_to_glMultiTexCoord2iv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord2iv)(GLenum target,const GLint* v) = _choose_glMultiTexCoord2iv;
static void API_ENTRY _choose_glMultiTexCoord2s(GLenum target,GLshort s,GLshort t)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord2s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord2s is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord2s = (void(API_ENTRY*)(GLenum,GLshort,GLshort))gotPtr;
		return _ptr_to_glMultiTexCoord2s(target,s,t);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord2s)(GLenum target,GLshort s,GLshort t) = _choose_glMultiTexCoord2s;
static void API_ENTRY _choose_glMultiTexCoord2sv(GLenum target,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord2sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord2sv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord2sv = (void(API_ENTRY*)(GLenum,const GLshort*))gotPtr;
		return _ptr_to_glMultiTexCoord2sv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord2sv)(GLenum target,const GLshort* v) = _choose_glMultiTexCoord2sv;
static void API_ENTRY _choose_glMultiTexCoord3d(GLenum target,GLdouble s,GLdouble t,GLdouble r)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord3d is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord3d = (void(API_ENTRY*)(GLenum,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glMultiTexCoord3d(target,s,t,r);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord3d)(GLenum target,GLdouble s,GLdouble t,GLdouble r) = _choose_glMultiTexCoord3d;
static void API_ENTRY _choose_glMultiTexCoord3dv(GLenum target,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord3dv = (void(API_ENTRY*)(GLenum,const GLdouble*))gotPtr;
		return _ptr_to_glMultiTexCoord3dv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord3dv)(GLenum target,const GLdouble* v) = _choose_glMultiTexCoord3dv;
static void API_ENTRY _choose_glMultiTexCoord3f(GLenum target,GLfloat s,GLfloat t,GLfloat r)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord3f is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord3f = (void(API_ENTRY*)(GLenum,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glMultiTexCoord3f(target,s,t,r);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord3f)(GLenum target,GLfloat s,GLfloat t,GLfloat r) = _choose_glMultiTexCoord3f;
static void API_ENTRY _choose_glMultiTexCoord3fv(GLenum target,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord3fv = (void(API_ENTRY*)(GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glMultiTexCoord3fv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord3fv)(GLenum target,const GLfloat* v) = _choose_glMultiTexCoord3fv;
static void API_ENTRY _choose_glMultiTexCoord3i(GLenum target,GLint s,GLint t,GLint r)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord3i is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord3i = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glMultiTexCoord3i(target,s,t,r);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord3i)(GLenum target,GLint s,GLint t,GLint r) = _choose_glMultiTexCoord3i;
static void API_ENTRY _choose_glMultiTexCoord3iv(GLenum target,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord3iv = (void(API_ENTRY*)(GLenum,const GLint*))gotPtr;
		return _ptr_to_glMultiTexCoord3iv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord3iv)(GLenum target,const GLint* v) = _choose_glMultiTexCoord3iv;
static void API_ENTRY _choose_glMultiTexCoord3s(GLenum target,GLshort s,GLshort t,GLshort r)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord3s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord3s is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord3s = (void(API_ENTRY*)(GLenum,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glMultiTexCoord3s(target,s,t,r);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord3s)(GLenum target,GLshort s,GLshort t,GLshort r) = _choose_glMultiTexCoord3s;
static void API_ENTRY _choose_glMultiTexCoord3sv(GLenum target,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord3sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord3sv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord3sv = (void(API_ENTRY*)(GLenum,const GLshort*))gotPtr;
		return _ptr_to_glMultiTexCoord3sv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord3sv)(GLenum target,const GLshort* v) = _choose_glMultiTexCoord3sv;
static void API_ENTRY _choose_glMultiTexCoord4d(GLenum target,GLdouble s,GLdouble t,GLdouble r,GLdouble q)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord4d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord4d is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord4d = (void(API_ENTRY*)(GLenum,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glMultiTexCoord4d(target,s,t,r,q);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord4d)(GLenum target,GLdouble s,GLdouble t,GLdouble r,GLdouble q) = _choose_glMultiTexCoord4d;
static void API_ENTRY _choose_glMultiTexCoord4dv(GLenum target,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord4dv = (void(API_ENTRY*)(GLenum,const GLdouble*))gotPtr;
		return _ptr_to_glMultiTexCoord4dv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord4dv)(GLenum target,const GLdouble* v) = _choose_glMultiTexCoord4dv;
static void API_ENTRY _choose_glMultiTexCoord4f(GLenum target,GLfloat s,GLfloat t,GLfloat r,GLfloat q)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord4f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord4f is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord4f = (void(API_ENTRY*)(GLenum,GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glMultiTexCoord4f(target,s,t,r,q);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord4f)(GLenum target,GLfloat s,GLfloat t,GLfloat r,GLfloat q) = _choose_glMultiTexCoord4f;
static void API_ENTRY _choose_glMultiTexCoord4fv(GLenum target,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord4fv = (void(API_ENTRY*)(GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glMultiTexCoord4fv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord4fv)(GLenum target,const GLfloat* v) = _choose_glMultiTexCoord4fv;
static void API_ENTRY _choose_glMultiTexCoord4i(GLenum target,GLint s,GLint t,GLint r,GLint q)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord4i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord4i is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord4i = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glMultiTexCoord4i(target,s,t,r,q);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord4i)(GLenum target,GLint s,GLint t,GLint r,GLint q) = _choose_glMultiTexCoord4i;
static void API_ENTRY _choose_glMultiTexCoord4iv(GLenum target,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord4iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord4iv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord4iv = (void(API_ENTRY*)(GLenum,const GLint*))gotPtr;
		return _ptr_to_glMultiTexCoord4iv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord4iv)(GLenum target,const GLint* v) = _choose_glMultiTexCoord4iv;
static void API_ENTRY _choose_glMultiTexCoord4s(GLenum target,GLshort s,GLshort t,GLshort r,GLshort q)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord4s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord4s is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord4s = (void(API_ENTRY*)(GLenum,GLshort,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glMultiTexCoord4s(target,s,t,r,q);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord4s)(GLenum target,GLshort s,GLshort t,GLshort r,GLshort q) = _choose_glMultiTexCoord4s;
static void API_ENTRY _choose_glMultiTexCoord4sv(GLenum target,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoord4sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoord4sv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoord4sv = (void(API_ENTRY*)(GLenum,const GLshort*))gotPtr;
		return _ptr_to_glMultiTexCoord4sv(target,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoord4sv)(GLenum target,const GLshort* v) = _choose_glMultiTexCoord4sv;
static void API_ENTRY _choose_glMultiTexCoordP1ui(GLenum texture,GLenum type,GLuint coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoordP1ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoordP1ui is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoordP1ui = (void(API_ENTRY*)(GLenum,GLenum,GLuint))gotPtr;
		return _ptr_to_glMultiTexCoordP1ui(texture,type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoordP1ui)(GLenum texture,GLenum type,GLuint coords) = _choose_glMultiTexCoordP1ui;
static void API_ENTRY _choose_glMultiTexCoordP1uiv(GLenum texture,GLenum type,const GLuint* coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoordP1uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoordP1uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoordP1uiv = (void(API_ENTRY*)(GLenum,GLenum,const GLuint*))gotPtr;
		return _ptr_to_glMultiTexCoordP1uiv(texture,type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoordP1uiv)(GLenum texture,GLenum type,const GLuint* coords) = _choose_glMultiTexCoordP1uiv;
static void API_ENTRY _choose_glMultiTexCoordP2ui(GLenum texture,GLenum type,GLuint coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoordP2ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoordP2ui is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoordP2ui = (void(API_ENTRY*)(GLenum,GLenum,GLuint))gotPtr;
		return _ptr_to_glMultiTexCoordP2ui(texture,type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoordP2ui)(GLenum texture,GLenum type,GLuint coords) = _choose_glMultiTexCoordP2ui;
static void API_ENTRY _choose_glMultiTexCoordP2uiv(GLenum texture,GLenum type,const GLuint* coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoordP2uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoordP2uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoordP2uiv = (void(API_ENTRY*)(GLenum,GLenum,const GLuint*))gotPtr;
		return _ptr_to_glMultiTexCoordP2uiv(texture,type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoordP2uiv)(GLenum texture,GLenum type,const GLuint* coords) = _choose_glMultiTexCoordP2uiv;
static void API_ENTRY _choose_glMultiTexCoordP3ui(GLenum texture,GLenum type,GLuint coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoordP3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoordP3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoordP3ui = (void(API_ENTRY*)(GLenum,GLenum,GLuint))gotPtr;
		return _ptr_to_glMultiTexCoordP3ui(texture,type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoordP3ui)(GLenum texture,GLenum type,GLuint coords) = _choose_glMultiTexCoordP3ui;
static void API_ENTRY _choose_glMultiTexCoordP3uiv(GLenum texture,GLenum type,const GLuint* coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoordP3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoordP3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoordP3uiv = (void(API_ENTRY*)(GLenum,GLenum,const GLuint*))gotPtr;
		return _ptr_to_glMultiTexCoordP3uiv(texture,type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoordP3uiv)(GLenum texture,GLenum type,const GLuint* coords) = _choose_glMultiTexCoordP3uiv;
static void API_ENTRY _choose_glMultiTexCoordP4ui(GLenum texture,GLenum type,GLuint coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoordP4ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoordP4ui is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoordP4ui = (void(API_ENTRY*)(GLenum,GLenum,GLuint))gotPtr;
		return _ptr_to_glMultiTexCoordP4ui(texture,type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoordP4ui)(GLenum texture,GLenum type,GLuint coords) = _choose_glMultiTexCoordP4ui;
static void API_ENTRY _choose_glMultiTexCoordP4uiv(GLenum texture,GLenum type,const GLuint* coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glMultiTexCoordP4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glMultiTexCoordP4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glMultiTexCoordP4uiv = (void(API_ENTRY*)(GLenum,GLenum,const GLuint*))gotPtr;
		return _ptr_to_glMultiTexCoordP4uiv(texture,type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glMultiTexCoordP4uiv)(GLenum texture,GLenum type,const GLuint* coords) = _choose_glMultiTexCoordP4uiv;
static void API_ENTRY _choose_glNewList(GLuint list,GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNewList");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNewList is NULL " << std::endl;
	else
	{
		_ptr_to_glNewList = (void(API_ENTRY*)(GLuint,GLenum))gotPtr;
		return _ptr_to_glNewList(list,mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNewList)(GLuint list,GLenum mode) = _choose_glNewList;
static void API_ENTRY _choose_glNormal3b(GLbyte nx,GLbyte ny,GLbyte nz)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3b");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3b is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3b = (void(API_ENTRY*)(GLbyte,GLbyte,GLbyte))gotPtr;
		return _ptr_to_glNormal3b(nx,ny,nz);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3b)(GLbyte nx,GLbyte ny,GLbyte nz) = _choose_glNormal3b;
static void API_ENTRY _choose_glNormal3bv(const GLbyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3bv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3bv is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3bv = (void(API_ENTRY*)(const GLbyte*))gotPtr;
		return _ptr_to_glNormal3bv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3bv)(const GLbyte* v) = _choose_glNormal3bv;
static void API_ENTRY _choose_glNormal3d(GLdouble nx,GLdouble ny,GLdouble nz)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3d is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glNormal3d(nx,ny,nz);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3d)(GLdouble nx,GLdouble ny,GLdouble nz) = _choose_glNormal3d;
static void API_ENTRY _choose_glNormal3dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glNormal3dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3dv)(const GLdouble* v) = _choose_glNormal3dv;
static void API_ENTRY _choose_glNormal3f(GLfloat nx,GLfloat ny,GLfloat nz)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3f is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glNormal3f(nx,ny,nz);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3f)(GLfloat nx,GLfloat ny,GLfloat nz) = _choose_glNormal3f;
static void API_ENTRY _choose_glNormal3fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glNormal3fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3fv)(const GLfloat* v) = _choose_glNormal3fv;
static void API_ENTRY _choose_glNormal3i(GLint nx,GLint ny,GLint nz)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3i is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3i = (void(API_ENTRY*)(GLint,GLint,GLint))gotPtr;
		return _ptr_to_glNormal3i(nx,ny,nz);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3i)(GLint nx,GLint ny,GLint nz) = _choose_glNormal3i;
static void API_ENTRY _choose_glNormal3iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glNormal3iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3iv)(const GLint* v) = _choose_glNormal3iv;
static void API_ENTRY _choose_glNormal3s(GLshort nx,GLshort ny,GLshort nz)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3s is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3s = (void(API_ENTRY*)(GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glNormal3s(nx,ny,nz);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3s)(GLshort nx,GLshort ny,GLshort nz) = _choose_glNormal3s;
static void API_ENTRY _choose_glNormal3sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormal3sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormal3sv is NULL " << std::endl;
	else
	{
		_ptr_to_glNormal3sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glNormal3sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormal3sv)(const GLshort* v) = _choose_glNormal3sv;
static void API_ENTRY _choose_glNormalP3ui(GLenum type,GLuint coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormalP3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormalP3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glNormalP3ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glNormalP3ui(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormalP3ui)(GLenum type,GLuint coords) = _choose_glNormalP3ui;
static void API_ENTRY _choose_glNormalP3uiv(GLenum type,const GLuint* coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormalP3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormalP3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glNormalP3uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glNormalP3uiv(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormalP3uiv)(GLenum type,const GLuint* coords) = _choose_glNormalP3uiv;
static void API_ENTRY _choose_glNormalPointer(GLenum type,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glNormalPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glNormalPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glNormalPointer = (void(API_ENTRY*)(GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glNormalPointer(type,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glNormalPointer)(GLenum type,GLsizei stride,const GLvoid* pointer) = _choose_glNormalPointer;
static void API_ENTRY _choose_glOrtho(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glOrtho");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glOrtho is NULL " << std::endl;
	else
	{
		_ptr_to_glOrtho = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glOrtho(left,right,bottom,top,zNear,zFar);
	}
	return;
}
void (API_ENTRY *_ptr_to_glOrtho)(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar) = _choose_glOrtho;
static void API_ENTRY _choose_glPassThrough(GLfloat token)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPassThrough");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPassThrough is NULL " << std::endl;
	else
	{
		_ptr_to_glPassThrough = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glPassThrough(token);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPassThrough)(GLfloat token) = _choose_glPassThrough;
static void API_ENTRY _choose_glPatchParameterfv(GLenum pname,const GLfloat* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPatchParameterfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPatchParameterfv is NULL " << std::endl;
	else
	{
		_ptr_to_glPatchParameterfv = (void(API_ENTRY*)(GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glPatchParameterfv(pname,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPatchParameterfv)(GLenum pname,const GLfloat* values) = _choose_glPatchParameterfv;
static void API_ENTRY _choose_glPatchParameteri(GLenum pname,GLint value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPatchParameteri");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPatchParameteri is NULL " << std::endl;
	else
	{
		_ptr_to_glPatchParameteri = (void(API_ENTRY*)(GLenum,GLint))gotPtr;
		return _ptr_to_glPatchParameteri(pname,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPatchParameteri)(GLenum pname,GLint value) = _choose_glPatchParameteri;
static void API_ENTRY _choose_glPauseTransformFeedback()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPauseTransformFeedback");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPauseTransformFeedback is NULL " << std::endl;
	else
	{
		_ptr_to_glPauseTransformFeedback = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glPauseTransformFeedback();
	}
	return;
}
void (API_ENTRY *_ptr_to_glPauseTransformFeedback)() = _choose_glPauseTransformFeedback;
static void API_ENTRY _choose_glPixelMapfv(GLenum map,GLsizei mapsize,const GLfloat* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPixelMapfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPixelMapfv is NULL " << std::endl;
	else
	{
		_ptr_to_glPixelMapfv = (void(API_ENTRY*)(GLenum,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glPixelMapfv(map,mapsize,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPixelMapfv)(GLenum map,GLsizei mapsize,const GLfloat* values) = _choose_glPixelMapfv;
static void API_ENTRY _choose_glPixelMapuiv(GLenum map,GLsizei mapsize,const GLuint* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPixelMapuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPixelMapuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glPixelMapuiv = (void(API_ENTRY*)(GLenum,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glPixelMapuiv(map,mapsize,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPixelMapuiv)(GLenum map,GLsizei mapsize,const GLuint* values) = _choose_glPixelMapuiv;
static void API_ENTRY _choose_glPixelMapusv(GLenum map,GLsizei mapsize,const GLushort* values)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPixelMapusv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPixelMapusv is NULL " << std::endl;
	else
	{
		_ptr_to_glPixelMapusv = (void(API_ENTRY*)(GLenum,GLsizei,const GLushort*))gotPtr;
		return _ptr_to_glPixelMapusv(map,mapsize,values);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPixelMapusv)(GLenum map,GLsizei mapsize,const GLushort* values) = _choose_glPixelMapusv;
static void API_ENTRY _choose_glPixelStoref(GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPixelStoref");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPixelStoref is NULL " << std::endl;
	else
	{
		_ptr_to_glPixelStoref = (void(API_ENTRY*)(GLenum,GLfloat))gotPtr;
		return _ptr_to_glPixelStoref(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPixelStoref)(GLenum pname,GLfloat param) = _choose_glPixelStoref;
static void API_ENTRY _choose_glPixelStorei(GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPixelStorei");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPixelStorei is NULL " << std::endl;
	else
	{
		_ptr_to_glPixelStorei = (void(API_ENTRY*)(GLenum,GLint))gotPtr;
		return _ptr_to_glPixelStorei(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPixelStorei)(GLenum pname,GLint param) = _choose_glPixelStorei;
static void API_ENTRY _choose_glPixelTransferf(GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPixelTransferf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPixelTransferf is NULL " << std::endl;
	else
	{
		_ptr_to_glPixelTransferf = (void(API_ENTRY*)(GLenum,GLfloat))gotPtr;
		return _ptr_to_glPixelTransferf(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPixelTransferf)(GLenum pname,GLfloat param) = _choose_glPixelTransferf;
static void API_ENTRY _choose_glPixelTransferi(GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPixelTransferi");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPixelTransferi is NULL " << std::endl;
	else
	{
		_ptr_to_glPixelTransferi = (void(API_ENTRY*)(GLenum,GLint))gotPtr;
		return _ptr_to_glPixelTransferi(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPixelTransferi)(GLenum pname,GLint param) = _choose_glPixelTransferi;
static void API_ENTRY _choose_glPixelZoom(GLfloat xfactor,GLfloat yfactor)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPixelZoom");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPixelZoom is NULL " << std::endl;
	else
	{
		_ptr_to_glPixelZoom = (void(API_ENTRY*)(GLfloat,GLfloat))gotPtr;
		return _ptr_to_glPixelZoom(xfactor,yfactor);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPixelZoom)(GLfloat xfactor,GLfloat yfactor) = _choose_glPixelZoom;
static void API_ENTRY _choose_glPointParameterf(GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPointParameterf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPointParameterf is NULL " << std::endl;
	else
	{
		_ptr_to_glPointParameterf = (void(API_ENTRY*)(GLenum,GLfloat))gotPtr;
		return _ptr_to_glPointParameterf(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPointParameterf)(GLenum pname,GLfloat param) = _choose_glPointParameterf;
static void API_ENTRY _choose_glPointParameterfv(GLenum pname,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPointParameterfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPointParameterfv is NULL " << std::endl;
	else
	{
		_ptr_to_glPointParameterfv = (void(API_ENTRY*)(GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glPointParameterfv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPointParameterfv)(GLenum pname,const GLfloat* params) = _choose_glPointParameterfv;
static void API_ENTRY _choose_glPointParameteri(GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPointParameteri");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPointParameteri is NULL " << std::endl;
	else
	{
		_ptr_to_glPointParameteri = (void(API_ENTRY*)(GLenum,GLint))gotPtr;
		return _ptr_to_glPointParameteri(pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPointParameteri)(GLenum pname,GLint param) = _choose_glPointParameteri;
static void API_ENTRY _choose_glPointParameteriv(GLenum pname,const GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPointParameteriv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPointParameteriv is NULL " << std::endl;
	else
	{
		_ptr_to_glPointParameteriv = (void(API_ENTRY*)(GLenum,const GLint*))gotPtr;
		return _ptr_to_glPointParameteriv(pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPointParameteriv)(GLenum pname,const GLint* params) = _choose_glPointParameteriv;
static void API_ENTRY _choose_glPointSize(GLfloat size)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPointSize");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPointSize is NULL " << std::endl;
	else
	{
		_ptr_to_glPointSize = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glPointSize(size);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPointSize)(GLfloat size) = _choose_glPointSize;
static void API_ENTRY _choose_glPolygonMode(GLenum face,GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPolygonMode");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPolygonMode is NULL " << std::endl;
	else
	{
		_ptr_to_glPolygonMode = (void(API_ENTRY*)(GLenum,GLenum))gotPtr;
		return _ptr_to_glPolygonMode(face,mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPolygonMode)(GLenum face,GLenum mode) = _choose_glPolygonMode;
static void API_ENTRY _choose_glPolygonOffset(GLfloat factor,GLfloat units)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPolygonOffset");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPolygonOffset is NULL " << std::endl;
	else
	{
		_ptr_to_glPolygonOffset = (void(API_ENTRY*)(GLfloat,GLfloat))gotPtr;
		return _ptr_to_glPolygonOffset(factor,units);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPolygonOffset)(GLfloat factor,GLfloat units) = _choose_glPolygonOffset;
static void API_ENTRY _choose_glPolygonStipple(const GLubyte* mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPolygonStipple");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPolygonStipple is NULL " << std::endl;
	else
	{
		_ptr_to_glPolygonStipple = (void(API_ENTRY*)(const GLubyte*))gotPtr;
		return _ptr_to_glPolygonStipple(mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPolygonStipple)(const GLubyte* mask) = _choose_glPolygonStipple;
static void API_ENTRY _choose_glPopAttrib()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPopAttrib");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPopAttrib is NULL " << std::endl;
	else
	{
		_ptr_to_glPopAttrib = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glPopAttrib();
	}
	return;
}
void (API_ENTRY *_ptr_to_glPopAttrib)() = _choose_glPopAttrib;
static void API_ENTRY _choose_glPopClientAttrib()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPopClientAttrib");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPopClientAttrib is NULL " << std::endl;
	else
	{
		_ptr_to_glPopClientAttrib = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glPopClientAttrib();
	}
	return;
}
void (API_ENTRY *_ptr_to_glPopClientAttrib)() = _choose_glPopClientAttrib;
static void API_ENTRY _choose_glPopMatrix()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPopMatrix");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPopMatrix is NULL " << std::endl;
	else
	{
		_ptr_to_glPopMatrix = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glPopMatrix();
	}
	return;
}
void (API_ENTRY *_ptr_to_glPopMatrix)() = _choose_glPopMatrix;
static void API_ENTRY _choose_glPopName()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPopName");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPopName is NULL " << std::endl;
	else
	{
		_ptr_to_glPopName = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glPopName();
	}
	return;
}
void (API_ENTRY *_ptr_to_glPopName)() = _choose_glPopName;
static void API_ENTRY _choose_glPrimitiveRestartIndex(GLuint index)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPrimitiveRestartIndex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPrimitiveRestartIndex is NULL " << std::endl;
	else
	{
		_ptr_to_glPrimitiveRestartIndex = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glPrimitiveRestartIndex(index);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPrimitiveRestartIndex)(GLuint index) = _choose_glPrimitiveRestartIndex;
static void API_ENTRY _choose_glPrioritizeTextures(GLsizei n,const GLuint* textures,const GLfloat* priorities)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPrioritizeTextures");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPrioritizeTextures is NULL " << std::endl;
	else
	{
		_ptr_to_glPrioritizeTextures = (void(API_ENTRY*)(GLsizei,const GLuint*,const GLfloat*))gotPtr;
		return _ptr_to_glPrioritizeTextures(n,textures,priorities);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPrioritizeTextures)(GLsizei n,const GLuint* textures,const GLfloat* priorities) = _choose_glPrioritizeTextures;
static void API_ENTRY _choose_glProgramBinary(GLuint program,GLenum binaryFormat,const GLvoid* binary,GLsizei length)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramBinary");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramBinary is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramBinary = (void(API_ENTRY*)(GLuint,GLenum,const GLvoid*,GLsizei))gotPtr;
		return _ptr_to_glProgramBinary(program,binaryFormat,binary,length);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramBinary)(GLuint program,GLenum binaryFormat,const GLvoid* binary,GLsizei length) = _choose_glProgramBinary;
static void API_ENTRY _choose_glProgramEnvParameter4dARB(GLenum target,GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramEnvParameter4dARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramEnvParameter4dARB is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramEnvParameter4dARB = (void(API_ENTRY*)(GLenum,GLuint,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glProgramEnvParameter4dARB(target,index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramEnvParameter4dARB)(GLenum target,GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w) = _choose_glProgramEnvParameter4dARB;
static void API_ENTRY _choose_glProgramEnvParameter4dvARB(GLenum target,GLuint index,const GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramEnvParameter4dvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramEnvParameter4dvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramEnvParameter4dvARB = (void(API_ENTRY*)(GLenum,GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glProgramEnvParameter4dvARB(target,index,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramEnvParameter4dvARB)(GLenum target,GLuint index,const GLdouble* params) = _choose_glProgramEnvParameter4dvARB;
static void API_ENTRY _choose_glProgramEnvParameter4fARB(GLenum target,GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramEnvParameter4fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramEnvParameter4fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramEnvParameter4fARB = (void(API_ENTRY*)(GLenum,GLuint,GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glProgramEnvParameter4fARB(target,index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramEnvParameter4fARB)(GLenum target,GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w) = _choose_glProgramEnvParameter4fARB;
static void API_ENTRY _choose_glProgramEnvParameter4fvARB(GLenum target,GLuint index,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramEnvParameter4fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramEnvParameter4fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramEnvParameter4fvARB = (void(API_ENTRY*)(GLenum,GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glProgramEnvParameter4fvARB(target,index,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramEnvParameter4fvARB)(GLenum target,GLuint index,const GLfloat* params) = _choose_glProgramEnvParameter4fvARB;
static void API_ENTRY _choose_glProgramLocalParameter4dARB(GLenum target,GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramLocalParameter4dARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramLocalParameter4dARB is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramLocalParameter4dARB = (void(API_ENTRY*)(GLenum,GLuint,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glProgramLocalParameter4dARB(target,index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramLocalParameter4dARB)(GLenum target,GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w) = _choose_glProgramLocalParameter4dARB;
static void API_ENTRY _choose_glProgramLocalParameter4dvARB(GLenum target,GLuint index,const GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramLocalParameter4dvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramLocalParameter4dvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramLocalParameter4dvARB = (void(API_ENTRY*)(GLenum,GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glProgramLocalParameter4dvARB(target,index,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramLocalParameter4dvARB)(GLenum target,GLuint index,const GLdouble* params) = _choose_glProgramLocalParameter4dvARB;
static void API_ENTRY _choose_glProgramLocalParameter4fARB(GLenum target,GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramLocalParameter4fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramLocalParameter4fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramLocalParameter4fARB = (void(API_ENTRY*)(GLenum,GLuint,GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glProgramLocalParameter4fARB(target,index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramLocalParameter4fARB)(GLenum target,GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w) = _choose_glProgramLocalParameter4fARB;
static void API_ENTRY _choose_glProgramLocalParameter4fvARB(GLenum target,GLuint index,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramLocalParameter4fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramLocalParameter4fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramLocalParameter4fvARB = (void(API_ENTRY*)(GLenum,GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glProgramLocalParameter4fvARB(target,index,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramLocalParameter4fvARB)(GLenum target,GLuint index,const GLfloat* params) = _choose_glProgramLocalParameter4fvARB;
static void API_ENTRY _choose_glProgramParameteri(GLuint program,GLenum pname,GLint value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramParameteri");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramParameteri is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramParameteri = (void(API_ENTRY*)(GLuint,GLenum,GLint))gotPtr;
		return _ptr_to_glProgramParameteri(program,pname,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramParameteri)(GLuint program,GLenum pname,GLint value) = _choose_glProgramParameteri;
static void API_ENTRY _choose_glProgramStringARB(GLenum target,GLenum format,GLsizei len,const GLvoid* string)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramStringARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramStringARB is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramStringARB = (void(API_ENTRY*)(GLenum,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glProgramStringARB(target,format,len,string);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramStringARB)(GLenum target,GLenum format,GLsizei len,const GLvoid* string) = _choose_glProgramStringARB;
static void API_ENTRY _choose_glProgramUniform1d(GLuint program,GLint location,GLdouble v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform1d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform1d is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform1d = (void(API_ENTRY*)(GLuint,GLint,GLdouble))gotPtr;
		return _ptr_to_glProgramUniform1d(program,location,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform1d)(GLuint program,GLint location,GLdouble v0) = _choose_glProgramUniform1d;
static void API_ENTRY _choose_glProgramUniform1dv(GLuint program,GLint location,GLsizei count,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform1dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform1dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform1dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniform1dv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform1dv)(GLuint program,GLint location,GLsizei count,const GLdouble* value) = _choose_glProgramUniform1dv;
static void API_ENTRY _choose_glProgramUniform1f(GLuint program,GLint location,GLfloat v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform1f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform1f is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform1f = (void(API_ENTRY*)(GLuint,GLint,GLfloat))gotPtr;
		return _ptr_to_glProgramUniform1f(program,location,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform1f)(GLuint program,GLint location,GLfloat v0) = _choose_glProgramUniform1f;
static void API_ENTRY _choose_glProgramUniform1fv(GLuint program,GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform1fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform1fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform1fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniform1fv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform1fv)(GLuint program,GLint location,GLsizei count,const GLfloat* value) = _choose_glProgramUniform1fv;
static void API_ENTRY _choose_glProgramUniform1i(GLuint program,GLint location,GLint v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform1i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform1i is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform1i = (void(API_ENTRY*)(GLuint,GLint,GLint))gotPtr;
		return _ptr_to_glProgramUniform1i(program,location,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform1i)(GLuint program,GLint location,GLint v0) = _choose_glProgramUniform1i;
static void API_ENTRY _choose_glProgramUniform1iv(GLuint program,GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform1iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform1iv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform1iv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glProgramUniform1iv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform1iv)(GLuint program,GLint location,GLsizei count,const GLint* value) = _choose_glProgramUniform1iv;
static void API_ENTRY _choose_glProgramUniform1ui(GLuint program,GLint location,GLuint v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform1ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform1ui is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform1ui = (void(API_ENTRY*)(GLuint,GLint,GLuint))gotPtr;
		return _ptr_to_glProgramUniform1ui(program,location,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform1ui)(GLuint program,GLint location,GLuint v0) = _choose_glProgramUniform1ui;
static void API_ENTRY _choose_glProgramUniform1uiv(GLuint program,GLint location,GLsizei count,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform1uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform1uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform1uiv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glProgramUniform1uiv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform1uiv)(GLuint program,GLint location,GLsizei count,const GLuint* value) = _choose_glProgramUniform1uiv;
static void API_ENTRY _choose_glProgramUniform2d(GLuint program,GLint location,GLdouble v0,GLdouble v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform2d is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform2d = (void(API_ENTRY*)(GLuint,GLint,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glProgramUniform2d(program,location,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform2d)(GLuint program,GLint location,GLdouble v0,GLdouble v1) = _choose_glProgramUniform2d;
static void API_ENTRY _choose_glProgramUniform2dv(GLuint program,GLint location,GLsizei count,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform2dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniform2dv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform2dv)(GLuint program,GLint location,GLsizei count,const GLdouble* value) = _choose_glProgramUniform2dv;
static void API_ENTRY _choose_glProgramUniform2f(GLuint program,GLint location,GLfloat v0,GLfloat v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform2f is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform2f = (void(API_ENTRY*)(GLuint,GLint,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glProgramUniform2f(program,location,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform2f)(GLuint program,GLint location,GLfloat v0,GLfloat v1) = _choose_glProgramUniform2f;
static void API_ENTRY _choose_glProgramUniform2fv(GLuint program,GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform2fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniform2fv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform2fv)(GLuint program,GLint location,GLsizei count,const GLfloat* value) = _choose_glProgramUniform2fv;
static void API_ENTRY _choose_glProgramUniform2i(GLuint program,GLint location,GLint v0,GLint v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform2i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform2i is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform2i = (void(API_ENTRY*)(GLuint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glProgramUniform2i(program,location,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform2i)(GLuint program,GLint location,GLint v0,GLint v1) = _choose_glProgramUniform2i;
static void API_ENTRY _choose_glProgramUniform2iv(GLuint program,GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform2iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform2iv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform2iv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glProgramUniform2iv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform2iv)(GLuint program,GLint location,GLsizei count,const GLint* value) = _choose_glProgramUniform2iv;
static void API_ENTRY _choose_glProgramUniform2ui(GLuint program,GLint location,GLuint v0,GLuint v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform2ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform2ui is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform2ui = (void(API_ENTRY*)(GLuint,GLint,GLuint,GLuint))gotPtr;
		return _ptr_to_glProgramUniform2ui(program,location,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform2ui)(GLuint program,GLint location,GLuint v0,GLuint v1) = _choose_glProgramUniform2ui;
static void API_ENTRY _choose_glProgramUniform2uiv(GLuint program,GLint location,GLsizei count,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform2uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform2uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform2uiv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glProgramUniform2uiv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform2uiv)(GLuint program,GLint location,GLsizei count,const GLuint* value) = _choose_glProgramUniform2uiv;
static void API_ENTRY _choose_glProgramUniform3d(GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform3d is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform3d = (void(API_ENTRY*)(GLuint,GLint,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glProgramUniform3d(program,location,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform3d)(GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2) = _choose_glProgramUniform3d;
static void API_ENTRY _choose_glProgramUniform3dv(GLuint program,GLint location,GLsizei count,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform3dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniform3dv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform3dv)(GLuint program,GLint location,GLsizei count,const GLdouble* value) = _choose_glProgramUniform3dv;
static void API_ENTRY _choose_glProgramUniform3f(GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform3f is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform3f = (void(API_ENTRY*)(GLuint,GLint,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glProgramUniform3f(program,location,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform3f)(GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2) = _choose_glProgramUniform3f;
static void API_ENTRY _choose_glProgramUniform3fv(GLuint program,GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform3fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniform3fv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform3fv)(GLuint program,GLint location,GLsizei count,const GLfloat* value) = _choose_glProgramUniform3fv;
static void API_ENTRY _choose_glProgramUniform3i(GLuint program,GLint location,GLint v0,GLint v1,GLint v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform3i is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform3i = (void(API_ENTRY*)(GLuint,GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glProgramUniform3i(program,location,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform3i)(GLuint program,GLint location,GLint v0,GLint v1,GLint v2) = _choose_glProgramUniform3i;
static void API_ENTRY _choose_glProgramUniform3iv(GLuint program,GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform3iv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glProgramUniform3iv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform3iv)(GLuint program,GLint location,GLsizei count,const GLint* value) = _choose_glProgramUniform3iv;
static void API_ENTRY _choose_glProgramUniform3ui(GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform3ui = (void(API_ENTRY*)(GLuint,GLint,GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glProgramUniform3ui(program,location,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform3ui)(GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2) = _choose_glProgramUniform3ui;
static void API_ENTRY _choose_glProgramUniform3uiv(GLuint program,GLint location,GLsizei count,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform3uiv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glProgramUniform3uiv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform3uiv)(GLuint program,GLint location,GLsizei count,const GLuint* value) = _choose_glProgramUniform3uiv;
static void API_ENTRY _choose_glProgramUniform4d(GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2,GLdouble v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform4d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform4d is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform4d = (void(API_ENTRY*)(GLuint,GLint,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glProgramUniform4d(program,location,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform4d)(GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2,GLdouble v3) = _choose_glProgramUniform4d;
static void API_ENTRY _choose_glProgramUniform4dv(GLuint program,GLint location,GLsizei count,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform4dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniform4dv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform4dv)(GLuint program,GLint location,GLsizei count,const GLdouble* value) = _choose_glProgramUniform4dv;
static void API_ENTRY _choose_glProgramUniform4f(GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform4f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform4f is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform4f = (void(API_ENTRY*)(GLuint,GLint,GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glProgramUniform4f(program,location,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform4f)(GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3) = _choose_glProgramUniform4f;
static void API_ENTRY _choose_glProgramUniform4fv(GLuint program,GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform4fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniform4fv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform4fv)(GLuint program,GLint location,GLsizei count,const GLfloat* value) = _choose_glProgramUniform4fv;
static void API_ENTRY _choose_glProgramUniform4i(GLuint program,GLint location,GLint v0,GLint v1,GLint v2,GLint v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform4i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform4i is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform4i = (void(API_ENTRY*)(GLuint,GLint,GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glProgramUniform4i(program,location,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform4i)(GLuint program,GLint location,GLint v0,GLint v1,GLint v2,GLint v3) = _choose_glProgramUniform4i;
static void API_ENTRY _choose_glProgramUniform4iv(GLuint program,GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform4iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform4iv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform4iv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glProgramUniform4iv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform4iv)(GLuint program,GLint location,GLsizei count,const GLint* value) = _choose_glProgramUniform4iv;
static void API_ENTRY _choose_glProgramUniform4ui(GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform4ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform4ui is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform4ui = (void(API_ENTRY*)(GLuint,GLint,GLuint,GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glProgramUniform4ui(program,location,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform4ui)(GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3) = _choose_glProgramUniform4ui;
static void API_ENTRY _choose_glProgramUniform4uiv(GLuint program,GLint location,GLsizei count,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniform4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniform4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniform4uiv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glProgramUniform4uiv(program,location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniform4uiv)(GLuint program,GLint location,GLsizei count,const GLuint* value) = _choose_glProgramUniform4uiv;
static void API_ENTRY _choose_glProgramUniformMatrix2dv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix2dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniformMatrix2dv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix2dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glProgramUniformMatrix2dv;
static void API_ENTRY _choose_glProgramUniformMatrix2fv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix2fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniformMatrix2fv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix2fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glProgramUniformMatrix2fv;
static void API_ENTRY _choose_glProgramUniformMatrix2x3dv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix2x3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix2x3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix2x3dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniformMatrix2x3dv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix2x3dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glProgramUniformMatrix2x3dv;
static void API_ENTRY _choose_glProgramUniformMatrix2x3fv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix2x3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix2x3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix2x3fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniformMatrix2x3fv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix2x3fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glProgramUniformMatrix2x3fv;
static void API_ENTRY _choose_glProgramUniformMatrix2x4dv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix2x4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix2x4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix2x4dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniformMatrix2x4dv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix2x4dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glProgramUniformMatrix2x4dv;
static void API_ENTRY _choose_glProgramUniformMatrix2x4fv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix2x4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix2x4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix2x4fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniformMatrix2x4fv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix2x4fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glProgramUniformMatrix2x4fv;
static void API_ENTRY _choose_glProgramUniformMatrix3dv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix3dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniformMatrix3dv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix3dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glProgramUniformMatrix3dv;
static void API_ENTRY _choose_glProgramUniformMatrix3fv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix3fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniformMatrix3fv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix3fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glProgramUniformMatrix3fv;
static void API_ENTRY _choose_glProgramUniformMatrix3x2dv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix3x2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix3x2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix3x2dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniformMatrix3x2dv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix3x2dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glProgramUniformMatrix3x2dv;
static void API_ENTRY _choose_glProgramUniformMatrix3x2fv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix3x2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix3x2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix3x2fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniformMatrix3x2fv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix3x2fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glProgramUniformMatrix3x2fv;
static void API_ENTRY _choose_glProgramUniformMatrix3x4dv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix3x4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix3x4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix3x4dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniformMatrix3x4dv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix3x4dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glProgramUniformMatrix3x4dv;
static void API_ENTRY _choose_glProgramUniformMatrix3x4fv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix3x4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix3x4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix3x4fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniformMatrix3x4fv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix3x4fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glProgramUniformMatrix3x4fv;
static void API_ENTRY _choose_glProgramUniformMatrix4dv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix4dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniformMatrix4dv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix4dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glProgramUniformMatrix4dv;
static void API_ENTRY _choose_glProgramUniformMatrix4fv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix4fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniformMatrix4fv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix4fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glProgramUniformMatrix4fv;
static void API_ENTRY _choose_glProgramUniformMatrix4x2dv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix4x2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix4x2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix4x2dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniformMatrix4x2dv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix4x2dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glProgramUniformMatrix4x2dv;
static void API_ENTRY _choose_glProgramUniformMatrix4x2fv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix4x2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix4x2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix4x2fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniformMatrix4x2fv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix4x2fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glProgramUniformMatrix4x2fv;
static void API_ENTRY _choose_glProgramUniformMatrix4x3dv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix4x3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix4x3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix4x3dv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glProgramUniformMatrix4x3dv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix4x3dv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glProgramUniformMatrix4x3dv;
static void API_ENTRY _choose_glProgramUniformMatrix4x3fv(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProgramUniformMatrix4x3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProgramUniformMatrix4x3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glProgramUniformMatrix4x3fv = (void(API_ENTRY*)(GLuint,GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glProgramUniformMatrix4x3fv(program,location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProgramUniformMatrix4x3fv)(GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glProgramUniformMatrix4x3fv;
static void API_ENTRY _choose_glProvokingVertex(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glProvokingVertex");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glProvokingVertex is NULL " << std::endl;
	else
	{
		_ptr_to_glProvokingVertex = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glProvokingVertex(mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glProvokingVertex)(GLenum mode) = _choose_glProvokingVertex;
static void API_ENTRY _choose_glPushAttrib(GLbitfield mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPushAttrib");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPushAttrib is NULL " << std::endl;
	else
	{
		_ptr_to_glPushAttrib = (void(API_ENTRY*)(GLbitfield))gotPtr;
		return _ptr_to_glPushAttrib(mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPushAttrib)(GLbitfield mask) = _choose_glPushAttrib;
static void API_ENTRY _choose_glPushClientAttrib(GLbitfield mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPushClientAttrib");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPushClientAttrib is NULL " << std::endl;
	else
	{
		_ptr_to_glPushClientAttrib = (void(API_ENTRY*)(GLbitfield))gotPtr;
		return _ptr_to_glPushClientAttrib(mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPushClientAttrib)(GLbitfield mask) = _choose_glPushClientAttrib;
static void API_ENTRY _choose_glPushMatrix()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPushMatrix");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPushMatrix is NULL " << std::endl;
	else
	{
		_ptr_to_glPushMatrix = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glPushMatrix();
	}
	return;
}
void (API_ENTRY *_ptr_to_glPushMatrix)() = _choose_glPushMatrix;
static void API_ENTRY _choose_glPushName(GLuint name)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glPushName");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glPushName is NULL " << std::endl;
	else
	{
		_ptr_to_glPushName = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glPushName(name);
	}
	return;
}
void (API_ENTRY *_ptr_to_glPushName)(GLuint name) = _choose_glPushName;
static void API_ENTRY _choose_glQueryCounter(GLuint id,GLenum target)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glQueryCounter");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glQueryCounter is NULL " << std::endl;
	else
	{
		_ptr_to_glQueryCounter = (void(API_ENTRY*)(GLuint,GLenum))gotPtr;
		return _ptr_to_glQueryCounter(id,target);
	}
	return;
}
void (API_ENTRY *_ptr_to_glQueryCounter)(GLuint id,GLenum target) = _choose_glQueryCounter;
static void API_ENTRY _choose_glRasterPos2d(GLdouble x,GLdouble y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos2d is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos2d = (void(API_ENTRY*)(GLdouble,GLdouble))gotPtr;
		return _ptr_to_glRasterPos2d(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos2d)(GLdouble x,GLdouble y) = _choose_glRasterPos2d;
static void API_ENTRY _choose_glRasterPos2dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos2dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glRasterPos2dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos2dv)(const GLdouble* v) = _choose_glRasterPos2dv;
static void API_ENTRY _choose_glRasterPos2f(GLfloat x,GLfloat y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos2f is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos2f = (void(API_ENTRY*)(GLfloat,GLfloat))gotPtr;
		return _ptr_to_glRasterPos2f(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos2f)(GLfloat x,GLfloat y) = _choose_glRasterPos2f;
static void API_ENTRY _choose_glRasterPos2fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos2fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glRasterPos2fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos2fv)(const GLfloat* v) = _choose_glRasterPos2fv;
static void API_ENTRY _choose_glRasterPos2i(GLint x,GLint y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos2i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos2i is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos2i = (void(API_ENTRY*)(GLint,GLint))gotPtr;
		return _ptr_to_glRasterPos2i(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos2i)(GLint x,GLint y) = _choose_glRasterPos2i;
static void API_ENTRY _choose_glRasterPos2iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos2iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos2iv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos2iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glRasterPos2iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos2iv)(const GLint* v) = _choose_glRasterPos2iv;
static void API_ENTRY _choose_glRasterPos2s(GLshort x,GLshort y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos2s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos2s is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos2s = (void(API_ENTRY*)(GLshort,GLshort))gotPtr;
		return _ptr_to_glRasterPos2s(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos2s)(GLshort x,GLshort y) = _choose_glRasterPos2s;
static void API_ENTRY _choose_glRasterPos2sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos2sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos2sv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos2sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glRasterPos2sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos2sv)(const GLshort* v) = _choose_glRasterPos2sv;
static void API_ENTRY _choose_glRasterPos3d(GLdouble x,GLdouble y,GLdouble z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos3d is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos3d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glRasterPos3d(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos3d)(GLdouble x,GLdouble y,GLdouble z) = _choose_glRasterPos3d;
static void API_ENTRY _choose_glRasterPos3dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos3dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glRasterPos3dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos3dv)(const GLdouble* v) = _choose_glRasterPos3dv;
static void API_ENTRY _choose_glRasterPos3f(GLfloat x,GLfloat y,GLfloat z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos3f is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos3f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glRasterPos3f(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos3f)(GLfloat x,GLfloat y,GLfloat z) = _choose_glRasterPos3f;
static void API_ENTRY _choose_glRasterPos3fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos3fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glRasterPos3fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos3fv)(const GLfloat* v) = _choose_glRasterPos3fv;
static void API_ENTRY _choose_glRasterPos3i(GLint x,GLint y,GLint z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos3i is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos3i = (void(API_ENTRY*)(GLint,GLint,GLint))gotPtr;
		return _ptr_to_glRasterPos3i(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos3i)(GLint x,GLint y,GLint z) = _choose_glRasterPos3i;
static void API_ENTRY _choose_glRasterPos3iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos3iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glRasterPos3iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos3iv)(const GLint* v) = _choose_glRasterPos3iv;
static void API_ENTRY _choose_glRasterPos3s(GLshort x,GLshort y,GLshort z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos3s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos3s is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos3s = (void(API_ENTRY*)(GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glRasterPos3s(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos3s)(GLshort x,GLshort y,GLshort z) = _choose_glRasterPos3s;
static void API_ENTRY _choose_glRasterPos3sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos3sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos3sv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos3sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glRasterPos3sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos3sv)(const GLshort* v) = _choose_glRasterPos3sv;
static void API_ENTRY _choose_glRasterPos4d(GLdouble x,GLdouble y,GLdouble z,GLdouble w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos4d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos4d is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos4d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glRasterPos4d(x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos4d)(GLdouble x,GLdouble y,GLdouble z,GLdouble w) = _choose_glRasterPos4d;
static void API_ENTRY _choose_glRasterPos4dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos4dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glRasterPos4dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos4dv)(const GLdouble* v) = _choose_glRasterPos4dv;
static void API_ENTRY _choose_glRasterPos4f(GLfloat x,GLfloat y,GLfloat z,GLfloat w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos4f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos4f is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos4f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glRasterPos4f(x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos4f)(GLfloat x,GLfloat y,GLfloat z,GLfloat w) = _choose_glRasterPos4f;
static void API_ENTRY _choose_glRasterPos4fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos4fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glRasterPos4fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos4fv)(const GLfloat* v) = _choose_glRasterPos4fv;
static void API_ENTRY _choose_glRasterPos4i(GLint x,GLint y,GLint z,GLint w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos4i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos4i is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos4i = (void(API_ENTRY*)(GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glRasterPos4i(x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos4i)(GLint x,GLint y,GLint z,GLint w) = _choose_glRasterPos4i;
static void API_ENTRY _choose_glRasterPos4iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos4iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos4iv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos4iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glRasterPos4iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos4iv)(const GLint* v) = _choose_glRasterPos4iv;
static void API_ENTRY _choose_glRasterPos4s(GLshort x,GLshort y,GLshort z,GLshort w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos4s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos4s is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos4s = (void(API_ENTRY*)(GLshort,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glRasterPos4s(x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos4s)(GLshort x,GLshort y,GLshort z,GLshort w) = _choose_glRasterPos4s;
static void API_ENTRY _choose_glRasterPos4sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRasterPos4sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRasterPos4sv is NULL " << std::endl;
	else
	{
		_ptr_to_glRasterPos4sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glRasterPos4sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRasterPos4sv)(const GLshort* v) = _choose_glRasterPos4sv;
static void API_ENTRY _choose_glReadBuffer(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glReadBuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glReadBuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glReadBuffer = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glReadBuffer(mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glReadBuffer)(GLenum mode) = _choose_glReadBuffer;
static void API_ENTRY _choose_glReadPixels(GLint x,GLint y,GLsizei width,GLsizei height,GLenum format,GLenum type,GLvoid* pixels)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glReadPixels");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glReadPixels is NULL " << std::endl;
	else
	{
		_ptr_to_glReadPixels = (void(API_ENTRY*)(GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,GLvoid*))gotPtr;
		return _ptr_to_glReadPixels(x,y,width,height,format,type,pixels);
	}
	return;
}
void (API_ENTRY *_ptr_to_glReadPixels)(GLint x,GLint y,GLsizei width,GLsizei height,GLenum format,GLenum type,GLvoid* pixels) = _choose_glReadPixels;
static void API_ENTRY _choose_glRectd(GLdouble x1,GLdouble y1,GLdouble x2,GLdouble y2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRectd");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRectd is NULL " << std::endl;
	else
	{
		_ptr_to_glRectd = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glRectd(x1,y1,x2,y2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRectd)(GLdouble x1,GLdouble y1,GLdouble x2,GLdouble y2) = _choose_glRectd;
static void API_ENTRY _choose_glRectdv(const GLdouble* v1,const GLdouble* v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRectdv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRectdv is NULL " << std::endl;
	else
	{
		_ptr_to_glRectdv = (void(API_ENTRY*)(const GLdouble*,const GLdouble*))gotPtr;
		return _ptr_to_glRectdv(v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRectdv)(const GLdouble* v1,const GLdouble* v2) = _choose_glRectdv;
static void API_ENTRY _choose_glRectf(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRectf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRectf is NULL " << std::endl;
	else
	{
		_ptr_to_glRectf = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glRectf(x1,y1,x2,y2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRectf)(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2) = _choose_glRectf;
static void API_ENTRY _choose_glRectfv(const GLfloat* v1,const GLfloat* v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRectfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRectfv is NULL " << std::endl;
	else
	{
		_ptr_to_glRectfv = (void(API_ENTRY*)(const GLfloat*,const GLfloat*))gotPtr;
		return _ptr_to_glRectfv(v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRectfv)(const GLfloat* v1,const GLfloat* v2) = _choose_glRectfv;
static void API_ENTRY _choose_glRecti(GLint x1,GLint y1,GLint x2,GLint y2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRecti");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRecti is NULL " << std::endl;
	else
	{
		_ptr_to_glRecti = (void(API_ENTRY*)(GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glRecti(x1,y1,x2,y2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRecti)(GLint x1,GLint y1,GLint x2,GLint y2) = _choose_glRecti;
static void API_ENTRY _choose_glRectiv(const GLint* v1,const GLint* v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRectiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRectiv is NULL " << std::endl;
	else
	{
		_ptr_to_glRectiv = (void(API_ENTRY*)(const GLint*,const GLint*))gotPtr;
		return _ptr_to_glRectiv(v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRectiv)(const GLint* v1,const GLint* v2) = _choose_glRectiv;
static void API_ENTRY _choose_glRects(GLshort x1,GLshort y1,GLshort x2,GLshort y2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRects");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRects is NULL " << std::endl;
	else
	{
		_ptr_to_glRects = (void(API_ENTRY*)(GLshort,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glRects(x1,y1,x2,y2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRects)(GLshort x1,GLshort y1,GLshort x2,GLshort y2) = _choose_glRects;
static void API_ENTRY _choose_glRectsv(const GLshort* v1,const GLshort* v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRectsv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRectsv is NULL " << std::endl;
	else
	{
		_ptr_to_glRectsv = (void(API_ENTRY*)(const GLshort*,const GLshort*))gotPtr;
		return _ptr_to_glRectsv(v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRectsv)(const GLshort* v1,const GLshort* v2) = _choose_glRectsv;
static void API_ENTRY _choose_glReleaseShaderCompiler()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glReleaseShaderCompiler");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glReleaseShaderCompiler is NULL " << std::endl;
	else
	{
		_ptr_to_glReleaseShaderCompiler = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glReleaseShaderCompiler();
	}
	return;
}
void (API_ENTRY *_ptr_to_glReleaseShaderCompiler)() = _choose_glReleaseShaderCompiler;
static GLint API_ENTRY _choose_glRenderMode(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRenderMode");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRenderMode is NULL " << std::endl;
	else
	{
		_ptr_to_glRenderMode = (GLint(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glRenderMode(mode);
	}
	typedef GLint RET_TYPE;
	return RET_TYPE();
}
GLint (API_ENTRY *_ptr_to_glRenderMode)(GLenum mode) = _choose_glRenderMode;
static void API_ENTRY _choose_glRenderbufferStorage(GLenum target,GLenum internalformat,GLsizei width,GLsizei height)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRenderbufferStorage");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRenderbufferStorage is NULL " << std::endl;
	else
	{
		_ptr_to_glRenderbufferStorage = (void(API_ENTRY*)(GLenum,GLenum,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glRenderbufferStorage(target,internalformat,width,height);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRenderbufferStorage)(GLenum target,GLenum internalformat,GLsizei width,GLsizei height) = _choose_glRenderbufferStorage;
static void API_ENTRY _choose_glRenderbufferStorageEXT(GLenum target,GLenum internalformat,GLsizei width,GLsizei height)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRenderbufferStorageEXT");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRenderbufferStorageEXT is NULL " << std::endl;
	else
	{
		_ptr_to_glRenderbufferStorageEXT = (void(API_ENTRY*)(GLenum,GLenum,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glRenderbufferStorageEXT(target,internalformat,width,height);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRenderbufferStorageEXT)(GLenum target,GLenum internalformat,GLsizei width,GLsizei height) = _choose_glRenderbufferStorageEXT;
static void API_ENTRY _choose_glRenderbufferStorageMultisample(GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRenderbufferStorageMultisample");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRenderbufferStorageMultisample is NULL " << std::endl;
	else
	{
		_ptr_to_glRenderbufferStorageMultisample = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glRenderbufferStorageMultisample(target,samples,internalformat,width,height);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRenderbufferStorageMultisample)(GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height) = _choose_glRenderbufferStorageMultisample;
static void API_ENTRY _choose_glResumeTransformFeedback()
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glResumeTransformFeedback");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glResumeTransformFeedback is NULL " << std::endl;
	else
	{
		_ptr_to_glResumeTransformFeedback = (void(API_ENTRY*)())gotPtr;
		return _ptr_to_glResumeTransformFeedback();
	}
	return;
}
void (API_ENTRY *_ptr_to_glResumeTransformFeedback)() = _choose_glResumeTransformFeedback;
static void API_ENTRY _choose_glRotated(GLdouble angle,GLdouble x,GLdouble y,GLdouble z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRotated");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRotated is NULL " << std::endl;
	else
	{
		_ptr_to_glRotated = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glRotated(angle,x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRotated)(GLdouble angle,GLdouble x,GLdouble y,GLdouble z) = _choose_glRotated;
static void API_ENTRY _choose_glRotatef(GLfloat angle,GLfloat x,GLfloat y,GLfloat z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glRotatef");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glRotatef is NULL " << std::endl;
	else
	{
		_ptr_to_glRotatef = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glRotatef(angle,x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glRotatef)(GLfloat angle,GLfloat x,GLfloat y,GLfloat z) = _choose_glRotatef;
static void API_ENTRY _choose_glSampleCoverage(GLfloat value,GLboolean invert)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSampleCoverage");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSampleCoverage is NULL " << std::endl;
	else
	{
		_ptr_to_glSampleCoverage = (void(API_ENTRY*)(GLfloat,GLboolean))gotPtr;
		return _ptr_to_glSampleCoverage(value,invert);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSampleCoverage)(GLfloat value,GLboolean invert) = _choose_glSampleCoverage;
static void API_ENTRY _choose_glSampleMaski(GLuint index,GLbitfield mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSampleMaski");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSampleMaski is NULL " << std::endl;
	else
	{
		_ptr_to_glSampleMaski = (void(API_ENTRY*)(GLuint,GLbitfield))gotPtr;
		return _ptr_to_glSampleMaski(index,mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSampleMaski)(GLuint index,GLbitfield mask) = _choose_glSampleMaski;
static void API_ENTRY _choose_glSamplerParameterIiv(GLuint sampler,GLenum pname,const GLint* param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSamplerParameterIiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSamplerParameterIiv is NULL " << std::endl;
	else
	{
		_ptr_to_glSamplerParameterIiv = (void(API_ENTRY*)(GLuint,GLenum,const GLint*))gotPtr;
		return _ptr_to_glSamplerParameterIiv(sampler,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSamplerParameterIiv)(GLuint sampler,GLenum pname,const GLint* param) = _choose_glSamplerParameterIiv;
static void API_ENTRY _choose_glSamplerParameterIuiv(GLuint sampler,GLenum pname,const GLuint* param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSamplerParameterIuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSamplerParameterIuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glSamplerParameterIuiv = (void(API_ENTRY*)(GLuint,GLenum,const GLuint*))gotPtr;
		return _ptr_to_glSamplerParameterIuiv(sampler,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSamplerParameterIuiv)(GLuint sampler,GLenum pname,const GLuint* param) = _choose_glSamplerParameterIuiv;
static void API_ENTRY _choose_glSamplerParameterf(GLuint sampler,GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSamplerParameterf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSamplerParameterf is NULL " << std::endl;
	else
	{
		_ptr_to_glSamplerParameterf = (void(API_ENTRY*)(GLuint,GLenum,GLfloat))gotPtr;
		return _ptr_to_glSamplerParameterf(sampler,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSamplerParameterf)(GLuint sampler,GLenum pname,GLfloat param) = _choose_glSamplerParameterf;
static void API_ENTRY _choose_glSamplerParameterfv(GLuint sampler,GLenum pname,const GLfloat* param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSamplerParameterfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSamplerParameterfv is NULL " << std::endl;
	else
	{
		_ptr_to_glSamplerParameterfv = (void(API_ENTRY*)(GLuint,GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glSamplerParameterfv(sampler,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSamplerParameterfv)(GLuint sampler,GLenum pname,const GLfloat* param) = _choose_glSamplerParameterfv;
static void API_ENTRY _choose_glSamplerParameteri(GLuint sampler,GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSamplerParameteri");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSamplerParameteri is NULL " << std::endl;
	else
	{
		_ptr_to_glSamplerParameteri = (void(API_ENTRY*)(GLuint,GLenum,GLint))gotPtr;
		return _ptr_to_glSamplerParameteri(sampler,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSamplerParameteri)(GLuint sampler,GLenum pname,GLint param) = _choose_glSamplerParameteri;
static void API_ENTRY _choose_glSamplerParameteriv(GLuint sampler,GLenum pname,const GLint* param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSamplerParameteriv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSamplerParameteriv is NULL " << std::endl;
	else
	{
		_ptr_to_glSamplerParameteriv = (void(API_ENTRY*)(GLuint,GLenum,const GLint*))gotPtr;
		return _ptr_to_glSamplerParameteriv(sampler,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSamplerParameteriv)(GLuint sampler,GLenum pname,const GLint* param) = _choose_glSamplerParameteriv;
static void API_ENTRY _choose_glScaled(GLdouble x,GLdouble y,GLdouble z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glScaled");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glScaled is NULL " << std::endl;
	else
	{
		_ptr_to_glScaled = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glScaled(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glScaled)(GLdouble x,GLdouble y,GLdouble z) = _choose_glScaled;
static void API_ENTRY _choose_glScalef(GLfloat x,GLfloat y,GLfloat z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glScalef");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glScalef is NULL " << std::endl;
	else
	{
		_ptr_to_glScalef = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glScalef(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glScalef)(GLfloat x,GLfloat y,GLfloat z) = _choose_glScalef;
static void API_ENTRY _choose_glScissor(GLint x,GLint y,GLsizei width,GLsizei height)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glScissor");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glScissor is NULL " << std::endl;
	else
	{
		_ptr_to_glScissor = (void(API_ENTRY*)(GLint,GLint,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glScissor(x,y,width,height);
	}
	return;
}
void (API_ENTRY *_ptr_to_glScissor)(GLint x,GLint y,GLsizei width,GLsizei height) = _choose_glScissor;
static void API_ENTRY _choose_glScissorArrayv(GLuint first,GLsizei count,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glScissorArrayv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glScissorArrayv is NULL " << std::endl;
	else
	{
		_ptr_to_glScissorArrayv = (void(API_ENTRY*)(GLuint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glScissorArrayv(first,count,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glScissorArrayv)(GLuint first,GLsizei count,const GLint* v) = _choose_glScissorArrayv;
static void API_ENTRY _choose_glScissorIndexed(GLuint index,GLint left,GLint bottom,GLsizei width,GLsizei height)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glScissorIndexed");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glScissorIndexed is NULL " << std::endl;
	else
	{
		_ptr_to_glScissorIndexed = (void(API_ENTRY*)(GLuint,GLint,GLint,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glScissorIndexed(index,left,bottom,width,height);
	}
	return;
}
void (API_ENTRY *_ptr_to_glScissorIndexed)(GLuint index,GLint left,GLint bottom,GLsizei width,GLsizei height) = _choose_glScissorIndexed;
static void API_ENTRY _choose_glScissorIndexedv(GLuint index,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glScissorIndexedv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glScissorIndexedv is NULL " << std::endl;
	else
	{
		_ptr_to_glScissorIndexedv = (void(API_ENTRY*)(GLuint,const GLint*))gotPtr;
		return _ptr_to_glScissorIndexedv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glScissorIndexedv)(GLuint index,const GLint* v) = _choose_glScissorIndexedv;
static void API_ENTRY _choose_glSecondaryColor3b(GLbyte red,GLbyte green,GLbyte blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3b");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3b is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3b = (void(API_ENTRY*)(GLbyte,GLbyte,GLbyte))gotPtr;
		return _ptr_to_glSecondaryColor3b(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3b)(GLbyte red,GLbyte green,GLbyte blue) = _choose_glSecondaryColor3b;
static void API_ENTRY _choose_glSecondaryColor3bv(const GLbyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3bv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3bv is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3bv = (void(API_ENTRY*)(const GLbyte*))gotPtr;
		return _ptr_to_glSecondaryColor3bv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3bv)(const GLbyte* v) = _choose_glSecondaryColor3bv;
static void API_ENTRY _choose_glSecondaryColor3d(GLdouble red,GLdouble green,GLdouble blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3d is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glSecondaryColor3d(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3d)(GLdouble red,GLdouble green,GLdouble blue) = _choose_glSecondaryColor3d;
static void API_ENTRY _choose_glSecondaryColor3dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glSecondaryColor3dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3dv)(const GLdouble* v) = _choose_glSecondaryColor3dv;
static void API_ENTRY _choose_glSecondaryColor3f(GLfloat red,GLfloat green,GLfloat blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3f is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glSecondaryColor3f(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3f)(GLfloat red,GLfloat green,GLfloat blue) = _choose_glSecondaryColor3f;
static void API_ENTRY _choose_glSecondaryColor3fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glSecondaryColor3fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3fv)(const GLfloat* v) = _choose_glSecondaryColor3fv;
static void API_ENTRY _choose_glSecondaryColor3i(GLint red,GLint green,GLint blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3i is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3i = (void(API_ENTRY*)(GLint,GLint,GLint))gotPtr;
		return _ptr_to_glSecondaryColor3i(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3i)(GLint red,GLint green,GLint blue) = _choose_glSecondaryColor3i;
static void API_ENTRY _choose_glSecondaryColor3iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glSecondaryColor3iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3iv)(const GLint* v) = _choose_glSecondaryColor3iv;
static void API_ENTRY _choose_glSecondaryColor3s(GLshort red,GLshort green,GLshort blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3s is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3s = (void(API_ENTRY*)(GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glSecondaryColor3s(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3s)(GLshort red,GLshort green,GLshort blue) = _choose_glSecondaryColor3s;
static void API_ENTRY _choose_glSecondaryColor3sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3sv is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glSecondaryColor3sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3sv)(const GLshort* v) = _choose_glSecondaryColor3sv;
static void API_ENTRY _choose_glSecondaryColor3ub(GLubyte red,GLubyte green,GLubyte blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3ub");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3ub is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3ub = (void(API_ENTRY*)(GLubyte,GLubyte,GLubyte))gotPtr;
		return _ptr_to_glSecondaryColor3ub(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3ub)(GLubyte red,GLubyte green,GLubyte blue) = _choose_glSecondaryColor3ub;
static void API_ENTRY _choose_glSecondaryColor3ubv(const GLubyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3ubv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3ubv is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3ubv = (void(API_ENTRY*)(const GLubyte*))gotPtr;
		return _ptr_to_glSecondaryColor3ubv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3ubv)(const GLubyte* v) = _choose_glSecondaryColor3ubv;
static void API_ENTRY _choose_glSecondaryColor3ui(GLuint red,GLuint green,GLuint blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3ui = (void(API_ENTRY*)(GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glSecondaryColor3ui(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3ui)(GLuint red,GLuint green,GLuint blue) = _choose_glSecondaryColor3ui;
static void API_ENTRY _choose_glSecondaryColor3uiv(const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3uiv = (void(API_ENTRY*)(const GLuint*))gotPtr;
		return _ptr_to_glSecondaryColor3uiv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3uiv)(const GLuint* v) = _choose_glSecondaryColor3uiv;
static void API_ENTRY _choose_glSecondaryColor3us(GLushort red,GLushort green,GLushort blue)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3us");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3us is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3us = (void(API_ENTRY*)(GLushort,GLushort,GLushort))gotPtr;
		return _ptr_to_glSecondaryColor3us(red,green,blue);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3us)(GLushort red,GLushort green,GLushort blue) = _choose_glSecondaryColor3us;
static void API_ENTRY _choose_glSecondaryColor3usv(const GLushort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColor3usv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColor3usv is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColor3usv = (void(API_ENTRY*)(const GLushort*))gotPtr;
		return _ptr_to_glSecondaryColor3usv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColor3usv)(const GLushort* v) = _choose_glSecondaryColor3usv;
static void API_ENTRY _choose_glSecondaryColorP3ui(GLenum type,GLuint color)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColorP3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColorP3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColorP3ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glSecondaryColorP3ui(type,color);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColorP3ui)(GLenum type,GLuint color) = _choose_glSecondaryColorP3ui;
static void API_ENTRY _choose_glSecondaryColorP3uiv(GLenum type,const GLuint* color)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColorP3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColorP3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColorP3uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glSecondaryColorP3uiv(type,color);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColorP3uiv)(GLenum type,const GLuint* color) = _choose_glSecondaryColorP3uiv;
static void API_ENTRY _choose_glSecondaryColorPointer(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSecondaryColorPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSecondaryColorPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glSecondaryColorPointer = (void(API_ENTRY*)(GLint,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glSecondaryColorPointer(size,type,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSecondaryColorPointer)(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer) = _choose_glSecondaryColorPointer;
static void API_ENTRY _choose_glSelectBuffer(GLsizei size,GLuint* buffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glSelectBuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glSelectBuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glSelectBuffer = (void(API_ENTRY*)(GLsizei,GLuint*))gotPtr;
		return _ptr_to_glSelectBuffer(size,buffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glSelectBuffer)(GLsizei size,GLuint* buffer) = _choose_glSelectBuffer;
static void API_ENTRY _choose_glShadeModel(GLenum mode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glShadeModel");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glShadeModel is NULL " << std::endl;
	else
	{
		_ptr_to_glShadeModel = (void(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glShadeModel(mode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glShadeModel)(GLenum mode) = _choose_glShadeModel;
static void API_ENTRY _choose_glShaderBinary(GLsizei count,const GLuint* shaders,GLenum binaryformat,const GLvoid* binary,GLsizei length)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glShaderBinary");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glShaderBinary is NULL " << std::endl;
	else
	{
		_ptr_to_glShaderBinary = (void(API_ENTRY*)(GLsizei,const GLuint*,GLenum,const GLvoid*,GLsizei))gotPtr;
		return _ptr_to_glShaderBinary(count,shaders,binaryformat,binary,length);
	}
	return;
}
void (API_ENTRY *_ptr_to_glShaderBinary)(GLsizei count,const GLuint* shaders,GLenum binaryformat,const GLvoid* binary,GLsizei length) = _choose_glShaderBinary;
static void API_ENTRY _choose_glShaderSource(GLuint shader,GLsizei count,const GLchar*const* string,const GLint* length)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glShaderSource");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glShaderSource is NULL " << std::endl;
	else
	{
		_ptr_to_glShaderSource = (void(API_ENTRY*)(GLuint,GLsizei,const GLchar*const*,const GLint*))gotPtr;
		return _ptr_to_glShaderSource(shader,count,string,length);
	}
	return;
}
void (API_ENTRY *_ptr_to_glShaderSource)(GLuint shader,GLsizei count,const GLchar*const* string,const GLint* length) = _choose_glShaderSource;
static void API_ENTRY _choose_glShaderSourceARB(GLhandleARB shaderObj,GLsizei count,const GLcharARB** string,const GLint* length)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glShaderSourceARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glShaderSourceARB is NULL " << std::endl;
	else
	{
		_ptr_to_glShaderSourceARB = (void(API_ENTRY*)(GLhandleARB,GLsizei,const GLcharARB**,const GLint*))gotPtr;
		return _ptr_to_glShaderSourceARB(shaderObj,count,string,length);
	}
	return;
}
void (API_ENTRY *_ptr_to_glShaderSourceARB)(GLhandleARB shaderObj,GLsizei count,const GLcharARB** string,const GLint* length) = _choose_glShaderSourceARB;
static void API_ENTRY _choose_glStencilFunc(GLenum func,GLint ref,GLuint mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glStencilFunc");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glStencilFunc is NULL " << std::endl;
	else
	{
		_ptr_to_glStencilFunc = (void(API_ENTRY*)(GLenum,GLint,GLuint))gotPtr;
		return _ptr_to_glStencilFunc(func,ref,mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glStencilFunc)(GLenum func,GLint ref,GLuint mask) = _choose_glStencilFunc;
static void API_ENTRY _choose_glStencilFuncSeparate(GLenum face,GLenum func,GLint ref,GLuint mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glStencilFuncSeparate");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glStencilFuncSeparate is NULL " << std::endl;
	else
	{
		_ptr_to_glStencilFuncSeparate = (void(API_ENTRY*)(GLenum,GLenum,GLint,GLuint))gotPtr;
		return _ptr_to_glStencilFuncSeparate(face,func,ref,mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glStencilFuncSeparate)(GLenum face,GLenum func,GLint ref,GLuint mask) = _choose_glStencilFuncSeparate;
static void API_ENTRY _choose_glStencilMask(GLuint mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glStencilMask");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glStencilMask is NULL " << std::endl;
	else
	{
		_ptr_to_glStencilMask = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glStencilMask(mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glStencilMask)(GLuint mask) = _choose_glStencilMask;
static void API_ENTRY _choose_glStencilMaskSeparate(GLenum face,GLuint mask)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glStencilMaskSeparate");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glStencilMaskSeparate is NULL " << std::endl;
	else
	{
		_ptr_to_glStencilMaskSeparate = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glStencilMaskSeparate(face,mask);
	}
	return;
}
void (API_ENTRY *_ptr_to_glStencilMaskSeparate)(GLenum face,GLuint mask) = _choose_glStencilMaskSeparate;
static void API_ENTRY _choose_glStencilOp(GLenum fail,GLenum zfail,GLenum zpass)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glStencilOp");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glStencilOp is NULL " << std::endl;
	else
	{
		_ptr_to_glStencilOp = (void(API_ENTRY*)(GLenum,GLenum,GLenum))gotPtr;
		return _ptr_to_glStencilOp(fail,zfail,zpass);
	}
	return;
}
void (API_ENTRY *_ptr_to_glStencilOp)(GLenum fail,GLenum zfail,GLenum zpass) = _choose_glStencilOp;
static void API_ENTRY _choose_glStencilOpSeparate(GLenum face,GLenum sfail,GLenum dpfail,GLenum dppass)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glStencilOpSeparate");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glStencilOpSeparate is NULL " << std::endl;
	else
	{
		_ptr_to_glStencilOpSeparate = (void(API_ENTRY*)(GLenum,GLenum,GLenum,GLenum))gotPtr;
		return _ptr_to_glStencilOpSeparate(face,sfail,dpfail,dppass);
	}
	return;
}
void (API_ENTRY *_ptr_to_glStencilOpSeparate)(GLenum face,GLenum sfail,GLenum dpfail,GLenum dppass) = _choose_glStencilOpSeparate;
static void API_ENTRY _choose_glTexBuffer(GLenum target,GLenum internalformat,GLuint buffer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexBuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexBuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glTexBuffer = (void(API_ENTRY*)(GLenum,GLenum,GLuint))gotPtr;
		return _ptr_to_glTexBuffer(target,internalformat,buffer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexBuffer)(GLenum target,GLenum internalformat,GLuint buffer) = _choose_glTexBuffer;
static void API_ENTRY _choose_glTexCoord1d(GLdouble s)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord1d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord1d is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord1d = (void(API_ENTRY*)(GLdouble))gotPtr;
		return _ptr_to_glTexCoord1d(s);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord1d)(GLdouble s) = _choose_glTexCoord1d;
static void API_ENTRY _choose_glTexCoord1dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord1dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord1dv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord1dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glTexCoord1dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord1dv)(const GLdouble* v) = _choose_glTexCoord1dv;
static void API_ENTRY _choose_glTexCoord1f(GLfloat s)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord1f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord1f is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord1f = (void(API_ENTRY*)(GLfloat))gotPtr;
		return _ptr_to_glTexCoord1f(s);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord1f)(GLfloat s) = _choose_glTexCoord1f;
static void API_ENTRY _choose_glTexCoord1fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord1fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord1fv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord1fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glTexCoord1fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord1fv)(const GLfloat* v) = _choose_glTexCoord1fv;
static void API_ENTRY _choose_glTexCoord1i(GLint s)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord1i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord1i is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord1i = (void(API_ENTRY*)(GLint))gotPtr;
		return _ptr_to_glTexCoord1i(s);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord1i)(GLint s) = _choose_glTexCoord1i;
static void API_ENTRY _choose_glTexCoord1iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord1iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord1iv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord1iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glTexCoord1iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord1iv)(const GLint* v) = _choose_glTexCoord1iv;
static void API_ENTRY _choose_glTexCoord1s(GLshort s)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord1s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord1s is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord1s = (void(API_ENTRY*)(GLshort))gotPtr;
		return _ptr_to_glTexCoord1s(s);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord1s)(GLshort s) = _choose_glTexCoord1s;
static void API_ENTRY _choose_glTexCoord1sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord1sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord1sv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord1sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glTexCoord1sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord1sv)(const GLshort* v) = _choose_glTexCoord1sv;
static void API_ENTRY _choose_glTexCoord2d(GLdouble s,GLdouble t)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord2d is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord2d = (void(API_ENTRY*)(GLdouble,GLdouble))gotPtr;
		return _ptr_to_glTexCoord2d(s,t);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord2d)(GLdouble s,GLdouble t) = _choose_glTexCoord2d;
static void API_ENTRY _choose_glTexCoord2dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord2dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glTexCoord2dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord2dv)(const GLdouble* v) = _choose_glTexCoord2dv;
static void API_ENTRY _choose_glTexCoord2f(GLfloat s,GLfloat t)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord2f is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord2f = (void(API_ENTRY*)(GLfloat,GLfloat))gotPtr;
		return _ptr_to_glTexCoord2f(s,t);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord2f)(GLfloat s,GLfloat t) = _choose_glTexCoord2f;
static void API_ENTRY _choose_glTexCoord2fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord2fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glTexCoord2fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord2fv)(const GLfloat* v) = _choose_glTexCoord2fv;
static void API_ENTRY _choose_glTexCoord2i(GLint s,GLint t)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord2i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord2i is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord2i = (void(API_ENTRY*)(GLint,GLint))gotPtr;
		return _ptr_to_glTexCoord2i(s,t);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord2i)(GLint s,GLint t) = _choose_glTexCoord2i;
static void API_ENTRY _choose_glTexCoord2iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord2iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord2iv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord2iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glTexCoord2iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord2iv)(const GLint* v) = _choose_glTexCoord2iv;
static void API_ENTRY _choose_glTexCoord2s(GLshort s,GLshort t)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord2s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord2s is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord2s = (void(API_ENTRY*)(GLshort,GLshort))gotPtr;
		return _ptr_to_glTexCoord2s(s,t);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord2s)(GLshort s,GLshort t) = _choose_glTexCoord2s;
static void API_ENTRY _choose_glTexCoord2sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord2sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord2sv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord2sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glTexCoord2sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord2sv)(const GLshort* v) = _choose_glTexCoord2sv;
static void API_ENTRY _choose_glTexCoord3d(GLdouble s,GLdouble t,GLdouble r)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord3d is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord3d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glTexCoord3d(s,t,r);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord3d)(GLdouble s,GLdouble t,GLdouble r) = _choose_glTexCoord3d;
static void API_ENTRY _choose_glTexCoord3dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord3dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glTexCoord3dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord3dv)(const GLdouble* v) = _choose_glTexCoord3dv;
static void API_ENTRY _choose_glTexCoord3f(GLfloat s,GLfloat t,GLfloat r)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord3f is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord3f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glTexCoord3f(s,t,r);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord3f)(GLfloat s,GLfloat t,GLfloat r) = _choose_glTexCoord3f;
static void API_ENTRY _choose_glTexCoord3fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord3fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glTexCoord3fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord3fv)(const GLfloat* v) = _choose_glTexCoord3fv;
static void API_ENTRY _choose_glTexCoord3i(GLint s,GLint t,GLint r)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord3i is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord3i = (void(API_ENTRY*)(GLint,GLint,GLint))gotPtr;
		return _ptr_to_glTexCoord3i(s,t,r);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord3i)(GLint s,GLint t,GLint r) = _choose_glTexCoord3i;
static void API_ENTRY _choose_glTexCoord3iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord3iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glTexCoord3iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord3iv)(const GLint* v) = _choose_glTexCoord3iv;
static void API_ENTRY _choose_glTexCoord3s(GLshort s,GLshort t,GLshort r)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord3s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord3s is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord3s = (void(API_ENTRY*)(GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glTexCoord3s(s,t,r);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord3s)(GLshort s,GLshort t,GLshort r) = _choose_glTexCoord3s;
static void API_ENTRY _choose_glTexCoord3sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord3sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord3sv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord3sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glTexCoord3sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord3sv)(const GLshort* v) = _choose_glTexCoord3sv;
static void API_ENTRY _choose_glTexCoord4d(GLdouble s,GLdouble t,GLdouble r,GLdouble q)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord4d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord4d is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord4d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glTexCoord4d(s,t,r,q);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord4d)(GLdouble s,GLdouble t,GLdouble r,GLdouble q) = _choose_glTexCoord4d;
static void API_ENTRY _choose_glTexCoord4dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord4dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glTexCoord4dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord4dv)(const GLdouble* v) = _choose_glTexCoord4dv;
static void API_ENTRY _choose_glTexCoord4f(GLfloat s,GLfloat t,GLfloat r,GLfloat q)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord4f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord4f is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord4f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glTexCoord4f(s,t,r,q);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord4f)(GLfloat s,GLfloat t,GLfloat r,GLfloat q) = _choose_glTexCoord4f;
static void API_ENTRY _choose_glTexCoord4fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord4fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glTexCoord4fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord4fv)(const GLfloat* v) = _choose_glTexCoord4fv;
static void API_ENTRY _choose_glTexCoord4i(GLint s,GLint t,GLint r,GLint q)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord4i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord4i is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord4i = (void(API_ENTRY*)(GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glTexCoord4i(s,t,r,q);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord4i)(GLint s,GLint t,GLint r,GLint q) = _choose_glTexCoord4i;
static void API_ENTRY _choose_glTexCoord4iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord4iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord4iv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord4iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glTexCoord4iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord4iv)(const GLint* v) = _choose_glTexCoord4iv;
static void API_ENTRY _choose_glTexCoord4s(GLshort s,GLshort t,GLshort r,GLshort q)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord4s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord4s is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord4s = (void(API_ENTRY*)(GLshort,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glTexCoord4s(s,t,r,q);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord4s)(GLshort s,GLshort t,GLshort r,GLshort q) = _choose_glTexCoord4s;
static void API_ENTRY _choose_glTexCoord4sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoord4sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoord4sv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoord4sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glTexCoord4sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoord4sv)(const GLshort* v) = _choose_glTexCoord4sv;
static void API_ENTRY _choose_glTexCoordP1ui(GLenum type,GLuint coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoordP1ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoordP1ui is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoordP1ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glTexCoordP1ui(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoordP1ui)(GLenum type,GLuint coords) = _choose_glTexCoordP1ui;
static void API_ENTRY _choose_glTexCoordP1uiv(GLenum type,const GLuint* coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoordP1uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoordP1uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoordP1uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glTexCoordP1uiv(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoordP1uiv)(GLenum type,const GLuint* coords) = _choose_glTexCoordP1uiv;
static void API_ENTRY _choose_glTexCoordP2ui(GLenum type,GLuint coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoordP2ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoordP2ui is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoordP2ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glTexCoordP2ui(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoordP2ui)(GLenum type,GLuint coords) = _choose_glTexCoordP2ui;
static void API_ENTRY _choose_glTexCoordP2uiv(GLenum type,const GLuint* coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoordP2uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoordP2uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoordP2uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glTexCoordP2uiv(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoordP2uiv)(GLenum type,const GLuint* coords) = _choose_glTexCoordP2uiv;
static void API_ENTRY _choose_glTexCoordP3ui(GLenum type,GLuint coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoordP3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoordP3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoordP3ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glTexCoordP3ui(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoordP3ui)(GLenum type,GLuint coords) = _choose_glTexCoordP3ui;
static void API_ENTRY _choose_glTexCoordP3uiv(GLenum type,const GLuint* coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoordP3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoordP3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoordP3uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glTexCoordP3uiv(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoordP3uiv)(GLenum type,const GLuint* coords) = _choose_glTexCoordP3uiv;
static void API_ENTRY _choose_glTexCoordP4ui(GLenum type,GLuint coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoordP4ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoordP4ui is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoordP4ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glTexCoordP4ui(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoordP4ui)(GLenum type,GLuint coords) = _choose_glTexCoordP4ui;
static void API_ENTRY _choose_glTexCoordP4uiv(GLenum type,const GLuint* coords)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoordP4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoordP4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoordP4uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glTexCoordP4uiv(type,coords);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoordP4uiv)(GLenum type,const GLuint* coords) = _choose_glTexCoordP4uiv;
static void API_ENTRY _choose_glTexCoordPointer(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexCoordPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexCoordPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glTexCoordPointer = (void(API_ENTRY*)(GLint,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glTexCoordPointer(size,type,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexCoordPointer)(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer) = _choose_glTexCoordPointer;
static void API_ENTRY _choose_glTexEnvf(GLenum target,GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexEnvf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexEnvf is NULL " << std::endl;
	else
	{
		_ptr_to_glTexEnvf = (void(API_ENTRY*)(GLenum,GLenum,GLfloat))gotPtr;
		return _ptr_to_glTexEnvf(target,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexEnvf)(GLenum target,GLenum pname,GLfloat param) = _choose_glTexEnvf;
static void API_ENTRY _choose_glTexEnvfv(GLenum target,GLenum pname,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexEnvfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexEnvfv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexEnvfv = (void(API_ENTRY*)(GLenum,GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glTexEnvfv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexEnvfv)(GLenum target,GLenum pname,const GLfloat* params) = _choose_glTexEnvfv;
static void API_ENTRY _choose_glTexEnvi(GLenum target,GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexEnvi");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexEnvi is NULL " << std::endl;
	else
	{
		_ptr_to_glTexEnvi = (void(API_ENTRY*)(GLenum,GLenum,GLint))gotPtr;
		return _ptr_to_glTexEnvi(target,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexEnvi)(GLenum target,GLenum pname,GLint param) = _choose_glTexEnvi;
static void API_ENTRY _choose_glTexEnviv(GLenum target,GLenum pname,const GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexEnviv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexEnviv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexEnviv = (void(API_ENTRY*)(GLenum,GLenum,const GLint*))gotPtr;
		return _ptr_to_glTexEnviv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexEnviv)(GLenum target,GLenum pname,const GLint* params) = _choose_glTexEnviv;
static void API_ENTRY _choose_glTexGend(GLenum coord,GLenum pname,GLdouble param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexGend");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexGend is NULL " << std::endl;
	else
	{
		_ptr_to_glTexGend = (void(API_ENTRY*)(GLenum,GLenum,GLdouble))gotPtr;
		return _ptr_to_glTexGend(coord,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexGend)(GLenum coord,GLenum pname,GLdouble param) = _choose_glTexGend;
static void API_ENTRY _choose_glTexGendv(GLenum coord,GLenum pname,const GLdouble* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexGendv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexGendv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexGendv = (void(API_ENTRY*)(GLenum,GLenum,const GLdouble*))gotPtr;
		return _ptr_to_glTexGendv(coord,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexGendv)(GLenum coord,GLenum pname,const GLdouble* params) = _choose_glTexGendv;
static void API_ENTRY _choose_glTexGenf(GLenum coord,GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexGenf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexGenf is NULL " << std::endl;
	else
	{
		_ptr_to_glTexGenf = (void(API_ENTRY*)(GLenum,GLenum,GLfloat))gotPtr;
		return _ptr_to_glTexGenf(coord,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexGenf)(GLenum coord,GLenum pname,GLfloat param) = _choose_glTexGenf;
static void API_ENTRY _choose_glTexGenfv(GLenum coord,GLenum pname,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexGenfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexGenfv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexGenfv = (void(API_ENTRY*)(GLenum,GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glTexGenfv(coord,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexGenfv)(GLenum coord,GLenum pname,const GLfloat* params) = _choose_glTexGenfv;
static void API_ENTRY _choose_glTexGeni(GLenum coord,GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexGeni");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexGeni is NULL " << std::endl;
	else
	{
		_ptr_to_glTexGeni = (void(API_ENTRY*)(GLenum,GLenum,GLint))gotPtr;
		return _ptr_to_glTexGeni(coord,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexGeni)(GLenum coord,GLenum pname,GLint param) = _choose_glTexGeni;
static void API_ENTRY _choose_glTexGeniv(GLenum coord,GLenum pname,const GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexGeniv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexGeniv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexGeniv = (void(API_ENTRY*)(GLenum,GLenum,const GLint*))gotPtr;
		return _ptr_to_glTexGeniv(coord,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexGeniv)(GLenum coord,GLenum pname,const GLint* params) = _choose_glTexGeniv;
static void API_ENTRY _choose_glTexImage1D(GLenum target,GLint level,GLint internalformat,GLsizei width,GLint border,GLenum format,GLenum type,const GLvoid* pixels)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexImage1D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexImage1D is NULL " << std::endl;
	else
	{
		_ptr_to_glTexImage1D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLsizei,GLint,GLenum,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glTexImage1D(target,level,internalformat,width,border,format,type,pixels);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexImage1D)(GLenum target,GLint level,GLint internalformat,GLsizei width,GLint border,GLenum format,GLenum type,const GLvoid* pixels) = _choose_glTexImage1D;
static void API_ENTRY _choose_glTexImage2D(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,GLenum format,GLenum type,const GLvoid* pixels)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexImage2D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexImage2D is NULL " << std::endl;
	else
	{
		_ptr_to_glTexImage2D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glTexImage2D(target,level,internalformat,width,height,border,format,type,pixels);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexImage2D)(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,GLenum format,GLenum type,const GLvoid* pixels) = _choose_glTexImage2D;
static void API_ENTRY _choose_glTexImage2DMultisample(GLenum target,GLsizei samples,GLint internalformat,GLsizei width,GLsizei height,GLboolean fixedsamplelocations)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexImage2DMultisample");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexImage2DMultisample is NULL " << std::endl;
	else
	{
		_ptr_to_glTexImage2DMultisample = (void(API_ENTRY*)(GLenum,GLsizei,GLint,GLsizei,GLsizei,GLboolean))gotPtr;
		return _ptr_to_glTexImage2DMultisample(target,samples,internalformat,width,height,fixedsamplelocations);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexImage2DMultisample)(GLenum target,GLsizei samples,GLint internalformat,GLsizei width,GLsizei height,GLboolean fixedsamplelocations) = _choose_glTexImage2DMultisample;
static void API_ENTRY _choose_glTexImage3D(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLenum format,GLenum type,const GLvoid* pixels)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexImage3D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexImage3D is NULL " << std::endl;
	else
	{
		_ptr_to_glTexImage3D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLsizei,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glTexImage3D(target,level,internalformat,width,height,depth,border,format,type,pixels);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexImage3D)(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLenum format,GLenum type,const GLvoid* pixels) = _choose_glTexImage3D;
static void API_ENTRY _choose_glTexImage3DMultisample(GLenum target,GLsizei samples,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLboolean fixedsamplelocations)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexImage3DMultisample");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexImage3DMultisample is NULL " << std::endl;
	else
	{
		_ptr_to_glTexImage3DMultisample = (void(API_ENTRY*)(GLenum,GLsizei,GLint,GLsizei,GLsizei,GLsizei,GLboolean))gotPtr;
		return _ptr_to_glTexImage3DMultisample(target,samples,internalformat,width,height,depth,fixedsamplelocations);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexImage3DMultisample)(GLenum target,GLsizei samples,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLboolean fixedsamplelocations) = _choose_glTexImage3DMultisample;
static void API_ENTRY _choose_glTexParameterIiv(GLenum target,GLenum pname,const GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexParameterIiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexParameterIiv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexParameterIiv = (void(API_ENTRY*)(GLenum,GLenum,const GLint*))gotPtr;
		return _ptr_to_glTexParameterIiv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexParameterIiv)(GLenum target,GLenum pname,const GLint* params) = _choose_glTexParameterIiv;
static void API_ENTRY _choose_glTexParameterIuiv(GLenum target,GLenum pname,const GLuint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexParameterIuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexParameterIuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexParameterIuiv = (void(API_ENTRY*)(GLenum,GLenum,const GLuint*))gotPtr;
		return _ptr_to_glTexParameterIuiv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexParameterIuiv)(GLenum target,GLenum pname,const GLuint* params) = _choose_glTexParameterIuiv;
static void API_ENTRY _choose_glTexParameterf(GLenum target,GLenum pname,GLfloat param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexParameterf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexParameterf is NULL " << std::endl;
	else
	{
		_ptr_to_glTexParameterf = (void(API_ENTRY*)(GLenum,GLenum,GLfloat))gotPtr;
		return _ptr_to_glTexParameterf(target,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexParameterf)(GLenum target,GLenum pname,GLfloat param) = _choose_glTexParameterf;
static void API_ENTRY _choose_glTexParameterfv(GLenum target,GLenum pname,const GLfloat* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexParameterfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexParameterfv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexParameterfv = (void(API_ENTRY*)(GLenum,GLenum,const GLfloat*))gotPtr;
		return _ptr_to_glTexParameterfv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexParameterfv)(GLenum target,GLenum pname,const GLfloat* params) = _choose_glTexParameterfv;
static void API_ENTRY _choose_glTexParameteri(GLenum target,GLenum pname,GLint param)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexParameteri");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexParameteri is NULL " << std::endl;
	else
	{
		_ptr_to_glTexParameteri = (void(API_ENTRY*)(GLenum,GLenum,GLint))gotPtr;
		return _ptr_to_glTexParameteri(target,pname,param);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexParameteri)(GLenum target,GLenum pname,GLint param) = _choose_glTexParameteri;
static void API_ENTRY _choose_glTexParameteriv(GLenum target,GLenum pname,const GLint* params)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexParameteriv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexParameteriv is NULL " << std::endl;
	else
	{
		_ptr_to_glTexParameteriv = (void(API_ENTRY*)(GLenum,GLenum,const GLint*))gotPtr;
		return _ptr_to_glTexParameteriv(target,pname,params);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexParameteriv)(GLenum target,GLenum pname,const GLint* params) = _choose_glTexParameteriv;
static void API_ENTRY _choose_glTexStorage1D(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexStorage1D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexStorage1D is NULL " << std::endl;
	else
	{
		_ptr_to_glTexStorage1D = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,GLsizei))gotPtr;
		return _ptr_to_glTexStorage1D(target,levels,internalformat,width);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexStorage1D)(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width) = _choose_glTexStorage1D;
static void API_ENTRY _choose_glTexStorage2D(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexStorage2D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexStorage2D is NULL " << std::endl;
	else
	{
		_ptr_to_glTexStorage2D = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glTexStorage2D(target,levels,internalformat,width,height);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexStorage2D)(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height) = _choose_glTexStorage2D;
static void API_ENTRY _choose_glTexStorage3D(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexStorage3D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexStorage3D is NULL " << std::endl;
	else
	{
		_ptr_to_glTexStorage3D = (void(API_ENTRY*)(GLenum,GLsizei,GLenum,GLsizei,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glTexStorage3D(target,levels,internalformat,width,height,depth);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexStorage3D)(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth) = _choose_glTexStorage3D;
static void API_ENTRY _choose_glTexSubImage1D(GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLenum type,const GLvoid* pixels)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexSubImage1D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexSubImage1D is NULL " << std::endl;
	else
	{
		_ptr_to_glTexSubImage1D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLsizei,GLenum,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glTexSubImage1D(target,level,xoffset,width,format,type,pixels);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexSubImage1D)(GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLenum type,const GLvoid* pixels) = _choose_glTexSubImage1D;
static void API_ENTRY _choose_glTexSubImage2D(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLenum type,const GLvoid* pixels)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexSubImage2D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexSubImage2D is NULL " << std::endl;
	else
	{
		_ptr_to_glTexSubImage2D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexSubImage2D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLenum type,const GLvoid* pixels) = _choose_glTexSubImage2D;
static void API_ENTRY _choose_glTexSubImage3D(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,const GLvoid* pixels)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTexSubImage3D");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTexSubImage3D is NULL " << std::endl;
	else
	{
		_ptr_to_glTexSubImage3D = (void(API_ENTRY*)(GLenum,GLint,GLint,GLint,GLint,GLsizei,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*))gotPtr;
		return _ptr_to_glTexSubImage3D(target,level,xoffset,yoffset,zoffset,width,height,depth,format,type,pixels);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTexSubImage3D)(GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,const GLvoid* pixels) = _choose_glTexSubImage3D;
static void API_ENTRY _choose_glTransformFeedbackVaryings(GLuint program,GLsizei count,const GLchar*const* varyings,GLenum bufferMode)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTransformFeedbackVaryings");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTransformFeedbackVaryings is NULL " << std::endl;
	else
	{
		_ptr_to_glTransformFeedbackVaryings = (void(API_ENTRY*)(GLuint,GLsizei,const GLchar*const*,GLenum))gotPtr;
		return _ptr_to_glTransformFeedbackVaryings(program,count,varyings,bufferMode);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTransformFeedbackVaryings)(GLuint program,GLsizei count,const GLchar*const* varyings,GLenum bufferMode) = _choose_glTransformFeedbackVaryings;
static void API_ENTRY _choose_glTranslated(GLdouble x,GLdouble y,GLdouble z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTranslated");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTranslated is NULL " << std::endl;
	else
	{
		_ptr_to_glTranslated = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glTranslated(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTranslated)(GLdouble x,GLdouble y,GLdouble z) = _choose_glTranslated;
static void API_ENTRY _choose_glTranslatef(GLfloat x,GLfloat y,GLfloat z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glTranslatef");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glTranslatef is NULL " << std::endl;
	else
	{
		_ptr_to_glTranslatef = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glTranslatef(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glTranslatef)(GLfloat x,GLfloat y,GLfloat z) = _choose_glTranslatef;
static void API_ENTRY _choose_glUniform1d(GLint location,GLdouble x)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1d is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1d = (void(API_ENTRY*)(GLint,GLdouble))gotPtr;
		return _ptr_to_glUniform1d(location,x);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1d)(GLint location,GLdouble x) = _choose_glUniform1d;
static void API_ENTRY _choose_glUniform1dv(GLint location,GLsizei count,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1dv = (void(API_ENTRY*)(GLint,GLsizei,const GLdouble*))gotPtr;
		return _ptr_to_glUniform1dv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1dv)(GLint location,GLsizei count,const GLdouble* value) = _choose_glUniform1dv;
static void API_ENTRY _choose_glUniform1f(GLint location,GLfloat v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1f is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1f = (void(API_ENTRY*)(GLint,GLfloat))gotPtr;
		return _ptr_to_glUniform1f(location,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1f)(GLint location,GLfloat v0) = _choose_glUniform1f;
static void API_ENTRY _choose_glUniform1fARB(GLint location,GLfloat v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1fARB = (void(API_ENTRY*)(GLint,GLfloat))gotPtr;
		return _ptr_to_glUniform1fARB(location,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1fARB)(GLint location,GLfloat v0) = _choose_glUniform1fARB;
static void API_ENTRY _choose_glUniform1fv(GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1fv = (void(API_ENTRY*)(GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glUniform1fv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1fv)(GLint location,GLsizei count,const GLfloat* value) = _choose_glUniform1fv;
static void API_ENTRY _choose_glUniform1fvARB(GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1fvARB = (void(API_ENTRY*)(GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glUniform1fvARB(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1fvARB)(GLint location,GLsizei count,const GLfloat* value) = _choose_glUniform1fvARB;
static void API_ENTRY _choose_glUniform1i(GLint location,GLint v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1i is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1i = (void(API_ENTRY*)(GLint,GLint))gotPtr;
		return _ptr_to_glUniform1i(location,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1i)(GLint location,GLint v0) = _choose_glUniform1i;
static void API_ENTRY _choose_glUniform1iARB(GLint location,GLint v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1iARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1iARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1iARB = (void(API_ENTRY*)(GLint,GLint))gotPtr;
		return _ptr_to_glUniform1iARB(location,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1iARB)(GLint location,GLint v0) = _choose_glUniform1iARB;
static void API_ENTRY _choose_glUniform1iv(GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1iv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1iv = (void(API_ENTRY*)(GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glUniform1iv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1iv)(GLint location,GLsizei count,const GLint* value) = _choose_glUniform1iv;
static void API_ENTRY _choose_glUniform1ivARB(GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1ivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1ivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1ivARB = (void(API_ENTRY*)(GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glUniform1ivARB(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1ivARB)(GLint location,GLsizei count,const GLint* value) = _choose_glUniform1ivARB;
static void API_ENTRY _choose_glUniform1ui(GLint location,GLuint v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1ui is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1ui = (void(API_ENTRY*)(GLint,GLuint))gotPtr;
		return _ptr_to_glUniform1ui(location,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1ui)(GLint location,GLuint v0) = _choose_glUniform1ui;
static void API_ENTRY _choose_glUniform1uiv(GLint location,GLsizei count,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform1uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform1uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform1uiv = (void(API_ENTRY*)(GLint,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glUniform1uiv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform1uiv)(GLint location,GLsizei count,const GLuint* value) = _choose_glUniform1uiv;
static void API_ENTRY _choose_glUniform2d(GLint location,GLdouble x,GLdouble y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2d is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2d = (void(API_ENTRY*)(GLint,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glUniform2d(location,x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2d)(GLint location,GLdouble x,GLdouble y) = _choose_glUniform2d;
static void API_ENTRY _choose_glUniform2dv(GLint location,GLsizei count,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2dv = (void(API_ENTRY*)(GLint,GLsizei,const GLdouble*))gotPtr;
		return _ptr_to_glUniform2dv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2dv)(GLint location,GLsizei count,const GLdouble* value) = _choose_glUniform2dv;
static void API_ENTRY _choose_glUniform2f(GLint location,GLfloat v0,GLfloat v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2f is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2f = (void(API_ENTRY*)(GLint,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glUniform2f(location,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2f)(GLint location,GLfloat v0,GLfloat v1) = _choose_glUniform2f;
static void API_ENTRY _choose_glUniform2fARB(GLint location,GLfloat v0,GLfloat v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2fARB = (void(API_ENTRY*)(GLint,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glUniform2fARB(location,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2fARB)(GLint location,GLfloat v0,GLfloat v1) = _choose_glUniform2fARB;
static void API_ENTRY _choose_glUniform2fv(GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2fv = (void(API_ENTRY*)(GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glUniform2fv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2fv)(GLint location,GLsizei count,const GLfloat* value) = _choose_glUniform2fv;
static void API_ENTRY _choose_glUniform2fvARB(GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2fvARB = (void(API_ENTRY*)(GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glUniform2fvARB(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2fvARB)(GLint location,GLsizei count,const GLfloat* value) = _choose_glUniform2fvARB;
static void API_ENTRY _choose_glUniform2i(GLint location,GLint v0,GLint v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2i is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2i = (void(API_ENTRY*)(GLint,GLint,GLint))gotPtr;
		return _ptr_to_glUniform2i(location,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2i)(GLint location,GLint v0,GLint v1) = _choose_glUniform2i;
static void API_ENTRY _choose_glUniform2iARB(GLint location,GLint v0,GLint v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2iARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2iARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2iARB = (void(API_ENTRY*)(GLint,GLint,GLint))gotPtr;
		return _ptr_to_glUniform2iARB(location,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2iARB)(GLint location,GLint v0,GLint v1) = _choose_glUniform2iARB;
static void API_ENTRY _choose_glUniform2iv(GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2iv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2iv = (void(API_ENTRY*)(GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glUniform2iv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2iv)(GLint location,GLsizei count,const GLint* value) = _choose_glUniform2iv;
static void API_ENTRY _choose_glUniform2ivARB(GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2ivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2ivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2ivARB = (void(API_ENTRY*)(GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glUniform2ivARB(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2ivARB)(GLint location,GLsizei count,const GLint* value) = _choose_glUniform2ivARB;
static void API_ENTRY _choose_glUniform2ui(GLint location,GLuint v0,GLuint v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2ui is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2ui = (void(API_ENTRY*)(GLint,GLuint,GLuint))gotPtr;
		return _ptr_to_glUniform2ui(location,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2ui)(GLint location,GLuint v0,GLuint v1) = _choose_glUniform2ui;
static void API_ENTRY _choose_glUniform2uiv(GLint location,GLsizei count,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform2uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform2uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform2uiv = (void(API_ENTRY*)(GLint,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glUniform2uiv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform2uiv)(GLint location,GLsizei count,const GLuint* value) = _choose_glUniform2uiv;
static void API_ENTRY _choose_glUniform3d(GLint location,GLdouble x,GLdouble y,GLdouble z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3d is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3d = (void(API_ENTRY*)(GLint,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glUniform3d(location,x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3d)(GLint location,GLdouble x,GLdouble y,GLdouble z) = _choose_glUniform3d;
static void API_ENTRY _choose_glUniform3dv(GLint location,GLsizei count,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3dv = (void(API_ENTRY*)(GLint,GLsizei,const GLdouble*))gotPtr;
		return _ptr_to_glUniform3dv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3dv)(GLint location,GLsizei count,const GLdouble* value) = _choose_glUniform3dv;
static void API_ENTRY _choose_glUniform3f(GLint location,GLfloat v0,GLfloat v1,GLfloat v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3f is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3f = (void(API_ENTRY*)(GLint,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glUniform3f(location,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3f)(GLint location,GLfloat v0,GLfloat v1,GLfloat v2) = _choose_glUniform3f;
static void API_ENTRY _choose_glUniform3fARB(GLint location,GLfloat v0,GLfloat v1,GLfloat v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3fARB = (void(API_ENTRY*)(GLint,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glUniform3fARB(location,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3fARB)(GLint location,GLfloat v0,GLfloat v1,GLfloat v2) = _choose_glUniform3fARB;
static void API_ENTRY _choose_glUniform3fv(GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3fv = (void(API_ENTRY*)(GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glUniform3fv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3fv)(GLint location,GLsizei count,const GLfloat* value) = _choose_glUniform3fv;
static void API_ENTRY _choose_glUniform3fvARB(GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3fvARB = (void(API_ENTRY*)(GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glUniform3fvARB(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3fvARB)(GLint location,GLsizei count,const GLfloat* value) = _choose_glUniform3fvARB;
static void API_ENTRY _choose_glUniform3i(GLint location,GLint v0,GLint v1,GLint v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3i is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3i = (void(API_ENTRY*)(GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glUniform3i(location,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3i)(GLint location,GLint v0,GLint v1,GLint v2) = _choose_glUniform3i;
static void API_ENTRY _choose_glUniform3iARB(GLint location,GLint v0,GLint v1,GLint v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3iARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3iARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3iARB = (void(API_ENTRY*)(GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glUniform3iARB(location,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3iARB)(GLint location,GLint v0,GLint v1,GLint v2) = _choose_glUniform3iARB;
static void API_ENTRY _choose_glUniform3iv(GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3iv = (void(API_ENTRY*)(GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glUniform3iv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3iv)(GLint location,GLsizei count,const GLint* value) = _choose_glUniform3iv;
static void API_ENTRY _choose_glUniform3ivARB(GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3ivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3ivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3ivARB = (void(API_ENTRY*)(GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glUniform3ivARB(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3ivARB)(GLint location,GLsizei count,const GLint* value) = _choose_glUniform3ivARB;
static void API_ENTRY _choose_glUniform3ui(GLint location,GLuint v0,GLuint v1,GLuint v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3ui = (void(API_ENTRY*)(GLint,GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glUniform3ui(location,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3ui)(GLint location,GLuint v0,GLuint v1,GLuint v2) = _choose_glUniform3ui;
static void API_ENTRY _choose_glUniform3uiv(GLint location,GLsizei count,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform3uiv = (void(API_ENTRY*)(GLint,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glUniform3uiv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform3uiv)(GLint location,GLsizei count,const GLuint* value) = _choose_glUniform3uiv;
static void API_ENTRY _choose_glUniform4d(GLint location,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4d is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4d = (void(API_ENTRY*)(GLint,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glUniform4d(location,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4d)(GLint location,GLdouble x,GLdouble y,GLdouble z,GLdouble w) = _choose_glUniform4d;
static void API_ENTRY _choose_glUniform4dv(GLint location,GLsizei count,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4dv = (void(API_ENTRY*)(GLint,GLsizei,const GLdouble*))gotPtr;
		return _ptr_to_glUniform4dv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4dv)(GLint location,GLsizei count,const GLdouble* value) = _choose_glUniform4dv;
static void API_ENTRY _choose_glUniform4f(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4f is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4f = (void(API_ENTRY*)(GLint,GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glUniform4f(location,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4f)(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3) = _choose_glUniform4f;
static void API_ENTRY _choose_glUniform4fARB(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4fARB = (void(API_ENTRY*)(GLint,GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glUniform4fARB(location,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4fARB)(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3) = _choose_glUniform4fARB;
static void API_ENTRY _choose_glUniform4fv(GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4fv = (void(API_ENTRY*)(GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glUniform4fv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4fv)(GLint location,GLsizei count,const GLfloat* value) = _choose_glUniform4fv;
static void API_ENTRY _choose_glUniform4fvARB(GLint location,GLsizei count,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4fvARB = (void(API_ENTRY*)(GLint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glUniform4fvARB(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4fvARB)(GLint location,GLsizei count,const GLfloat* value) = _choose_glUniform4fvARB;
static void API_ENTRY _choose_glUniform4i(GLint location,GLint v0,GLint v1,GLint v2,GLint v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4i is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4i = (void(API_ENTRY*)(GLint,GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glUniform4i(location,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4i)(GLint location,GLint v0,GLint v1,GLint v2,GLint v3) = _choose_glUniform4i;
static void API_ENTRY _choose_glUniform4iARB(GLint location,GLint v0,GLint v1,GLint v2,GLint v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4iARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4iARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4iARB = (void(API_ENTRY*)(GLint,GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glUniform4iARB(location,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4iARB)(GLint location,GLint v0,GLint v1,GLint v2,GLint v3) = _choose_glUniform4iARB;
static void API_ENTRY _choose_glUniform4iv(GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4iv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4iv = (void(API_ENTRY*)(GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glUniform4iv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4iv)(GLint location,GLsizei count,const GLint* value) = _choose_glUniform4iv;
static void API_ENTRY _choose_glUniform4ivARB(GLint location,GLsizei count,const GLint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4ivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4ivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4ivARB = (void(API_ENTRY*)(GLint,GLsizei,const GLint*))gotPtr;
		return _ptr_to_glUniform4ivARB(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4ivARB)(GLint location,GLsizei count,const GLint* value) = _choose_glUniform4ivARB;
static void API_ENTRY _choose_glUniform4ui(GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4ui is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4ui = (void(API_ENTRY*)(GLint,GLuint,GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glUniform4ui(location,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4ui)(GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3) = _choose_glUniform4ui;
static void API_ENTRY _choose_glUniform4uiv(GLint location,GLsizei count,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniform4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniform4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniform4uiv = (void(API_ENTRY*)(GLint,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glUniform4uiv(location,count,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniform4uiv)(GLint location,GLsizei count,const GLuint* value) = _choose_glUniform4uiv;
static void API_ENTRY _choose_glUniformBlockBinding(GLuint program,GLuint uniformBlockIndex,GLuint uniformBlockBinding)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformBlockBinding");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformBlockBinding is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformBlockBinding = (void(API_ENTRY*)(GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glUniformBlockBinding(program,uniformBlockIndex,uniformBlockBinding);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformBlockBinding)(GLuint program,GLuint uniformBlockIndex,GLuint uniformBlockBinding) = _choose_glUniformBlockBinding;
static void API_ENTRY _choose_glUniformMatrix2dv(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix2dv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glUniformMatrix2dv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix2dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glUniformMatrix2dv;
static void API_ENTRY _choose_glUniformMatrix2fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix2fv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix2fv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix2fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix2fv;
static void API_ENTRY _choose_glUniformMatrix2fvARB(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix2fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix2fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix2fvARB = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix2fvARB(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix2fvARB)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix2fvARB;
static void API_ENTRY _choose_glUniformMatrix2x3dv(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix2x3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix2x3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix2x3dv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glUniformMatrix2x3dv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix2x3dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glUniformMatrix2x3dv;
static void API_ENTRY _choose_glUniformMatrix2x3fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix2x3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix2x3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix2x3fv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix2x3fv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix2x3fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix2x3fv;
static void API_ENTRY _choose_glUniformMatrix2x4dv(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix2x4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix2x4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix2x4dv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glUniformMatrix2x4dv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix2x4dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glUniformMatrix2x4dv;
static void API_ENTRY _choose_glUniformMatrix2x4fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix2x4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix2x4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix2x4fv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix2x4fv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix2x4fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix2x4fv;
static void API_ENTRY _choose_glUniformMatrix3dv(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix3dv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glUniformMatrix3dv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix3dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glUniformMatrix3dv;
static void API_ENTRY _choose_glUniformMatrix3fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix3fv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix3fv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix3fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix3fv;
static void API_ENTRY _choose_glUniformMatrix3fvARB(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix3fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix3fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix3fvARB = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix3fvARB(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix3fvARB)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix3fvARB;
static void API_ENTRY _choose_glUniformMatrix3x2dv(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix3x2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix3x2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix3x2dv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glUniformMatrix3x2dv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix3x2dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glUniformMatrix3x2dv;
static void API_ENTRY _choose_glUniformMatrix3x2fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix3x2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix3x2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix3x2fv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix3x2fv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix3x2fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix3x2fv;
static void API_ENTRY _choose_glUniformMatrix3x4dv(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix3x4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix3x4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix3x4dv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glUniformMatrix3x4dv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix3x4dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glUniformMatrix3x4dv;
static void API_ENTRY _choose_glUniformMatrix3x4fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix3x4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix3x4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix3x4fv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix3x4fv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix3x4fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix3x4fv;
static void API_ENTRY _choose_glUniformMatrix4dv(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix4dv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glUniformMatrix4dv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix4dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glUniformMatrix4dv;
static void API_ENTRY _choose_glUniformMatrix4fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix4fv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix4fv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix4fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix4fv;
static void API_ENTRY _choose_glUniformMatrix4fvARB(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix4fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix4fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix4fvARB = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix4fvARB(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix4fvARB)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix4fvARB;
static void API_ENTRY _choose_glUniformMatrix4x2dv(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix4x2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix4x2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix4x2dv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glUniformMatrix4x2dv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix4x2dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glUniformMatrix4x2dv;
static void API_ENTRY _choose_glUniformMatrix4x2fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix4x2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix4x2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix4x2fv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix4x2fv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix4x2fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix4x2fv;
static void API_ENTRY _choose_glUniformMatrix4x3dv(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix4x3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix4x3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix4x3dv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLdouble*))gotPtr;
		return _ptr_to_glUniformMatrix4x3dv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix4x3dv)(GLint location,GLsizei count,GLboolean transpose,const GLdouble* value) = _choose_glUniformMatrix4x3dv;
static void API_ENTRY _choose_glUniformMatrix4x3fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformMatrix4x3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformMatrix4x3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformMatrix4x3fv = (void(API_ENTRY*)(GLint,GLsizei,GLboolean,const GLfloat*))gotPtr;
		return _ptr_to_glUniformMatrix4x3fv(location,count,transpose,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformMatrix4x3fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value) = _choose_glUniformMatrix4x3fv;
static void API_ENTRY _choose_glUniformSubroutinesuiv(GLenum shadertype,GLsizei count,const GLuint* indices)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUniformSubroutinesuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUniformSubroutinesuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glUniformSubroutinesuiv = (void(API_ENTRY*)(GLenum,GLsizei,const GLuint*))gotPtr;
		return _ptr_to_glUniformSubroutinesuiv(shadertype,count,indices);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUniformSubroutinesuiv)(GLenum shadertype,GLsizei count,const GLuint* indices) = _choose_glUniformSubroutinesuiv;
static GLboolean API_ENTRY _choose_glUnmapBuffer(GLenum target)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUnmapBuffer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUnmapBuffer is NULL " << std::endl;
	else
	{
		_ptr_to_glUnmapBuffer = (GLboolean(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glUnmapBuffer(target);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glUnmapBuffer)(GLenum target) = _choose_glUnmapBuffer;
static GLboolean API_ENTRY _choose_glUnmapBufferARB(GLenum target)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUnmapBufferARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUnmapBufferARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUnmapBufferARB = (GLboolean(API_ENTRY*)(GLenum))gotPtr;
		return _ptr_to_glUnmapBufferARB(target);
	}
	typedef GLboolean RET_TYPE;
	return RET_TYPE();
}
GLboolean (API_ENTRY *_ptr_to_glUnmapBufferARB)(GLenum target) = _choose_glUnmapBufferARB;
static void API_ENTRY _choose_glUseProgram(GLuint program)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUseProgram");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUseProgram is NULL " << std::endl;
	else
	{
		_ptr_to_glUseProgram = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glUseProgram(program);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUseProgram)(GLuint program) = _choose_glUseProgram;
static void API_ENTRY _choose_glUseProgramObjectARB(GLhandleARB programObj)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUseProgramObjectARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUseProgramObjectARB is NULL " << std::endl;
	else
	{
		_ptr_to_glUseProgramObjectARB = (void(API_ENTRY*)(GLhandleARB))gotPtr;
		return _ptr_to_glUseProgramObjectARB(programObj);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUseProgramObjectARB)(GLhandleARB programObj) = _choose_glUseProgramObjectARB;
static void API_ENTRY _choose_glUseProgramStages(GLuint pipeline,GLbitfield stages,GLuint program)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glUseProgramStages");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glUseProgramStages is NULL " << std::endl;
	else
	{
		_ptr_to_glUseProgramStages = (void(API_ENTRY*)(GLuint,GLbitfield,GLuint))gotPtr;
		return _ptr_to_glUseProgramStages(pipeline,stages,program);
	}
	return;
}
void (API_ENTRY *_ptr_to_glUseProgramStages)(GLuint pipeline,GLbitfield stages,GLuint program) = _choose_glUseProgramStages;
static void API_ENTRY _choose_glValidateProgram(GLuint program)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glValidateProgram");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glValidateProgram is NULL " << std::endl;
	else
	{
		_ptr_to_glValidateProgram = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glValidateProgram(program);
	}
	return;
}
void (API_ENTRY *_ptr_to_glValidateProgram)(GLuint program) = _choose_glValidateProgram;
static void API_ENTRY _choose_glValidateProgramARB(GLhandleARB programObj)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glValidateProgramARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glValidateProgramARB is NULL " << std::endl;
	else
	{
		_ptr_to_glValidateProgramARB = (void(API_ENTRY*)(GLhandleARB))gotPtr;
		return _ptr_to_glValidateProgramARB(programObj);
	}
	return;
}
void (API_ENTRY *_ptr_to_glValidateProgramARB)(GLhandleARB programObj) = _choose_glValidateProgramARB;
static void API_ENTRY _choose_glValidateProgramPipeline(GLuint pipeline)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glValidateProgramPipeline");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glValidateProgramPipeline is NULL " << std::endl;
	else
	{
		_ptr_to_glValidateProgramPipeline = (void(API_ENTRY*)(GLuint))gotPtr;
		return _ptr_to_glValidateProgramPipeline(pipeline);
	}
	return;
}
void (API_ENTRY *_ptr_to_glValidateProgramPipeline)(GLuint pipeline) = _choose_glValidateProgramPipeline;
static void API_ENTRY _choose_glVertex2d(GLdouble x,GLdouble y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex2d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex2d = (void(API_ENTRY*)(GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertex2d(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex2d)(GLdouble x,GLdouble y) = _choose_glVertex2d;
static void API_ENTRY _choose_glVertex2dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex2dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glVertex2dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex2dv)(const GLdouble* v) = _choose_glVertex2dv;
static void API_ENTRY _choose_glVertex2f(GLfloat x,GLfloat y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex2f is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex2f = (void(API_ENTRY*)(GLfloat,GLfloat))gotPtr;
		return _ptr_to_glVertex2f(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex2f)(GLfloat x,GLfloat y) = _choose_glVertex2f;
static void API_ENTRY _choose_glVertex2fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex2fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glVertex2fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex2fv)(const GLfloat* v) = _choose_glVertex2fv;
static void API_ENTRY _choose_glVertex2i(GLint x,GLint y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex2i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex2i is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex2i = (void(API_ENTRY*)(GLint,GLint))gotPtr;
		return _ptr_to_glVertex2i(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex2i)(GLint x,GLint y) = _choose_glVertex2i;
static void API_ENTRY _choose_glVertex2iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex2iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex2iv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex2iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glVertex2iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex2iv)(const GLint* v) = _choose_glVertex2iv;
static void API_ENTRY _choose_glVertex2s(GLshort x,GLshort y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex2s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex2s is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex2s = (void(API_ENTRY*)(GLshort,GLshort))gotPtr;
		return _ptr_to_glVertex2s(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex2s)(GLshort x,GLshort y) = _choose_glVertex2s;
static void API_ENTRY _choose_glVertex2sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex2sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex2sv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex2sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glVertex2sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex2sv)(const GLshort* v) = _choose_glVertex2sv;
static void API_ENTRY _choose_glVertex3d(GLdouble x,GLdouble y,GLdouble z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex3d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex3d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertex3d(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex3d)(GLdouble x,GLdouble y,GLdouble z) = _choose_glVertex3d;
static void API_ENTRY _choose_glVertex3dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex3dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glVertex3dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex3dv)(const GLdouble* v) = _choose_glVertex3dv;
static void API_ENTRY _choose_glVertex3f(GLfloat x,GLfloat y,GLfloat z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex3f is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex3f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glVertex3f(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex3f)(GLfloat x,GLfloat y,GLfloat z) = _choose_glVertex3f;
static void API_ENTRY _choose_glVertex3fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex3fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glVertex3fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex3fv)(const GLfloat* v) = _choose_glVertex3fv;
static void API_ENTRY _choose_glVertex3i(GLint x,GLint y,GLint z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex3i is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex3i = (void(API_ENTRY*)(GLint,GLint,GLint))gotPtr;
		return _ptr_to_glVertex3i(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex3i)(GLint x,GLint y,GLint z) = _choose_glVertex3i;
static void API_ENTRY _choose_glVertex3iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex3iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glVertex3iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex3iv)(const GLint* v) = _choose_glVertex3iv;
static void API_ENTRY _choose_glVertex3s(GLshort x,GLshort y,GLshort z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex3s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex3s is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex3s = (void(API_ENTRY*)(GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glVertex3s(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex3s)(GLshort x,GLshort y,GLshort z) = _choose_glVertex3s;
static void API_ENTRY _choose_glVertex3sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex3sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex3sv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex3sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glVertex3sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex3sv)(const GLshort* v) = _choose_glVertex3sv;
static void API_ENTRY _choose_glVertex4d(GLdouble x,GLdouble y,GLdouble z,GLdouble w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex4d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex4d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex4d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertex4d(x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex4d)(GLdouble x,GLdouble y,GLdouble z,GLdouble w) = _choose_glVertex4d;
static void API_ENTRY _choose_glVertex4dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex4dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glVertex4dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex4dv)(const GLdouble* v) = _choose_glVertex4dv;
static void API_ENTRY _choose_glVertex4f(GLfloat x,GLfloat y,GLfloat z,GLfloat w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex4f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex4f is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex4f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glVertex4f(x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex4f)(GLfloat x,GLfloat y,GLfloat z,GLfloat w) = _choose_glVertex4f;
static void API_ENTRY _choose_glVertex4fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex4fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glVertex4fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex4fv)(const GLfloat* v) = _choose_glVertex4fv;
static void API_ENTRY _choose_glVertex4i(GLint x,GLint y,GLint z,GLint w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex4i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex4i is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex4i = (void(API_ENTRY*)(GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glVertex4i(x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex4i)(GLint x,GLint y,GLint z,GLint w) = _choose_glVertex4i;
static void API_ENTRY _choose_glVertex4iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex4iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex4iv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex4iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glVertex4iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex4iv)(const GLint* v) = _choose_glVertex4iv;
static void API_ENTRY _choose_glVertex4s(GLshort x,GLshort y,GLshort z,GLshort w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex4s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex4s is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex4s = (void(API_ENTRY*)(GLshort,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glVertex4s(x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex4s)(GLshort x,GLshort y,GLshort z,GLshort w) = _choose_glVertex4s;
static void API_ENTRY _choose_glVertex4sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertex4sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertex4sv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertex4sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glVertex4sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertex4sv)(const GLshort* v) = _choose_glVertex4sv;
static void API_ENTRY _choose_glVertexAttrib1d(GLuint index,GLdouble x)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1d = (void(API_ENTRY*)(GLuint,GLdouble))gotPtr;
		return _ptr_to_glVertexAttrib1d(index,x);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1d)(GLuint index,GLdouble x) = _choose_glVertexAttrib1d;
static void API_ENTRY _choose_glVertexAttrib1dARB(GLuint index,GLdouble v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1dARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1dARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1dARB = (void(API_ENTRY*)(GLuint,GLdouble))gotPtr;
		return _ptr_to_glVertexAttrib1dARB(index,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1dARB)(GLuint index,GLdouble v0) = _choose_glVertexAttrib1dARB;
static void API_ENTRY _choose_glVertexAttrib1dv(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1dv = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttrib1dv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1dv)(GLuint index,const GLdouble* v) = _choose_glVertexAttrib1dv;
static void API_ENTRY _choose_glVertexAttrib1dvARB(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1dvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1dvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1dvARB = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttrib1dvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1dvARB)(GLuint index,const GLdouble* v) = _choose_glVertexAttrib1dvARB;
static void API_ENTRY _choose_glVertexAttrib1f(GLuint index,GLfloat x)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1f is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1f = (void(API_ENTRY*)(GLuint,GLfloat))gotPtr;
		return _ptr_to_glVertexAttrib1f(index,x);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1f)(GLuint index,GLfloat x) = _choose_glVertexAttrib1f;
static void API_ENTRY _choose_glVertexAttrib1fARB(GLuint index,GLfloat v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1fARB = (void(API_ENTRY*)(GLuint,GLfloat))gotPtr;
		return _ptr_to_glVertexAttrib1fARB(index,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1fARB)(GLuint index,GLfloat v0) = _choose_glVertexAttrib1fARB;
static void API_ENTRY _choose_glVertexAttrib1fv(GLuint index,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1fv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1fv = (void(API_ENTRY*)(GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glVertexAttrib1fv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1fv)(GLuint index,const GLfloat* v) = _choose_glVertexAttrib1fv;
static void API_ENTRY _choose_glVertexAttrib1fvARB(GLuint index,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1fvARB = (void(API_ENTRY*)(GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glVertexAttrib1fvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1fvARB)(GLuint index,const GLfloat* v) = _choose_glVertexAttrib1fvARB;
static void API_ENTRY _choose_glVertexAttrib1s(GLuint index,GLshort x)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1s is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1s = (void(API_ENTRY*)(GLuint,GLshort))gotPtr;
		return _ptr_to_glVertexAttrib1s(index,x);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1s)(GLuint index,GLshort x) = _choose_glVertexAttrib1s;
static void API_ENTRY _choose_glVertexAttrib1sARB(GLuint index,GLshort v0)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1sARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1sARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1sARB = (void(API_ENTRY*)(GLuint,GLshort))gotPtr;
		return _ptr_to_glVertexAttrib1sARB(index,v0);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1sARB)(GLuint index,GLshort v0) = _choose_glVertexAttrib1sARB;
static void API_ENTRY _choose_glVertexAttrib1sv(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1sv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1sv = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib1sv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1sv)(GLuint index,const GLshort* v) = _choose_glVertexAttrib1sv;
static void API_ENTRY _choose_glVertexAttrib1svARB(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib1svARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib1svARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib1svARB = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib1svARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib1svARB)(GLuint index,const GLshort* v) = _choose_glVertexAttrib1svARB;
static void API_ENTRY _choose_glVertexAttrib2d(GLuint index,GLdouble x,GLdouble y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2d = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertexAttrib2d(index,x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2d)(GLuint index,GLdouble x,GLdouble y) = _choose_glVertexAttrib2d;
static void API_ENTRY _choose_glVertexAttrib2dARB(GLuint index,GLdouble v0,GLdouble v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2dARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2dARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2dARB = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertexAttrib2dARB(index,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2dARB)(GLuint index,GLdouble v0,GLdouble v1) = _choose_glVertexAttrib2dARB;
static void API_ENTRY _choose_glVertexAttrib2dv(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2dv = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttrib2dv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2dv)(GLuint index,const GLdouble* v) = _choose_glVertexAttrib2dv;
static void API_ENTRY _choose_glVertexAttrib2dvARB(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2dvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2dvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2dvARB = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttrib2dvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2dvARB)(GLuint index,const GLdouble* v) = _choose_glVertexAttrib2dvARB;
static void API_ENTRY _choose_glVertexAttrib2f(GLuint index,GLfloat x,GLfloat y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2f is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2f = (void(API_ENTRY*)(GLuint,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glVertexAttrib2f(index,x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2f)(GLuint index,GLfloat x,GLfloat y) = _choose_glVertexAttrib2f;
static void API_ENTRY _choose_glVertexAttrib2fARB(GLuint index,GLfloat v0,GLfloat v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2fARB = (void(API_ENTRY*)(GLuint,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glVertexAttrib2fARB(index,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2fARB)(GLuint index,GLfloat v0,GLfloat v1) = _choose_glVertexAttrib2fARB;
static void API_ENTRY _choose_glVertexAttrib2fv(GLuint index,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2fv = (void(API_ENTRY*)(GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glVertexAttrib2fv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2fv)(GLuint index,const GLfloat* v) = _choose_glVertexAttrib2fv;
static void API_ENTRY _choose_glVertexAttrib2fvARB(GLuint index,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2fvARB = (void(API_ENTRY*)(GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glVertexAttrib2fvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2fvARB)(GLuint index,const GLfloat* v) = _choose_glVertexAttrib2fvARB;
static void API_ENTRY _choose_glVertexAttrib2s(GLuint index,GLshort x,GLshort y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2s is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2s = (void(API_ENTRY*)(GLuint,GLshort,GLshort))gotPtr;
		return _ptr_to_glVertexAttrib2s(index,x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2s)(GLuint index,GLshort x,GLshort y) = _choose_glVertexAttrib2s;
static void API_ENTRY _choose_glVertexAttrib2sARB(GLuint index,GLshort v0,GLshort v1)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2sARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2sARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2sARB = (void(API_ENTRY*)(GLuint,GLshort,GLshort))gotPtr;
		return _ptr_to_glVertexAttrib2sARB(index,v0,v1);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2sARB)(GLuint index,GLshort v0,GLshort v1) = _choose_glVertexAttrib2sARB;
static void API_ENTRY _choose_glVertexAttrib2sv(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2sv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2sv = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib2sv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2sv)(GLuint index,const GLshort* v) = _choose_glVertexAttrib2sv;
static void API_ENTRY _choose_glVertexAttrib2svARB(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib2svARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib2svARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib2svARB = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib2svARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib2svARB)(GLuint index,const GLshort* v) = _choose_glVertexAttrib2svARB;
static void API_ENTRY _choose_glVertexAttrib3d(GLuint index,GLdouble x,GLdouble y,GLdouble z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3d = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertexAttrib3d(index,x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3d)(GLuint index,GLdouble x,GLdouble y,GLdouble z) = _choose_glVertexAttrib3d;
static void API_ENTRY _choose_glVertexAttrib3dARB(GLuint index,GLdouble v0,GLdouble v1,GLdouble v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3dARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3dARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3dARB = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertexAttrib3dARB(index,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3dARB)(GLuint index,GLdouble v0,GLdouble v1,GLdouble v2) = _choose_glVertexAttrib3dARB;
static void API_ENTRY _choose_glVertexAttrib3dv(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3dv = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttrib3dv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3dv)(GLuint index,const GLdouble* v) = _choose_glVertexAttrib3dv;
static void API_ENTRY _choose_glVertexAttrib3dvARB(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3dvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3dvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3dvARB = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttrib3dvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3dvARB)(GLuint index,const GLdouble* v) = _choose_glVertexAttrib3dvARB;
static void API_ENTRY _choose_glVertexAttrib3f(GLuint index,GLfloat x,GLfloat y,GLfloat z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3f is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3f = (void(API_ENTRY*)(GLuint,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glVertexAttrib3f(index,x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3f)(GLuint index,GLfloat x,GLfloat y,GLfloat z) = _choose_glVertexAttrib3f;
static void API_ENTRY _choose_glVertexAttrib3fARB(GLuint index,GLfloat v0,GLfloat v1,GLfloat v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3fARB = (void(API_ENTRY*)(GLuint,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glVertexAttrib3fARB(index,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3fARB)(GLuint index,GLfloat v0,GLfloat v1,GLfloat v2) = _choose_glVertexAttrib3fARB;
static void API_ENTRY _choose_glVertexAttrib3fv(GLuint index,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3fv = (void(API_ENTRY*)(GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glVertexAttrib3fv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3fv)(GLuint index,const GLfloat* v) = _choose_glVertexAttrib3fv;
static void API_ENTRY _choose_glVertexAttrib3fvARB(GLuint index,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3fvARB = (void(API_ENTRY*)(GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glVertexAttrib3fvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3fvARB)(GLuint index,const GLfloat* v) = _choose_glVertexAttrib3fvARB;
static void API_ENTRY _choose_glVertexAttrib3s(GLuint index,GLshort x,GLshort y,GLshort z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3s is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3s = (void(API_ENTRY*)(GLuint,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glVertexAttrib3s(index,x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3s)(GLuint index,GLshort x,GLshort y,GLshort z) = _choose_glVertexAttrib3s;
static void API_ENTRY _choose_glVertexAttrib3sARB(GLuint index,GLshort v0,GLshort v1,GLshort v2)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3sARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3sARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3sARB = (void(API_ENTRY*)(GLuint,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glVertexAttrib3sARB(index,v0,v1,v2);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3sARB)(GLuint index,GLshort v0,GLshort v1,GLshort v2) = _choose_glVertexAttrib3sARB;
static void API_ENTRY _choose_glVertexAttrib3sv(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3sv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3sv = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib3sv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3sv)(GLuint index,const GLshort* v) = _choose_glVertexAttrib3sv;
static void API_ENTRY _choose_glVertexAttrib3svARB(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib3svARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib3svARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib3svARB = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib3svARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib3svARB)(GLuint index,const GLshort* v) = _choose_glVertexAttrib3svARB;
static void API_ENTRY _choose_glVertexAttrib4Nbv(GLuint index,const GLbyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4Nbv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4Nbv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4Nbv = (void(API_ENTRY*)(GLuint,const GLbyte*))gotPtr;
		return _ptr_to_glVertexAttrib4Nbv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4Nbv)(GLuint index,const GLbyte* v) = _choose_glVertexAttrib4Nbv;
static void API_ENTRY _choose_glVertexAttrib4NbvARB(GLuint index,const GLbyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4NbvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4NbvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4NbvARB = (void(API_ENTRY*)(GLuint,const GLbyte*))gotPtr;
		return _ptr_to_glVertexAttrib4NbvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4NbvARB)(GLuint index,const GLbyte* v) = _choose_glVertexAttrib4NbvARB;
static void API_ENTRY _choose_glVertexAttrib4Niv(GLuint index,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4Niv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4Niv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4Niv = (void(API_ENTRY*)(GLuint,const GLint*))gotPtr;
		return _ptr_to_glVertexAttrib4Niv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4Niv)(GLuint index,const GLint* v) = _choose_glVertexAttrib4Niv;
static void API_ENTRY _choose_glVertexAttrib4NivARB(GLuint index,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4NivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4NivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4NivARB = (void(API_ENTRY*)(GLuint,const GLint*))gotPtr;
		return _ptr_to_glVertexAttrib4NivARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4NivARB)(GLuint index,const GLint* v) = _choose_glVertexAttrib4NivARB;
static void API_ENTRY _choose_glVertexAttrib4Nsv(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4Nsv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4Nsv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4Nsv = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib4Nsv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4Nsv)(GLuint index,const GLshort* v) = _choose_glVertexAttrib4Nsv;
static void API_ENTRY _choose_glVertexAttrib4NsvARB(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4NsvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4NsvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4NsvARB = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib4NsvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4NsvARB)(GLuint index,const GLshort* v) = _choose_glVertexAttrib4NsvARB;
static void API_ENTRY _choose_glVertexAttrib4Nub(GLuint index,GLubyte x,GLubyte y,GLubyte z,GLubyte w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4Nub");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4Nub is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4Nub = (void(API_ENTRY*)(GLuint,GLubyte,GLubyte,GLubyte,GLubyte))gotPtr;
		return _ptr_to_glVertexAttrib4Nub(index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4Nub)(GLuint index,GLubyte x,GLubyte y,GLubyte z,GLubyte w) = _choose_glVertexAttrib4Nub;
static void API_ENTRY _choose_glVertexAttrib4NubARB(GLuint index,GLubyte x,GLubyte y,GLubyte z,GLubyte w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4NubARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4NubARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4NubARB = (void(API_ENTRY*)(GLuint,GLubyte,GLubyte,GLubyte,GLubyte))gotPtr;
		return _ptr_to_glVertexAttrib4NubARB(index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4NubARB)(GLuint index,GLubyte x,GLubyte y,GLubyte z,GLubyte w) = _choose_glVertexAttrib4NubARB;
static void API_ENTRY _choose_glVertexAttrib4Nubv(GLuint index,const GLubyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4Nubv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4Nubv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4Nubv = (void(API_ENTRY*)(GLuint,const GLubyte*))gotPtr;
		return _ptr_to_glVertexAttrib4Nubv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4Nubv)(GLuint index,const GLubyte* v) = _choose_glVertexAttrib4Nubv;
static void API_ENTRY _choose_glVertexAttrib4NubvARB(GLuint index,const GLubyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4NubvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4NubvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4NubvARB = (void(API_ENTRY*)(GLuint,const GLubyte*))gotPtr;
		return _ptr_to_glVertexAttrib4NubvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4NubvARB)(GLuint index,const GLubyte* v) = _choose_glVertexAttrib4NubvARB;
static void API_ENTRY _choose_glVertexAttrib4Nuiv(GLuint index,const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4Nuiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4Nuiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4Nuiv = (void(API_ENTRY*)(GLuint,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttrib4Nuiv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4Nuiv)(GLuint index,const GLuint* v) = _choose_glVertexAttrib4Nuiv;
static void API_ENTRY _choose_glVertexAttrib4NuivARB(GLuint index,const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4NuivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4NuivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4NuivARB = (void(API_ENTRY*)(GLuint,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttrib4NuivARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4NuivARB)(GLuint index,const GLuint* v) = _choose_glVertexAttrib4NuivARB;
static void API_ENTRY _choose_glVertexAttrib4Nusv(GLuint index,const GLushort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4Nusv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4Nusv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4Nusv = (void(API_ENTRY*)(GLuint,const GLushort*))gotPtr;
		return _ptr_to_glVertexAttrib4Nusv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4Nusv)(GLuint index,const GLushort* v) = _choose_glVertexAttrib4Nusv;
static void API_ENTRY _choose_glVertexAttrib4NusvARB(GLuint index,const GLushort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4NusvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4NusvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4NusvARB = (void(API_ENTRY*)(GLuint,const GLushort*))gotPtr;
		return _ptr_to_glVertexAttrib4NusvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4NusvARB)(GLuint index,const GLushort* v) = _choose_glVertexAttrib4NusvARB;
static void API_ENTRY _choose_glVertexAttrib4bv(GLuint index,const GLbyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4bv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4bv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4bv = (void(API_ENTRY*)(GLuint,const GLbyte*))gotPtr;
		return _ptr_to_glVertexAttrib4bv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4bv)(GLuint index,const GLbyte* v) = _choose_glVertexAttrib4bv;
static void API_ENTRY _choose_glVertexAttrib4bvARB(GLuint index,const GLbyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4bvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4bvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4bvARB = (void(API_ENTRY*)(GLuint,const GLbyte*))gotPtr;
		return _ptr_to_glVertexAttrib4bvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4bvARB)(GLuint index,const GLbyte* v) = _choose_glVertexAttrib4bvARB;
static void API_ENTRY _choose_glVertexAttrib4d(GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4d = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertexAttrib4d(index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4d)(GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w) = _choose_glVertexAttrib4d;
static void API_ENTRY _choose_glVertexAttrib4dARB(GLuint index,GLdouble v0,GLdouble v1,GLdouble v2,GLdouble v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4dARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4dARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4dARB = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertexAttrib4dARB(index,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4dARB)(GLuint index,GLdouble v0,GLdouble v1,GLdouble v2,GLdouble v3) = _choose_glVertexAttrib4dARB;
static void API_ENTRY _choose_glVertexAttrib4dv(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4dv = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttrib4dv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4dv)(GLuint index,const GLdouble* v) = _choose_glVertexAttrib4dv;
static void API_ENTRY _choose_glVertexAttrib4dvARB(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4dvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4dvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4dvARB = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttrib4dvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4dvARB)(GLuint index,const GLdouble* v) = _choose_glVertexAttrib4dvARB;
static void API_ENTRY _choose_glVertexAttrib4f(GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4f is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4f = (void(API_ENTRY*)(GLuint,GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glVertexAttrib4f(index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4f)(GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w) = _choose_glVertexAttrib4f;
static void API_ENTRY _choose_glVertexAttrib4fARB(GLuint index,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4fARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4fARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4fARB = (void(API_ENTRY*)(GLuint,GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glVertexAttrib4fARB(index,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4fARB)(GLuint index,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3) = _choose_glVertexAttrib4fARB;
static void API_ENTRY _choose_glVertexAttrib4fv(GLuint index,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4fv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4fv = (void(API_ENTRY*)(GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glVertexAttrib4fv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4fv)(GLuint index,const GLfloat* v) = _choose_glVertexAttrib4fv;
static void API_ENTRY _choose_glVertexAttrib4fvARB(GLuint index,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4fvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4fvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4fvARB = (void(API_ENTRY*)(GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glVertexAttrib4fvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4fvARB)(GLuint index,const GLfloat* v) = _choose_glVertexAttrib4fvARB;
static void API_ENTRY _choose_glVertexAttrib4iv(GLuint index,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4iv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4iv = (void(API_ENTRY*)(GLuint,const GLint*))gotPtr;
		return _ptr_to_glVertexAttrib4iv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4iv)(GLuint index,const GLint* v) = _choose_glVertexAttrib4iv;
static void API_ENTRY _choose_glVertexAttrib4ivARB(GLuint index,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4ivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4ivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4ivARB = (void(API_ENTRY*)(GLuint,const GLint*))gotPtr;
		return _ptr_to_glVertexAttrib4ivARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4ivARB)(GLuint index,const GLint* v) = _choose_glVertexAttrib4ivARB;
static void API_ENTRY _choose_glVertexAttrib4s(GLuint index,GLshort x,GLshort y,GLshort z,GLshort w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4s is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4s = (void(API_ENTRY*)(GLuint,GLshort,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glVertexAttrib4s(index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4s)(GLuint index,GLshort x,GLshort y,GLshort z,GLshort w) = _choose_glVertexAttrib4s;
static void API_ENTRY _choose_glVertexAttrib4sARB(GLuint index,GLshort v0,GLshort v1,GLshort v2,GLshort v3)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4sARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4sARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4sARB = (void(API_ENTRY*)(GLuint,GLshort,GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glVertexAttrib4sARB(index,v0,v1,v2,v3);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4sARB)(GLuint index,GLshort v0,GLshort v1,GLshort v2,GLshort v3) = _choose_glVertexAttrib4sARB;
static void API_ENTRY _choose_glVertexAttrib4sv(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4sv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4sv = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib4sv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4sv)(GLuint index,const GLshort* v) = _choose_glVertexAttrib4sv;
static void API_ENTRY _choose_glVertexAttrib4svARB(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4svARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4svARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4svARB = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttrib4svARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4svARB)(GLuint index,const GLshort* v) = _choose_glVertexAttrib4svARB;
static void API_ENTRY _choose_glVertexAttrib4ubv(GLuint index,const GLubyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4ubv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4ubv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4ubv = (void(API_ENTRY*)(GLuint,const GLubyte*))gotPtr;
		return _ptr_to_glVertexAttrib4ubv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4ubv)(GLuint index,const GLubyte* v) = _choose_glVertexAttrib4ubv;
static void API_ENTRY _choose_glVertexAttrib4ubvARB(GLuint index,const GLubyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4ubvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4ubvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4ubvARB = (void(API_ENTRY*)(GLuint,const GLubyte*))gotPtr;
		return _ptr_to_glVertexAttrib4ubvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4ubvARB)(GLuint index,const GLubyte* v) = _choose_glVertexAttrib4ubvARB;
static void API_ENTRY _choose_glVertexAttrib4uiv(GLuint index,const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4uiv = (void(API_ENTRY*)(GLuint,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttrib4uiv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4uiv)(GLuint index,const GLuint* v) = _choose_glVertexAttrib4uiv;
static void API_ENTRY _choose_glVertexAttrib4uivARB(GLuint index,const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4uivARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4uivARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4uivARB = (void(API_ENTRY*)(GLuint,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttrib4uivARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4uivARB)(GLuint index,const GLuint* v) = _choose_glVertexAttrib4uivARB;
static void API_ENTRY _choose_glVertexAttrib4usv(GLuint index,const GLushort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4usv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4usv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4usv = (void(API_ENTRY*)(GLuint,const GLushort*))gotPtr;
		return _ptr_to_glVertexAttrib4usv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4usv)(GLuint index,const GLushort* v) = _choose_glVertexAttrib4usv;
static void API_ENTRY _choose_glVertexAttrib4usvARB(GLuint index,const GLushort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttrib4usvARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttrib4usvARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttrib4usvARB = (void(API_ENTRY*)(GLuint,const GLushort*))gotPtr;
		return _ptr_to_glVertexAttrib4usvARB(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttrib4usvARB)(GLuint index,const GLushort* v) = _choose_glVertexAttrib4usvARB;
static void API_ENTRY _choose_glVertexAttribDivisor(GLuint index,GLuint divisor)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribDivisor");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribDivisor is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribDivisor = (void(API_ENTRY*)(GLuint,GLuint))gotPtr;
		return _ptr_to_glVertexAttribDivisor(index,divisor);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribDivisor)(GLuint index,GLuint divisor) = _choose_glVertexAttribDivisor;
static void API_ENTRY _choose_glVertexAttribI1i(GLuint index,GLint x)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI1i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI1i is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI1i = (void(API_ENTRY*)(GLuint,GLint))gotPtr;
		return _ptr_to_glVertexAttribI1i(index,x);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI1i)(GLuint index,GLint x) = _choose_glVertexAttribI1i;
static void API_ENTRY _choose_glVertexAttribI1iv(GLuint index,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI1iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI1iv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI1iv = (void(API_ENTRY*)(GLuint,const GLint*))gotPtr;
		return _ptr_to_glVertexAttribI1iv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI1iv)(GLuint index,const GLint* v) = _choose_glVertexAttribI1iv;
static void API_ENTRY _choose_glVertexAttribI1ui(GLuint index,GLuint x)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI1ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI1ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI1ui = (void(API_ENTRY*)(GLuint,GLuint))gotPtr;
		return _ptr_to_glVertexAttribI1ui(index,x);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI1ui)(GLuint index,GLuint x) = _choose_glVertexAttribI1ui;
static void API_ENTRY _choose_glVertexAttribI1uiv(GLuint index,const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI1uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI1uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI1uiv = (void(API_ENTRY*)(GLuint,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttribI1uiv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI1uiv)(GLuint index,const GLuint* v) = _choose_glVertexAttribI1uiv;
static void API_ENTRY _choose_glVertexAttribI2i(GLuint index,GLint x,GLint y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI2i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI2i is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI2i = (void(API_ENTRY*)(GLuint,GLint,GLint))gotPtr;
		return _ptr_to_glVertexAttribI2i(index,x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI2i)(GLuint index,GLint x,GLint y) = _choose_glVertexAttribI2i;
static void API_ENTRY _choose_glVertexAttribI2iv(GLuint index,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI2iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI2iv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI2iv = (void(API_ENTRY*)(GLuint,const GLint*))gotPtr;
		return _ptr_to_glVertexAttribI2iv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI2iv)(GLuint index,const GLint* v) = _choose_glVertexAttribI2iv;
static void API_ENTRY _choose_glVertexAttribI2ui(GLuint index,GLuint x,GLuint y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI2ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI2ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI2ui = (void(API_ENTRY*)(GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glVertexAttribI2ui(index,x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI2ui)(GLuint index,GLuint x,GLuint y) = _choose_glVertexAttribI2ui;
static void API_ENTRY _choose_glVertexAttribI2uiv(GLuint index,const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI2uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI2uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI2uiv = (void(API_ENTRY*)(GLuint,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttribI2uiv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI2uiv)(GLuint index,const GLuint* v) = _choose_glVertexAttribI2uiv;
static void API_ENTRY _choose_glVertexAttribI3i(GLuint index,GLint x,GLint y,GLint z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI3i is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI3i = (void(API_ENTRY*)(GLuint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glVertexAttribI3i(index,x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI3i)(GLuint index,GLint x,GLint y,GLint z) = _choose_glVertexAttribI3i;
static void API_ENTRY _choose_glVertexAttribI3iv(GLuint index,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI3iv = (void(API_ENTRY*)(GLuint,const GLint*))gotPtr;
		return _ptr_to_glVertexAttribI3iv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI3iv)(GLuint index,const GLint* v) = _choose_glVertexAttribI3iv;
static void API_ENTRY _choose_glVertexAttribI3ui(GLuint index,GLuint x,GLuint y,GLuint z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI3ui = (void(API_ENTRY*)(GLuint,GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glVertexAttribI3ui(index,x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI3ui)(GLuint index,GLuint x,GLuint y,GLuint z) = _choose_glVertexAttribI3ui;
static void API_ENTRY _choose_glVertexAttribI3uiv(GLuint index,const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI3uiv = (void(API_ENTRY*)(GLuint,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttribI3uiv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI3uiv)(GLuint index,const GLuint* v) = _choose_glVertexAttribI3uiv;
static void API_ENTRY _choose_glVertexAttribI4bv(GLuint index,const GLbyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI4bv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI4bv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI4bv = (void(API_ENTRY*)(GLuint,const GLbyte*))gotPtr;
		return _ptr_to_glVertexAttribI4bv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI4bv)(GLuint index,const GLbyte* v) = _choose_glVertexAttribI4bv;
static void API_ENTRY _choose_glVertexAttribI4i(GLuint index,GLint x,GLint y,GLint z,GLint w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI4i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI4i is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI4i = (void(API_ENTRY*)(GLuint,GLint,GLint,GLint,GLint))gotPtr;
		return _ptr_to_glVertexAttribI4i(index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI4i)(GLuint index,GLint x,GLint y,GLint z,GLint w) = _choose_glVertexAttribI4i;
static void API_ENTRY _choose_glVertexAttribI4iv(GLuint index,const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI4iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI4iv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI4iv = (void(API_ENTRY*)(GLuint,const GLint*))gotPtr;
		return _ptr_to_glVertexAttribI4iv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI4iv)(GLuint index,const GLint* v) = _choose_glVertexAttribI4iv;
static void API_ENTRY _choose_glVertexAttribI4sv(GLuint index,const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI4sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI4sv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI4sv = (void(API_ENTRY*)(GLuint,const GLshort*))gotPtr;
		return _ptr_to_glVertexAttribI4sv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI4sv)(GLuint index,const GLshort* v) = _choose_glVertexAttribI4sv;
static void API_ENTRY _choose_glVertexAttribI4ubv(GLuint index,const GLubyte* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI4ubv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI4ubv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI4ubv = (void(API_ENTRY*)(GLuint,const GLubyte*))gotPtr;
		return _ptr_to_glVertexAttribI4ubv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI4ubv)(GLuint index,const GLubyte* v) = _choose_glVertexAttribI4ubv;
static void API_ENTRY _choose_glVertexAttribI4ui(GLuint index,GLuint x,GLuint y,GLuint z,GLuint w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI4ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI4ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI4ui = (void(API_ENTRY*)(GLuint,GLuint,GLuint,GLuint,GLuint))gotPtr;
		return _ptr_to_glVertexAttribI4ui(index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI4ui)(GLuint index,GLuint x,GLuint y,GLuint z,GLuint w) = _choose_glVertexAttribI4ui;
static void API_ENTRY _choose_glVertexAttribI4uiv(GLuint index,const GLuint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI4uiv = (void(API_ENTRY*)(GLuint,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttribI4uiv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI4uiv)(GLuint index,const GLuint* v) = _choose_glVertexAttribI4uiv;
static void API_ENTRY _choose_glVertexAttribI4usv(GLuint index,const GLushort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribI4usv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribI4usv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribI4usv = (void(API_ENTRY*)(GLuint,const GLushort*))gotPtr;
		return _ptr_to_glVertexAttribI4usv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribI4usv)(GLuint index,const GLushort* v) = _choose_glVertexAttribI4usv;
static void API_ENTRY _choose_glVertexAttribIPointer(GLuint index,GLint size,GLenum type,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribIPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribIPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribIPointer = (void(API_ENTRY*)(GLuint,GLint,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glVertexAttribIPointer(index,size,type,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribIPointer)(GLuint index,GLint size,GLenum type,GLsizei stride,const GLvoid* pointer) = _choose_glVertexAttribIPointer;
static void API_ENTRY _choose_glVertexAttribL1d(GLuint index,GLdouble x)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribL1d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribL1d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribL1d = (void(API_ENTRY*)(GLuint,GLdouble))gotPtr;
		return _ptr_to_glVertexAttribL1d(index,x);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribL1d)(GLuint index,GLdouble x) = _choose_glVertexAttribL1d;
static void API_ENTRY _choose_glVertexAttribL1dv(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribL1dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribL1dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribL1dv = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttribL1dv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribL1dv)(GLuint index,const GLdouble* v) = _choose_glVertexAttribL1dv;
static void API_ENTRY _choose_glVertexAttribL2d(GLuint index,GLdouble x,GLdouble y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribL2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribL2d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribL2d = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertexAttribL2d(index,x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribL2d)(GLuint index,GLdouble x,GLdouble y) = _choose_glVertexAttribL2d;
static void API_ENTRY _choose_glVertexAttribL2dv(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribL2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribL2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribL2dv = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttribL2dv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribL2dv)(GLuint index,const GLdouble* v) = _choose_glVertexAttribL2dv;
static void API_ENTRY _choose_glVertexAttribL3d(GLuint index,GLdouble x,GLdouble y,GLdouble z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribL3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribL3d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribL3d = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertexAttribL3d(index,x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribL3d)(GLuint index,GLdouble x,GLdouble y,GLdouble z) = _choose_glVertexAttribL3d;
static void API_ENTRY _choose_glVertexAttribL3dv(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribL3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribL3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribL3dv = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttribL3dv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribL3dv)(GLuint index,const GLdouble* v) = _choose_glVertexAttribL3dv;
static void API_ENTRY _choose_glVertexAttribL4d(GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribL4d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribL4d is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribL4d = (void(API_ENTRY*)(GLuint,GLdouble,GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glVertexAttribL4d(index,x,y,z,w);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribL4d)(GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w) = _choose_glVertexAttribL4d;
static void API_ENTRY _choose_glVertexAttribL4dv(GLuint index,const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribL4dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribL4dv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribL4dv = (void(API_ENTRY*)(GLuint,const GLdouble*))gotPtr;
		return _ptr_to_glVertexAttribL4dv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribL4dv)(GLuint index,const GLdouble* v) = _choose_glVertexAttribL4dv;
static void API_ENTRY _choose_glVertexAttribLPointer(GLuint index,GLint size,GLenum type,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribLPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribLPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribLPointer = (void(API_ENTRY*)(GLuint,GLint,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glVertexAttribLPointer(index,size,type,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribLPointer)(GLuint index,GLint size,GLenum type,GLsizei stride,const GLvoid* pointer) = _choose_glVertexAttribLPointer;
static void API_ENTRY _choose_glVertexAttribP1ui(GLuint index,GLenum type,GLboolean normalized,GLuint value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribP1ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribP1ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribP1ui = (void(API_ENTRY*)(GLuint,GLenum,GLboolean,GLuint))gotPtr;
		return _ptr_to_glVertexAttribP1ui(index,type,normalized,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribP1ui)(GLuint index,GLenum type,GLboolean normalized,GLuint value) = _choose_glVertexAttribP1ui;
static void API_ENTRY _choose_glVertexAttribP1uiv(GLuint index,GLenum type,GLboolean normalized,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribP1uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribP1uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribP1uiv = (void(API_ENTRY*)(GLuint,GLenum,GLboolean,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttribP1uiv(index,type,normalized,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribP1uiv)(GLuint index,GLenum type,GLboolean normalized,const GLuint* value) = _choose_glVertexAttribP1uiv;
static void API_ENTRY _choose_glVertexAttribP2ui(GLuint index,GLenum type,GLboolean normalized,GLuint value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribP2ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribP2ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribP2ui = (void(API_ENTRY*)(GLuint,GLenum,GLboolean,GLuint))gotPtr;
		return _ptr_to_glVertexAttribP2ui(index,type,normalized,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribP2ui)(GLuint index,GLenum type,GLboolean normalized,GLuint value) = _choose_glVertexAttribP2ui;
static void API_ENTRY _choose_glVertexAttribP2uiv(GLuint index,GLenum type,GLboolean normalized,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribP2uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribP2uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribP2uiv = (void(API_ENTRY*)(GLuint,GLenum,GLboolean,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttribP2uiv(index,type,normalized,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribP2uiv)(GLuint index,GLenum type,GLboolean normalized,const GLuint* value) = _choose_glVertexAttribP2uiv;
static void API_ENTRY _choose_glVertexAttribP3ui(GLuint index,GLenum type,GLboolean normalized,GLuint value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribP3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribP3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribP3ui = (void(API_ENTRY*)(GLuint,GLenum,GLboolean,GLuint))gotPtr;
		return _ptr_to_glVertexAttribP3ui(index,type,normalized,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribP3ui)(GLuint index,GLenum type,GLboolean normalized,GLuint value) = _choose_glVertexAttribP3ui;
static void API_ENTRY _choose_glVertexAttribP3uiv(GLuint index,GLenum type,GLboolean normalized,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribP3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribP3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribP3uiv = (void(API_ENTRY*)(GLuint,GLenum,GLboolean,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttribP3uiv(index,type,normalized,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribP3uiv)(GLuint index,GLenum type,GLboolean normalized,const GLuint* value) = _choose_glVertexAttribP3uiv;
static void API_ENTRY _choose_glVertexAttribP4ui(GLuint index,GLenum type,GLboolean normalized,GLuint value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribP4ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribP4ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribP4ui = (void(API_ENTRY*)(GLuint,GLenum,GLboolean,GLuint))gotPtr;
		return _ptr_to_glVertexAttribP4ui(index,type,normalized,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribP4ui)(GLuint index,GLenum type,GLboolean normalized,GLuint value) = _choose_glVertexAttribP4ui;
static void API_ENTRY _choose_glVertexAttribP4uiv(GLuint index,GLenum type,GLboolean normalized,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribP4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribP4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribP4uiv = (void(API_ENTRY*)(GLuint,GLenum,GLboolean,const GLuint*))gotPtr;
		return _ptr_to_glVertexAttribP4uiv(index,type,normalized,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribP4uiv)(GLuint index,GLenum type,GLboolean normalized,const GLuint* value) = _choose_glVertexAttribP4uiv;
static void API_ENTRY _choose_glVertexAttribPointer(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribPointer = (void(API_ENTRY*)(GLuint,GLint,GLenum,GLboolean,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glVertexAttribPointer(index,size,type,normalized,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribPointer)(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const GLvoid* pointer) = _choose_glVertexAttribPointer;
static void API_ENTRY _choose_glVertexAttribPointerARB(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexAttribPointerARB");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexAttribPointerARB is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexAttribPointerARB = (void(API_ENTRY*)(GLuint,GLint,GLenum,GLboolean,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glVertexAttribPointerARB(index,size,type,normalized,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexAttribPointerARB)(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const GLvoid* pointer) = _choose_glVertexAttribPointerARB;
static void API_ENTRY _choose_glVertexP2ui(GLenum type,GLuint value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexP2ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexP2ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexP2ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glVertexP2ui(type,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexP2ui)(GLenum type,GLuint value) = _choose_glVertexP2ui;
static void API_ENTRY _choose_glVertexP2uiv(GLenum type,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexP2uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexP2uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexP2uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glVertexP2uiv(type,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexP2uiv)(GLenum type,const GLuint* value) = _choose_glVertexP2uiv;
static void API_ENTRY _choose_glVertexP3ui(GLenum type,GLuint value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexP3ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexP3ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexP3ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glVertexP3ui(type,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexP3ui)(GLenum type,GLuint value) = _choose_glVertexP3ui;
static void API_ENTRY _choose_glVertexP3uiv(GLenum type,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexP3uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexP3uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexP3uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glVertexP3uiv(type,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexP3uiv)(GLenum type,const GLuint* value) = _choose_glVertexP3uiv;
static void API_ENTRY _choose_glVertexP4ui(GLenum type,GLuint value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexP4ui");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexP4ui is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexP4ui = (void(API_ENTRY*)(GLenum,GLuint))gotPtr;
		return _ptr_to_glVertexP4ui(type,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexP4ui)(GLenum type,GLuint value) = _choose_glVertexP4ui;
static void API_ENTRY _choose_glVertexP4uiv(GLenum type,const GLuint* value)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexP4uiv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexP4uiv is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexP4uiv = (void(API_ENTRY*)(GLenum,const GLuint*))gotPtr;
		return _ptr_to_glVertexP4uiv(type,value);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexP4uiv)(GLenum type,const GLuint* value) = _choose_glVertexP4uiv;
static void API_ENTRY _choose_glVertexPointer(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glVertexPointer");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glVertexPointer is NULL " << std::endl;
	else
	{
		_ptr_to_glVertexPointer = (void(API_ENTRY*)(GLint,GLenum,GLsizei,const GLvoid*))gotPtr;
		return _ptr_to_glVertexPointer(size,type,stride,pointer);
	}
	return;
}
void (API_ENTRY *_ptr_to_glVertexPointer)(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer) = _choose_glVertexPointer;
static void API_ENTRY _choose_glViewport(GLint x,GLint y,GLsizei width,GLsizei height)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glViewport");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glViewport is NULL " << std::endl;
	else
	{
		_ptr_to_glViewport = (void(API_ENTRY*)(GLint,GLint,GLsizei,GLsizei))gotPtr;
		return _ptr_to_glViewport(x,y,width,height);
	}
	return;
}
void (API_ENTRY *_ptr_to_glViewport)(GLint x,GLint y,GLsizei width,GLsizei height) = _choose_glViewport;
static void API_ENTRY _choose_glViewportArrayv(GLuint first,GLsizei count,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glViewportArrayv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glViewportArrayv is NULL " << std::endl;
	else
	{
		_ptr_to_glViewportArrayv = (void(API_ENTRY*)(GLuint,GLsizei,const GLfloat*))gotPtr;
		return _ptr_to_glViewportArrayv(first,count,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glViewportArrayv)(GLuint first,GLsizei count,const GLfloat* v) = _choose_glViewportArrayv;
static void API_ENTRY _choose_glViewportIndexedf(GLuint index,GLfloat x,GLfloat y,GLfloat w,GLfloat h)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glViewportIndexedf");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glViewportIndexedf is NULL " << std::endl;
	else
	{
		_ptr_to_glViewportIndexedf = (void(API_ENTRY*)(GLuint,GLfloat,GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glViewportIndexedf(index,x,y,w,h);
	}
	return;
}
void (API_ENTRY *_ptr_to_glViewportIndexedf)(GLuint index,GLfloat x,GLfloat y,GLfloat w,GLfloat h) = _choose_glViewportIndexedf;
static void API_ENTRY _choose_glViewportIndexedfv(GLuint index,const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glViewportIndexedfv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glViewportIndexedfv is NULL " << std::endl;
	else
	{
		_ptr_to_glViewportIndexedfv = (void(API_ENTRY*)(GLuint,const GLfloat*))gotPtr;
		return _ptr_to_glViewportIndexedfv(index,v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glViewportIndexedfv)(GLuint index,const GLfloat* v) = _choose_glViewportIndexedfv;
static void API_ENTRY _choose_glWaitSync(GLsync sync,GLbitfield flags,GLuint64 timeout)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWaitSync");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWaitSync is NULL " << std::endl;
	else
	{
		_ptr_to_glWaitSync = (void(API_ENTRY*)(GLsync,GLbitfield,GLuint64))gotPtr;
		return _ptr_to_glWaitSync(sync,flags,timeout);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWaitSync)(GLsync sync,GLbitfield flags,GLuint64 timeout) = _choose_glWaitSync;
static void API_ENTRY _choose_glWindowPos2d(GLdouble x,GLdouble y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos2d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos2d is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos2d = (void(API_ENTRY*)(GLdouble,GLdouble))gotPtr;
		return _ptr_to_glWindowPos2d(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos2d)(GLdouble x,GLdouble y) = _choose_glWindowPos2d;
static void API_ENTRY _choose_glWindowPos2dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos2dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos2dv is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos2dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glWindowPos2dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos2dv)(const GLdouble* v) = _choose_glWindowPos2dv;
static void API_ENTRY _choose_glWindowPos2f(GLfloat x,GLfloat y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos2f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos2f is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos2f = (void(API_ENTRY*)(GLfloat,GLfloat))gotPtr;
		return _ptr_to_glWindowPos2f(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos2f)(GLfloat x,GLfloat y) = _choose_glWindowPos2f;
static void API_ENTRY _choose_glWindowPos2fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos2fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos2fv is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos2fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glWindowPos2fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos2fv)(const GLfloat* v) = _choose_glWindowPos2fv;
static void API_ENTRY _choose_glWindowPos2i(GLint x,GLint y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos2i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos2i is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos2i = (void(API_ENTRY*)(GLint,GLint))gotPtr;
		return _ptr_to_glWindowPos2i(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos2i)(GLint x,GLint y) = _choose_glWindowPos2i;
static void API_ENTRY _choose_glWindowPos2iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos2iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos2iv is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos2iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glWindowPos2iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos2iv)(const GLint* v) = _choose_glWindowPos2iv;
static void API_ENTRY _choose_glWindowPos2s(GLshort x,GLshort y)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos2s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos2s is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos2s = (void(API_ENTRY*)(GLshort,GLshort))gotPtr;
		return _ptr_to_glWindowPos2s(x,y);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos2s)(GLshort x,GLshort y) = _choose_glWindowPos2s;
static void API_ENTRY _choose_glWindowPos2sv(const GLshort* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos2sv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos2sv is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos2sv = (void(API_ENTRY*)(const GLshort*))gotPtr;
		return _ptr_to_glWindowPos2sv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos2sv)(const GLshort* v) = _choose_glWindowPos2sv;
static void API_ENTRY _choose_glWindowPos3d(GLdouble x,GLdouble y,GLdouble z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos3d");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos3d is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos3d = (void(API_ENTRY*)(GLdouble,GLdouble,GLdouble))gotPtr;
		return _ptr_to_glWindowPos3d(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos3d)(GLdouble x,GLdouble y,GLdouble z) = _choose_glWindowPos3d;
static void API_ENTRY _choose_glWindowPos3dv(const GLdouble* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos3dv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos3dv is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos3dv = (void(API_ENTRY*)(const GLdouble*))gotPtr;
		return _ptr_to_glWindowPos3dv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos3dv)(const GLdouble* v) = _choose_glWindowPos3dv;
static void API_ENTRY _choose_glWindowPos3f(GLfloat x,GLfloat y,GLfloat z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos3f");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos3f is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos3f = (void(API_ENTRY*)(GLfloat,GLfloat,GLfloat))gotPtr;
		return _ptr_to_glWindowPos3f(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos3f)(GLfloat x,GLfloat y,GLfloat z) = _choose_glWindowPos3f;
static void API_ENTRY _choose_glWindowPos3fv(const GLfloat* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos3fv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos3fv is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos3fv = (void(API_ENTRY*)(const GLfloat*))gotPtr;
		return _ptr_to_glWindowPos3fv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos3fv)(const GLfloat* v) = _choose_glWindowPos3fv;
static void API_ENTRY _choose_glWindowPos3i(GLint x,GLint y,GLint z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos3i");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos3i is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos3i = (void(API_ENTRY*)(GLint,GLint,GLint))gotPtr;
		return _ptr_to_glWindowPos3i(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos3i)(GLint x,GLint y,GLint z) = _choose_glWindowPos3i;
static void API_ENTRY _choose_glWindowPos3iv(const GLint* v)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos3iv");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos3iv is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos3iv = (void(API_ENTRY*)(const GLint*))gotPtr;
		return _ptr_to_glWindowPos3iv(v);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos3iv)(const GLint* v) = _choose_glWindowPos3iv;
static void API_ENTRY _choose_glWindowPos3s(GLshort x,GLshort y,GLshort z)
{
	void (*gotPtr)() = (void(*)())retrieveProcAddress("glWindowPos3s");
	if (!gotPtr)
		fg::OpenGL_log << "Error: glWindowPos3s is NULL " << std::endl;
	else
	{
		_ptr_to_glWindowPos3s = (void(API_ENTRY*)(GLshort,GLshort,GLshort))gotPtr;
		return _ptr_to_glWindowPos3s(x,y,z);
	}
	return;
}
void (API_ENTRY *_ptr_to_glWindowPos3s)(GLshort x,GLshort y,GLshort z) = _choose_glWindowPos3s;
