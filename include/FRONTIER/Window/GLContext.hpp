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
#ifndef FRONTIER_GLCONTEXT_INCLUDED
#define FRONTIER_GLCONTEXT_INCLUDED
#include <FRONTIER/Window/WindowPredef.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Graphics/Image.hpp>
#define FRONTIER_GLCONTEXT

namespace fm
{
	template<class>
	class vector4;
}

namespace fw
{
	class GLContext;
	namespace priv
	{
		fw::GLContext &getSharedContext();
	}

	/////////////////////////////////////////////////////////////
	/// @brief Encodes types of blending
	///
	/// By default blending is not used (same as Overwrite)
	///
	/////////////////////////////////////////////////////////////
	enum BlendMode {
		Overwrite, ///< Destination = Input
		Additive,  ///< Destination = Destination + Input
		Alpha      ///< Destination = Destination*(1-Input.a) + Input*Input.a
	};

	/////////////////////////////////////////////////////////////
	/// @brief Encodes types of depth testing
	///
	/// By default depth test is not used
	///
	/////////////////////////////////////////////////////////////
	enum DepthTestMode {
		Less,    ///< Writes the fragment iff it's distance is smaller than the current in the depth buffer
		LEqual,  ///< Writes the fragment iff it's distance is smaller or equal than the current in the depth buffer
		GEqual,  ///< Writes the fragment iff it's distance is greater or equal than the current in the depth buffer
		Greater, ///< Writes the fragment iff it's distance is greater than the current in the depth buffer
		Always,  ///< Always writes the fragment in the depth buffer
		Unused   ///< Does not perform depth test
	};

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

			unsigned char majorVersion; ///< The major OpenGL version
			unsigned char minorVersion; ///< The minor OpenGL version
			bool compatiblityProfile;   ///< Indicates that the context is backward compatible

			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			///
			/// If an attribute is 0 it means default value
			///
			/// @param majorVersion The major OpenGL version
			/// @param minorVersion The minor OpenGL version
			/// @param compatiblityProfile Indicates that the context is backward compatible
			///
			/////////////////////////////////////////////////////////////
			Settings(unsigned char majorVersion = 4,
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
		virtual ~GLContext();

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
		virtual bool create(priv::WindowHandle windowHandle,fw::GLContext::Settings settings = fw::GLContext::Settings());

		/////////////////////////////////////////////////////////////
		/// @brief Initializes the context without a visible window
		///
		/// @param size The size of the offscreen buffer
		/// @param settings The attributes of the context (only a hint!)
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		virtual bool create(const fm::vec2s &size,fw::GLContext::Settings settings = fw::GLContext::Settings());

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
		/// @brief Set the value of the clearing color used by the currently active context
		///
		/// @param color The new clear-color
		///
		/////////////////////////////////////////////////////////////
		static void setClearColor(const fm::vector4<float> &color);

		/////////////////////////////////////////////////////////////
		/// @brief Set the clearing depth used by the currently active context
		///
		/// @param depth The new clearing depth
		///
		/////////////////////////////////////////////////////////////
		static void setClearDepth(float depth);

		/////////////////////////////////////////////////////////////
		/// @brief Clears the specified buffers of the currently active context
		///
		/// @param colorBuffer If true the color buffer will be cleared with the set clear-color
		/// @param depthBuffer If true the depth buffer will be cleared with the specified value
		/// @param stencilBuffer If true the stencil buffer will be cleared with the specified value
		///
		/// @see setClearColor
		///
		/////////////////////////////////////////////////////////////
		static void clear(bool colorBuffer = true,bool depthBuffer = false,bool stencilBuffer = false);

		/////////////////////////////////////////////////////////////
		/// @brief Set the blending mode of the current context
		///
		/// @param mode The new blending mode
		///
		/// @see BlendMode
		///
		/////////////////////////////////////////////////////////////
		static void setBlend(BlendMode mode);

		/////////////////////////////////////////////////////////////
		/// @brief Set the depthtest mode of the current context
		///
		/// @param mode The new depth-test mode
		///
		/// @see DepthTestMode
		///
		/////////////////////////////////////////////////////////////
		static void setDepthTest(DepthTestMode mode);

		/////////////////////////////////////////////////////////////
		/// @brief Copy the content of the currently bound render target to the cpu
		///
		/// OpenGL uses a coordinate system with origin at the lower-left corner
		/// so flipping the image is desireable if a coordinate system with origin at
		/// the upper left corner is used
		///
		/// @param pos The coordinates of the rectangle to be read
		/// @param size The size of the rectangle to be read
		/// @param flip If true the image will be flipped vertically
		///
		/// @return The read Image
		///
		/////////////////////////////////////////////////////////////
		static fg::Image capture(const fm::vec2s &pos,const fm::vec2s &size,bool flip = true);
	};
}

#endif // FRONTIER_GLCONTEXT_INCLUDED
