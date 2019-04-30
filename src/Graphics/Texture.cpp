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
#include <FRONTIER/Graphics/FrameBuffer.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/System/Ref.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <cstring>

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
	fm::Int32  Texture::getInternalFormat() const {return GL_RGBA8;}
	fm::Uint32 Texture::getTexRebinding() const {return GL_TEXTURE_2D;}
	fm::Uint32 Texture::getAttachement() const {return GL_COLOR_ATTACHMENT0;}
	fm::Uint32 Texture::getTexBinding() const {return GL_TEXTURE_BINDING_2D;}
	fm::Uint32 Texture::getTexTarget() const {return GL_TEXTURE_2D;}
	fm::Uint32 Texture::getFormat() const {return GL_RGBA;}
	fm::Uint32 Texture::getType() const {return GL_UNSIGNED_BYTE;}

	/// constructor /////////////////////////////////////////////////////////
	Texture::Texture() : m_isRepeated(false),
						 m_isSmooth(false)
	{

	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	Texture::Texture(const Texture &copy) : m_isRepeated(false),
											m_isSmooth(false)
	{
		*this = copy;
	}
#endif

	////////////////////////////////////////////////////////////
	Texture::Texture(Texture &&move) : m_isRepeated(false),
									   m_isSmooth(false)
	{
		this->swap(move);
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	Texture::reference Texture::operator=(const Texture &tex)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		if (getGlId() && glIsTexture(getGlId()) == GL_TRUE)
		{
			GLuint glId = getGlId();
			glDeleteTextures(1,&glId);
		}
		
		m_isRepeated = tex.m_isRepeated;
		m_isSmooth   = tex.m_isSmooth;
		getGlId()    = 0;
		
		if (tex.getGlId())
			loadFromImage(tex.copyToImage());
	}
#endif

	////////////////////////////////////////////////////////////
	Texture::reference Texture::operator=(Texture &&tex)
	{
		return this->swap(tex);
	}

	////////////////////////////////////////////////////////////
	Texture::Texture(const Image &img) : m_isRepeated(false),
										 m_isSmooth(false)

	{
		loadFromImage(img);
	}

	////////////////////////////////////////////////////////////
	Texture::Texture(const std::string &filename) : m_isRepeated(false),
                                                    m_isSmooth(false)

	{
		loadFromFile(filename);
	}

	////////////////////////////////////////////////////////////
	Texture::Texture(fm::vec2s size) : m_isRepeated(false),
                                       m_isSmooth(false)

	{
		create(size);
	}

	////////////////////////////////////////////////////////////
	Texture::Texture(fm::vec2s size,fm::vec4 color) : 
		m_isRepeated(false),
		m_isSmooth(false)

	{
		create(size,color);
	}

	////////////////////////////////////////////////////////////
	Texture::~Texture()
	{
		if (getGlId() && glIsTexture(getGlId()) == GL_TRUE)
		{
			GLuint glId = getGlId();
			glDeleteTextures(1,&glId);
		}
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::create(fm::vec2s size)
	{
		fm::Result res;
		
		bool sizeChange = (m_size != size);
		
		m_size = size;

		if (glIsTexture(getGlId()) == GL_FALSE)
		{
			GLuint glId;
			res += glCheck(glGenTextures(1,&glId));
			getGlId() = glId;
		}

		// bind the texture for uplading
		TextureSaver save(getTexBinding(),getTexRebinding());

		bind();

		// set TEXTURE_WRAP
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));

		// set TEXTURE_FILTER
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));

		if (getFormat() == GL_DEPTH_COMPONENT)
			res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE));

		// upload data
		if (sizeChange)
			res += glCheck(glTexImage2D(getTexTarget(),0,getInternalFormat(),m_size.w,m_size.h,0,getFormat(),getType(),nullptr));

		return res;
	}
	
	////////////////////////////////////////////////////////////
	fm::Result Texture::create(fm::vec2s size,fm::vec4 color)
	{
		Image tmpImg;
		tmpImg.create(size,color*255);
		
		return loadFromImage(tmpImg);
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::loadFromImage(const Image &img)
	{
		fm::Result res = create(img.getSize());

		if (!res) return res;
		else
		{
			TextureSaver save(getTexBinding(),getTexRebinding());
			bind();

			res += glCheck(glTexSubImage2D(getTexTarget(),0,0,0,img.getSize().w,img.getSize().h,getFormat(),getType(), img.getPtr()));
		}

		return res;
	}


	////////////////////////////////////////////////////////////
	fm::Result Texture::loadFromFile(const std::string &filename)
	{
		Image img;
		fm::Result res = img.loadFromFile(filename);

		if (!res) return res;

		return loadFromImage(img);
	}


	////////////////////////////////////////////////////////////
	Texture::reference Texture::setRepeated(bool repeat)
	{
		if (m_isRepeated != repeat)
		{
			m_isRepeated = repeat;

			if (getGlId())
			{
				TextureSaver save(getTexBinding(),getTexRebinding());

				bind();

				glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
				glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
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
				TextureSaver save(getTexBinding(),getTexRebinding());

				bind();
				glTexParameteri(getTexTarget(),GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST);
				glTexParameteri(getTexTarget(),GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST);
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
	Texture::reference Texture::update(const Color *pixels,fm::vec2s pos,fm::vec2s size)
	{
		if (pixels && getGlId())
		{
			TextureSaver save(getTexBinding(),getTexRebinding());

			bind();
			glCheck(glTexSubImage2D(getTexTarget(), 0, pos.x, pos.y, size.w, size.h, getFormat(), getType(), pixels));
		}

		return *this;
	}

	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Image &sourceImage,fm::vec2s pos)
	{
		return update(sourceImage.getPtr(),pos,sourceImage.getSize());
	}

	////////////////////////////////////////////////////////////
	Image Texture::copyToImage(fm::Result *error,fm::rect2s part) const
	{
		if (!getGlId())
			return Image();
		
		Image ret;
		
		fm::Result res = copyToImage(ret,part);
		
		if (error) *error = res;
		
		return ret;
	}
	
	////////////////////////////////////////////////////////////
	fm::Result Texture::copyToImage(Image &target,fm::rect2s part) const
	{
		if (!getGlId())
			return fm::Result();
			
		if (part.size.w == 0) part.size.w = getSize().w;
		if (part.size.h == 0) part.size.h = getSize().h;
		if (part.pos.x + part.size.w > getSize().w) part.pos.x = getSize().w - part.size.w;
		if (part.pos.y + part.size.h > getSize().h) part.pos.y = getSize().h - part.size.h;
		
		if (target.getSize() != part.size)
			target.create(part.size);
		
		fm::Result res;
		
		if (fg::FrameBuffer::isAvailable())
		{
			// retrieve bound framebuffer
			GLint framebuffer;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);
			
			// build a new framebuffer
			GLuint fb_id;
			res += glCheck(glGenFramebuffers(1,&fb_id));
			res += glCheck(glBindFramebuffer(GL_FRAMEBUFFER,fb_id));
			res += glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER,getAttachement(),getTexTarget(),getGlId(),0));

			// Tell GL no draw and read buffers will be used
			if (getAttachement() == GL_DEPTH_ATTACHMENT)
			{
				res += glCheck(glDrawBuffer(GL_NONE));
				res += glCheck(glReadBuffer(GL_NONE));
			}

			// read back texture
			res += glCheck(glReadPixels(0,0,part.size.w,part.size.h,getFormat(),getType(),target.getPtr()));
			glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
			glDeleteFramebuffers(1,&fb_id);
			return res;
		}

		return fm::Result("GLError",fm::Result::OPFailed,"FrameBufferNotSupported","copyToImage",__FILE__,__LINE__,"copy from texture failed");
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::bind() const
	{
		return glCheck(glBindTexture(getTexRebinding(),getGlId()));
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::bind(fm::Ref<const Texture> texture)
	{
		if (texture)
			return texture->bind();

		return glCheck(glBindTexture(GL_TEXTURE_2D,0));
	}

	/////////////////////////////////////////////////////////////
	fm::mat4 Texture::getPixToUnitMatrix() const
	{
		return fm::MATRIX::scaling(fm::vec2(1,1)/getSize());
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

	/////////////////////////////////////////////////////////////
	Texture::operator bool() const
	{
		return getGlId();
	}
	
	/////////////////////////////////////////////////////////////
	Texture::reference Texture::swap(Texture &tex)
	{
		std::swap(getGlId()   ,tex.getGlId()    );
		std::swap(m_size      ,tex.m_size       );
		std::swap(m_isRepeated,tex.m_isRepeated );
		std::swap(m_isSmooth  ,tex.m_isSmooth   );
		
		return *this;
	}
}
