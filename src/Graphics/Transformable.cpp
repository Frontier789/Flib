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
#include <FRONTIER/Graphics/Transformable.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	Transformable::Transformable() : m_size(1,1)
	{
		
	}

	/////////////////////////////////////////////////////////////
	Transformable &Transformable::setPosition(fm::vec3 pos)
	{
		m_pos = pos;
		return *this;
	}

	fm::vec3 Transformable::getPosition() const
	{
		return m_pos;
	}


	/////////////////////////////////////////////////////////////
	Transformable &Transformable::setSize(fm::vec2 size)
	{
		m_size = size;
		return *this;
	}

	fm::vec2 Transformable::getSize() const
	{
		return m_size;
	}

	/////////////////////////////////////////////////////////////
	fm::mat4 Transformable::getMatrix() const
	{
		return fm::mat4(m_size.w,       0,0,m_pos.x,
							   0,m_size.h,0,m_pos.y,
							   0,       0,1,0,
							   0,       0,0,1);
	}
}
