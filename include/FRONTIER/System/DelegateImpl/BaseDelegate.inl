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
#ifndef FRONTIER_BASEDELEGATE_INL_INCLUDED
#define FRONTIER_BASEDELEGATE_INL_INCLUDED

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline BaseDelegate<R,Args...>::~BaseDelegate()
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline R BaseDelegate<R,Args...>::call(Args...) const
	{
		return R();
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class... ExtraArgs>
	inline R BaseDelegate<R,Args...>::call(Args... callArgs,ExtraArgs...) const
	{
		return call(callArgs...);
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline BaseDelegate<R,Args...> *BaseDelegate<R,Args...>::makeCopy() const
	{
		return new BaseDelegate<R,Args...>();
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	bool BaseDelegate<R,Args...>::isBaseDelegate() const
	{
		return true;
	}
}

#endif // FRONTIER_BASEDELEGATE_INL_INCLUDED
