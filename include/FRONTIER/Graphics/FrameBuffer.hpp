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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_FRAMEBUFFER_HPP_INCLUDED
#define FRONTIER_FRAMEBUFFER_HPP_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#include <FRONTIER/Config.hpp>

#define FRONTIER_FRAMEBUFFER
#include <cstddef>
namespace fm
{
	template<class>
	class vector2;
	typedef vector2<float> vec2;
}
namespace fg
{
	class Texture;
	
	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Class used to crae and handle OpenGL <a href="http://www.opengl.org/wiki/Framebuffer_Object">Framebuffers</a>
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API FrameBuffer : public fm::NonCopyable, public GlObject
	{
		unsigned int m_depthBufID; ///< The id of the depth buffer
		void init(); ///< Iternal function used at setup
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
			std::size_t width;  ///< The width of the depth buffer
			std::size_t height; ///< The height of the depth buffer
		
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
			DepthBuffer(const std::size_t &w,const std::size_t &h);
		
			/////////////////////////////////////////////////////////////
			/// @brief Indicates that no depth buffer is present
			/// 
			/////////////////////////////////////////////////////////////
			static const DepthBuffer noDepthBuffer;
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// After this call the framebuffer is uninitialized (invalid)
		/// 
		/////////////////////////////////////////////////////////////
		FrameBuffer();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the framebuffer from attachments and depth buffer
		/// 
		/// If colorAttachments is NULL or count is 0 then an error is prompted to
		/// fg_log and the framebuffer becomes invalid
		/// 
		/// @param colorAttachments A pointer to the textures that will be used as color attachments
		/// @param count The number of textures in @a colorAttachments
		/// @param depthBuf The details of the depth attachment (noDepthBuffer means no depth buffer will be used)
		/// 
		/////////////////////////////////////////////////////////////
		FrameBuffer(Texture *colorAttachments,unsigned int count,const DepthBuffer &depthBuf=DepthBuffer::noDepthBuffer);

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
		/// If colorAttachments is NULL or count is 0 then an error is prompted to
		/// fg_log and the framebuffer becomes invalid
		/// 
		/// @param colorAttachments A pointer to the textures that will be used as color attachments
		/// @param count The number of textures in @a colorAttachments
		/// 
		/// @return True if no error is occured
		/// 
		/////////////////////////////////////////////////////////////
		bool setColorAttachments(Texture *colorAttachments,unsigned int count);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the framebuffer's depth attachment
		/// 
		/// If colorAttachments is NULL or count is 0 then an error is prompted to
		/// fg_log and the framebuffer becomes invalid
		/// 
		/// @param depthBuf The details of the depth attachment (noDepthBuffer means no depth buffer will be used)
		/// 
		/// @return True if no error is occured
		/// 
		/////////////////////////////////////////////////////////////
		bool setDepthBuffer(const DepthBuffer &depthBuf);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the framebuffer from attachments and depth buffer
		/// 
		/// If colorAttachments is NULL or count is 0 then an error is prompted to
		/// fg_log and the framebuffer becomes invalid
		/// 
		/// @param colorAttachments A pointer to the textures that will be used as color attachments
		/// @param count The number of textures in @a colorAttachments
		/// @param depthBuf The details of the depth attachment (noDepthBuffer means no depth buffer will be used)
		/// 
		/// @return True if no error is occured
		/// 
		/////////////////////////////////////////////////////////////
		bool create(Texture *colorAttachments,unsigned int count,const DepthBuffer &depthBuf=DepthBuffer::noDepthBuffer);

		/////////////////////////////////////////////////////////////
		/// @brief Bind a framebuffer for usage
		/// 
		/// @param fbo The framebuffer to be bound (can be NULL which means unbind)
		/// 
		/////////////////////////////////////////////////////////////
		static void bind(const FrameBuffer *fbo);
		
		/////////////////////////////////////////////////////////////
		/// @brief Bind a framebuffer for usage
		/// 
		/////////////////////////////////////////////////////////////
		static void bind(const FrameBuffer &fbo);
		
		/////////////////////////////////////////////////////////////
		/// @brief Test if framebuffers are available
		/// 
		/// @return True if available
		/// 
		/////////////////////////////////////////////////////////////
		static bool isAvailable();
		
		/////////////////////////////////////////////////////////////
		/// @brief Bind the framebuffer for usage
		/// 
		/////////////////////////////////////////////////////////////
		void bind() const;
	};
}
#endif // FRONTIER_FRAMEBUFFER_INCLUDED

////////////////////////////////////////////////////////////
/// @class fg::FrameBuffer
/// @ingroup Graphics
///
/// fg::FrameBuffer class can be used to create and handle OpenGL framebuffers
/// 
/// 
/// Usage example:
/// @code
/// fm::vert2f verts[360];
/// 
/// for (std::size_t i=0;i<360;i++)
/// 	verts[i].clr = fm::vec4::Blue,
/// 	verts[i].pos = fm::pol2(std::cos(fm::deg(i*4.f)),fm::deg(i));
/// 
/// fg::Image img;
/// img.create(500,500,fm::vec4(.3,0,0));
/// 
/// fg::Texture tex;
/// tex.loadFromImage(img);
/// 
/// fg::FrameBuffer fbo(&tex,1);
/// fbo.bind();
/// 
/// fg::draw(verts,fg::LineLoop);
/// 
/// tex.copyToImage().saveToFile("polar_rose.png");
/// @endcode
///
/// @see fg::FrameBuffer
/// @see fg::Texture
/// 
////////////////////////////////////////////////////////////
