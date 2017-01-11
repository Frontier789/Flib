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
#ifndef FRONTIER_SHADER_INL_INCLUDED
#define FRONTIER_SHADER_INL_INCLUDED
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/String.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	inline Shader::TexUniformData::TexUniformData(int location,int slot,int act_id) : location(location),
																					  slot(slot),
																					  act_id(act_id)
	{

	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class>
	inline fm::Result Shader::setAttribPointer(const std::string &name,const T *pointer,unsigned int stride)
	{
		return setAttribPointer(name,1,fg::Is_GLDataType<T>::enumVal,0,pointer,stride);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class,class>
	inline fm::Result Shader::setAttribPointer(const std::string &name,const T *pointer,unsigned int stride)
	{
		return setAttribPointer(name,T::components,fg::Is_GLDataType<typename T::component_type>::enumVal,0,pointer,stride);
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
}

#endif // FRONTIER_SHADER_INL_INCLUDED


