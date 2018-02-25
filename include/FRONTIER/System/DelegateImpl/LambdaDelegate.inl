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
#ifndef FRONTIER_LAMBDADELEGATE_INL_INCLUDED
#define FRONTIER_LAMBDADELEGATE_INL_INCLUDED
#include <FRONTIER/System/CallForwarder.hpp>
#include <type_traits>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class LambdaT,class R,class... Args>
	LambdaDelegate<LambdaT,R,Args...>::LambdaDelegate(LambdaT lambda) : m_lambda(lambda)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class LambdaT,class R,class... Args>
	R LambdaDelegate<LambdaT,R,Args...>::call(Args... callArgs) const
	{
		return fm::CallForwarder<typename std::remove_pointer<LambdaT>::type,R,Args...>::call(m_lambda,callArgs...);
	}
	
	/////////////////////////////////////////////////////////////
	template<class LambdaT,class R,class... Args>
	BaseDelegate<R,Args...> *LambdaDelegate<LambdaT,R,Args...>::makeCopy() const
	{
		return new LambdaDelegate<LambdaT,R,Args...>(m_lambda);
	}
	
	/////////////////////////////////////////////////////////////
	template<class LambdaT,class R,class... Args>
	bool LambdaDelegate<LambdaT,R,Args...>::isBaseDelegate() const
	{
		return false;
	}
}

#endif // FRONTIER_LAMBDADELEGATE_INL_INCLUDED
