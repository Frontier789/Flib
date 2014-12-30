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
#include <FRONTIER/OpenGL.hpp>
namespace fg
{
	/// constructors /////////////////////////////////////////////////////////
	Buffer::Buffer() : m_type(ArrayBuffer),
					   m_usage(StaticDraw)
	{

	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(BufferType type,Buffer::Usage usage) : m_type(type),
														  m_usage(usage)
	{

	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(Buffer::Usage usage,BufferType type) : m_type(type),
														  m_usage(usage)
	{
		
	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(const void *data,fm::Size bytesToCopy,BufferType type,Buffer::Usage usage)
	{
		setData(data,bytesToCopy,type,usage);
	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(const void *data,fm::Size bytesToCopy,Buffer::Usage usage,BufferType type)
	{
		setData(data,bytesToCopy,type,usage);
	}

	/// destructors /////////////////////////////////////////////////////////
	Buffer::~Buffer()
	{
		if (getGlId() && glIsBufferARB(getGlId()))
			glCheck(glDeleteBuffersARB(1,&getGlId()));
		
	}

	/// functions /////////////////////////////////////////////////////////
	void Buffer::init()
	{
		if (!glIsBufferARB(getGlId()))
			glCheck(glGenBuffersARB(1,&getGlId()));
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::setData(const void *data,fm::Size bytesToCopy)
	{
		return setData(data,bytesToCopy,m_type,m_usage);
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::setData(const void *data,fm::Size bytesToCopy,Buffer::Usage usage)
	{
		return setData(data,bytesToCopy,m_type,usage);
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::setData(const void *data,fm::Size bytesToCopy,BufferType type)
	{
		return setData(data,bytesToCopy,type,m_usage);
	}

	////////////////////////////////////////////////////////////
	unsigned int typeToGetBinding(BufferType type)
	{
		return type == IndexBuffer ? GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB : 
									 GL_ARRAY_BUFFER_BINDING_ARB;
	}

	////////////////////////////////////////////////////////////
	unsigned int getFallBackUsage(Buffer::Usage usage)
	{
		if (usage==GL_STATIC_READ_ARB) return GL_STATIC_DRAW_ARB;
		if (usage==GL_STATIC_COPY_ARB) return GL_STATIC_DRAW_ARB;
		           
		if (usage==GL_DYNAMIC_DRAW_ARB) return GL_STATIC_DRAW_ARB;
		if (usage==GL_DYNAMIC_READ_ARB) return GL_STATIC_READ_ARB;
		if (usage==GL_DYNAMIC_COPY_ARB) return GL_STATIC_COPY_ARB;
		           
		if (usage==GL_STREAM_DRAW_ARB) return GL_STATIC_DRAW_ARB;
		if (usage==GL_STREAM_READ_ARB) return GL_STATIC_READ_ARB;
		if (usage==GL_STREAM_COPY_ARB) return GL_STATIC_COPY_ARB;
		
		return 0;
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::setData(const void *data,fm::Size bytesToCopy,BufferType type,Buffer::Usage usage)
	{
		m_type = type;
		m_usage = usage;
		init();
		if (!getGlId())
			return *this;
		
		int boundBuffer=0;
		
		glCheck(glGetIntegerv(typeToGetBinding(m_type),&boundBuffer));
		glCheck(glBindBufferARB(m_type,getGlId()));
		
		glBufferDataARB(m_type,bytesToCopy,data,m_usage);
		
		unsigned int errCode = glGetError();
		if (errCode)
		{
			if (errCode == GL_OUT_OF_MEMORY)
				fg_log << "Error: couldn't set data of the buffer because OpenGL is out of memory" << std::endl;
			if (errCode == GL_INVALID_ENUM)
				glCheck(glBufferDataARB(m_type,bytesToCopy,data,getFallBackUsage(m_usage)));
		}
		

		glCheck(glBindBufferARB(m_type,boundBuffer));
		return *this;
	}

	////////////////////////////////////////////////////////////
	Buffer::reference Buffer::updateData(const void *data,fm::Size bytesToCopy,fm::Size byteOffset)
	{
		init();
		if (!getGlId())
			return *this;
		int boundBuffer=0;
		
		glCheck(glGetIntegerv(typeToGetBinding(m_type),&boundBuffer));
		
		glCheck(glBindBufferARB(m_type,getGlId()));
		glCheck(glBufferSubDataARB(m_type,byteOffset,bytesToCopy,data));
		
		glCheck(glBindBufferARB(m_type,boundBuffer));
		
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
		glCheck(glBindBufferARB(targetType,buffer ? buffer->getGlId() : 0));
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
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB,&testBound);
		return (glGetError()==GL_NO_ERROR);
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