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
#include <FRONTIER/Graphics/GlObject.hpp>
namespace fg
{
	//////////////////////////////////
	GlObject::GlObject() : m_id(0u)
	{
		
	}
	
	//////////////////////////////////
	GlObject::~GlObject()
	{
		
	}

	//////////////////////////////////
	unsigned int &GlObject::getGlId()
	{
		return m_id;
	}
	
	//////////////////////////////////
	const unsigned int &GlObject::getGlId() const
	{
		return m_id;
	}
}