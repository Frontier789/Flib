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
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/GL/GL_API_ENTRY.h>
#include <FRONTIER/System/util/OS.h>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	void printNULL(const char *funcName)
	{
		fg::GL::raiseGLError(fm::Result("GLError",fm::Result::OPFailed,"MissingFunc",funcName,__FILE__,__LINE__,fm::String(funcName).str() + " couldn't be loaded from shared object"));
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
											   NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR) : nullptr;
		
		return symbol ? NSAddressOfSymbol(symbol) : nullptr;
	}
	
	#define FRONTIER_HAS_SO_LOADER 1
#endif // macOS

#if defined(FRONTIER_OS_SOLARIS) || defined(FRONTIER_OS_IRIX)
	#include <dlfcn.h>
	#include <stdio.h>
	
	priv::SO_LOADER::SO_LOADER()
	{
		p1 = dlopen(nullptr, RTLD_LAZY | RTLD_LOCAL);
		
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
	}
	
	priv::SO_LOADER::~SO_LOADER()
	{
		if (p1) dlclose(p1);
	}
	
	void (*priv::SO_LOADER::getProcAddr(const std::string &name))()
	{
		if (!p1) return nullptr;
		
		void *symbol = dlsym(p1, name.c_str());
		if (!symbol) 
		{
			// append an underscore for platforms that need that.
			std::string name2 = '_'+name;
			
			symbol = dlsym(p1, name2.c_str());
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
	#ifndef FRONTIER_MISSING_IMPL_NO_WARNING
		#warning No retrieveProcAddress implementation!
	#endif
	
	priv::SO_LOADER::SO_LOADER()
	{
		
	}
	
	priv::SO_LOADER::~SO_LOADER()
	{
		
	}
	
	void (*priv::SO_LOADER::getProcAddr(const std::string &name))()
	{
		return nullptr;
	}
#endif // Unsupported

#define retrieveProcAddress(name) ::priv::so_loader.getProcAddr(name)

namespace priv
{
	SO_LOADER so_loader;
}

namespace fg
{
	namespace priv
	{
		void (API_ENTRY *gl_func_array[1003])();
		const char *gl_func_names[1003] = {
"glAccum","glActiveShaderProgram","glActiveTexture","glAlphaFunc","glAreTexturesResident","glArrayElement","glAttachObjectARB","glAttachShader","glBegin","glBeginConditionalRender",
"glBeginQuery","glBeginQueryIndexed","glBeginTransformFeedback","glBindAttribLocation","glBindAttribLocationARB","glBindBuffer","glBindBufferARB","glBindBufferBase","glBindBufferRange","glBindFragDataLocation",
"glBindFragDataLocationIndexed","glBindFramebuffer","glBindFramebufferEXT","glBindImageTexture","glBindProgramARB","glBindProgramPipeline","glBindRenderbuffer","glBindRenderbufferEXT","glBindSampler","glBindTexture",
"glBindTransformFeedback","glBindVertexArray","glBitmap","glBlendColor","glBlendEquation","glBlendEquationSeparate","glBlendEquationSeparatei","glBlendEquationi","glBlendFunc","glBlendFuncSeparate",
"glBlendFuncSeparatei","glBlendFunci","glBlitFramebuffer","glBufferData","glBufferDataARB","glBufferSubData","glBufferSubDataARB","glCallList","glCallLists","glCheckFramebufferStatus",
"glCheckFramebufferStatusEXT","glClampColor","glClear","glClearAccum","glClearBufferfi","glClearBufferfv","glClearBufferiv","glClearBufferuiv","glClearColor","glClearDepth",
"glClearDepthf","glClearIndex","glClearStencil","glClientActiveTexture","glClientWaitSync","glClipPlane","glColor3b","glColor3bv","glColor3d","glColor3dv",
"glColor3f","glColor3fv","glColor3i","glColor3iv","glColor3s","glColor3sv","glColor3ub","glColor3ubv","glColor3ui","glColor3uiv",
"glColor3us","glColor3usv","glColor4b","glColor4bv","glColor4d","glColor4dv","glColor4f","glColor4fv","glColor4i","glColor4iv",
"glColor4s","glColor4sv","glColor4ub","glColor4ubv","glColor4ui","glColor4uiv","glColor4us","glColor4usv","glColorMask","glColorMaski",
"glColorMaterial","glColorP3ui","glColorP3uiv","glColorP4ui","glColorP4uiv","glColorPointer","glCompileShader","glCompileShaderARB","glCompressedTexImage1D","glCompressedTexImage2D",
"glCompressedTexImage3D","glCompressedTexSubImage1D","glCompressedTexSubImage2D","glCompressedTexSubImage3D","glCopyBufferSubData","glCopyPixels","glCopyTexImage1D","glCopyTexImage2D","glCopyTexSubImage1D","glCopyTexSubImage2D",
"glCopyTexSubImage3D","glCreateProgram","glCreateProgramObjectARB","glCreateShader","glCreateShaderObjectARB","glCreateShaderProgramv","glCullFace","glDeleteBuffers","glDeleteBuffersARB","glDeleteFramebuffers",
"glDeleteFramebuffersEXT","glDeleteLists","glDeleteObjectARB","glDeleteProgram","glDeleteProgramPipelines","glDeleteProgramsARB","glDeleteQueries","glDeleteRenderbuffers","glDeleteRenderbuffersEXT","glDeleteSamplers",
"glDeleteShader","glDeleteSync","glDeleteTextures","glDeleteTransformFeedbacks","glDeleteVertexArrays","glDepthFunc","glDepthMask","glDepthRange","glDepthRangeArrayv","glDepthRangeIndexed",
"glDepthRangef","glDetachObjectARB","glDetachShader","glDisable","glDisableClientState","glDisableVertexAttribArray","glDisableVertexAttribArrayARB","glDisablei","glDispatchCompute","glDispatchComputeIndirect",
"glDrawArrays","glDrawArraysIndirect","glDrawArraysInstanced","glDrawArraysInstancedBaseInstance","glDrawBuffer","glDrawBuffers","glDrawElements","glDrawElementsBaseVertex","glDrawElementsIndirect","glDrawElementsInstanced",
"glDrawElementsInstancedBaseInstance","glDrawElementsInstancedBaseVertex","glDrawElementsInstancedBaseVertexBaseInstance","glDrawPixels","glDrawRangeElements","glDrawRangeElementsBaseVertex","glDrawTransformFeedback","glDrawTransformFeedbackInstanced","glDrawTransformFeedbackStream","glDrawTransformFeedbackStreamInstanced",
"glEdgeFlag","glEdgeFlagPointer","glEdgeFlagv","glEnable","glEnableClientState","glEnableVertexAttribArray","glEnableVertexAttribArrayARB","glEnablei","glEnd","glEndConditionalRender",
"glEndList","glEndQuery","glEndQueryIndexed","glEndTransformFeedback","glEvalCoord1d","glEvalCoord1dv","glEvalCoord1f","glEvalCoord1fv","glEvalCoord2d","glEvalCoord2dv",
"glEvalCoord2f","glEvalCoord2fv","glEvalMesh1","glEvalMesh2","glEvalPoint1","glEvalPoint2","glFeedbackBuffer","glFenceSync","glFinish","glFlush",
"glFlushMappedBufferRange","glFogCoordPointer","glFogCoordd","glFogCoorddv","glFogCoordf","glFogCoordfv","glFogf","glFogfv","glFogi","glFogiv",
"glFramebufferRenderbuffer","glFramebufferRenderbufferEXT","glFramebufferTexture","glFramebufferTexture1D","glFramebufferTexture1DEXT","glFramebufferTexture2D","glFramebufferTexture2DEXT","glFramebufferTexture3D","glFramebufferTexture3DEXT","glFramebufferTextureLayer",
"glFrontFace","glFrustum","glGenBuffers","glGenBuffersARB","glGenFramebuffers","glGenFramebuffersEXT","glGenLists","glGenProgramPipelines","glGenProgramsARB","glGenQueries",
"glGenRenderbuffers","glGenRenderbuffersEXT","glGenSamplers","glGenTextures","glGenTransformFeedbacks","glGenVertexArrays","glGenerateMipmap","glGenerateMipmapEXT","glGetActiveAtomicCounterBufferiv","glGetActiveAttrib",
"glGetActiveAttribARB","glGetActiveSubroutineName","glGetActiveSubroutineUniformName","glGetActiveSubroutineUniformiv","glGetActiveUniform","glGetActiveUniformARB","glGetActiveUniformBlockName","glGetActiveUniformBlockiv","glGetActiveUniformName","glGetActiveUniformsiv",
"glGetAttachedObjectsARB","glGetAttachedShaders","glGetAttribLocation","glGetAttribLocationARB","glGetBooleani_v","glGetBooleanv","glGetBufferParameteri64v","glGetBufferParameteriv","glGetBufferParameterivARB","glGetBufferPointerv",
"glGetBufferPointervARB","glGetBufferSubData","glGetBufferSubDataARB","glGetClipPlane","glGetCompressedTexImage","glGetDoublei_v","glGetDoublev","glGetError","glGetFloati_v","glGetFloatv",
"glGetFragDataIndex","glGetFragDataLocation","glGetFramebufferAttachmentParameteriv","glGetFramebufferAttachmentParameterivEXT","glGetHandleARB","glGetInfoLogARB","glGetInteger64i_v","glGetInteger64v","glGetIntegeri_v","glGetIntegerv",
"glGetInternalformati64v","glGetLightfv","glGetLightiv","glGetMapdv","glGetMapfv","glGetMapiv","glGetMaterialfv","glGetMaterialiv","glGetMultisamplefv","glGetObjectParameterfvARB",
"glGetObjectParameterivARB","glGetPixelMapfv","glGetPixelMapuiv","glGetPixelMapusv","glGetPointerv","glGetPolygonStipple","glGetProgramBinary","glGetProgramEnvParameterdvARB","glGetProgramEnvParameterfvARB","glGetProgramInfoLog",
"glGetProgramLocalParameterdvARB","glGetProgramLocalParameterfvARB","glGetProgramPipelineInfoLog","glGetProgramPipelineiv","glGetProgramStageiv","glGetProgramStringARB","glGetProgramiv","glGetProgramivARB","glGetQueryIndexediv","glGetQueryObjecti64v",
"glGetQueryObjectiv","glGetQueryObjectui64v","glGetQueryObjectuiv","glGetQueryiv","glGetRenderbufferParameteriv","glGetRenderbufferParameterivEXT","glGetSamplerParameterIiv","glGetSamplerParameterIuiv","glGetSamplerParameterfv","glGetSamplerParameteriv",
"glGetShaderInfoLog","glGetShaderPrecisionFormat","glGetShaderSource","glGetShaderSourceARB","glGetShaderiv","glGetString","glGetStringi","glGetSubroutineIndex","glGetSubroutineUniformLocation","glGetSynciv",
"glGetTexEnvfv","glGetTexEnviv","glGetTexGendv","glGetTexGenfv","glGetTexGeniv","glGetTexImage","glGetTexLevelParameterfv","glGetTexLevelParameteriv","glGetTexParameterIiv","glGetTexParameterIuiv",
"glGetTexParameterfv","glGetTexParameteriv","glGetTransformFeedbackVarying","glGetUniformBlockIndex","glGetUniformIndices","glGetUniformLocation","glGetUniformLocationARB","glGetUniformSubroutineuiv","glGetUniformdv","glGetUniformfv",
"glGetUniformfvARB","glGetUniformiv","glGetUniformivARB","glGetUniformuiv","glGetVertexAttribIiv","glGetVertexAttribIuiv","glGetVertexAttribLdv","glGetVertexAttribPointerv","glGetVertexAttribPointervARB","glGetVertexAttribdv",
"glGetVertexAttribdvARB","glGetVertexAttribfv","glGetVertexAttribfvARB","glGetVertexAttribiv","glGetVertexAttribivARB","glHint","glIndexMask","glIndexPointer","glIndexd","glIndexdv",
"glIndexf","glIndexfv","glIndexi","glIndexiv","glIndexs","glIndexsv","glIndexub","glIndexubv","glInitNames","glInterleavedArrays",
"glIsBuffer","glIsBufferARB","glIsEnabled","glIsEnabledi","glIsFramebuffer","glIsFramebufferEXT","glIsList","glIsProgram","glIsProgramARB","glIsProgramPipeline",
"glIsQuery","glIsRenderbuffer","glIsRenderbufferEXT","glIsSampler","glIsShader","glIsSync","glIsTexture","glIsTransformFeedback","glIsVertexArray","glLightModelf",
"glLightModelfv","glLightModeli","glLightModeliv","glLightf","glLightfv","glLighti","glLightiv","glLineStipple","glLineWidth","glLinkProgram",
"glLinkProgramARB","glListBase","glLoadIdentity","glLoadMatrixd","glLoadMatrixf","glLoadName","glLoadTransposeMatrixd","glLoadTransposeMatrixf","glLogicOp","glMap1d",
"glMap1f","glMap2d","glMap2f","glMapBuffer","glMapBufferARB","glMapBufferRange","glMapGrid1d","glMapGrid1f","glMapGrid2d","glMapGrid2f",
"glMaterialf","glMaterialfv","glMateriali","glMaterialiv","glMatrixMode","glMemoryBarrier","glMinSampleShading","glMultMatrixd","glMultMatrixf","glMultTransposeMatrixd",
"glMultTransposeMatrixf","glMultiDrawArrays","glMultiDrawElements","glMultiDrawElementsBaseVertex","glMultiTexCoord1d","glMultiTexCoord1dv","glMultiTexCoord1f","glMultiTexCoord1fv","glMultiTexCoord1i","glMultiTexCoord1iv",
"glMultiTexCoord1s","glMultiTexCoord1sv","glMultiTexCoord2d","glMultiTexCoord2dv","glMultiTexCoord2f","glMultiTexCoord2fv","glMultiTexCoord2i","glMultiTexCoord2iv","glMultiTexCoord2s","glMultiTexCoord2sv",
"glMultiTexCoord3d","glMultiTexCoord3dv","glMultiTexCoord3f","glMultiTexCoord3fv","glMultiTexCoord3i","glMultiTexCoord3iv","glMultiTexCoord3s","glMultiTexCoord3sv","glMultiTexCoord4d","glMultiTexCoord4dv",
"glMultiTexCoord4f","glMultiTexCoord4fv","glMultiTexCoord4i","glMultiTexCoord4iv","glMultiTexCoord4s","glMultiTexCoord4sv","glMultiTexCoordP1ui","glMultiTexCoordP1uiv","glMultiTexCoordP2ui","glMultiTexCoordP2uiv",
"glMultiTexCoordP3ui","glMultiTexCoordP3uiv","glMultiTexCoordP4ui","glMultiTexCoordP4uiv","glNewList","glNormal3b","glNormal3bv","glNormal3d","glNormal3dv","glNormal3f",
"glNormal3fv","glNormal3i","glNormal3iv","glNormal3s","glNormal3sv","glNormalP3ui","glNormalP3uiv","glNormalPointer","glOrtho","glPassThrough",
"glPatchParameterfv","glPatchParameteri","glPauseTransformFeedback","glPixelMapfv","glPixelMapuiv","glPixelMapusv","glPixelStoref","glPixelStorei","glPixelTransferf","glPixelTransferi",
"glPixelZoom","glPointParameterf","glPointParameterfv","glPointParameteri","glPointParameteriv","glPointSize","glPolygonMode","glPolygonOffset","glPolygonStipple","glPopAttrib",
"glPopClientAttrib","glPopMatrix","glPopName","glPrimitiveRestartIndex","glPrioritizeTextures","glProgramBinary","glProgramEnvParameter4dARB","glProgramEnvParameter4dvARB","glProgramEnvParameter4fARB","glProgramEnvParameter4fvARB",
"glProgramLocalParameter4dARB","glProgramLocalParameter4dvARB","glProgramLocalParameter4fARB","glProgramLocalParameter4fvARB","glProgramParameteri","glProgramStringARB","glProgramUniform1d","glProgramUniform1dv","glProgramUniform1f","glProgramUniform1fv",
"glProgramUniform1i","glProgramUniform1iv","glProgramUniform1ui","glProgramUniform1uiv","glProgramUniform2d","glProgramUniform2dv","glProgramUniform2f","glProgramUniform2fv","glProgramUniform2i","glProgramUniform2iv",
"glProgramUniform2ui","glProgramUniform2uiv","glProgramUniform3d","glProgramUniform3dv","glProgramUniform3f","glProgramUniform3fv","glProgramUniform3i","glProgramUniform3iv","glProgramUniform3ui","glProgramUniform3uiv",
"glProgramUniform4d","glProgramUniform4dv","glProgramUniform4f","glProgramUniform4fv","glProgramUniform4i","glProgramUniform4iv","glProgramUniform4ui","glProgramUniform4uiv","glProgramUniformMatrix2dv","glProgramUniformMatrix2fv",
"glProgramUniformMatrix2x3dv","glProgramUniformMatrix2x3fv","glProgramUniformMatrix2x4dv","glProgramUniformMatrix2x4fv","glProgramUniformMatrix3dv","glProgramUniformMatrix3fv","glProgramUniformMatrix3x2dv","glProgramUniformMatrix3x2fv","glProgramUniformMatrix3x4dv","glProgramUniformMatrix3x4fv",
"glProgramUniformMatrix4dv","glProgramUniformMatrix4fv","glProgramUniformMatrix4x2dv","glProgramUniformMatrix4x2fv","glProgramUniformMatrix4x3dv","glProgramUniformMatrix4x3fv","glProvokingVertex","glPushAttrib","glPushClientAttrib","glPushMatrix",
"glPushName","glQueryCounter","glRasterPos2d","glRasterPos2dv","glRasterPos2f","glRasterPos2fv","glRasterPos2i","glRasterPos2iv","glRasterPos2s","glRasterPos2sv",
"glRasterPos3d","glRasterPos3dv","glRasterPos3f","glRasterPos3fv","glRasterPos3i","glRasterPos3iv","glRasterPos3s","glRasterPos3sv","glRasterPos4d","glRasterPos4dv",
"glRasterPos4f","glRasterPos4fv","glRasterPos4i","glRasterPos4iv","glRasterPos4s","glRasterPos4sv","glReadBuffer","glReadPixels","glRectd","glRectdv",
"glRectf","glRectfv","glRecti","glRectiv","glRects","glRectsv","glReleaseShaderCompiler","glRenderMode","glRenderbufferStorage","glRenderbufferStorageEXT",
"glRenderbufferStorageMultisample","glResumeTransformFeedback","glRotated","glRotatef","glSampleCoverage","glSampleMaski","glSamplerParameterIiv","glSamplerParameterIuiv","glSamplerParameterf","glSamplerParameterfv",
"glSamplerParameteri","glSamplerParameteriv","glScaled","glScalef","glScissor","glScissorArrayv","glScissorIndexed","glScissorIndexedv","glSecondaryColor3b","glSecondaryColor3bv",
"glSecondaryColor3d","glSecondaryColor3dv","glSecondaryColor3f","glSecondaryColor3fv","glSecondaryColor3i","glSecondaryColor3iv","glSecondaryColor3s","glSecondaryColor3sv","glSecondaryColor3ub","glSecondaryColor3ubv",
"glSecondaryColor3ui","glSecondaryColor3uiv","glSecondaryColor3us","glSecondaryColor3usv","glSecondaryColorP3ui","glSecondaryColorP3uiv","glSecondaryColorPointer","glSelectBuffer","glShadeModel","glShaderBinary",
"glShaderSource","glShaderSourceARB","glStencilFunc","glStencilFuncSeparate","glStencilMask","glStencilMaskSeparate","glStencilOp","glStencilOpSeparate","glTexBuffer","glTexCoord1d",
"glTexCoord1dv","glTexCoord1f","glTexCoord1fv","glTexCoord1i","glTexCoord1iv","glTexCoord1s","glTexCoord1sv","glTexCoord2d","glTexCoord2dv","glTexCoord2f",
"glTexCoord2fv","glTexCoord2i","glTexCoord2iv","glTexCoord2s","glTexCoord2sv","glTexCoord3d","glTexCoord3dv","glTexCoord3f","glTexCoord3fv","glTexCoord3i",
"glTexCoord3iv","glTexCoord3s","glTexCoord3sv","glTexCoord4d","glTexCoord4dv","glTexCoord4f","glTexCoord4fv","glTexCoord4i","glTexCoord4iv","glTexCoord4s",
"glTexCoord4sv","glTexCoordP1ui","glTexCoordP1uiv","glTexCoordP2ui","glTexCoordP2uiv","glTexCoordP3ui","glTexCoordP3uiv","glTexCoordP4ui","glTexCoordP4uiv","glTexCoordPointer",
"glTexEnvf","glTexEnvfv","glTexEnvi","glTexEnviv","glTexGend","glTexGendv","glTexGenf","glTexGenfv","glTexGeni","glTexGeniv",
"glTexImage1D","glTexImage2D","glTexImage2DMultisample","glTexImage3D","glTexImage3DMultisample","glTexParameterIiv","glTexParameterIuiv","glTexParameterf","glTexParameterfv","glTexParameteri",
"glTexParameteriv","glTexStorage1D","glTexStorage2D","glTexStorage3D","glTexSubImage1D","glTexSubImage2D","glTexSubImage3D","glTransformFeedbackVaryings","glTranslated","glTranslatef",
"glUniform1d","glUniform1dv","glUniform1f","glUniform1fARB","glUniform1fv","glUniform1fvARB","glUniform1i","glUniform1iARB","glUniform1iv","glUniform1ivARB",
"glUniform1ui","glUniform1uiv","glUniform2d","glUniform2dv","glUniform2f","glUniform2fARB","glUniform2fv","glUniform2fvARB","glUniform2i","glUniform2iARB",
"glUniform2iv","glUniform2ivARB","glUniform2ui","glUniform2uiv","glUniform3d","glUniform3dv","glUniform3f","glUniform3fARB","glUniform3fv","glUniform3fvARB",
"glUniform3i","glUniform3iARB","glUniform3iv","glUniform3ivARB","glUniform3ui","glUniform3uiv","glUniform4d","glUniform4dv","glUniform4f","glUniform4fARB",
"glUniform4fv","glUniform4fvARB","glUniform4i","glUniform4iARB","glUniform4iv","glUniform4ivARB","glUniform4ui","glUniform4uiv","glUniformBlockBinding","glUniformMatrix2dv",
"glUniformMatrix2fv","glUniformMatrix2fvARB","glUniformMatrix2x3dv","glUniformMatrix2x3fv","glUniformMatrix2x4dv","glUniformMatrix2x4fv","glUniformMatrix3dv","glUniformMatrix3fv","glUniformMatrix3fvARB","glUniformMatrix3x2dv",
"glUniformMatrix3x2fv","glUniformMatrix3x4dv","glUniformMatrix3x4fv","glUniformMatrix4dv","glUniformMatrix4fv","glUniformMatrix4fvARB","glUniformMatrix4x2dv","glUniformMatrix4x2fv","glUniformMatrix4x3dv","glUniformMatrix4x3fv",
"glUniformSubroutinesuiv","glUnmapBuffer","glUnmapBufferARB","glUseProgram","glUseProgramObjectARB","glUseProgramStages","glValidateProgram","glValidateProgramARB","glValidateProgramPipeline","glVertex2d",
"glVertex2dv","glVertex2f","glVertex2fv","glVertex2i","glVertex2iv","glVertex2s","glVertex2sv","glVertex3d","glVertex3dv","glVertex3f",
"glVertex3fv","glVertex3i","glVertex3iv","glVertex3s","glVertex3sv","glVertex4d","glVertex4dv","glVertex4f","glVertex4fv","glVertex4i",
"glVertex4iv","glVertex4s","glVertex4sv","glVertexAttrib1d","glVertexAttrib1dARB","glVertexAttrib1dv","glVertexAttrib1dvARB","glVertexAttrib1f","glVertexAttrib1fARB","glVertexAttrib1fv",
"glVertexAttrib1fvARB","glVertexAttrib1s","glVertexAttrib1sARB","glVertexAttrib1sv","glVertexAttrib1svARB","glVertexAttrib2d","glVertexAttrib2dARB","glVertexAttrib2dv","glVertexAttrib2dvARB","glVertexAttrib2f",
"glVertexAttrib2fARB","glVertexAttrib2fv","glVertexAttrib2fvARB","glVertexAttrib2s","glVertexAttrib2sARB","glVertexAttrib2sv","glVertexAttrib2svARB","glVertexAttrib3d","glVertexAttrib3dARB","glVertexAttrib3dv",
"glVertexAttrib3dvARB","glVertexAttrib3f","glVertexAttrib3fARB","glVertexAttrib3fv","glVertexAttrib3fvARB","glVertexAttrib3s","glVertexAttrib3sARB","glVertexAttrib3sv","glVertexAttrib3svARB","glVertexAttrib4Nbv",
"glVertexAttrib4NbvARB","glVertexAttrib4Niv","glVertexAttrib4NivARB","glVertexAttrib4Nsv","glVertexAttrib4NsvARB","glVertexAttrib4Nub","glVertexAttrib4NubARB","glVertexAttrib4Nubv","glVertexAttrib4NubvARB","glVertexAttrib4Nuiv",
"glVertexAttrib4NuivARB","glVertexAttrib4Nusv","glVertexAttrib4NusvARB","glVertexAttrib4bv","glVertexAttrib4bvARB","glVertexAttrib4d","glVertexAttrib4dARB","glVertexAttrib4dv","glVertexAttrib4dvARB","glVertexAttrib4f",
"glVertexAttrib4fARB","glVertexAttrib4fv","glVertexAttrib4fvARB","glVertexAttrib4iv","glVertexAttrib4ivARB","glVertexAttrib4s","glVertexAttrib4sARB","glVertexAttrib4sv","glVertexAttrib4svARB","glVertexAttrib4ubv",
"glVertexAttrib4ubvARB","glVertexAttrib4uiv","glVertexAttrib4uivARB","glVertexAttrib4usv","glVertexAttrib4usvARB","glVertexAttribDivisor","glVertexAttribI1i","glVertexAttribI1iv","glVertexAttribI1ui","glVertexAttribI1uiv",
"glVertexAttribI2i","glVertexAttribI2iv","glVertexAttribI2ui","glVertexAttribI2uiv","glVertexAttribI3i","glVertexAttribI3iv","glVertexAttribI3ui","glVertexAttribI3uiv","glVertexAttribI4bv","glVertexAttribI4i",
"glVertexAttribI4iv","glVertexAttribI4sv","glVertexAttribI4ubv","glVertexAttribI4ui","glVertexAttribI4uiv","glVertexAttribI4usv","glVertexAttribIPointer","glVertexAttribL1d","glVertexAttribL1dv","glVertexAttribL2d",
"glVertexAttribL2dv","glVertexAttribL3d","glVertexAttribL3dv","glVertexAttribL4d","glVertexAttribL4dv","glVertexAttribLPointer","glVertexAttribP1ui","glVertexAttribP1uiv","glVertexAttribP2ui","glVertexAttribP2uiv",
"glVertexAttribP3ui","glVertexAttribP3uiv","glVertexAttribP4ui","glVertexAttribP4uiv","glVertexAttribPointer","glVertexAttribPointerARB","glVertexP2ui","glVertexP2uiv","glVertexP3ui","glVertexP3uiv",
"glVertexP4ui","glVertexP4uiv","glVertexPointer","glViewport","glViewportArrayv","glViewportIndexedf","glViewportIndexedfv","glWaitSync","glWindowPos2d","glWindowPos2dv",
"glWindowPos2f","glWindowPos2fv","glWindowPos2i","glWindowPos2iv","glWindowPos2s","glWindowPos2sv","glWindowPos3d","glWindowPos3dv","glWindowPos3f","glWindowPos3fv",
"glWindowPos3i","glWindowPos3iv","glWindowPos3s"
		};
	}
}

namespace fg
{
	namespace gl
	{
		void (API_ENTRY *getGlFuncPtr(int index))()
		{
			void (API_ENTRY *&ptr)() = fg::priv::gl_func_array[index];
			
			if (!ptr) ptr = (void(API_ENTRY *)())retrieveProcAddress(fg::priv::gl_func_names[index]);
			
			if (!ptr) fg::printNULL(fg::priv::gl_func_names[index]);
			
			return ptr;
		}
	}
}

