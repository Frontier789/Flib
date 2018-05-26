#include <FRONTIER/Graphics/CubeTexture.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/Graphics/Color.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <algorithm>

namespace fg
{
	/////////////////////////////////////////////////////////////
	fm::Uint32 CubeTextureFace::getTexRebinding() const {return GL_TEXTURE_CUBE_MAP;}
	fm::Uint32 CubeTextureFace::getTexBinding() const {return GL_TEXTURE_BINDING_CUBE_MAP;}
	fm::Uint32 CubeTextureFace::getTexTarget() const {return GL_TEXTURE_CUBE_MAP_POSITIVE_X + m_faceId;}

	/////////////////////////////////////////////////////////////
	CubeTextureFace::CubeTextureFace(CubeTexture &cubeTex,unsigned int faceId) : m_cubeTex(cubeTex),
																				 m_faceId(faceId % 6)
	{
		m_size = fm::vec2(cubeTex.getSize(),cubeTex.getSize());
		m_isRepeated = cubeTex.isRepeated();
		m_isSmooth   = cubeTex.isSmooth();
		getGlId()    = m_cubeTex.getGlId();
	}

	/////////////////////////////////////////////////////////////
	CubeTextureFace::CubeTextureFace(const CubeTextureFace &face) : Texture(),
																	m_cubeTex(face.m_cubeTex),
																	m_faceId(face.m_faceId)
	{
		m_size       = face.m_size;
		m_isRepeated = face.m_isRepeated;
		m_isSmooth   = face.m_isSmooth;
		getGlId()    = face.getGlId();
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result CubeTextureFace::copyFace(const CubeTexture &cubeTex,unsigned int face)
	{
		CubeTextureFace tmpFace(m_cubeTex,face);
		tmpFace.getGlId() = cubeTex.getGlId();
		tmpFace.m_faceId  = face % 6;
		
		return loadFromImage(tmpFace.copyToImage());
	}

	/////////////////////////////////////////////////////////////
	CubeTextureFace::~CubeTextureFace()
	{
		getGlId() = 0;
	}

	/////////////////////////////////////////////////////////////
	fm::Result CubeTextureFace::create(fm::vec2s size)
	{
        if (m_cubeTex.getSize() != std::max(size.w,size.h)) 
		{
			fm::Result res = m_cubeTex.create(std::max(size.w,size.h));
			
			getGlId() = m_cubeTex.getGlId();
			
			return res;
		}

		return fm::Result();
	}

	/////////////////////////////////////////////////////////////
	Texture::reference CubeTextureFace::setRepeated(bool repeat)
	{
		m_isRepeated = repeat;
		m_cubeTex.setRepeated(repeat);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Texture::reference CubeTextureFace::setSmooth(bool smooth)
	{
		m_isSmooth = smooth;
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

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	CubeTexture::CubeTexture(const CubeTexture &cubeTex) : m_size(0),
														   m_isRepeated(false),
														   m_isSmooth(false)
	{
		(*this) = cubeTex;
	}
#endif

	/////////////////////////////////////////////////////////////
	CubeTexture::CubeTexture(CubeTexture &&cubeTex) : m_size(0),
													  m_isRepeated(false),
													  m_isSmooth(false)
	{
		this->swap(cubeTex);
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	CubeTexture &CubeTexture::operator=(const CubeTexture &cubeTex)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		if (cubeTex.getGlId())
		{
			create(cubeTex.getSize());
			C(6)
				getFace(i).copyFace(cubeTex,i);
		}
		else
		{
			if (getGlId() && glIsTexture(getGlId()) == GL_TRUE)
				glDeleteTextures(1,&getGlId());
			
			getGlId() = 0;
			m_size    = 0;
		}
		
		setRepeated(cubeTex.m_isRepeated);
		setSmooth(cubeTex.m_isSmooth);
	}
#endif

	/////////////////////////////////////////////////////////////
	CubeTexture &CubeTexture::operator=(CubeTexture &&cubeTex)
	{
		return this->swap(cubeTex);
	}
	
	/////////////////////////////////////////////////////////////
	CubeTexture &CubeTexture::swap(CubeTexture &cubeTex)
	{
		std::swap(m_isRepeated,cubeTex.m_isRepeated);
		std::swap(m_isSmooth,cubeTex.m_isSmooth);
		std::swap(getGlId(),cubeTex.getGlId());
		std::swap(m_size,cubeTex.m_size);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	CubeTexture::~CubeTexture()
	{
		if (getGlId())
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
			return fm::Result("CubeTextureError",fm::Result::OPFailed,"CreatingEmptyTexture","CubeTexture.create",__FILE__,__LINE__);

		// opengl wouldn't accpet too big textures
		fm::Size maxSize = getMaximumSize();
		if (size > maxSize)
			return fm::Result("CubeTextureError",fm::Result::OPFailed,"CreatingTooBigTexture","CubeTexture.create",__FILE__,__LINE__,fm::toString(size).str());

		fm::Result err;

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
				err += glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,getInternalFormat(),m_size,m_size,0,getFormat(),getType(),nullptr));
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
		fm::vec3 n = normal.unsign();

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
	fm::Result CubeTexture::loadFromFile(const std::string &file)
	{
		fg::Image img;
		fm::Result res = img.loadFromFile(file);
		
		if (!res) return res;

		fm::vec2 startPts[] = {fm::vec2(2,1),fm::vec2(0,1),fm::vec2(1,0),fm::vec2(1,2),fm::vec2(1,1),fm::vec2(3,1)};
		fm::vec2 tileSize = img.getSize()/fm::vec2(4,3);

		create(tileSize.w);
		
		C(6)
			res += getFace(i).loadFromImage(img.getSubImage(startPts[i]*tileSize,tileSize));
		
		return res;
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
	fm::Result CubeTexture::bind(fm::Ref<const CubeTexture> texture)
	{
		if (texture)
			return texture->bind();

		return glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,0));
	}

	/////////////////////////////////////////////////////////////
	fm::Size CubeTexture::getMaximumSize()
	{
		GLint size;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&size);

		return size;
	}
}
