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
#ifndef FRONTIER_XLIB_GLCONTEXT_INCLUDED
#define FRONTIER_XLIB_GLCONTEXT_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Window/GLContext.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>
#define FRONTIER_XLIB_GLCONTEXT
#include <GL/glx.h>

namespace fw
{
	namespace Xlib
	{
		/////////////////////////////////////////////////////////////
		/// @brief Xlib OpenGL context
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API GLContext : public fm::NonCopyable
		{
			::Window m_win;      ///< Holds the host window (if any)
			Display *m_disp;     ///< The connection to X
			GLXPbuffer m_pbuf;   ///< Holds the offscreen buffer (if any)
			GLXContext m_handle; ///< Holds the identifier of the context
			fw::GLContext::Settings m_settings; ///< The settings of the context

			static int errorHandler(Display *disp,XErrorEvent *xerrev); ///< Function used to avoid X's automatic error handling boiling us out
			fm::Result createContext(GLXFBConfig config,::GLXContext sharedContext); ///< Function used to set up the context

		public:
			typedef GLContext &reference;
			typedef const GLContext &const_reference;
			
			typedef GLXContext Handle; ///< The handle type is defined for all GLContext implementations
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			/// 
			/////////////////////////////////////////////////////////////
			GLContext();
			
			/////////////////////////////////////////////////////////////
			/// @brief Default destructor
			/// 
			/////////////////////////////////////////////////////////////
			~GLContext();
			
			/////////////////////////////////////////////////////////////
			/// @brief Makes the context active in the current thread
			/// 
			/// If the context wasn't successfully initialized
			/// then this function does nothing
			/// 
			/// @param active True to make the context active False to deactive the currently active one
			/// 
			/// @return The result of the call
			/// 
			/////////////////////////////////////////////////////////////
			fm::Result setActive(bool active = true);

			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the size of the context
			///
			/// @param w The width is returned
			/// @param h The height is returned
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result getSize(unsigned int &w,unsigned int &h);
			
			/////////////////////////////////////////////////////////////
			/// @brief Swap the back and front buffer
			/// 
			/// @return The result of the call
			/// 
			/////////////////////////////////////////////////////////////
			fm::Result swapBuffers();
			
			/////////////////////////////////////////////////////////////
			/// @brief Initializes the context
			/// 
			/// @param window The handle of the associated window (shouldn't be NULL)
			/// @param sharedContext The context to share the OpenGL objects with 
			/// @param settings The attributes of the context (only a hint!)
			/// 
			/// @return The result of the call
			/// 
			/////////////////////////////////////////////////////////////
			fm::Result create(::Window window,::GLXContext sharedContext=None,fw::GLContext::Settings settings = fw::GLContext::Settings());
			
			/////////////////////////////////////////////////////////////
			/// @brief Initializes the context without a window
			/// 
			/// @param sharedContext The context to share the OpenGL objects with 
			/// @param width  The width  of the offscreen target
			/// @param height The height of the offscreen target
			/// @param settings The attributes of the context (only a hint!)
			/// 
			/// @return The result of the call
			/// 
			/////////////////////////////////////////////////////////////
			fm::Result create(::GLXContext sharedContext=None,unsigned int width=1,unsigned int height=1,fw::GLContext::Settings settings = fw::GLContext::Settings());
			
			/////////////////////////////////////////////////////////////
			/// @brief Destroys the GL context
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result destroy();

			/////////////////////////////////////////////////////////////
			/// @brief Check if there is a OpenGL context active in the thread
			///
			/// @return True if a GL context is active in the thread
			///
			/////////////////////////////////////////////////////////////
			static bool hasThreadGL();

			/////////////////////////////////////////////////////////////
			/// @brief Get the handle of the context
			/// 
			/// Returns None if not initialized
			/// 
			/// @return The handle
			/// 
			/////////////////////////////////////////////////////////////
			Handle getHandle() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Implicitly convert to handle
			/// 
			/// Returns NULL if not initialized
			/// 
			/// @return The handle
			/// 
			/////////////////////////////////////////////////////////////
			operator Handle() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Read the attributes of the context
			/// 
			/// @return The settings
			/// 
			/////////////////////////////////////////////////////////////
			const fw::GLContext::Settings &getSettings() const;
		};
	}
}


#endif // FRONTIER_XLIB_GLCONTEXT_INCLUDED
