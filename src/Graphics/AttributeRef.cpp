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
///																	   ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/AttributeRef.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	AttributeRef::AttributeRef(DrawData &drawData,AssocPoint assoc) : m_drawData(drawData),
																	  m_assoc(assoc)
	{
		
	}																

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	AttributeRef &AttributeRef::operator=(const Attribute &attr)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		m_drawData.getAttribute(m_assoc) = attr;
		
		return *this;
	}
#endif

	/////////////////////////////////////////////////////////////
	AttributeRef &AttributeRef::operator=(Attribute &&attr)
	{
		m_drawData.getAttribute(m_assoc).swap(attr);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	AttributeRef &AttributeRef::swap(Attribute &attr)
	{
		m_drawData.getAttribute(m_assoc).swap(attr);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	AttributeRef &AttributeRef::set(fm::Size components,
									fm::Size stride,
									fm::Size count,
									fm::Size componentType,
									const void *pointer,
									fm::Size bytesToCopy)
	{
		m_drawData.getAttribute(m_assoc).set(components,stride,count,componentType,pointer,bytesToCopy);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	AttributeRef &AttributeRef::set(fm::Size components,
									fm::Size stride,
									fm::Size count,
									fm::Size componentType,
									fg::Buffer *buffer,
									bool ownBuffer)
	{
		m_drawData.getAttribute(m_assoc).set(components,stride,count,componentType,buffer,ownBuffer);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Attribute &AttributeRef::getData()
	{
		return m_drawData.getAttribute(m_assoc);
	}
}
