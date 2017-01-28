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
#ifndef FRONTIER_FUNCDELEGATE_INL_INCLUDED
#define FRONTIER_FUNCDELEGATE_INL_INCLUDED

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class AltR,class FuncArgList,class R,class... Args>
	inline FuncDelegate<AltR,FuncArgList,R,Args...>::FuncDelegate(typename FuncDelegate<AltR,FuncArgList,R,Args...>::FuncType func) : m_pointer(func)
	{
		
	}
	
	namespace priv
	{
		template<class R,class... Args>
		class FuncCaller
		{
		public:
			template<class... ExtraArgs>
			static inline R call(R (*funcPtr)(Args...),Args... args,ExtraArgs... eargs)
			{
				return funcPtr(args...);
			}
			
		};
		
		template<class AltR,class FuncArgList>
		class FuncCallerDeducer
		{
		public:
			template<class... CallArgs>
			using FuncCallerProt = FuncCaller<AltR,CallArgs...>;
			using type = typename FuncArgList::template Apply<FuncCallerProt>;
		};
	}
	
	/////////////////////////////////////////////////////////////
	template<class AltR,class FuncArgList,class R,class... Args>
	inline R FuncDelegate<AltR,FuncArgList,R,Args...>::call(Args... callArgs) const
	{
		return (R)priv::FuncCallerDeducer<AltR,FuncArgList>::type::call(m_pointer,callArgs...);
	}
	
	/////////////////////////////////////////////////////////////
	template<class AltR,class FuncArgList,class R,class... Args>
	inline BaseDelegate<R,Args...> *FuncDelegate<AltR,FuncArgList,R,Args...>::makeCopy() const
	{
		return new FuncDelegate<AltR,FuncArgList,R,Args...>(m_pointer);
	}
	
	/////////////////////////////////////////////////////////////
	template<class AltR,class FuncArgList,class R,class... Args>
	bool FuncDelegate<AltR,FuncArgList,R,Args...>::isBaseDelegate() const
	{
		return false;
	}
}

#endif // FRONTIER_FUNCDELEGATE_INL_INCLUDED
