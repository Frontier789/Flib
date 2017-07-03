////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)		   ///
///																	   ///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			   ///
/// condition that this disclaimer is not modified/removed.			   ///
/// You may not misclaim the origin of this software.				   ///
///																	   ///
/// If you use this software in your program/project a				   ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							   ///
///																	   ///
/// You should have received a copy of GNU GPL with this software	   ///
///																       ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>

namespace fg
{
	//////////////////////////////////////////////////////////////////////////
	Attribute::Attribute(fm::Size components,
						 fm::Size stride,
						 fm::Size count,
						 fm::Size componentType,
						 fg::Buffer *buffer,
						 bool ownBuffer,
						 fg::Buffer::Usage bufferUsage) : bufferUsage(bufferUsage),
														  componentType(componentType),
														  components(components),
														  buf(buffer ? buffer : new fg::Buffer),
														  stride(stride),
														  count(count),
														  ownBuffer(buffer ? ownBuffer : true)
	{

	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	//////////////////////////////////////////////////////////////////////////
	Attribute::Attribute(const Attribute &attr) : buf(nullptr),
												  ownBuffer(false)
	{
		(*this) = attr;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	Attribute::Attribute(Attribute &&attr) : buf(nullptr),
											 ownBuffer(false)
	{
		attr.swap(*this);
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	//////////////////////////////////////////////////////////////////////////
	Attribute &Attribute::operator=(const Attribute &attr)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		bufferUsage   = attr.bufferUsage;
		componentType = attr.componentType;
		components    = attr.components;
		stride = attr.stride;
		count  = attr.count;
		
		if (ownBuffer) delete buf;
		
		ownBuffer = attr.ownBuffer;
		
		if (attr.buf)
		{
			if (ownBuffer)
			{
				buf = new fg::Buffer;
				(*buf) = (*attr.buf);
			}
			else buf = attr.buf;
		}
		else buf = nullptr;

		return *this;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	Attribute &Attribute::operator=(Attribute &&attr)
	{
		return this->swap(attr);
	}
	
	/////////////////////////////////////////////////////////////
	Attribute &Attribute::swap(Attribute &attr)
	{
		std::swap(bufferUsage,attr.bufferUsage);
		std::swap(componentType,attr.componentType);
		std::swap(components,attr.components);
		std::swap(ownBuffer,attr.ownBuffer);
		std::swap(stride,attr.stride);
		std::swap(count,attr.count);
		std::swap(buf,attr.buf);
		
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	Attribute &Attribute::set(fm::Size components,
							  fm::Size stride,
							  fm::Size count,
							  fm::Size componentType,
							  const void *pointer,
							  fm::Size bytesToCopy,
							  fg::Buffer::Usage bufferUsage)
	{
		this->bufferUsage = bufferUsage;
		this->components  = components;
		this->stride = stride;
		this->count  = count;
		this->componentType = componentType;
		if (!ownBuffer || !buf)
		{
			ownBuffer = true;
			buf = new fg::Buffer;
		}

		buf->setData(pointer,bytesToCopy,bufferUsage);

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	Attribute &Attribute::set(fm::Size components,
							  fm::Size stride,
							  fm::Size count,
							  fm::Size componentType,
							  fg::Buffer *buf,
							  bool ownBuffer,
							  fg::Buffer::Usage bufferUsage)
	{
		this->bufferUsage = bufferUsage;
		this->components  = components;
		this->stride = stride;
		this->count  = count;
		this->componentType = componentType;

		if (ownBuffer) delete buf;

		this->buf = buf;
		this->ownBuffer = ownBuffer;

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	Attribute::~Attribute()
	{
		if (ownBuffer)
			delete buf;
	}
}
