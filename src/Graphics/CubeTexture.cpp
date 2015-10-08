#include <FRONTIER/Graphics/CubeTexture.hpp>
#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	fm::Uint32 CubeTextureFace::getTexRebinding() const {return GL_TEXTURE_CUBE_MAP;}
	fm::Uint32 CubeTextureFace::getTexBinding() const {return GL_TEXTURE_BINDING_CUBE_MAP;}
	fm::Uint32 CubeTextureFace::getTexTarget() const {return GL_TEXTURE_CUBE_MAP_POSITIVE_X+m_faceId;}

	/////////////////////////////////////////////////////////////
	CubeTextureFace::CubeTextureFace(CubeTexture &cubeTex,unsigned int faceId) : m_cubeTex(cubeTex),
																				 m_faceId(faceId)
	{
		m_realSize = fm::vec2(cubeTex.getSize(),cubeTex.getSize()),
		m_size = fm::vec2(cubeTex.getSize(),cubeTex.getSize()),
		m_isRepeated = cubeTex.isRepeated(),
		m_isSmooth = cubeTex.isSmooth(),
		getGlId() = m_cubeTex.getGlId();
	}

	/////////////////////////////////////////////////////////////
	CubeTextureFace::~CubeTextureFace()
	{
		getGlId() = 0;
	}

	/////////////////////////////////////////////////////////////
	bool CubeTextureFace::create(fm::Size width,fm::Size height)
	{
        if (m_cubeTex.getSize() != fm::math::max(width,height))
			return m_cubeTex.create(fm::math::max(width,height));

		return true;
	}

	/////////////////////////////////////////////////////////////
	Texture::reference CubeTextureFace::setRepeated(bool repeat)
	{
		m_cubeTex.setRepeated(repeat);
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Texture::reference CubeTextureFace::setSmooth(bool smooth)
	{
		m_cubeTex.setSmooth(smooth);
		return *this;
	}

	/////////////////////////////////////////////////////////////
	fm::Int32  CubeTexture::getInternalFormat() const {return GL_RGBA;}
	fm::Uint32 CubeTexture::getFormat() const {return GL_RGBA;}
	fm::Uint32 CubeTexture::getType() const {return GL_UNSIGNED_BYTE;}

	/////////////////////////////////////////////////////////////
	CubeTexture::CubeTexture() : m_size(0),
								 m_isRepeated(false),
								 m_isSmooth(false)
	{

	}

	/////////////////////////////////////////////////////////////
	CubeTexture::~CubeTexture()
	{
		if (getGlId() && glIsTexture(getGlId())==GL_TRUE)
			glDeleteTextures(1,&getGlId());
	}

	/////////////////////////////////////////////////////////////
	CubeTexture::CubeTexture(fm::Size size) : m_size(0),
											  m_isRepeated(false),
											  m_isSmooth(false)
	{
		create(size);
	}

	/////////////////////////////////////////////////////////////
	bool CubeTexture::create(fm::Size size)
	{
		// check for empty texture
		if (!size)
		{
			fg_log << "Couldn't create Texture, invalid cube texture size: ("<<size<<")"<<std::endl;
			return false;
		}

		// opengl wouldn't accpet too big textures
		fm::Size maxSize = getMaximumSize();
		if (size > maxSize)
		{
			fg_log << "Couldn't create texture with size ("<<size<<").\n"
				   << "Because its bigger than the maximum ("<<maxSize<<")"<<std::endl;
			return false;
		}

		// setup internal data
		m_size = size;

		if (glIsTexture(getGlId()) == GL_FALSE)
		{
			GLuint glId;
			glCheck(glGenTextures(1,&glId));
			getGlId() = glId;
		}

		// bind the texture for uplading
		GLint boundTex;
		glCheck(glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTex));
		{
			glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,getGlId()));

			// set TEXTURE_WRAP
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);

			if (glGetError() != GL_NO_ERROR)
			{
				m_isRepeated = !m_isRepeated;
				glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
				glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
				glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			}

			// set TEXTURE_FILTER
			glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
			glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));

			// upload data
			C(6)
				glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,getInternalFormat(),m_size,m_size,0,getFormat(),getType(),fm::nullPtr));
		}
		glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,boundTex));

		return true;
	}

	/////////////////////////////////////////////////////////////
	fm::Size CubeTexture::getSize() const
	{
		return m_size;
	}

	/////////////////////////////////////////////////////////////
	CubeTextureFace CubeTexture::getFace(fm::Size index)
	{
		return CubeTextureFace(*this,index);
	}

	/////////////////////////////////////////////////////////////
	CubeTextureFace CubeTexture::operator[](fm::Size index)
	{
		return getFace(index);
	}

	/////////////////////////////////////////////////////////////
	CubeTextureFace CubeTexture::getFace(const fm::vec3 &normal)
	{
		fm::vec3 n = normal;
		n.unsign();

		int index = 0;

		if (n.x >= n.y && n.x >= n.z) index = (normal.x>0 ? 0 : 1);
		if (n.y >= n.z && n.y >= n.x) index = (normal.y>0 ? 2 : 3);
		if (n.z >= n.x && n.z >= n.y) index = (normal.z>0 ? 4 : 5);

		return getFace(index);
	}

	/////////////////////////////////////////////////////////////
	CubeTextureFace CubeTexture::operator[](const fm::vec3 &normal)
	{
		return getFace(normal);
	}

	/////////////////////////////////////////////////////////////
	CubeTexture::reference CubeTexture::setRepeated(bool repeat)
	{
		if (repeat != m_isRepeated)
		{
			m_isRepeated = repeat;

			if (getGlId())
			{
				GLint boundTex;
				glCheck(glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTex));
				{
					glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,getGlId()));

					// set TEXTURE_WRAP
					glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
				}
				glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,boundTex));
			}
		}

		return *this;
	}

	/////////////////////////////////////////////////////////////
	CubeTexture::reference CubeTexture::setSmooth(bool smooth)
	{
		if (smooth != m_isSmooth)
		{
			m_isSmooth = smooth;

			if (getGlId())
			{
				GLint boundTex;
				glCheck(glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTex));
				{
					glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,getGlId()));

					// set TEXTURE_FILTER
					glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
					glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
				}
				glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,boundTex));
			}
		}

		return *this;
	}

	/////////////////////////////////////////////////////////////
	bool CubeTexture::isRepeated() const
	{
		return m_isRepeated;
	}

	/////////////////////////////////////////////////////////////
	bool CubeTexture::isSmooth() const
	{
		return m_isSmooth;
	}

	/////////////////////////////////////////////////////////////
	void CubeTexture::bind() const
	{
		glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,getGlId()));
	}

	/////////////////////////////////////////////////////////////
	void CubeTexture::bind(const CubeTexture *texture)
	{
		if (texture)
			texture->bind();
		else
			glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,0));
	}

	/////////////////////////////////////////////////////////////
	void CubeTexture::bind(const CubeTexture &texture)
	{
		texture.bind();
	}

	/////////////////////////////////////////////////////////////
	fm::Size CubeTexture::getMaximumSize()
	{
		GLint size;
		glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE,&size));

		return size;
	}
}
