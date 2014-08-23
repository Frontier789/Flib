#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/Config.hpp>
#include "glExtensionWrap.hpp"
namespace fg
{

	BufferType ArrayBuffer    = (BufferType)(0x8892); ///GL_ARRAY_BUFFER
	BufferType IndexBuffer    = (BufferType)(0x8893); ///GL_ELEMENT_ARRAY_BUFFER
	Usage Buffer::StaticDraw  = (Usage)(0x88E4); ///GL_STATIC_DRAW
	Usage Buffer::StaticRead  = (Usage)(0x88E5); ///GL_STATIC_READ
	Usage Buffer::StaticCopy  = (Usage)(0x88E6); ///GL_STATIC_WRITE
	Usage Buffer::DynamicDraw = (Usage)(0x88E8); ///GL_DYNAMIC_DRAW
	Usage Buffer::DynamicRead = (Usage)(0x88E9); ///GL_DYNAMIC_READ
	Usage Buffer::DynamicCopy = (Usage)(0x88EA); ///GL_DYNAMIC_WRITE
	Usage Buffer::StreamDraw  = (Usage)(0x88E0); ///GL_STREAM_DRAW
	Usage Buffer::StreamRead  = (Usage)(0x88E1); ///GL_STREAM_READ
	Usage Buffer::StreamCopy  = (Usage)(0x88E2); ///GL_STREAM_WRITE
		
	/// constructors /////////////////////////////////////////////////////////
	Buffer::Buffer() : m_type(ArrayBuffer),
					   m_usage(StaticDraw)
	{

	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(BufferType type,Usage usage) : m_type(type),
												  m_usage(usage)
	{

	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(Usage usage,BufferType type) : m_type(type),
												  m_usage(usage)
	{
		
	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(const void *data,unsigned int bytesToCopy,BufferType type,Usage usage)
	{
		setData(data,bytesToCopy,type,usage);
	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(const void *data,unsigned int bytesToCopy,Usage usage,BufferType type)
	{
		setData(data,bytesToCopy,type,usage);
	}

	/// destructors /////////////////////////////////////////////////////////
	Buffer::~Buffer()
	{
		if (glWrap::IsBuffer(getGlId()))
			glCheck(glWrap::DeleteBuffers(1,&getGlId()));
		
	}

	/// functions /////////////////////////////////////////////////////////
	void Buffer::init()
	{
		if (!glWrap::IsBuffer(getGlId()))
			glCheck(glWrap::GenBuffers(1,&getGlId()));
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::setData(const void *data,unsigned int bytesToCopy)
	{
		return setData(data,bytesToCopy,m_type,m_usage);
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::setData(const void *data,unsigned int bytesToCopy,Usage usage)
	{
		return setData(data,bytesToCopy,m_type,usage);
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::setData(const void *data,unsigned int bytesToCopy,BufferType type)
	{
		return setData(data,bytesToCopy,type,m_usage);
	}

	////////////////////////////////////////////////////////////
	unsigned int typeToGetBinding(BufferType type)
	{
		return type == IndexBuffer ? 0x8895 : ///GL_ELEMENT_ARRAY_BUFFER
									 0x8894;  ///GL_ARRAY_BUFFER_BINDING 
	}

	////////////////////////////////////////////////////////////
	unsigned int getFallBackUsage(Usage usage)
	{
		if (usage==0x88E5) return 0x88E4;
		if (usage==0x88E6) return 0x88E4;
		
		if (usage==0x88E8) return 0x88E4;
		if (usage==0x88E9) return 0x88E5;
		if (usage==0x88EA) return 0x88E6;
		
		if (usage==0x88E0) return 0x88E4;
		if (usage==0x88E1) return 0x88E5;
		if (usage==0x88E2) return 0x88E6;
		
		return 0;
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::setData(const void *data,unsigned int bytesToCopy,BufferType type,Usage usage)
	{
		m_type = type;
		m_usage = usage;
		init();
		if (!getGlId())
			return *this;
		
		int boundBuffer=0;
		
		glCheck(glWrap::GetIntegerv(typeToGetBinding(m_type),&boundBuffer));
		glCheck(glWrap::BindBuffer(m_type,getGlId()));
		
		glWrap::BufferData(m_type,bytesToCopy,data,m_usage);
		
		unsigned int errCode = glWrap::GetError();
		if (errCode)
		{
			if (errCode==0x505) ///GL_OUT_OF_MEMORY
				fg_log << "Error: couldn't set data of the buffer because OpenGL is out of memory" << std::endl;
			if (errCode==0x500) ///GL_INVALID_ENUM
				glCheck(glWrap::BufferData(m_type,bytesToCopy,data,getFallBackUsage(m_usage)));
		}
		

		glCheck(glWrap::BindBuffer(m_type,boundBuffer));
		return *this;
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::updateData(const void *data,unsigned int bytesToCopy,unsigned int byteOffset)
	{
		init();
		if (!getGlId())
			return *this;
		int boundBuffer=0;
		
		glCheck(glWrap::GetIntegerv(typeToGetBinding(m_type),&boundBuffer));
		
		glCheck(glWrap::BindBuffer(m_type,getGlId()));
		glCheck(glWrap::BufferSubData(m_type,byteOffset,bytesToCopy,data));
		
		glCheck(glWrap::BindBuffer(m_type,boundBuffer));
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	void Buffer::bind(BufferType targetType) const
	{
		Buffer::bind(this,targetType);
	}

	////////////////////////////////////////////////////////////
	void Buffer::bind() const
	{
		Buffer::bind(this,m_type);
	}

	////////////////////////////////////////////////////////////
	void Buffer::bind(const Buffer &buffer,BufferType targetType)
	{
		bind(&buffer,targetType);
	}

	////////////////////////////////////////////////////////////
	void Buffer::bind(const Buffer *buffer,BufferType targetType)
	{
		glCheck(glWrap::BindBuffer(targetType,buffer ? buffer->getGlId() : 0));
	}

	////////////////////////////////////////////////////////////
	void Buffer::bind(const Buffer &buffer)
	{
		buffer.bind();
	}

	////////////////////////////////////////////////////////////
	void Buffer::unBind(BufferType targetType)
	{
		bind(0,targetType);
	}

	////////////////////////////////////////////////////////////
	bool Buffer::isAvailable()
	{
		int testBound;
		glWrap::GetIntegerv(0x8894, ///GL_ARRAY_BUFFER_BINDING
							  &testBound);
		return (glWrap::GetError()==0x000);
	}

	////////////////////////////////////////////////////////////
	bool operator==(const Buffer &left,const Buffer &right)
	{
		return left.getGlId() == right.getGlId();
	}

	////////////////////////////////////////////////////////////
	bool operator!=(const Buffer &left,const Buffer &right)
	{
		return left.getGlId() != right.getGlId();
	}
}