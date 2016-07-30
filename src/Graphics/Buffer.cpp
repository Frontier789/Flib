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
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Error.hpp>
#include <FRONTIER/System/Swap.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <cstring>

namespace fg
{
	/// constructors /////////////////////////////////////////////////////////
	Buffer::Buffer() : m_type(ArrayBuffer),
					   m_usage(StaticDraw),
					   m_data(fm::nullPtr),
					   m_size(0)
	{

	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(const Buffer &buf) : m_data(fm::nullPtr),
                                        m_size(0)
	{
        (*this) = buf;
	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(BufferType type,Buffer::Usage usage) : m_type(type),
														  m_usage(usage),
                                                          m_data(fm::nullPtr),
                                                          m_size(0)
	{

	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(Buffer::Usage usage,BufferType type) : m_type(type),
														  m_usage(usage),
                                                          m_data(fm::nullPtr),
                                                          m_size(0)
	{

	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(const void *data,fm::Size bytesToCopy,BufferType type,Buffer::Usage usage) : m_data(fm::nullPtr),
                                                                                                m_size(0)
	{
		setData(data,bytesToCopy,type,usage);
	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(const void *data,fm::Size bytesToCopy,Buffer::Usage usage,BufferType type) : m_data(fm::nullPtr),
                                                                                                m_size(0)
	{
		setData(data,bytesToCopy,type,usage);
	}

	/// destructors /////////////////////////////////////////////////////////
	Buffer::~Buffer()
	{
	    delete[] m_data;

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
	fm::Result Buffer::setData(const void *data,fm::Size bytesToCopy)
	{
		return setData(data,bytesToCopy,m_type,m_usage);
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::setData(const void *data,fm::Size bytesToCopy,Buffer::Usage usage)
	{
		return setData(data,bytesToCopy,m_type,usage);
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::setData(const void *data,fm::Size bytesToCopy,BufferType type)
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
	bool needShadowCopy()
	{
        if (::priv::so_loader.getProcAddr("glCopyBufferSubData"))
            return false;

        if (::priv::so_loader.getProcAddr("glMapBuffer"))
            return false;

        return true;
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::setData(const void *data,fm::Size bytesToCopy,BufferType type,Buffer::Usage usage)
	{
	    // copy attrs
		m_type  = type;
		m_usage = usage;

		// make sure buffer exists
		init();
		if (!getGlId())
			return fm::Error("BufferError","0ID","setData was called with 0 id","Buffer.setData",__FILE__,__LINE__);

        fm::Error err;

        // switch to this buffer
		int boundBuffer = 0;

		err += glCheck(glGetIntegerv(typeToGetBinding(m_type),&boundBuffer));
		err += glCheck(glBindBuffer(m_type,getGlId()));

        // copy data
		glBufferData(m_type,bytesToCopy,data,m_usage);

        // use fallback usage hint if needed
		unsigned int errCode = glGetError();
		if (errCode)
		{
			if (errCode == GL_OUT_OF_MEMORY)
                return fm::Error("GLError","OutOfMemory","OpenGL couldn't allocate " + fm::toString(bytesToCopy).str() + " bytes","Buffer.setData",__FILE__,__LINE__);
			if (errCode == GL_INVALID_ENUM)
				err += glCheck(glBufferData(m_type,bytesToCopy,data,getFallBackUsage(m_usage)));
		}

        // on old hardware use cpu shadowcopy
		if (needShadowCopy())
        {
            delete[] m_data;
            m_data = new fm::Uint8[bytesToCopy];
            std::memcpy(m_data,data,bytesToCopy);
        }

        m_size = bytesToCopy;

        // reset binding
		err += glCheck(glBindBuffer(m_type,boundBuffer));

		return err;
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::updateData(const void *data,fm::Size bytesToCopy,fm::Size byteOffset)
	{
		// make sure buffer exists
		init();
		if (!getGlId())
			return fm::Error("BufferError","0ID","updateData was called with 0 id","Buffer.updateData",__FILE__,__LINE__);

        fm::Error err;

        // activate this buffer
		int boundBuffer = 0;

		err += glCheck(glGetIntegerv(typeToGetBinding(m_type),&boundBuffer));

        // upload data
		err += glCheck(glBindBuffer(m_type,getGlId()));
		err += glCheck(glBufferSubData(m_type,byteOffset,bytesToCopy,data));

		// on old hardware use cpu shadowcopy
		if (m_data)
            std::memcpy(m_data+byteOffset,data,bytesToCopy);

		err += glCheck(glBindBuffer(m_type,boundBuffer));

		return err;
	}

	/////////////////////////////////////////////////////////////
	void *Buffer::map(bool read,bool write)
	{
		if (!read && !write)
			return fm::nullPtr;

		bind();

        if (m_data) return m_data;

		return glMapBuffer(m_type,read ? (write ? GL_READ_WRITE : GL_READ_ONLY) : (write ? GL_WRITE_ONLY : GL_NONE));
	}

	/////////////////////////////////////////////////////////////
	void Buffer::unMap()
	{
		bind();

        if (m_data) return;

        glUnmapBuffer(m_type);
	}


	/////////////////////////////////////////////////////////////
	BufferType Buffer::getType() const
	{
		return m_type;
	}

	/////////////////////////////////////////////////////////////
	Buffer &Buffer::operator=(const Buffer &buf)
	{
        // copy attrs
	    m_usage = buf.m_usage;
	    m_type  = buf.m_type;
	    m_size  = buf.m_size;

        // reset data
        delete[] m_data;
        m_data = fm::nullPtr;

        // check if there is data
        if (!m_size) return *this;

		// make sure buffer exists
		init();
		if (!getGlId())
			return *this;

        // decide whether it's local data
        if (buf.m_data)
        {
            m_data = new fm::Uint8[m_size];
            std::memcpy(m_data,buf.m_data,m_size);
            setData(m_data,m_size);
        }
        else
        {
            glBindBuffer(GL_COPY_READ_BUFFER, buf.getGlId());
            glBindBuffer(GL_COPY_WRITE_BUFFER, getGlId());

            glBufferData(GL_COPY_WRITE_BUFFER, m_size, fm::nullPtr, m_usage);

            // Copy Data
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_size);

            glBindBuffer(GL_COPY_READ_BUFFER, 0);
            glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
        }

        return *this;
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::bind(BufferType targetType) const
	{
		return Buffer::bind(this,targetType);
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::bind() const
	{
		return Buffer::bind(this,m_type);
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::bind(const Buffer &buffer,BufferType targetType)
	{
		return bind(&buffer,targetType);
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::bind(const Buffer *buffer,BufferType targetType)
	{
		return glCheck(glBindBuffer(targetType,buffer ? buffer->getGlId() : 0));
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::bind(const Buffer &buffer)
	{
		return buffer.bind();
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::unBind(BufferType targetType)
	{
		return bind(fm::nullPtr,targetType);
	}

	////////////////////////////////////////////////////////////
	bool Buffer::isAvailable()
	{
		int testBound;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&testBound);
		return (glGetError() == GL_NO_ERROR);
	}
	
	/////////////////////////////////////////////////////////////
	Buffer::reference Buffer::swap(Buffer &buf)
	{
		fm::swap(getGlId(),buf.getGlId());
		fm::swap(m_type   ,buf.m_type   );
		fm::swap(m_usage  ,buf.m_usage  );
		fm::swap(m_data   ,buf.m_data   );
		fm::swap(m_size   ,buf.m_size   );
		
		return *this;
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
