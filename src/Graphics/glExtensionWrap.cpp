#include <FRONTIER/Graphics/FgLog.hpp>
#include "glExtensionWrap.hpp"
#include <FRONTIER/OpenGL.hpp>
#include <string>
/*****************************************************
	
	GL_ARB_vertex_buffer_object
	
*****************************************************/
#ifndef FRONTIER_LOG_GL_FUNCTION_CHECK
	#define GLF_LOG_ERR(text) (void)0
	#define GLF_LOG_WAR(text) (void)0
#else

	#if FRONTIER_LOG_GL_FUNCTION_CHECK == 0
		#define GLF_LOG_ERR(text) (void)0
		#define GLF_LOG_WAR(text) (void)0
	#endif
	
	#if FRONTIER_LOG_GL_FUNCTION_CHECK == 1
		#define GLF_LOG_ERR(text) fg::fg_log << (text) << std::endl
		#define GLF_LOG_WAR(text) (void)0
	#endif
	
	#if FRONTIER_LOG_GL_FUNCTION_CHECK >= 2
		#define GLF_LOG_ERR(text) fg::fg_log << (text) << std::endl
		#define GLF_LOG_WAR(text) fg::fg_log << (text) << std::endl
	#endif
	
#endif

////////////////////
/// Create a macro which will expand to 0 or 1
////////////////////
#ifdef GL_ARB_vertex_buffer_object
	#define GL_ARB_vertex_buffer_object_defined 1
#else
	#define GL_ARB_vertex_buffer_object_defined 0
#endif

////////////////////
/// This macro adds two expressions after expanding them 
////////////////////
#define FUSE(a,b) FUSE_(a,b)
#define FUSE_(a,b) a##b


////////////////////
/// This macro will "choose" depending on the availablity of extName
////////////////////
#define FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(extName,doesGlFuncExist,funcName,extFuncName,returnType,paramList,callParamList,oglParamList) \
FUSE(FUSE(FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT_,extName##_defined),doesGlFuncExist)(extName,funcName,extFuncName,returnType,paramList,callParamList,oglParamList)


////////////////////
/// The extension exists and the gl fuction too
////////////////////
#define FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT_11(extName,funcName,extFuncName,returnType,paramList,callParamList,oglParamList) \
                                                                                                                                  \
returnType extFuncName paramList                                                                                                  \
{                                                                                                                                 \
	return (returnType) gl##extFuncName oglParamList;                                                                             \
}                                                                                                                                 \
returnType funcName##CORE paramList                                                                                               \
{                                                                                                                                 \
	return (returnType) gl##funcName oglParamList;                                                                                \
}                                                                                                                                 \
returnType funcName##INIT paramList                                                                                               \
{                                                                                                                                 \
	if (gl##funcName)                                                                                                             \
	{                                                                                                                             \
		funcName = funcName##CORE;                                                                                                \
		return (returnType) funcName callParamList;                                                                               \
	}                                                                                                                             \
	GLF_LOG_WAR(std::string("gl")+#funcName+" is NULL");                                                                          \
	                                                                                                                              \
	if (isExtSupported(#extName))                                                                                                 \
	{                                                                                                                             \
		GLF_LOG_WAR(std::string("Using supported ")+#extName+" fallback for gl"+#funcName);                                       \
		funcName = extFuncName;                                                                                                   \
		return (returnType) funcName callParamList;                                                                               \
	}                                                                                                                             \
	                                                                                                                              \
	GLF_LOG_ERR(std::string("Error: gl")+#funcName+"is NULL and "+#extName+" is not supported");                                  \
	typedef returnType RETTYPE;                                                                                                   \
	return RETTYPE ();                                                                                                            \
}                                                                                                                                 \
returnType (*funcName) paramList = funcName##INIT

////////////////////
/// The extension does not exist but the gl fuction does
////////////////////
#define FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT_01(extName,funcName,extFuncName,returnType,paramList,callParamList,oglParamList) \
                                                                                                                                  \
                                                                                                                                  \
returnType funcName##CORE paramList                                                                                               \
{                                                                                                                                 \
	return (returnType) gl##funcName oglParamList;                                                                                \
}                                                                                                                                 \
returnType funcName##INIT paramList                                                                                               \
{                                                                                                                                 \
	if (gl##funcName)                                                                                                             \
	{                                                                                                                             \
		funcName = funcName##CORE;                                                                                                \
		return (returnType) funcName callParamList;                                                                               \
	}                                                                                                                             \
	GLF_LOG_ERR(std::string("Error: gl")+#funcName+"is NULL and "+#extName+" is not defined");                                    \
	typedef returnType RETTYPE;                                                                                                   \
	return RETTYPE ();                                                                                                            \
}                                                                                                                                 \
returnType (*funcName) paramList = funcName##INIT


////////////////////
/// The extension exists but the gl fuction does not
////////////////////
#define FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT_10(extName,funcName,extFuncName,returnType,paramList,callParamList,oglParamList) \
                                                                                                                                  \
returnType extFuncName paramList                                                                                                  \
{                                                                                                                                 \
	return (returnType) gl##extFuncName oglParamList;                                                                             \
}                                                                                                                                 \
returnType funcName##INIT paramList                                                                                               \
{                                                                                                                                 \
	GLF_LOG_WAR(std::string("gl")+#funcName+" is not defined");                                                                   \
	                                                                                                                              \
	if (isExtSupported(#extName))                                                                                                 \
	{                                                                                                                             \
		GLF_LOG_WAR(std::string("Using supported ")+#extName+" fallback for gl"+#funcName);                                       \
		funcName = extFuncName;                                                                                                   \
		return (returnType) funcName callParamList;                                                                               \
	}                                                                                                                             \
	                                                                                                                              \
	GLF_LOG_ERR(std::string("Error: gl")+#funcName+"is not defined and "+#extName+" is not supported");                           \
	typedef returnType RETTYPE;                                                                                                   \
	return RETTYPE ();                                                                                                            \
}                                                                                                                                 \
returnType (*funcName) paramList = funcName##INIT


////////////////////
/// Neither the extension or the gl function is defined
////////////////////
#define FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT_00(extName,funcName,extFuncName,returnType,paramList,callParamList,oglParamList) \
                                                                                                                                  \
returnType funcName##INIT paramList                                                                                               \
{                                                                                                                                 \
	GLF_LOG_ERR(std::string("Error: gl")+#funcName+"is not defined also "+#extName+" is not defined");                            \
	typedef returnType RETTYPE;                                                                                                   \
	return RETTYPE ();                                                                                                            \
}                                                                                                                                 \
returnType (*funcName) paramList = funcName##INIT


////////////////////////////////////////////////////////////////////////////////
/// The gl fuction must exist
////////////////////////////////////////////////////////////////////////////////
#define FRONTIER_CREATE_GL_FUNCTION_WITH_NO_EXT(funcName,returnType,paramList,callParamList,oglParamList)  \
                                                                                                           \
returnType funcName##CORE paramList                                                                        \
{                                                                                                          \
	return (returnType) gl##funcName oglParamList;                                                         \
}                                                                                                          \
returnType funcName##INIT paramList                                                                        \
{                                                                                                          \
	if (gl##funcName)                                                                                      \
	{                                                                                                      \
		funcName = funcName##CORE;                                                                         \
		return (returnType) funcName callParamList;                                                        \
	}                                                                                                      \
	                                                                                                       \
	GLF_LOG_ERR(std::string("Error: gl")+#funcName+"is NULL");                                             \
	typedef returnType RETTYPE;                                                                                                   \
	return RETTYPE ();                                                                                                            \
}                                                                                                          \
returnType (*funcName) paramList = funcName##INIT

namespace glWrap
{
	bool isExtSupported(const std::string &extName);
	float getGlVersion();
	
	
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	/**                                                        **/
	/**                       isBuffer                         **/
	/**                                                        **/
	/**  //////  ////////  //////  ////////  //////  ////////  **/
#ifdef glIsBuffer
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,1,
										 IsBuffer,IsBufferARB,bool,
										 (unsigned int id),
										 (id),
										 ((GLuint)id));
#else
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,0,
										 IsBuffer,IsBufferARB,bool,
										 (unsigned int id),
										 (id),
										 ((GLuint)id));
#endif
	
	
	
	
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	/**                                                        **/
	/**                    DeleteBuffers                       **/
	/**                                                        **/
	/**  //////  ////////  //////  ////////  //////  ////////  **/
#ifdef glDeleteBuffers
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,1,
										 DeleteBuffers,DeleteBuffersARB,void,
										 (std::size_t count,const unsigned int *buffers),
										 (count,buffers),
										 (GLsizei(count),(const GLuint *)buffers));
#else
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,0,
										 DeleteBuffers,DeleteBuffersARB,void,
										 (std::size_t count,const unsigned int *buffers),
										 (count,buffers),
										 (GLsizei(count),(const GLuint *)buffers));
#endif
	
	
	
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	/**                                                        **/
	/**                      GenBuffers                        **/
	/**                                                        **/
	/**  //////  ////////  //////  ////////  //////  ////////  **/
#ifdef glGenBuffers
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,1,
										 GenBuffers,GenBuffersARB,void,
										 (std::size_t count,unsigned int *buffers),
										 (count,buffers),
										 (GLsizei(count),(GLuint *)buffers));
#else
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,0,
										 GenBuffers,GenBuffersARB,void,
										 (std::size_t count,unsigned int *buffers),
										 (count,buffers),
										 (GLsizei(count),(GLuint *)buffers));
#endif
	
	
	
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	/**                                                        **/
	/**                     GetIntegerv                        **/
	/**                                                        **/
	/**  //////  ////////  //////  ////////  //////  ////////  **/
FRONTIER_CREATE_GL_FUNCTION_WITH_NO_EXT(GetIntegerv,void,
										(unsigned int pname,int *data),
										(pname,data),
										(GLuint(pname),(GLint *)data));
	
	
	
	
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	/**                                                        **/
	/**                      BindBuffer                        **/
	/**                                                        **/
	/**  //////  ////////  //////  ////////  //////  ////////  **/
#ifdef glBindBuffer
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,1,
										 BindBuffer,BindBufferARB,void,
										 (unsigned int target,unsigned int buffer),
										 (target,buffer),
										 (GLenum(target),(GLuint)buffer));
#else
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,0,
										 BindBuffer,BindBufferARB,void,
										 (unsigned int target,unsigned int buffer),
										 (target,buffer),
										 (GLenum(target),(GLuint)buffer));
#endif
	
	
	
	
	
	
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	/**                                                        **/
	/**                       GetError                         **/
	/**                                                        **/
	/**  //////  ////////  //////  ////////  //////  ////////  **/
FRONTIER_CREATE_GL_FUNCTION_WITH_NO_EXT(GetError,unsigned int,
										(),
										(),
										());
										
										
	
	
	
	
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	/**                                                        **/
	/**                      BufferData                        **/
	/**                                                        **/
	/**  //////  ////////  //////  ////////  //////  ////////  **/
#ifdef glBufferData
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,1,
										 BufferData,BufferDataARB,void,
										 (unsigned int target,std::size_t size,const void *data,unsigned int usage),
										 (target,size,data,usage),
										 (GLenum(target),(GLsizeiptrARB)size,(const GLvoid *)data,(GLenum)usage));
#else
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,0,
										 BufferData,BufferDataARB,void,
										 (unsigned int target,std::size_t size,const void *data,unsigned int usage),
										 (target,size,data,usage),
										 (GLenum(target),(GLsizeiptrARB)size,(const GLvoid *)data,(GLenum)usage));
#endif
	
	
	
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	/**                                                        **/
	/**                    BufferSubData                       **/
	/**                                                        **/
	/**  //////  ////////  //////  ////////  //////  ////////  **/
#ifdef glBufferSubData
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,1,
										 BufferSubData,BufferSubDataARB,void,
										 (unsigned int target,std::ptrdiff_t offset,std::size_t size,const void *data),
										 (target,offset,size,data),
										 (GLenum(target),(GLintptrARB)offset,(GLsizeiptrARB)size,(const void *)data));
#else
FRONTIER_CREATE_GL_FUNCTION_WITH_ONE_EXT(GL_ARB_vertex_buffer_object,0,
										 BufferSubData,BufferSubDataARB,void,
										 (unsigned int target,std::ptrdiff_t offset,std::size_t size,const void *data),
										 (target,offset,size,data),
										 (GLenum(target),(GLintptrARB)offset,(GLsizeiptrARB)size,(const void *)data));
#endif
	
	
	
	
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	/**                                                        **/
	/**                       external                         **/
	/**                                                        **/
	/**  //////  ////////  //////  ////////  //////  ////////  **/
	
/**  **/	bool isExtSupported(const std::string &extName)
/**  **/	{
/**  **/		if (glGetStringi && glGetIntegerv && glGetError)
/**  **/		{
/**  **/			int numExts;
/**  **/			glGetIntegerv(0x821D, ///GL_NUM_EXTENSIONS
/**  **/						  &numExts);
/**  **/			if (!glGetError())
/**  **/			{
/**  **/				bool error=false;
/**  **/				for (int i=0;i<numExts;i++)
/**  **/				{
/**  **/					std::string exti = (const char *)glGetStringi(0x01F03, ///GL_EXTENSIONS
/**  **/																  i);
/**  **/					if (!i)
/**  **/					{
/**  **/						if (glGetError())
/**  **/						{
/**  **/							error = true;
/**  **/							break;
/**  **/						}
/**  **/						if (exti==extName)
/**  **/							return true;
/**  **/					}
/**  **/				}
/**  **/				if (!error)
/**  **/					return false;
/**  **/			}
/**  **/		}
/**  **/		if (!glGetString || !glGetError)
/**  **/			return false;
/**  **/			
/**  **/		glGetError();
/**  **/		std::string allExts = (const char *)glGetString(0x1F03); ///GL_EXTENSIONS​
/**  **/		
/**  **/		if (glGetError())
/**  **/			return false;
/**  **/		
/**  **/		return allExts.find(extName) != std::string::npos;
/**  **/	}

/**  **/	float getGlVersion()
/**  **/	{
/**  **/		if (!glGetString || !glGetError)
/**  **/			return 0.0f;
/**  **/			
/**  **/		std::string ver = (const char *)glGetString(0x01F02); ///GL_VERSION
/**  **/		
/**  **/		if (glGetError())
/**  **/			return 0.0f;
/**  **/		
/**  **/		float ret=0,floatcount=1;
/**  **/		std::size_t i=0;
/**  **/		for (;i<ver.length() && ver[i]<='9' && ver[i]>='0';i++)
/**  **/			ret*=10.f,
/**  **/			ret+=ver[i]-'0';
/**  **/		i++;
/**  **/		for (;i<ver.length() && ver[i]<='9' && ver[i]>='0';i++)
/**  **/			floatcount/=10.f,
/**  **/			ret+=(ver[i]-'0')*floatcount;
/**  **/		return ret;
/**  **/	}

}
