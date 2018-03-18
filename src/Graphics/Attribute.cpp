////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)		   ///
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
	/////////////////////////////////////////////////////////////
	Attribute::Attribute() : instancesPerUpdate(0),
							 compCount(0),
							 compType(0),
							 buf(nullptr),
							 stride(0),
							 offset(0),
							 count(0),
							 ownBuffer(false)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Attribute::Attribute(std::nullptr_t) : Attribute()
	{
		
	}

	/////////////////////////////////////////////////////////////
	Attribute::Attribute(fg::Buffer &buf,fm::Size instancesPerUpdate) : 
		instancesPerUpdate(instancesPerUpdate),
		compCount(buf.getCompCount()),
		compType(buf.getCompType()),
		buf(&buf),
		stride(0),
		offset(0),
		count(buf.getItemCount()),
		ownBuffer(false)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Attribute::~Attribute()
	{
		if (ownBuffer)
			delete buf;
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	Attribute::Attribute(const Attribute &attr) : 
		instancesPerUpdate(attr.instancesPerUpdate),
		compCount(attr.compCount),
		compType(attr.compType),
		buf(attr.buf),
		stride(attr.stride),
		offset(attr.offset),
		count(attr.count),
		ownBuffer(attr.ownBuffer)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		if (ownBuffer)
		{
			Buffer *tmp = buf;
			buf = new Buffer;
			
			buf->swap(tmp->makeCopy());
		}
	}
#endif

	/////////////////////////////////////////////////////////////
	Attribute::Attribute(Attribute &&attr) : 
		instancesPerUpdate(0),
		compCount(0),
		compType(0),
		buf(nullptr),
		stride(0),
		offset(0),
		count(0),
		ownBuffer(false)
	{
		attr.swap(*this);
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	Attribute &Attribute::operator=(const Attribute &attr)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		Buffer b = attr;
		return this->swap(b);
	}
#endif

	/////////////////////////////////////////////////////////////
	Attribute &Attribute::operator=(Attribute &&attr)
	{
		return this->swap(attr);
	}

	/////////////////////////////////////////////////////////////
	Attribute &Attribute::swap(Attribute &attr)
	{
		std::swap(instancesPerUpdate,attr.instancesPerUpdate);
		std::swap(compCount,attr.compCount);
		std::swap(compType,attr.compType);
		std::swap(buf,attr.buf);
		std::swap(stride,attr.stride);
		std::swap(offset,attr.offset);
		std::swap(count,attr.count);
		std::swap(ownBuffer,attr.ownBuffer);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Attribute &Attribute::operator=(fg::Buffer &buf)
	{
		instancesPerUpdate = 0;
		compCount = buf.getCompCount();
		compType  = buf.getCompType();
		this->buf = &buf;
		stride    = 0;
		offset    = 0;
		count     = buf.getItemCount();
		ownBuffer = false;
		
		return *this;
	}
}
