#include <FRONTIER/Graphics/FrameBuffer.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/System/Log.hpp>
#include <FRONTIER/OpenGL.hpp>
class ObjectBinder
{
public:
	int m_oldId;
	int m_newId;
	ObjectBinder(int idToBeBound) : m_oldId(getBoundId()),
									m_newId(idToBeBound)
	{
		if (m_newId != m_oldId)
			glCheck(glBindFramebuffer(GL_FRAMEBUFFER,m_newId));
	}
	~ObjectBinder()
	{
		if (m_newId != m_oldId)
			glCheck(glBindFramebuffer(GL_FRAMEBUFFER,m_oldId));
	}
	static int getBoundId()
	{
		int ret;
		glCheck(glGetIntegerv(GL_FRAMEBUFFER_BINDING,&ret));
		return ret;
	}
};
bool checkFramebufferStatus(fg::FrameBuffer *fbo)
{
	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == 0x8CD6) ///GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
	{
		fg::fg_log << "Error: the framebuffer object has a incomplete attachment (invalid size?)" << std::endl;
		return 0;
	}
	if (status == 0x8CDD) ///GL_FRAMEBUFFER_UNSUPPORTED
	{
		fg::fg_log << "Error: FrameBuffers are not supported by implementation of OpenGL." << std::endl;
		return 0;
	}
	return 1;
}
namespace fg
{
	////////////////////////////////////////////////////////////
	FrameBuffer::DepthBuffer::DepthBuffer(const fm::vec2s &size) : width(*((std::size_t*)&size)),
																   height(*((std::size_t*)&size+1))
	{
		
	}

	////////////////////////////////////////////////////////////
	FrameBuffer::DepthBuffer::DepthBuffer(const std::size_t &w,const std::size_t &h) : width(w),
																					   height(h)
	{
		
	}

	////////////////////////////////////////////////////////////
	const FrameBuffer::DepthBuffer FrameBuffer::DepthBuffer::noDepthBuffer = FrameBuffer::DepthBuffer(0,0);

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer() : m_depthBufID(0)
	{
		
	}

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer(Texture *colorAttachments,unsigned int count,const DepthBuffer &depthBuf) : m_depthBufID(0)
	{
		setColorAttachments(colorAttachments,count);
		setDepthBuffer(depthBuf);
		glViewport(0,0,colorAttachments->getSize().w,colorAttachments->getSize().h);
	}

	////////////////////////////////////////////////////////////
	FrameBuffer::~FrameBuffer()
	{
		if (glIsRenderbuffer(m_depthBufID)==GL_TRUE)
			glCheck(glDeleteRenderbuffers(1,&m_depthBufID));
		if (glIsFramebuffer(getGlId())==GL_TRUE)
			glCheck(glDeleteFramebuffers(1,&getGlId()));
	}

	////////////////////////////////////////////////////////////
	bool FrameBuffer::setColorAttachments(Texture *colorAttachments,unsigned int count)
	{
		init();
		
		if (!colorAttachments || !count)
		{
			fg_log << "Error: no attachment is specified in framebuffer constructor" << std::endl;
			return;
		}
		
		ObjectBinder binder(getGlId());
		C(count)
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, (colorAttachments+i)->getGlId(), 0));
		GLenum *DrawBuffers;
		DrawBuffers = new GLenum[count];
		C(count)
			*(DrawBuffers+i) = GL_COLOR_ATTACHMENT0+i;
		glCheck(glDrawBuffers(count, DrawBuffers));
		delete[] DrawBuffers;
		return checkFramebufferStatus(this);
	}
	
	bool FrameBuffer::setDepthBuffer(const DepthBuffer &depthBuf)
	{
		if (depthBuf.width && depthBuf.height)
		{
			ObjectBinder binder(getGlId());
			if (glIsRenderbuffer(m_depthBufID)==GL_FALSE)
				glCheck(glGenRenderbuffers(1, &m_depthBufID));
			glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufID));
			glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, depthBuf.width, depthBuf.height));
			glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufID));			
		}
		return checkFramebufferStatus(this);
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::init() 
	{
		if (glIsFramebuffer(getGlId())==GL_FALSE)
			glCheck(glGenFramebuffers(1, &getGlId()));
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::bind() const
	{
		FrameBuffer::bind(this);
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::bind(const FrameBuffer *fbo)
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fbo ? fbo->getGlId() : 0));
	}

	////////////////////////////////////////////////////////////
	bool FrameBuffer::isAvailable()
	{
		#ifdef GL_FRAMEBUFFER_BINDING
			int testBound;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING,&testBound);
			return glGetError();
		#else
			return false;
		#endif
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::bind(const FrameBuffer &fbo)
	{
		bind(&fbo);
	}
}