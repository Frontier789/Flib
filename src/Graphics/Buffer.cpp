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
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <algorithm>

namespace fg
{
	namespace 
	{
		fm::Size bytesPerItem(fm::Size compType,fm::Size compCount)
		{
			if (compType == GL_UNSIGNED_SHORT) return compCount * sizeof(GLushort);
			if (compType == GL_UNSIGNED_BYTE)  return compCount * sizeof(GLubyte);
			if (compType == GL_UNSIGNED_INT)   return compCount * sizeof(GLuint);
			if (compType == GL_DOUBLE)         return compCount * sizeof(GLdouble);
			if (compType == GL_FLOAT)          return compCount * sizeof(GLfloat);
			if (compType == GL_SHORT)          return compCount * sizeof(GLshort);
			if (compType == GL_BYTE)           return compCount * sizeof(GLbyte);
			if (compType == GL_INT)            return compCount * sizeof(GLint);
			
			return 0;	
		}
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result Buffer::destroy()
	{
		if (getGlId()) {
			auto res = glCheck(glDeleteBuffers(1,&getGlId()));
			getGlId() = 0;
			return res;
		}
		
		return fm::Result();
	}
	
	////////////////////////////////////////////////////////////
	Buffer::Buffer() : m_type(ArrayBuffer),
					   m_usage(StaticDraw),
					   m_compType(0),
					   m_compCount(0),
					   m_byteCount(0)
	{

	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	Buffer::Buffer(const Buffer &buf) : m_type(ArrayBuffer),
										m_usage(StaticDraw),
										m_compType(0),
										m_compCount(0),
										m_byteCount(0)
	{
        (*this) = buf;
	}
#endif

	////////////////////////////////////////////////////////////
	Buffer::Buffer(Buffer &&buf) : m_type(ArrayBuffer),
								   m_usage(StaticDraw),
								   m_compType(0),
								   m_compCount(0),
								   m_byteCount(0)
	{
		buf.swap(*this);
	}

	////////////////////////////////////////////////////////////
	Buffer::Buffer(BufferType type,BufferUsage usage) : m_type(type),
														m_usage(usage),
														m_compType(0),
														m_compCount(0),
														m_byteCount(0)
	{

	}
	
	/////////////////////////////////////////////////////////////
	fm::Size Buffer::getCompType() const
	{
		return m_compType;
	} 
	
	/////////////////////////////////////////////////////////////
	fm::Size Buffer::getCompCount() const
	{
		return m_compCount;
	} 
	
	/////////////////////////////////////////////////////////////
	fm::Size Buffer::getItemCount() const
	{
		if (getItemSize() == 0) return 0;
		
		return m_byteCount / getItemSize();
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size Buffer::getItemSize() const
	{
		return bytesPerItem(m_compType,m_compCount);
	}

	/////////////////////////////////////////////////////////////
	Buffer &Buffer::setType(BufferType type)
	{
		m_type = type;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Buffer &Buffer::setUsage(BufferUsage usage)
	{
		m_usage = usage;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Buffer::~Buffer()
	{
		destroy();
	}

	////////////////////////////////////////////////////////////
	unsigned int typeToGetBinding(BufferType type)
	{
		if (type == ArrayBuffer) return GL_ARRAY_BUFFER_BINDING;
		if (type == IndexBuffer) return GL_ELEMENT_ARRAY_BUFFER_BINDING;
		if (type == ShaderStorageBuffer) return GL_SHADER_STORAGE_BUFFER_BINDING;
		
		return GL_NONE;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Buffer::setData(const void *ptr,fm::Size compType,fm::Size compCount,fm::Size byteCount)
	{
		init();
		
		GLint boundBuffer = 0;
        fm::Result res;

		res += glCheck(glGetIntegerv(typeToGetBinding(m_type),&boundBuffer));
		res += bind();

		glBufferData(m_type,byteCount,ptr,m_usage);

		unsigned int errCode = glGetError();
		if (errCode)
		{
			if (errCode == GL_OUT_OF_MEMORY)
                return fm::Result("GLError",fm::Result::OPFailed,"OutOfMemory","Buffer.setData",__FILE__,__LINE__,fm::toString(byteCount).str());
		}
		
		m_compType  = compType;
		m_compCount = compCount;
		m_byteCount = byteCount;

		res += glCheck(glBindBuffer(m_type,boundBuffer));

		return res;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Buffer::updateData(const void *ptr,fm::Size byteCount,fm::Size byteOffset)
	{
		if (!getGlId() || byteOffset >= m_byteCount) return fm::Result();
		
		byteCount = std::min(byteCount,m_byteCount - byteOffset);
		
		int boundBuffer = 0;
        fm::Result res;

		res += glCheck(glGetIntegerv(typeToGetBinding(m_type),&boundBuffer));

		res += bind();
		res += glCheck(glBufferSubData(m_type,byteOffset,byteCount,ptr));

		res += glCheck(glBindBuffer(m_type,boundBuffer));

		return res;
	}

	/////////////////////////////////////////////////////////////
	BufferType Buffer::getType() const
	{
		return m_type;
	}

	/////////////////////////////////////////////////////////////
	BufferUsage Buffer::getUsage() const
	{
		return m_usage;
	}

	////////////////////////////////////////////////////////////
	void Buffer::init()
	{
		if (!getGlId())
			glCheck(glGenBuffers(1,&getGlId()));
	}

	/////////////////////////////////////////////////////////////
	fm::Result Buffer::mapData(void *&ptr,BufferAccess access)
	{
		bind();

		ptr = glMapBuffer(m_type,access);
		
		return glCheck((void)0);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Buffer::unMap()
	{
		if (!getGlId()) return fm::Result();
		
		bind();

        return glCheck(glUnmapBuffer(m_type));
	}
	
	/////////////////////////////////////////////////////////////
	Buffer Buffer::makeCopy() const
	{
		Buffer ret;
		
	    ret.m_usage = m_usage;
	    ret.m_type  = m_type;
	    ret.m_compType  = m_compType;
	    ret.m_compCount = m_compCount;
	    ret.m_byteCount = m_byteCount;

        if (!m_byteCount) return ret;

		ret.init();

		glBindBuffer(GL_COPY_READ_BUFFER, getGlId());
		glBindBuffer(GL_COPY_WRITE_BUFFER, ret.getGlId());

		glBufferData(GL_COPY_WRITE_BUFFER, m_byteCount, nullptr, m_usage);

		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_byteCount);

		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

        return ret;
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	Buffer &Buffer::operator=(const Buffer &buf)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
        buf.makeCopy().swap(*this);
		
		return *this;
	}
#endif

	/////////////////////////////////////////////////////////////
	Buffer &Buffer::operator=(Buffer &&buf)
	{
		return this->swap(buf);
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::bind(BufferType type) const
	{
		return Buffer::bind(this,type);
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::bind() const
	{
		return Buffer::bind(this,m_type);
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::bind(fm::Ref<const Buffer> buffer,BufferType type)
	{
		return glCheck(glBindBuffer(type,buffer ? buffer->getGlId() : 0));
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::bind(const Buffer &buffer)
	{
		return buffer.bind();
	}

	////////////////////////////////////////////////////////////
	fm::Result Buffer::unBind(BufferType type)
	{
		return bind(nullptr,type);
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
		std::swap(getGlId(),buf.getGlId());
		std::swap(m_type   ,buf.m_type   );
		std::swap(m_usage  ,buf.m_usage  );
		std::swap(m_compType  ,buf.m_compType  );
		std::swap(m_compCount ,buf.m_compCount );
		std::swap(m_byteCount ,buf.m_byteCount );
		
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
