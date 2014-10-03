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
#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/Config.hpp>
#include <FRONTIER/OpenGL.hpp>
namespace fg
{

	BufferType ArrayBuffer    = (BufferType)(GL_ARRAY_BUFFER        );
	BufferType IndexBuffer    = (BufferType)(GL_ELEMENT_ARRAY_BUFFER);
	Usage Buffer::StaticDraw  = (Usage)(GL_STATIC_DRAW );
	Usage Buffer::StaticRead  = (Usage)(GL_STATIC_READ );
	Usage Buffer::StaticCopy  = (Usage)(GL_STATIC_COPY );
	Usage Buffer::DynamicDraw = (Usage)(GL_DYNAMIC_DRAW);
	Usage Buffer::DynamicRead = (Usage)(GL_DYNAMIC_READ);
	Usage Buffer::DynamicCopy = (Usage)(GL_DYNAMIC_COPY);
	Usage Buffer::StreamDraw  = (Usage)(GL_STREAM_DRAW );
	Usage Buffer::StreamRead  = (Usage)(GL_STREAM_READ );
	Usage Buffer::StreamCopy  = (Usage)(GL_STREAM_COPY );
		
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
		if (glIsBuffer(getGlId()))
			glCheck(glDeleteBuffers(1,&getGlId()));
		
	}

	/// functions /////////////////////////////////////////////////////////
	void Buffer::init()
	{
		if (!glIsBuffer(getGlId()))
			glCheck(glGenBuffers(1,&getGlId()));
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
		
		glCheck(glGetIntegerv(typeToGetBinding(m_type),&boundBuffer));
		glCheck(glBindBuffer(m_type,getGlId()));
		
		glBufferData(m_type,bytesToCopy,data,m_usage);
		
		unsigned int errCode = glGetError();
		if (errCode)
		{
			if (errCode==0x505) ///GL_OUT_OF_MEMORY
				fg_log << "Error: couldn't set data of the buffer because OpenGL is out of memory" << std::endl;
			if (errCode==0x500) ///GL_INVALID_ENUM
				glCheck(glBufferData(m_type,bytesToCopy,data,getFallBackUsage(m_usage)));
		}
		

		glCheck(glBindBuffer(m_type,boundBuffer));
		return *this;
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::updateData(const void *data,unsigned int bytesToCopy,unsigned int byteOffset)
	{
		init();
		if (!getGlId())
			return *this;
		int boundBuffer=0;
		
		glCheck(glGetIntegerv(typeToGetBinding(m_type),&boundBuffer));
		
		glCheck(glBindBuffer(m_type,getGlId()));
		glCheck(glBufferSubData(m_type,byteOffset,bytesToCopy,data));
		
		glCheck(glBindBuffer(m_type,boundBuffer));
		
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
		glCheck(glBindBuffer(targetType,buffer ? buffer->getGlId() : 0));
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
		glGetIntegerv(0x8894, ///GL_ARRAY_BUFFER_BINDING
							  &testBound);
		return (glGetError()==0x000);
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