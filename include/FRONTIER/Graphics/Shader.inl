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
#ifndef FRONTIER_SHADER_INL_INCLUDED
#define FRONTIER_SHADER_INL_INCLUDED
#include <FRONTIER/System/String.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Result Shader::setAttribute(const std::string &name,fm::Ref<fg::Buffer> buf,fm::Size stride,fm::Size offset)
	{
		return m_vao.setAttribute<T>(getAttribLocation(name),buf,stride,offset);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,fm::Size N>
	inline fm::Result Shader::setUniform(const std::string &name,const T (&values)[N])
	{
		for (fm::Size i = 0;i<N;++i)
		{
			fm::Result res = setUniform( (name + "[" + fm::toString(i) + "]").str() , values[i] );
			
			if (!res)
				return res;
		}
		
		return fm::Result();
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class>
	inline fm::Result Shader::setAttribute(const std::string &name,const T &val)
	{
		return m_vao.setAttribute<T>(getAttribLocation(name),val);
	}
}

#endif // FRONTIER_SHADER_INL_INCLUDED


