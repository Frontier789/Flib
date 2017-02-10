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
#include <FRONTIER/Graphics/IndexHolder.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <algorithm>

namespace fg
{
	/////////////////////////////////////////////////////////////
	IndexHolder::IndexHolder(fm::Size N,bool m_use16bits) : m_use16bits(m_use16bits),
															m_size(N)
	{
		if (m_use16bits)
			m_ptr = new fm::Uint16[m_size];
		else
			m_ptr = new fm::Uint32[m_size];
	}
		
	/////////////////////////////////////////////////////////////
	IndexHolder::IndexHolder(const fm::Uint32 *arrayToCopy,fm::Size indexCount) : m_use16bits(true),
																				  m_size(indexCount)
	{
		C(m_size)
			if (arrayToCopy[i] > fm::Uint32(fm::Uint16(-1)))
			{
				m_use16bits = false;
				break;
			}
		
		if (m_use16bits)
		{
			m_ptr = new fm::Uint16[m_size];
			C(m_size)
				((fm::Uint16*)m_ptr)[i] = (fm::Uint16)arrayToCopy[i];
		}
		else
		{
			m_ptr = new fm::Uint32[m_size];
			std::copy(arrayToCopy,arrayToCopy+indexCount,(fm::Uint32*)m_ptr);
		}
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	IndexHolder::IndexHolder(const IndexHolder &other) : m_use16bits(other.m_use16bits),
														 m_size(other.m_size)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		if (m_use16bits)
		{
			m_ptr = new fm::Uint16[m_size];
			std::copy((fm::Uint16*)other.m_ptr,(fm::Uint16*)other.m_ptr + other.m_size,(fm::Uint16*)m_ptr);
		}
		else
		{
			m_ptr = new fm::Uint32[m_size];
			std::copy((fm::Uint32*)other.m_ptr,(fm::Uint32*)other.m_ptr + other.m_size,(fm::Uint32*)m_ptr);
		}
	}
#endif

	/////////////////////////////////////////////////////////////
	IndexHolder::~IndexHolder()
	{
		if (m_use16bits)
			delete[] (fm::Uint16*)m_ptr;
		else
			delete[] (fm::Uint32*)m_ptr;
	}

	/////////////////////////////////////////////////////////////
	void IndexHolder::set(fm::Size index,fm::Uint32 val)
	{
		if (val > fm::Uint32(fm::Uint16(-1)) && m_use16bits)
		{
			fm::Uint32 *newptr = new fm::Uint32[m_size];
			C(m_size)
				newptr[i] = ((fm::Uint16*)m_ptr)[i];
			
			delete[] (fm::Uint16*)m_ptr;
			
			m_ptr = newptr;
			
			m_use16bits = false;
		}
		
		if (m_use16bits)
			((fm::Uint16*)m_ptr)[index] = val;
		else
			((fm::Uint32*)m_ptr)[index] = val;
	}

	/////////////////////////////////////////////////////////////
	fm::Uint32 IndexHolder::get(fm::Size index) const
	{
		if (m_use16bits)
			return ((fm::Uint16*)m_ptr)[index];
		else
			return ((fm::Uint32*)m_ptr)[index];
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size IndexHolder::getSize() const
	{
		return m_size;
	}
	
	/////////////////////////////////////////////////////////////
	bool IndexHolder::doUse16bits() const
	{
		return m_use16bits;
	}
	
	/////////////////////////////////////////////////////////////
	const void *IndexHolder::getPtr() const
	{
		return m_ptr;
	}
	
	/////////////////////////////////////////////////////////////
	void *IndexHolder::getPtr()
	{
		return m_ptr;
	}
		
	/////////////////////////////////////////////////////////////
	IndexHolder::reference IndexHolder::swap(IndexHolder &arr)
	{
		std::swap(m_ptr      ,arr.m_ptr      );
		std::swap(m_use16bits,arr.m_use16bits);
		std::swap(m_size     ,arr.m_size     );
		
		return *this;
	}
}
