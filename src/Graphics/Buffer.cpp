////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/SYstem/NullPtr.hpp>
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
		if (getGlId() && glIsBuffer(getGlId()))
			glCheck(glDeleteBuffers(1,&getGlId()));

	}

	/// functions /////////////////////////////////////////////////////////
	void Buffer::init()
	{
		if (!getGlId() || !glIsBuffer(getGlId()))
			glCheck(glGenBuffers(1,&getGlId()));
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
		return type == IndexBuffer ? GL_ELEMENT_ARRAY_BUFFER_BINDING:
									 GL_ARRAY_BUFFER_BINDING;
	}

	////////////////////////////////////////////////////////////
	unsigned int getFallBackUsage(Buffer::Usage usage)
	{
		if (usage==GL_STATIC_READ) return GL_STATIC_DRAW;
		if (usage==GL_STATIC_COPY) return GL_STATIC_DRAW;

		if (usage==GL_DYNAMIC_DRAW) return GL_STATIC_DRAW;
		if (usage==GL_DYNAMIC_READ) return GL_STATIC_READ;
		if (usage==GL_DYNAMIC_COPY) return GL_STATIC_COPY;

		if (usage==GL_STREAM_DRAW) return GL_STATIC_DRAW;
		if (usage==GL_STREAM_READ) return GL_STATIC_READ;
		if (usage==GL_STREAM_COPY) return GL_STATIC_COPY;

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
		glCheck(glBindBuffer(m_type,getGlId()));

		glBufferData(m_type,bytesToCopy,data,m_usage);

		unsigned int errCode = glGetError();
		if (errCode)
		{
			if (errCode == GL_OUT_OF_MEMORY)
				fg_log << "Error: couldn't set data of the buffer because OpenGL is out of memory" << std::endl;
			if (errCode == GL_INVALID_ENUM)
				glCheck(glBufferData(m_type,bytesToCopy,data,getFallBackUsage(m_usage)));
		}


		glCheck(glBindBuffer(m_type,boundBuffer));
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

		glCheck(glBindBuffer(m_type,getGlId()));
		glCheck(glBufferSubData(m_type,byteOffset,bytesToCopy,data));

		glCheck(glBindBuffer(m_type,boundBuffer));

		return *this;
	}

	/////////////////////////////////////////////////////////////
	void *Buffer::map(bool read,bool write)
	{
		if (!read && !write)
			return fm::nullPtr;

		bind();

		return glMapBuffer(m_type,read ? (write ? GL_READ_WRITE : GL_READ_ONLY) : (write ? GL_WRITE_ONLY : GL_NONE));
	}

	/////////////////////////////////////////////////////////////
	void Buffer::unMap()
	{
		bind();

		glUnmapBuffer(m_type);
	}


	/////////////////////////////////////////////////////////////
	BufferType Buffer::getType() const
	{
		return m_type;
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
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&testBound);
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
