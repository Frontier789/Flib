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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_WAPI_GLCONTEXT_INCLUDED
#define FRONTIER_WAPI_GLCONTEXT_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Window/GLContext.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_WAPI_GLCONTEXT
#include <windows.h>

namespace fw
{
	namespace Wapi
	{
		/////////////////////////////////////////////////////////////
		/// @brief Windows api OpenGL context
		///
		/// Upon error every function of this class prints to fw::fw_log
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API GLContext : public fm::NonCopyable
		{
			HDC   m_hdc;    ///< The handle of the device context
			HWND  m_hwnd;   ///< The handle of the window
			HGLRC m_hglrc;  ///< The handle of the OpenGL rednering context
			bool init(HGLRC sharedContext);     ///< Internal function used to initialize the context
			fw::GLContext::Settings m_settings; ///< The attributes of the context
			bool setPixelFormat(); ///< Internal function
			bool m_ownWindow;      ///< Indicates whether the context holds a dummy window
			static unsigned int m_contextWindowCount; ///< Holds the number of contexts with dummy windows

		public:
			typedef GLContext &reference;
			typedef const GLContext &const_reference;

			typedef HGLRC Handle; ///< The handle type is defined for all GLContext implementations

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
			/// then this function deactivates the current context
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
			/// @param windowHandle The handle of the associated window (shouldn't be NULL)
			/// @param sharedContext The context to share the OpenGL objects with
			/// @param settings The attributes of the context (only a hint!)
			///
			/// @return True if everything went right
			///
			/////////////////////////////////////////////////////////////
			bool create(HWND windowHandle,HGLRC sharedContext=NULL,fw::GLContext::Settings settings = fw::GLContext::Settings());

			/////////////////////////////////////////////////////////////
			/// @brief Initializes the context with a dummy (hidden) window
			///
			/// @param sharedContext The context to share the OpenGL objects with
			/// @param settings The attributes of the context (only a hint!)
			///
			/// @return True if everything went right
			///
			/////////////////////////////////////////////////////////////
			bool create(HGLRC sharedContext=NULL,unsigned int width=1,unsigned int height=1,fw::GLContext::Settings settings = fw::GLContext::Settings());

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
			/// Returns NULL if not initialized
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


#endif // FRONTIER_WAPI_GLCONTEXT_INCLUDED
