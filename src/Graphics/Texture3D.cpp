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
#include <FRONTIER/Graphics/Texture3D.hpp>
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
	fm::Int32  Texture3D::getInternalFormat() const {return GL_RGBA8;}
	fm::Uint32 Texture3D::getTexRebinding() const {return GL_TEXTURE_3D;}
	fm::Uint32 Texture3D::getAttachement() const {return GL_COLOR_ATTACHMENT0;}
	fm::Uint32 Texture3D::getTexBinding() const {return GL_TEXTURE_BINDING_3D;}
	fm::Uint32 Texture3D::getTexTarget() const {return GL_TEXTURE_3D;}
	fm::Uint32 Texture3D::getFormat() const {return GL_RGBA;}
	fm::Uint32 Texture3D::getType() const {return GL_UNSIGNED_BYTE;}

	/// constructor /////////////////////////////////////////////////////////
	Texture3D::Texture3D() : m_isRepeated(false),
						     m_isSmooth(false)
	{

	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	Texture3D::Texture3D(const Texture3D &copy) : m_isRepeated(false),
											      m_isSmooth(false)
	{
		*this = copy;
	}
#endif

	////////////////////////////////////////////////////////////
	Texture3D::Texture3D(Texture3D &&move) : m_isRepeated(false),
									         m_isSmooth(false)
	{
		this->swap(move);
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	Texture3D::reference Texture3D::operator=(const Texture3D &tex)
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
		
		if (tex.getGlId()) {
            fg::Color *ptr = tex.copyToMemory();
			loadFromMemory(tex.getSize(),ptr);
            delete ptr;
        }
	}
#endif

	////////////////////////////////////////////////////////////
	Texture3D::reference Texture3D::operator=(Texture3D &&tex)
	{
		return this->swap(tex);
	}

	////////////////////////////////////////////////////////////
	Texture3D::Texture3D(fm::vec3s size) : m_isRepeated(false),
                                           m_isSmooth(false)

	{
		create(size);
	}

	////////////////////////////////////////////////////////////
	Texture3D::Texture3D(fm::vec3s size,fg::Color color) : m_isRepeated(false),
														   m_isSmooth(false)

	{
		create(size,color);
	}

	////////////////////////////////////////////////////////////
	Texture3D::Texture3D(fm::vec3s size,const fg::Color *color) : m_isRepeated(false),
														          m_isSmooth(false)

	{
		loadFromMemory(size,color);
	}

	////////////////////////////////////////////////////////////
	Texture3D::~Texture3D()
	{
		if (getGlId() && glIsTexture(getGlId()) == GL_TRUE)
		{
			GLuint glId = getGlId();
			glDeleteTextures(1,&glId);
		}
	}

	/// functions /////////////////////////////////////////////////////////
	fm::Result Texture3D::create(fm::vec3s size)
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

		// bind the Texture3D for uplading
		TextureSaver save(getTexBinding(),getTexRebinding());

		bind();

		// set TEXTURE_WRAP
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_R,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));

		// set TEXTURE_FILTER
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
		res += glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));

		// upload data
		if (sizeChange)
			res += glCheck(glTexImage3D(getTexTarget(),0,getInternalFormat(),m_size.x,m_size.y,m_size.z,0,getFormat(),getType(),nullptr));

		return res;
	}
	
	////////////////////////////////////////////////////////////
	fm::Result Texture3D::create(fm::vec3s size,fg::Color color)
	{
		std::vector<Color> c(size.volume(),color);
        
		return loadFromMemory(size,&c[0]);
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture3D::loadFromMemory(fm::vec3s size,const fg::Color *data)
	{
		fm::Result res = create(size);

		if (!res) return res;
		else
		{
			TextureSaver save(getTexBinding(),getTexRebinding());
			bind();

			res += glCheck(glTexSubImage3D(getTexTarget(),0,0,0,0,size.w,size.h,size.d,getFormat(),getType(),data));
		}

		return res;
	}

	////////////////////////////////////////////////////////////
	Texture3D::reference Texture3D::setRepeated(bool repeat)
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
				glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_WRAP_R,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			}
		}
		return *this;
	}


	////////////////////////////////////////////////////////////
	Texture3D::reference Texture3D::setSmooth(bool smooth)
	{
		if (m_isSmooth != smooth)
		{
			m_isSmooth = smooth;

			if (getGlId())
			{
				TextureSaver save(getTexBinding(),getTexRebinding());

				bind();

				glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
				glCheck(glTexParameteri(getTexTarget(),GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
			}
		}
		return *this;
	}

	////////////////////////////////////////////////////////////
	bool Texture3D::isRepeated() const
	{
		return m_isRepeated;
	}

	////////////////////////////////////////////////////////////
	bool Texture3D::isSmooth() const
	{
		return m_isSmooth;
	}

	////////////////////////////////////////////////////////////
	Texture3D::reference Texture3D::update(const Color *pixels,fm::vec3s pos,fm::vec3s size)
	{
		if (pixels && getGlId())
		{
			TextureSaver save(getTexBinding(),getTexRebinding());

			bind();
			glCheck(glTexSubImage3D(getTexTarget(), 0, pos.x, pos.y, pos.z, size.w, size.h, size.d, getFormat(), getType(), pixels));
		}

		return *this;
	}

    namespace {
        void fixsizepos(fm::vec3s &p,fm::vec3s &s,fm::vec3s size) {
            if (s.w == 0) s.w = size.w;
            if (s.h == 0) s.h = size.h;
            if (s.d == 0) s.d = size.d;
            if (p.x + s.w > size.w) p.x = size.w - s.w;
            if (p.y + s.h > size.h) p.y = size.h - s.h;
            if (p.z + s.d > size.d) p.z = size.d - s.d;
        }
    }
	
	////////////////////////////////////////////////////////////
	Color *Texture3D::copyToMemory(fm::Result *result,fm::vec3s pos,fm::vec3s size) const
	{
        fixsizepos(pos,size,getSize());

        Color *data = new Color[size.volume()];

        fm::Result res = copyToMemory(data,pos,size);

        if (result)
            *result = res;

        return data;
    }

	////////////////////////////////////////////////////////////
	fm::Result Texture3D::copyToMemory(Color *data,fm::vec3s pos,fm::vec3s size) const
	{
		if (!getGlId())
			return fm::Result();
			
		fixsizepos(pos,size,getSize());
		
		fm::Result res;
		
		if (fg::FrameBuffer::isAvailable())
		{
			GLint framebuffer;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);
			
			GLuint fb_id;
			res += glCheck(glGenFramebuffers(1,&fb_id));
			res += glCheck(glBindFramebuffer(GL_FRAMEBUFFER,fb_id));

            C(size.d) {
                res += glCheck(glFramebufferTexture3D(GL_FRAMEBUFFER,getAttachement(),getTexTarget(),getGlId(),0,i + pos.z));
                res += glCheck(glReadPixels(0,0,size.w,size.h,getFormat(),getType(),data));

                data += size.w * size.h;
            }

			glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
			glDeleteFramebuffers(1,&fb_id);
			return res;
		}

		return fm::Result("GLError",fm::Result::OPFailed,"FrameBufferNotSupported","copyToImage",__FILE__,__LINE__,"copy from Texture3D failed");
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture3D::bind() const
	{
		return glCheck(glBindTexture(getTexRebinding(),getGlId()));
	}

	////////////////////////////////////////////////////////////
	fm::Result Texture3D::bind(fm::Ref<const Texture3D> Texture3D)
	{
		if (Texture3D)
			return Texture3D->bind();

		return glCheck(glBindTexture(GL_TEXTURE_2D,0));
	}

	/////////////////////////////////////////////////////////////
	fm::mat4 Texture3D::getPixToUnitMatrix() const
	{
		return fm::MATRIX::scaling(fm::vec3(1)/getSize());
	}

	////////////////////////////////////////////////////////////
	fm::Size Texture3D::getMaximumSize()
	{
		GLint size;
		glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE,&size));

		return size;
	}

	////////////////////////////////////////////////////////////
	fm::vec3 Texture3D::getSize() const
	{
		return m_size;
	}

	/////////////////////////////////////////////////////////////
	Texture3D::operator bool() const
	{
		return getGlId();
	}
	
	/////////////////////////////////////////////////////////////
	Texture3D::reference Texture3D::swap(Texture3D &tex)
	{
		std::swap(getGlId()   ,tex.getGlId()    );
		std::swap(m_size      ,tex.m_size       );
		std::swap(m_isRepeated,tex.m_isRepeated );
		std::swap(m_isSmooth  ,tex.m_isSmooth   );
		
		return *this;
	}
}
