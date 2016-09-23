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
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Error.hpp>
#include <FRONTIER/System/Swap.hpp>
#include <FRONTIER/System/Rect.hpp>
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
		if (copy.getGlId() && copy.getSize().w && copy.getSize().h)
		{
			setRepeated(copy.m_isRepeated),
			setSmooth(copy.m_isSmooth);
			loadFromImage(copy.copyToImage());
		}
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
	Texture::Texture(const fm::vec2s &size) : m_isRepeated(false),
                                              m_isSmooth(false)

	{
		create(size);
	}

	/// destructor /////////////////////////////////////////////////////////
	Texture::~Texture()
	{
		if (getGlId() && glIsTexture(getGlId())==GL_TRUE)
		{
			GLuint glId = getGlId();
			glDeleteTextures(1,&glId);
		}
	}

	/// functions /////////////////////////////////////////////////////////
	fm::Result Texture::create(fm::Size width,fm::Size height)
	{
		// check for empty texture
		if (!width || !height)
			return fm::Error("TextureError","CreatingEmptyTexture","Couldn't create Texture, invalid texture size: (" + fm::toString(width).str() + ";" + fm::toString(height).str() + ")","Texture.create",__FILE__,__LINE__);


		// validate size
		fm::vec2s realSize = getValidSize(fm::vec2s(width,height));
		fm::Size maxSize = getMaximumSize();

		// opengl wouldn't accpet too big textures
		if (realSize.w > maxSize || realSize.h > maxSize)
			return fm::Error("TextureError","CreatingTooBigTexture","Couldn't create texture with size (" + fm::toString(width).str() + ";" + fm::toString(height).str() + ")","Texture.create",__FILE__,__LINE__);

		fm::Error err;

		// setup internal data
		m_realSize = realSize;
		m_size(width,height);

		if (glIsTexture(getGlId()) == GL_FALSE)
		{
			GLuint glId;
			err += glCheck(glGenTextures(1,&glId));
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
			err += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			err += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		}

		// set TEXTURE_FILTER
		err += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
		err += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));

		if (getFormat() == GL_DEPTH_COMPONENT)
			err += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE));

		// upload data
		err += glCheck(glTexImage2D(getTexTarget(),0,getInternalFormat(),m_realSize.w,m_realSize.h,0,getFormat(),getType(),fm::nullPtr));

		return err;
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::create(const fm::vec2s &size)
	{
		return create(size.w,size.h);
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::loadFromImage(const Image &img)
	{
		fm::Error err;

		// resize texture to needed size
		if (err = create(img.getSize()))
			return err;
		else
		{
			// upload data
			priv::TextureSaver save(getTexBinding(),getTexRebinding());
			bind();

			err += glCheck(glTexSubImage2D(getTexTarget(),0,0,0,img.getSize().w,img.getSize().h,getFormat(),getType(), img.getPixelsPtr()));
		}

		return err;
	}


	////////////////////////////////////////////////////////////
	fm::Result Texture::loadFromFile(const std::string &filename)
	{
		Image img;
		fm::Error err;

		if (err = img.loadFromFile(filename)) return err;

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
	fm::Result Texture::update(const Color *pixels,fm::Size x,fm::Size y,fm::Size w,fm::Size h)
	{
		fm::Error err;

		if (pixels && getGlId())
		{
			priv::TextureSaver save(getTexBinding(),getTexRebinding());

			bind();
			err += glCheck(glTexSubImage2D(getTexTarget(), 0, x, y, w, h, getFormat(), getType(), pixels));
		}
		else
			return fm::Error("TextureError","UpdateWith0ID","Texture::update was called with 0 id","Texture.update",__FILE__,__LINE__);

		return err;
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::update(const Color *pixels,fm::Size x,fm::Size y,fm::vec2s size)
	{
		return update(pixels,x,y,size.w,size.h);
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::update(const Color *pixels,fm::vec2s pos,fm::Size w,fm::Size h)
	{
		return update(pixels,pos.x,pos.y,w,h);
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::update(const Color *pixels,fm::vec2s pos,fm::vec2s size)
	{
		return update(pixels,pos.x,pos.y,size.w,size.h);
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::update(const Color *pixels)
	{
		return update(pixels,0,0,m_size);
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::update(const Image &image)
	{
		return update(image.getPixelsPtr(),0,0,image.getSize());
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::update(const Image &image,fm::vec2s pos)
	{
		return update(image.getPixelsPtr(),pos,image.getSize());
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::update(const Image &image,unsigned x,unsigned y)
	{
		return update(image.getPixelsPtr(),x,y,image.getSize());
	}

	////////////////////////////////////////////////////////////
	Image Texture::copyToImage(fm::Error *error) const
	{
		if (!getGlId())
			return Image();

		fm::Error err;

		// retrieve bound framebuffer
		GLint framebuffer;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);

		// error means framebuffers aren't supported
		if (fg::FrameBuffer::isAvailable())
		{
			// build a new framebuffer
			GLuint fb_id;
			err += glCheck(glGenFramebuffers(1,&fb_id));
			err += glCheck(glBindFramebuffer(GL_FRAMEBUFFER,fb_id));
			err += glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER,getAttachement(),getTexTarget(),getGlId(),0));

			// Tell GL no draw and read buffers will be needed
			if (getAttachement() == GL_DEPTH_ATTACHMENT)
			{
				err += glCheck(glDrawBuffer(GL_NONE));
				err += glCheck(glReadBuffer(GL_NONE));
			}

			// read back texture
			fg::Image ret;
			ret.create(m_size);
			err += glCheck(glReadPixels(0,0,m_size.w,m_size.h,getFormat(),getType(),ret.getPixelsPtr()));
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
			err += glCheck(glGetTexImage(getTexTarget(),0,getFormat(),getType(),ret.getPixelsPtr()));
		}
		else
		{
			// copy the whole texture
			std::vector<Color> allPixels(m_realSize.w * m_realSize.h);
			bind();
			err += glCheck(glGetTexImage(getTexTarget(),0,getFormat(),getType(),&allPixels[0]));

			// cut the useful part
			const Color *src = &allPixels[0];
			Color *dst = ret.getPixelsPtr();

			C((fm::Size)m_size.h)
			{
				std::memcpy(dst,src,m_size.w*sizeof(Color));
				src += m_realSize.w,
				dst += m_size.w;
			}
		}

		if (error)
			*error = err;

		return ret;
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::bind() const
	{
		return glCheck(glBindTexture(getTexRebinding(),getGlId()));
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::bind(const Texture *texture)
	{
		if (texture)
			return texture->bind();

		return glCheck(glBindTexture(GL_TEXTURE_2D,0));
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture::bind(const Texture &texture)
	{
		return texture.bind();
	}

	/////////////////////////////////////////////////////////////
	fm::mat4 Texture::getPixToUnitMatrix() const
	{
		return fm::MATRIX::scaling(getRealSize().inv());
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
		if (ver && ver[0] > '1')
			return size;

		fm::vec2s ret(1,1);
		while (ret.w < size.w) ret.w*=2;
		while (ret.h < size.h) ret.h*=2;

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
		fm::swap(getGlId()   ,tex.getGlId()    );
		fm::swap(m_realSize  ,tex.m_realSize   );
		fm::swap(m_size      ,tex.m_size       );
		fm::swap(m_isRepeated,tex.m_isRepeated );
		fm::swap(m_isSmooth  ,tex.m_isSmooth   );
		
		return *this;
	}
}
