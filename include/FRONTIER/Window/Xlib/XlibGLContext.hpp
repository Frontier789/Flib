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
#ifndef FRONTIER_XLIB_GLCONTEXT_INCLUDED
#define FRONTIER_XLIB_GLCONTEXT_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Window/GLContext.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_XLIB_GLCONTEXT
#include <GL/glx.h>

namespace fw
{
	namespace priv
	{
		Display *FRONTIER_API getGlobDisp();
		void FRONTIER_API freeGlobDisp(Display *disp);
		bool FRONTIER_API getFBConfig(Display *disp,void *fbconfptr,bool window);
	}
	
	namespace Xlib
	{
		/////////////////////////////////////////////////////////////
		/// @brief Xlib OpenGL context
		///
		/// Upon error every function of this class prints to fw::fw_log
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
			bool createContext(GLXFBConfig config,::GLXContext sharedContext); ///< Function used to set up the context

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
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool setActive(bool active = true);
			
			/////////////////////////////////////////////////////////////
			/// @brief Swap the back and front buffer
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool swapBuffers();
			
			/////////////////////////////////////////////////////////////
			/// @brief Initializes the context
			/// 
			/// @param window The handle of the associated window (shouldn't be NULL)
			/// @param sharedContext The context to share the OpenGL objects with 
			/// @param settings The attributes of the context (only a hint!)
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool create(::Window window,::GLXContext sharedContext=None,fw::GLContext::Settings settings = fw::GLContext::Settings());
			
			/////////////////////////////////////////////////////////////
			/// @brief Initializes the context without a window
			/// 
			/// @param sharedContext The context to share the OpenGL objects with 
			/// @param width  The width  of the offscreen target
			/// @param height The height of the offscreen target
			/// @param settings The attributes of the context (only a hint!)
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool create(::GLXContext sharedContext=None,unsigned int width=1,unsigned int height=1,fw::GLContext::Settings settings = fw::GLContext::Settings());
			
			/////////////////////////////////////////////////////////////
			/// @brief Destroys the GL context
			///
			/// @return True if everything went right
			///
			/////////////////////////////////////////////////////////////
			bool destroy();

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
