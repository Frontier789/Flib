#ifndef FRONTIER_OPENGL_HPP_INCLUDED
#define FRONTIER_OPENGL_HPP_INCLUDED
#define FRONTIER_OPENGL
#include <FRONTIER/Config.hpp>

//////////////////////////////////
/// includeing GL headers
//////////////////////////////////
#ifndef GL_GLEXT_PROTOTYPES
    #define GL_GLEXT_PROTOTYPES
#endif

#if defined(FRONTIER_OS_WINDOWS)

    #ifdef _MSC_VER
        #include <windows.h>
    #endif

	#include <FRONTIER/Graphics/GL/gl_comp_4_2.h>

    #include <GL/gl.h>
    #include <GL/glext.h>

#elif defined(FRONTIER_OS_ANDROID)

	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <android/api-level.h>

	#if __ANDROID_API__ >= 8
		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>
	#endif

#elif defined(FRONTIER_OS_MACOS)

	#include <FRONTIER/Graphics/GL/gl_comp_4_2.h>

    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>

#elif defined(FRONTIER_OS_LINUX)

	#include <FRONTIER/Graphics/GL/gl_comp_4_2.h>

    #include <GL/gl.h>
    #include <GL/glext.h>

#else

    #error OS not supported!

#endif

#endif
