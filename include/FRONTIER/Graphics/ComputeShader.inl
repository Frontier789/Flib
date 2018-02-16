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
#ifndef FRONTIER_COMPUTESHADER_INL_INCLUDED
#define FRONTIER_COMPUTESHADER_INL_INCLUDED

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Result ComputeShader::setUniform(const std::string &name,const T &x)
	{
		return m_shader.setUniform(name,x);
	}

	/////////////////////////////////////////////////////////////
	template<class T,fm::Size N>
	inline fm::Result ComputeShader::setUniform(const std::string &name,const T (&values)[N])
	{
		return m_shader.setUniform(name,values);
	}
}

#endif // FRONTIER_SHADER_INL_INCLUDED


