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
#ifndef FRONTIER_GLCONTEXT_INCLUDED
#define FRONTIER_GLCONTEXT_INCLUDED
#include <FRONTIER/Window/WindowPredef.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_GLCONTEXT

namespace fw
{
	class GLContext;
	namespace priv
	{
		extern fw::GLContext theSharedContext;
		class theSharedContextInitializer;
	}

	/////////////////////////////////////////////////////////////
	/// @brief GLContext
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GLContext : public fm::NonCopyable
	{
		priv::GLContext *m_context;
	public:

		typedef GLContext &reference;
		typedef const GLContext &const_reference;

		typedef priv::ContextHandle Handle; ///< The undelying handle type

		/////////////////////////////////////////////////////////////
		/// @brief Holds the attributes of a OpenGL context
		///
		/// @ingroup Window
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API Settings
		{
		public:
			typedef Settings &reference;
			typedef const Settings &const_reference;

			unsigned char bitsPerPixel; ///< The number of bits describing a pixel (usually 32)
			unsigned char depthBits;    ///< The number of bits holding the depth value
			unsigned char stencilBits;  ///< The number of stencil bits
			unsigned char majorVersion; ///< The major OpenGL version
			unsigned char minorVersion; ///< The minor OpenGL version
			bool compatiblityProfile;   ///< Indicates that the context is backward compatible

			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			///
			/// If an attribute is 0 it means default value
			///
			/// @param bitsPerPixel The number of bits describing a pixel (usually 32)
			/// @param depthBits    The number of bits holding the depth value
			/// @param stencilBits  The number of stencil bits
			/// @param majorVersion The major OpenGL version
			/// @param minorVersion The minor OpenGL version
			/// @param compatiblityProfile Indicates that the context is backward compatible
			///
			/////////////////////////////////////////////////////////////
			Settings(unsigned char bitsPerPixel = 32,
					 unsigned char depthBits    = 24,
					 unsigned char stencilBits  = 8,
					 unsigned char majorVersion = 4,
					 unsigned char minorVersion = 5,
					 bool compatiblityProfile   = true);

			/////////////////////////////////////////////////////////////
			/// @brief Decreases the OpenGL version AND keeps it valid (max knwown is 4.5)
			///
			/// Known versions are:
			/// 	4.5 (2014)
			/// 	4.4 (2013)
			/// 	4.3 (2012)
			/// 	4.2 (2011)
			/// 	4.1 (2010)
			/// 	4.0 (2010)
			/// 	3.3 (2010)
			/// 	3.2 (2009)
			/// 	3.1 (2009)
			/// 	3.0 (2008)
			/// 	2.1 (2006)
			/// 	2.0 (2004)
			/// 	1.5 (2003)
			/// 	1.4 (2002)
			/// 	1.3 (2001)
			/// 	1.2 (1998)
			/// 	1.1 (1997)
			/// 	1.0 (1992)
			/// Above 4.5 every version is considered valid
			///
			/////////////////////////////////////////////////////////////
			void decreaseVersion();
		};

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		GLContext();

		/////////////////////////////////////////////////////////////
		/// @brief Initializes the context
		///
		/// @param windowHandle The handle of the associated window (shouldn't be NULL)
		/// @param settings The attributes of the context (only a hint!)
		///
		/////////////////////////////////////////////////////////////
		GLContext(priv::WindowHandle windowHandle,fw::GLContext::Settings settings = fw::GLContext::Settings());

		/////////////////////////////////////////////////////////////
		/// @brief Initializes the context without a visible window
		///
		/// @param size The size of the offscreen buffer
		/// @param settings The attributes of the context (only a hint!)
		///
		/////////////////////////////////////////////////////////////
		GLContext(const fm::vec2s &size,fw::GLContext::Settings settings = fw::GLContext::Settings());

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
		/// @param settings The attributes of the context (only a hint!)
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		bool create(priv::WindowHandle windowHandle,fw::GLContext::Settings settings = fw::GLContext::Settings());

		/////////////////////////////////////////////////////////////
		/// @brief Initializes the context without a visible window
		///
		/// @param size The size of the offscreen buffer
		/// @param settings The attributes of the context (only a hint!)
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		bool create(const fm::vec2s &size,fw::GLContext::Settings settings = fw::GLContext::Settings());

		/////////////////////////////////////////////////////////////
		/// @brief Initializes the context without a visible window
		///
		/// @param settings The attributes of the context (only a hint!)
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		bool create(fw::GLContext::Settings settings = fw::GLContext::Settings());

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
		/// @brief Get the OS specific implementation of the context
		///
		/// @return The context
		///
		/////////////////////////////////////////////////////////////
		priv::GLContext &getOSContext();

		/////////////////////////////////////////////////////////////
		/// @brief Get the OS specific implementation of the context
		///
		/// @return The context
		///
		/////////////////////////////////////////////////////////////
		const priv::GLContext &getOSContext() const;

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
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the view of the currently active GL context
		/// 
		/// Sets the projection matrix and the viewport
		/// 
		/// @param x The x coordinate of the viewport
		/// @param y The y coordinate of the viewport
		/// @param width  The width   of the viewport
		/// @param height The height  of the viewport
		/// @param left The X coordinate of the left-bottom corner
		/// @param bottom The Y coordinate of the left-bottom corner
		/// @param right The X coordinate of the right-top corner
		/// @param top The Y coordinate of the right-top corner
		/// @param znear The first limit of the distance (beyond this the object is not visible)
		/// @param zfar The second limit of the distance (beyond this the object is not visible)
		/// 
		/////////////////////////////////////////////////////////////
		static void setView2d(fm::Size x,fm::Size y,fm::Size width,fm::Size height,float left,float bottom,float right,float top,float znear=-1,float zfar=1);
		
		/////////////////////////////////////////////////////////////
		/// @brief Read the attributes of the context
		/// 
		/// Sets the projection matrix and the viewport
		/// 
		/// @param x The x coordinate of the viewport
		/// @param y The y coordinate of the viewport
		/// @param width  The width   of the viewport
		/// @param height The height  of the viewport
		/// @param fov The field of view in degrees
		/// @param znear The distance of the nearest  visible object (should be greater than 0)
		/// @param zfar  The distance of the furthest visible object (should be greater than @a znear)
		/// 
		/////////////////////////////////////////////////////////////
		static void setView3d(fm::Size x,fm::Size y,fm::Size width,fm::Size height,float fov,float znear=1,float zfar=100);

		friend class priv::theSharedContextInitializer;
	};
}

#endif // FRONTIER_GLCONTEXT_INCLUDED
