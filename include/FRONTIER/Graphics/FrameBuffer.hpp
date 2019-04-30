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
#ifndef FRONTIER_FRAMEBUFFER_HPP_INCLUDED
#define FRONTIER_FRAMEBUFFER_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/Graphics/DepthTexture.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/System/Vector4.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/Graphics/DepthTestMode.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>

#define FRONTIER_FRAMEBUFFER
#include <cstddef>

namespace fm
{
	template<class>
	class rect;
	typedef rect<fm::Size> rect2s;
}

namespace fg
{
	class Texture;

	/////////////////////////////////////////////////////////////
	/// @brief Class used to crae and handle OpenGL <a href="http://www.opengl.org/wiki/Framebuffer_Object">Framebuffers</a>
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API FrameBuffer : public fm::NonCopyable, public GlObject
	{
		unsigned int m_depthBufID;   ///< The id of the depth buffer
		void init();        ///< Internal function used at setup
		fm::Size m_width;   ///< The width of the FrameBuffer
		fm::Size m_height;  ///< The height of the FrameBuffer
		float m_clearDepth; ///< The depth value used when clearing
		fm::vec4 m_clearColor;         ///< The color thats used when clearing the framebuffer
		DepthTestMode m_depthTestMode; ///< The current depth test mode
	public:
		typedef FrameBuffer &reference;
		typedef const FrameBuffer &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Class used to store data about a depth buffer
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API DepthBuffer
		{
		public:
			fm::Size width;  ///< The width of the depth buffer
			fm::Size height; ///< The height of the depth buffer
			const DepthTexture *dtex; ///< The depth texture (if any)

			/////////////////////////////////////////////////////////////
			/// @brief Construct the depth buffer from its size
			///
			/// @param size The size of the depth buffer
			///
			/////////////////////////////////////////////////////////////
			DepthBuffer(const fm::vec2s &size);

			/////////////////////////////////////////////////////////////
			/// @brief Construct the depth buffer from its size
			///
			/// @param w The width of the depth buffer
			/// @param h The height of the depth buffer
			///
			/////////////////////////////////////////////////////////////
			DepthBuffer(const fm::Size &w,const fm::Size &h);

			/////////////////////////////////////////////////////////////
			/// @brief Construct the depth buffer from its size
			///
			/// @param depthTexture The target of the depth rendering
			///
			/////////////////////////////////////////////////////////////
			DepthBuffer(const DepthTexture &depthTexture);

			/////////////////////////////////////////////////////////////
			/// @brief Indicates that no depth buffer is present
			///
			/////////////////////////////////////////////////////////////
			static FRONTIER_API const DepthBuffer noDepthBuffer;
		};

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// After this call the framebuffer is uninitialized (invalid)
		///
		/////////////////////////////////////////////////////////////
		FrameBuffer();

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param move The instance to be moved
		///
		/////////////////////////////////////////////////////////////
		FrameBuffer(FrameBuffer &&move);

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment
		///
		/// @param move The instance to be moved
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		FrameBuffer &operator=(FrameBuffer &&move);

		/////////////////////////////////////////////////////////////
		/// @brief Swap two framebuffers
		///
		/// @param fbo The framebuffer to swap with
		/// 
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		FrameBuffer &swap(FrameBuffer &fbo);

		/////////////////////////////////////////////////////////////
		/// @brief Construct the framebuffer from attachments and depth buffer
		///
		/// @param colorAttachments A pointer to the textures that will be used as color attachments
		/// @param count The number of textures in @a colorAttachments
		/// @param depthBuf The details of the depth attachment (noDepthBuffer means no depth buffer will be used)
		/// @param depthMode The new depth test mode
		///
		/////////////////////////////////////////////////////////////
		explicit FrameBuffer(const Texture **colorAttachments,fm::Size count = 1,const DepthBuffer &depthBuf = DepthBuffer::noDepthBuffer,fg::DepthTestMode depthMode = fg::AutoDepthTest);

		/////////////////////////////////////////////////////////////
		/// @brief Construct the framebuffer from attachments and depth buffer
		///
		/// @param colorAttachment The texturesthat will be used as color attachment
		/// @param depthBuf The details of the depth attachment (noDepthBuffer means no depth buffer will be used)
		/// @param depthMode The new depth test mode
		///
		/////////////////////////////////////////////////////////////
		explicit FrameBuffer(const Texture &colorAttachment,const DepthBuffer &depthBuf = DepthBuffer::noDepthBuffer,fg::DepthTestMode depthMode = fg::AutoDepthTest);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/// Automatically frees OpenGL id
		///
		/////////////////////////////////////////////////////////////
		~FrameBuffer();

		/////////////////////////////////////////////////////////////
		/// @brief Set the framebuffer's color attachments
		///
		/// @param colorAttachments A pointer to the textures that will be used as color attachments
		/// @param count The number of textures in @a colorAttachments
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setColorAttachments(const Texture **colorAttachments,fm::Size count = 1);

		/////////////////////////////////////////////////////////////
		/// @brief Set the framebuffer's depth attachment
		///
		/// @param depthBuf The details of the depth attachment (noDepthBuffer means no depth buffer will be used)
		/// @param depthMode The new depth test mode
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setDepthBuffer(const DepthBuffer &depthBuf,fg::DepthTestMode depthMode = fg::AutoDepthTest);

		/////////////////////////////////////////////////////////////
		/// @brief Construct the framebuffer from attachments and depth buffer
		///
		/// @param colorAttachments A pointer to the textures that will be used as color attachments
		/// @param count The number of textures in @a colorAttachments
		/// @param depthBuf The details of the depth attachment (noDepthBuffer means no depth buffer will be used)
		/// @param depthMode The new depth test mode
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result create(const Texture **colorAttachments,fm::Size count = 1,const DepthBuffer &depthBuf = DepthBuffer::noDepthBuffer,fg::DepthTestMode depthMode = fg::AutoDepthTest);

		/////////////////////////////////////////////////////////////
		/// @brief Construct the framebuffer from attachments and depth buffer
		///
		/// @param colorAttachment The texturesthat will be used as color attachment
		/// @param depthBuf The details of the depth attachment (noDepthBuffer means no depth buffer will be used)
		/// @param depthMode The new depth test mode
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result create(const Texture &colorAttachment,const DepthBuffer &depthBuf = DepthBuffer::noDepthBuffer,fg::DepthTestMode depthMode = fg::AutoDepthTest);

		/////////////////////////////////////////////////////////////
		/// @brief Bind a framebuffer for usage
		///
		/// @param fbo The framebuffer to be bound (can be NULL which means default framebuffer)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		static fm::Result bind(const FrameBuffer *fbo);

		/////////////////////////////////////////////////////////////
		/// @brief Bind a framebuffer for usage
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		static fm::Result bind(const FrameBuffer &fbo);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of the clearing color
		///
		/// @param color The new clear-color
		///
		/////////////////////////////////////////////////////////////
		void setClearColor(fm::vec4 color);

		/////////////////////////////////////////////////////////////
		/// @brief Set the clearing depth
		///
		/// @param depth The new clearing depth
		///
		/////////////////////////////////////////////////////////////
		void setClearDepth(float depth);

		/////////////////////////////////////////////////////////////
		/// @brief Clears the specified buffers
		/// 
		/// The depth buffer gets cleared if the depth test mode is not set to Unused
		/// 
		/// @param colorBuffer If true the color buffer will be cleared with the set clear-color
		///
		/// @see setClearColor
		///
		/////////////////////////////////////////////////////////////
		void clear(bool colorBuffer = true);

		/////////////////////////////////////////////////////////////
		/// @brief Clears the specified buffers
		///
		/// @param colorBuffer If true the color buffer will be cleared with the set clear-color
		/// @param depthBuffer If true the depth buffer will be cleared with the specified value
		/// @param stencilBuffer If true the stencil buffer will be cleared with the specified value
		///
		/// @see setClearColor
		///
		/////////////////////////////////////////////////////////////
		void clear(bool colorBuffer,bool depthBuffer,bool stencilBuffer = false);

		/////////////////////////////////////////////////////////////
		/// @brief Set the depthtest mode
		///
		/// @param mode The new depth-test mode
		///
		/// @see DepthTestMode
		///
		/////////////////////////////////////////////////////////////
		void setDepthTest(DepthTestMode mode);

		/////////////////////////////////////////////////////////////
		/// @brief Get the depth testing mode set
		///
		/// @return The DepthTestMode set
		///
		/////////////////////////////////////////////////////////////
		DepthTestMode getDepthTest() const;


		/////////////////////////////////////////////////////////////
		/// @brief Test if framebuffers are available
		///
		/// @return True if available
		///
		/////////////////////////////////////////////////////////////
		static bool isAvailable();

		/////////////////////////////////////////////////////////////
		/// @brief Set the viewport
		///
		/// The viewport specifies the part of the render
		/// target that is used for rendering (in pixels)
		/// For more details see <a href="https://www.opengl.org/sdk/docs/man/html/glViewport.xhtml">this article</a>
		///
		/// @param viewport The new viewport
		///
		/////////////////////////////////////////////////////////////
		static void setViewport(const fm::rect2s &viewport);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current viewport
		///
		/// The viewport specifies the part of the render
		/// target that is used for rendering (in pixels)
		/// For more details see <a href="https://www.opengl.org/sdk/docs/man/html/glViewport.xhtml">this article</a>
		///
		/// @return The current viewport
		///
		/////////////////////////////////////////////////////////////
		static fm::rect2s getViewport();

		/////////////////////////////////////////////////////////////
		/// @brief Bind the framebuffer for usage
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result bind() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the FrameBuffer
		///
		/// @return The size of the framebuffer
		///
		/////////////////////////////////////////////////////////////
		const fm::vec2s &getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Apply the given depth test to the currently bound framebuffer
		///
		/////////////////////////////////////////////////////////////
		static void applyDepthTest(fg::DepthTestMode depthMode);
	};
}
#endif // FRONTIER_FRAMEBUFFER_INCLUDED
