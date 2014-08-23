#ifndef FRONTIER_GL_EXTENSION_WRAP_HPP_INCLUDED
#define FRONTIER_GL_EXTENSION_WRAP_HPP_INCLUDED

#define FRONTIER_LOG_GL_FUNCTION_CHECK 2

#include <cstddef>
namespace glWrap
{
	extern bool (*IsBuffer)(unsigned int id);
	
	extern void (*DeleteBuffers)(std::size_t count,const unsigned int *buffers);
	extern void (*GenBuffers)(std::size_t count,unsigned int *buffers);
	extern void (*GetIntegerv)(unsigned int pname,int *data);
	extern void (*BindBuffer)(unsigned int target,unsigned int buffer);
	extern void (*BufferSubData)(unsigned int target,std::ptrdiff_t offset,std::size_t size,const void *data);
	extern void (*BufferData)(unsigned int target,std::size_t size,const void *data,unsigned int usage);

 
	extern unsigned int (*GetError)();
}
#endif
/// http://www.khronos.org/registry/gles/extensions/OES/OES_framebuffer_object.txt
/// http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_buffer_object.txt
/// http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
/// https://www.opengl.org/registry/specs/EXT/framebuffer_object.txt
/// file:///F:/Doc/glTricks/opengl%20es%20-%20OES%20EXT%20ARB_framebuffer_object%20-%20Stack%20Overflow.htm