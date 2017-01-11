////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <cstring>

namespace fg
{
	namespace priv
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
	fm::Int32  Texture::getInternalFormat() const {return /*GL_RGBA8*/GL_RGBA;}
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

	////////////////////////////////////////////////////////////
	Texture::Texture(const Texture &copy) : m_isRepeated(false),
											m_isSmooth(false)
	{
		*this = copy;
	}

	////////////////////////////////////////////////////////////
	Texture::Texture(Texture &&move) : m_isRepeated(false),
									   m_isSmooth(false)
	{
		this->swap(move);
	}

	////////////////////////////////////////////////////////////
	Texture::reference Texture::operator=(const Texture &tex)
	{
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

	////////////////////////////////////////////////////////////
	Texture::reference Texture::operator=(Texture &&tex)
	{
		this->swap(tex);
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
	Texture::Texture(fm::vec2s size,const fm::vec4 &color) : m_isRepeated(false),
															 m_isSmooth(false)

	{
		create(size,color);
	}

	/// destructor /////////////////////////////////////////////////////////
	Texture::~Texture()
	{
		if (getGlId() && glIsTexture(getGlId()) == GL_TRUE)
		{
			GLuint glId = getGlId();
			glDeleteTextures(1,&glId);
		}
	}

	/// functions /////////////////////////////////////////////////////////
	fm::Result Texture::create(fm::vec2s size)
	{
		fm::vec2s realSize = getValidSize(size);
		
		fm::Result res;
		
		// opengl wouldn't accpet too big textures
		if (realSize != size)
			res += fm::Result("TextureError",fm::Result::OPChanged,"OutOfBunds","create",__FILE__,__LINE__,fm::toString(size.w).str(),fm::toString(size.h).str());
		
		// setup internal data
		m_realSize = realSize;
		m_size = size;

		if (glIsTexture(getGlId()) == GL_FALSE)
		{
			GLuint glId;
			res += glCheck(glGenTextures(1,&glId));
			getGlId() = glId;
		}

		// bind the texture for uplading
		priv::TextureSaver save(getTexBinding(),getTexRebinding());

		bind();

		// set TEXTURE_WRAP
		glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		if (glGetError() != GL_NO_ERROR)
		{
			m_isRepeated = !m_isRepeated;
			res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		}

		// set TEXTURE_FILTER
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));

		if (getFormat() == GL_DEPTH_COMPONENT)
			res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE));

		// upload data
		res += glCheck(glTexImage2D(getTexTarget(),0,getInternalFormat(),m_realSize.w,m_realSize.h,0,getFormat(),getType(),nullptr));

		return res;
	}
	
	////////////////////////////////////////////////////////////
	fm::Result Texture::create(fm::vec2s size,const fm::vec4 &color)
	{
		Image tmpImg;
		tmpImg.create(size,color*255);
		
		return loadFromImage(tmpImg);
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::loadFromImage(const Image &img)
	{
		fm::Result res;

		// resize texture to needed size
		if ((res = create(img.getSize())))
			return res;
		else
		{
			// upload data
			priv::TextureSaver save(getTexBinding(),getTexRebinding());
			bind();

			res += glCheck(glTexSubImage2D(getTexTarget(),0,0,0,img.getSize().w,img.getSize().h,getFormat(),getType(), img.getPtr()));
		}

		return res;
	}


	////////////////////////////////////////////////////////////
	fm::Result Texture::loadFromFile(const std::string &filename)
	{
		Image img;
		fm::Result res;

		if ((res = img.loadFromFile(filename))) return res;

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
				priv::TextureSaver save(getTexBinding(),getTexRebinding());

				bind();

				glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
				glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
				if (glGetError() != GL_NO_ERROR)
				{
					glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_S,m_isRepeated ? GL_CLAMP_TO_EDGE : GL_REPEAT);
					glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_T,m_isRepeated ? GL_CLAMP_TO_EDGE : GL_REPEAT);
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
				priv::TextureSaver save(getTexBinding(),getTexRebinding());

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
			priv::TextureSaver save(getTexBinding(),getTexRebinding());

			bind();
			glCheck(glTexSubImage2D(getTexTarget(), 0, pos.x, pos.y, size.w, size.h, getFormat(), getType(), pixels));
		}

		return *this;
	}

	////////////////////////////////////////////////////////////
	Texture::reference Texture::update(const Image &image,fm::vec2s pos)
	{
		return update(image.getPtr(),pos,image.getSize());
	}

	////////////////////////////////////////////////////////////
	Image Texture::copyToImage(fm::Result *error) const
	{
		if (!getGlId())
			return Image();

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
			fg::Image ret;
			ret.create(m_size);
			res += glCheck(glReadPixels(0,0,m_size.w,m_size.h,getFormat(),getType(),ret.getPtr()));
			glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
			glDeleteFramebuffers(1,&fb_id);
			return ret;
		}

		// prepare image
		priv::TextureSaver save(getTexBinding(),getTexRebinding());
		Image ret;
		ret.create(m_size);

		// if npot textures are present or m_size is pot
		if (m_size == m_realSize)
		{
			// simply copy back texture
			bind();
			res += glCheck(glGetTexImage(getTexTarget(),0,getFormat(),getType(),ret.getPtr()));
		}
		else
		{
			// copy the whole texture
			std::vector<Color> allPixels(m_realSize.w * m_realSize.h);
			bind();
			res += glCheck(glGetTexImage(getTexTarget(),0,getFormat(),getType(),&allPixels[0]));

			// cut the useful part
			const Color *src = &allPixels[0];
			Color *dst = ret.getPtr();

			C((fm::Size)m_size.h)
			{
				std::memcpy(dst,src,m_size.w*sizeof(Color));
				src += m_realSize.w,
				dst += m_size.w;
			}
		}

		if (error)
			*error = res;

		return ret;
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
		return fm::MATRIX::scaling(fm::vec2(1,1)/getRealSize());
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
	fm::vec2s Texture::getValidSize(fm::vec2s size)
	{
		fm::Size mx = getMaximumSize();
		
		size.w = std::max<unsigned int>(std::min(size.w,mx),1);
		size.h = std::max<unsigned int>(std::min(size.h,mx),1);
		
		const GLubyte *ver = glGetString(GL_VERSION);
		if (ver && ver[0] > '1')
			return size;

		fm::vec2s ret(1,1);
		while (ret.w < size.w) ret.w <<= 1;
		while (ret.h < size.h) ret.h <<= 1;
		
		return ret;
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
		std::swap(m_realSize  ,tex.m_realSize   );
		std::swap(m_size      ,tex.m_size       );
		std::swap(m_isRepeated,tex.m_isRepeated );
		std::swap(m_isSmooth  ,tex.m_isSmooth   );
		
		return *this;
	}
}
