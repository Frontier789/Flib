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
#include <FRONTIER/Graphics/FloatTexture.hpp>
#include <FRONTIER/Graphics/FrameBuffer.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	namespace
	{
		//////////////////////////////////
		/// TextureSaver
		//////////////////////////////////
		class TextureSaver
		{
			GLint m_textureId;
			GLenum m_target;
		public:

			//////////////////////////////////
			TextureSaver(GLenum binding,GLenum texTarget) : m_target(texTarget)
			{
				GLint textureId;
				glGetIntegerv(binding, &textureId);
				m_textureId = textureId;
			}

			//////////////////////////////////
			~TextureSaver()
			{
				GLint textureId = m_textureId;
				glBindTexture(m_target, textureId);
			}
		};
	}
	
	////////////////////////////////////////////////////////////
	fm::Int32  FloatTexture::getInternalFormat() const {return GL_RGBA32F;}

	/// constructor /////////////////////////////////////////////////////////
	FloatTexture::FloatTexture() : Texture::Texture()
	{

	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	FloatTexture::FloatTexture(const FloatTexture &copy) : Texture::Texture(copy)
	{
		
	}
#endif

	////////////////////////////////////////////////////////////
	FloatTexture::FloatTexture(FloatTexture &&move)
	{
		((Texture*)this)->swap(move);
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	FloatTexture &FloatTexture::operator=(const FloatTexture &copy)
	{
		*((Texture*)this) = (const Texture &)copy;
		
		return *this;
	}
#endif

	////////////////////////////////////////////////////////////
	FloatTexture &FloatTexture::operator=(FloatTexture &&move)
	{
		((Texture*)this)->swap(move);
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	FloatTexture::FloatTexture(const Image &img) : Texture::Texture(img)
	{

	}

	/////////////////////////////////////////////////////////////
	fm::Result FloatTexture::copyToArray(float *ptr,fm::rect2s region)
	{
		if (!getGlId() || !getSize().area())
			return fm::Result();

		if (!fm::rect2s(fm::vec2(),getSize() - fm::vec2s(1,1)).contains(region.pos))
			return fm::Result();
		
		if (region.size.w == 0) region.size.w = getSize().w - region.pos.x;
		if (region.size.h == 0) region.size.h = getSize().h - region.pos.y;

		if (fg::FrameBuffer::isAvailable())
		{
			fm::Result res;
			
			// retrieve bound framebuffer
			GLint framebuffer;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);
			
			// build a new framebuffer
			GLuint fb_id;
			res += glCheck(glGenFramebuffers(1,&fb_id));
			res += glCheck(glBindFramebuffer(GL_FRAMEBUFFER,fb_id));
			res += glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER,getAttachement(),getTexTarget(),getGlId(),0));

			// read back texture
			res += glCheck(glReadPixels(region.pos.x,region.pos.y,region.size.w,region.size.h,getFormat(),GL_FLOAT,ptr));
			glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
			glDeleteFramebuffers(1,&fb_id);
			return res;
		}

		return fm::Result("GLError",fm::Result::OPFailed,"FrameBufferUnavailable","copyToArray",__FILE__,__LINE__);
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result FloatTexture::loadFromArray(const float *ptr,fm::vec2s size)
	{
		fm::Result res = create(size);

		if (!res) return res;
		else
		{
			// upload data
			TextureSaver save(getTexBinding(),getTexRebinding());
			bind();

			res += glCheck(glTexSubImage2D(getTexTarget(),0,0,0,size.w,size.h,getFormat(),GL_FLOAT,ptr));
		}

		return res;
	}

	////////////////////////////////////////////////////////////
	FloatTexture::reference FloatTexture::update(const float *data,fm::vec2s pos,fm::vec2s size)
	{
		if (data && getGlId())
		{
			TextureSaver save(getTexBinding(),getTexRebinding());

			bind();
			glCheck(glTexSubImage2D(getTexTarget(), 0, pos.x, pos.y, size.w, size.h, getFormat(), GL_FLOAT, data));
		}

		return *this;
	}
}
