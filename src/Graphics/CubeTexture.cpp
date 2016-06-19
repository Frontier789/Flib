#include <FRONTIER/Graphics/CubeTexture.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Error.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
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
	fm::Result CubeTextureFace::create(fm::Size width,fm::Size height)
	{
        if (m_cubeTex.getSize() != fm::math::max(width,height))
			return m_cubeTex.create(fm::math::max(width,height));

		return fm::Result();
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
	fm::Result CubeTexture::create(fm::Size size)
	{
		// check for empty texture
		if (!size)
			return fm::Error("CubeTextureError","CreatingEmptyTexture","Couldn't create CubeTexture, invalid texture size: (0)","CubeTexture.create",__FILE__,__LINE__);

		// opengl wouldn't accpet too big textures
		fm::Size maxSize = getMaximumSize();
		if (size > maxSize)
			return fm::Error("CubeTextureError","CreatingTooBigTexture","Couldn't create texture with size (" + fm::toString(size).str() + ")","CubeTexture.create",__FILE__,__LINE__);

		fm::Error err;

		// setup internal data
		m_size = size;

		if (glIsTexture(getGlId()) == GL_FALSE)
		{
			GLuint glId;
			err += glCheck(glGenTextures(1,&glId));
			getGlId() = glId;
		}

		// bind the texture for uplading
		GLint boundTex;
		glCheck(glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTex));
		{
			err += glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,getGlId()));

			// set TEXTURE_WRAP
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);

			if (glGetError() != GL_NO_ERROR)
			{
				m_isRepeated = !m_isRepeated;
				err += glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
				err += glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
				err += glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			}

			// set TEXTURE_FILTER
			err += glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));
			err += glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST));

			// upload data
			C(6)
				err += glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,getInternalFormat(),m_size,m_size,0,getFormat(),getType(),fm::nullPtr));
		}
		err += glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,boundTex));

		return err;
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
				glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTex);
				{
					glBindTexture(GL_TEXTURE_CUBE_MAP,getGlId());

					// set TEXTURE_WRAP
					glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
				}
				glBindTexture(GL_TEXTURE_CUBE_MAP,boundTex);
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
				glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTex);
				{
					glBindTexture(GL_TEXTURE_CUBE_MAP,getGlId());

					// set TEXTURE_FILTER
					glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,m_isSmooth ? GL_LINEAR : GL_NEAREST);
				}
				glBindTexture(GL_TEXTURE_CUBE_MAP,boundTex);
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
	fm::Result CubeTexture::bind() const
	{
		return glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,getGlId()));
	}

	/////////////////////////////////////////////////////////////
	fm::Result CubeTexture::bind(const CubeTexture *texture)
	{
		if (texture)
			return texture->bind();

		return glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,0));
	}

	/////////////////////////////////////////////////////////////
	fm::Result CubeTexture::bind(const CubeTexture &texture)
	{
		return texture.bind();
	}

	/////////////////////////////////////////////////////////////
	fm::Size CubeTexture::getMaximumSize()
	{
		GLint size;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&size);

		return size;
	}
}