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
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/OpenGL.hpp>
#include "TextureSaver.hpp"
#include <cstring>
namespace fg
{
	namespace priv
	{
		void defaultSetTextureMatrixFunc(const fm::mat4 &m)
		{
			glCheck(glMatrixMode(GL_TEXTURE));
			glCheck(glLoadMatrixf(&m.transpose()[0][0]));
			glCheck(glMatrixMode(GL_MODELVIEW));
		}
	}
	void (*Texture::m_setTexMat)(const fm::mat4 &m) = priv::defaultSetTextureMatrixFunc;
	
	/// constructor /////////////////////////////////////////////////////////
	Texture::Texture() : m_isRepeated(false),
						 m_isSmooth(false)
	{
		
	}


	////////////////////////////////////////////////////////////
	Texture::Texture(const Texture &copy) : m_isRepeated(false),
											m_isSmooth(false)
	{
		if (copy.getGlId() && copy.getSize().w && copy.getSize().h)
		{
			m_isRepeated = copy.m_isRepeated,
			m_isSmooth = copy.m_isSmooth;
			loadFromImage(copy.copyToImage());
		}
	}


	////////////////////////////////////////////////////////////
	Texture::Texture(const Image &img) : m_isRepeated(false),
										 m_isSmooth(false)
	
	{
		loadFromImage(img);
	}


	/// destructor /////////////////////////////////////////////////////////
	Texture::~Texture()
	{
		if (glIsTexture(getGlId())==GL_TRUE)
		{
			GLuint glId = getGlId();
			glCheck(glDeleteTextures(1,&glId));
		}
	}


	/// functions /////////////////////////////////////////////////////////
	bool Texture::create(fm::Size width,fm::Size height)
	{
		if (!(width && height))
		{
			fg_log << "Couldn't create Texture, invalid texture size: ("<<width<<";"<<height<<")"<<std::endl;
			return false;
		}



		fm::vec2s realSize = getValidSize(fm::vec2s(width,height));
		fm::Size maxSize = getMaximumSize();




		if (realSize.w > maxSize || realSize.h > maxSize)
		{
			fg_log << "Couldn't create texture with size ("<<width<<";"<<height<<").\n"
					  << "Because its iternal size ("<<realSize.w<<";"<<realSize.h<<")"
					  <<"is bigger than the maximum ("<<maxSize<<";"<<maxSize<<")"<<std::endl;
			return false;
		}

		
		m_realSize = realSize;
		m_size(width,height);
		
		if (glIsTexture(getGlId()) == GL_FALSE)
		{
			GLuint glId;
			glCheck(glGenTextures(1,&glId));
			getGlId() = glId;
		}
		priv::TextureSaver save;

		glCheck(glBindTexture(GL_TEXTURE_2D,getGlId()));
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		if (glGetError() != GL_NO_ERROR)
		{
			glCheck(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,(!m_isRepeated) ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			glCheck(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,(!m_isRepeated) ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		}
		
		glCheck(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
		glCheck(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
		glCheck(glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_realSize.w,m_realSize.h,0,GL_RGBA,GL_UNSIGNED_BYTE, NULL));

		return true;
	}


	////////////////////////////////////////////////////////////
	bool Texture::create(fm::vec2s size)
	{
		return create(size.w,size.h);
	}


	////////////////////////////////////////////////////////////
	bool Texture::loadFromImage(const Image &img)
	{
		if (create(img.getSize()))
		{
			priv::TextureSaver save;
			glCheck(glBindTexture(GL_TEXTURE_2D,getGlId()));
			glCheck(glTexSubImage2D(GL_TEXTURE_2D,0,0,0,img.getSize().w,img.getSize().h,GL_RGBA,GL_UNSIGNED_BYTE, img.getPixelsPtr()));
		}
		else
			return false;
		return true;
	}


	////////////////////////////////////////////////////////////
	bool Texture::loadFromFile(const std::string &filename)
	{
		Image img;
		return img.loadFromFile(filename) && loadFromImage(img);
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::setRepeated(bool repeat)
	{
		if (m_isRepeated != repeat)
		{
			m_isRepeated = repeat;

			if (getGlId())
			{
				priv::TextureSaver save;

				glCheck(glBindTexture(GL_TEXTURE_2D,getGlId()));
				
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
				if (glGetError() != GL_NO_ERROR)
				{
					glCheck(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,m_isRepeated ? GL_CLAMP_TO_EDGE : GL_REPEAT));
					glCheck(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,m_isRepeated ? GL_CLAMP_TO_EDGE : GL_REPEAT));
				}
			}
		}
		return *this;
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::setSmooth(bool smooth)
	{
		if (m_isSmooth != smooth)
		{
			m_isSmooth = smooth;

			if (getGlId())
			{
				priv::TextureSaver save;

				glCheck(glBindTexture(GL_TEXTURE_2D,getGlId()));
				glCheck(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
				glCheck(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
			}
		}
		return *this;
	}


	////////////////////////////////////////////////////////////
	bool Texture::isRepeated() const
	{
		return m_isRepeated;
	}


	////////////////////////////////////////////////////////////
	bool Texture::isSmooth() const
	{
		return m_isSmooth;
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Color *pixels,fm::Size x,fm::Size y,fm::Size w,fm::Size h)
	{
		if (pixels && getGlId())
		{
			priv::TextureSaver save;

			glCheck(glBindTexture(GL_TEXTURE_2D, getGlId()));
			glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
		}
		return *this;
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Color *pixels,fm::Size x,fm::Size y,fm::vec2s size)
	{
		return update(pixels,x,y,size.w,size.h);
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Color *pixels,fm::vec2s pos,fm::Size w,fm::Size h)
	{
		return update(pixels,pos.x,pos.y,w,h);
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Color *pixels,fm::vec2s pos,fm::vec2s size)
	{
		return update(pixels,pos.x,pos.y,size.w,size.h);
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Color *pixels)
	{
		return update(pixels,0,0,m_size);
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Image &image)
	{
		return update(image.getPixelsPtr(),0,0,image.getSize());
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Image &image,fm::vec2s pos)
	{
		return update(image.getPixelsPtr(),pos,image.getSize());
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Image &image,unsigned x,unsigned y)
	{
		return update(image.getPixelsPtr(),x,y,image.getSize());
	}


	////////////////////////////////////////////////////////////
	Image Texture::copyToImage() const
	{
		if (!getGlId())
			return Image();
		
		#ifdef GL_FRAMEBUFFER_BINDING
		GLint framebuffer;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);
		if (glGetError() == GL_NO_ERROR)
		{
			GLuint fb_id;
			glCheck(glGenFramebuffers(1,&fb_id));
			glCheck(glBindFramebuffer(GL_FRAMEBUFFER,fb_id));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,getGlId(),0));

			fg::Image ret;
			ret.create(m_size);
			glCheck(glReadPixels(0,0,m_size.w,m_size.h,GL_RGBA,GL_UNSIGNED_BYTE,ret.getPixelsPtr()));
			glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
			glDeleteFramebuffers(1,&fb_id);
			return ret;			
		}
		#endif
		
		#ifdef glGetTexImage
		
		priv::TextureSaver save;
		Image ret;
		ret.create(m_size);
		
		if (m_size == m_realSize)
		{
			glCheck(glBindTexture(GL_TEXTURE_2D,getGlId()));
			glCheck(glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,ret.getPixelsPtr()));
		}
		else
		{
			std::vector<Color> allPixels(m_realSize.w * m_realSize.h);
			glCheck(glBindTexture(GL_TEXTURE_2D,getGlId()));
			glCheck(glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,&allPixels[0]));

			const Color *src = &allPixels[0];
			Color *dst = ret.getPixelsPtr();

			C((fm::Size)m_size.h)
			{
				std::memcpy(dst,src,m_size.w*sizeof(Color));
				src += m_realSize.w,
				dst += m_size.w;
			}
		}

		return ret;
		#endif
		
		fg_log << "Error no available method found to retrieve teture data " << std::endl;

		return Image(m_size,fg::Color::White);
	}


	////////////////////////////////////////////////////////////
	void Texture::bind(CoordinateSystem coordinateSystem) const
	{
		if (getGlId())
		{
			glCheck(glBindTexture(GL_TEXTURE_2D,getGlId()));

			if (coordinateSystem == Pixels)
			{
				float texMatrix[]={1.f/m_realSize.w,	 		 	 0,0,0,
												  0,1.f/m_realSize.h,0,0,
												  0,			 	 0,1,0,
												  0,			 	 0,0,1};

				m_setTexMat(fm::mat4(texMatrix));
			}
			else
				m_setTexMat(fm::mat4::identity);
		}
	}


	////////////////////////////////////////////////////////////
	void Texture::bind() const
	{
		if (getGlId())
			glCheck(glBindTexture(GL_TEXTURE_2D,getGlId()));
	}


	////////////////////////////////////////////////////////////
	void Texture::bind(const Texture *texture, CoordinateSystem coordinateSystem)
	{
		if (texture && texture->getGlId())
			texture->bind(coordinateSystem);
		else
			glCheck(glBindTexture(GL_TEXTURE_2D,0)),
			m_setTexMat(fm::mat4::identity);
	}


	////////////////////////////////////////////////////////////
	void Texture::bind(const Texture *texture)
	{
		if (texture)
			texture->bind();
		else
			glCheck(glBindTexture(GL_TEXTURE_2D,0));
	}


	////////////////////////////////////////////////////////////
	void Texture::bind(const Texture &texture, CoordinateSystem coordinateSystem)
	{
		if (texture.getGlId())
			texture.bind(coordinateSystem);
		else
			glCheck(glBindTexture(GL_TEXTURE_2D,0)),
			m_setTexMat(fm::mat4::identity);
	}


	////////////////////////////////////////////////////////////
	void Texture::bind(const Texture &texture)
	{
		texture.bind();
	}


	////////////////////////////////////////////////////////////
	fm::Size Texture::getMaximumSize()
	{
		GLint size;
		glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE,&size));

		return size;
	}


	////////////////////////////////////////////////////////////
	fm::vec2 Texture::getSize() const
	{
		return m_size;
	}


	////////////////////////////////////////////////////////////
	fm::vec2 Texture::getRealSize() const
	{
		return m_realSize;
	}


	////////////////////////////////////////////////////////////
	fm::vec2s Texture::getValidSize(const fm::vec2s &size)
	{
		const GLubyte *ver = glGetString(GL_VERSION);
		if (ver[0] > '1')
			return size;

		fm::vec2s ret(1,1);
		while (ret.w < size.w) ret.w*=2;
		while (ret.h < size.h) ret.h*=2;

		return ret;
	}


	////////////////////////////////////////////////////////////
	void Texture::changeSetTexMat(void (*newFunc)(const fm::mat4 &))
	{
		m_setTexMat = newFunc ? newFunc : priv::defaultSetTextureMatrixFunc;
	}
}